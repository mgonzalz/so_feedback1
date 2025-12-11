#include <stdio.h>
#define ARRAY_LEN 10        // Tamaño fijo del array


// Apartado 1
/* Declaración de la estructura y creación de un tipo con typedef */

typedef struct arrayLength {
    int arrInt[ARRAY_LEN];  // Array que almacena hasta 10 enteros positivos
    int arrSize;            // Número de elementos almacenados actualmente
    int arrAdd;             // Suma de todos los elementos positivos del array
} arrayLength_t;


// Apartado 2
/* Funciones para inicializar, imprimir y añadir elementos al array */

// Apartado 2.1 - Función initArray: inicializa el array y pone marcadores -1

int initArray(arrayLength_t *arr) {
    // Comprobamos que el puntero recibido no sea NULL
    if (arr == NULL) {
        return -1;           // Error si el puntero no apunta a una estructura válida
    }

    // Recorremos todas las posiciones del array
    for (int i = 0; i < ARRAY_LEN; i++) {
        arr->arrInt[i] = -1; // Marcamos todas las posiciones como vacías con -1
    }

    arr->arrSize = 0;        // No hay elementos almacenados todavía
    arr->arrAdd  = 0;        // La suma de elementos es 0

    return 0;                // Éxito
}

// Apartado 2.2 - Función printArray: imprime el contenido del array

void printArr(const arrayLength_t *arr) {
    // Comprobamos que el puntero recibido no sea NULL
    if (arr == NULL) {
        printf("Error: Estructura NULL en printArr\n");
        return;             // Salimos si no hay estructura válida
    }

    printf("{[");           // Inicio del formato de salida

    // Recorremos el array para imprimir sus 10 posiciones
    for (int i = 0; i < ARRAY_LEN; i++) {
        printf("%d", arr->arrInt[i]);   // Mostramos el valor de la posición i

        // Imprimimos coma y espacio para todos menos el último elemento
        if (i < ARRAY_LEN - 1) {
            printf(", ");
        }
    }

    // Imprimimos los campos arrSize y arrAdd
    printf("], %d, %d}\n", arr->arrSize, arr->arrAdd);
}

// Apartado 2.3 - Función addElement: añade un elemento entero positivo al array

int addElement(arrayLength_t *arr, int value) {
    // Comprobamos que el puntero recibido no sea NULL
    if (arr == NULL) {
        return -1;                // Error si el puntero no es válido
    }

    // Comprobamos que el valor a añadir sea positivo
    if (value < 0) {
        return -1;                // Error: solo se aceptan valores positivos
    }

    // Comprobamos si hay espacio libre en el array
    if (arr->arrSize >= ARRAY_LEN) {
        return -1;                // Error: el array está lleno
    }

    // Calculamos la posición donde insertar: siguiente a la última ocupada
    int pos = arr->arrSize;       // La siguiente posición libre es arrSize

    arr->arrInt[pos] = value;     // Guardamos el nuevo valor en la posición calculada
    arr->arrSize++;               // Aumentamos el contador de elementos almacenados
    arr->arrAdd += value;         // Actualizamos la suma total de los elementos

    return 0;                     // Éxito
}


// Apartado 3
/* Funciones para consultar tamaño, suma y un elemento concreto del array */

// Apartado 3.1 - Función getArrSize: devuelve el número de elementos almacenados

int getArrSize(const arrayLength_t *arr) {
    // Comprobación de puntero nulo
    if (arr == NULL) {
        return -1;                          // Error: estructura inválida
    }
    return arr->arrSize;                    // Devuelve el número de elementos almacenados
}

// Apartado 3.2 - Función getArrAdd: devuelve la suma de los elementos almacenados

int getArrAdd(const arrayLength_t *arr) {
    // Comprobación de puntero nulo
    if (arr == NULL) {
        return -1;                          // Error: estructura inválida
    }
    return arr->arrAdd;                     // Devuelve la suma de los elementos almacenados
}

// Apartado 3.3 - Función getElement: devuelve el elemento en la posición index

int getElement(const arrayLength_t *arr, int pos) {
    // Comprobamos que el puntero sea válido
    if (arr == NULL) {
        return -1;                          // Error: estructura inválida
    }

    // Comprobamos que la posición esté dentro de los límites del array
    if (pos < 0 || pos >= ARRAY_LEN) {
        return -1;                          // Error: posición fuera de rango
    }

    // Comprobamos que la posición esté ocupada con un valor positivo
    if (arr->arrInt[pos] < 0) {
        return -1;                          // Error: posición no ocupada por un valor válido
    }

    return arr->arrInt[pos];                // Devuelve el valor almacenado en esa posición
}


// Apartado 4
/* Función para modificar un elemento existente del array y actualizar la suma */

int setElement(arrayLength_t *arr, int pos, int newValue) {
    // Comprobamos que el puntero sea válido
    if (arr == NULL) {
        return -1;                      // Error: estructura inválida
    }

    // Comprobamos que la posición esté dentro de los límites del array
    if (pos < 0 || pos >= ARRAY_LEN) {
        return -1;                      // Error: posición fuera de rango
    }

    // Comprobamos que en esa posición haya un valor positivo (ocupada)
    if (arr->arrInt[pos] < 0) {
        return -1;                      // Error: posición no ocupada por un valor positivo
    }

    // Comprobamos que el nuevo valor sea positivo
    if (newValue < 0) {
        return -1;                      // Error: el nuevo valor debe ser positivo
    }

    // Guardamos el valor antiguo para poder actualizar bien la suma
    int oldValue = arr->arrInt[pos];

    // Sustituimos el valor antiguo por el nuevo
    arr->arrInt[pos] = newValue;

    // Actualizamos la suma total: restamos el viejo y sumamos el nuevo
    arr->arrAdd = arr->arrAdd - oldValue + newValue;

    return 0;                           // Éxito
}


// Apartado 5
/* Función para “resetear” la estructura */

int resetArr(arrayLength_t *arr) {
    // Reutilizar la lógica de inicialización
    return initArray(arr);
}


// Apartado 6
/* Función main para comprobar el funcionamiento de todas las funciones del Ejercicio 1 */

int main(void) {
    arrayLength_t al1, al2;
    int i, valor;

    /* 1) Inicializar las dos estructuras al1 y al2 */
    if (initArray(&al1) != 0 || initArray(&al2) != 0) {
        printf("Error al inicializar las estructuras\n");
        return 1;
    }

    /* 2) al1 almacena 0, 10, 20, ..., 90 */
    for (i = 0; i < ARRAY_LEN; i++) {
        /* i*10 genera la secuencia 0,10,20,...,90 */
        if (addElement(&al1, i * 10) != 0) {
            printf("Error al añadir elemento %d a al1\n", i * 10);
            return 1;
        }
    }

    /* 3) Mostrar al1 */
    printf("Contenido inicial de al1:\n");
    printArr(&al1);

    /* 4) Actualizar posiciones impares del array con valores 1, 3, 5, 7, 9 */
    for (i = 0; i < 5; i++) {
        int pos   = 2 * i + 1;  /* 1, 3, 5, 7, 9 */
        int value = 2 * i + 1;  /* 1, 3, 5, 7, 9 */

        if (setElement(&al1, pos, value) != 0) {
            printf("Error al actualizar al1 en posicion %d\n", pos);
            return 1;
        }
    }

    /* 5) Mostrar de nuevo al1 */
    printf("Contenido de al1 tras actualizar posiciones impares:\n");
    printArr(&al1);

    /* 6) Añadir a al2 las posiciones pares de al1 de forma consecutiva en al2 */
    for (i = 0; i < ARRAY_LEN; i += 2) {  /* i = 0, 2, 4, 6, 8 */
        valor = getElement(&al1, i);
        if (valor == -1) {
            printf("Error al obtener elemento par de al1 en posicion %d\n", i);
            return 1;
        }
        if (addElement(&al2, valor) != 0) {
            printf("Error al añadir a al2 el valor de al1[%d]\n", i);
            return 1;
        }
    }

    /* 7) Actualizar las posiciones finales de al2 para que almacenen 0,1,2,3,4 */
    for (i = 0; i < 5; i++) {
        if (addElement(&al2, i) != 0) {
            printf("Error al añadir el valor %d a las posiciones finales de al2\n", i);
            return 1;
        }
    }

    /* 8) Mostrar al2 */
    printf("Contenido final de al2:\n");
    printArr(&al2);

    return 0;
}
