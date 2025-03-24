set(LIBRARY_NAME api)
add_executable(${LIBRARY_NAME} bin/main.cpp)

target_link_libraries(${LIBRARY_NAME}
    PRIVATE
        httplib::httplib
        cpp_dotenv
        nlohmann_json::nlohmann_json
)

target_include_directories(${LIBRARY_NAME}
    PUBLIC
        $<BUILD_INTERFACE:
            ${CMAKE_CURRENT_SOURCE_DIR}/include
            ${CMAKE_CURRENT_SOURCE_DIR}/include/${LIBRARY_NAME}
        >
        $<INSTALL_INTERFACE:include/>
)
