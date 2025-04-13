set(LIBRARY_NAME integration)

add_compile_definitions(SOURCE_ROOT_PATH="${CMAKE_CURRENT_SOURCE_DIR}")

add_executable(${LIBRARY_NAME})

file(GLOB_RECURSE SOURCES bin/main.cpp src/*.cpp)
file(GLOB_RECURSE HEADERS include/*.hpp)

target_sources(${LIBRARY_NAME}
    PRIVATE
        ${SOURCES}
        ${HEADERS}
)

target_link_libraries(${LIBRARY_NAME}
    PRIVATE
        httplib::httplib
)

target_include_directories(${LIBRARY_NAME}
    PUBLIC
        $<BUILD_INTERFACE:
            ${CMAKE_CURRENT_SOURCE_DIR}/include
            ${CMAKE_CURRENT_SOURCE_DIR}/include/${LIBRARY_NAME}
        >
        $<INSTALL_INTERFACE:include/>
)
