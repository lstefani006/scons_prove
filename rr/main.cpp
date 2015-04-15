#include <mcheck.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <string>
#include <vector>
#include <stack>

#include "c.hpp"
#include "rr_tab.hpp"


#ifdef __cplusplus
extern "C" {
#endif
	int yywrap() { return 1; }
#ifdef __cplusplus
}
#endif

extern int yylineno;
extern FILE *yyin;
extern std::stack<std::string> filename;

int yylex_destroy();

int yyerror(Pgm *&pgm, const char *msg) 
{
	fprintf(stderr, "%d: %s\n", yylineno, msg);
	exit(1);
}

struct Location {
	Location(const char *fn, int line) : fn(fn), line(line) {}
	std::string fn;
	int line;
};

bool compile(const char *fn) {

	FILE *f = fopen(fn, "r");
	if (f == NULL) {
		fprintf(stderr, "Can't open file %s!", fn);
		return false;
	}
	yyin = f;
	filename.push(fn);
	yylineno = 1;
	
	try {
		Pgm *p = NULL;
		yyparse(p);
		delete p;
	}
	catch (...) {
		fclose(f);
		filename.pop();
		yyin = NULL;
		return false;
	}
	
	fclose(f);
	filename.pop();
	yyin = NULL;

	yylex_destroy();

	return true;
}

int main(int ac, char *av[])
{
	yydebug = 0;
	
	for (int i = 1; i < ac; ++i)
		compile(av[i]);

	return 0;
}
