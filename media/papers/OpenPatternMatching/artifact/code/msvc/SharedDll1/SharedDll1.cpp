// MathFuncsDll.cpp
// compile with: /EHsc /LD

#include "SharedDll1.hpp"
#include <stdexcept>

//------------------------------------------------------------------------------

template <size_t N>
struct shape_kind_dll : shape_kind<N>
{
    typedef shape_kind<N> base_class;
    shape_kind_dll(size_t n = N) : base_class(n) {}
};

//------------------------------------------------------------------------------

Shape* make_shape_dll(size_t i)
{
    switch (i)
    {
        #define FOR_EACH_MAX  NUMBER_OF_DERIVED-1
        #define FOR_EACH_N(N) case N: return i%4 == 0 ? new shape_kind<N> : new shape_kind_dll<N>;
        #include "loop_over_numbers.hpp"
        #undef  FOR_EACH_N
        #undef  FOR_EACH_MAX
    }
    return 0;
}

//------------------------------------------------------------------------------

using namespace std;

namespace MathFuncs
{
    double MyMathFuncs::Add(double a, double b)
    {
        return a + b;
    }

    double MyMathFuncs::Subtract(double a, double b)
    {
        return a - b;
    }

    double MyMathFuncs::Multiply(double a, double b)
    {
        return a * b;
    }

    double MyMathFuncs::Divide(double a, double b)
    {
        if (b == 0)
        {
            throw new invalid_argument("b cannot be zero!");
        }

        return a / b;
    }
}
