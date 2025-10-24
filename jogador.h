#ifndef JOGADOR_H
#define JOGADOR_H

#include <string>
#include "territorio.h"
#include "carta.h"

class Jogador {
    private:
        const std::string _nome;
        int _num_territorios; // Funcionara tambem como indice do array de ponteiros (territorios)
        int _capacidade_max_territorios; // Flag para dobrar array de ponteiros
        Carta* _carta;
        Territorio** _territorios;
        bool _eliminado;
        
        void dobraArrayTerritorios ();
    public:
        Jogador (std::string nome);
        ~Jogador();
        int getNumTerritorios() const;
        std::string getObjetivo() const;
        std::string getNome() const;
        Territorio** getTerritorios() const;

        void setCarta(Carta* carta);
        void adicionarTerritorio(int n_exercitos, Territorio* territorio);
        void removerTerritorio(Territorio* territorio);

        void setEliminado(bool status);
        bool estaEliminado() const;
};  

#endif