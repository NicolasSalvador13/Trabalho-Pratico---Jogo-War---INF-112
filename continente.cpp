#include "continente.h"

#include <iostream>
#include <string>

#include "territorio.h"


// Construtor que inicializa atributos 
Continente::Continente (std::string nome, int num_territorios) :
    _nome(nome), _num_territorios(num_territorios) {

        // Inicializa atributo que sera indice
        _qtd_terr = 0;

        // Aloca dinamicamente espaco para territorios do continente
        _territorios = new Territorio* [_num_territorios];
        for (int i = 0; i < _num_territorios; i++) {
            _territorios[i] = nullptr;
        }
    }


// Destrutor  
Continente::~Continente() {
    
    // Deleta array
    delete[] _territorios;
    
    // Nao e necessario deletar cada ponteiro do array, pois quem fara isso sera a classe Jogo
}


// Retorna o nome do continente
std::string Continente::getNome() const {
    return _nome;
}


// Retorna o vetor de territorios
Territorio** Continente::getTerritorios() const {
    return _territorios;
}


// Atualiza o nome do continente
void Continente::setNome(std::string nome) {
    _nome = nome;
}


// Adiciona um territorio ao continente
void Continente::adicionarTerritorio(Territorio* territorio) {

    // Tratamento de erros (excedeu o limite de territorios)
    if (_qtd_terr >= _num_territorios) {
        std::cerr << "Ja chegou ao limite maximo de territorios para o continente " << this->_nome << std::endl;
        return;
    }

    // Adiciona novo territorio e incrementa indice
    this->_territorios[_qtd_terr] = territorio;
    _qtd_terr++;
}


// Remove um territorio do continente
void Continente::removerTerritorio(Territorio* territorio) {

    // Indice do territorio a ser removido
    int indice_para_remover = -1;

    // Encontra o indice do territorio a ser removido
    for (int i = 0; i < _qtd_terr; i++) {
        if (_territorios[i] == territorio) {
            indice_para_remover = i;
            break; 
        }
    }

    // Se o territorio foi encontrado no array
    if (indice_para_remover != -1) {

        // Move todos os elementos apos o indice uma posicao para a esquerda
        for (int i = indice_para_remover; i < _qtd_terr - 1; i++) {
            _territorios[i] = _territorios[i + 1];
        }

        // Decrementa o contador de territorios
        _qtd_terr--;

        // Limpa a ultima posicao que ficou duplicada
        _territorios[_qtd_terr] = nullptr; 
    }
}


// Retorna o numero de territorios do continente
int Continente::getNumTerritorios() const {
    return _num_territorios;
}
