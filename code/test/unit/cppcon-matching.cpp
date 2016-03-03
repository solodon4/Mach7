//
//  Mach7: Pattern Matching Library for C++
//
//  Copyright 2011-2013, Texas A&M University.
//  Copyright 2014 Yuriy Solodkyy.
//  All rights reserved.
//
//  Redistribution and use in source and binary forms, with or without
//  modification, are permitted provided that the following conditions are met:
//
//      * Redistributions of source code must retain the above copyright
//        notice, this list of conditions and the following disclaimer.
//
//      * Redistributions in binary form must reproduce the above copyright
//        notice, this list of conditions and the following disclaimer in the
//        documentation and/or other materials provided with the distribution.
//
//      * Neither the names of Mach7 project nor the names of its contributors
//        may be used to endorse or promote products derived from this software
//        without specific prior written permission.
//
//  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
//  ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
//  WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
//  IN NO EVENT SHALL THE COPYRIGHT HOLDERS OR CONTRIBUTORS BE LIABLE FOR ANY
//  DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
//  (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
//  LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
//  ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
//  (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
//  SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

///
/// \file
///
/// This file contains examples used in our CppCon 2014 talk "Accept No Visitors".
/// \see http://bit.ly/AcceptNoVisitors
///
/// \author Yuriy Solodkyy <yuriy.solodkyy@gmail.com>
///
/// \see https://parasol.tamu.edu/mach7/
/// \see https://github.com/solodon4/Mach7
/// \see https://github.com/solodon4/SELL
///

#include <string>
#include <mach7/type_switchN-patterns.hpp> // Support for N-ary Match statement on patterns
#include <mach7/patterns/address.hpp>      // Address and dereference combinators
#include <mach7/patterns/bindings.hpp>     // Mach7 support for bindings on arbitrary UDT
#include <mach7/patterns/constructor.hpp>  // Support for constructor patterns
#include <mach7/patterns/equivalence.hpp>  // Equivalence combinator +
#include <mach7/patterns/primitive.hpp>    // Wildcard, variable and value patterns

struct BoolExp          { virtual ~BoolExp() {} };
struct VarExp : BoolExp { VarExp(const char* n)            : name(n)        {} std::string name; };
struct ValExp : BoolExp { ValExp(bool b)                   : value(b)       {} bool value; };
struct NotExp : BoolExp { NotExp(BoolExp* e)               : e(e)           {} BoolExp* e; };
struct AndExp : BoolExp { AndExp(BoolExp* e1, BoolExp* e2) : e1(e1), e2(e2) {} BoolExp* e1; BoolExp* e2; };
struct OrExp  : BoolExp { OrExp (BoolExp* e1, BoolExp* e2) : e1(e1), e2(e2) {} BoolExp* e1; BoolExp* e2; };

namespace mch ///< Mach7 library namespace
{
    template <> struct bindings<VarExp> { Members(VarExp::name);  };
    template <> struct bindings<ValExp> { Members(ValExp::value); };
    template <> struct bindings<NotExp> { Members(NotExp::e);     };
    template <> struct bindings<AndExp> { Members(AndExp::e1, AndExp::e2); };
    template <> struct bindings<OrExp>  { Members( OrExp::e1,  OrExp::e2); };
} // of namespace mch

using mch::C;
using mch::var;
using mch::_;

#include <iostream>

void print(const BoolExp* exp)
{
    var<std::string> name; var<bool> value; var<const BoolExp*> e1, e2;

    Match(exp)
        Case(C<VarExp>(name))  std::cout << name;  break;
        Case(C<ValExp>(value)) std::cout << value; break;
        Case(C<NotExp>(e1))    std::cout << '!'; print(e1); break;
        Case(C<AndExp>(e1,e2)) std::cout << '('; print(e1); std::cout << " & "; print(e2); std::cout << ')'; break;
        Case(C<OrExp >(e1,e2)) std::cout << '('; print(e1); std::cout << " | "; print(e2); std::cout << ')'; break;
    EndMatch
}

BoolExp* copy(const BoolExp* exp)
{
    var<std::string> name; var<bool> value; var<const BoolExp*> e1, e2;

    Match(exp)
        Case(C<VarExp>(name))  return new VarExp(name.c_str());
        Case(C<ValExp>(value)) return new ValExp(value);
        Case(C<NotExp>(e1))    return new NotExp(copy(e1));
        Case(C<AndExp>(e1,e2)) return new AndExp(copy(e1), copy(e2));
        Case(C<OrExp >(e1,e2)) return new  OrExp(copy(e1), copy(e2));
    EndMatch

    XTL_UNREACHABLE; // To avoid warning that control may reach end of a non-void function
}

#include <map>

template <class K, class T, class C,  class A>
std::ostream& operator<<(std::ostream& os, const std::map<K,T,C,A>& m)
{
	for (typename std::map<K,T,C,A>::const_iterator p = m.begin(); p != m.end(); ++p)
	{
		os << p->first << '=' << p->second << std::endl;
	}

	return os;
}

typedef std::map<std::string,bool> Context;

bool eval(Context& ctx, const BoolExp* exp)
{
    var<std::string> name; var<bool> value; var<const BoolExp*> e1, e2;

    Match(exp)
        Case(C<VarExp>(name))  return ctx[name];
        Case(C<ValExp>(value)) return value;
        Case(C<NotExp>(e1))    return!eval(ctx, e1);
        Case(C<AndExp>(e1,e2)) return eval(ctx, e1) && eval(ctx, e2);
        Case(C<OrExp >(e1,e2)) return eval(ctx, e1) || eval(ctx, e2);
    EndMatch

    XTL_UNREACHABLE; // To avoid warning that control may reach end of a non-void function
}

BoolExp* replace(const BoolExp* where, const char* what, const BoolExp* with)
{
    var<std::string> name; var<bool> value; var<const BoolExp*> e1, e2;

    Match(where)
        Case(C<VarExp>(name))  return name == what ? copy(with) : copy(&match0);
        Case(C<ValExp>(value)) return copy(&match0);
        Case(C<NotExp>(e1))    return new NotExp(replace(e1, what, with));
        Case(C<AndExp>(e1,e2)) return new AndExp(replace(e1, what, with), replace(e2, what, with));
        Case(C<OrExp >(e1,e2)) return new  OrExp(replace(e1, what, with), replace(e2, what, with));
    EndMatch

    XTL_UNREACHABLE; // To avoid warning that control may reach end of a non-void function
}

BoolExp* inplace(/*const*/ BoolExp* where, const char* what, const BoolExp* with)
{
    var<std::string> name; var<bool> value; var</*const*/ BoolExp*> e1, e2;

    Match(where)
        Case(C<VarExp>(name))  return name == what ? copy(with) : &match0;
        Case(C<ValExp>(value)) return &match0;
        Case(C<NotExp>(e1))    match0.e  = inplace(e1, what, with); return &match0;
        Case(C<AndExp>(e1,e2)) match0.e1 = inplace(e1, what, with); match0.e2 = inplace(e2, what, with); return &match0;
        Case(C<OrExp >(e1,e2)) match0.e1 = inplace(e1, what, with); match0.e2 = inplace(e2, what, with); return &match0;
    EndMatch

    XTL_UNREACHABLE; // To avoid warning that control may reach end of a non-void function
}

bool equal(const BoolExp* x1, const BoolExp* x2)
{
    var<std::string> name; var<bool> value; var<const BoolExp*> e1, e2, e3, e4;

    Match(x1,x2)
        Case(C<VarExp>(name),  C<VarExp>(+name) ) return true;
        Case(C<ValExp>(value), C<ValExp>(+value)) return true;
        Case(C<NotExp>(e1),    C<NotExp>(e2))     return equal(e1,e2);
        Case(C<AndExp>(e1,e2), C<AndExp>(e3,e4))  return equal(e1,e3) && equal(e2,e4);
        Case(C< OrExp>(e1,e2), C< OrExp>(e3,e4))  return equal(e1,e3) && equal(e2,e4);
        Otherwise()                               return false;
    EndMatch

    XTL_UNREACHABLE; // To avoid warning that control may reach end of a non-void function
}

typedef std::map<std::string,const BoolExp*> Assignments;

// Other example: unify
bool match(const BoolExp* p, const BoolExp* x, Assignments& ctx)
{
    var<std::string> name; var<bool> value; var<const BoolExp*> p1, p2, e1, e2;

    Match(p,x)
        Case(C<VarExp>(name),  _                ) if (ctx[name] == nullptr) { ctx[name] = copy(x); return true; } else { return equal(ctx[name],x); }
        Case(C<ValExp>(value), C<ValExp>(+value)) return true;
        Case(C<NotExp>(p1),    C<NotExp>(e1)    ) return match(p1, e1, ctx);
        Case(C<AndExp>(p1,p2), C<AndExp>(e1,e2) ) return match(p1, e1, ctx) && match(p2, e2, ctx);
        Case(C< OrExp>(p1,p2), C< OrExp>(e1,e2) ) return match(p1, e1, ctx) && match(p2, e2, ctx);
        Otherwise()                               return false;
    EndMatch

    XTL_UNREACHABLE; // To avoid warning that control may reach end of a non-void function
}

// NOTE: It took me 1 hour 20 minutes (from 9:20am till 10:42am according to Event Viewer) to rewrite all the visitors
//       using pattern matching, get rid of any compilation errors and run.

int main()
{
    BoolExp* exp1 = new AndExp(new OrExp(new VarExp("X"), new VarExp("Y")), new NotExp(new VarExp("Z")));

    std::cout << "exp1 = "; print(exp1); std::cout << std::endl;

    BoolExp* exp2 = copy(exp1);

    std::cout << "exp2 = "; print(exp2); std::cout << std::endl;

    BoolExp* exp3 = replace(exp1, "Z", exp2);

    std::cout << "exp3 = "; print(exp3); std::cout << std::endl;

    BoolExp* exp4 = inplace(exp1, "Z", exp2);

    std::cout << "exp4 = "; print(exp4); std::cout << std::endl;
    std::cout << "exp1 = "; print(exp1); std::cout << " updated! " << std::endl;

    std::cout << (equal(exp1,exp4) ? "exp1 == exp4" : "exp1 <> exp4") << std::endl;
    std::cout << (equal(exp1,exp2) ? "exp1 == exp2" : "exp1 <> exp2") << std::endl;

    Context ctx;
    ctx["Y"] = true;
    std::cout << eval(ctx, exp1) << std::endl;
    std::cout << eval(ctx, exp2) << std::endl;
    std::cout << eval(ctx, exp3) << std::endl;

	std::cout << ctx << std::endl;

    Assignments ctx2;

    if (match(exp2,exp3,ctx2))
    {
        std::cout << "exp2 matches exp3 with assignments: " << std::endl;

		for (Assignments::const_iterator p = ctx2.begin(); p != ctx2.end(); ++p)
		{
			std::cout << p->first << '='; print(p->second); std::cout << std::endl;
		}
    }
}
