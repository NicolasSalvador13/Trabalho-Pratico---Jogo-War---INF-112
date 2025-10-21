#ifndef EXERCITO_TERRESTRE_H
#define EXERCITO_TERRESTRE_H

#include "exercito.h"
#include "jogador.h"
#include <string>

class ExercitoTerrestre : public Exercito {
    public:
        ExercitoTerrestre (std::string nome, Jogador* dono);
        ~ExercitoTerrestre();

        void ataque(Territorio* origem, Territorio* destino) override;
};  

#endif