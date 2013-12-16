/*
	Grammar for COU hierarchy class
*/

%{
#include "descriptor.hpp"
#include "coudefs.hpp"

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

ClassDescriptor* cur = 0;
%}

%union
{
    std::string* m_str;
    int          m_int;
}

%start cou

%token <m_str> ID "identifier"

%printer    { debug_stream() << *$$; } ID
%destructor { delete $$; } ID

%%

cou	: '(' classes ')'
	;

classes	
    : classes class
	| class
	;

class : '(' ID { cur = get_class(*$2); } '(' bases ')' '(' attributes ')' '(' methods ')' ')' { cur->finalize(); }
    ;

bases : bases ID           { cur->add_base(get_class(*$2)); }
    | /* empty */
    ;

attributes : attributes ID { cur->add_attr(*$2); }
    | /* empty */
    ;

methods 
    : methods ID           { cur->add_mthd(*$2); }
    | methods operator
    | /* empty */
    ;

operator
    : '+'
    | '-'
    | '*'
    | '/'
    | '%'
    | '^'
    | '@'
    | '$'
    | '~'
    | '!'
    | '|'
    | '&'
    | '&''&'
    | '|''|'
    | '='
    | '<'
    | '>'
    | '<''='
    | '>''='
    | '=''='
    | '!''='
    | '\\''\\'
    ;

%%
