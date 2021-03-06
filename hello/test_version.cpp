#define BOOST_TEST_MODULE test_version

#include <boost/test/unit_test.hpp>

#include "lib.h"
#include "version.h"

BOOST_AUTO_TEST_SUITE(test_version)

BOOST_AUTO_TEST_CASE(test_valid_version)
{
    BOOST_CHECK(version() == PROJECT_VERSION_PATCH);
}

}
