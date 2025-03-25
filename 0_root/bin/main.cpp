#include <iostream>
#include <httplib.h>
#include <filesystem>

#include "env.hpp"
#include "endpoints/capybara.hpp"
#include "endpoints/habitat.hpp"
#include "endpoints/visitor.hpp"

namespace fs = std::filesystem;

static const fs::path ROOT_PATH = SOURCE_ROOT_PATH;
static const fs::path ROOT_MOUNT_POINT_PATH = ROOT_PATH / "public";

int main()
{
    httplib::Server server;
    server.set_mount_point("/", ROOT_MOUNT_POINT_PATH.string());
    const int port = resolve_env<int>("SERVER_PORT");

    std::cout << "Server's listening on port " << port << '\n' << std::flush;

    capybara_endpoints_check_in(server);
    habitat_endpoints_check_in(server);
    visitor_endpoints_check_in(server);

    server.listen("0.0.0.0", port);

    return EXIT_SUCCESS;
}
