#include "endpoints/habitat.hpp"
#include "utilities.hpp"
#include "common_json.hpp"

#include <array>

void habitat_endpoints_check_in(httplib::Server& server)
{
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
}
