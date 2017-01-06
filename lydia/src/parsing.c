#include <stdio.h>
#include <stdlib.h>

#include "mpc.h"

/* If we are compiling on Windows compile these functions */
#ifdef _WIN32
#include <string.h>

static char buffer[2048];

/* Fake readline function */
char* readline(char* prompt) {
  fputs(prompt, stdout);
  fgets(buffer, 2048, stdin);
  char* cpy = malloc(strlen(buffer)+1);
  strcpy(cpy, buffer);
  cpy[strlen(cpy)-1] = '\0';
  return cpy;
}

/* Fake add_history function */
void add_history(char* unused) {}

/* Otherwise include the editline headers */
#else
#include <editline/readline.h>
// #include <editline/history.h>
#include "histedit.h"
#endif

int number_of_nodes(mpc_ast_t* tree)
{
  if (tree->children_num == 0) { return 1; }
  if (tree->children_num >= 1)
  {
    int total = 1;
    for (int i = 0; i < tree->children_num; i++)
    {
      total = total + number_of_nodes(tree->children[i]);
    }
    return total;
  }
  return 0;
}

int is_add(char* op)
{
  if (strcmp(op, "+") == 0 || strcmp(op, "add") == 0) return 1;
  return 0;
}

int is_sub(char* op)
{
  if (strcmp(op, "-") == 0 || strcmp(op, "sub") == 0) return 1;
  return 0;
}

int is_mul(char* op)
{
  if (strcmp(op, "*") == 0 || strcmp(op, "mul") == 0) return 1;
  return 0;
}

int is_div(char* op)
{
  if (strcmp(op, "/") == 0 || strcmp(op, "div") == 0) return 1;
  return 0;
}

int is_mod(char* op)
{
  if (strcmp(op, "%") == 0 || strcmp(op, "mod") == 0) return 1;
  return 0;
}

int is_pow(char* op)
{
  if (strcmp(op, "^") == 0 || strcmp(op, "pow") == 0) return 1;
  return 0;
}

long pow_long(long x, long y)
{
  int result = 1;

  while (y){
    if (y & 1){
      result *= x;
    }
    y >>= 1;
    x *= x;
  }

  return result;
}

long eval_op(long x, char* op, long y)
{
  if (is_add(op)) { return x + y; }
  if (is_sub(op)) { return x - y; }
  if (is_mul(op)) { return x * y; }
  if (is_div(op)) { return x / y; }
  if (is_mod(op)) { return x % y; }
  if (is_pow(op)) { return pow_long(x, y); }
  return 0;
}

long eval(mpc_ast_t* tree)
{
  if (strstr(tree->tag, "number"))
  {
    return atoi(tree->contents);
  }

  /* the operator is always the second child */
  char* op = tree->children[1]->contents;

  /* Store the third child in `x` */
  long x = eval(tree->children[2]);

  /* Iterate the remaining children and combining. */
  int i = 3;
  while (strstr(tree->children[i]->tag, "expr"))
  {
    x = eval_op(x, op, eval(tree->children[i]));
    i++;
  }

  /* Negate x if the operator is_sub and there are no additional children */
  if (is_sub(op) && i == 3)
  {
    x = -x;
  }

  return x;
}

int main(int argc, char** argv) {
  mpc_parser_t* Number   = mpc_new("number");
  mpc_parser_t* Operator = mpc_new("operator");
  mpc_parser_t* Expr     = mpc_new("expr");
  mpc_parser_t* Lydia    = mpc_new("lydia");

  mpca_lang(MPCA_LANG_DEFAULT,
    "                                                                     \
      number    : /-?[0-9]+/ ;                                            \
      operator  : '+' | '-' | '*' | '/' | '%' | '^' | /(add|sub|mul|div|mod|pow)/ ; \
      expr      : <number> | '(' <operator> <expr>+ ')' ;                 \
      lydia     : /^/ <operator> <expr>+ /$/ ;                            \
    ",
    Number, Operator, Expr, Lydia);

  puts("Lydia Version 0.0.1");
  puts("Press Ctrl+c to Exit\n");
   
  while (1)
  {
    /* Now in either case readline will be correctly defined */
    char* input = readline("lydia> ");
    add_history(input);

    mpc_result_t r;
    if(mpc_parse("<stdin>", input, Lydia, &r))
    {
      /* On success print the AST */
      long result = eval(r.output);
      printf("%li\n", result);
      mpc_ast_delete(r.output);
    }
    else
    {
      /* Otherwise print error */
      mpc_err_print(r.error);
      mpc_err_delete(r.error);
    }

    free(input);
  }
  
  /* Always clean up before returning */
  mpc_cleanup(4, Number, Operator, Expr, Lydia);
  return 0;
}

