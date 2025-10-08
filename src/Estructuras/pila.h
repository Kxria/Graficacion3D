#pragma once

#include "nodo.h"

typedef struct _pila{
    Nodo *tope;
    int cantidad;
}Pila;

void push(Pila *pila, void *vector);
void* pop(Pila *pila);
void freePila(Pila* pila);