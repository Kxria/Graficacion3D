#pragma once

typedef struct _nodo{
    void *dato;
    struct _nodo *sig;
}Nodo;

Nodo* crearNodo(void* vector);