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
/// This file is a part of Mach7 library test suite.
///
/// \author Yuriy Solodkyy <yuriy.solodkyy@gmail.com>
///
/// \see https://parasol.tamu.edu/mach7/
/// \see https://github.com/solodon4/Mach7
/// \see https://github.com/solodon4/SELL
///

#include "versity-bindings.hpp"
#include <mach7/type_switchN-patterns.hpp> // Support for N-ary Match statement on patterns
#include <mach7/patterns/address.hpp>      // Address and dereference combinators
#include <mach7/patterns/combinators.hpp>  // Support for pattern combinators
#include <mach7/patterns/constructor.hpp>  // Support for constructor patterns
#include <mach7/patterns/equivalence.hpp>  // Equivalence combinator +
#include <mach7/patterns/guard.hpp>        // Support for guard patterns
#include <mach7/patterns/n+k.hpp>          // Generalized n+k patterns

//------------------------------------------------------------------------------

using namespace mch;

//------------------------------------------------------------------------------

//renameRegs src dst (x:xs) = 
//    if stop 
//       then (case x of
//                  InstMove a b     -> InstMove    (rename a) b
//                  InstUn op a b    -> InstUn   op (rename a) b
//                  InstBin op a b c -> InstBin  op (rename a) (rename b) c
//            ) : xs
//       else (case x of
//                  InstMove a b     -> InstMove    (rename a) (rename b)
//                  InstUn op a b    -> InstUn   op (rename a) (rename b)
//                  InstBin op a b c -> InstBin  op (rename a) (rename b) (rename c)
//            ) : (renameRegs src dst xs)
//    where stop = case x of
//                      InstMove _ b    -> (b == src) || (b == dst)
//                      InstUn _ _ b    -> (b == src) || (b == dst)
//                      InstBin _ _ _ c -> (c == src) || (c == dst)
//          rename reg = if reg == src
//                          then dst
//                          else reg
//
//renameRegs _ _ [] = []

void renameRegs(
    Reg& src,
    Reg& dst,
    instruction_stream_type::iterator p,
    instruction_stream_type::iterator q)
{
    auto rename = [&src,&dst] (Reg& reg) -> Reg*
    {
        return reg == src ? &dst : &src;
    };

    std::string op;
    var<Reg&> a, b, c;
    // Convenience pattern that will match either src or dst register by value
    auto either_reg = &val(src) || &val(dst);

    while(p != q)
    {
        bool stop = false;

        Match(**p)
        Case(C<InstMove>(_,       either_reg)) stop = true; break;
        Case(C<InstUn  >(_, _,    either_reg)) stop = true; break;
        Case(C<InstBin >(_, _, _, either_reg)) stop = true; break;
        EndMatch

        Match(**p)
        Case(C<InstMove>(&a, &b))
            Reg*  d = !stop ? rename(b) : std::move(b.m_value);
            Inst* i = new InstMove(rename(a),               d);
            //delete *p;
            *p = i;
            break;
        Case(C<InstUn>(C<Op>(op), &a, &b))
            Reg*  d = !stop ? rename(b) : std::move(b.m_value);
            Inst* i = new InstUn(op, rename(a),             d);
            //delete *p;
            *p = i;
            break;
        Case(C<InstBin>(C<Op>(op), &a, &b, &c))
            Reg*  d = !stop ? rename(c) : std::move(c.m_value);
            Inst* i = new InstBin(op, rename(a), rename(b), d);
            //delete *p;
            *p = i;
            break;
        EndMatch

        if(!stop) {
            p++;
        }
        else {
            break;
        }
    }
}

//------------------------------------------------------------------------------

//chooseReg a b = case (a, b) of
//    (a@(Tmp _ _), b@(Tmp _ _)) -> (True, a, b)
//    (a@(Tmp _ _), b@(Ext _ _)) -> (True, a, b)
//    (a@(Ext _ _), b@(Tmp _ _)) -> (True, b, a)
//    (a@(Lit _), b@(Tmp _ _)) -> (True, b, a)
//    (a, b) -> (False, err, err)
//    where err = error $ "trying to use results of chooseReg " ++ (show (a, b))

bool chooseReg(Reg& a, Reg& b, Reg*& src_out, Reg*& dst_out)
{
    Match(a, b);
    Case(C<Tmp>(_, _), C<Tmp>(_, _)) src_out = &match0; dst_out = &match1; return true;
    Case(C<Tmp>(_, _), C<Ext>(_, _)) src_out = &match0; dst_out = &match1; return true;
    Case(C<Ext>(_, _), C<Tmp>(_, _)) src_out = &match0; dst_out = &match1; return true;
    Case(C<Lit>(_),    C<Tmp>(_, _)) src_out = &match0; dst_out = &match1; return true;
    EndMatch

    return false;
}

//------------------------------------------------------------------------------

//renameRegsMove :: Reg -> Reg -> Reg -> [Inst] -> [Inst]
//renameRegsMove a b d xs =
//    (InstMove a dst):if doRename
//                        then renameRegs src dst xs
//                        else xs
//
//    where (doRename, src, dst) = chooseReg b d

void renameRegsMove(
    Reg& a,
    Reg& b,
    Reg& d,
    Inst*& x1_ref,
    Inst*& x2_out,
    instruction_stream_type::iterator& p_ref,
    const instruction_stream_type::iterator q)
{
    Reg* src;
    Reg* dst;

    bool doRename = chooseReg(b, d, src, dst);
    
    Inst* i = new InstMove(&a, dst);
    //delete x1_ref;
    x1_ref = i;

    if (doRename) {
        renameRegs(*src, *dst, p_ref, q);
    }

    //delete x2_out
    x2_out = p_ref == q ? *p_ref++ : nullptr;
}

//------------------------------------------------------------------------------

//renameRegsUn :: Op -> Reg -> Reg -> Reg -> [Inst] -> [Inst]
//renameRegsUn op a b d xs = 
//    (InstUn op a dst):if doRename
//                         then renameRegs src dst xs
//                         else xs
//
//    where (doRename, src, dst) = chooseReg b d

void renameRegsUn(
    std::string& op,
    Reg& a,
    Reg& b,
    Reg& d,
    Inst*& x1_ref,
    Inst*& x2_out,
    instruction_stream_type::iterator& p_ref,
    const instruction_stream_type::iterator q)
{
    Reg* src;
    Reg* dst;

    bool doRename = chooseReg(b, d, src, dst);
    
    Inst* i = new InstUn(op, &a, dst);
    //delete x1_ref;
    x1_ref = i;

    if (doRename) {
        renameRegs(*src, *dst, p_ref, q);
    }

    //delete x2_out
    x2_out = p_ref == q ? *p_ref++ : nullptr;
}

//------------------------------------------------------------------------------

//renameRegsBin :: Op -> Reg -> Reg -> Reg -> Reg -> [Inst] -> [Inst]
//renameRegsBin op a b d e xs = 
//    (InstBin op a b dst):if doRename
//                            then renameRegs src dst xs
//                            else xs
//
//    where (doRename, src, dst) = chooseReg d e

void renameRegsBin(
    std::string& op,
    Reg& a,
    Reg& b,
    Reg& d,
    Reg& e,
    Inst*& x1_ref,
    Inst*& x2_out,
    instruction_stream_type::iterator& p_ref,
    const instruction_stream_type::iterator q)
{
    Reg* src;
    Reg* dst;

    bool doRename = chooseReg(d, e, src, dst);
    
    Inst* i = new InstBin(op, &a, &b, dst);
    //delete x1_ref;
    x1_ref = i;

    if (doRename) {
        renameRegs(*src, *dst, p_ref, q);
    }

    //delete x2_out
    x2_out = p_ref == q ? *p_ref++ : nullptr;
}

// peep3 (x1:x2:x3:xs) = case x1 of
//     InstMove a b -> case x2 of
//                          InstMove c d     | b == c               -> case x3 of
//                                                                          InstMove _ f    | b == f -> peep3 $ (InstMove a d):x3:xs
//                                                                          InstUn _ _ f    | b == f -> peep3 $ (InstMove a d):x3:xs
//                                                                          InstBin _ _ _ f | b == f -> peep3 $ (InstMove a d):x3:xs
//                                                                          _ -> x1:(peep3 $ x2:x3:xs)
//                          InstMove _ _                            -> case x3 of
//                                                                          InstMove e g     | b == g && b == e                   -> peep3 $ x2:(InstMove a g):x3:xs
//                                                                          InstUn op e g    | b == g && b == e                   -> peep3 $ x2:(InstUn op a g):x3:xs
//                                                                          InstBin op e f g | (b == g) && ((b == e) && (b /= f)) -> peep3 $ x2:(InstBin op a f g):xs
//                                                                          InstBin op e f g | (b == g) && ((b /= e) && (b == f)) -> peep3 $ x2:(InstBin op e a g):xs
//                                                                          InstBin op e f g | (b == g) && ((b == e) && (b == f)) -> peep3 $ x2:(InstBin op a a g):xs
//                                                                          _ -> x1:(peep3 $ x2:x3:xs)
//                          InstUn _ c d     | b == c               -> case x3 of
//                                                                          InstMove _ f    | b == f -> peep3 $ (InstMove a d):x3:xs
//                                                                          InstUn _ _ f    | b == f -> peep3 $ (InstMove a d):x3:xs
//                                                                          InstBin _ _ _ f | b == f -> peep3 $ (InstMove a d):x3:xs
//                                                                          _ -> x1:(peep3 $ x2:x3:xs)
//                          InstBin op c d e | (b == d) && (b /= c) -> case x3 of
//                                                                          InstMove _ f    | b == f -> peep3 $ (InstBin op c a e):x3:xs
//                                                                          InstUn _ _ f    | b == f -> peep3 $ (InstBin op c a e):x3:xs
//                                                                          InstBin _ _ _ f | b == f -> peep3 $ (InstBin op c a e):x3:xs
//                                                                          _ -> x1:(peep3 $ x2:x3:xs)
//                          InstBin op c d e | (b /= d) && (b == c) -> case x3 of
//                                                                          InstMove _ f    | b == f -> peep3 $ (InstBin op a d e):x3:xs
//                                                                          InstUn _ _ f    | b == f -> peep3 $ (InstBin op a d e):x3:xs
//                                                                          InstBin _ _ _ f | b == f -> peep3 $ (InstBin op a d e):x3:xs
//                                                                          _ -> x1:(peep3 $ x2:x3:xs)
//                          InstBin op c d e | (b == d) && (b == c) -> case x3 of
//                                                                          InstMove _ f    | b == f -> peep3 $ (InstBin op a a e):x3:xs
//                                                                          InstUn _ _ f    | b == f -> peep3 $ (InstBin op a a e):x3:xs
//                                                                          InstBin _ _ _ f | b == f -> peep3 $ (InstBin op a a e):x3:xs
//                                                                          _ -> x1:(peep3 $ x2:x3:xs)
//                          _ -> x1:(peep3 $ x2:x3:xs)
//     _ -> x1:(peep3 $ x2:x3:xs)
/*
instruction_stream_type::iterator peep3(
    instruction_stream_type::iterator& p, 
    instruction_stream_type::iterator& q,
    instruction_stream_type::iterator& r)
{
    var<Reg&> a, b, c, d, e, f, g;
    std::string op;

    Inst* x1 = *p++;

    if (p == q) {
        *r++ = x1;
        return;
    }

    Inst* x2 = *p++;

    if (p == q) {
        *r++ = x1;
        *r++ = x2;
        return;
    }

    Inst* x3 = *p++;

    do {
    } while (p != q);

    return r;
}
*/
//------------------------------------------------------------------------------

//peep2 (x1:x2:xs) = case x1 of
//    InstMove a b -> case x2 of
//                         InstMove c d     | (a == d) && (b == c)               -> peep2 $ x1:xs
//                         InstMove c d     | (b == c)                           -> peep2 $ renameRegsMove a c d xs
//                         InstUn  op c d   | (b == d) && (b == c)               -> peep2 $ (InstUn  op a b  ):xs
//                         InstBin op c d e | (b == e) && ((c == b) && (d /= b)) -> peep2 $ (InstBin op a d e):xs
//                         InstBin op c d e | (b == e) && ((c /= b) && (d == b)) -> peep2 $ (InstBin op c a e):xs
//                         InstBin op c d e | (b == e) && ((c == b) && (d == b)) -> peep2 $ (InstBin op a a e):xs
//                         _ -> x1:(peep2 $ x2:xs)
//    InstUn op a b -> case x2 of 
//                         InstMove c d     | (b == c)                           -> peep2 $ renameRegsUn op a c d xs
//                         _ -> x1:(peep2 $ x2:xs)
//    InstBin op a b c -> case x2 of 
//                         InstMove d e     | (c == d)                           -> peep2 $ renameRegsBin op a b d e xs
//                         _ -> x1:(peep2 $ x2:xs)
//
//peep2 (x:xs) = x:(peep2 xs)
//peep2 [] = []

instruction_stream_type::iterator peep2(
    instruction_stream_type::iterator& p, 
    instruction_stream_type::iterator& q,
    instruction_stream_type::iterator& r)
{
    var<Reg&> a, b, c, d;
    std::string op;

    Inst* x1 = *p++;

    if (p == q) {
        *r++ = x1;
        return r;
    }

    Inst* x2 = *p++;

    do {
        Match(*x1,*x2)
        {
        Case(C<InstMove>(&a,&b), C<InstMove>(&+b,&+a))
            // delete x2
            x2 = p != q ? *p++ : nullptr;
            break;
        Case(C<InstMove>(&a,&b), C<InstMove>(&+b,&d))
            renameRegsMove(a,b,d,x1,x2,p,q);
            break;
        Case(C<InstMove>(&a,&b), C<InstUn>(C<Op>(op),&+b,&+b))
            InstUn* i = new InstUn(op,a.m_value,b.m_value);
            //delete x1; delete x2;
            x1 = i;
            x2 = p != q ? *p++ : nullptr;
            break;
        Case(C<InstMove>(&a,&b), C<InstBin>(C<Op>(op),&+b, &d |= d!=b, &+b)) 
            InstBin* i = new InstBin(op,a.m_value,d.m_value,b.m_value);
            //delete x1; delete x2;
            x1 = i;
            x2 = p != q ? *p++ : nullptr;
            break;
        Case(C<InstMove>(&a,&b), C<InstBin>(C<Op>(op),&c |= c != b ,&d |= d==b, &+b)) 
            InstBin* i = new InstBin(op,c.m_value,a.m_value,b.m_value);
            //delete x1; delete x2;
            x1 = i;
            x2 = p != q ? *p++ : nullptr;
            break;
        Case(C<InstMove>(&a,&b), C<InstBin>(C<Op>(op),&+b,&d |= d==b, &+b)) 
            InstBin* i = new InstBin(op,a.m_value,a.m_value,b.m_value);
            //delete x1; delete x2;
            x1 = i;
            x2 = p != q ? *p++ : nullptr;
            break;
        Case(C<InstUn>(C<Op>(op),&a,&b), C<InstMove>(&+b,&d))
            renameRegsUn(op,a,b,d,x1,x2,p,q);
            break;
        Case(C<InstBin>(C<Op>(op),&a,&b,&c), C<InstMove>(&+c,&d))
            renameRegsBin(op,a,b,c,d,x1,x2,p,q);
            break;
        Otherwise()
            *r++ = x1;
            x1   = x2;
            x2 = p != q ? *p++ : nullptr;
            break;
        }
        EndMatch
    } while (p != q);

    *r++ = x1;

    return r;
}

//------------------------------------------------------------------------------

//peep1 (x1:xs) = case x1 of
//    InstMove a b | a == b -> peep1 xs
//                 | otherwise -> if doRename
//                                   then x1:(renameRegs src dst xs)
//                                   else x1:(peep1 xs)
//                                where (doRename, src, dst) = chooseReg a b
//                                      rename reg = if reg == src
//                                                      then dst
//                                                      else reg
//    _ -> x1:(peep1 xs)

//peep1 [] = []

instruction_stream_type::iterator peep1(
    instruction_stream_type::iterator& p, 
    instruction_stream_type::iterator& q,
    instruction_stream_type::iterator& r)
{
    var<Reg&> a, b;
    Inst *x;

    while (p != q)
    {
        x = *p++;

        Match(*x)
        {
        Case(C<InstMove>(&a, &+a))
            //delete x;
            break;
        Case(C<InstMove>(_, _))
            Reg *src;
            Reg *dst;
            bool doRename = chooseReg(a, b, src, dst);

            if (doRename) {
                renameRegs(*src, *dst, p, q);
            }

            *r++ = x;

            break;

        Otherwise()
            *r++ = x;
            break;
        }
        EndMatch
    }

    return r;
}

//------------------------------------------------------------------------------
