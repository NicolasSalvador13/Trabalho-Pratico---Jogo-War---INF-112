#ifndef EXERCITO_AEREO_H
#define EXERCITO_AEREO_H

#include "exercito.h"
#include "jogador.h"
#include <string>

class ExercitoAereo : public Exercito {
   public:
        ExercitoAereo (std::string nome, Jogador* dono);
        ~ExercitoAereo();

        void ataque(Territorio* origem, Territorio* destino) override;
};  

#endif