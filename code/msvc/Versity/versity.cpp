#include "versity-bindings.hpp"
#include "type_switchN-patterns.hpp"
#include "patterns/constructor.hpp"    // Constructor pattern

#include <cassert>
#include <cstdio>
#include <cstdlib>
#include <fstream>
#include <iomanip>
#include <iostream>

//------------------------------------------------------------------------------

extern int yyparse(void);
extern int yy_flex_debug;
int yydebug;

//------------------------------------------------------------------------------

int main(int argc, char* argv[])
{
    yydebug = 0;       // disable Yacc debugging mode
    yy_flex_debug = 0; // disable Flex debugging mode
    std::string base_name = "undefined";

    try
    {
        if (argc < 2)
        {
            std::cerr << "Usage: " << argv[0] << " filename.i0" << std::endl;
            return 1;
        }

        if (!freopen(argv[1],"r",stdin)) //redirect standard input
        {
            std::cerr << "Error: Can't open file " << argv[1] << std::endl;
            return 2;
        }

        base_name = argv[1];
        base_name.erase(base_name.find_last_of('.'));

        int result = yyparse();
        std::clog.flush();

        if (result == 0)
        {
            extern std::vector<Inst*>* instruction_stream;

            if (instruction_stream)
            {
                std::cout << '[';

                for (std::vector<Inst*>::const_iterator p = instruction_stream->begin(); p != instruction_stream->end(); ++p)
                    std::cout << **p << ',' << std::endl;

                std::cout << ']';
            }
        }

        return result;
    }
    catch (...)
    {
        std::cerr << "ERROR: Unhandled exception caught while parsing" << std::endl;
    	return 4;
    }
}

//------------------------------------------------------------------------------

using mch::C; // Enable use of constructor pattern without namespace qualification

//------------------------------------------------------------------------------

std::ostream& operator<<(std::ostream& os, const TyPrim& t)
{
    int n;

    Match(t)
    {
    Case(C<TyChar   >( )) os << "TyChar"         ; break;
    Case(C<TyUChar  >( )) os << "TyUChar"        ; break;
    Case(C<TyShort  >( )) os << "TyShort"        ; break;
    Case(C<TyUShort >( )) os << "TyUShort"       ; break;
    Case(C<TyInt    >( )) os << "TyInt"          ; break;
    Case(C<TyUInt   >( )) os << "TyUInt"         ; break;
    Case(C<TyLong   >( )) os << "TyLong"         ; break;
    Case(C<TyULong  >( )) os << "TyULong"        ; break;
    Case(C<TyLLong  >( )) os << "TyLLong"        ; break;
    Case(C<TyULLong >( )) os << "TyULLong"       ; break;
    Case(C<TyFloat  >( )) os << "TyFloat"        ; break;
    Case(C<TyDouble >( )) os << "TyDouble"       ; break;
    Case(C<TyCharN  >(n)) os << "TyCharN"    << n; break;
    Case(C<TyUCharN >(n)) os << "TyUCharN"   << n; break;
    Case(C<TyShortN >(n)) os << "TyShortN"   << n; break;
    Case(C<TyUShortN>(n)) os << "TyUShortN"  << n; break;
    Case(C<TyIntN   >(n)) os << "TyIntN"     << n; break;
    Case(C<TyUIntN  >(n)) os << "TyUIntN"    << n; break;
    Case(C<TyLongN  >(n)) os << "TyLongN"    << n; break;
    Case(C<TyULongN >(n)) os << "TyULongN"   << n; break;
    Case(C<TyLLongN >(n)) os << "TyLLongN"   << n; break;
    Case(C<TyULLongN>(n)) os << "TyULLongN"  << n; break;
    Case(C<TyFloatN >(n)) os << "TyFloatN"   << n; break;
    Case(C<TyDoubleN>(n)) os << "TyDoubleN"  << n; break;
    }
    EndMatch

    return os;
}

//------------------------------------------------------------------------------

std::ostream& operator<<(std::ostream& os, const Val& v)
{
    unsigned long long u;
             long long n;
                double d;

    Match(v)
    {
    Case(C<ValDouble >(d)) os << "(ValDouble  " << d << ')'; break;
    Case(C<ValFloat  >(d)) os << "(ValFloat   " << d << ')'; break;
    Case(C<ValChar   >(n)) os << "(ValChar    " << n << ')'; break;
    Case(C<ValUChar  >(u)) os << "(ValUChar   " << u << ')'; break;
    Case(C<ValShort  >(n)) os << "(ValShort   " << n << ')'; break;
    Case(C<ValUShort >(u)) os << "(ValUShort  " << u << ')'; break;
    Case(C<ValInt    >(n)) os << "(ValInt     " << n << ')'; break;
    Case(C<ValUInt   >(u)) os << "(ValUInt    " << u << ')'; break;
    Case(C<ValLong   >(n)) os << "(ValLong    " << n << ')'; break;
    Case(C<ValULong  >(u)) os << "(ValULong   " << u << ')'; break;
    Case(C<ValLLong  >(n)) os << "(ValLLong   " << n << ')'; break;
    Case(C<ValULLong >(u)) os << "(ValULLong  " << u << ')'; break;
    Case(C<ValDoubleN>( )) os << "(ValDoubleN "      << ')'; break;
    Case(C<ValFloatN >( )) os << "(ValFloatN  "      << ')'; break;
    Case(C<ValCharN  >( )) os << "(ValCharN   "      << ')'; break;
    Case(C<ValUCharN >( )) os << "(ValUCharN  "      << ')'; break;
    Case(C<ValShortN >( )) os << "(ValShortN  "      << ')'; break;
    Case(C<ValUShortN>( )) os << "(ValUShortN "      << ')'; break;
    Case(C<ValIntN   >( )) os << "(ValIntN    "      << ')'; break;
    Case(C<ValUIntN  >( )) os << "(ValUIntN   "      << ')'; break;
    Case(C<ValLongN  >( )) os << "(ValLongN   "      << ')'; break;
    Case(C<ValULongN >( )) os << "(ValULongN  "      << ')'; break;
    Case(C<ValLLongN >( )) os << "(ValLLongN  "      << ')'; break;
    Case(C<ValULLongN>( )) os << "(ValULLongN "      << ')'; break;
    }
    EndMatch

    return os;
}

//------------------------------------------------------------------------------

std::ostream& operator<<(std::ostream& os, const Reg& r)
{
    std::string name;
    int         n;
    TyPrim*     type;
    Val*        value;

    Match(r)
    {
    Case(C<Ext>(type, name)) os << "(Ext " << *type << ' ' << name << ')'; break;
    Case(C<Tmp>(type, n))    os << "(Tmp " << *type << ' ' << n    << ')'; break;
    Case(C<Lit>(value))      os << "(Lit " << *value               << ')'; break;
    }
    EndMatch

    return os;
}

//------------------------------------------------------------------------------

std::ostream& operator<<(std::ostream& os, const Inst& i)
{
    std::string op; 
    Reg* arg1; Reg* arg2; Reg* src; Reg* dest;

    Match(i)
    {
    Case(C<InstBin >(C<Op>(op), arg1, arg2, dest)) 
        os << "InstBin (Op " << op << ") " << *arg1 << ' ' << *arg2 << ' ' << *dest; break;
    Case(C<InstUn  >(C<Op>(op), arg1, dest))
        os << "InstUn  (Op " << op << ") " << *arg1 << ' ' << *dest; break;
    Case(C<InstMove>(src, dest))
        os << "InstMove " << *src  << ' ' << *dest; break;
    }
    EndMatch

    return os;
}

//------------------------------------------------------------------------------
