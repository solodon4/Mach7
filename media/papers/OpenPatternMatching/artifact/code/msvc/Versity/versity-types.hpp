#pragma once

#include <ostream>
#include <string>
#include <vector>

typedef               double CDouble;
typedef               float  CFloat;
typedef               char   CChar;
typedef unsigned      char   CUChar;
typedef               short  CShort;
typedef unsigned      short  CUShort;
typedef               int    CInt;
typedef unsigned      int    CUInt;
typedef               long   CLong;
typedef unsigned      long   CULong;
typedef          long long   CLLong;
typedef unsigned long long   CULLong;

struct Op  { std::string name; Op(const std::string& s) : name(s) {} };

struct TyPrim { virtual ~TyPrim(){} };
struct TyChar    : TyPrim { };
struct TyUChar   : TyPrim { };
struct TyShort   : TyPrim { };
struct TyUShort  : TyPrim { };
struct TyInt     : TyPrim { };
struct TyUInt    : TyPrim { };
struct TyLong    : TyPrim { };
struct TyULong   : TyPrim { };
struct TyLLong   : TyPrim { };
struct TyULLong  : TyPrim { };
struct TyFloat   : TyPrim { };
struct TyDouble  : TyPrim { };
struct TyCharN   : TyPrim { int size; TyCharN  (int n) : size(n) {} };
struct TyUCharN  : TyPrim { int size; TyUCharN (int n) : size(n) {} };
struct TyShortN  : TyPrim { int size; TyShortN (int n) : size(n) {} };
struct TyUShortN : TyPrim { int size; TyUShortN(int n) : size(n) {} };
struct TyIntN    : TyPrim { int size; TyIntN   (int n) : size(n) {} };
struct TyUIntN   : TyPrim { int size; TyUIntN  (int n) : size(n) {} };
struct TyLongN   : TyPrim { int size; TyLongN  (int n) : size(n) {} };
struct TyULongN  : TyPrim { int size; TyULongN (int n) : size(n) {} };
struct TyLLongN  : TyPrim { int size; TyLLongN (int n) : size(n) {} };
struct TyULLongN : TyPrim { int size; TyULLongN(int n) : size(n) {} };
struct TyFloatN  : TyPrim { int size; TyFloatN (int n) : size(n) {} };
struct TyDoubleN : TyPrim { int size; TyDoubleN(int n) : size(n) {} };

struct Val  { virtual ~Val(){} };
struct ValDouble  : Val { CDouble              value; ValDouble(CDouble v) : value(v) {} };
struct ValFloat   : Val { CFloat               value; ValFloat (CFloat  v) : value(v) {} };
struct ValChar    : Val { CChar                value; ValChar  (CChar   v) : value(v) {} };
struct ValUChar   : Val { CUChar               value; ValUChar (CUChar  v) : value(v) {} };
struct ValShort   : Val { CShort               value; ValShort (CShort  v) : value(v) {} };
struct ValUShort  : Val { CUShort              value; ValUShort(CUShort v) : value(v) {} };
struct ValInt     : Val { CInt                 value; ValInt   (CInt    v) : value(v) {} };
struct ValUInt    : Val { CUInt                value; ValUInt  (CUInt   v) : value(v) {} };
struct ValLong    : Val { CLong                value; ValLong  (CLong   v) : value(v) {} };
struct ValULong   : Val { CULong               value; ValULong (CULong  v) : value(v) {} };
struct ValLLong   : Val { CLLong               value; ValLLong (CLLong  v) : value(v) {} };
struct ValULLong  : Val { CULLong              value; ValULLong(CULLong v) : value(v) {} };
struct ValDoubleN : Val { std::vector<CDouble> value; };
struct ValFloatN  : Val { std::vector<CFloat>  value; };
struct ValCharN   : Val { std::vector<CChar>   value; };
struct ValUCharN  : Val { std::vector<CUChar>  value; };
struct ValShortN  : Val { std::vector<CShort>  value; };
struct ValUShortN : Val { std::vector<CUShort> value; };
struct ValIntN    : Val { std::vector<CInt>    value; };
struct ValUIntN   : Val { std::vector<CUInt>   value; };
struct ValLongN   : Val { std::vector<CLong>   value; };
struct ValULongN  : Val { std::vector<CULong>  value; };
struct ValLLongN  : Val { std::vector<CLLong>  value; };
struct ValULLongN : Val { std::vector<CULLong> value; };

struct Reg { virtual ~Reg(){} };
struct Ext : Reg { TyPrim* type; std::string name; Ext(TyPrim* t, const std::string& n) : type(t), name(n) {} };
struct Tmp : Reg { TyPrim* type; int         name; Tmp(TyPrim* t, int n)                : type(t), name(n) {} };
struct Lit : Reg { Val*    value;                  Lit(Val*    v)                       : value(v)         {} };

struct Inst { virtual ~Inst(){} };
struct InstBin      : Inst { Op operation; Reg* arg1; Reg* arg2; Reg* dest; InstBin (const std::string& op, Reg* a1, Reg* a2, Reg* d) : operation(op), arg1(a1), arg2(a2), dest(d) {} };
struct InstUn       : Inst { Op operation; Reg* arg1; Reg* dest;            InstUn  (const std::string& op, Reg* a1, Reg* d) : operation(op), arg1(a1), dest(d) {} };
struct InstMove     : Inst { Reg* src; Reg* dest;                           InstMove(Reg* s, Reg* d) : src(s), dest(d) {} };

typedef std::vector<Inst*> instruction_stream_type;

std::ostream& operator<<(std::ostream&, const TyPrim&);
std::ostream& operator<<(std::ostream&, const Val&);
std::ostream& operator<<(std::ostream&, const Reg&);
std::ostream& operator<<(std::ostream&, const Inst&);

bool operator==(const TyPrim&, const TyPrim&);
bool operator==(const    Val&, const    Val&);
bool operator==(const    Reg&, const    Reg&);

inline bool operator!=(const TyPrim& a, const TyPrim& b) { return !(a == b); }
inline bool operator!=(const    Val& a, const    Val& b) { return !(a == b); }
inline bool operator!=(const    Reg& a, const    Reg& b) { return !(a == b); }
