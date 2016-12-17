
#include "operation.h"

/* UNION OPERATION: concatenation of the results of two operations with the same columns */

typedef struct {
  operation_t* operation1;
	operation_t* operation2;
	int flag;
} operation_union_args_t;

void
operation_union_reset(void* vargs) {
    operation_union_args_t* args = vargs;
    args->flag = 0;
    operation_reset(args->operation1);
    operation_reset(args->operation2);
	return;
}

int operation_union_next(void* vargs) {
	operation_union_args_t* args = vargs;
  operation_t* operation1 = args->operation1;
	operation_t* operation2 = args->operation2;
  int ret;

  if(args->flag == 0){
	   ret = operation_next(operation1);
	   if(ret!=0){
       return ret;
     }
  }

  args->flag = 1;
  if(ret = operation_next(operation2)){
		    return ret;
	  }

  return 0;
}

void* operation_union_get(int col, void* vargs) {
  operation_union_args_t* args = vargs;
	operation_t* operation1 = args->operation1;
	operation_t* operation2 = args->operation2;

  if(args->flag == 0)
		 return operation_get(col, operation1);

	if(args->flag == 1)
		return  operation_get(col, operation2);

	return NULL;
}

void operation_union_close(void* vargs) {
    operation_union_args_t* args = vargs;

    operation_close(args->operation1);
    operation_close(args->operation2);
    free(args);
	return;
}

operation_t*
operation_union_create(operation_t* operation1, operation_t* operation2) {
    operation_t* operation;
    operation_union_args_t* args;

	if(operation1->ncols != operation2->ncols) return NULL;

    args = malloc(sizeof(operation_union_args_t));
    args->operation1 = operation1;
    args->operation2 = operation2;
	  args->flag = 0;

    operation = malloc(sizeof(operation_t));
    operation->args = args;
    operation->reset = operation_union_reset;
    operation->next = operation_union_next;
    operation->get = operation_union_get;
    operation->close = operation_union_close;
    operation->ncols = operation_ncols(operation1);
    operation->types = malloc(operation->ncols * sizeof(type_t));
    memcpy(operation->types, operation1->types, operation1->ncols * sizeof(type_t));

    return operation;
}
