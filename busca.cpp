#include "busca.h"
#include <chrono>
using namespace std;

// Busca Sequencial — O(n)
// Melhor caso: elemento na 1ª posição (1 verificação)
// Pior caso  : elemento na última posição ou ausente (n verificações)
ResultadoBusca buscaSequencial(const vector<Registro> &arr, int alvo, const string &caso) {
    long long verif = 0;
    auto ini = chrono::high_resolution_clock::now();

    for (int repeticao = 0; repeticao < 1000; repeticao++) {
        for (int i = 0; i < (int)arr.size(); i++) {
            verif++;
            if (arr[i].zipCode == alvo)
                break;
        }
    }

    auto fim = chrono::high_resolution_clock::now();

    return {"Sequencial", (verif / 1000), chrono::duration<double>(fim - ini).count(), caso};
}

// Busca Binária — O(log n)
// Melhor caso: elemento no centro (1 verificação)
// Pior caso  : elemento na primeira ou última posição (log n verificações)
ResultadoBusca buscaBinaria(const vector<Registro> &arr, int alvo, const string &caso) {

    long long verif = 0;
    auto ini = chrono::high_resolution_clock::now();

    for (int repeticao = 0; repeticao < 1000; repeticao++) {
        int esq = 0;
        int dir = (int)arr.size() - 1;

        while (esq <= dir) {
            verif++;
            int meio = esq + (dir - esq) / 2;

            if (arr[meio].zipCode == alvo)
                break;
            else if (arr[meio].zipCode < alvo)
                esq = meio + 1;
            else
                dir = meio - 1;
        }
    }

    auto fim = chrono::high_resolution_clock::now();

    return {"Binaria", verif / 1000, chrono::duration<double>(fim - ini).count(), caso};
}
