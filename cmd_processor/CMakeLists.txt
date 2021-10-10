cmake_minimum_required(VERSION 3.2)

project(bulk VERSION 0.0.$ENV{GITHUB_RUN_NUMBER})

add_executable(bulk cmd_processor.cpp)
add_library(bulk_lib block.cpp)

set_target_properties(bulk bulk PROPERTIES
    CXX_STANDARD 14
    CXX_STANDARD_REQUIRED ON
)
target_include_directories(cmd_processor
    PRIVATE "${CMAKE_BINARY_DIR}"
)

target_link_libraries(bulk PRIVATE
    bulk_lib
)


if (MSVC)
    target_compile_options(bulk PRIVATE
        /W4
    )
else ()
    target_compile_options(bulk PRIVATE
        -Wall -Wextra -pedantic -Werror
    )
endif()


install(TARGETS bulk RUNTIME DESTINATION bin)

set(CPACK_GENERATOR DEB)
set(CPACK_PACKAGE_VERSION_MAJOR "${PROJECT_VERSION_MAJOR}")
set(CPACK_PACKAGE_VERSION_MINOR "${PROJECT_VERSION_MINOR}")
set(CPACK_PACKAGE_VERSION_PATCH "${PROJECT_VERSION_PATCH}")

set(CPACK_PACKAGE_CONTACT example@example.com)

include(CPack)

