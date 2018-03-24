/**
 * @file   TestUtils.cpp
 * @brief  Google-test utilities.
 * @author zer0
 * @date   2018-03-24
 */

#include <libtbag/util/TestUtils.hpp>
#include <libtbag/log/Log.hpp>
#include <libtbag/string/Format.hpp>
#include <iostream>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace util      {
namespace test      {
namespace behaviour {

TBAG_CONSTEXPR static char const * const MESSAGE_SCENARIO_NO_TAG_FORMAT = "[@BEHAVIOUR] [SCENARIO] {}";
TBAG_CONSTEXPR static char const * const MESSAGE_GIVEN_NO_TAG_FORMAT    = "[@BEHAVIOUR] [#  GIVEN] {}";
TBAG_CONSTEXPR static char const * const MESSAGE_WHEN_NO_TAG_FORMAT     = "[@BEHAVIOUR] [##  WHEN] {}";
TBAG_CONSTEXPR static char const * const MESSAGE_THEN_NO_TAG_FORMAT     = "[@BEHAVIOUR] [### THEN] {}";

TBAG_CONSTEXPR static char const * const MESSAGE_SCENARIO_FORMAT = "[@BEHAVIOUR] [SCENARIO:{}] {}";
TBAG_CONSTEXPR static char const * const MESSAGE_GIVEN_FORMAT    = "[@BEHAVIOUR] [#  GIVEN:{}] {}";
TBAG_CONSTEXPR static char const * const MESSAGE_WHEN_FORMAT     = "[@BEHAVIOUR] [##  WHEN:{}] {}";
TBAG_CONSTEXPR static char const * const MESSAGE_THEN_FORMAT     = "[@BEHAVIOUR] [### THEN:{}] {}";

Scenario::Scenario(std::string const & name) : Scenario(name, std::string())
{
}

Scenario::Scenario(std::string const & name, std::string const & tag)
        : NAME(name), TAG(tag)
{
    using namespace libtbag::string;
    using namespace std;
    if (TAG.empty()) {
        cout << fformat(MESSAGE_SCENARIO_NO_TAG_FORMAT, name) << endl;
    } else {
        cout << fformat(MESSAGE_SCENARIO_FORMAT, tag, name) << endl;
    }
}

Scenario::~Scenario()
{
    // EMPTY.
}

Given::Given(Scenario & parent, std::string const & name)
        : SCENARIO(parent), NAME(name)
{
    using namespace libtbag::string;
    using namespace std;
    if (SCENARIO.TAG.empty()) {
        cout << fformat(MESSAGE_GIVEN_NO_TAG_FORMAT, name) << endl;
    } else {
        cout << fformat(MESSAGE_GIVEN_FORMAT, SCENARIO.TAG, name) << endl;
    }
}

Given::~Given()
{
    // EMPTY.
}

When::When(Given & parent, std::string const & name)
        : GIVEN(parent), NAME(name)
{
    using namespace libtbag::string;
    using namespace std;
    if (GIVEN.SCENARIO.TAG.empty()) {
        cout << fformat(MESSAGE_WHEN_NO_TAG_FORMAT, name) << endl;
    } else {
        cout << fformat(MESSAGE_WHEN_FORMAT, GIVEN.SCENARIO.TAG, name) << endl;
    }
}

When::~When()
{
    // EMPTY.
}

Then::Then(When const & parent, std::string const & name)
        : WHEN(parent), NAME(name)
{
    using namespace libtbag::string;
    using namespace std;
    if (WHEN.GIVEN.SCENARIO.TAG.empty()) {
        cout << fformat(MESSAGE_THEN_NO_TAG_FORMAT, name) << endl;
    } else {
        cout << fformat(MESSAGE_THEN_FORMAT, WHEN.GIVEN.SCENARIO.TAG, name) << endl;
    }
}

Then::~Then()
{
    // EMPTY.
}

} // namespace behaviour
} // namespace test
} // namespace util

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

