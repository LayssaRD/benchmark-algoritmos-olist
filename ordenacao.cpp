#include "ordenacao.h"
#include <chrono>
#include <algorithm>
#include <vector>
using namespace std;


// ── Bubble Sort ──────────────────────────────────────────────
// Complexidade: O(n²) comparações e trocas
Resultado bubbleSort(vector<Registro> arr) {
    int n = arr.size();
    long long comp = 0, troca = 0;
    auto ini = chrono::high_resolution_clock::now();

    for (int i = 0; i < n - 1; i++)
        for (int j = 0; j < n - 1 - i; j++) {
            comp++;
            if (arr[j].zipCode > arr[j+1].zipCode) {
                swap(arr[j], arr[j+1]);
                troca++;
            }
        }

    auto fim = chrono::high_resolution_clock::now();
    return {"Bubble Sort", comp, troca, chrono::duration<double>(fim - ini).count()};
}

// ── Insertion Sort ───────────────────────────────────────────
// Complexidade: O(n²) — eficiente para dados quase ordenados
Resultado insertionSort(vector<Registro> arr) {
    int n = arr.size();
    long long comp = 0;
    long long troca = 0;
    auto ini = chrono::high_resolution_clock::now();

    for (int i = 1; i < n; i++) {
        Registro chave = arr[i];
        int j = i - 1;
        bool moveu = false;
        while (j >= 0) {
            comp++;
            if (arr[j].zipCode <= chave.zipCode)
                break;
            arr[j + 1] = arr[j];
            j--;
            moveu = true;
        }
        arr[j + 1] = chave;
        if (moveu)
            troca++;
    }

    auto fim = chrono::high_resolution_clock::now();
    return {"Insertion Sort", comp, troca, chrono::duration<double>(fim - ini).count()};
}

// ── Selection Sort ───────────────────────────────────────────
// Complexidade: O(n²) comparações, O(n) trocas
Resultado selectionSort(vector<Registro> arr) {
    int n = arr.size();
    long long comp = 0, troca = 0;
    auto ini = chrono::high_resolution_clock::now();

    for (int i = 0; i < n - 1; i++) {
        int menor = i;
        for (int j = i+1; j < n; j++) {
            comp++;
            if (arr[j].zipCode < arr[menor].zipCode)
                menor = j;
        }
        if (menor != i) {
            swap(arr[i], arr[menor]);
            troca++;
        }
    }

    auto fim = chrono::high_resolution_clock::now();
    return {"Selection Sort", comp, troca, chrono::duration<double>(fim - ini).count()};
}

// ── Shell Sort ───────────────────────────────────────────────
// Complexidade: depende da sequência de gaps — aprox. O(n log² n)
Resultado shellSort(vector<Registro> arr) {
    int n = arr.size();
    long long comp = 0, troca = 0;
    auto ini = chrono::high_resolution_clock::now();

    for (int gap = n/2; gap > 0; gap /= 2) {
        for (int i = gap; i < n; i++) {
            Registro chave = arr[i];
            int j = i;
            bool moveu = false;
            while (j >= gap) {
                comp++;
                if (arr[j-gap].zipCode <= chave.zipCode)
                    break;
                arr[j] = arr[j-gap];
                j -= gap;
                moveu = true;
            }
            arr[j] = chave;
            if (moveu)
                troca++;
        }
    }

    auto fim = chrono::high_resolution_clock::now();
    return {"Shell Sort", comp, troca, chrono::duration<double>(fim - ini).count()};
}

// ── Quick Sort Lomuto ────────────────────────────────────────
// Pivô = último elemento; partição da esquerda para direita
static int particionaLomuto(vector<Registro> &arr, int ini, int fim,
                             long long &comp, long long &troca) {
    int pivo = arr[fim].zipCode;
    int i = ini - 1;
    for (int j = ini; j < fim; j++) {
        comp++;
        if (arr[j].zipCode <= pivo) {
            i++;
            if (i != j) { swap(arr[i], arr[j]); troca++; }
        }
    }
    if (i+1 != fim) { swap(arr[i+1], arr[fim]); troca++; }
    return i+1;
}

static void qsLomuto(vector<Registro> &arr, int ini, int fim,
                     long long &comp, long long &troca) {
    if (ini < fim) {
        int p = particionaLomuto(arr, ini, fim, comp, troca);
        qsLomuto(arr, ini,   p-1, comp, troca);
        qsLomuto(arr, p+1,  fim,  comp, troca);
    }
}

Resultado lomuto(vector<Registro> arr) {
    long long comp = 0, troca = 0;
    auto ini = chrono::high_resolution_clock::now();
    qsLomuto(arr, 0, (int)arr.size()-1, comp, troca);
    auto fim = chrono::high_resolution_clock::now();
    return {"QuickSort Lomuto", comp, troca, chrono::duration<double>(fim - ini).count()};
}

// ── Quick Sort Hoare ─────────────────────────────────────────
// Pivô = primeiro elemento; dois ponteiros se aproximam
static int particionaHoare(vector<Registro> &arr, int ini, int fim,
                            long long &comp, long long &troca) {
    int pivo = arr[ini].zipCode;
    int i = ini - 1, j = fim + 1;
    while (true) {
        do { i++; comp++; } while (arr[i].zipCode < pivo);
        do { j--; comp++; } while (arr[j].zipCode > pivo);
        if (i >= j) return j;
        swap(arr[i], arr[j]);
        troca++;
    }
}

static void qsHoare(vector<Registro> &arr, int ini, int fim,
                    long long &comp, long long &troca) {
    if (ini < fim) {
        int p = particionaHoare(arr, ini, fim, comp, troca);
        qsHoare(arr, ini,  p,   comp, troca);
        qsHoare(arr, p+1, fim,  comp, troca);
    }
}

Resultado hoare(vector<Registro> arr) {
    long long comp = 0, troca = 0;
    auto ini = chrono::high_resolution_clock::now();
    qsHoare(arr, 0, (int)arr.size()-1, comp, troca);
    auto fim = chrono::high_resolution_clock::now();
    return {"QuickSort Hoare", comp, troca, chrono::duration<double>(fim - ini).count()};
}

// ── Merge Sort ───────────────────────────────────────────────
// Complexidade: O(n log n) garantido em todos os casos
static void mergeFn(vector<Registro> &arr, int ini, int meio, int fim,
                    long long &comp, long long &troca) {
    vector<Registro> esq(arr.begin()+ini, arr.begin()+meio+1);
    vector<Registro> dir(arr.begin()+meio+1, arr.begin()+fim+1);
    int i = 0, j = 0, k = ini;

    while (i < (int)esq.size() && j < (int)dir.size()) {
        comp++;
        if (esq[i].zipCode <= dir[j].zipCode) {
            if (arr[k].zipCode != esq[i].zipCode)
                troca++;
            arr[k++] = esq[i++];
        } else {
            if (arr[k].zipCode != dir[j].zipCode)
                troca++;
            arr[k++] = dir[j++];
        }
    }
    while (i < (int)esq.size()) {
        if (arr[k].zipCode != esq[i].zipCode)
            troca++;
        arr[k++] = esq[i++];
    }

    while (j < (int)dir.size()) {
        if (arr[k].zipCode != dir[j].zipCode)
            troca++;
        arr[k++] = dir[j++];
    }
}

static void ms(vector<Registro> &arr, int ini, int fim,
               long long &comp, long long &troca) {
    if (ini < fim) {
        int meio = ini + (fim - ini) / 2;
        ms(arr, ini,    meio, comp, troca);
        ms(arr, meio+1, fim,  comp, troca);
        mergeFn(arr, ini, meio, fim, comp, troca);
    }
}

Resultado mergeSort(vector<Registro> arr) {
    long long comp = 0, troca = 0;
    auto ini = chrono::high_resolution_clock::now();
    ms(arr, 0, (int)arr.size()-1, comp, troca);
    auto fim = chrono::high_resolution_clock::now();
    return {"Merge Sort", comp, troca, chrono::duration<double>(fim - ini).count()};
}

vector<Registro> mergeSortOrdenado(vector<Registro> arr) {
    long long comp = 0, troca = 0;
    ms(arr, 0, (int)arr.size()-1, comp, troca);
    return arr;
}

// ── Radix Sort ───────────────────────────────────────────────
// Complexidade: O(d·n) onde d = número de dígitos da chave
Resultado radixSort(vector<Registro> arr) {
    long long comp = 0, troca = 0;
    auto ini = chrono::high_resolution_clock::now();
    int n = arr.size();
    int maior = arr[0].zipCode;

    for (int i = 1; i < n; i++) {
        comp++;
        if (arr[i].zipCode > maior)
            maior = arr[i].zipCode;
    }

    for (int exp = 1; maior / exp > 0; exp *= 10) {
        vector<Registro> saida(n);
        int cont[10] = {0};

        for (int i = 0; i < n; i++) {
            int digito = (arr[i].zipCode / exp) % 10;
            cont[digito]++;
        }

        for (int i = 1; i < 10; i++)
            cont[i] += cont[i - 1];

        for (int i = n - 1; i >= 0; i--) {
            int digito = (arr[i].zipCode / exp) % 10;
            saida[--cont[digito]] = arr[i];
        }

        for (int i = 0; i < n; i++) {
            if (arr[i].zipCode != saida[i].zipCode)
                troca++;
            arr[i] = saida[i];
        }
    }

    auto fim = chrono::high_resolution_clock::now();
    return {"Bubble Sort", comp, troca, chrono::duration<double>(fim - ini).count()};
}

// ── Heap Sort ────────────────────────────────────────────────
// Complexidade: O(n log n) garantido; in-place
static void heapify(vector<Registro> &arr, int n, int i,
                    long long &comp, long long &troca) {
    int maior = i;
    int esq   = 2*i + 1;
    int dir   = 2*i + 2;

    if (esq < n) { comp++; if (arr[esq].zipCode > arr[maior].zipCode) maior = esq; }
    if (dir < n) { comp++; if (arr[dir].zipCode > arr[maior].zipCode) maior = dir; }

    if (maior != i) {
        swap(arr[i], arr[maior]);
        troca++;
        heapify(arr, n, maior, comp, troca);
    }
}

Resultado heapSort(vector<Registro> arr) {
    int n = arr.size();
    long long comp = 0, troca = 0;
    auto ini = chrono::high_resolution_clock::now();

    for (int i = n/2 - 1; i >= 0; i--)
        heapify(arr, n, i, comp, troca);

    for (int i = n-1; i >= 1; i--) {
        swap(arr[0], arr[i]);
        troca++;
        heapify(arr, i, 0, comp, troca);
    }

    auto fim = chrono::high_resolution_clock::now();
    return {"Heap Sort", comp, troca, chrono::duration<double>(fim - ini).count()};
}
