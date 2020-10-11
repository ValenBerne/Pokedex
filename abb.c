#include <stdio.h>
#include "abb.h"

#define ERROR -1
#define EXITO 0

int comparador(void* uno, void* dos){
    if(!uno || !dos)
        return 0;

    if(uno>dos)
        return 1;

    if(uno<dos)
        return -1;

    return 0;
}

void destructor(void* destruido){
    if(!destruido)
        return;
    free(destruido);
}

abb_t* arbol_crear(abb_comparador comparador, abb_liberar_elemento destructor){
    abb_t* arbolito =   (abb_t*)calloc(1, sizeof(abb_t));
    if(arbolito ==  NULL){
        return NULL;
    }

    arbolito->comparador =   (*comparador);
    arbolito->destructor =   (*destructor);

    return arbolito;
}

nodo_abb_t* crear_nodo_abb(void* elemento){
    nodo_abb_t* nuevo_nodo  =   calloc(1, sizeof(nodo_abb_t));
    if(nuevo_nodo   ==  NULL){
        return NULL;
    }

    nuevo_nodo->elemento    =   elemento;

    return nuevo_nodo;
}

int arbol_insertar(abb_t* arbol, void* elemento){
    if(arbol    ==  NULL    ||  elemento    ==  NULL){
        return ERROR;
    }

    if(arbol->nodo_raiz ==  NULL){
        nodo_abb_t* nodo =  crear_nodo_abb(elemento);
        if(nodo ==  NULL){
            return ERROR;
        }
        arbol->nodo_raiz    =   nodo;
        return EXITO;
    }

    nodo_abb_t* nodo =   arbol->nodo_raiz;
    nodo_abb_t* padre   =   NULL;
    int posicion    =   0;
    while(nodo  !=  NULL){
        if(arbol->comparador(nodo->elemento, elemento)  ==  1   ||  arbol->comparador(nodo->elemento, elemento)  ==  0){
            posicion    =   1;
            padre   =   nodo;
            nodo    =   nodo->izquierda;
        }
        else if(arbol->comparador(nodo->elemento, elemento)  ==  -1){
            posicion    =   -1;
            padre   =   nodo;
            nodo    =   nodo->derecha;
        }
    }
    if(posicion ==  1){
        nodo    =   crear_nodo_abb(elemento);
        if(nodo ==  NULL){
            return ERROR;
        }
        padre->izquierda  =   nodo;
        return EXITO;
    }
    else if(posicion    ==  -1){
        nodo    =   crear_nodo_abb(elemento);
        if(nodo ==  NULL){
            return ERROR;
        }
        padre->derecha  =   nodo;
        return EXITO;
    }
    return ERROR;
}

int cantidad_de_hijos(nodo_abb_t* nodo){
    if(nodo->izquierda  ==  NULL && nodo->derecha   ==  NULL){
        return 0;
    }
    if(nodo->izquierda  !=  NULL && nodo->derecha   !=  NULL){
        return 2;
    }
    return 1;
}

int acomodar_sin_hijos(bool   apunta_a_izquierda, nodo_abb_t* padre, nodo_abb_t* actual, abb_t* arbol){
    if(apunta_a_izquierda   ==  true    &&  padre   !=  NULL){
        padre->izquierda    =   NULL;
    }
    else if(apunta_a_izquierda   !=  true    &&  padre   !=  NULL){
        padre->derecha  =   NULL;
    }
    else{
        arbol->nodo_raiz    =   NULL;
    }
    arbol->destructor(actual->elemento);
    free(actual);
    return EXITO;
}

int acomodar_un_hijo(bool apunta_a_izquierda, nodo_abb_t* padre, nodo_abb_t* actual, abb_t* arbol){
    if(apunta_a_izquierda   ==  true    &&  padre   !=  NULL){
        if(actual->derecha  !=  NULL){
            padre->izquierda    =   actual->derecha;
        }
        else{
            padre->izquierda    =   actual->izquierda;
        }
    }
    else if(apunta_a_izquierda   !=  true    &&  padre   !=  NULL){
        if(actual->derecha  !=  NULL){
            padre->derecha    =   actual->derecha;
        }
        else{
            padre->derecha    =   actual->izquierda;
        }
    }
    else{
        if(actual->derecha  !=  NULL){
            arbol->nodo_raiz    =   actual->derecha;
        }
        else{
            arbol->nodo_raiz    =   actual->izquierda;
        }
    }

    arbol->destructor(actual->elemento);
    free(actual);
    return EXITO;
}

int acomodar_dos_hijos(bool apunta_a_izquierda, nodo_abb_t* padre, nodo_abb_t* actual, abb_t* arbol){
    if(actual->izquierda->derecha   ==  NULL){
        if(padre    !=  NULL){
            if(apunta_a_izquierda   ==  true){
                padre->izquierda    =   actual->izquierda;
                actual->izquierda->derecha  =   actual->derecha;
            }
            else{
                padre->derecha    =   actual->izquierda;
                actual->izquierda->derecha  =   actual->derecha;
            }
        }
        else{
            arbol->nodo_raiz    =   actual->izquierda;
            actual->izquierda->derecha  =   actual->derecha;
        }
        arbol->destructor(actual->elemento);
        free(actual);
        return EXITO;
    }

    nodo_abb_t* izquierda_y_despues_derecha_al_fondo    =   actual->izquierda->derecha;
    nodo_abb_t* padrino =   actual->izquierda;
    while(izquierda_y_despues_derecha_al_fondo->derecha !=  NULL){
        padrino =   izquierda_y_despues_derecha_al_fondo;
        izquierda_y_despues_derecha_al_fondo    =   izquierda_y_despues_derecha_al_fondo->derecha;
    }

    padrino->derecha    =   NULL;
    if(padre    ==  NULL){
        arbol->nodo_raiz    =   izquierda_y_despues_derecha_al_fondo;
    }
    else if(apunta_a_izquierda   ==  true){
        padre->izquierda    =   izquierda_y_despues_derecha_al_fondo;
    }
    else{
        padre->derecha    =   izquierda_y_despues_derecha_al_fondo;
    }

    izquierda_y_despues_derecha_al_fondo->izquierda =   actual->izquierda;
    izquierda_y_despues_derecha_al_fondo->derecha =   actual->derecha;

    arbol->destructor(actual->elemento);
    free(actual);
    return EXITO;
}

int borrar_recursivo(nodo_abb_t* actual, void* elemento, nodo_abb_t* padre, bool apunta_izquierda, abb_t* arbol){
    if(arbol    ==  NULL    ||  arbol->nodo_raiz ==  NULL   ||  elemento    ==  NULL    ||  actual  ==  NULL){
        return ERROR;
    }

    if(arbol->comparador(actual->elemento, elemento)    ==  1){
        return borrar_recursivo(actual->izquierda, elemento, actual, true, arbol);
    }
    else if(arbol->comparador(actual->elemento, elemento)    ==  -1){
        return borrar_recursivo(actual->derecha, elemento, actual, false, arbol);
    }
    else{
        int hijos   =   cantidad_de_hijos(actual);
        if(hijos    ==  0){
            return acomodar_sin_hijos(apunta_izquierda, padre, actual, arbol);
        }
        else if(hijos   ==  1){
            return acomodar_un_hijo(apunta_izquierda, padre, actual, arbol);
        }
        else{
            return acomodar_dos_hijos(apunta_izquierda, padre, actual, arbol);
        }
    }
}

int arbol_borrar(abb_t* arbol, void* elemento){
    if(arbol    ==  NULL    ||  arbol->nodo_raiz ==  NULL   ||  elemento    ==  NULL){
        return ERROR;
    }

    return borrar_recursivo(arbol->nodo_raiz, elemento, NULL, false, arbol);
}

void* arbol_buscar(abb_t* arbol, void* elemento){
    if(arbol    ==  NULL    ||  elemento    ==  NULL    ||  arbol->nodo_raiz    ==  NULL){
        return NULL;
    }

    nodo_abb_t*  actual  =   arbol->nodo_raiz;
    while(actual    !=  NULL    &&  arbol->comparador(actual->elemento, elemento)   !=  0){
        if(arbol->comparador(actual->elemento, elemento)  >   0){
            actual  =   actual->izquierda;
        }
        else if(arbol->comparador(actual->elemento, elemento)  <  0){
            actual  =   actual->derecha;
        }
    }

    if(actual   ==  NULL)   return NULL;
    return actual->elemento;
}

void* arbol_raiz(abb_t* arbol){
    if(arbol    ==  NULL    ||  arbol_vacio(arbol)){
        return NULL;
    }
    return arbol->nodo_raiz->elemento;
}

bool arbol_vacio(abb_t* arbol){
    if(arbol    ==  NULL    ||  arbol->nodo_raiz    ==  NULL){
        return true;
    }
    return false;
}

// Izquierda medio derecha
int arbol_recorrido_inorden1(nodo_abb_t* nodo, void** array, int tamanio_array, int indice){

    if(nodo->izquierda  !=  NULL){
        indice   =       arbol_recorrido_inorden1(nodo->izquierda, array, tamanio_array, indice);
    }

    if(indice   <   tamanio_array){
        array[indice]    =   nodo->elemento;
        indice++;
    }

    if(nodo->derecha    !=  NULL){
        indice   =       arbol_recorrido_inorden1(nodo->derecha, array, tamanio_array, indice);
    }

    return indice;
}

// Padre izquierda derecha
int arbol_recorrido_preorden1(nodo_abb_t* nodo, void** array, int tamanio_array, int indice){

    if(indice   <   tamanio_array){
        array[indice]    =   nodo->elemento;
        indice++;
    }

    if(nodo->izquierda  !=  NULL){
        indice   =       arbol_recorrido_preorden1(nodo->izquierda, array, tamanio_array, indice);
    }

    if(nodo->derecha    !=  NULL){
        indice   =       arbol_recorrido_preorden1(nodo->derecha, array, tamanio_array, indice);
    }

    return indice;
}

// Derecha izquierda medio
int arbol_recorrido_postorden1(nodo_abb_t* nodo, void** array, int tamanio_array, int indice){

    if(nodo->izquierda  !=  NULL){
        indice   =       arbol_recorrido_postorden1(nodo->izquierda, array, tamanio_array, indice);
    }

    if(nodo->derecha    !=  NULL){
        indice   =       arbol_recorrido_postorden1(nodo->derecha, array, tamanio_array, indice);
    }

    if(indice   <   tamanio_array){
        array[indice]    =   nodo->elemento;
        indice++;
    }


    return indice;
}

int arbol_recorrido_inorden(abb_t* arbol, void** array, int tamanio_array){
    if(arbol    ==  NULL    ||  arbol_vacio(arbol)  ||  arbol->nodo_raiz    ==  NULL){
        return 0;
    }

    return arbol_recorrido_inorden1(arbol->nodo_raiz, array, tamanio_array, 0);
}

int arbol_recorrido_preorden(abb_t* arbol, void** array, int tamanio_array){
    if(arbol    ==  NULL    ||  arbol_vacio(arbol)  ||  arbol->nodo_raiz    ==  NULL){
        return 0;
    }

    return arbol_recorrido_preorden1(arbol->nodo_raiz, array, tamanio_array, 0);
}

int arbol_recorrido_postorden(abb_t* arbol, void** array, int tamanio_array){
    if(arbol    ==  NULL    ||  arbol_vacio(arbol)  ||  arbol->nodo_raiz    ==  NULL){
        return 0;
    }

    return arbol_recorrido_postorden1(arbol->nodo_raiz, array, tamanio_array, 0);
}

/*void destructor_de_ramas(nodo_abb_t* nodo, abb_liberar_elemento destructor){
    if(nodo->izquierda !=  NULL){
        destructor_de_ramas(nodo->izquierda, destructor);
    }

    if(nodo->derecha !=  NULL){
        destructor_de_ramas(nodo->derecha, destructor);
    }

    destructor(nodo);
}*/

void arbol_destruir(abb_t* arbol){
    if(arbol    !=  NULL){
        /*if(arbol->nodo_raiz !=  NULL){
            destructor_de_ramas(arbol->nodo_raiz, arbol->destructor);
        }*/
        while(arbol->nodo_raiz  !=  NULL){
            arbol_borrar(arbol, arbol_raiz(arbol));
        }
        free(arbol);
    }
}

// Izquierda medio derecha
bool abb_run_inorder(nodo_abb_t* nodo, bool (*funcion)(void*, void*), void* extra, bool estado){

    if(nodo->izquierda  !=  NULL    &&  estado  ==  false){
        estado   =       abb_run_inorder(nodo->izquierda, (*funcion), extra, estado);
    }

    if(estado   ==  false){
        estado   =       (*funcion)(nodo->elemento, extra);
    }

    if(nodo->derecha    !=  NULL    &&  estado  ==  false){
        estado   =       abb_run_inorder(nodo->derecha, (*funcion), extra, estado);
    }

    return estado;
}

// Medio izquierda derecha
bool abb_run_preorden(nodo_abb_t* nodo, bool (*funcion)(void*, void*), void* extra, bool estado){

    if(estado   ==  false){
        estado   =       (*funcion)(nodo->elemento, extra);
    }

    if(nodo->izquierda  !=  NULL    &&  estado  ==  false){
        estado   =       abb_run_preorden(nodo->izquierda, (*funcion), extra, estado);
    }

    if(nodo->derecha    !=  NULL    &&  estado  ==  false){
        estado   =       abb_run_preorden(nodo->derecha, (*funcion), extra, estado);
    }

    return estado;
}

// Derecha izquierda medio
bool abb_run_postorden(nodo_abb_t* nodo, bool (*funcion)(void*, void*), void* extra, bool estado){

    if(nodo->izquierda  !=  NULL    &&  estado  ==  false){
        estado   =       abb_run_postorden(nodo->izquierda, (*funcion), extra, estado);
    }

    if(nodo->derecha    !=  NULL    &&  estado  ==  false){
        estado   =       abb_run_postorden(nodo->derecha, (*funcion), extra, estado);
    }

    if(estado   ==  false){
        estado   =       (*funcion)(nodo->elemento, extra);
    }

    return estado;
}

/*
 * Iterador interno. Recorre el arbol e invoca la funcion con cada
 * elemento del mismo. El puntero 'extra' se pasa como segundo
 * parámetro a la función. Si la función devuelve true, se finaliza el
 * recorrido aun si quedan elementos por recorrer. Si devuelve false
 * se sigue recorriendo mientras queden elementos.
 * El recorrido se realiza de acuerdo al recorrido solicitado.  Los
 * recorridos válidos son: ABB_RECORRER_INORDEN, ABB_RECORRER_PREORDEN
 * y ABB_RECORRER_POSTORDEN.
 */
void abb_con_cada_elemento(abb_t* arbol, int recorrido, bool (*funcion)(void*, void*), void* extra){
    bool estado =   false;
    if(arbol    ==  NULL    ||  arbol_vacio(arbol)){
        estado  =   true;
    }
    else if(recorrido   !=  ABB_RECORRER_INORDEN    &&  recorrido    !=  ABB_RECORRER_PREORDEN  &&  recorrido    !=  ABB_RECORRER_POSTORDEN){
        estado  =   true;
    }

    if(recorrido    ==  ABB_RECORRER_INORDEN    &&  estado  ==  false){
        abb_run_inorder(arbol->nodo_raiz, (*funcion), extra, estado);
    }
    else if(recorrido    ==  ABB_RECORRER_PREORDEN    &&  estado  ==  false){
        abb_run_preorden(arbol->nodo_raiz, (*funcion), extra, estado);
    }
    else if(recorrido    ==  ABB_RECORRER_POSTORDEN    &&  estado  ==  false){
        abb_run_postorden(arbol->nodo_raiz, (*funcion), extra, estado);
    }
}

