#include "exercito.h"

#include <iostream>
#include <string>
#include <ctime>
#include <cstdlib>

#include "jogador.h"


// Construtor do exercito
Exercito::Exercito (std::string nome, Jogador* dono) : 
    _nome(nome), _dono(dono) {}

    
// Destrutor do exercito (nao precisa desalocar memoria dinamica)
Exercito::~Exercito() {}


// Retorna o nome do exercito
std::string Exercito::getNome() const {
    return _nome;
}


// Retorna o dono do exercito
Jogador* Exercito::getDono() const {
    return _dono;
}


// Atualiza o nome do exercito
void Exercito::setNome(std::string nome) {
    _nome = nome;
}


// Atualiza o dono do exercito
void Exercito::setDono(Jogador* dono) {
    _dono = dono;
}


std::string Exercito::rolarDado() {
    // Gerando numeros aleatorios entre 1 e 6 para o ataque e defesa
    int num_ataque, num_defesa;
    num_ataque = (rand() % 6) + 1;
    num_defesa = (rand() % 6) + 1;

    // Retornando ganhador
    if (num_defesa >= num_ataque) return "defesa";
    return "ataque";
}