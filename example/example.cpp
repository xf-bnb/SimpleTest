#include "../include/xf_simple_test.h"

namespace xf::example
{
    _xfTest(test_1)
    {
        _xfExpect(1 == 2);  // error
        _xfExpect(1 < 2);   // ok
        _xfExpect(1 > 2);   // error
    }

    _xfTest(test_2)
    {
        _xfAssert(1 == 2);  // error
        _xfAssert(1 < 2);   // ignore
        _xfAssert(1 > 2);   // ignore
    }

}

_xfTest(test_3)
{
    int a = 4, b = 2;

    _xfExpect(a > b);       // ok
    _xfExpect(a < b);       // error
    _xfExpect(2 == a + b);  // error
    _xfExpect(2 == a - b);  // ok
    _xfExpect(2 == a / b);  // ok
    _xfAssert(2 == a * b);  // error
    _xfExpect(2 == a % b);  // ignore
}

int main()
{
    return xf::test::Test();
    // return xf::test::Test({ "test_2", "test_3" });
}
