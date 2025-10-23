#include "exercitoaereo.h"

#include <iostream>

#include "exercito.h"
#include "territorio.h"
#include "jogador.h"


// Construtor do Exercito Aereo (herda da classe Exercito)
ExercitoAereo::ExercitoAereo(std::string nome, Jogador* dono) : Exercito(nome, dono) {}


// Destrutor (nao necessario desalocar memoria dinamica)
ExercitoAereo::~ExercitoAereo() {}


void ExercitoAereo::ataque(Territorio* origem, Territorio* destino) {
    // Tratamento de erros
    // Auto ataque
    if (origem->getDono() == destino->getDono()) {
        std::cerr << "Nao e possivel atacar o proprio territorio" << std::endl;
        return;
    }
    // Sem exercitos suficientes
    if (origem->getExercitos() < 2){
        std::cerr << "Deve haver no minimo 2 exercitos no territorio para fazer o ataque" << std::endl;
        return;
    }
    // Sem exercito aereo
    if (origem->getExercitosAereos() < 1) {
        std::cerr << "Ataque aereo invalido. O territorio de origem nao possui exercitos aereos" << std::endl;
        return;
    }
    
    // Rolagem de dados
    // Defesa ganha
    if (rolarDado() == "defesa") {

        // FEEDBACK AQUI
        std::cout << ">> A defesa foi bem-sucedida! O atacante perdeu 1 exercito aereo em " << origem->getNome() << ".\n";

        // Remove exercito do territorio atacante
        origem->removerExercitos(1, origem, "aereo");

    } 
    // Ataque ganha
    else {

        // FEEDBACK AQUI
        std::cout << ">> Ataque bem-sucedido! O defensor perdeu 1 exercito terrestre em " << destino->getNome() << ".\n";
        // Remove exercito do territorio na defesa
        destino->removerExercitos(1, destino, "terrestre");

        // Checa se conquistou
        if (destino->getExercitos() == 0) { 

            // FEEDBACK AQUI
            std::cout << ">> O territorio " << destino->getNome() << " foi conquistado!\n";

            // Guarda ponteiros dos jogadores envolvidos
            Jogador* conquistador = origem->getDono();
            Jogador* perdedor = destino->getDono();

            // Troca o dono do territorio
            destino->setDono(conquistador);

            if (perdedor != nullptr) { // Garante que o território tinha um dono
                perdedor->removerTerritorio(destino); 
            }
            // Adiciona uma tropa TERRESTRE no novo territorio conquistado
            conquistador->adicionarTerritorio(1, destino);

            // Adiciona uma tropa AEREA no novo territorio conquistado
            destino->adicionarExercitos(1, destino, "aereo");

            // Remove uma tropa TERRESTRE no novo territorio conquistado
            destino->removerExercitos(1, destino, "terrestre");

            // Remove uma tropa do territorio atacante
            origem->removerExercitos(1, origem, "aereo");
        }
    }
}


