#include "jogo.h"

#include <string>
#include <iostream>
#include <cstring>
#include <sstream>
#include <fstream>
#include <algorithm>
#include <random>
#include <ctime>
#include <iomanip>
#include <limits>   // std::ws (limpar \n e espaços anteriores ao ler com cin)
#include <cctype>   // tolower()

#include "jogador.h"
#include "territorio.h"
#include "continente.h"
#include "carta.h"
#include "exercito.h"
#include "exercitoterrestre.h"
#include "exercitoaereo.h"

// Constantes para cores ANSI
const std::string RESET = "\033[0m";
const std::string YELLOW = "\033[1;33m"; // Amarelo em negrito
const std::string CYAN = "\033[0;36m";
const std::string GREEN = "\033[0;32m";
const std::string RED = "\033[1;31m";   // Vermelho em negrito
const std::string WHITE = "\033[0;37m"; // Branco normal para as fronteiras

// Construtor que inicializa atributos constantes
Jogo::Jogo (int n_jogadores, int n_territorios, int n_continentes, int n_objetivos) :
    _num_jogadores(n_jogadores), _num_territorios(n_territorios), _num_continentes(n_continentes), _num_objetivos(n_objetivos) {

        // Inicializa demais atributos
        _indice_jogador_atual = 0;
        
        // Alocacao dinamica de arrays
        // JOGADORES
        _jogadores = new Jogador* [_num_jogadores];
        for (int i = 0; i < _num_jogadores; i++) {
            _jogadores[i] = nullptr;
        }

        // TERRITORIOS
        _territorios = new Territorio* [_num_territorios];
        for (int i = 0; i < _num_territorios; i++) {
            _territorios[i] = nullptr;
        }

        // CONTINENTES
        _continentes = new Continente* [_num_continentes];
        for (int i = 0; i < _num_continentes; i++) {
            _continentes[i] = nullptr;
        }

        // CARTAS
        _cartas = new Carta* [_num_objetivos];
        for (int i = 0; i < _num_objetivos; i++) {
            _cartas[i] = nullptr;
        }
    }


// Destrutor
Jogo::~Jogo() {
    // Destruindo ponteiros e arrays
    // JOGADOR
    for (int i = 0; i < _num_jogadores; i++) {
            delete _jogadores[i];
        }
    delete[] _jogadores;

    // TERRITORIO
    for (int i = 0; i < _num_territorios; i++) {
            delete _territorios[i];
        }
    delete[] _territorios;

    // CONTINENTES
    for (int i = 0; i < _num_continentes; i++) {
            delete _continentes[i];
        }
    delete[] _continentes;

    // CARTAS
    for (int i = 0; i < _num_objetivos; i++) {
        delete _cartas[i];
    }
    delete[] _cartas;   
}


// Retorna jogador da vez
Jogador* Jogo::getJogadorDaVez() const {

    // Usa o atributo de indice para acessar o array de jogadores
    if (_jogadores != nullptr && _indice_jogador_atual < _num_jogadores) {
        return _jogadores[_indice_jogador_atual];
    }
    return nullptr; // Em caso de erro, retorna nullptr
}


// Atualiza jogador da vez
void Jogo::setJogadorDaVez(Jogador* jogador) {

    // Itera sobre o array de jogadores, ate achar o jogador informado
    for (int i = 0; i < _num_jogadores; i++) {
        if (_jogadores[i] == jogador) {
            _indice_jogador_atual = i;  // Atualiza o indice do jogador dentro do array
            return;
        }
    }
    // Agora o jogador atual e decidido com base nesse indice
}


// Passa a vez para o proximo jogador, com base no indice
void Jogo::proximoJogador() {
    
    // Contagem de jogadores ativos
    int jogadores_ativos = 0;

    // Itera sobre jogadores ativos e incrementa contador
    for(int i = 0; i < _num_jogadores; i++) {
        if (!_jogadores[i]->estaEliminado()) {
            jogadores_ativos++;
        }
    }

    // Se so resta 1 jogador ou menos, o jogo acabou, nao mude o turno
    if (jogadores_ativos <= 1) {
        return;
    }

    // Loop do-while para garantir que sempre avance pelo menos 1
    // e continue procurando ate achar alguem nao eliminado
    do {
        _indice_jogador_atual = (_indice_jogador_atual + 1) % _num_jogadores;
    } while (_jogadores[_indice_jogador_atual]->estaEliminado());
}


// Coordena acoes relacionadas a vez do jogador
void Jogo::iniciarJogada(Jogador* jogador_da_vez) {

    // ==============================
    // FASE 1: EXERCITOS DE REFORCO
    // ==============================
    faseDeReforco(jogador_da_vez);

    // ==============================
    // FASE 2: ATAQUE
    // ==============================
    faseDeAtaque(jogador_da_vez);

    // ==============================
    // FASE 3: MOVIMENTAÇÃO
    // ==============================
    faseDeMovimentacao(jogador_da_vez); 

    // ... finaliza o turno ...
    std::cout << "\n--- Finalizando o turno de " << jogador_da_vez->getNome() << ". ---\n" << std::endl;
    proximoJogador();
}


// Metodo auxiliar para executar toda fase de movimentacao
void Jogo::faseDeMovimentacao(Jogador* jogador){

    std::cout << "\n===== FASE DE MOVIMENTACAO ESTRATEGICA =====\n";

    // Exibe informacoes de todos territorios e exercitos do jogador
    verMapaDeGuerra(jogador);

    char deseja_movimentar;
    std::string nome_origem, nome_destino;
    int tipo_tropa, qtd_exercitos;
    Territorio* origem = nullptr;
    Territorio* destino = nullptr;

    deseja_movimentar = obterConfirmacao("Deseja movimentar? (s/n): ");

    // Se nao quer movimentar, finaliza turno
    if (deseja_movimentar == 's' || deseja_movimentar == 'S') {

        // Loop para validar entradas do jogador
        while (true) {
           
            // --- Coleta de Informacoes da movimentacao ---
            std::cout << "Digite o nome do territorio de origem: ";
            std::getline(std::cin >> std::ws, nome_origem);     // std::ws limpa \n anterior (alternativa do cin.ignore())  
            std::cout << "Digite o nome do territorio de destino: ";
            std::getline(std::cin >> std::ws, nome_destino);
            std::cout << "Digite o tipo de tropa (1 para Terrestre, 2 para Aereo): ";
            std::cin >> tipo_tropa;
            std::cout << "Digite a quantidade de exercitos: ";
            std::cin >> qtd_exercitos;

            // VERIFICA SE A LEITURA DOS NUMEROS FALHOU (ex: usuario digitou uma letra)
            if (std::cin.fail()) {
                std::cerr << "ERRO: Entrada numerica invalida. Tente novamente.\n";
                std::cin.clear(); // Limpa o estado de erro do cin
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // Descarta a linha invalida
                continue; // Volta para o inicio do loop de validacao
            }

            // --- Validacao e Preparacao ---
            origem = encontrarTerritorioPorNome(nome_origem);
            destino = encontrarTerritorioPorNome(nome_destino);

            if (tipo_tropa != 1 && tipo_tropa != 2) {
                std::cerr << "ERRO: Tipo de tropa invalida. Tente novamente.\n";
                continue; // Volta para o inicio do loop while
            }
            else if (qtd_exercitos < 1) {
                std::cerr << "ERRO: Quantidade de exercitos invalida. Tente novamente.\n";
                continue;
            }
            else if (origem == nullptr || destino == nullptr) {
                std::cerr << "ERRO: Territorio de origem ou destino invalido. Tente novamente.\n";
                continue; 
            }
            else if (origem->getDono() != jogador) {
                std::cerr << "ERRO: Voce nao e o dono do territorio de origem. Tente novamente.\n";
                continue; 
            }
            else if (destino->getDono() != jogador) {
                std::cerr << "ERRO: Voce nao e o dono do territorio de destino. Tente novamente.\n";
                continue;
            }
            else if (origem->getExercitosAereos() < qtd_exercitos && tipo_tropa == 2) {
                std::cerr << "ERRO: Voce nao tem tropas aereas suficientes para movimentar. Tente novamente.\n";
                continue;
            }
            else if (origem->getExercitosTerrestres() < qtd_exercitos && tipo_tropa == 1) {
                std::cerr << "ERRO: Voce nao tem tropas terrestres suficientes para movimentar. Tente novamente.\n";
                continue;
            }
            else if (origem->getExercitos() <= qtd_exercitos) {
                std::cerr << "ERRO: Pelo menos 1 exercito deve permanecer na origem.\n";
                continue;
            }
            else if (!origem->fazFronteira(destino) && tipo_tropa == 1) {
                std::cerr << "ERRO: O territorio de destino nao faz fronteira com o de origem. Tente novamente.\n";
                continue;
            }
            else break;
        }
    
        std::string tipo = (tipo_tropa == 1)?"terrestre":"aereo";
        // Chama o metodo de movimentacao
        distribuirExercitos(qtd_exercitos, origem->getNome(), destino->getNome(), tipo);
    }

    std::cout << "\n--- Fim da fase de movimentacao. ---" << std::endl;
}


// Metodo auxiliar para executar checagem de informacoes
void Jogo::executarChecagemDeInfo() const {

    // Informacoes do territorio = 1, do jogador = 2
    int tipo_info = 0;
    tipo_info = obterInt("Checar info de Territorio (1) ou Jogador (2)? ");
    
    // Checa as informacoes solicitadas
    if (tipo_info == 1) {
        std::string nome_t = obterString("Digite o nome do territorio: ");
        checarInfoTerritorios(nome_t);
    } else if (tipo_info == 2) {
        std::string nome_j = obterString("Digite o nome do jogador: ");
        Jogador* jogador = nullptr;
        jogador = encontrarJogadorPorNome(nome_j);
        if (jogador != nullptr) verMapaDeGuerra(jogador);
        else std::cout << "Jogador nao encontrado\n";
    } else {
        std::cerr << "ERRO: Opcao invalida.\n";
    }
}


// Metodo auxiliar para executar o ataque
void Jogo::executarAtaque(Jogador* jogador_da_vez) {
    std::cout << "\n===== NOVO ATAQUE =====\n";
    std::string nome_origem, nome_destino, input_str;
    int tipo_ataque_int;
    Territorio* origem = nullptr;
    Territorio* destino = nullptr;
    const std::string CANCELAR_KEYWORD = "cancelar";

    // Loop para obter uma ORIGEM valida ou cancelar
    while(true) {
        nome_origem = obterString("Digite o nome do territorio de origem (ou '" + CANCELAR_KEYWORD + "'): ");
        
        // Converte para minusculas para comparacao 
        std::string lower_nome_origem = nome_origem;
        std::transform(lower_nome_origem.begin(), lower_nome_origem.end(), lower_nome_origem.begin(), ::tolower);

        if (lower_nome_origem == CANCELAR_KEYWORD) {
            std::cout << "\n>> Ataque cancelado pelo usuario.\n";
            return; // Sai da funcao executarAtaque
        }

        origem = encontrarTerritorioPorNome(nome_origem);
        if (origem == nullptr) {
            std::cerr << "ERRO: Territorio nao encontrado. Tente novamente.\n";
        } else if (origem->getDono() != jogador_da_vez) {
            std::cerr << "ERRO: Voce nao e o dono deste territorio. Tente novamente.\n";
        } else {
            break; // Origem valida
        }
    }

    // Loop para obter um DESTINO valido ou cancelar
    while(true) {
        nome_destino = obterString("Digite o nome do territorio de destino (ou '" + CANCELAR_KEYWORD + "'): ");

        std::string lower_nome_destino = nome_destino;
        std::transform(lower_nome_destino.begin(), lower_nome_destino.end(), lower_nome_destino.begin(), ::tolower);

        if (lower_nome_destino == CANCELAR_KEYWORD) {
            std::cout << "\n>> Ataque cancelado pelo usuario.\n";
            return; // Sai da funcao executarAtaque
        }

        destino = encontrarTerritorioPorNome(nome_destino);
        if (destino == nullptr) {
            std::cerr << "ERRO: Territorio nao encontrado. Tente novamente.\n";
        } else if (destino->getDono() == jogador_da_vez) {
            std::cerr << "ERRO: Voce nao pode atacar seu proprio territorio. Tente novamente.\n";
        } else {
            break; // Destino valido
        }
    }

    // Loop para obter um TIPO de ataque valido ou cancelar
    while(true) {
        input_str = obterString("Ataque terrestre (1) ou aereo (2) (ou '" + CANCELAR_KEYWORD + "'): ");

        std::string lower_input = input_str;
        std::transform(lower_input.begin(), lower_input.end(), lower_input.begin(), ::tolower);

        if (lower_input == CANCELAR_KEYWORD) {
            std::cout << "\n>> Ataque cancelado pelo usuario.\n";
            return; // Sai da funcao executarAtaque
        }

        // Tenta converter a string para inteiro
        try {
            tipo_ataque_int = std::stoi(input_str); // Converte string para int
            if (tipo_ataque_int == 1 || tipo_ataque_int == 2) {
                break; // Tipo valido
            } else {
                std::cerr << "ERRO: Tipo de ataque invalido. Escolha 1 ou 2.\n";
            }
        } catch (const std::invalid_argument& ia) {
            std::cerr << "ERRO: Entrada invalida. Por favor, digite um numero (1 ou 2) ou '" << CANCELAR_KEYWORD << "'.\n";
        } catch (const std::out_of_range& oor) {
            std::cerr << "ERRO: Numero fora do intervalo permitido.\n"; 
        }
    }

    // --- Execucao do Ataque --- (Se chegou aqui, todas as entradas são validas)
    Exercito* unidade_de_ataque = nullptr;
    std::cout << "\n=> Iniciando ataque de " << nome_origem << " contra " << nome_destino << "...\n";
    if (tipo_ataque_int == 1) {
        unidade_de_ataque = new ExercitoTerrestre("Ataque Terrestre", jogador_da_vez);
    } else {
        unidade_de_ataque = new ExercitoAereo("Ataque Aereo", jogador_da_vez);
    }

    Jogador* perdedor_potencial = destino->getDono();

    unidade_de_ataque->ataque(origem, destino);

    if (perdedor_potencial != nullptr && perdedor_potencial->getNumTerritorios() == 0 && 
        !perdedor_potencial->estaEliminado()) 
    {
        // O jogador foi eliminado NESTE ATAQUE
        executaEliminacao(jogador_da_vez, perdedor_potencial);
    }

    delete unidade_de_ataque;

    // Exibe feedback
    std::cout << "\n--- Resultado do Combate ---\n";
    checarInfoTerritorios(origem->getNome()); // Mostra estado da origem
    checarInfoTerritorios(destino->getNome()); // Mostra estado do destino
    std::cout << "---------------------------\n";
}


// Metodo auxiliar para executar toda fase de ataque
void Jogo::faseDeAtaque(Jogador* jogador_da_vez) {
    std::cout << "\n===== FASE DE ATAQUE =====\n";
    
    // Loop para repetir o ataque/checar info ate ir para prox fase (movimentacao)
    while (true) {
        char acao = '\0';
        std::cout << "\nDeseja (a)tacar, (c)hecar informacoes ou (p)assar para a proxima fase? ";
        std::cin >> acao;
        
        // Limpa o buffer para a proxima leitura (especialmente getline)
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

        if (acao == 'p' || acao == 'P') {
            break; // Passa para a fase de movimentacao
        } 
        else if (acao == 'c' || acao == 'C') {
            executarChecagemDeInfo();
        } 
        else if (acao == 'a' || acao == 'A') {
            executarAtaque(jogador_da_vez);
        } 
        else {
            std::cerr << "ERRO: Acao invalida. Tente novamente.\n";
        }
    }
}


//  Metodo auxiliar para posicionar reforços
void Jogo::posicionarReforcos(Jogador* jogador, int quantidade_total, const std::string& tipo_tropa) {
    int tropas_para_posicionar = quantidade_total;
    
    // Adiciona uma verificacao para pular o loop se nao houver tropas para posicionar
    if (tropas_para_posicionar == 0) {
        std::cout << "\nVoce nao tem tropas do tipo '" << tipo_tropa << "' para posicionar.\n";
        return;
    }

    std::cout << "\n===== ADICIONANDO TROPAS DO TIPO: " << tipo_tropa << " =====\n";

    // Loop para posicionar enquanto restarem tropas
    while (tropas_para_posicionar > 0) {
        int qtd;
        std::string nome_territorio;
        
        // Loop para validacao de entradas
        while (true) {
            std::cout << "Restam " << tropas_para_posicionar << " exercitos do tipo '" << tipo_tropa << "'" << std::endl;
            qtd = obterInt("Informe a quantidade: ");
            nome_territorio = obterString("Informe o nome do territorio: ");

            Territorio* t = encontrarTerritorioPorNome(nome_territorio);

            if (qtd < 1 || qtd > tropas_para_posicionar) {
                std::cerr << "ERRO: Quantidade invalida\n";
            } else if (t == nullptr) {
                std::cerr << "ERRO: Territorio nao encontrado\n";
            } else if (t->getDono() != jogador) { 
                std::cerr << "ERRO: Voce nao e o dono deste territorio\n";
            } else {
                break; // Input valido
            }
        }
        
        // Execucao da distribuicao e atualizacao das tropas restantes a posicionar
        distribuirExercitos(qtd, nome_territorio, jogador, tipo_tropa);
        tropas_para_posicionar -= qtd;
    }
}


// Metodo auxiliar para coordenar fase de reforco
void Jogo::faseDeReforco(Jogador* jogador_da_vez) {
    // Exibe informacoes do jogador da vez 
    std::cout << "\n===== NOVA RODADA =====\n";
    std::cout << "E a vez do jogador: " << jogador_da_vez->getNome() << std::endl;
    std::cout << "Seu objetivo e: " << jogador_da_vez->getObjetivo() << std::endl;
    verMapaDeGuerra(jogador_da_vez);

    // Calcula reforcos do exercito 
    int reforcos_terrestres = jogador_da_vez->getNumTerritorios() / 2;
    int reforcos_aereos = 0;
    
    // Bonus por continente conquistado
    for (int i = 0; i < _num_continentes; i++) {
        if (jogadorControlaContinente(jogador_da_vez, _continentes[i]->getNome())){
            reforcos_aereos += BONUS_AEREO_POR_CONTINENTE;
            std::cout << "\n\n === BONUS === \n";
            std::cout << "Bonus por controlar " << _continentes[i]->getNome() << ": +5 exercitos aereos!" << std::endl;
            std::cout << "=============\n";
        }
    }

    // Posicionar reforcos 
    std::cout << "\n\n ===== REFORCOS =====\n";
    std::cout << "Voce tem " << reforcos_terrestres << " exercitos terrestres e " 
              << reforcos_aereos << " aereos para posicionar" << std::endl;

    // Chama o metodo auxiliar para as tropas terrestres
    posicionarReforcos(jogador_da_vez, reforcos_terrestres, "terrestre");

    // Chama o mesmo metodo auxiliar para as tropas aereas
    posicionarReforcos(jogador_da_vez, reforcos_aereos, "aereo");
}


// Checa condicao de vitoria e retorna o nome do jogador ganhador, se houver
std::string Jogo::fimDoJogo() {

    int jogadores_ativos = 0;
    Jogador* ultimo_jogador_ativo = nullptr;

    // Itera sobre cada jogador
    for (int i = 0; i < _num_jogadores; i++) {

        // Ignora jogadores eliminados
        if (_jogadores[i]->estaEliminado()) {
            continue;
        }

        // Se chegou aqui, o jogador esta ativo
        jogadores_ativos++;
        ultimo_jogador_ativo = _jogadores[i];

        // Objetivo 1
        if (_jogadores[i]-> getObjetivo() == "Conquistar a América do Norte e Africa"){

            // Checa se jogador controla continentes
            if (jogadorControlaContinente(_jogadores[i], "AMERICA DO NORTE") && 
                jogadorControlaContinente(_jogadores[i], "AFRICA")) {
                    return _jogadores[i]->getNome();
            }
        }
        
        // Objetivo 2
        else if (_jogadores[i]-> getObjetivo() == "Conquistar a Asia e Africa"){

            // Checa se jogador controla continentes
            if (jogadorControlaContinente(_jogadores[i], "ASIA") && 
                jogadorControlaContinente(_jogadores[i], "AFRICA")) {
                    return _jogadores[i]->getNome();
            }
        }

        // Objetivo 3
        else if (_jogadores[i]-> getObjetivo() == "Conquistar América do Norte e Oceania"){

            // Checa se jogador controla continentes
            if (jogadorControlaContinente(_jogadores[i], "AMERICA DO NORTE") && 
                jogadorControlaContinente(_jogadores[i], "OCEANIA")) {
                    return _jogadores[i]->getNome();
            }
        }

        // Objetivo 4
        else if (_jogadores[i]-> getObjetivo() == "Conquistar 18 territórios"){

            // Checa o numero de territorios do jogador
            if (_jogadores[i]->getNumTerritorios() >= 18) 
                return _jogadores[i]->getNome();    
        }

        // Objetivo 5
        else if (_jogadores[i]-> getObjetivo() == "Conquistar 24 territórios"){

            // Checa o numero de territorios do jogador
            if (_jogadores[i]->getNumTerritorios() >= 24) 
                return _jogadores[i]->getNome();
        }

        // Objetivo 6
        else if (_jogadores[i]-> getObjetivo() == "Conquistar 30 territórios"){

            // Checa o numero de territorios do jogador
            if (_jogadores[i]->getNumTerritorios() >= 30) 
                return _jogadores[i]->getNome();
        }
    }

    // Ultimo jogador ativo
    if (jogadores_ativos == 1 && ultimo_jogador_ativo != nullptr) {
        std::cout << "Todos os oponentes foram eliminados!\n";
        return ultimo_jogador_ativo->getNome();
    }

    return "nao"; // Se nenhum jogador ganhou, retorna "nao"
}


// Organizacao inicial do jogo
// Ele cria os jogadores, os territorios, as cartas, define as fronteiras e distribui tudo aleatoriamente
void Jogo::organizarJogo() {
    // Le o arquivo e cria territorios, continentes, fronteiras e cartas
    this->carregarMapa("./territorios.txt");

    // Cria os jogadores
    for (int i = 0; i < _num_jogadores; i++) {
        std::string nome_jogador;
        std::cout << "Jogador " << (i+1) << "\n";
        nome_jogador = obterString("Nome: ");
        _jogadores[i] = new Jogador(nome_jogador);
    }
        
    // Gera semente aleatoria
    std::random_device rd;

    // Motor de geracao de numeros
    std::mt19937 g(rd());

    // Embaralha array de territorios
    std::shuffle(_territorios, _territorios + _num_territorios, g);

    // Distribui os territorios para cada jogador
    for (int i = 0; i < _num_territorios; i++) {

        // Seleciona um jogador com base no indice
        Jogador* jogador_x = this->getJogadorDaVez();

        // Atribui um territorio a ele
        jogador_x -> adicionarTerritorio(1, _territorios[i]);

        // Avanca para o proximo jogador
        proximoJogador();
    }
    
    // Embaralha array de cartas 
    std::shuffle(_cartas, _cartas + _num_objetivos, g);

    // Distribui as cartas para cada jogador
    for (int i = 0; i < _num_jogadores; i++) {

        // Para garantir que nao acesse carta invalida (se tiver mais jogadores que objetivos)
        if (i < _num_objetivos){
            // Atribui a carta _cartas[i] diretamente ao jogador _jogadores[i]
            _jogadores[i]->setCarta(_cartas[i]);
        } else {
            // Caso de erro: não ha cartas de objetivo suficientes para todos os jogadores
            std::cerr << "AVISO: Nao ha cartas de objetivo suficientes para todos os jogadores.\n";
        }
    }

}


// Funcao auxiliar para carregar o mapa do jogo
void Jogo::carregarMapa(const std::string &arquivo_territorios)
{
    using namespace std;
    ifstream arquivo(arquivo_territorios);
    if (!arquivo.is_open())
    {
        cout << "ERRO: Não foi possivel abrir o arquivo, verifique se o nome esta correto e se o arquivo está na pasta correta: " << arquivo_territorios << "\n";
        return;
    }

    string linha;
    int tamanho_das_entradas[4]; // contem o numero de continentes, territorios, jogadores e objetivos respectivamente

    if (getline(arquivo, linha))
    {
        stringstream ss(linha);
        string item;
        int primeira_linha[4];
        int tam = 0;

        while (getline(ss, item, ','))
        {
            tamanho_das_entradas[tam] = stoi(item);
            tam++;
        }
    }

    string continentes[tamanho_das_entradas[0]];                                
    string territorios[tamanho_das_entradas[1]];                                
    int numero_de_jogadores = tamanho_das_entradas[2];                          
    int numero_de_territorios_por_continente[tamanho_das_entradas[0]];          
    string objetivos[tamanho_das_entradas[3]];                                  
    int contador_territorios = 0;                                              
    string **fronteiras_por_territorio = new string *[tamanho_das_entradas[1]]; 
    int numero_de_fronteiras_por_territorio[tamanho_das_entradas[1]];           

    int contador_continente = 0;
    int contador_objetivos = 0;
    while (getline(arquivo, linha)) // Leitura feita linha a linha, no caso aqui a leitura que está sendo feita e a da primeira linha
    {
        string continente, numero_de_territorios;
        if (linha.find(':') == string::npos && linha.find(',') != string::npos)
        {
            stringstream ss(linha);

            getline(ss, continente, ',');
            getline(ss, numero_de_territorios);
            continentes[contador_continente] = continente;

            numero_de_territorios_por_continente[contador_continente] = stoi(numero_de_territorios);

            contador_continente++;
        }

        else if (linha.find(':') != string::npos)
        {
            stringstream ss(linha);
            string territorio, fronteiras, fronteira;

            getline(ss, territorio, ':');

            territorios[contador_territorios] = territorio;

            getline(ss, fronteiras);
            int numero_de_fronteiras = count(fronteiras.begin(), fronteiras.end(), ',') + 1;
            fronteiras_por_territorio[contador_territorios] = new string[numero_de_fronteiras];
            numero_de_fronteiras_por_territorio[contador_territorios] = numero_de_fronteiras;

            stringstream fs_filler(fronteiras);
            int contador_fronteira = 0;
            while (getline(fs_filler, fronteira, ','))
            {

                fronteiras_por_territorio[contador_territorios][contador_fronteira] = fronteira;
                contador_fronteira++;
            }
            contador_territorios++;
        }
    
        else if (!linha.empty())
        {
            objetivos[contador_objetivos] = linha;
            contador_objetivos++;
        }
    }

    arquivo.close();

    // ========================================================================
    // ETAPA 1: CRIACAO DOS OBJETOS PRINCIPAIS
    // ========================================================================

    // Cria os objetos Continente e os armazena no atributo da classe Jogo
    for (int i = 0; i < _num_continentes; i++) {
        _continentes[i] = new Continente(continentes[i], numero_de_territorios_por_continente[i]);
    }

    // Cria os objetos Territorio e os armazena no atributo da classe Jogo
    for (int i = 0; i < _num_territorios; i++) {
        _territorios[i] = new Territorio(territorios[i], numero_de_fronteiras_por_territorio[i]);
    }

    // Cria os objetos Carta e os armazena no atributo da classe Jogo
    for (int i = 0; i < _num_objetivos; i++) {
        _cartas[i] = new Carta(objetivos[i]);
    }

    // ========================================================================
    // ETAPA 2: ASSOCIACAO ENTRE OS OBJETOS (LIGACOES)
    // ========================================================================

    // --- Ligando Territorios aos seus Continentes ---
    // Usamos um contador global para percorrer a lista de territorios, ja que o arquivo os agrupa por continente
    int contador_global_territorios = 0;
    for (int i = 0; i < _num_continentes; i++) {
        // Pega o ponteiro para o continente atual
        Continente* continente_atual = _continentes[i];
        
        // Pega o numero de territorios que pertencem a este continente
        int territorios_neste_continente = numero_de_territorios_por_continente[i];

        // Adiciona a quantidade correta de territorios da lista principal ao continente
        for (int j = 0; j < territorios_neste_continente; j++) {
            Territorio* territorio_para_adicionar = _territorios[contador_global_territorios];
            continente_atual->adicionarTerritorio(territorio_para_adicionar);
            contador_global_territorios++; // Avança para o proximo territorio na lista global
        }
    }

    // --- Ligando Territorios as suas Fronteiras ---
    for (int i = 0; i < _num_territorios; i++) {
        // Pega o ponteiro para o territorio de origem
        Territorio* territorio_origem = _territorios[i];

        // Pega o numero de fronteiras que ele possui
        int num_fronteiras = numero_de_fronteiras_por_territorio[i];

        // Itera sobre os NOMES das fronteiras que lemos do arquivo
        for (int j = 0; j < num_fronteiras; j++) {
            // Obtem o nome da fronteira (ex: "Brasil")
            string nome_da_fronteira_str = fronteiras_por_territorio[i][j];

            // Limpa espaços em branco do nome da fronteira
            nome_da_fronteira_str.erase(0, nome_da_fronteira_str.find_first_not_of(" \t"));
            nome_da_fronteira_str.erase(nome_da_fronteira_str.find_last_not_of(" \t") + 1);
            
            // Usa sua funcao auxiliar para encontrar o PONTEIRO do territorio com aquele nome
            Territorio* ponteiro_da_fronteira = encontrarTerritorioPorNome(nome_da_fronteira_str);
            
            // Se o ponteiro foi encontrado, faz a ligacao
            if (ponteiro_da_fronteira != nullptr) {
                territorio_origem->adicionarFronteira(ponteiro_da_fronteira);
            }
        }
    }

    // ========================================================================
    // ETAPA 3: LIMPEZA DA MEMORIA TEMPORARIA
    // ========================================================================

    // Limpa a matriz de strings de fronteiras
    for (int i = 0; i < tamanho_das_entradas[1]; i++) {
        delete[] fronteiras_por_territorio[i];
    }
    delete[] fronteiras_por_territorio;

} 


// Metodo auxiliar para buscar um territorio no array pelo nome
Territorio* Jogo::encontrarTerritorioPorNome(const std::string &nome) const {

    // Para cada territorio do array
    for (int i = 0; i < _num_territorios; i++) {

        // Se o nome informado for igual ao nome do territorio, retorna o territorio
        if (_territorios[i] != nullptr && _territorios[i]->getNome() == nome) {
            return _territorios[i];
        }
    }
    return nullptr; // Retorna nulo se nao encontrar
}


// Desloca uma certa quantidade de exercitos, de um territorio para outro
void Jogo::distribuirExercitos(int n_exercitos, std::string nome_territ_origem, std::string nome_territ_dest, const std::string& tipo){
    Territorio* origem = encontrarTerritorioPorNome(nome_territ_origem);
    Territorio* destino = encontrarTerritorioPorNome(nome_territ_dest);

    // Tratamento de erros
    // Verifica se os territorios existem
    if (origem == nullptr || destino == nullptr) {
        std::cerr << "Um ou ambos os territorios nao foram encontrados" << std::endl;
        return;
    }
    // Territorio de origem fica sem exercitos ou tentativa de deslocar valores invalidos
    if (n_exercitos < 1 || origem->getExercitos() <= n_exercitos) {
        std::cerr << "Pelo menos 1 exercito deve permanecer na origem" << std::endl;
        return;
    }
    // Tentativa de deslocar exercito para territorio do oponente
    if (origem -> getDono() != destino -> getDono()) {
        std::cerr << "So e possivel deslocar tropas entre territorios proprios" << std::endl;
        return;
    }
    // Tentativa de deslocar exercito que nao faz fronteira
    if (!origem->fazFronteira(destino) && tipo == "terrestre") {
        std::cerr << "O territorio de destino nao faz fronteira com o de origem" << std::endl;
        return; 
    }
    
    // Deslocamento das tropas
    // Remove exercitos do territorio de origem
    origem -> removerExercitos(n_exercitos, origem, tipo);

    // Adiciona exercitos no territorio de destino
    destino -> adicionarExercitos(n_exercitos, destino, tipo);

    // Mensagem de confirmacao
    std::cout << n_exercitos << " exercitos " << tipo << " movidos de " << origem->getNome() << " para " << destino->getNome() << "." << std::endl;
}


// Adiciona uma certa quantidade de exercitos no territorio de um jogador
void Jogo::distribuirExercitos(int n_exercitos, std::string nome_territorio, Jogador* jogador, const std::string& tipo){
    
    // Encontra territorio a partir do nome
    Territorio* territorio = encontrarTerritorioPorNome(nome_territorio);

    // Tratamento de Erros
    // Territorio nao encontrado
    if (territorio == nullptr){
        std::cerr << "Territorio nao encontrado" << std::endl;
        return;
    }
    // Jogador nao encontrado
    if (jogador == nullptr) {
        std::cerr << "Jogador nao encontrado" << std::endl;
        return;
    }
    // Jogador nao e dono do territorio
    if (territorio->getDono() != jogador) {
        std::cerr << "O jogador nao e dono do territorio informado" << std::endl;
        return;
    }
    // Informou quantidade invalida de exercitos
    if (n_exercitos < 1) {
        std::cerr << "A quantidade de exercitos para reforco deve ser no minimo 1" << std::endl;
        return;
    }

    // Adiciona exercitos ao territorio do jogador
    territorio -> adicionarExercitos(n_exercitos, territorio, tipo);

    // Mensagem de confirmacao
    std::cout << n_exercitos << " exercitos " << tipo << " de reforco adicionados em " << nome_territorio << "." << std::endl;
}


// Metodo auxiliar para checar continentes dominados por um jogador
bool Jogo::jogadorControlaContinente(Jogador* jogador, const std::string& nome_continente) const {
    
    // Inicia ponteiro para o continente
    Continente* continente_alvo = nullptr;

    // Encontra continente a partir do nome
    for (int i = 0; i < _num_continentes; i++) {
        if (_continentes[i]->getNome() == nome_continente){
            continente_alvo = _continentes[i];
            break;
        }
    }

    // Se nao achou continente, retorna false 
    if (continente_alvo == nullptr) return false;

    // Obtem territorios do continente
    Territorio** territorios_do_continente = continente_alvo -> getTerritorios();

    // Checa se todos territorios do continente estao dominados pelo jogador
    for (int i = 0; i < continente_alvo -> getNumTerritorios(); i++) {
        
        // Se nao for dono, retorna false
        if (territorios_do_continente[i]->getDono() != jogador) return false;
    }

    // Se chegou aqui, o jogador domina o continente
    return true;
}


// Metodo auxiliar para checar informacoes de certo territorio
void Jogo::checarInfoTerritorios(const std::string& nome_territorio) const {

    // Checa se existe o territorio com o nome informado
    Territorio* t = encontrarTerritorioPorNome(nome_territorio);
    if (t == nullptr) {
        std::cerr << "Territorio '" << nome_territorio << "' nao encontrado.\n";
        return;
    }

    // Imprime informacoes pertinentes ao territorio
    std::cout << "--- Info: " << t->getNome() << " ---\n";
    std::cout << "  Dono: " << (t->getDono() ? t->getDono()->getNome() : "Ninguem") << "\n";
    std::cout << "  Tropas Terrestres: " << t->getExercitosTerrestres() << "\n";
    std::cout << "  Tropas Aereas: " << t->getExercitosAereos() << "\n";

    Territorio** f = t->getFronteiras();
    std::cout << "  Fronteiras: ";
    for (int i = 0; i < t->getNumFronteiras(); i++) {
        if (f[i] != nullptr) { 
        std::cout << f[i]->getNome() << " ";
        } else {
        std::cout << "[Fronteira NULA!] "; // Indica possivel problema de leitura das fronteiras
        }
    }
    std::cout << "\n";
}


// Metodo auxiliar para encontrar jogador a partir do nome
Jogador* Jogo::encontrarJogadorPorNome(const std::string &nome) const {

    // Para cada jogador do array
    for (int i = 0; i < _num_jogadores; i++) {

        // Se o nome informado for igual ao nome de um jogador, retorna o jogador
        if (_jogadores[i] != nullptr && _jogadores[i]->getNome() == nome) {
            return _jogadores[i];
        }
    }
    return nullptr; // Retorna nulo se nao encontrar
}


// Metodo auxiliar para encontrar continente a partir do nome
Continente* Jogo::encontrarContinentePorNome(const std::string& nome_continente) const {
    // Para cada continente do array
    for (int i = 0; i < _num_continentes; i++) {

        // Se o nome informado for igual ao nome do continente, retorna o continente
        if (_continentes[i] != nullptr && _continentes[i]->getNome() == nome_continente) {
            return _continentes[i];
        }
    }

    return nullptr; // Retorna nulo se nao encontrar
}


// Metodo auxiliar para obter 's' ou 'n' do usuario
char Jogo::obterConfirmacao(const std::string& prompt) const {
    char resposta;
    while (true) {
        std::cout << prompt;

        // Tenta ler um char, pulando espaços em branco
        if (!(std::cin >> std::ws >> resposta)) {
            std::cin.clear(); // Limpa o estado de erro 
        }

        // Limpa o resto da linha (ex: "im" de "sim")
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

        // Converte para minusculo para facilitar a verificacao
        resposta = std::tolower(resposta);

        if (resposta == 's' || resposta == 'n') {
            return resposta; // Retorna o char valido
        } else {
            std::cout << "Resposta inválida. Por favor, digite 's' ou 'n'." << std::endl;
        }
    }
}


// Metodo auxiliar para obter string valida do usuario
std::string Jogo::obterString(const std::string& prompt) const {
    std::string entrada;

    while (true) {
        std::cout << prompt;

        // Limpa o buffer de entrada de lixo anterior (como '\n') e le a linha ate o proximo \n
        if (std::getline(std::cin >> std::ws, entrada)) {
            
            // Verifica se o usuario digitou algo
            if (!entrada.empty()) {
                return entrada; // Retorna a string
            }
        }

        // Se a entrada estava vazia ou se houve um erro de stream
        std::cout << "A entrada não pode ser vazia. Tente novamente." << std::endl;
        
        // Em caso de erro grave, limpa o estado de erro
        if (std::cin.fail()) {
            std::cin.clear();
            // Esvazia o buffer de qualquer coisa que tenha sobrado
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        }
    }
}


// Metodo auxiliar para obter inteiro valido do usuario
int Jogo::obterInt(const std::string& prompt) const {
    int valor;
    while (true) {
        std::cout << prompt;

        // Se a leitura for bem-sucedida (for um numero)
        if (std::cin >> valor) {
            // Limpa o buffer para caso o usuario tenha digitado "10abc"
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            return valor; // retorna inteiro valido
        } else {
            // Se falhou (digitou "abc")
            std::cout << "Entrada inválida. Por favor, digite um número." << std::endl;
            std::cin.clear(); // Limpa o estado de erro
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // Descarta a entrada ruim
        }
    }
}


// Metodo auxiliar para apresentar mapa de guerra e sumario de um jogador 
void Jogo::verMapaDeGuerra(Jogador* jogador) const {
    
    // ===== CHECAR SE ESTA ELIMINADO =====
    if (jogador->estaEliminado()) {
        std::cout << "\n===== " << YELLOW << "STATUS DE " << jogador->getNome() << RESET << " =====\n";
        std::cout << "\n--- " << RED << "JOGADOR ELIMINADO" << RESET << " ---\n";
        return; // Não precisa mostrar o resto
    }

    // ===== CALCULAR OS TOTAIS =====
    int num_territorios = jogador->getNumTerritorios();
    Territorio** territorios_do_jogador = jogador->getTerritorios();
    int total_exercitos = 0, total_aereos = 0, total_terrestres = 0;
    int continentes_conquistados = 0; // <<< Req 5

    for (int i = 0; i < num_territorios; i++) {
        total_exercitos += territorios_do_jogador[i]->getExercitos();
        total_aereos += territorios_do_jogador[i]->getExercitosAereos();
        total_terrestres += territorios_do_jogador[i]->getExercitosTerrestres();
    }

    for (int i = 0; i < _num_continentes; i++) {
        if (jogadorControlaContinente(jogador, _continentes[i]->getNome())) {
            continentes_conquistados++;
        }
    }

    // ===== EXIBIR O CABEÇALHO E O SUMARIO =====
    std::cout << "\n===== " << YELLOW << "MAPA DE GUERRA E STATUS DE " << jogador->getNome() << RESET << " =====\n";

    std::cout << "\n--- " << YELLOW << "SUMARIO DAS FORCAS" << RESET << " ---\n";
    std::cout << "  Objetivo:                 " << YELLOW << jogador->getObjetivo() << RESET << "\n";
    std::cout << "  Territorios:              " << GREEN << num_territorios << RESET << " de " << _num_territorios << "\n";
    std::cout << "  Continentes Conquistados: " << GREEN << continentes_conquistados << RESET << " de " << _num_continentes << "\n";
    std::cout << "  Exercito Total:           " << GREEN << total_exercitos << RESET << "\n";
    std::cout << "  Tropas Terrestres:        " << GREEN << total_terrestres << RESET << "\n";
    std::cout << "  Tropas Aereas:            " << GREEN << total_aereos << RESET << "\n";

    // ===== EXIBIR A TABELA DETALHADA =====
    std::cout << "\n--- " << YELLOW << "DETALHES DOS TERRITORIOS" << RESET << " ---\n";

    // Define larguras das colunas.
    const int NOME_WIDTH = 25; 
    const int TERRESTRE_WIDTH = 12;
    const int AEREO_WIDTH = 10;
    const int FRONTEIRA_WIDTH = 60; 
    const int TOTAL_WIDTH = NOME_WIDTH + TERRESTRE_WIDTH + AEREO_WIDTH + FRONTEIRA_WIDTH;

    for (int i = 0; i < _num_continentes; i++) {
        Continente* cont = _continentes[i];
        bool imprimiu_cabecalho_continente = false;

        // Itera sobre os territorios do jogador
        for (int j = 0; j < num_territorios; j++) {
            Territorio* terr = territorios_do_jogador[j];
            bool pertence = false;
            Territorio** terrs_do_cont = cont->getTerritorios();
            for(int k = 0; k < cont->getNumTerritorios(); k++){
                if(terrs_do_cont[k] == terr){
                    pertence = true;
                    break;
                }
            }

            // Se pertence ao continente
            if (pertence) {
                // Imprime o cabecalho do continente (apenas uma vez)
                if (!imprimiu_cabecalho_continente) {
                    imprimiu_cabecalho_continente = true;
                    
                    // Status de Conquista
                    std::cout << "\n--- " << YELLOW << cont->getNome() << RESET;
                    if (jogadorControlaContinente(jogador, cont->getNome())) {
                        std::cout << GREEN << " (CONQUISTADO)" << RESET;
                    }
                    std::cout << " ---\n";

                    // Imprime o cabecalho da tabela
                    std::cout << std::left 
                              << std::setw(NOME_WIDTH) << "Territorio" 
                              << std::setw(TERRESTRE_WIDTH) << "Terrestres" 
                              << std::setw(AEREO_WIDTH) << "Aereos"
                              << std::setw(FRONTEIRA_WIDTH) << "Fronteiras" 
                              << std::endl;
                    // Linha separadora
                    std::cout << std::setw(TOTAL_WIDTH) << std::setfill('-') << "" << std::setfill(' ') << std::endl;
                }

                // String de fronteiras
                std::string fronteiras_str = "";
                Territorio** f = terr->getFronteiras();
                int num_fronteiras = terr->getNumFronteiras();
                for (int f_idx = 0; f_idx < num_fronteiras; f_idx++) {
                    if (f[f_idx] != nullptr) {
                        fronteiras_str += f[f_idx]->getNome();
                        if (f_idx < num_fronteiras - 1) { // Adiciona virgula se nao for a ultima
                            fronteiras_str += ", ";
                        }
                    }
                }

                // Imprime a linha de dados do territorio
                std::cout << std::left 
                          << CYAN << std::setw(NOME_WIDTH) << terr->getNome() << RESET
                          << GREEN << std::setw(TERRESTRE_WIDTH) << terr->getExercitosTerrestres() << RESET
                          << GREEN << std::setw(AEREO_WIDTH) << terr->getExercitosAereos() << RESET
                          << WHITE << std::setw(FRONTEIRA_WIDTH) << fronteiras_str << RESET 
                          << std::endl;
            }
        }
        // Pausa apos cada continente, exceto o ultimo
        if (i < _num_continentes - 1) {
        pressioneEnterParaContinuar();
        }
    }
    std::cout << std::endl;
}


// Metodo para lidar com a eliminacao de um jogador
void Jogo::executaEliminacao(Jogador* conquistador, Jogador* perdedor) {
    // Verificacao de segurança
    if (perdedor == nullptr || conquistador == nullptr || perdedor->estaEliminado()) {
        return;
    }

    std::cout << "\n\n!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!\n";
    std::cout << "ELIMINACAO! " << conquistador->getNome() << " eliminou " << perdedor->getNome() << " do jogo!\n";
    std::cout << "!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!\n\n";

    // Marca o jogador como eliminado
    perdedor->setEliminado(true);

}


// Metodo auxiliar para apresentar o mapa de guerra por partes
void Jogo::pressioneEnterParaContinuar() const {
    std::cout << "\n--- Pressione ENTER para continuar ---" << std::flush;

    // Limpa o buffer de novo para garantir que a proxima leitura (obterString, etc.) funcione
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
}
