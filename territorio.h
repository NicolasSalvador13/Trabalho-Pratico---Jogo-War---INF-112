#ifndef TERRITORIO_H
#define TERRITORIO_H

#include <string>

class Jogador;

class Territorio {
    private:
        std::string _nome;
        const int _num_fronteiras;
        int _exercitos_aereos;
        int _exercitos_terrestres;
        Jogador* _dono;
        Territorio** _fronteiras; // array de ponteiros
        int _fronteira_atual; // Funcionara como indice do array de ponteiros

    public:
        Territorio (std::string nome, int num_fronteiras);
        ~Territorio();
        
        std::string getNome() const;
        void setNome(std::string nome);
        void setDono(Jogador* dono);
        Jogador* getDono() const;

        Territorio** getFronteiras() const;
        int getNumFronteiras() const;  
        bool fazFronteira(Territorio* destino) const;

        int getExercitos() const;   // retorna o total de exercitos no territorio
        int getExercitosTerrestres() const;     // retorna o total de exercitos terrestres no territorio
        int getExercitosAereos() const;     // retorna o total de exercitos aereos no territorio
        void adicionarExercitos(int qtd_exercitos, Territorio* territorio, const std::string& tipo);  // Adicionado tipo (terrestre ou aereo)
        void removerExercitos(int qtd_exercitos, Territorio* territorio, const std::string& tipo);
        void adicionarFronteira(Territorio* fronteira);

};  

#endif