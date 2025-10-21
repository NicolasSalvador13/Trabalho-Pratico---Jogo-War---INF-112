#include "jogador.h"

#include <iostream>
#include <string>

#include "territorio.h"


// Construtor, que inicializa atributo constante
Jogador::Jogador (std::string nome) :
    _nome(nome) {
        // Inicializacao dos demais atributos
        _num_territorios = 0;
        _capacidade_max_territorios = 1;

        // Ponteiros com nullptr, como boa pratica
        _carta = nullptr;

        // Alocacao dinamica de uma posicao do array de ponteiros
        _territorios = new Territorio* [_capacidade_max_territorios];
        _territorios[0] = nullptr;
    }


// Destrutor
Jogador::~Jogador(){
    // Destruo apenas o array de ponteiros do jogador
    // Cada ponteiro sera destruido por quem o iniciar (classe Jogo)
    delete[] _territorios;
}


// Retorna o numero atual de territorios do jogador
int Jogador::getNumTerritorios() const {
    return _num_territorios;
}


// A partir da classe carta, eu acesso o objetivo
std::string Jogador::getObjetivo() const {
    // Se carta ja foi atribuida, aciono metodo de obter objetivo da classe Carta (delegacao)
    if (_carta != nullptr) {
        return _carta->getObjetivo(); // retorna string com o objetivo da carta
    }
    // Se carta ainda nao foi atribuida ao jogador, retorna string de erro
    return "Nenhum objetivo definido.";
}  


// Atribui uma carta ao jogador
void Jogador::setCarta(Carta* carta) {
    _carta = carta;
}


// Adiciona determinado territorio ao jogador, especificando o numero de exercitos
void Jogador::adicionarTerritorio(int n_exercitos, Territorio* territorio) {
    // Mensagem de erro, caso numero invalido de exercitos
    if (n_exercitos <= 0) std::cerr << "Para adicionar um territorio, e necessario haver pelo menos um exercito nele" << std::endl;
    else {
        // Dobra capacidade do array de ponteiros se necessario
        if ((_num_territorios >= _capacidade_max_territorios)) dobraArrayTerritorios();

        // Adiciona novo territorio e incrementa indice
        _territorios[_num_territorios] = territorio;
        _num_territorios++;

        // Adiciona exercitos ao territorio
        territorio->adicionarExercitos(n_exercitos, territorio, "terrestre");

        // Atualiza dono do territorio
        territorio->setDono(this);
    }
}


// Dobra o tamanho do array de ponteiros com os territorios do jogador
void Jogador::dobraArrayTerritorios(){
    // Dobra capacidade maxima e armazena em uma variavel
    int nova_capacidade_max = _capacidade_max_territorios * 2;

    // Cria novo array de ponteiros com o dobro de capacidade
    Territorio** territorios_temp = new Territorio* [nova_capacidade_max];

    // Copia informacoes do array anterior
    for (int i = 0; i < _capacidade_max_territorios; i++) {
        territorios_temp[i] = _territorios[i];
    }

    // Deleta array anterior
    delete[] _territorios;

    // Atribui array temporario ao atual
    _territorios = territorios_temp;

    // Atualiza capacidade maxima
    _capacidade_max_territorios = nova_capacidade_max;
}


// Retorna o nome do jogador
std::string Jogador::getNome() const {
    return _nome;
}


// Retorna os territorios atuais do jogador
Territorio** Jogador::getTerritorios() const {
    return _territorios;
}

