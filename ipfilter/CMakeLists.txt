cmake_minimum_required(VERSION 3.2)

project(ipfilter VERSION 1.0.$ENV{GITHUB_RUN_NUMBER})
# find_package(Boost COMPONENTS unit_test_framework REQUIRED)

add_executable(ipfilter_cli ipfilter.cpp)
# add_library(ipfilter lib.cpp)
# add_executable(test_ipfilter test_ipfilter.cpp)

# set_target_properties(test_ipfilter PROPERTIES
#     COMPILE_DEFINITIONS BOOST_TEST_DYN_LINK
#     INCLUDE_DIRECTORIES ${Boost_INCLUDE_DIR}
# )

# target_link_libraries(test_ipfilter
#     ${Boost_LIBRARIES}
#     ipfilter
# )

if (MSVC)
    target_compile_options(ipfilter_cli PRIVATE
        /W4
    )
#    target_compile_options(ipfilter PRIVATE
#        /W4
#    )
#    target_compile_options(test_ipfilter PRIVATE
#        /W4
#    )
else ()
    target_compile_options(ipfilter_cli PRIVATE
        -Wall -Wextra -pedantic -Werror
    )
#    target_compile_options(ipfilter PRIVATE
#        -Wall -Wextra -pedantic -Werror
#    )
#    target_compile_options(test_ipfilter PRIVATE
#        -Wall -Wextra -pedantic -Werror
#    )
endif()



install(TARGETS ipfilter_cli RUNTIME DESTINATION bin)

set(CPACK_GENERATOR DEB)
set(CPACK_PACKAGE_VERSION_MAJOR "${PROJECT_VERSION_MAJOR}")
set(CPACK_PACKAGE_VERSION_MINOR "${PROJECT_VERSION_MINOR}")
set(CPACK_PACKAGE_VERSION_PATCH "${PROJECT_VERSION_PATCH}")

set(CPACK_PACKAGE_CONTACT example@example.com)

include(CPack)

# enable_testing()

# add_test(test_version test_version)
