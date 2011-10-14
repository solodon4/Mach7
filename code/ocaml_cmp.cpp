#define XTL_TIMING_METHOD_3

#include <iostream>
#include <iomanip>
#include <vector>
#include "timing.hpp"
#include "match.hpp"

#define FOR_EACH_MAX 99

struct Shape
{
    virtual ~Shape() {}
};

template <int N>
struct shape_kind : Shape
{
};

int do_match(Shape& s)
{
    SWITCH_N(s,FOR_EACH_MAX+1)
    {
    CASES_BEGIN
    #define FOR_EACH_N(N) CASE(shape_kind<N>) return N;
    #include "loop_over_numbers.hpp"
    #undef  FOR_EACH_N
    CASES_END
    }
    //XTL_ASSERT(!"Inexhaustive search");
    return -1;
}

Shape* make_shape(int i)
{
    switch (i)
    {
    #define FOR_EACH_N(N) case N: return new shape_kind<N>;
    #include "loop_over_numbers.hpp"
    #undef  FOR_EACH_N
    }
    return new shape_kind<0>;
}

const int N = 1000000; // The amount of times visitor and matching procedure is invoked in one time measuring
const int M = 101;   // The amount of times time measuring is done
const int K = FOR_EACH_MAX+1;

int main()
{
    std::vector<Shape*> array(N);

    for (int j = 0; j < N; ++j)
        array[j] = make_shape(j%K);

    Shape* s = make_shape(42);

    time_stamp total_time = 0;
    int z = 0;

    for (int i = 0; i < M; ++i)
    {
        time_stamp start = get_time_stamp();

        for (int j = 0; j < N; ++j)
            z = z + do_match(*array[j]);
            //z = z + do_match(*s);

        time_stamp finish = get_time_stamp();
        total_time += finish-start;
    }

    for (int j = 0; j < N; ++j)
        delete array[j];

    std::cout << "\nAverage time for " << N << " runs takes " << std::setprecision(5) << dbl::seconds(total_time)/M << " seconds: " << z << std::endl;
}

#undef  FOR_EACH_MAX
