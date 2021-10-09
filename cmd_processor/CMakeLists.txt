cmake_minimum_required(VERSION 3.2)

project(bulk VERSION 0.0.$ENV{GITHUB_RUN_NUMBER})

add_executable(bulk_cli cmd_processor.cpp)
# add_library(bulk block.cpp)

if (MSVC)
    target_compile_options(bulk_cli PRIVATE
        /W4
    )
else ()
    target_compile_options(bulk_cli PRIVATE
        -Wall -Wextra -pedantic -Werror
    )
endif()


install(TARGETS bulk_cli RUNTIME DESTINATION bin)

set(CPACK_GENERATOR DEB)
set(CPACK_PACKAGE_VERSION_MAJOR "${PROJECT_VERSION_MAJOR}")
set(CPACK_PACKAGE_VERSION_MINOR "${PROJECT_VERSION_MINOR}")
set(CPACK_PACKAGE_VERSION_PATCH "${PROJECT_VERSION_PATCH}")

set(CPACK_PACKAGE_CONTACT example@example.com)

include(CPack)

