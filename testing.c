#include <stdio.h>
/*#include "pokedex.h"

int imprimir_el_arbol(abb_t* arbol);

void imprimir_particulares(void* usable, void* extra){
    particular_pokemon_t* Pokemon   =   usable;
    printf("--%s\n", Pokemon->nombre);
}

bool imprimir_una_linea_arbol(void* usable, void* extra){
    especie_pokemon_t* especiePokemon   =   usable;
    printf("%i %s\n", especiePokemon->numero, especiePokemon->nombre);
    lista_con_cada_elemento(especiePokemon->pokemones, (imprimir_particulares), NULL);

    return false;
}

int imprimir_el_arbol(abb_t* arbol){
    printf("\nVamos a imprimir los nombres de las especies que tenemos:\n");
    abb_con_cada_elemento(arbol, 0, (imprimir_una_linea_arbol), NULL);
}

int testing(){
    char entrenador[] =   "valen";
    pokedex_t* poke  =   pokedex_crear((char *) entrenador);
    pokedex_avistar(poke, "C:\\Users\\valentin.berne\\Desktop\\2020 Primer Cuatrimestre\\Pokedex\\avistamientos.txt");
    pokedex_evolucionar(poke, "C:\\Users\\valentin.berne\\Desktop\\2020 Primer Cuatrimestre\\Pokedex\\evoluciones.txt");
    pokedex_ultimos_capturados(poke);
    pokedex_ultimos_vistos(poke);
    pokedex_informacion(poke, 46, "Parrrras");
    pokedex_apagar(poke);
    pokedex_prender();
    imprimir_el_arbol(poke->pokemones);
    pokedex_destruir(poke);
}*/