#include <iostream>
#include <string_view>
#include <optional>
#include <format>
#include "retry.hpp"

#define PRINT_BODY
#undef PRINT_BODY

struct Request {
    std::string description;
    std::function<std::optional<httplib::Result>(httplib::Client&)> request;
    std::size_t repeat_times { 5 };

public:
    void log(const std::size_t attempt, const std::string_view message) const
    {
        std::cout << "#";

        if (attempt != 0) {
            std::cout << attempt;
        }

        std::cout << ' ' << this->description << " | " << message << '\n';
    }
};

static const std::array REQUESTS {
    Request {
        .description = "GET /concerts/",
        .request = [](httplib::Client& client) {
            return Retry([&client] { return client.Get("/concerts/"); }).send();
        }
    },
    Request {
        .description = "POST /concerts/ to create concert",
        .request = [](httplib::Client& client) {
            const httplib::Headers headers = {
                {"Content-Type", "application/json"}
            };

            const std::string body = R"({
                "id": 1,
                "title": "Title",
                "location": "Location",
                "date": "2025-01-01",
                "current_participant_count": 100,
                "status": "scheduled"
            })";

            return client.Post("/concerts/", headers, body, "application/json");
        }
    },
    Request {
        .description = "GET /concerts/1",
        .request = [](httplib::Client& client) {
            return Retry([&client] { return client.Get("/concerts/1"); }).send();
        }
    },
    Request {
        .description = "PUT /concerts/1",
        .request = [](httplib::Client& client) {
            const httplib::Headers headers = {
                {"Content-Type", "application/json"}
            };

            const std::string body = R"({
                "id": 1,
                "title": "Updated Title",
                "location": "Updated Location",
                "date": "2025-02-01",
                "current_participant_count": 150,
                "status": "scheduled"
            })";

            return Retry([&client, &headers, &body] {
                return client.Put("/concerts/1", headers, body, "application/json");
            }).send();
        }
    },
    Request {
        .description = "DELETE /concerts/1",
        .request = [](httplib::Client& client) {
            return Retry([&client] { return client.Delete("/concerts/1"); }).send();
        }
    },
    Request {
        .description = "PATCH /concerts/1/validate_tickets",
        .request = [](httplib::Client& client) {
            httplib::Headers headers = {
                {"Content-Type", "application/json"}
            };
            std::string body = R"({
                "id": 123,
                "concert_id": 1,
                "validation_token": "AABBCC123",
                "date": "2025-01-01",
                "status": "unused"
            })";
            return client.Patch("/concerts/1/validate_tickets", headers, body, "application/json");
        }
    },
    Request {
        .description = "GET /tickets/",
        .request = [](httplib::Client& client) {
            return Retry([&client] { return client.Get("/tickets/"); }).send();
        }
    },
    Request {
        .description = "POST /tickets/",
        .request = [](httplib::Client& client) {
            const std::string body = R"({
                "id": 123,
                "concert_id": 1,
                "validation_token": "AABBCC123",
                "date": "2025-01-01",
                "status": "unused"
            })";
            return client.Post("/tickets/", body, "application/json");
        }
    },
    Request {
        .description = "GET /tickets/123",
        .request = [](httplib::Client& client) {
            return Retry([&client] { return client.Get("/tickets/123"); }).send();
        }
    },
    Request {
        .description = "PATCH /tickets/123 [retry]",
        .request = [](httplib::Client& client) {
            return client.Patch("/tickets/123");
        }
    },
    Request {
        .description = "PUT /tickets/123",
        .request = [](httplib::Client& client) {
            const httplib::Headers headers = {
                {"Content-Type", "application/json"}
            };

            const std::string body = R"({
                "id": 123,
                "concert_id": 1,
                "validation_token": "AABBCC456",
                "date": "2025-01-01",
                "status": "used"
            })";

            return Retry([&client, &body, &headers] {
                return client.Put("/tickets/123", headers, body, "application/json");
            }).send();
        }
    },
    Request {
        .description = "DELETE /tickets/123",
        .request = [](httplib::Client& client) {
            return Retry([&client] { return client.Delete("/tickets/123"); }).send();
        }
    },
};

void handle_single_request(const std::size_t request_attempt, httplib::Client& client, const Request& request)
{
    std::optional<httplib::Result> maybe_result = request.request(client);

    if (!maybe_result.has_value())
    {
        request.log(request_attempt, "No responce received.");
        return;
    }

    httplib::Result& result = maybe_result.value();

    if (!result || result->status >= 300)
    {
#ifdef PRINT_BODY
        request.log(request_attempt, std::format(
            "\n\tRequest failed with status: {}. \n\tError message: {}. \n\tBody: {}",
            result->status,
            httplib::to_string(result.error()),
            result->body
        ));
#else
        request.log(request_attempt, std::format(
            "\n\tRequest failed with status: {}. \n\tError message: {}.",
            result->status,
            httplib::to_string(result.error())
        ));
#endif

        return;
    }

#ifdef PRINT_BODY
    request.log(request_attempt, std::format(
        "\n\tRequest succeeded with status: {}. \n\tBody: {}",
        result->status,
        result->body
    ));
#else
    request.log(request_attempt, std::format(
        "\n\tRequest succeeded with status: {}.",
        result->status
    ));
#endif
}

int main()
{
    httplib::Client client("http://external_api:8888");

    for (const Request& request : REQUESTS)
    {
        for (std::size_t request_attempt = 0; request_attempt < request.repeat_times; ++request_attempt)
        {
            handle_single_request(request_attempt, client, request);
        }

        std::this_thread::sleep_for(std::chrono::milliseconds(500));
        std::cout << "--------------------------------------------------------------\n" << std::flush;
    }

    return EXIT_SUCCESS;
}
