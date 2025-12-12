#include <stdio.h>


// Apartado 1
/* Definición de constantes, estructuras y funciones básicas de potencias */

/* Apartado 1.1 - Tamaño fijo del array */

#define SIZE 10


// Apartado 1.2 - Estructura potencia_t con base, exponente y resultado

typedef struct potencia {
    int base;                               // Valor de la base de la potencia
    int exp;                                // Valor del exponente de la potencia
    int potencia;                           // Resultado de base^exp
} potencia_t;

// Apartado 1.3 - Definición de un tipo puntero a estructura potencia_t

typedef potencia_t *potenciaP_t;           // Tipo puntero a potencia_t (potenciaP_t)


// Apartado 1.4 - setBaseExp: inicializa base y exp, y deja potencia a -1

void setBaseExp(potencia_t *p, int base, int exp) {
    if (p == NULL) {
        return;                            // Si el puntero es NULL, nada que hacer
    }

    p->base = base;                        // Asignamos el valor de la base
    p->exp = exp;                          // Asignamos el valor del exponente
    p->potencia = -1;                      // Marcamos el resultado como “no calculado” con -1
}


// Apartado 1.5 - getPotencia: calcula base^exp de forma iterativa

int getPotencia(int base, int exp) {
    if (exp < 0) {
        // No se contemplan exponentes negativos;
        // devolvemos -1 para indicar un caso no soportado.
        return -1;
    }

    int resultado = 1;                    // Cualquier número elevado a 0 es 1

    // Multiplicamos "base" por sí misma "exp" veces
    for (int i = 0; i < exp; i++) {
        resultado = resultado * base;    // Acumulamos el producto
    }

    return resultado;                    // Devolvemos base^exp
}


// Apartado 1.6 - setPotenciaEst: calcula base^exp y lo guarda en el campo potencia

void setPotenciaEst(potencia_t *p) {
    if (p == NULL) {
        return;                         // Si el puntero es NULL, no hacemos nada
    }

    // Usamos la función getPotencia para calcular base^exp
    p->potencia = getPotencia(p->base, p->exp);
}


// Apartado 2
/* Funciones para manejar arrays de estructuras potencia_t y función para hebras */

// Apartado 2.1 - initArrayPotencias: inicializa un array de potencia_t con base=i+1, exp=0, potencia=1

void initArrayEst(potencia_t arr[SIZE]) {
    // Recorremos todas las posiciones del array
    for (int i = 0; i < SIZE; i++) {
        arr[i].base = i + 1;            // base = índice + 1  → 1,2,3,...,10
        arr[i].exp = 0;                 // exponentes a 0
        arr[i].potencia = 1;            // Cualquier número ^0 = 1
    }
}


// Apartado 2.2 - printArrayEst: muestra el contenido del array en el formato indicado

void printArrayEst(potencia_t arr[SIZE]) {
    // Recorremos todas las posiciones del array
    for (int i = 0; i < SIZE; i++) {
        // Mostramos: arr[i]: base: X exp: Y potencia Z
        printf("arr[%d]: base: %d exp: %d potencia %d\n",
            i,
            arr[i].base,
            arr[i].exp,
            arr[i].potencia);
    }
}


// Apartado 2.3 - calcuPotHeb: función preparada para hebras; calcula base^exp en potencia

void *calcuPotHeb(void *arg) {
    // Convertimos el puntero genérico void* a puntero a potencia_t
    potenciaP_t p = (potenciaP_t) arg;

    // Comprobamos que el puntero no sea NULL
    if (p != NULL) {
        // Calculamos la potencia y la almacenamos en el campo potencia
        p->potencia = getPotencia(p->base, p->exp);
    }

    return NULL;
}


//Apartado 3
/* Función main: uso de procesos (fork) y hebras (pthread) con potencia_t */

#include <stdlib.h>                        // exit, EXIT_FAILURE, EXIT_SUCCESS
#include <unistd.h>                        // fork
#include <sys/types.h>                     // pid_t
#include <sys/wait.h>                      // wait, WIFEXITED, WEXITSTATUS
#include <pthread.h>                       // pthread_t, pthread_create, pthread_join

int main(void) {
    // 1) Declarar array arr1 de estructuras potencia_t de SIZE elementos
    potencia_t arr1[SIZE];

    // 2) Inicializar arr1
    initArrayEst(arr1);

    // 3) Mostrar arr1 tras la inicialización
    printf("Estado inicial de arr1 tras initArrayEst:\n");
    printArrayEst(arr1);
    printf("\n");

    // 4) Usando setBaseExp, hacer que:
    //    base = índice (0..9) y exp = 2. potencia quedará a -1.
    for (int i = 0; i < SIZE; i++) {
        setBaseExp(&arr1[i], i, 2);    // base = i, exp = 2, potencia = -1
    }

    // 5) Mostrar arr1 después de modificar
    printf("arr1 tras setBaseExp (base=i, exp=2, potencia=-1):\n");
    printArrayEst(arr1);
    printf("\n");

    // 6) Crear SIZE hijos con fork().
    //    Cada hijo calcula la potencia de una posición de arr1
    //    y el padre asigna ese resultado al campo potencia.
    for (int i = 0; i < SIZE; i++) {
        pid_t pid = fork();            // Creamos un nuevo proceso

        if (pid < 0) {
            // Error al hacer fork
            perror("Error en fork");
            exit(EXIT_FAILURE);
        } else if (pid == 0) {
            // Proceso hijo: calcula base^exp para arr1[i]
            int resultado = getPotencia(arr1[i].base, arr1[i].exp);
            exit(resultado);
        } else {
            // Proceso padre: espera a que termine este hijo
            int status;
            if (wait(&status) == -1) {
                perror("Error en wait");
                exit(EXIT_FAILURE);
            }

            if (WIFEXITED(status)) {
                int valor_hijo = WEXITSTATUS(status);  // Resultado devuelto por el hijo

                // Asignamos el resultado al campo potencia de la posición i
                arr1[i].potencia = valor_hijo;
            } else {
                printf("Hijo %d no terminó correctamente\n", pid);
            }
        }
    }

    // 7) Mostrar de nuevo el array arr1 con las potencias calculadas
    printf("arr1 tras el cálculo de potencias con procesos (fork):\n");
    printArrayEst(arr1);
    printf("\n");

    // 8) Declarar un nuevo array arr2 de estructuras potencia_t
    potencia_t arr2[SIZE];

    // 9) Inicializar arr2 con initArrayEst
    initArrayEst(arr2);

    // 10) Mostrar arr2 tras la inicialización
    printf("Estado inicial de arr2 tras initArrayEst:\n");
    printArrayEst(arr2);
    printf("\n");

    // 11) Usando setBaseExp, hacer que en arr2:
    //     base = índice (0..9) y exp = 2, potencia=-1
    for (int i = 0; i < SIZE; i++) {
        setBaseExp(&arr2[i], i, 2);
    }

    // 12) Mostrar arr2 tras modificar base y exp
    printf("arr2 tras setBaseExp (base=i, exp=2, potencia=-1):\n");
    printArrayEst(arr2);
    printf("\n");

    // 13) Crear SIZE hebras (threads) para que calculen base^exp en cada estructura
    pthread_t threads[SIZE];

    for (int i = 0; i < SIZE; i++) {
        // Cada hebra recibe la dirección de arr2[i]
        int err = pthread_create(&threads[i], NULL, calcuPotHeb, (void *)&arr2[i]);
        if (err != 0) {
            printf("Error al crear la hebra %d\n", i);
        }
    }

    // Esperamos a que todas las hebras terminen
    for (int i = 0; i < SIZE; i++) {
        pthread_join(threads[i], NULL);
    }

    // 14) Mostrar de nuevo el array arr2 tras el cálculo con hebras
    printf("arr2 tras el cálculo de potencias con hebras (pthread):\n");
    printArrayEst(arr2);
    printf("\n");

    return 0;
}
