#include "versity-bindings.hpp"
#include "type_switchN-patterns.hpp"
#include "patterns/address.hpp"        // Address pattern
//#include "patterns/combinators.hpp"    // Pattern combinators
#include "patterns/constructor.hpp"    // Constructor pattern
#include "patterns/equivalence.hpp"    // Equivalence pattern
#include "patterns/guard.hpp"          // Guard pattern
#include "patterns/n+k.hpp"            // n+k pattern
//#include "patterns/primitive.hpp"      // Value, Variable and Wildcard patterns
//#include "patterns/quantifiers.hpp"    // Quantifiers

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

    while(p != q) {
        bool stop;

        Match(**p)
        Case(C<InstMove>(_, &b)      ) stop = (*b.m_value==src || *b.m_value==dst); break;
        Case(C<InstUn  >(_, _, &b)   ) stop = (*b.m_value==src || *b.m_value==dst); break;
        Case(C<InstBin >(_, _, _, &c)) stop = (*c.m_value==src || *c.m_value==dst); break;
        Otherwise() stop = false;
        EndMatch

        Match(**p)
        Case(C<InstMove>(&a, &b))
            Reg*  d = !stop ? rename(b) : std::move(b.m_value);
            Inst* i = new InstMove(rename(a),               d);
            delete *p;
            *p = i;
            break;
        Case(C<InstUn>(C<Op>(op), &a, &b))
            Reg*  d = !stop ? rename(b) : std::move(b.m_value);
            Inst* i = new InstUn(op, rename(a),             d);
            delete *p;
            *p = i;
            break;
        Case(C<InstBin>(C<Op>(op), &a, &b, &c))
            Reg*  d = !stop ? rename(c) : std::move(c.m_value);
            Inst* i = new InstBin(op, rename(a), rename(b), d);
            delete *p;
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
    delete x1_ref;
    x1_ref = i;

    if (doRename) {
        renameRegs(*src, *dst, p_ref, q);
    }

    x2_out = *p_ref++;
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
    Reg& a,
    Reg& b,
    Reg& d,
    Inst*& x1_ref,
    Inst*& x2_out,
    instruction_stream_type::iterator& p_ref,
    const instruction_stream_type::iterator q)
{
    std::string op;
    Reg* src;
    Reg* dst;

    bool doRename = chooseReg(b, d, src, dst);
    
    Inst* i = new InstUn(op, &a, dst);
    delete x1_ref;
    x1_ref = i;

    if (doRename) {
        renameRegs(*src, *dst, p_ref, q);
    }

    x2_out = *p_ref++;
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
    Reg& a,
    Reg& b,
    Reg& d,
    Reg& e,
    Inst*& x1_ref,
    Inst*& x2_out,
    instruction_stream_type::iterator& p_ref,
    const instruction_stream_type::iterator q)
{
    std::string op;
    Reg* src;
    Reg* dst;

    bool doRename = chooseReg(d, e, src, dst);
    
    Inst* i = new InstBin(op, &a, &b, dst);
    delete x1_ref;
    x1_ref = i;

    if (doRename) {
        renameRegs(*src, *dst, p_ref, q);
    }

    x2_out = *p_ref++;
}

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

void peep2(
    instruction_stream_type::iterator& p, 
    instruction_stream_type::iterator& q,
    instruction_stream_type::iterator& r)
{
    var<Reg&> a, b, c, d;
    std::string op;

    Inst* x1 = *p++; // FIX: Check == q
    Inst* x2 = *p++; // FIX: Check == q

    Match(*x1,*x2)
    {
    Case(C<InstMove>(&a,&b), C<InstMove>(&+b,&+a) ) delete x2; x2 = *p++; break;
  //Case(C<InstMove>(&a,&b), C<InstMove>(&+b,&d)  ) renameRegsMove(a,b,d,x1,x2,p,q); break;
    Case(C<InstMove>(&a,&b), C<InstUn>(C<Op>(op),&+b,&+b) ) 
        InstUn* i = new InstUn(op,std::move(a.m_value),std::move(b.m_value)); 
        delete x1; delete x2; 
        x1 = i;
        x2 = *p++; 
        break;
    Case(C<InstMove>(&a,&b), C<InstBin>(C<Op>(op), &+b, &d |= d!=b, &+b) ) 
        InstBin* i = new InstBin(op,std::move(a.m_value),std::move(d.m_value),std::move(b.m_value));
        delete x1; delete x2;
        x1 = i;
        x2 = *p++; 
        break;
    }
    EndMatch
}

//------------------------------------------------------------------------------

void peep1(
    instruction_stream_type::const_iterator& p, 
    instruction_stream_type::const_iterator& q,
    instruction_stream_type::iterator&       r)
{
    var<const Reg&> a;

    while (p != q)
    {
        Match(**p++)
        {
        Case(C<InstMove>(&a, &+a)) break;
        Otherwise() *r++ = &match0;
        }
        EndMatch
    }
}

//------------------------------------------------------------------------------
