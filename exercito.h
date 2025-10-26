#ifndef EXERCITO_H
#define EXERCITO_H

#include <string>
#include "jogador.h"

class Exercito {
    private:
        std::string _nome;
        Jogador* _dono;
    public:
        Exercito (std::string nome, Jogador* dono);
        ~Exercito();
        
        std::string getNome() const;
        Jogador* getDono() const;     
        void setNome(std::string nome);
        void setDono(Jogador* jogador);
        std::string rolarDado();   // gera dois numeros aleatorios, compara e retorna vencedor (ataque ou defesa)
        virtual void ataque(Territorio* origem, Territorio* destino) = 0;
};  

#endif