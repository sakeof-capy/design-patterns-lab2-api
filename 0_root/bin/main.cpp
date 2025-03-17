#include <iostream>
#include <httplib.h>
#include <filesystem>

int main()
{
    httplib::Server server;

    std::cout << "Server's just started\n" << std::flush;

    server.Get("/hi", [](const httplib::Request&, httplib::Response& res) {
        res.set_content("Hello World!", "text/plain");
    });

    server.Get("/", [](const httplib::Request&, httplib::Response& res) {
        res.set_content("Root!", "text/plain");
    });

    server.listen("0.0.0.0", 8080);

    return EXIT_SUCCESS;
}
