#include "exercitoterrestre.h"

#include <iostream>

#include "exercito.h"
#include "territorio.h"
#include "jogador.h"


// Construtor do Exercito Terrestre (herda da classe Exercito)
ExercitoTerrestre::ExercitoTerrestre(std::string nome, Jogador* dono) : Exercito(nome, dono) {}


// Destrutor (nao necessario desalocar memoria dinamica)
ExercitoTerrestre::~ExercitoTerrestre() {}


// Ataque terrestre
void ExercitoTerrestre::ataque(Territorio* origem, Territorio* destino) {
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
    // Se nao for fronteira, mensagem de erro
    if (!origem->fazFronteira(destino)) {
        std::cerr << "O territorio de destino nao faz fronteira com o de origem" << std::endl;
        return; 
    }
    // Se continuou aqui, entao e fronteira

    // Rolagem de dados
    // Defesa ganha
    if (rolarDado() == "defesa") {

        // FEEDBACK AQUI
        std::cout << ">> A defesa foi bem-sucedida! O atacante perdeu 1 exercito terrestre em " << origem->getNome() << ".\n";

        // Remove exercito do territorio atacante
        origem->removerExercitos(1, origem, "terrestre");

    } 
    // Ataque ganha
    else {

        // Remove tropa terrestre do territorio na defesa
        if (destino->getExercitosTerrestres() > 0){
            // FEEDBACK AQUI
            std::cout << ">> Ataque bem-sucedido! O defensor perdeu 1 exercito terrestre em " << destino->getNome() << ".\n";

            // Remove exercito do territorio na defesa
            destino->removerExercitos(1, destino, "terrestre");
        }
        // Remove tropa aérea do territorio na defesa
        else if (destino->getExercitosAereos() > 0) {
            std::cout << ">> Ataque bem-sucedido! O defensor perdeu 1 exercito aereo em " << destino->getNome() << ".\n";

            // Remove exercito do territorio na defesa
            destino->removerExercitos(1, destino, "aereo");
        }

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
            // Adiciona uma tropa no novo territorio conquistado
            conquistador->adicionarTerritorio(1, destino);

            // Remove uma tropa do territorio atacante
            origem->removerExercitos(1, origem, "terrestre");

            
        }
    }
}
