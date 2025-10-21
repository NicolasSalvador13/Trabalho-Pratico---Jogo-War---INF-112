#ifndef CONTINENTE_H
#define CONTINENTE_H

#include <string>
#include "territorio.h"

class Continente {
    private:
        std::string _nome;
        const int _num_territorios;
        int _qtd_terr;      // sera indice do array de ponteiros
        Territorio** _territorios;      // array de ponteiros

    public:
        Continente (std::string nome, int num_territorios);
        ~Continente();
        
        std::string getNome() const;
        Territorio** getTerritorios() const;   
        void setNome(std::string nome);
        void adicionarTerritorio(Territorio* territorio);
        void removerTerritorio(Territorio* territorio);
        int getNumTerritorios() const;
};  

#endif