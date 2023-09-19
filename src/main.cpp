extern "C" {
#include <mpc.h>
}
#include "lisp.h"

#include <cstdio>
#include <cstdlib>

#ifdef _WIN32
#include <cstring>
const int INPUT_BUFFER_SIZE = 2048;
static char buffer[INPUT_BUFFER_SIZE];

char* readline(const char* prompt)
{
	fputs(prompt, stdout);
	fgets(buffer, INPUT_BUFFER_SIZE, stdin);
	char* cpy = (char*) malloc(strlen(buffer) + 1);
	strcpy(cpy, buffer);
	cpy[strlen(cpy) - 1] = '\0';
	return cpy;
}

void add_history(char* unused) {}

#else
#include <editline/readline.h>
#include <editline/history.h>
#endif


int main(int argc, char** argv)
{
	mpc_parser_t* Number = mpc_new("number");
	mpc_parser_t* Symbol = mpc_new("symbol");
	mpc_parser_t* Sexpr = mpc_new("sexpr");
	mpc_parser_t* Expr = mpc_new("expr");
	mpc_parser_t* Obrspis = mpc_new("obrspis");

	mpca_lang(MPCA_LANG_DEFAULT,
		"                                               \
			number   : /[+-]?([0-9]*[.])?[0-9]+/ ;      \
			symbol   : '+' | '-' | '*' | '/' | '%' ;    \
			sexpr    : '(' <expr>* ')' ;				\
			expr     : <number> | <symbol> | <sexpr> ;  \
			obrspis    : /^/ <expr>* /$/ ;     \
		",
		Number, Symbol, Sexpr, Expr, Obrspis
	);

	puts("Obrpis version 0.0.1");
	puts("Press Ctrl+c to exit\n");
	
	while (1)
	{
		char* input = readline("obrspis> ");
		add_history(input);
		mpc_result_t r;
		if (mpc_parse("<stdin>", input, Obrspis, &r))
		{
			mpc_ast_t* r_output = (mpc_ast_t*) r.output;
			lval* x = lval_eval(lval_read(r_output));
			lval_println(x);
			lval_del(x);
			mpc_ast_delete(r_output);
		}
		else
		{
			mpc_err_print(r.error);
			mpc_err_delete(r.error);
		}
		free(input);
	}

	mpc_cleanup(5, Number, Symbol, Sexpr, Expr, Obrspis);
	return EXIT_SUCCESS;
}
