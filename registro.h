#ifndef REGISTRO_H
#define REGISTRO_H

#include <string>

// Dataset: Olist E-Commerce Brazilian Dataset (Kaggle)
// Arquivo: olist_customers_dataset.csv
// 99.441 clientes reais de e-commerce brasileiro
//
// Colunas do CSV:
//   [0] customer_id                → id do registro/pedido
//   [1] customer_unique_id         → id único do cliente
//   [2] customer_zip_code_prefix   → zipCode (int) ← CHAVE DE ORDENAÇÃO
//   [3] customer_city              → city (string) ← CHAVE DE BUSCA
//   [4] customer_state             → state (string)
//
// Chave principal: zipCode (int — prefixo de CEP de 5 dígitos)
//   - Alta variância: milhares de CEPs distintos em todo o Brasil
//   - Narrativa: "ordenar base de clientes por CEP para roteirização de entregas"

struct Registro {
    int             id;                  // índice sequencial interno
    std::string     customer_id;         // id do registro/pedido
    std::string     customer_unique_id;  // id único do cliente
    std::string     city;                // cidade do cliente
    std::string     state;               // estado (UF)
    int             zipCode;             // CEP ← chave de ordenação
};

#endif
