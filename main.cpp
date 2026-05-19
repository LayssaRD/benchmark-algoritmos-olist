#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <iomanip>
#include "registro.h"
#include "ordenacao.h"
#include "busca.h"

using namespace std;

// ─────────────────────────────────────────────────────────────
// Leitura do CSV — Olist Customers Dataset
//
// Colunas:
//   [0] customer_id            → ignorado na ordenação
//   [1] customer_unique_id     → ignorado
//   [2] customer_zip_code_prefix → zipCode (int) ← CHAVE
//   [3] customer_city          → city
//   [4] customer_state         → state
//
// O CSV pode ter campos com ou sem aspas duplas — ambos tratados.
// ─────────────────────────────────────────────────────────────
static string limparAspas(const string &s) {
    if (s.size() >= 2 && s.front() == '"' && s.back() == '"')
        return s.substr(1, s.size() - 2);
    return s;
}

vector<Registro> carregarCSV(const string &caminho) {
    vector<Registro> dados;
    ifstream arquivo(caminho);
    if (!arquivo.is_open()) {
        cerr << "Erro ao abrir o arquivo: " << caminho << "\n";
        return dados;
    }

    string linha;
    getline(arquivo, linha); // ignora cabeçalho

    int id = 0;
    while (getline(arquivo, linha)) {
        // Parser simples: separa por vírgula respeitando aspas
        vector<string> campos;
        string campo;
        bool dentroAspas = false;

        for (char c : linha) {
            if (c == '"') {
                dentroAspas = !dentroAspas;
            } else if (c == ',' && !dentroAspas) {
                campos.push_back(limparAspas(campo));
                campo.clear();
            } else {
                campo += c;
            }
        }
        campos.push_back(limparAspas(campo)); // último campo

        if (campos.size() < 5) continue;

        try {
            int zip = stoi(campos[2]);
            if (zip <= 0) continue;

            Registro r;
            r.id                 = ++id;
            r.customer_id        = campos[0];
            r.customer_unique_id = campos[1];
            r.zipCode            = zip;
            r.city               = campos[3];
            r.state              = campos[4];
            dados.push_back(r);
        } catch (...) { continue; }
    }
    return dados;
}

// ─────────────────────────────────────────────────────────────
// Salvamento dos resultados
// ─────────────────────────────────────────────────────────────
void salvarOrdenacaoCSV(const string &caminho,
                        const vector<Resultado> &resultados, int total) {
    ofstream f(caminho, ios::app);
    for (const auto &r : resultados)
        f << r.algoritmo << "," << total << ","
          << r.comparacoes << "," << r.trocas << ","
          << fixed << setprecision(6) << r.tempo << "\n";
}

void salvarBuscaCSV(const string &caminho,
                    const vector<ResultadoBusca> &resultados, int total) {
    ofstream f(caminho, ios::app);
    for (const auto &r : resultados)
        f << r.algoritmo << "," << total << "," << r.caso << ","
          << r.verificacoes << ","
          << fixed << setprecision(6) << r.tempo << "\n";
}

// ─────────────────────────────────────────────────────────────
// Exibição no console
// ─────────────────────────────────────────────────────────────
void exibirOrdenacao(const Resultado &r) {
    cout << "  " << left  << setw(20) << r.algoritmo
         << " | comp: "   << setw(12) << r.comparacoes
         << " | trocas: " << setw(10) << r.trocas
         << " | tempo: "  << fixed << setprecision(4) << r.tempo << "s\n";
}

void exibirBusca(const ResultadoBusca &r) {
    cout << "  " << left << setw(12) << r.algoritmo
         << " [" << setw(7) << r.caso << "]"
         << " | verif: " << setw(8) << r.verificacoes
         << " | tempo: " << fixed << setprecision(6) << r.tempo << "s\n";
}

// ─────────────────────────────────────────────────────────────
// Main
// ─────────────────────────────────────────────────────────────
int main() {
    const string arquivoDataset   = "olist_customers_dataset.csv";
    const string arquivoOrdenacao = "resultado_ordenacao.csv";
    const string arquivoBusca     = "resultado_busca.csv";

    // Cria/limpa arquivos de saída com cabeçalho
    ofstream(arquivoOrdenacao) << "Algoritmo,Total,Comparacoes,Trocas,Tempo(s)\n";
    ofstream(arquivoBusca)     << "Algoritmo,Total,Caso,Verificacoes,Tempo(s)\n";

    // ── Carregamento completo ────────────────────────────────
    cout << "Carregando dataset Olist Customers...\n";
    vector<Registro> dadosCompletos = carregarCSV(arquivoDataset);
    int totalCompleto = (int)dadosCompletos.size();
    cout << "Total de registros carregados: " << totalCompleto << "\n\n";

    if (totalCompleto == 0) {
        cerr << "Nenhum registro carregado. Verifique o arquivo CSV.\n";
        return 1;
    }

    vector<int> tamanhos = {10000, 20000, 30000, 50000};

    cout << "Blocos de teste: ";
    for (int t : tamanhos) cout << t << " ";
    cout << "\n\n";

    for (int tamanho : tamanhos) {

        // Fatia do vetor original (mantém ordem de inserção do CSV)
        vector<Registro> fatia(dadosCompletos.begin(),
                               dadosCompletos.begin() + tamanho);

        cout << "========================================\n";
        cout << "BLOCO: " << tamanho << " registros\n";
        cout << "========================================\n";

        // ── BUSCA SEQUENCIAL — vetor ORIGINAL (não ordenado) ─
        // Enunciado seção 4 item 2:
        // "Busca Sequencial ANTES de ordenar (melhor, médio e pior caso)"
        //
        // Melhor caso: alvo na posição 0   → 1 verificação (garantido)
        // Caso médio : alvo na posição n/2 → ~n/2 verificações
        // Pior caso  : alvo na posição n-1 → n verificações (garantido)
        cout << "\n[Busca Sequencial — vetor ORIGINAL (nao ordenado)]\n";

        int alvoMelhorSeq = fatia[0].zipCode;
        int alvoMedioSeq  = fatia[tamanho / 2].zipCode;
        int alvoPiorSeq   = -1;  // Elemento que NÃO existe no array

        vector<ResultadoBusca> resBuscaAntes = {
            buscaSequencial(fatia, alvoMelhorSeq, "Melhor"),
            buscaSequencial(fatia, alvoMedioSeq,  "Medio"),
            buscaSequencial(fatia, alvoPiorSeq,   "Pior")
        };
        for (const auto &r : resBuscaAntes) exibirBusca(r);
        salvarBuscaCSV(arquivoBusca, resBuscaAntes, tamanho);

        // ── ORDENAÇÃO ────────────────────────────────────────
        // Cada função recebe uma CÓPIA da fatia (passagem por valor)
        // Bubble Sort por último — mais lento com n²
        cout << "\n[Algoritmos de Ordenacao — chave: zipCode (int)]\n";

        vector<Resultado> resOrdenacao = {
            insertionSort(fatia),
            shellSort    (fatia),
            hoare        (fatia),
            mergeSort    (fatia),
            bubbleSort   (fatia)
        };
        for (const auto &r : resOrdenacao) exibirOrdenacao(r);
        salvarOrdenacaoCSV(arquivoOrdenacao, resOrdenacao, tamanho);

        // ── BUSCA BINÁRIA — vetor ORDENADO ───────────────────
        // Enunciado seção 4 item 3:
        // "Busca Binária APÓS a ordenação para os mesmos itens"
        //
        // Melhor caso bin.: alvo no centro     → 1 comparação
        // Caso médio bin. : alvo no 1º quartil → ~log2(n)/2 comparações
        // Pior caso bin.  : alvo na extremidade → ceil(log2(n)) comparações
        cout << "\n[Busca Binaria — vetor ORDENADO por zipCode]\n";

        vector<Registro> ordenado = mergeSortOrdenado(fatia);

        int alvoMelhorBin = ordenado[(tamanho - 1) / 2].zipCode;
        int alvoMedioBin  = ordenado[tamanho / 4].zipCode;
        int alvoPiorBin   = -1;

        vector<ResultadoBusca> resBuscaDepois = {
            buscaBinaria(ordenado, alvoMelhorBin, "Melhor"),
            buscaBinaria(ordenado, alvoMedioBin,  "Medio"),
            buscaBinaria(ordenado, alvoPiorBin,   "Pior")
        };
        for (const auto &r : resBuscaDepois) exibirBusca(r);
        salvarBuscaCSV(arquivoBusca, resBuscaDepois, tamanho);

        cout << "\n";
    }

    cout << "Resultados salvos em:\n";
    cout << "  -> " << arquivoOrdenacao << "\n";
    cout << "  -> " << arquivoBusca     << "\n";

    return 0;
}
