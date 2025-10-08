#include "nodo.h"
#include <stdlib.h>

Nodo* crearNodo(void* vector) {
    Nodo *nuevoNodo = calloc(1, sizeof(Nodo));
    nuevoNodo -> dato = vector;
    nuevoNodo->sig = NULL;

    return nuevoNodo;
}