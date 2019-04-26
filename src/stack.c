#include "stack.h"
#include <stdlib.h>

City *popStack(Stack **s) {
  City *ret = (*s)->city;
  Stack *aux = *s;
  (*s) = (*s)->next;
  free(aux);
  return ret;
}

void addStack(Stack **s, City *city) {
  Stack *aux = malloc(sizeof(Stack));
  aux->city = city;
  aux->next = *s;
  (*s) = aux;
}

bool isEmptyStack(Stack *s) {
  return s == NULL;
}