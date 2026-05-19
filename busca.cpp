#include "busca.h"
#include <ctime>
using namespace std;

// ─────────────────────────────────────────────────────────────
// CORREÇÕES:
//  1. Alvo: zipCode (int nativo) — sem bug de == em float
//  2. Busca sequencial opera sobre vetor ORIGINAL (não ordenado)
//  3. Busca binária opera sobre vetor JÁ ordenado
// ─────────────────────────────────────────────────────────────

// Busca Sequencial — O(n)
// Deve ser chamada com o vetor ORIGINAL (não ordenado)
// Melhor caso: elemento na 1ª posição (1 verificação)
// Pior caso  : elemento na última posição ou ausente (n verificações)
ResultadoBusca buscaSequencial(const vector<Registro> &arr,
                               int alvo, const string &caso) {

    long long verif = 0;
    clock_t ini = clock();

    for (int repeticao = 0; repeticao < 1000; repeticao++) {

        for (int i = 0; i < (int)arr.size(); i++) {
            verif++;

            if (arr[i].zipCode == alvo)
                break;
        }
    }

    clock_t fim = clock();

    return {
        "Sequencial",
        caso,
        verif / 1000,
        (double)(fim - ini) / CLOCKS_PER_SEC
    };
}

// Busca Binária — O(log n)
// Deve ser chamada com o vetor JÁ ORDENADO por zipCode
// Melhor caso: elemento no centro (1 verificação)
// Pior caso  : elemento na primeira ou última posição (log n verificações)
ResultadoBusca buscaBinaria(const vector<Registro> &arr,
                            int alvo, const string &caso) {

    long long verif = 0;
    clock_t ini = clock();

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

    clock_t fim = clock();

    return {
        "Binaria",
        caso,
        verif / 1000,
        (double)(fim - ini) / CLOCKS_PER_SEC
    };
}
