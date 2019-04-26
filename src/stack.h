//
// Created by resul on 26.04.19.
//

#ifndef DROGI_STACK_H
#define DROGI_STACK_H

#include "map.h"

typedef struct Stack Stack;

struct Stack {
  City *city;
  Stack *next;
};

City *popStack(Stack **s);
void addStack(Stack **s, City *city);
bool isEmptyStack(Stack *s);

#endif //DROGI_STACK_H
