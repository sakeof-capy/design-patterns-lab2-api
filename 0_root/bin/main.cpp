#include <iostream>
#include <httplib.h>
#include <string_view>
#include <array>
#include <format>
#include <span>
#include "nlohmann/json.hpp"

#include "env.hpp"

namespace json = nlohmann;

static constexpr std::string_view SINGLE_CAPY_JSON = R"({
    "id": 1,
    "name": "Capy McCapface",
    "age": 4,
    "friends": [2, 3],
    "habitatId": 101
})";

static constexpr std::string_view CAPY_ARRAY_JSON = R"([
  {
    "id": 1,
    "name": "Capy McCapface",
    "age": 4,
    "friends": [2, 3],
    "habitatId": 101
  },
  {
    "id": 2,
    "name": "Hydrochoerus",
    "age": 6,
    "friends": [1, 3],
    "habitatId": 102
  },
    {
    "id": 3,
    "name": "Gort",
    "age": 2,
    "friends": [1, 2],
    "habitatId": 101
  }
])";

static constexpr std::string_view SINGLE_HABITAT_JSON = R"({
    "id": 0,
    "name": "Lake",
    "location": "Big Mountain"
})";

static constexpr std::string_view HABITAT_ARRAY_JSON = R"([
    {
        "id": 0,
        "name": "Lake",
        "location": "Big Mountain"
    },
    {
        "id": 1,
        "name": "Hut",
        "location": "Small Mountain"
    },
])";

int next_status(const std::span<const int> statuses)
{
    if (statuses.empty()) 
    {
        std::cout << "Panic: empty statuses list\n" << std::flush;
        std::terminate();
    }

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> distrib(0, statuses.size() - 1);

    const int randomIndex = distrib(gen);
    return statuses[randomIndex];
}

struct Visitor {
    std::string name;
    int id;
    int age;
};

void to_json(json::json& j, const Visitor& p) {
    j = json::json{
        {"id", p.id},
        {"name", p.name},
        {"age", p.age}
    };
}

std::vector<Visitor> generate_visitors()
{
    std::vector<Visitor> visitors;
    int num_visitors = rand() % 10 + 1;
    for (int i = 0; i < num_visitors; ++i) {
        visitors.push_back(Visitor {
            .id = i,
            .name = std::format("Capy McCapface{}", i),
            .age = i * 10 + 22,
        });
    }
    return visitors;
}

[[noreturn]] void stream_sse(const httplib::Request&, httplib::Response& res) {
    res.set_content_provider("text/event-stream", [&](size_t offset, httplib::DataSink& sink) {
        res.set_header("Content-Type", "text/event-stream");
        res.set_header("Cache-Control", "no-cache");
        res.set_header("Connection", "keep-alive");

        while (true) {
            std::vector<Visitor> visitors = generate_visitors();

            json::json visitors_json = json::json::array();
            for (const Visitor& visitor : visitors) {
                visitors_json.push_back(visitor);
            }

            std::string message = "data: " + visitors_json.dump() + "\n\n";
            sink.write(message.data(), message.size());

            std::this_thread::sleep_for(std::chrono::seconds(3));
        }

        return true;
    });
}

int main()
{
    httplib::Server server;
    const int port = resolve_env<int>("SERVER_PORT");

    std::cout << "Server's listening on port " << port << '\n' << std::flush;

    server.Get("/sse", stream_sse);

    server.Get("/capybaras", [](const httplib::Request& req, httplib::Response& res) {
        // GET /capybaras
        if (!req.has_param("id")) 
        {
            const int success_status = 200;
            const std::array status_codes = { 200, 500 };

            res.status = next_status(status_codes);

            if (res.status == success_status) 
            {
                res.set_content(std::string { CAPY_ARRAY_JSON }, "application/json");
            }
        }
        // GET /capybaras/{id}
        else 
        {
            const int success_status = 200;
            const std::array status_codes = { 200, 404, 500 };

            res.status = next_status(status_codes);

            if (res.status == success_status) 
            {
                res.set_content(std::string { SINGLE_CAPY_JSON }, "application/json");
            }
        }
    });

    // POST /capybaras
    server.Post("/capybaras", [](const httplib::Request&, httplib::Response& res) {
        const int success_status = 201;
        const std::array status_codes = { 201, 400, 500 };
        
        res.status = next_status(status_codes);

        if (res.status == success_status) 
        {
            res.set_content(std::string { SINGLE_CAPY_JSON }, "application/json");
        }
    });

    // PUT /capybaras/{id}
    server.Put("/capybaras", [](const httplib::Request& req, httplib::Response& res) {
        if (req.has_param("id"))  
        {
            const int success_status = 200;
            const std::array status_codes = { 200, 400, 404, 500 };

            res.status = next_status(status_codes);

            if (res.status == success_status) 
            {
                res.set_content(std::string { SINGLE_CAPY_JSON }, "application/json");
            }
        }
        else 
        {
            res.status = 404;
        }
    });

    // PATCH /capybaras/{id}
    server.Patch("/capybaras", [](const httplib::Request& req, httplib::Response& res) {
        if (req.has_param("id"))  
        {
            const int success_status = 200;
            const std::array status_codes = { 200, 400, 404, 500 };

            res.status = next_status(status_codes);

            if (res.status == success_status) 
            {
                res.set_content(std::string { SINGLE_CAPY_JSON }, "application/json");
            }
        }
        else 
        {
            res.status = 404;
        }
    });

    // DELETE /capybaras/{id}
    server.Delete("/capybaras", [](const httplib::Request& req, httplib::Response& res) {
        if (req.has_param("id"))  
        {
            const int success_status = 204;
            const std::array status_codes = { 204, 404, 500 };

            res.status = next_status(status_codes);

            if (res.status == success_status) 
            {
                res.set_content(std::string { SINGLE_CAPY_JSON }, "application/json");
            }
        }
        else 
        {
            res.status = 404;
        }
    });

    // GET /capybaras/{id}
    server.Delete("/capybaras", [](const httplib::Request& req, httplib::Response& res) {
        if (req.has_param("id"))  
        {
            const std::array status_codes = { 204, 404, 500 };
            res.status = next_status(status_codes);
        }
        else 
        {
            res.status = 404;
        }
    });

    // GET /capybaras/{id}/friends
    server.Get(R"(/capybaras/(\d+)/friends)", [&](const httplib::Request& req, httplib::Response& res) {
        const int success_status = 200;
        const std::array status_codes = { 200, 404, 500 };

        res.status = next_status(status_codes);

        if (res.status == success_status) 
        {
            res.set_content(std::string { CAPY_ARRAY_JSON }, "application/json");
        }
    });

    // GET /capybaras/{id}/habitat
    server.Get(R"(/capybaras/(\d+)/habitat)", [&](const httplib::Request& req, httplib::Response& res) {
        const int success_status = 200;
        const std::array status_codes = { 200, 404, 500 };

        res.status = next_status(status_codes);

        if (res.status == success_status) 
        {
            res.set_content(std::string { SINGLE_HABITAT_JSON }, "application/json");
        }
    });
    
    // *** Habitats ***

    server.Get("/habitats", [&](const httplib::Request& req, httplib::Response& res) {
        // GET /capybaras/{id}
        if (req.has_param("id"))  
        {
            const int success_status = 200;
            const std::array status_codes = { 200, 404, 500 };

            res.status = next_status(status_codes);

            if (res.status == success_status) 
            {
                res.set_content(std::string { SINGLE_CAPY_JSON }, "application/json");
            }
        }
        // GET /habitats 
        else 
        {
            const int success_status = 200;
            const std::array status_codes = { 200, 500 };

            res.status = next_status(status_codes);

            if (res.status == success_status) 
            {
                res.set_content(std::string { HABITAT_ARRAY_JSON }, "application/json");
            }
        }
    });

    // POST /habitats
    server.Post("/habitats", [](const httplib::Request& req, httplib::Response& res) {
        if (req.has_param("id"))  
        {
            const int success_status = 201;
            const std::array status_codes = { 201, 400, 500 };

            res.status = next_status(status_codes);

            if (res.status == success_status) 
            {
                res.set_content(std::string { SINGLE_HABITAT_JSON }, "application/json");
            }
        }
        else 
        {
            res.status = 404;
        }
    });

    // PUT /capybaras/{id}
    server.Put("/habitats", [](const httplib::Request& req, httplib::Response& res) {
        if (req.has_param("id"))  
        {
            const int success_status = 200;
            const std::array status_codes = { 200, 400, 404, 500 };

            res.status = next_status(status_codes);

            if (res.status == success_status) 
            {
                res.set_content(std::string { SINGLE_CAPY_JSON }, "application/json");
            }
        }
        else 
        {
            res.status = 404;
        }
    });

    // DELETE /capybaras/{id}
    server.Delete("/habitats", [](const httplib::Request& req, httplib::Response& res) {
        if (req.has_param("id"))  
        {
            const std::array status_codes = { 204, 404, 500 };
            res.status = next_status(status_codes);
        }
        else 
        {
            res.status = 404;
        }
    });

    server.listen("0.0.0.0", port);

    return EXIT_SUCCESS;
}
