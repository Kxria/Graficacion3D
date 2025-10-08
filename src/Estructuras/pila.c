#include <stdio.h>
#include <stdlib.h>

#include "pila.h"

void push(Pila *pila, void *vector) {
    Nodo *aux = crearNodo(vector);

    aux -> sig = pila -> tope;
    pila -> tope = aux;
    pila -> cantidad++;
}

void* pop(Pila *pila) {
    Nodo *aux = pila->tope;
    pila->tope = aux->sig;
    void *dato = aux->dato;

    free(aux);
    pila->cantidad--;
    return dato;
}

void freePila(Pila* pila) {
    Nodo* aux;

    while (pila && pila->tope != NULL) {
        aux = pila->tope;
        pila->tope = aux->sig;
        free(aux);
    }
}
