%{ 
#include <stdlib.h>
#include "c.hpp"

void yyerror(Pgm *&pgm, const char *s);
int yylex();
%}

%union 
{
	int   i;
	float f;
	char *s;

	Pgm      *p;
	Decl     *d;
	DeclVar  *dv;
	DeclEnum *de;
	DeclList *dd;

	Type *t;
	Expr *e;
	ExprList *ee;

	Stmt     *m;
	StmtList *mm;

	VarList  *vv;
}

%type <p> pgm
%type <d> d
%type <dv> av
%type <de> de
%type <dd> dd

%type <t> ni t
%type <e> e
%type <m> s ev
%type <mm> ss
%type <vv> a ae

%type <ee> ee eee

%token VAR FUN ENUM CLASS
%token IF ELSE WHILE FOR IN
%token CAST 
%token NEW
%token <i> NUM 
%token <s> ID STR


//%define api.pure
%define parse.trace
%parse-param {Pgm *&ret}

%token INC "++"
%token DEC "--"

%token LS "<<"
%token RS ">>"

%token EQ "=="
%token NE "!="
%token GE ">="
%token LE "<="
%token AA "&&"
%token OO "||"

%right '='
%left "||"
%left "&&"
%left '|'
%left '^'
%left '&'
%left "!=" "=="
%left ">=" "<=" '>' '<'
%left ">>" "<<"
%left '+' '-'
%left '*' '/' '%'
%left '!' '~' "++" "--" PRE UMINUS
%left '.' '[' '(' POST

%expect 1

%%

pgm
	:                        { ret = $$ = new Pgm(); }
	| pgm d                  { $$ = $1->add($2); }
	;

d
	: FUN ID '(' ae ')' ':' t '{' ss '}' { $$ = new DeclFun($2, $4, $7, $9); }
	| FUN ID '(' ae ')' ':' t ';'        { $$ = new DeclFun($2, $4, $7, NULL); }
	| VAR ID '=' e ';'                   { $$ = new DeclVar($2, NULL, $4); }
	| VAR ID ':' t ';'                   { $$ = new DeclVar($2, $4, NULL); }
	| VAR ID ':' t '=' e ';'             { $$ = new DeclVar($2, $4, $6); }
	| ENUM ID '{' de '}'                 { $$ = $4->setName($2); }
	| CLASS ID '{' dd '}'                { $$ = new DeclClass($2, $4); }
	;

dd
	:                                    { $$ = new DeclList(); }
	| dd d                               { $$ = $1->add($2); }
	;

de
	: ID                                { $$ = new DeclEnum(); $$->add($1); }
	| de ',' ID                         { $$ = $1->add($3); }
	;

ae
	:                         { $$ = new VarList(); }
	| a                       { $$ = $1; }
	;

a
	: av                      { $$ = new VarList(); $$->add($1); }
	| a ',' av                { $$ = $1->add($3); }
	;

av
	: ID '=' e                { $$ = new DeclVar($1, NULL, $3); }
	| ID ':' t                { $$ = new DeclVar($1, $3, NULL); }
	| ID ':' t '=' e          { $$ = new DeclVar($1, $3, $5); }
	;

s
	: VAR ID '=' e ';'             { $$ = new StmtDecl(new DeclVar($2, NULL, $4)); }
	| VAR ID ':' t ';'             { $$ = new StmtDecl(new DeclVar($2, $4, NULL)); }
	| VAR ID ':' t '=' e ';'       { $$ = new StmtDecl(new DeclVar($2, $4, $6)); }
	| e ';'                        { $$ = new StmtExpr($1); }
	| IF '(' ev ')' s              { $$ = new StmtIf($3, $5, NULL); }
	| IF '(' ev ')' s ELSE s       { $$ = new StmtIf($3, $5, $7); }
	| FOR '(' ev ';' e ';' e ')' s { $$ = new StmtFor($3, $5, $7, $9); }
	//| FOR '(' ev IN e ')' s        { $$ = new StmtForEach($3, $5, $7); }
	| WHILE '(' ev ')' s           { $$ = new StmtWhile($3, $5); }
	| '{' ss '}'                   { $$ = $2; }
	| ';'                          { $$ = new StmtExpr(NULL); }
	;

ss 
	:                          { $$ = new StmtList(); }
	| ss s                     { $$ = $1->add($2); }
	;

ev
	: e                          { $$ = new StmtExpr($1); }
	| VAR ID ':' t '=' e         { $$ = new StmtDecl(new DeclVar($2, $4, $6)); }
	| VAR ID '=' e               { $$ = new StmtDecl(new DeclVar($2, NULL, $4)); }
	//| VAR ID                     { $$ = new StmtDecl(new DeclVar($2, NULL, NULL)); }
	;

e
	: NUM                       { $$ = new ExprNum($1); }
	| ID                        { $$ = new ExprId($1); }
	| STR                       { $$ = new ExprStr($1); }
	| e '=' e                   { $$ = new ExprBin($1, "=", $3); }
	| e '+' e                   { $$ = new ExprBin($1, "+", $3); }
	| e '-' e                   { $$ = new ExprBin($1, "-", $3); }
	| e '*' e                   { $$ = new ExprBin($1, "*", $3); }
	| e '/' e                   { $$ = new ExprBin($1, "/", $3); }
	| e '%' e                   { $$ = new ExprBin($1, "%", $3); }
	| e '<' e                   { $$ = new ExprBin($1, "<", $3); }
	| e "<=" e                  { $$ = new ExprBin($1, "<=", $3); }
	| e '>' e                   { $$ = new ExprBin($1, ">", $3); }
	| e ">=" e                  { $$ = new ExprBin($1, ">=", $3); }
	| e "!=" e                  { $$ = new ExprBin($1, "!=", $3); }
	| e "==" e                  { $$ = new ExprBin($1, "==", $3); }
	| e "<<" e                  { $$ = new ExprBin($1, "<<", $3); }
	| e ">>" e                  { $$ = new ExprBin($1, ">>", $3); }
	| e "&&" e                  { $$ = new ExprBin($1, "&&", $3); }
	| e "||" e                  { $$ = new ExprBin($1, "||", $3); }
	| CAST '(' t ',' e ')'      { $$ = new ExprCast($3, $5); }
	| '(' e ')'                 { $$ = $2; }
	| "++" e  %prec PRE         { $$ = new ExprPre("++", $2); }
	| "--" e  %prec PRE         { $$ = new ExprPre("--", $2); }
	| e "++"  %prec POST        { $$ = new ExprPost($1, "++"); }
	| e "--"  %prec POST        { $$ = new ExprPost($1, "--"); }
	| e '.' e                   { $$ = new ExprDot($1, $3); }
	| e '[' ee ']'              { $$ = new ExprArray($1, $3); }
	| e '(' eee ')'             { $$ = new ExprCall($1, $3); }
	| '+' e  %prec UMINUS       { $$ = new ExprUni("+", $2); }
	| '-' e  %prec UMINUS       { $$ = new ExprUni("-", $2); }
	| '!' e                     { $$ = new ExprUni("!", $2); }
	| e '|' e                   { $$ = new ExprBin($1, "|", $3); }
	| e '&' e                   { $$ = new ExprBin($1, "&", $3); }
	| e '^' e                   { $$ = new ExprBin($1, "^", $3); }
	| '&' e                     { $$ = new ExprUni("&", $2); }
	| '~' e                     { $$ = new ExprUni("~", $2); }
	| NEW t '(' eee ')'         { $$ = new ExprNew($2, $4); } 
	| NEW t '[' ee ']'          { $$ = new ExprNewArray($2, $4); } 
	;

eee
	:                           { $$ = new ExprList(); }
	| ee                        { $$ = $1; }
	;

ee
	: e                         { $$ = new ExprList(); $$->add($1); }
	| ee ',' e                  { $$ = $1->add($3); }
	;

ni
	: ID             { $$ = new TypeId($1); }
	| ni '.' ID      { $$ = new TypeDot($1, $3); }
	;

t
	: ni          { $$ = $1; }
	| t '[' ']'   { $$ = new TypeArray($1); }
	;
