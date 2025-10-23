#include "territorio.h"

#include <iostream>
#include <string>

#include "jogador.h"


// Construtor do Territorio, inicializando o atributo constante
Territorio::Territorio (std::string nome, int num_fronteiras) :
    _num_fronteiras(num_fronteiras) {
        // Inicializa demais atributos
        _nome = nome;
        _exercitos_aereos = 0; 
        _exercitos_terrestres = 0;
        _dono = nullptr;
        _fronteira_atual = 0;

        // Alocacao dinamica das fronteiras
        _fronteiras = new Territorio* [num_fronteiras];
        for (int i = 0; i < num_fronteiras; i++){
            _fronteiras[i] = nullptr;   
        }
    }


// Destrutor que desaloca memoria dinamica
Territorio::~Territorio() {
   // Ele nao tem responsabilidade de destruir cada ponteiro, apenas o array criado
   // Quem destruira os ponteiros de cada posicao sera quem alocar eles dinamicamente (classe Jogo)
    delete[] _fronteiras;
}


// Retorna o nome do territorio
std::string Territorio::getNome() const {
    return _nome;
}


// Retorna o numero de exercitos total do territorio
int Territorio::getExercitos() const {
    return _exercitos_terrestres + _exercitos_aereos;
}


// Retorna o numero de exercitos terrestres do territorio
int Territorio::getExercitosTerrestres() const {
    return _exercitos_terrestres;
}


// Retorna o numero de exercitos aereos do territorio
int Territorio::getExercitosAereos() const {
    return _exercitos_aereos;
}


// Retorna o array de ponteiros das fronteiras do territorio
Territorio** Territorio::getFronteiras() const {
    return _fronteiras;
}


// Atualiza o nome do territorio
void Territorio::setNome(std::string nome) {
    _nome = nome;
}


// Adiciona exercitos a um territorio especifico
void Territorio::adicionarExercitos(int qtd_exercitos, Territorio* territorio, const std::string& tipo) {
    if (tipo == "terrestre"){
        territorio->_exercitos_terrestres += qtd_exercitos;
    }
    else if (tipo == "aereo"){
        territorio->_exercitos_aereos += qtd_exercitos;
    }
}


// Remove exercitos de um territorio especifico
void Territorio::removerExercitos(int qtd_exercitos, Territorio* territorio, const std::string& tipo) {
    if (tipo == "terrestre"){
        // Remove so se conseguir
        if (territorio->_exercitos_terrestres - qtd_exercitos >= 0) territorio->_exercitos_terrestres -= qtd_exercitos;
        // Mensagem de erro, caso nao seja possivel
        else std::cerr << "Nao foi possivel remover essa quantidade de exercitos do territorio selecionado" << std::endl;
    }
    else if (tipo == "aereo"){
        // Remove so se conseguir
        if (territorio->_exercitos_aereos - qtd_exercitos >= 0) territorio->_exercitos_aereos -= qtd_exercitos;
        // Mensagem de erro, caso nao seja possivel
        else std::cerr << "Nao foi possivel remover essa quantidade de exercitos do territorio selecionado" << std::endl;
    }
}


// Adiciona as fronteiras do territorio
void Territorio::adicionarFronteira(Territorio* fronteira) {
    // Mensagem de erro caso esteja no limite de fronteiras adicionadas no territorio
    if (_fronteira_atual >= _num_fronteiras) std::cerr << "Nao e possivel adicionar novas fronteiras" << std::endl;

    // Adiciona, se possivel, a nova fronteira ao array de ponteiros e incrementa o indice
    else {
        this->_fronteiras[_fronteira_atual] = fronteira;
        _fronteira_atual++;
    }
}


// Atualiza o dono do territorio atual
void Territorio::setDono(Jogador* dono){
    _dono = dono;
}


// Retorna o dono atual do territorio
Jogador* Territorio::getDono() const {
    return _dono;
}


// Retorno o numero de fronteiras
int Territorio::getNumFronteiras() const {
    return _fronteira_atual;
}


// Checa se territorios fazem fronteira
bool Territorio::fazFronteira(Territorio* destino) const {
    for (int i = 0; i < this->getNumFronteiras(); i++){
        if (this->_fronteiras[i] == destino) {
            return true;    // Encontrou fronteira
        }
    }
    return false;   // Nao encontrou fronteira
}
