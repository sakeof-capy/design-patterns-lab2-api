#include "endpoints/capybara.hpp"
#include "utilities.hpp"
#include "common_json.hpp"

#include <array>

void capybara_endpoints_check_in(httplib::Server& server)
{
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
}
