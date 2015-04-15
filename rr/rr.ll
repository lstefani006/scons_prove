%option yylineno

%top{
	#include <stdint.h>
	#include <stdlib.h>
	#include <stdio.h>
	#include <inttypes.h>
	#include <string>
	#include <vector>
	#include <stack>

	#include "c.hpp"
	#include "rr_tab.hpp"
	
	std::stack<std::string> filename;

	extern int yylineno;

	void error(const char *msg) {
		fprintf(stderr, "%s(%d): %s\n", filename.top().c_str(), yylineno, msg);
		exit(1);
	}

}
%%
":"    { return ':'; }
";"    { return ';'; }
"+"    { return '+'; }
"-"    { return '-'; }
"*"    { return '*'; }
"/"    { return '/'; }
"%"    { return '%'; }
"!"    { return '!'; }
"&"    { return '&'; }
"|"    { return '|'; }
"~"    { return '~'; }
"="    { return '='; }
"("    { return '('; }
")"    { return ')'; }
"["    { return '['; }
"]"    { return ']'; }
"{"    { return '{'; }
"}"    { return '}'; }
","    { return ','; }
"."    { return '.'; }
"<"    { return '<'; }
">"    { return '>'; }
"<="   { return LE; }
">="   { return GE; }
"=="   { return EQ; }
"!="   { return NE; }
"&&"   { return AA; }
"||"   { return OO; }
"<<"   { return LS; }
">>"   { return RS; }
"++"   { return INC; }
"--"   { return DEC; }
"var"  { return VAR; }
"fun"  { return FUN; }
"cast" { return CAST; }
"new"  { return NEW; }
if     { return IF; }  
else   { return ELSE; }  
for    { return FOR; }  
class  { return CLASS; }  
enum   { return ENUM; }  
in     { return IN; }  
[0-9]+ { yylval.i = atoi(yytext); return NUM; }
[_a-zA-Z][_a-zA-Z0-9]* { yylval.s = strdup(yytext); return ID; }
[ |\t|\n|\r] {}
"//".*   {}
\"[^\n\"]*\" { char *p = yytext + 1; *strrchr(p, '"') = 0; yylval.s = strdup(p); return STR; }
.      { error("invalid token"); }
