#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <iomanip>
#include "registro.h"
#include "ordenacao.h"
#include "busca.h"

using namespace std;

// Leitura do CSV — Olist Customers Dataset

// Colunas:
//   [0] customer_id                → ignorado na ordenação
//   [1] customer_unique_id         → código único do cliente
//   [2] customer_zip_code_prefix   → zipCode (int) ← CHAVE
//   [3] customer_city              → city
//   [4] customer_state             → state

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
    getline(arquivo, linha);

    int id = 0;
    while (getline(arquivo, linha)) {
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
        campos.push_back(limparAspas(campo));

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
        f << r.algoritmo << "," << total << ","
          << r.verificacoes << ","
          << r.caso << ","
          << fixed << setprecision(6) << r.tempo << "\n";
}

void exibirOrdenacao(const Resultado &r) {
    cout << "  " << left  << setw(20) << r.algoritmo
         << " | comp: "   << setw(12) << r.comparacoes
         << " | trocas: " << setw(10) << r.trocas
         << " | tempo: "  << fixed << setprecision(4) << r.tempo << "s\n";
}

void exibirBusca(const ResultadoBusca &r) {
    cout << "  " << left << setw(12) << r.algoritmo
         << " | verif: " << setw(8) << r.verificacoes
         << " | caso: " << setw(12) << r.caso
         << " | tempo: " << fixed << setprecision(6) << r.tempo << "s\n";
}

int main() {
    const string arquivoDataset   = "olist_customers_dataset.csv";
    const string arquivoOrdenacao = "resultado_ordenacao.csv";
    const string arquivoBusca     = "resultado_busca.csv";

    ofstream(arquivoOrdenacao) << "Algoritmo,Total,Comparacoes,Trocas,Tempo(s)\n";
    ofstream(arquivoBusca)     << "Algoritmo,Total,Caso,Verificacoes,Tempo(s)\n";

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

        vector<Registro> fatia(dadosCompletos.begin(),
                               dadosCompletos.begin() + tamanho);

        cout << "BLOCO: " << tamanho << " registros\n";
        cout << "========================================\n";

        cout << "\n[Busca — vetor ORIGINAL (nao ordenado)]\n";

        int alvoPrimeiroAntes = fatia[0].zipCode;
        int alvoMeioAntes = fatia[(tamanho - 1) / 2].zipCode;
        int alvoInexistenteAntes = -1;

        vector<ResultadoBusca> buscaAntes = {
            buscaSequencial(fatia, alvoPrimeiroAntes, "Primeiro"),
            buscaSequencial(fatia, alvoMeioAntes, "Meio"),
            buscaSequencial(fatia, alvoInexistenteAntes, "Inexistente"),

            buscaBinaria(fatia, alvoPrimeiroAntes, "Primeiro"),
            buscaBinaria(fatia, alvoMeioAntes, "Meio"),
            buscaBinaria(fatia, alvoInexistenteAntes, "Inexistente")
        };
        for (const auto &r : buscaAntes) exibirBusca(r);
        salvarBuscaCSV(arquivoBusca, buscaAntes, tamanho);

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

        cout << "\n[Busca — vetor ORDENADO por zipCode]\n";

        vector<Registro> ordenado = mergeSortOrdenado(fatia);

        int alvoPrimeiroDepois = ordenado[0].zipCode;
        int alvoMeioDepois = ordenado[(tamanho - 1) / 2].zipCode;
        int alvoInexistenteDepois = -1;

        vector<ResultadoBusca> buscaDepois = {
            buscaSequencial(ordenado, alvoPrimeiroDepois, "Primeiro"),
            buscaSequencial(ordenado, alvoMeioDepois, "Meio"),
            buscaSequencial(ordenado, alvoInexistenteDepois, "Inexistente"),

            buscaBinaria(ordenado, alvoPrimeiroDepois, "Primeiro"),
            buscaBinaria(ordenado, alvoMeioDepois, "Meio"),
            buscaBinaria(ordenado, alvoInexistenteDepois, "Inexistente")
        };

        for (const auto &r : buscaDepois) exibirBusca(r);
        salvarBuscaCSV(arquivoBusca, buscaDepois, tamanho);

        cout << "\n";
    }

    cout << "Resultados salvos em:\n";
    cout << "  -> " << arquivoOrdenacao << "\n";
    cout << "  -> " << arquivoBusca     << "\n";

    return 0;
}
