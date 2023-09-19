#pragma once

extern "C" {
#include <mpc.h>
}

enum { LVAL_ERR, LVAL_NUM, LVAL_SYM, LVAL_SEXPR };
enum { LERR_DIV_ZERO, LERR_BAD_OP, LERR_BAD_NUM };

/* LISP Value */
typedef struct lval {
	int type;
	long num;
	/* Error and Symbol types have some string data */
	char* err;
	char* sym;
	/* Count and Pointer to a list of "lval*" */
	int count;
	struct lval** cell;
} lval;

/*
** LISP values constructors
*/

lval* lval_num(long x);
lval* lval_err(const char* m);
lval* lval_sym(const char* s);
lval* lval_sexpr(void);

/*
** LISP values operations
*/

void lval_del(lval* v);
lval* lval_add(lval* v, lval* x);

/*
** LISP values readers
*/

lval* lval_read_num(mpc_ast_t* t);
lval* lval_read(mpc_ast_t* t);

/*
** LISP values printing utilities
*/

void lval_print(lval* v);
void lval_println(lval* v);
void lval_expr_print(lval* v, char open, char close);

/*
** Evalulation
*/

lval* lval_eval(lval* v);
lval* lval_eval_sexpr(lval* v);
lval* lval_pop(lval* v, int i);
lval* lval_take(lval* v, int i);

lval* builtin_op(lval* a, char* op);