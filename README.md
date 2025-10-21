# Jogo de Estratégia War (Implementação em C++)

Este repositório contém uma implementação em C++ do clássico jogo de tabuleiro de estratégia War (similar ao Risk), desenvolvido como parte de um trabalho da disciplina de Programação II. O jogo simula conflitos militares entre jogadores pelo controle de territórios em um mapa-múndi simplificado.

## Funcionalidades Principais

* **Setup Dinâmico:** O mapa (territórios, continentes, fronteiras) e os objetivos são carregados a partir de um arquivo de texto (`territorios.txt`), permitindo flexibilidade na configuração do jogo.
* **Distribuição Aleatória:** Territórios e cartas de objetivo são embaralhados e distribuídos aleatoriamente entre os jogadores no início da partida.
* **Sistema de Turnos:** O jogo segue um sistema de turnos, onde cada jogador passa pelas fases clássicas do War:
    * **Reforço:** Cálculo de novas tropas com base nos territórios possuídos e bônus por continentes dominados.
    * **Ataque:** Permite múltiplos ataques a territórios inimigos adjacentes (terrestre) ou quaisquer outros (aéreo).
    * **Movimentação:** Permite uma movimentação estratégica de tropas entre territórios aliados adjacentes ao final do turno.
* **Tipos de Exércitos:** Implementa a distinção entre exércitos **Terrestres** e **Aéreos**:
    * Reforços base são terrestres.
    * Bônus por domínio de continente concede exércitos aéreos (+5 por continente).
    * Ataques aéreos podem ser realizados entre quaisquer territórios (sem necessidade de fronteira), mas exigem tropas aéreas na origem.
* **Combate Simplificado:** A resolução de combates é baseada em uma simulação de "rolagem de dados" (50/50), resultando na perda de uma unidade para o atacante ou defensor.
* **Condições de Vitória:** O jogo verifica automaticamente a cada turno se algum jogador cumpriu seu objetivo secreto (conquistar continentes específicos ou um número total de territórios).
* **Interface de Linha de Comando (CLI):** O jogo é totalmente jogável através do terminal, com prompts claros para as ações do jogador.
* **Checagem de Informações:** Permite ao jogador, durante seu turno, verificar o status detalhado de qualquer território ou o resumo das forças de qualquer jogador.

## Tecnologias Utilizadas

* **C++:** Linguagem principal de desenvolvimento.
* **Compilador:** g++ (recomendado).
* **Bibliotecas Padrão C++:** `<iostream>`, `<fstream>`, `<sstream>`, `<string>`, `<vector>`, `<algorithm>`, `<random>`, `<iomanip>`, `<limits>`, `<stdexcept>`.

## Pré-requisitos

* Um compilador C++ moderno (como g++) instalado.
* Git instalado (para clonar o repositório).
* O arquivo `territorios.txt` (com a formatação esperada) presente na mesma pasta do executável.

## Como Compilar e Executar

1.  **Clone o Repositório:**
    ```bash
    git clone [https://docs.github.com/pt/repositories/creating-and-managing-repositories/quickstart-for-repositories](https://docs.github.com/pt/repositories/creating-and-managing-repositories/quickstart-for-repositories)
    cd [Nome da Pasta do Repositório]
    ```

2.  **Compile o Código:**
    Abra o terminal na pasta do projeto e execute o comando de compilação. Usar a flag `-g` é recomendado para facilitar a depuração.
    ```bash
    g++ -g *.cpp -o war
    ```
    *(Isso criará um executável chamado `war`)*

3.  **Execute o Jogo:**
    Certifique-se de que o arquivo `territorios.txt` está na mesma pasta e execute:
    ```bash
    ./war
    ```

4.  **Siga as Instruções:** O jogo solicitará o número de jogadores (se aplicável à sua versão do `main`), os nomes dos jogadores e guiará através das fases do jogo.

## Estrutura do Arquivo `territorios.txt`

O jogo espera que o arquivo `territorios.txt` siga o seguinte formato:

1.  **Linha 1:** `num_continentes,num_territorios,num_jogadores,num_objetivos`
2.  **Linhas de Continente:** `NomeContinente,NumTerritoriosNoContinente` (repetido para cada continente)
3.  **Linhas de Território:** `NomeTerritorio: Fronteira1, Fronteira2, ...` (repetido para cada território, agrupados por continente na ordem da seção anterior)
4.  **Linhas de Objetivo:** `Descricao completa do objetivo` (repetido para cada objetivo)

## Autor

* **Nicolas Loures Salvador** - nicolas.salvador.br@gmail.com

## Contexto

Este projeto foi desenvolvido para a disciplina de INF 112 - Programação II, do curso de Ciência da Computação na Universidade Federal de Viçosa, sob orientação do Prof. Júlio Reis.
Período: 2º Semestre/2025

---