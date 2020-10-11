#include "pokedex.h"
#include <stdio.h>

#define AVISTAR "avistamientos.txt"
#define EVOLUCIONAR "evoluciones.txt"

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

/*int imprimir_el_arbol(abb_t* arbol){
    printf("\nVamos a imprimir los nombres de las especies que tenemos:\n");
    abb_con_cada_elemento(arbol, 0, (imprimir_una_linea_arbol), NULL);
}*/

int main(){
    char nombre[MAX_NOMBRE];
    bool andando    =   true;
    char instruccion;
    int numero;
    pokedex_t* pokedex;

    printf("\nBienvenido a la Pokedex!\n\n\n");

    do{
        printf("Que queres hacer?\n\n");
        scanf("%s", &instruccion);
        if(instruccion  ==  'i' ||  instruccion  ==  'I') pokedex    = pokedex_prender();
        else if(instruccion  == 'g' ||  instruccion  ==  'G') pokedex_apagar(pokedex);
        else if(instruccion  == 's' ||  instruccion  ==  'S'){
            pokedex_destruir(pokedex);
            andando   =   false;
        }
        else if(instruccion  == 'h' ||  instruccion  ==  'H'){
            printf("Iniciar Pokedex (tecla I): Da inicio a la Pokedex. Debera leer la infomacion del archivo pokedex.txt.\n"
                    "Guardar Pokedex (tecla G): Guarda la Pokedex. Debera guardar la infomación en el archivo pokedex.txt.\n"
                    "Salir del programa (tecla S): Finaliza la ejecucion del programa sin guardar la informacion.\n"
                    "Ayuda (tecla H): Informa cuales son los comandos validos a utilizar en cada momento.\n"
                    "Avistar Pokemon (tecla A): Actualizara la Pokedex incorporando a ella los Pokemon avistados. La informacion\n"
                    "de los nuevos avistamientos se encuenta en en archivo avistamientos.txt.\n"
                    "Evolucionar Pokemon (tecla E): Actualizara la Pokedex evolucionando los Pokemon que asi lo hayan hecho. La\n"
                    "informacion de las nuevas evoluciones se encuenta en en archivo evoluciones.txt.\n"
                    "Capturas recientes (tecla C): Muestra los ultimos Pokemon capturados.\n"
                    "Vistas recientes (tecla V): Muestra los últimos Pokemon vistos.\n"
                    "Informacion especie (tecla M): Muestra la informacion de la especie.\n"
                    "Informacion Pokemon (tecla P): Muestra la informacion de un Pokemon de una determinada especie.\n");
        }
        else if(instruccion  == 'a' ||  instruccion  ==  'A') pokedex_avistar(pokedex, AVISTAR);
        else if(instruccion  == 'e' ||  instruccion  ==  'E') pokedex_evolucionar(pokedex, EVOLUCIONAR);
        else if(instruccion  == 'c' ||  instruccion  ==  'C') pokedex_ultimos_capturados(pokedex);
        else if(instruccion  == 'v' ||  instruccion  ==  'V') pokedex_ultimos_vistos(pokedex);
        else if(instruccion  == 'm' ||  instruccion  ==  'M') {
            printf("De que Especie queres saber?(NUMERO)\n");
            scanf("%i", &numero);
            pokedex_informacion(pokedex, numero, " ");
        }
        else if(instruccion  == 'p' ||  instruccion  ==  'P'){
                printf("De que Especie queres saber?(NUMERO)\n");
                scanf("%i", &numero);
                printf("De que Pokemon queres saber?\n");
                scanf("%s", nombre);
                pokedex_informacion(pokedex, numero, nombre);
        }
        else printf("Esa instrucción no existe, escribi la H si no sabes que hacer\n");
    }while(andando==true);
}
