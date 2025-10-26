#include <iostream>
#include <string>
#include <fstream>   
#include <sstream>   

#include "jogo.h"

// --- FUNCAO AUXILIAR ---
// Le a primeira linha de territorios.txt para obter os parametros do jogo
// Retorna 'true' se for bem-sucedida, 'false' se houver erro
bool lerParametrosDoJogo(int& num_territorios, int& num_continentes, int& num_jogadores, int& num_objetivos) {
    std::ifstream arquivo("./territorios.txt");
    if (!arquivo.is_open()) {
        std::cerr << "ERRO FATAL: Nao foi possivel abrir o arquivo 'territorios.txt'.\n";
        return false;
    }

    // Le primeira linha e separa informacoes nas variaveis correspondentes
    std::string primeira_linha;
    if (getline(arquivo, primeira_linha)) {
        std::stringstream ss(primeira_linha);
        std::string item;
        
        getline(ss, item, ','); num_continentes = std::stoi(item);
        getline(ss, item, ','); num_territorios = std::stoi(item);
        getline(ss, item, ','); num_jogadores = std::stoi(item);
        getline(ss, item, ','); num_objetivos = std::stoi(item);
        
        arquivo.close();
        return true;
    }

    arquivo.close();
    return false; // Falha ao ler a primeira linha
}

// --- FUNCAO PRINCIPAL ---
int main() {
    // Garante aleatoriedade do metodo rolarDados()
    srand(time(0));

    std::cout <<   "==================================";
    std::cout << "\n===== BEM-VINDO AO JOGO WAR! =====\n";
    std::cout <<   "==================================";


    // --- CONFIGURACAO INICIAL ---
    int num_territorios, num_continentes, num_jogadores, num_objetivos;

    // Tenta ler os parametros do arquivo
    // Isso garante uma chamada correta do construtor do Jogo
    if (!lerParametrosDoJogo(num_territorios, num_continentes, num_jogadores, num_objetivos)) {
        return 1; // Encerra se o arquivo nao puder ser lido
    }

    // --- CRIACAO E ORGANIZACAO DO JOGO ---

    // Cria o objeto Jogo com os parametros corretos
    Jogo war(num_jogadores, num_territorios, num_continentes, num_objetivos);

    std::cout << "\nOrganizando o tabuleiro, distribuindo territorios e objetivos...\n";
    war.organizarJogo();
    std::cout << "O jogo esta pronto para comecar!\n";

    // --- LOOP PRINCIPAL DO JOGO ---

    // String para armazenar nome do ganhador
    std::string ganhador;
    // O loop continua enquanto fimDoJogo() retornar "nao"
    while ((ganhador = war.fimDoJogo()) == "nao") {

        // Inicia novas rodadas enquanto jogo nao terminar
        war.iniciarJogada(war.getJogadorDaVez());
    }

    // --- FIM DE JOGO ---
    std::cout << "\n\n===== FIM DE JOGO! =====\n";
    std::cout << "O jogador " << ganhador << " cumpriu seu objetivo e venceu a partida!\n";
    std::cout << "Parabens!\n";

    return 0;
}