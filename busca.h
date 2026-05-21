#ifndef BUSCA_H
#define BUSCA_H

#include <vector>
#include <string>
#include "registro.h"

struct ResultadoBusca {
    std::string algoritmo;
    long long   verificacoes;
    double      tempo;
    std::string caso;
};

// Busca sequencial — funciona em vetor não ordenado (antes de ordenar)
ResultadoBusca buscaSequencial(const std::vector<Registro> &arr, int alvo, const std::string &caso);

// Busca binária — requer vetor previamente ordenado
ResultadoBusca buscaBinaria   (const std::vector<Registro> &arr, int alvo, const std::string &caso);

#endif
