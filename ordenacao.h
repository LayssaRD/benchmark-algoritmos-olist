#ifndef ORDENACAO_H
#define ORDENACAO_H

#include <vector>
#include <string>
#include "registro.h"

struct Resultado {
    std::string algoritmo;
    long long   comparacoes;
    long long   trocas;
    double      tempo;
};

Resultado bubbleSort    (std::vector<Registro> arr);
Resultado insertionSort (std::vector<Registro> arr);
Resultado selectionSort (std::vector<Registro> arr);
Resultado shellSort     (std::vector<Registro> arr);
Resultado lomuto        (std::vector<Registro> arr);
Resultado hoare         (std::vector<Registro> arr);
Resultado mergeSort     (std::vector<Registro> arr);
Resultado radixSort     (std::vector<Registro> arr);
Resultado heapSort      (std::vector<Registro> arr);

std::vector<Registro> mergeSortOrdenado(std::vector<Registro> arr);

#endif
