#include "pokedex.h"
#include <stdio.h>
#include <string.h>
#include "testing.c"

#define EXITO 0
#define ERROR -1
#define CAPTURADISIMO 'S'
#define AVISTAR_FORMAT "%i;%[^;];%[^;];%[^;];%i;%c\r\n"
#define EVOLUCIONAR_FORMAT "%i;%[^;];%i;%[^;];%[^\n]\r\n"

/*Funciones creadas por mi*/

int imprimir_especie(especie_pokemon_t* especie){
    printf("Vamos a imprimir toda la especie %s:\n", especie->nombre);

    size_t cantidad =   lista_elementos(especie->pokemones);
    for(size_t i=0; i<cantidad; i++){
        printf("-%s\n", (char*)lista_elemento_en_posicion(especie->pokemones, i));
    }
    return EXITO;
}

int comparador_especies(void* uno, void* dos){
    if(!uno || !dos)
        return 0;

    especie_pokemon_t* primero  =   uno;
    especie_pokemon_t* segundo  =   dos;

    if(primero->numero>segundo->numero)
        return 1;

    if(primero->numero<segundo->numero)
        return -1;

    return 0;
}

void destructoraso(void* destruido){
    if(!destruido)
        return;
    free(destruido);
}

// Inserta pokemon avistado
int pokedex_insertar_avistado(pokedex_t* pokedex, abb_t* arbol, especie_pokemon_t* especie, particular_pokemon_t particular){
    if(!pokedex ||  !arbol  ||  !especie) return ERROR;

    particular_pokemon_t* individuo =   (particular_pokemon_t*)calloc(1, sizeof(particular_pokemon_t));
    if(!individuo) return ERROR;

    strcpy(individuo->nombre, particular.nombre);
    individuo->nivel =   particular.nivel;
    individuo->capturado =   particular.capturado;

    lista_apilar(pokedex->ultimos_capturados, individuo);
    lista_encolar(pokedex->ultimos_vistos, individuo);
    especie_pokemon_t* especie_encontrada =   arbol_buscar(arbol, especie);
    lista_t* particulares =   especie_encontrada->pokemones;
    return lista_insertar(particulares, individuo);
}

// Inserta la especie y después el pokemon avistado
int pokedex_insertar_especie(pokedex_t* pokedex, abb_t* arbol, especie_pokemon_t* especie, particular_pokemon_t particular){
    especie_pokemon_t* especie_guardada =   (especie_pokemon_t*)calloc(1, sizeof(especie_pokemon_t));
    if(!especie_guardada) return ERROR;
    especie_guardada->numero    =   especie->numero;
    strcpy(especie_guardada->nombre, especie->nombre);
    strcpy(especie_guardada->descripcion, especie->descripcion);

    especie_guardada->pokemones =   lista_crear();
    if(!especie_guardada->pokemones){
        free(especie_guardada);
        return ERROR;
    }
    arbol_insertar(arbol, especie_guardada);
    return pokedex_insertar_avistado(pokedex, arbol, especie, particular);
}

size_t buscar_posicion(lista_t* lista, particular_pokemon_t* particularPokemon){
    size_t cantidad =   lista_elementos(lista);
    particular_pokemon_t* actual;
    for(size_t i=0; i<cantidad; i++){
        actual  =   lista_elemento_en_posicion(lista, i);
        if(strcmp(particularPokemon->nombre, actual->nombre)==0){
            return i;
        }
    }
    return 0;
}

int evolucionador(pokedex_t* pokedex, especie_pokemon_t* especie, especie_pokemon_t especie_nueva, particular_pokemon_t* particular, particular_pokemon_t* anterior){
    particular->nivel   =   anterior->nivel;
    particular->capturado   =   true;

    size_t posicion =   buscar_posicion(especie->pokemones, anterior);
    lista_borrar_de_posicion(especie->pokemones, posicion);

    especie_pokemon_t* nueva   =   arbol_buscar(pokedex->pokemones, &especie_nueva);
    if(nueva   ==   NULL) pokedex_insertar_especie(pokedex, pokedex->pokemones, &especie_nueva, *particular);
    else pokedex_insertar_avistado(pokedex, pokedex->pokemones, nueva, *particular);
    return EXITO;
}

bool comparador_de_particulares(void* uno, void* dos){
    particular_pokemon_t* part_uno =   uno;
    particular_pokemon_t* part_dos =   dos;

    //printf("%s --- %s\n", part_uno->nombre, part_dos->nombre);
    if(strcmp(part_uno->nombre, part_dos->nombre)==0){
        return true;
    }
    return false;
}

bool imprimidor_de_pokemon(void* pokemon, void* archivo){
    particular_pokemon_t* particularPokemon =   pokemon;
    fprintf(archivo, "P;%s;%i;", particularPokemon->nombre, particularPokemon->nivel);
    if(particularPokemon->capturado ==  true){
        fprintf(archivo, "S\n");
    }
    else{
        fprintf(archivo, "N\n");
    }
    return false;
}

bool imprimidor_de_especies(void* especievoid, void* archivo){
    especie_pokemon_t* especie  =   especievoid;
    fprintf(archivo, "E;%s;%i;%s\n", especie->nombre, especie->numero, especie->descripcion);
    lista_con_cada_elemento(especie->pokemones, (void (*)(void *, void *)) (imprimidor_de_pokemon), archivo);
    return false;
}

especie_pokemon_t* insertar_especie_al_prender(pokedex_t* pokedex, especie_pokemon_t especie){
    especie_pokemon_t* especie_guardada =   (especie_pokemon_t*)calloc(1, sizeof(especie_pokemon_t));
    especie_guardada->numero    =   especie.numero;
    strcpy(especie_guardada->nombre, especie.nombre);
    strcpy(especie_guardada->descripcion, especie.descripcion);

    especie_guardada->pokemones =   lista_crear();
    if(!especie_guardada->pokemones){
        free(especie_guardada);
        return NULL;
    }
    arbol_insertar(pokedex->pokemones, especie_guardada);
    return especie_guardada;
}

int insertar_pokemon_al_prender(pokedex_t* pokedex, especie_pokemon_t* especie, particular_pokemon_t pokemon){
    if(!pokedex ||  !pokedex->pokemones  ||  !especie) return ERROR;

    particular_pokemon_t* individuo =   (particular_pokemon_t*)calloc(1, sizeof(particular_pokemon_t));
    if(!individuo) return ERROR;

    strcpy(individuo->nombre, pokemon.nombre);
    individuo->nivel =   pokemon.nivel;
    individuo->capturado =   pokemon.capturado;

    especie_pokemon_t* especie_encontrada =   arbol_buscar(pokedex->pokemones, especie);
    lista_t* particulares =   especie_encontrada->pokemones;
    return lista_insertar(particulares, individuo);
}

/*Funciones del .h*/

pokedex_t* pokedex_crear(char entrenador[MAX_NOMBRE]){
    if(!entrenador) return NULL;

    pokedex_t* poke =   (pokedex_t*)calloc(1, sizeof(pokedex_t));
    if(!poke) return NULL;

    poke->pokemones =   arbol_crear(comparador_especies, destructoraso);
    poke->ultimos_capturados    =   lista_crear();
    poke->ultimos_vistos    =   lista_crear();
    strcpy(poke->nombre_entrenador, entrenador);
    return poke;
}

int pokedex_avistar(pokedex_t* pokedex, char ruta_archivo[MAX_RUTA]){
    if(!pokedex||  !ruta_archivo) return ERROR;
    if(!pokedex->pokemones) pokedex->pokemones  =   arbol_crear(comparador_especies, destructoraso);
    if(!pokedex->pokemones) return ERROR;
    if(!pokedex->ultimos_capturados) pokedex->ultimos_capturados    =   lista_crear();
    if(!pokedex->ultimos_capturados) return ERROR;
    if(!pokedex->ultimos_vistos) pokedex->ultimos_vistos    =   lista_crear();
    if(!pokedex->ultimos_vistos)    return ERROR;

    FILE *lector = fopen(ruta_archivo, "r");
    if(!lector) return ERROR;

    especie_pokemon_t especie;
    particular_pokemon_t    particular;
    char* capturado =   malloc(sizeof(char));
    int leidos  =   6;
    while(leidos    ==  6){
        printf("Entro\n");
        leidos  =   fscanf(lector, AVISTAR_FORMAT, &especie.numero, especie.nombre,
                            especie.descripcion, particular.nombre, &particular.nivel, capturado);
        if(*capturado == (char)CAPTURADISIMO){
            particular.capturado    =   true;
        }
        else{
            particular.capturado    =   false;
        }
        void* resultado =   arbol_buscar(pokedex->pokemones, &especie);
        if(resultado    ==  NULL){
            pokedex_insertar_especie(pokedex, pokedex->pokemones, &especie, particular);
        }
        else{
            pokedex_insertar_avistado(pokedex, pokedex->pokemones, &especie, particular);
        }
    }
    free(capturado);

    fclose(lector);
    printf("Avistamos bien!\n");
    return EXITO;
}

int pokedex_evolucionar(pokedex_t* pokedex, char ruta_archivo[MAX_RUTA]){
    if(!pokedex ||  !pokedex->pokemones ||  !ruta_archivo) return ERROR;

    FILE *archivo   =   fopen(ruta_archivo, "r");
    if(!archivo) return ERROR;

    int leidos;
    especie_pokemon_t especie_vieja;
    especie_pokemon_t especie;
    particular_pokemon_t particular;
    do{
        leidos  =   fscanf(archivo, EVOLUCIONAR_FORMAT,
                           &especie_vieja.numero, particular.nombre, &especie.numero,
                           especie.nombre, especie.descripcion);
        especie_pokemon_t* p_especie_vieja    =   arbol_buscar(pokedex->pokemones, &especie_vieja);
        if(p_especie_vieja   ==  NULL) return ERROR;

        particular_pokemon_t* particular_buscado    =   lista_buscar(p_especie_vieja->pokemones,
                        comparador_de_particulares, &particular);
        if(particular_buscado   ==  NULL) return printf("No existe el pokemon a evolucionar %s\n", particular.nombre);
        if(particular_buscado->capturado  ==  false) return printf("No esta capturado el pokemon %s\n", particular_buscado->nombre);
        if(evolucionador(pokedex, p_especie_vieja, especie, &particular, particular_buscado)==ERROR) return ERROR;
    }while(leidos   ==  5);

    fclose(archivo);
    printf("Evolucionamos bien!\n");
    return EXITO;
}

void pokedex_ultimos_capturados(pokedex_t* pokedex){
    if(!pokedex ||  !pokedex->ultimos_capturados) return;

    printf("\nÚltimos pokemon capturados:\n");
    while(lista_vacia(pokedex->ultimos_capturados)  ==  false){
        printf("-%s\n", (char*)lista_ultimo(pokedex->ultimos_capturados));
        lista_desapilar(pokedex->ultimos_capturados);
    }
}

void pokedex_ultimos_vistos(pokedex_t* pokedex){
    if(!pokedex ||  !pokedex->ultimos_vistos) return;

    printf("\nÚltimos pokemon vistos:\n");
    while(lista_vacia(pokedex->ultimos_vistos)  ==  false){
        printf("-%s\n", (char*)lista_primero(pokedex->ultimos_vistos));
        lista_desencolar(pokedex->ultimos_vistos);
    }
}

void pokedex_informacion(pokedex_t* pokedex, int numero_pokemon, char nombre_pokemon[MAX_NOMBRE]){
    if(!pokedex ||  !pokedex->pokemones  ||  numero_pokemon<0) return;

    especie_pokemon_t* especie;
    especie_pokemon_t especie1;
    especie1.numero  =   numero_pokemon;

    especie =   (especie_pokemon_t*)arbol_buscar(pokedex->pokemones, &especie1);
    if(!especie){
        printf("No existe la especie\n");
        return;
    }

    particular_pokemon_t particularPokemon;
    strcpy(particularPokemon.nombre, nombre_pokemon);
    particular_pokemon_t* Pokemon  =   lista_buscar(especie->pokemones, comparador_de_particulares, &particularPokemon);
    if(!Pokemon){
        printf("No existe el Pokemon\n");
        imprimir_especie(especie);
        return;
    }

    printf("-%s", especie->nombre);
    printf("-%s\n", particularPokemon.nombre);
}

void pokedex_destruir(pokedex_t* pokedex){
    while(!arbol_vacio(pokedex->pokemones)){
        lista_destruir(arbol_raiz(pokedex->pokemones));
        arbol_borrar(pokedex->pokemones, arbol_raiz(pokedex->pokemones));
    }
    arbol_destruir(pokedex->pokemones);
    free(pokedex);
    printf("Borramos todo\n");
}

int pokedex_apagar(pokedex_t* pokedex){
    if(!pokedex) return ERROR;

    FILE* archivo   =   fopen("pokedex.txt", "w+");
    if(!archivo) return ERROR;

    fprintf(archivo, "%s\n", pokedex->nombre_entrenador);
    abb_con_cada_elemento(pokedex->pokemones, 1, (imprimidor_de_especies), archivo);
    fclose(archivo);
    printf("Apago bien!\n");
    return EXITO;
}

pokedex_t* pokedex_prender(){
    FILE* archivo   =   fopen("pokedex.txt", "r");
    if(!archivo) return NULL;

    char nombre[MAX_NOMBRE];
    int leidos = fscanf(archivo, "%[^\n]\r\n", nombre);
    if(leidos   !=  1){
        printf("El nombre del entrenador esta mal escrito en el archivo\n");
        return NULL;
    }

    pokedex_t* pokedex  =   pokedex_crear(nombre);
    if(!pokedex){
        printf("Error al volver a crear la Pokedex");
        fclose(archivo);
        return NULL;
    }

    char primero_letra;
    char capturado;
    especie_pokemon_t especie;
    particular_pokemon_t particular;
    especie_pokemon_t* actual;
    do{
        leidos  =   fscanf(archivo, "%[^;]", &primero_letra);
        if(primero_letra    ==  'E'){
            leidos  =   fscanf(archivo, ";%[^;];%i;%[^\n]\r\n",
                               especie.nombre, &especie.numero, especie.descripcion);
            actual = insertar_especie_al_prender(pokedex, especie);
            if(actual==NULL) return NULL;
        }
        else if(primero_letra   ==  'P'){
            leidos  =   fscanf(archivo, ";%[^;];%i;%[^\n]\r\n",
                               particular.nombre, &particular.nivel, &capturado);
            insertar_pokemon_al_prender(pokedex, actual, particular);
        }
    }while(leidos   == 1 ||  leidos   == 2 ||  leidos   == 3);
    printf("Prendio bien! (Estamos en linea)\n");
    return pokedex;
}
