add_executable(api bin/main.cpp)
target_link_libraries(api httplib::httplib cpp_dotenv)
