#ifndef JOGO_H
#define JOGO_H

#include <string>

#include "jogador.h"
#include "territorio.h"
#include "continente.h"
#include "carta.h"
#include "exercito.h"
#include "exercitoterrestre.h"
#include "exercitoaereo.h"

class Jogo {
    private:
        const int _num_jogadores;   
        const int _num_territorios;
        const int _num_continentes;
        const int _num_objetivos;
        static const int BONUS_AEREO_POR_CONTINENTE = 5;
        int _indice_jogador_atual;
        Jogador** _jogadores;
        Territorio** _territorios;
        Continente** _continentes;
        Carta** _cartas;
        void carregarMapa(const std::string &arquivo_territorios);
        Territorio* encontrarTerritorioPorNome(const std::string &nome_do_territorio) const;
        bool jogadorControlaContinente(Jogador* jogador, const std::string& nome_continente) const;
        void checarInfoTerritorios(const std::string& nome_territorio) const;
        Jogador* encontrarJogadorPorNome(const std::string &nome_do_jogador) const;
        Continente* encontrarContinentePorNome(const std::string& nome_continente) const;
        char obterConfirmacao(const std::string& prompt) const; // Le do usuario ('s' ou 'n')
        int obterInt(const std::string& prompt) const;  // Le inteiro valido
        void verMapaDeGuerra (Jogador* jogador) const;  // Mostra mapa de guerra
        std::string obterString (const std::string& prompt) const; // Le uma string valida (nao vazia)
        void faseDeReforco(Jogador* jogador_da_vez);
        void posicionarReforcos(Jogador* jogador, int quantidade_total, const std::string& tipo_tropa);
        void faseDeAtaque(Jogador* jogador_da_vez);
        void executarAtaque(Jogador* jogador_da_vez);
        void executarChecagemDeInfo() const;
        void faseDeMovimentacao(Jogador* jogador);
        void executaEliminacao(Jogador* conquistador, Jogador* perdedor);
        void pressioneEnterParaContinuar() const;   // Pausa na visualizacao do mapa de guerra

    public:
        Jogo (int n_jogadores, int n_territorios, int n_continentes, int n_objetivos);
        ~Jogo();

        Jogador* getJogadorDaVez() const;
        void setJogadorDaVez(Jogador* jogador);
        void proximoJogador();

        void iniciarJogada(Jogador* jogador_da_vez);
        std::string fimDoJogo();
        void distribuirExercitos(int n_exercitos, std::string nome_territ_origem, std::string nome_territ_dest, const std::string& tipo);
        void distribuirExercitos(int n_exercitos, std::string nome_territorio, Jogador* jogador, const std::string& tipo);
        void organizarJogo();
};      

#endif