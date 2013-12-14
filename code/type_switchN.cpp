#include <iostream>
#include <utility>
#include "type_switchN.hpp"

//------------------------------------------------------------------------------

typedef std::pair<double,double> loc;

//------------------------------------------------------------------------------

// An Algebraic Data Type implemented through inheritance
struct Shape
{
    virtual ~Shape() {}
};

//------------------------------------------------------------------------------

struct Circle : Shape
{
    Circle(const loc& c, const double& r) : center(c), radius(r) {}

    const loc& get_center() const { return center; }

    loc    center;
    double radius;
};

//------------------------------------------------------------------------------

struct Square : Shape
{
    Square(const loc& c, const double& s) : upper_left(c), side(s) {}

    loc    upper_left;
    double side;
};

//------------------------------------------------------------------------------

struct Triangle : Shape
{
    Triangle(const loc& a, const loc& b, const loc& c) : first(a), second(b), third(c) {}

    loc first;
    loc second;
    loc third;
};

//------------------------------------------------------------------------------

void do_match(const Shape* s0)
{
    const char* text = "unknown";

    Match(s0)
    {
    Case(Circle)   text = "C"; break;
    Case(Square)   text = "S"; break;
    Case(Triangle) text = "T"; break;
    Otherwise()    text = "other"; break;
    }
    EndMatch

    std::cout << text << std::endl;
}

//------------------------------------------------------------------------------
void do_match(const Shape* s0, const Shape* s1)
{
    const char* text = "unknown";
    {
        struct match_uid_type 
        {
        }
        ;
        enum 
        {
            is_inside_case_clause = 0, number_of_subjects = 2 
        }
        ;
        enum 
        {
            __base_counter = 6 
        }
        ;
        auto&& subject_ref0 = s0;
        auto const subject_ptr0 = addr(subject_ref0);
        typedef  underlying<decltype(*subject_ptr0)>::type source_type0;
        typedef source_type0 target_type0;
        enum 
        {
            target_layout0 = default_layout 
        }
        ;
        ;
        auto const match0 = subject_ptr0;
        (void)match0;
        ;
        static_assert(std::is_polymorphic<source_type0>::value, "Type of subject " "0" " should be polymorphic when you use Match");
        register const void* __casted_ptr0 = 0;
        auto&& subject_ref1 = s1;
        auto const subject_ptr1 = addr(subject_ref1);
        typedef  underlying<decltype(*subject_ptr1)>::type source_type1;
        typedef source_type1 target_type1;
        enum 
        {
            target_layout1 = default_layout 
        }
        ;
        ;
        auto const match1 = subject_ptr1;
        (void)match1;
        ;
        static_assert(std::is_polymorphic<source_type1>::value, "Type of subject " "1" " should be polymorphic when you use Match");
        register const void* __casted_ptr1 = 0;
        typedef vtbl_map<2,type_switch_info<2>> vtbl_map_type;
        vtbl_map_type& __vtbl2case_map = preallocated<vtbl_map_type,match_uid_type>::value;
        type_switch_info<2>& __switch_info = __vtbl2case_map.get(subject_ptr0 , subject_ptr1);
        switch (__switch_info.target) 
        {
        default:
            {
                {
                }
                if ((__casted_ptr0 = dynamic_cast<const Circle*>(subject_ptr0)) != 0) 
                if ((__casted_ptr1 = dynamic_cast<const Circle*>(subject_ptr1)) != 0) 
                {
                    static_assert(number_of_subjects == 2, "Number of targets in the case clause must be the same as the number of subjects in the Match statement");
                    enum 
                    {
                        target_label = 7-__base_counter, is_inside_case_clause = 1 
                    }
                    ;
                    if ((__switch_info.target == 0)) 
                    {
                        __switch_info.target = target_label;
                        __switch_info.offset[0] = intptr_t(__casted_ptr0)-intptr_t(subject_ptr0);
                        __switch_info.offset[1] = intptr_t(__casted_ptr1)-intptr_t(subject_ptr1);
                    }
        case target_label:
            auto& match0 = *adjust_ptr<Circle>(subject_ptr0,__switch_info.offset[0]);
            (void)match0;
            auto& match1 = *adjust_ptr<Circle>(subject_ptr1,__switch_info.offset[1]);
            (void)match1;
            text = "C,C";
            break;
                }
                if ((__casted_ptr0 = dynamic_cast<const Circle*>(subject_ptr0)) != 0) 
                if ((__casted_ptr1 = dynamic_cast<const Square*>(subject_ptr1)) != 0) 
                {
                    static_assert(number_of_subjects == 2, "Number of targets in the case clause must be the same as the number of subjects in the Match statement");
                    enum 
                    {
                        target_label = 8-__base_counter, is_inside_case_clause = 1 
                    }
                    ;
                    if ((__switch_info.target == 0)) 
                    {
                        __switch_info.target = target_label;
                        __switch_info.offset[0] = intptr_t(__casted_ptr0)-intptr_t(subject_ptr0);
                        __switch_info.offset[1] = intptr_t(__casted_ptr1)-intptr_t(subject_ptr1);
                    }
        case target_label:
            auto& match0 = *adjust_ptr<Circle>(subject_ptr0,__switch_info.offset[0]);
            (void)match0;
            auto& match1 = *adjust_ptr<Square>(subject_ptr1,__switch_info.offset[1]);
            (void)match1;
            text = "C,S";
            break;
                }
                if ((__casted_ptr0 = dynamic_cast<const Square*>(subject_ptr0)) != 0) 
                if ((__casted_ptr1 = dynamic_cast<const Circle*>(subject_ptr1)) != 0) 
                {
                    static_assert(number_of_subjects == 2, "Number of targets in the case clause must be the same as the number of subjects in the Match statement");
                    enum 
                    {
                        target_label = 9-__base_counter, is_inside_case_clause = 1 
                    }
                    ;
                    if ((__switch_info.target == 0)) 
                    {
                        __switch_info.target = target_label;
                        __switch_info.offset[0] = intptr_t(__casted_ptr0)-intptr_t(subject_ptr0);
                        __switch_info.offset[1] = intptr_t(__casted_ptr1)-intptr_t(subject_ptr1);
                    }
        case target_label:
            auto& match0 = *adjust_ptr<Square>(subject_ptr0,__switch_info.offset[0]);
            (void)match0;
            auto& match1 = *adjust_ptr<Circle>(subject_ptr1,__switch_info.offset[1]);
            (void)match1;
            text = "S,C";
            break;
                }
                if ((__casted_ptr0 = dynamic_cast<const Square*>(subject_ptr0)) != 0)
                if ((__casted_ptr1 = dynamic_cast<const Square*>(subject_ptr1)) != 0) 
                {
                    static_assert(number_of_subjects == 2, "Number of targets in the case clause must be the same as the number of subjects in the Match statement");
                    enum 
                    {
                        target_label = 10-__base_counter, is_inside_case_clause = 1 
                    }
                    ;
                    if ((__switch_info.target == 0)) 
                    {
                        __switch_info.target = target_label;
                        __switch_info.offset[0] = intptr_t(__casted_ptr0)-intptr_t(subject_ptr0);
                        __switch_info.offset[1] = intptr_t(__casted_ptr1)-intptr_t(subject_ptr1);
                    }
        case target_label:
            auto& match0 = *adjust_ptr<Square>(subject_ptr0,__switch_info.offset[0]);
            (void)match0;
            auto& match1 = *adjust_ptr<Square>(subject_ptr1,__switch_info.offset[1]);
            (void)match1;
            text = "S,S";
            break;
                }
                if ((__casted_ptr0 = dynamic_cast<const Square*>(subject_ptr0)) != 0)
                if ((__casted_ptr1 = dynamic_cast<const Triangle*>(subject_ptr1)) != 0) 
                {
                    static_assert(number_of_subjects == 2, "Number of targets in the case clause must be the same as the number of subjects in the Match statement");
                    enum 
                    {
                        target_label = 11-__base_counter, is_inside_case_clause = 1 
                    }
                    ;
                    if ((__switch_info.target == 0)) 
                    {
                        __switch_info.target = target_label;
                        __switch_info.offset[0] = intptr_t(__casted_ptr0)-intptr_t(subject_ptr0);
                        __switch_info.offset[1] = intptr_t(__casted_ptr1)-intptr_t(subject_ptr1);
                    }
        case target_label:
            auto& match0 = *adjust_ptr<Square>(subject_ptr0,__switch_info.offset[0]);
            (void)match0;
            auto& match1 = *adjust_ptr<Triangle>(subject_ptr1,__switch_info.offset[1]);
            (void)match1;
            text = "S,T";
            break;
                }
                if ((__casted_ptr0 = dynamic_cast<const Triangle*>(subject_ptr0)) != 0)
                if ((__casted_ptr1 = dynamic_cast<const Square*>(subject_ptr1)) != 0)
                {
                    static_assert(number_of_subjects == 2, "Number of targets in the case clause must be the same as the number of subjects in the Match statement");
                    enum 
                    {
                        target_label = 12-__base_counter, is_inside_case_clause = 1 
                    }
                    ;
                    if ((__switch_info.target == 0)) 
                    {
                        __switch_info.target = target_label;
                        __switch_info.offset[0] = intptr_t(__casted_ptr0)-intptr_t(subject_ptr0);
                        __switch_info.offset[1] = intptr_t(__casted_ptr1)-intptr_t(subject_ptr1);
                    }
        case target_label:
            auto& match0 = *adjust_ptr<Triangle>(subject_ptr0,__switch_info.offset[0]);
            (void)match0;
            auto& match1 = *adjust_ptr<Square>(subject_ptr1,__switch_info.offset[1]);
            (void)match1;
            text = "T,S";
            break;
                }
                if ((__casted_ptr0 = dynamic_cast<const Triangle*>(subject_ptr0)) != 0)
                if ((__casted_ptr1 = dynamic_cast<const Triangle*>(subject_ptr1)) != 0) 
                {
                    static_assert(number_of_subjects == 2, "Number of targets in the case clause must be the same as the number of subjects in the Match statement");
                    enum 
                    {
                        target_label = 13-__base_counter, is_inside_case_clause = 1 
                    }
                    ;
                    if ((__switch_info.target == 0)) 
                    {
                        __switch_info.target = target_label;
                        __switch_info.offset[0] = intptr_t(__casted_ptr0)-intptr_t(subject_ptr0);
                        __switch_info.offset[1] = intptr_t(__casted_ptr1)-intptr_t(subject_ptr1);
                    }
        case target_label:
            auto& match0 = *adjust_ptr<Triangle>(subject_ptr0,__switch_info.offset[0]);
            (void)match0;
            auto& match1 = *adjust_ptr<Triangle>(subject_ptr1,__switch_info.offset[1]);
            (void)match1;
            text = "T,T";
            break;
                }
                if ((__casted_ptr0 = dynamic_cast<const Triangle*>(subject_ptr0)) != 0)
                if ((__casted_ptr1 = dynamic_cast<const Circle*>(subject_ptr1)) != 0) 
                {
                    static_assert(number_of_subjects == 2, "Number of targets in the case clause must be the same as the number of subjects in the Match statement");
                    enum 
                    {
                        target_label = 14-__base_counter, is_inside_case_clause = 1 
                    }
                    ;
                    if ((__switch_info.target == 0)) 
                    {
                        __switch_info.target = target_label;
                        __switch_info.offset[0] = intptr_t(__casted_ptr0)-intptr_t(subject_ptr0);
                        __switch_info.offset[1] = intptr_t(__casted_ptr1)-intptr_t(subject_ptr1);
                    }
        case target_label:
            auto& match0 = *adjust_ptr<Triangle>(subject_ptr0,__switch_info.offset[0]);
            (void)match0;
            auto& match1 = *adjust_ptr<Circle>(subject_ptr1,__switch_info.offset[1]);
            (void)match1;
            text = "T,C";
            break;
                }
                if ((__casted_ptr0 = dynamic_cast<const Circle*>(subject_ptr0)) != 0)
                if ((__casted_ptr1 = dynamic_cast<const Triangle*>(subject_ptr1)) != 0) 
                {
                    static_assert(number_of_subjects == 2, "Number of targets in the case clause must be the same as the number of subjects in the Match statement");
                    enum 
                    {
                        target_label = 15-__base_counter, is_inside_case_clause = 1 
                    }
                    ;
                    if ((__switch_info.target == 0)) 
                    {
                        __switch_info.target = target_label;
                        __switch_info.offset[0] = intptr_t(__casted_ptr0)-intptr_t(subject_ptr0);
                        __switch_info.offset[1] = intptr_t(__casted_ptr1)-intptr_t(subject_ptr1);
                    }
        case target_label:
            auto& match0 = *adjust_ptr<Circle>(subject_ptr0,__switch_info.offset[0]);
            (void)match0;
            auto& match1 = *adjust_ptr<Triangle>(subject_ptr1,__switch_info.offset[1]);
            (void)match1;
            text = "C,T";
            break;
            static_assert(is_inside_case_clause, "Otherwise() must follow actual clauses! If you are trying to use it as a default sub-clause, use When() instead");
                }
                {
                    enum 
                    {
                        target_label = 16-__base_counter, is_inside_case_clause = 1 
                    }
                    ;
                    if ((__switch_info.target == 0)) __switch_info.target = target_label;
        case target_label:
            text = "other";
            break;
                }
            }
            if (((__switch_info.target == 0))) 
            {
                enum 
                {
                    target_label = 17-__base_counter 
                }
                ;
                __switch_info.target = target_label;
        case target_label:
            ;
            }
        }
    }
    std::cout << text << std::endl;
}

//void do_match(const Shape* s0, const Shape* s1)
//{
//    const char* text = "unknown";
//
//    Match(s0,s1)
//    {
//    Case(Circle  , Circle  ) text = "C,C"; break;
//    Case(Circle  , Square  ) text = "C,S"; break;
//    Case(Square  , Circle  ) text = "S,C"; break;
//    Case(Square  , Square  ) text = "S,S"; break;
//    Case(Square  , Triangle) text = "S,T"; break;
//    Case(Triangle, Square  ) text = "T,S"; break;
//    Case(Triangle, Triangle) text = "T,T"; break;
//    Case(Triangle, Circle  ) text = "T,C"; break;
//    Case(Circle  , Triangle) text = "C,T"; break;
//    Otherwise()              text = "other"; break;
//    }
//    EndMatch
//
//    std::cout << text << std::endl;
//}

//------------------------------------------------------------------------------

void do_match(const Shape* s0, const Shape* s1, const Shape* s2)
{
    const char* text = "unknown";

    Match(s0,s1,s2)
    {
    Case(Circle,   Circle  , Circle  ) text = "C,C,C"; break;
    Case(Circle,   Circle  , Square  ) text = "C,C,S"; break;
    Case(Circle,   Square  , Circle  ) text = "C,S,C"; break;
    Case(Circle,   Square  , Square  ) text = "C,S,S"; break;
    Case(Circle,   Square  , Triangle) text = "C,S,T"; break;
    Case(Circle,   Triangle, Square  ) text = "C,T,S"; break;
    Case(Circle,   Triangle, Triangle) text = "C,T,T"; break;
    Case(Circle,   Triangle, Circle  ) text = "C,T,C"; break;
    Case(Circle,   Circle  , Triangle) text = "C,C,T"; break;

  //Case(Square,   Circle  , Circle  ) text = "S,C,C"; break;
  //Case(Square,   Circle  , Square  ) text = "S,C,S"; break;
    Case(Square,   Square  , Circle  ) text = "S,S,C"; break;
    Case(Square,   Square  , Square  ) text = "S,S,S"; break;
    Case(Square,   Square  , Triangle) text = "S,S,T"; break;
    Case(Square,   Triangle, Square  ) text = "S,T,S"; break;
    Case(Square,   Triangle, Triangle) text = "S,T,T"; break;
    Case(Square,   Triangle, Circle  ) text = "S,T,C"; break;
  //Case(Square,   Circle  , Triangle) text = "S,C,T"; break;

    Case(Triangle, Circle  , Circle  ) text = "T,C,C"; break;
    Case(Triangle, Circle  , Square  ) text = "T,C,S"; break;
    Case(Triangle, Square  , Circle  ) text = "T,S,C"; break;
    Case(Triangle, Square  , Square  ) text = "T,S,S"; break;
    Case(Triangle, Square  , Triangle) text = "T,S,T"; break;
    Case(Triangle, Triangle, Square  ) text = "T,T,S"; break;
    Case(Triangle, Triangle, Triangle) text = "T,T,T"; break;
    Case(Triangle, Triangle, Circle  ) text = "T,T,C"; break;
    Case(Triangle, Circle  , Triangle) text = "T,C,T"; break;

    Otherwise()                        text = "other"; break;
    }
    EndMatch

    std::cout << text << std::endl;
}

//------------------------------------------------------------------------------

void do_match(const Shape* s0, const Shape* s1, const Shape* s2, const Shape* s3)
{
    const char* text = "unknown";

    Match(s0,s1,s2,s3)
    {
    Case(Circle,   Circle,   Circle  , Circle  ) text = "C,C,C,C"; break;
    Case(Circle,   Circle,   Circle  , Square  ) text = "C,C,C,S"; break;
    Case(Circle,   Circle,   Square  , Circle  ) text = "C,C,S,C"; break;
    Case(Circle,   Circle,   Square  , Square  ) text = "C,C,S,S"; break;
    Case(Circle,   Circle,   Square  , Triangle) text = "C,C,S,T"; break;
    Case(Circle,   Circle,   Triangle, Square  ) text = "C,C,T,S"; break;
    Case(Circle,   Circle,   Triangle, Triangle) text = "C,C,T,T"; break;
    Case(Circle,   Circle,   Triangle, Circle  ) text = "C,C,T,C"; break;
    Case(Circle,   Circle,   Circle  , Triangle) text = "C,C,C,T"; break;
                                                           
    Case(Circle,   Square,   Circle  , Circle  ) text = "C,S,C,C"; break;
    Case(Circle,   Square,   Circle  , Square  ) text = "C,S,C,S"; break;
    Case(Circle,   Square,   Square  , Circle  ) text = "C,S,S,C"; break;
    Case(Circle,   Square,   Square  , Square  ) text = "C,S,S,S"; break;
    Case(Circle,   Square,   Square  , Triangle) text = "C,S,S,T"; break;
    Case(Circle,   Square,   Triangle, Square  ) text = "C,S,T,S"; break;
    Case(Circle,   Square,   Triangle, Triangle) text = "C,S,T,T"; break;
    Case(Circle,   Square,   Triangle, Circle  ) text = "C,S,T,C"; break;
    Case(Circle,   Square,   Circle  , Triangle) text = "C,S,C,T"; break;
                                                           
    Case(Circle,   Triangle, Circle  , Circle  ) text = "C,T,C,C"; break;
    Case(Circle,   Triangle, Circle  , Square  ) text = "C,T,C,S"; break;
    Case(Circle,   Triangle, Square  , Circle  ) text = "C,T,S,C"; break;
    Case(Circle,   Triangle, Square  , Square  ) text = "C,T,S,S"; break;
    Case(Circle,   Triangle, Square  , Triangle) text = "C,T,S,T"; break;
    Case(Circle,   Triangle, Triangle, Square  ) text = "C,T,T,S"; break;
    Case(Circle,   Triangle, Triangle, Triangle) text = "C,T,T,T"; break;
    Case(Circle,   Triangle, Triangle, Circle  ) text = "C,T,T,C"; break;
    Case(Circle,   Triangle, Circle  , Triangle) text = "C,T,C,T"; break;
                                                           
                                                           
    Case(Square,   Circle,   Circle  , Circle  ) text = "S,C,C,C"; break;
    Case(Square,   Circle,   Circle  , Square  ) text = "S,C,C,S"; break;
    Case(Square,   Circle,   Square  , Circle  ) text = "S,C,S,C"; break;
    Case(Square,   Circle,   Square  , Square  ) text = "S,C,S,S"; break;
    Case(Square,   Circle,   Square  , Triangle) text = "S,C,S,T"; break;
    Case(Square,   Circle,   Triangle, Square  ) text = "S,C,T,S"; break;
    Case(Square,   Circle,   Triangle, Triangle) text = "S,C,T,T"; break;
    Case(Square,   Circle,   Triangle, Circle  ) text = "S,C,T,C"; break;
    Case(Square,   Circle,   Circle  , Triangle) text = "S,C,C,T"; break;
                                                           
    Case(Square,   Square,   Circle  , Circle  ) text = "S,S,C,C"; break;
    Case(Square,   Square,   Circle  , Square  ) text = "S,S,C,S"; break;
    Case(Square,   Square,   Square  , Circle  ) text = "S,S,S,C"; break;
    Case(Square,   Square,   Square  , Square  ) text = "S,S,S,S"; break;
    Case(Square,   Square,   Square  , Triangle) text = "S,S,S,T"; break;
    Case(Square,   Square,   Triangle, Square  ) text = "S,S,T,S"; break;
    Case(Square,   Square,   Triangle, Triangle) text = "S,S,T,T"; break;
    Case(Square,   Square,   Triangle, Circle  ) text = "S,S,T,C"; break;
    Case(Square,   Square,   Circle  , Triangle) text = "S,S,C,T"; break;
                                                           
    Case(Square,   Triangle, Circle  , Circle  ) text = "S,T,C,C"; break;
    Case(Square,   Triangle, Circle  , Square  ) text = "S,T,C,S"; break;
    Case(Square,   Triangle, Square  , Circle  ) text = "S,T,S,C"; break;
    Case(Square,   Triangle, Square  , Square  ) text = "S,T,S,S"; break;
    Case(Square,   Triangle, Square  , Triangle) text = "S,T,S,T"; break;
    Case(Square,   Triangle, Triangle, Square  ) text = "S,T,T,S"; break;
    Case(Square,   Triangle, Triangle, Triangle) text = "S,T,T,T"; break;
    Case(Square,   Triangle, Triangle, Circle  ) text = "S,T,T,C"; break;
    Case(Square,   Triangle, Circle  , Triangle) text = "S,T,C,T"; break;
                                                           
                                                           
    Case(Triangle, Circle,   Circle  , Circle  ) text = "T,C,C,C"; break;
    Case(Triangle, Circle,   Circle  , Square  ) text = "T,C,C,S"; break;
    Case(Triangle, Circle,   Square  , Circle  ) text = "T,C,S,C"; break;
    Case(Triangle, Circle,   Square  , Square  ) text = "T,C,S,S"; break;
    Case(Triangle, Circle,   Square  , Triangle) text = "T,C,S,T"; break;
    Case(Triangle, Circle,   Triangle, Square  ) text = "T,C,T,S"; break;
    Case(Triangle, Circle,   Triangle, Triangle) text = "T,C,T,T"; break;
    Case(Triangle, Circle,   Triangle, Circle  ) text = "T,C,T,C"; break;
    Case(Triangle, Circle,   Circle  , Triangle) text = "T,C,C,T"; break;
                                                           
    Case(Triangle, Square,   Circle  , Circle  ) text = "T,S,C,C"; break;
    Case(Triangle, Square,   Circle  , Square  ) text = "T,S,C,S"; break;
    Case(Triangle, Square,   Square  , Circle  ) text = "T,S,S,C"; break;
    Case(Triangle, Square,   Square  , Square  ) text = "T,S,S,S"; break;
    Case(Triangle, Square,   Square  , Triangle) text = "T,S,S,T"; break;
    Case(Triangle, Square,   Triangle, Square  ) text = "T,S,T,S"; break;
    Case(Triangle, Square,   Triangle, Triangle) text = "T,S,T,T"; break;
    Case(Triangle, Square,   Triangle, Circle  ) text = "T,S,T,C"; break;
    Case(Triangle, Square,   Circle  , Triangle) text = "T,S,C,T"; break;
                                                           
    Case(Triangle, Triangle, Circle  , Circle  ) text = "T,T,C,C"; break;
    Case(Triangle, Triangle, Circle  , Square  ) text = "T,T,C,S"; break;
    Case(Triangle, Triangle, Square  , Circle  ) text = "T,T,S,C"; break;
    Case(Triangle, Triangle, Square  , Square  ) text = "T,T,S,S"; break;
    Case(Triangle, Triangle, Square  , Triangle) text = "T,T,S,T"; break;
    Case(Triangle, Triangle, Triangle, Square  ) text = "T,T,T,S"; break;
    Case(Triangle, Triangle, Triangle, Triangle) text = "T,T,T,T"; break;
    Case(Triangle, Triangle, Triangle, Circle  ) text = "T,T,T,C"; break;
    Case(Triangle, Triangle, Circle  , Triangle) text = "T,T,C,T"; break;
    
    Otherwise()                                  text = "other";   break;
    }
    EndMatch

    std::cout << text << std::endl;
}

//------------------------------------------------------------------------------

int main()
{
    Shape* c = new Circle(loc(1,1),7);
    Shape* s = new Square(loc(1,1),2);
    Shape* t = new Triangle(loc(1,1),loc(1,0),loc(0,0));

    Shape* shapes[] = {c,s,t};

    for (size_t n = 0; n < 3; n++)
    {
        // 1 argument
        for (size_t i = 0; i < 3; ++i)
            do_match(shapes[i]);
        
        // 2 arguments
        for (size_t i = 0; i < 3; ++i)
        for (size_t j = 0; j < 3; ++j)
            do_match(shapes[i], shapes[j]);
        
        // 3 arguments
        for (size_t i = 0; i < 3; ++i)
        for (size_t j = 0; j < 3; ++j)
        for (size_t k = 0; k < 3; ++k)
            do_match(shapes[i], shapes[j], shapes[k]);
        
        // 4 arguments
        for (size_t i = 0; i < 3; ++i)
        for (size_t j = 0; j < 3; ++j)
        for (size_t k = 0; k < 3; ++k)
        for (size_t l = 0; l < 3; ++l)
            do_match(shapes[i], shapes[j], shapes[k], shapes[l]);
    }
}

//------------------------------------------------------------------------------
