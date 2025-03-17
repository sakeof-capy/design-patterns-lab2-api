#include <iostream>
#include <httplib.h>
#include <dotenv.h>
#include <optional>

#include "environ.h"

template<typename EnvType>
EnvType resolve_env(const std::string_view env_name)
{
    const auto [port_is_read, port_str] = dotenv::getenv(env_name.data());

    if (!port_is_read)
    {
        std::cerr << "Env-variable with name " << env_name << " is missing. Add one to the .env\n" << std::flush;
        std::exit(-1);
    }

    std::stringstream ss(port_str);
    EnvType result;
    ss >> result;

    if (ss.fail())
    {
        std::cerr
            << "Env-variable with name " << env_name
            << " does not match type " << typeid(EnvType).name()
            << '\n' << std::flush;
        std::exit(-1);
    }

    return result;
}

int main()
{
    try
    {
        dotenv::env.load_dotenv();
    }
    catch (const std::invalid_argument& err)
    {
        std::cerr << ".env file missing. Create one: $ cp env-template .env\n" << std::flush;
        return EXIT_FAILURE;
    }

    httplib::Server server;
    const int port = resolve_env<int>("SERVER_PORT");

    std::cout << "Server's listening on port " << port << '\n' << std::flush;

    server.Get("/hi", [](const httplib::Request&, httplib::Response& res) {
        res.set_content("Hello World!", "text/plain");
    });

    server.Get("/", [](const httplib::Request&, httplib::Response& res) {
        res.set_content("Root!", "text/plain");
    });

    server.listen("0.0.0.0", 8080);

    return EXIT_SUCCESS;
}
