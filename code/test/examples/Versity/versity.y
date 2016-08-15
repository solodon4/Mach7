/*
	Grammar for Versity
*/

%{
#include "versity-defs.hpp"
#include <iostream>

using namespace std;

int  yyparse(void);
int  yylex(void);
void yyerror(const char *str)
{
    extern int g_line;
    extern char* yytext;
    cerr << "error[" << g_line << "]: " << str << endl;
    cerr << "before symbol: " << yytext << endl;
}

std::vector<Inst*>* instruction_stream;

%}

/*----------------------------------------------------------------------------*/

%union
{
    std::string*        m_str;
    double              m_flt;
    int                 m_int;
    TyPrim*             m_ty_prim;
    Val*                m_val;
    Reg*                m_reg;
    Inst*               m_inst;
    std::vector<Inst*>* m_instructions;
}

/*----------------------------------------------------------------------------*/

%start program

%token xOp
%token xInstBin 
%token xInstUn  
%token xInstMove
%token xExt
%token xTmp
%token xLit
%token xTyChar   
%token xTyUChar  
%token xTyShort  
%token xTyUShort 
%token xTyInt    
%token xTyUInt   
%token xTyLong   
%token xTyULong  
%token xTyLLong  
%token xTyULLong 
%token xTyFloat  
%token xTyDouble 
%token xTyCharN  
%token xTyUCharN 
%token xTyShortN 
%token xTyUShortN
%token xTyIntN   
%token xTyUIntN  
%token xTyLongN  
%token xTyULongN 
%token xTyLLongN 
%token xTyULLongN
%token xTyFloatN 
%token xTyDoubleN
%token xValDouble 
%token xValFloat  
%token xValChar   
%token xValUChar  
%token xValShort  
%token xValUShort 
%token xValInt    
%token xValUInt   
%token xValLong   
%token xValULong  
%token xValLLong  
%token xValULLong 
%token xValDoubleN
%token xValFloatN 
%token xValCharN  
%token xValUCharN 
%token xValShortN 
%token xValUShortN
%token xValIntN   
%token xValUIntN  
%token xValLongN  
%token xValULongN 
%token xValLLongN 
%token xValULLongN

%token <m_str> xString
%token <m_flt> xFloat
%token <m_int> xChar xInt

/*----------------------------------------------------------------------------*/

%type <m_instructions> instructions
%type <m_inst>         inst
%type <m_str>          op
%type <m_reg>          reg
%type <m_ty_prim>      ty_prim
%type <m_val>          val

/*----------------------------------------------------------------------------*/

%printer    { debug_stream() << *$$; } xString
%destructor { delete $$; } xString

%% /*=========================================================================*/

program	: '[' instructions ']'         { instruction_stream = $2; }
	;

instructions	
    : instructions ',' inst            { $$ = $1; $$->push_back($3); }
	| inst                             { $$ = new std::vector<Inst*>; $$->push_back($1); }
	;

inst
    : xInstBin  op  reg reg reg        { $$ = new InstBin (*$2,$3,$4,$5); }
    | xInstUn   op  reg reg            { $$ = new InstUn  (*$2,$3,$4); }
    | xInstMove reg reg                { $$ = new InstMove($2,$3); }
    ;

op  : '(' xOp xString ')'              { $$ = $3; }
    ;

reg 
    : '(' xExt ty_prim xString ')'     { $$ = new Ext($3,*$4); }
    | '(' xTmp ty_prim xInt ')'        { $$ = new Tmp($3,$4); }
    | '(' xLit '(' val ')' ')'         { $$ = new Lit($4);    }
    ;

ty_prim   
    : xTyChar                          { $$ = new TyChar   (); }
    | xTyUChar                         { $$ = new TyUChar  (); }
    | xTyShort                         { $$ = new TyShort  (); }
    | xTyUShort                        { $$ = new TyUShort (); }
    | xTyInt                           { $$ = new TyInt    (); }
    | xTyUInt                          { $$ = new TyUInt   (); }
    | xTyLong                          { $$ = new TyLong   (); }
    | xTyULong                         { $$ = new TyULong  (); }
    | xTyLLong                         { $$ = new TyLLong  (); }
    | xTyULLong                        { $$ = new TyULLong (); }
    | xTyFloat                         { $$ = new TyFloat  (); }
    | xTyDouble                        { $$ = new TyDouble (); }
    | xTyCharN   xInt                  { $$ = new TyCharN  ($2); }
    | xTyUCharN  xInt                  { $$ = new TyUCharN ($2); }
    | xTyShortN  xInt                  { $$ = new TyShortN ($2); }
    | xTyUShortN xInt                  { $$ = new TyUShortN($2); }
    | xTyIntN    xInt                  { $$ = new TyIntN   ($2); }
    | xTyUIntN   xInt                  { $$ = new TyUIntN  ($2); }
    | xTyLongN   xInt                  { $$ = new TyLongN  ($2); }
    | xTyULongN  xInt                  { $$ = new TyULongN ($2); }
    | xTyLLongN  xInt                  { $$ = new TyLLongN ($2); }
    | xTyULLongN xInt                  { $$ = new TyULLongN($2); }
    | xTyFloatN  xInt                  { $$ = new TyFloatN ($2); }
    | xTyDoubleN xInt                  { $$ = new TyDoubleN($2); }
    ;


val
    : xValDouble  xFloat               { $$ = new ValDouble($2); }
    | xValFloat   xFloat               { $$ = new ValFloat ($2); }
    | xValChar    xChar                { $$ = new ValChar  ($2); }
    | xValUChar   xChar                { $$ = new ValUChar ($2); }
    | xValShort   xInt                 { $$ = new ValShort ($2); }
    | xValUShort  xInt                 { $$ = new ValUShort($2); }
    | xValInt     xInt                 { $$ = new ValInt   ($2); }
    | xValUInt    xInt                 { $$ = new ValUInt  ($2); }
    | xValLong    xInt                 { $$ = new ValLong  ($2); }
    | xValULong   xInt                 { $$ = new ValULong ($2); }
    | xValLLong   xInt                 { $$ = new ValLLong ($2); }
    | xValULLong  xInt                 { $$ = new ValULLong($2); }
    | xValDoubleN '[' float_list ']'   { $$ = new ValDoubleN; }
    | xValFloatN  '[' float_list ']'   { $$ = new ValFloatN ; }
    | xValCharN   '[' char_list  ']'   { $$ = new ValCharN  ; }
    | xValUCharN  '[' char_list  ']'   { $$ = new ValUCharN ; }
    | xValShortN  '[' int_list   ']'   { $$ = new ValShortN ; }
    | xValUShortN '[' int_list   ']'   { $$ = new ValUShortN; }
    | xValIntN    '[' int_list   ']'   { $$ = new ValIntN   ; }
    | xValUIntN   '[' int_list   ']'   { $$ = new ValUIntN  ; }
    | xValLongN   '[' int_list   ']'   { $$ = new ValLongN  ; }
    | xValULongN  '[' int_list   ']'   { $$ = new ValULongN ; }
    | xValLLongN  '[' int_list   ']'   { $$ = new ValLLongN ; }
    | xValULLongN '[' int_list   ']'   { $$ = new ValULLongN; }
    ;

float_list 
    : float_list xFloat
    | /* empty */
    ;

char_list 
    : char_list xString
    | /* empty */
    ;
 
int_list   
    : int_list xInt
    | /* empty */
    ;

%% /*=========================================================================*/
