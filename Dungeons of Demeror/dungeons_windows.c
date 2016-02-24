#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

// DEFINES :: ON

// ===== EDIÇÃO DOS CONTROLES ===== //
// Caso você deseje mudar as teclas do jogo, modifique apenas o que estiver abaixo, seguindo o esquema.
// Por exemplo, caso você queira fazer seu personagem andar para cima ao pressionar a tecla U do teclado,
// a modificação a ser feita deverá ficar:
//
// #define KEY_UP 'U'
// #define KEY_up 'u'

#define KEY_UP 'W'
#define KEY_up 'w'

#define KEY_DOWN 'S'
#define KEY_down 's'

#define KEY_LEFT 'A'
#define KEY_left 'a'

#define KEY_RIGHT 'D'
#define KEY_right 'd'

#define KEY_ATTACK 'E'
#define KEY_attack 'e'

#define KEY_SP_ATTACK 'Q'
#define KEY_sp_attack 'q'

#define KEY_HP_POTION '1'
#define KEY_hp_potion '1'

#define KEY_SP_POTION '2'
#define KEY_sp_potion '2'

// ====== A PARTIR DAQUI EM BAIXO: NÃO MEXA EM MAIS NADA!!!! ====== //


#define MAX_ENEMIES 5 // O número máximo de inimigos que podem ser gerados no mapa
#define DIMENSION_X 25 // Largura do Mapa
#define DIMENSION_Y 14 // Altura do Mapa
#define CHESTS 4 // Número de baús q podem ser gerados (n-1)
#define SP_HABILITY_COST 5 // Custo em sp das habilidades especiais

#define CODE_BLANKSPACE 0
#define CODE_WALL 1
#define CODE_WATER 2
#define CODE_FIRE 3
#define CODE_STAIR 4
#define CODE_CHEST 10

// DEFINES :: OFF

// ESTRUTURAS DE DADOS :: ON
typedef struct Char{ // Estrutura de dados dos personagens do jogo
    int code; // Código do personagem. Caso seja menor que 0, é um inimigo
    char name[20]; // Nome do personagem
    char chClass[20]; // Nome da classe do personagem;
    char sprite; // Representação do personagem no mapa
    int posX, posY; // Posição do personagem no mapa
    int hp; // Pontos de vida do personagem
    int sp; // Pontos especiais do personagem
    int armor,weapon; // Valores de ataque e defesa do personagem
    int isDead; // Variável auxiliar que guarda se o personagem está morto ou não
    int isBurning; // Variável auxiliar que guarda se o personagem está queimando ou não
}Char;

// ESTRYTURAS DE DADOS :: OFF

// DEFINIÇÕES GLOBAIS :: ON
int map[DIMENSION_Y][DIMENSION_X]; // Mapa do jogo. Como o primeiro elemento da matriz conta as linhas (eixo vertical), ele fica com o eixo Y
int modelMap[DIMENSION_Y][DIMENSION_X]; // Mapa modelo do nível. Serve como um mapa de memória para se desenhar as coisas
int quantEnemies = 0; // Guarda a quantidade de inimigos gerados
int enemiesDefeated = 0; // Guarda a quantidade de inmigos derrotados
Char enemyList[MAX_ENEMIES]; // Vetor de lista de inimigos a serem gerados
int canExistChests = 0; // Guarda se pode gerar os baús
int chestsContents[CHESTS]; // Guarda os conteúdos dos baús
int playerHPPotions = 5; // Guarda o número de poções de HP do jogador. O jogador começa com 5
int playerSPPotions = 0; // Guarda o número de poções de SP do jogador. O jogador começa com 0
int actualLevel = 0; // Guarda o andar atual que o jogador estádo personagem atualmente;
int spMax = 10; // Guarda o SP maximo do personagem atualmente
int hpMax = 100; // Guarda o HP maximo do personagem atualmente
int berserkMode = 0; // Guarda se o jogador está em modo Berserk

// DEFINIÇÕES GLOBAIS :: OFF

// FUNÇÕES :: ON
    // FUNÇÕES DE CRIAÇÃO :: ON
    void createMap(){ // Função que cria o mapa e seus contornos, com tamanho definido pelas constantes DIMENSION_X e DIMENSION_Y
        int i, j, k; // Variáveis auxiliares dos for
        int mapObstacles, obsOrignX, obsOrignY, obsType;

        for(i=0; i<DIMENSION_Y - 1; i++){
            for(j=0; j<DIMENSION_X; j++){ // For das colunas do mapa
                map[i][j] = 0; // A princípio todos os espaços do mapa são vazios
                modelMap[i][j] = 0;
                if (i == 0 || j == 0){
                        map[i][j] = CODE_WALL; // Insere paredes ao redor da sala
                        modelMap[i][j] = CODE_WALL;
                }
                if (i == (DIMENSION_Y-2) || j == (DIMENSION_X-1)){
                        map[i][j] = CODE_WALL; // Insere paredes ao redor da sala
                        modelMap[i][j] = CODE_WALL;
                }
                if ((i == 1 && j == DIMENSION_X-2) || (i== DIMENSION_Y-3 && j == DIMENSION_X-2) || (i == DIMENSION_Y-3 && j == 1) || (i == 1 && j == 1)){
                        map[i][j] = CODE_WALL;
                        modelMap[i][j] = CODE_WALL;
                }
            }
        }
            //Após gerar o mapa básico, geram-se também alguns obstáculos aleatórios no mapa
                mapObstacles = rand()%10;
                for (k=0; k < mapObstacles; k++){

                        obsType = (rand()%4) + 1; // Decide o tipo de obstáculo a ser gerado
                        beginGeneration:

                        obsOrignX = rand()%(DIMENSION_X-4);
                        obsOrignY = rand()%(DIMENSION_Y-4)+1;

                        if ((obsOrignX == 1 && obsOrignY == DIMENSION_Y/2) || (obsOrignX == 1 && obsOrignY == DIMENSION_Y/2 -1)) goto beginGeneration;

                        if (map[obsOrignY][obsOrignX] == 0){
                                switch(obsType){

                                case 1:
                                    map[obsOrignY][obsOrignX] = CODE_WALL;
                                    map[obsOrignY+1][obsOrignX] = CODE_WALL;
                                    map[obsOrignY][obsOrignX+1] = CODE_WALL;
                                    map[obsOrignY+1][obsOrignX+1] = CODE_WALL;
                                    modelMap[obsOrignY][obsOrignX] = CODE_WALL;
                                    modelMap[obsOrignY+1][obsOrignX] = CODE_WALL;
                                    modelMap[obsOrignY][obsOrignX+1] = CODE_WALL;
                                    modelMap[obsOrignY+1][obsOrignX+1] = CODE_WALL;
                                break;
                                case 2:
                                    map[obsOrignY][obsOrignX] = CODE_WALL;
                                    map[obsOrignY+1][obsOrignX] = CODE_WALL;
                                    map[obsOrignY][obsOrignX+1] = CODE_WALL;
                                    map[obsOrignY+1][obsOrignX+1] = CODE_WALL;
                                    modelMap[obsOrignY][obsOrignX] = CODE_WALL;
                                    modelMap[obsOrignY+1][obsOrignX] = CODE_WALL;
                                    modelMap[obsOrignY][obsOrignX+1] = CODE_WALL;
                                    modelMap[obsOrignY+1][obsOrignX+1] = CODE_WALL;
                                break;
                                case 3:
                                    map[obsOrignY][obsOrignX] = CODE_WATER;
                                    map[obsOrignY+1][obsOrignX] = CODE_WATER;
                                    map[obsOrignY][obsOrignX+1] = CODE_WATER;
                                    map[obsOrignY+1][obsOrignX+1] = CODE_WATER;
                                    modelMap[obsOrignY][obsOrignX] = CODE_WATER;
                                    modelMap[obsOrignY+1][obsOrignX] = CODE_WATER;
                                    modelMap[obsOrignY][obsOrignX+1] = CODE_WATER;
                                    modelMap[obsOrignY+1][obsOrignX+1] = CODE_WATER;
                                break;
                                case 4:
                                    map[obsOrignY][obsOrignX] = CODE_FIRE;
                                    map[obsOrignY+1][obsOrignX] = CODE_FIRE;
                                    map[obsOrignY][obsOrignX+1] = CODE_FIRE;
                                    map[obsOrignY+1][obsOrignX+1] = CODE_FIRE;
                                    modelMap[obsOrignY][obsOrignX] = CODE_FIRE;
                                    modelMap[obsOrignY+1][obsOrignX] = CODE_FIRE;
                                    modelMap[obsOrignY][obsOrignX+1] = CODE_FIRE;
                                    modelMap[obsOrignY+1][obsOrignX+1] = CODE_FIRE;
                                break;
                                }
                        }
                        else goto beginGeneration;
                }
        }
    void createEnemies(){ // Função que gera a lista de inimigos. No momento só um tipo de inimigo é gerado
        /*
        ||========= OBS ==========||
        || Para criar inimigos, o ||
        || código a ser utilizado ||
        ||    no campo code do    ||
        || char inimigo deve ser  ||
        ||    menor que zero.     ||
        ||========================||
        */
        int i; // Variável auxiliar do For
        int rQuant,enemyType,rPosX,rPosY; // Variáveis auxiliares de geração dos inimigos (quantidade, tipo, posição em X e posição em Y)

        for (i=0; i<MAX_ENEMIES; i++) enemyList[i].code = 0; // "Limpa" a lista, para não imprimir inimigos que da outra rodada

        rQuant = (rand()%MAX_ENEMIES)+1; // Quantidade de inimigos gerados, entre 1 e MAX_ENEMIES

        if (actualLevel == 0) quantEnemies = (rand()%3)+1;
        else if (actualLevel < 99)quantEnemies = rQuant;
        else quantEnemies = 1; // Caso chegue ao nível 100, só se spawna 1 monstro

        for (i=0; i<quantEnemies; i++ ){ // Para inimigo gerado, a rotina abaixo será criada

            if(actualLevel < 10) enemyType = rand()%2; // Sorteia o tipo de inimigo a ser spawnado nos níveis 0 - 9
            else if (actualLevel >= 10 && actualLevel < 20) enemyType = (rand()%2) + 1; // Sorteia o tipo de inimigo a ser spawnado nos níveis 10 - 19
            else if (actualLevel >= 20 && actualLevel < 30) enemyType = (rand()%2) + 2; // Sorteia o tipo de inimigo a ser spawnado nos níveis 20 - 29
            else if (actualLevel >= 30 && actualLevel < 40) enemyType = (rand()%2) + 3; // Sorteia o tipo de inimigo a ser spawnado nos níveis 30 - 39
            else if (actualLevel >= 40 && actualLevel < 50) enemyType = (rand()%2) + 4; // Sorteia o tipo de inimigo a ser spawnado nos níveis 40 - 49
            else if (actualLevel >= 50 && actualLevel < 60) enemyType = (rand()%2) + 5; // Sorteia o tipo de inimigo a ser spawnado nos níveis 50 - 59
            else if (actualLevel >= 60 && actualLevel < 70) enemyType = (rand()%2) + 6; // Sorteia o tipo de inimigo a ser spawnado nos níveis 60 - 69
            else if (actualLevel >= 70 && actualLevel < 80) enemyType = (rand()%2) + 7; // Sorteia o tipo de inimigo a ser spawnado nos níveis 70 - 79
            else if (actualLevel >= 80 && actualLevel < 90) enemyType = (rand()%3) + 6; // Sorteia o tipo de inimigo a ser spawnado nos níveis 80 - 89
            else if (actualLevel >= 90 && actualLevel < 99) enemyType = (rand()%2) +  8; // Sorteia o tipo de inimigo a ser spawnado nos níveis 90 - 99
            else if (actualLevel == 99) enemyType = 10; // No nível 100, só se spawna o Necromante

            switch(enemyType){
                case 0:
                    strcpy(enemyList[i].name, "Goblin"); // Nome do inimigo
                    enemyList[i].code = -(i+1); // Código do inimigo
                    enemyList[i].sprite = 'g'; // Representação do inimigo no mapa
                    enemyList[i].hp = 5; // Pontos de vida do inimigo
                    enemyList[i].armor = 0; // Defesa do inimigo
                    enemyList[i].weapon = (rand()%3)+1; // Ataque do inimigo
                break;

                case 1:
                    strcpy(enemyList[i].name, "Imp"); // Nome do inimigo
                    enemyList[i].code = -(i+1); // Código do inimigo
                    enemyList[i].sprite = 'i'; // Representação do inimigo no mapa
                    enemyList[i].hp = 7; // Pontos de vida do inimigo
                    enemyList[i].armor = 1; // Defesa do inimigo
                    enemyList[i].weapon = (rand()%3)+1; // Ataque do inimigo
                break;

                case 2:
                    strcpy(enemyList[i].name, "Kobold"); // Nome do inimigo
                    enemyList[i].code = -(i+1); // Código do inimigo
                    enemyList[i].sprite = 'k'; // Representação do inimigo no mapa
                    enemyList[i].hp = 10; // Pontos de vida do inimigo
                    enemyList[i].armor = 1; // Defesa do inimigo
                    enemyList[i].weapon = (rand()%4)+1; // Ataque do inimigo
                break;

                case 3:
                    strcpy(enemyList[i].name, "Troll"); // Nome do inimigo
                    enemyList[i].code = -(i+1); // Código do inimigo
                    enemyList[i].sprite = 'T'; // Representação do inimigo no mapa
                    enemyList[i].hp = 25; // Pontos de vida do inimigo
                    enemyList[i].armor = 3; // Defesa do inimigo
                    enemyList[i].weapon = (rand()%7)+1; // Ataque do inimigo
                break;

                case 4:
                    strcpy(enemyList[i].name, "Ogre"); // Nome do inimigo
                    enemyList[i].code = -(i+1); // Código do inimigo
                    enemyList[i].sprite = 'O'; // Representação do inimigo no mapa
                    enemyList[i].hp = 35; // Pontos de vida do inimigo
                    enemyList[i].armor = 5; // Defesa do inimigo
                    enemyList[i].weapon = (rand()%8)+1; // Ataque do inimigo
                break;

                case 5:
                    strcpy(enemyList[i].name, "Plagued Warrior"); // Nome do inimigo
                    enemyList[i].code = -(i+1); // Código do inimigo
                    enemyList[i].sprite = 'W'; // Representação do inimigo no mapa
                    enemyList[i].hp = 45; // Pontos de vida do inimigo
                    enemyList[i].armor = 7; // Defesa do inimigo
                    enemyList[i].weapon = (rand()%10)+1; // Ataque do inimigo
                break;

                case 6:
                    strcpy(enemyList[i].name, "Black Knight"); // Nome do inimigo
                    enemyList[i].code = -(i+1); // Código do inimigo
                    enemyList[i].sprite = 'K'; // Representação do inimigo no mapa
                    enemyList[i].hp = 70; // Pontos de vida do inimigo
                    enemyList[i].armor = 15; // Defesa do inimigo
                    enemyList[i].weapon = (rand()%15)+1; // Ataque do inimigo
                break;

                case 7:
                    strcpy(enemyList[i].name, "Dragon"); // Nome do inimigo
                    enemyList[i].code = -(i+1); // Código do inimigo
                    enemyList[i].sprite = '$'; // Representação do inimigo no mapa
                    enemyList[i].hp = 120; // Pontos de vida do inimigo
                    enemyList[i].armor = 20; // Defesa do inimigo
                    enemyList[i].weapon = (rand()%25)+1; // Ataque do inimigo
                break;

                case 8:
                    strcpy(enemyList[i].name, "Corrupted Dragon"); // Nome do inimigo
                    enemyList[i].code = -(i+1); // Código do inimigo
                    enemyList[i].sprite = '&'; // Representação do inimigo no mapa
                    enemyList[i].hp = 150; // Pontos de vida do inimigo
                    enemyList[i].armor = 25; // Defesa do inimigo
                    enemyList[i].weapon = (rand()%35)+1; // Ataque do inimigo
                break;

                case 9:
                    strcpy(enemyList[i].name, "Evil Essence"); // Nome do inimigo
                    enemyList[i].code = -(i+1); // Código do inimigo
                    enemyList[i].sprite = 'E'; // Representação do inimigo no mapa
                    enemyList[i].hp = 180; // Pontos de vida do inimigo
                    enemyList[i].armor = 35; // Defesa do inimigo
                    enemyList[i].weapon = (rand()%35)+1; // Ataque do inimigo
                break;

                case 10:
                    strcpy(enemyList[i].name, "The Necromancer"); // Nome do inimigo
                    enemyList[i].code = -(i+1); // Código do inimigo
                    enemyList[i].sprite = 'N'; // Representação do inimigo no mapa
                    enemyList[i].hp = 300; // Pontos de vida do inimigo
                    enemyList[i].armor = 45; // Defesa do inimigo
                    enemyList[i].weapon = (rand()%50)+1; // Ataque do inimigo
                break;
            }

            enemyList[i].isDead = 0;// Declara q o inimgo está vivo

            startPosGen:
            rPosX = (rand()%DIMENSION_X) + 1; // Coordenada X aleatórea
            rPosY = (rand()%DIMENSION_Y) + 1; // Coordenada Y aleatórea

            if (map[rPosY][rPosX] == 0 && rPosY < DIMENSION_Y-2){
                enemyList[i].posX = rPosX;
                enemyList[i].posY = rPosY;
            }
            else{
                goto startPosGen;
            }

            map[rPosY][rPosX] = enemyList[i].code; // Aloca a posição do inimigo no mapa
        }
    }
    void createPlayer(Char *p){ // Função que inicializa o jogador
        p->code = 99; // Código do jogador
        p->posX = 1; // Posição inicial do jogador em X
        p->posY = DIMENSION_Y/2; // Posição inicial do jogador em Y
        p->hp = 100; // Pontos de vida do jogador
        p->sp = 10; // Pontos especiais do jogador
        p->armor = 0; // Defesa do jogador
        p->weapon = 3; // Ataque do jogador
        p->isDead = 0; // Sinaliza que o jogador está vivo
        p->isBurning = 0;
    }

    // FUNÇÕES DE CRIAÇÃO :: OFF

    // FUNÇÕES DE ATUALIZAÇÃO :: ON
    void updatePlayer(Char *p, char input){ // Função responsável pela movimentação do jogador

        int moveChance = rand()%30;
        int actualDamage;

        map[p->posY][p->posX] = modelMap[p->posY][p->posX] ; // Limpa a posição anterior do jogador no mapa

        // Destruição do jogador
        if (p->hp <= 0){
                p->sprite = '#';
                p->posX = 0;
                p->posY = 0;
        }

        else{

            if(p->isBurning == 1){// Caso o player ainda esteja queimando do tick anterior, dá dano nele novamente e verifica se saiu do burning (1 em 10)
                if ((rand()%10) >= 8){
                    p->isBurning = 0;
                }
                else p->hp = p->hp - (rand()%4+1);
            }

            // Movimentação do jogador nas escadas
            if ((input == KEY_up && map[p->posY-1][p->posX] == CODE_STAIR) || (input == KEY_UP && map[p->posY-1][p->posX] == CODE_STAIR)) p->posY--;
            else if ((input == KEY_left && map[p->posY][p->posX-1] == CODE_STAIR) || (input == KEY_LEFT && map[p->posY][p->posX-1] == CODE_STAIR)) p->posX--;
            else if ((input == KEY_down && map[p->posY+1][p->posX] == CODE_STAIR) || (input == KEY_DOWN && map[p->posY+1][p->posX] == CODE_STAIR)) p->posY++;
            else if ((input == KEY_right && map[p->posY][p->posX+1] == CODE_STAIR) || (input == KEY_RIGHT && map[p->posY][p->posX+1] == CODE_STAIR)) p->posX++;

            // Movimentação do jogador em campo aberto
            if ((input == KEY_up && map[p->posY-1][p->posX] == CODE_BLANKSPACE) || (input == KEY_UP && map[p->posY-1][p->posX] == CODE_BLANKSPACE)) p->posY--;
            else if ((input == KEY_left && map[p->posY][p->posX-1] == CODE_BLANKSPACE) || (input == KEY_LEFT && map[p->posY][p->posX-1] == CODE_BLANKSPACE)) p->posX--;
            else if ((input == KEY_down && map[p->posY+1][p->posX] == CODE_BLANKSPACE) || (input == KEY_DOWN && map[p->posY+1][p->posX] == CODE_BLANKSPACE)) p->posY++;
            else if ((input == KEY_right && map[p->posY][p->posX+1] == CODE_BLANKSPACE) || (input == KEY_RIGHT && map[p->posY][p->posX+1] == CODE_BLANKSPACE)) p->posX++;

            // Movimentação do jogador na água (66% de chance de se movimentar)
            // Caso esteja queimando, ao entrar na água perde o status
            else if ((input == KEY_up && map[p->posY-1][p->posX] == CODE_WATER && moveChance <= 10) || (input == KEY_UP && map[p->posY-1][p->posX] == CODE_WATER && moveChance <= 10)){
                    if (p->isBurning == 1) p->isBurning = 0; // Perde o status Burning
                    p->posY--;
            }
            else if ((input == KEY_left && map[p->posY][p->posX-1] == CODE_WATER && moveChance <= 10) || (input == KEY_LEFT && map[p->posY][p->posX-1] == CODE_WATER && moveChance <= 10)){
                    if (p->isBurning == 1) p->isBurning = 0; // Perde o status Burning
                    p->posX--;
            }
            else if ((input == KEY_down && map[p->posY+1][p->posX] == CODE_WATER && moveChance <= 10) || (input == KEY_DOWN && map[p->posY+1][p->posX] == CODE_WATER && moveChance <= 10)){
                    if (p->isBurning == 1) p->isBurning = 0; // Perde o status Burning
                    p->posY++;
            }
            else if ((input == KEY_right && map[p->posY][p->posX+1] == CODE_WATER && moveChance <= 10) || (input == KEY_RIGHT && map[p->posY][p->posX+1] == CODE_WATER && moveChance <= 10)){
                    if (p->isBurning == 1) p->isBurning = 0; // Perde o status Burning
                    p->posX++;
            }

            // Movimentação do jogador no fogo(Ganhará o status Burning)
            else if ((input == KEY_up && map[p->posY-1][p->posX] == CODE_FIRE) || (input == KEY_UP && map[p->posY-1][p->posX] == CODE_FIRE)){
                    p->hp = p->hp - (rand()%4+1);
                    p->isBurning = 1;
                    p->posY--;
            }
            else if ((input == KEY_left && map[p->posY][p->posX-1] == CODE_FIRE) || (input == KEY_LEFT && map[p->posY][p->posX-1] == CODE_FIRE)){
                    p->hp = p->hp - (rand()%4+1);
                    p->isBurning = 1;
                    p->posX--;
            }
            else if ((input == KEY_down && map[p->posY+1][p->posX] == CODE_FIRE) || (input == KEY_DOWN && map[p->posY+1][p->posX] == CODE_FIRE)){
                    p->hp = p->hp - (rand()%4+1);
                    p->isBurning = 1;
                    p->posY++;
            }
            else if ((input == KEY_right && map[p->posY][p->posX+1] == CODE_FIRE) || (input == KEY_RIGHT && map[p->posY][p->posX+1] == CODE_FIRE)){
                    p->hp = p->hp - (rand()%4+1);
                    p->isBurning = 1;
                    p->posX++;
            }


            //Ataque do jogador
            if (input == KEY_attack || input == KEY_ATTACK){
                if (map[p->posY-1][p->posX] < 0){
                    actualDamage = (rand()%p->weapon)+1;
                    if (actualDamage - enemyList[-(map[p->posY-1][p->posX]) - 1].armor >= 0) enemyList[-(map[p->posY-1][p->posX]) - 1].hp = enemyList[-(map[p->posY-1][p->posX]) - 1].hp - actualDamage;
                    else enemyList[-(map[p->posY-1][p->posX]) - 1].hp = enemyList[-(map[p->posY-1][p->posX]) - 1].hp - actualDamage%2;

                    if (enemyList[-(map[p->posY-1][p->posX]) - 1].hp <= 0) enemyList[-(map[p->posY-1][p->posX]) - 1].isDead++; // Se matar inimigo incrementa quantidade de inimgos mortos
                }
                else if (map[p->posY+1][p->posX] < 0){
                    actualDamage = (rand()%p->weapon)+1;
                    if (actualDamage - enemyList[-(map[p->posY+1][p->posX]) - 1].armor >= 0) enemyList[-(map[p->posY+1][p->posX]) - 1].hp = enemyList[-(map[p->posY+1][p->posX]) - 1].hp - actualDamage;
                    else enemyList[-(map[p->posY+1][p->posX]) - 1].hp = enemyList[-(map[p->posY+1][p->posX]) - 1].hp - actualDamage%2;

                    if (enemyList[-(map[p->posY+1][p->posX]) - 1].hp <= 0) enemyList[-(map[p->posY+1][p->posX]) - 1].isDead++; // Se matar inimigo incrementa quantidade de inimgos mortos
                    }
                else if (map[p->posY][p->posX-1] < 0){
                    actualDamage = (rand()%p->weapon)+1;
                    if (actualDamage - enemyList[-(map[p->posY][p->posX-1]) - 1].armor >= 0) enemyList[-(map[p->posY][p->posX-1]) - 1].hp = enemyList[-(map[p->posY][p->posX-1]) - 1].hp - actualDamage;
                    else enemyList[-(map[p->posY][p->posX-1]) - 1].hp = enemyList[-(map[p->posY][p->posX-1]) - 1].hp - actualDamage%2;

                    if (enemyList[-(map[p->posY][p->posX-1]) - 1].hp <= 0) enemyList[-(map[p->posY][p->posX-1]) - 1].isDead++; // Se matar inimigo incrementa quantidade de inimgos mortos
                    }
                else if (map[p->posY][p->posX+1] < 0){
                    actualDamage = (rand()%p->weapon)+1;
                    if (actualDamage - enemyList[-(map[p->posY][p->posX+1]) - 1].armor >= 0) enemyList[-(map[p->posY][p->posX+1]) - 1].hp = enemyList[-(map[p->posY][p->posX+1]) - 1].hp - actualDamage;
                    else enemyList[-(map[p->posY][p->posX+1]) - 1].hp = enemyList[-(map[p->posY][p->posX+1]) - 1].hp - actualDamage%2;

                    if (enemyList[-(map[p->posY][p->posX+1]) - 1].hp <= 0) enemyList[-(map[p->posY][p->posX+1]) - 1].isDead++; // Se matar inimigo incrementa quantidade de inimgos mortos
                    }

                // Ação ao pegar baús
                if (map[p->posY-1][p->posX] >= CODE_CHEST){
                    switch(chestsContents[(map[p->posY-1][p->posX]) - CODE_CHEST]){
                        case 1: playerHPPotions++;
                        break;
                        case 2: p->armor++;
                        break;
                        case 3: p->weapon++;
                        break;
                        case 4: playerSPPotions++;
                        break;
                        case 5: spMax++;
                        break;
                    }
                    map[p->posY-1][p->posX] = 0;
                }
                else if (map[p->posY+1][p->posX] >= CODE_CHEST){
                    switch(chestsContents[(map[p->posY+1][p->posX]) - CODE_CHEST]){
                        case 1: playerHPPotions++;
                        break;
                        case 2: p->armor++;
                        break;
                        case 3: p->weapon++;
                        break;
                        case 4: playerSPPotions++;
                        break;
                        case 5: spMax++;
                        break;
                    }
                    map[p->posY+1][p->posX] = 0;
                }
                else if (map[p->posY][p->posX-1] >= CODE_CHEST){
                    switch(chestsContents[(map[p->posY][p->posX-1]) - CODE_CHEST]){
                        case 1: playerHPPotions++;
                        break;
                        case 2: p->armor++;
                        break;
                        case 3: p->weapon++;
                        break;
                        case 4: playerSPPotions++;
                        break;
                        case 5: spMax++;
                        break;
                    }
                    map[p->posY][p->posX-1] = 0;
                }
                else if (map[p->posY][p->posX+1] >= CODE_CHEST){
                    switch(chestsContents[(map[p->posY][p->posX+1]) - CODE_CHEST]){
                        case 1: playerHPPotions++;
                        break;
                        case 2: p->armor++;
                        break;
                        case 3: p->weapon++;
                        break;
                        case 4: playerSPPotions++;
                        break;
                        case 5: spMax++;
                        break;
                    }
                    map[p->posY][p->posX+1] = 0;
                }
            }

            //Habilidade especial do jogador
            if (input == KEY_sp_attack || input == KEY_SP_ATTACK){

                if (strcmp(p->chClass, "Warrior") == 0 && p->sp >= SP_HABILITY_COST){ // Habilidade do guerreiro

                    if (map[p->posY-1][p->posX] < 0){
                        enemyList[-(map[p->posY-1][p->posX]) - 1].hp = enemyList[-(map[p->posY-1][p->posX]) - 1].hp - ((((rand()%p->weapon)+1)*3) - (rand()%(enemyList[-(map[p->posY-1][p->posX]) - 1].armor+1)));
                        if (enemyList[-(map[p->posY-1][p->posX]) - 1].hp <= 0) enemyList[-(map[p->posY-1][p->posX]) - 1].isDead++; // Se matar inimigo incrementa quantidade de inimgos mortos
                    }
                    if (map[p->posY+1][p->posX] < 0){
                        enemyList[-(map[p->posY+1][p->posX]) - 1].hp = enemyList[-(map[p->posY+1][p->posX]) - 1].hp - ((((rand()%p->weapon)+1)*3) - (rand()%(enemyList[-(map[p->posY+1][p->posX]) - 1].armor+1)));
                        if (enemyList[-(map[p->posY+1][p->posX]) - 1].hp <= 0) enemyList[-(map[p->posY+1][p->posX]) - 1].isDead++; // Se matar inimigo incrementa quantidade de inimgos mortos
                    }
                    if (map[p->posY][p->posX-1] < 0){
                        enemyList[-(map[p->posY][p->posX-1]) - 1].hp = enemyList[-(map[p->posY][p->posX-1]) - 1].hp - ((((rand()%p->weapon)+1)*3) - (rand()%(enemyList[-(map[p->posY][p->posX-1]) - 1].armor+1)));
                        if (enemyList[-(map[p->posY][p->posX-1]) - 1].hp <= 0) enemyList[-(map[p->posY][p->posX-1]) - 1].isDead++; // Se matar inimigo incrementa quantidade de inimgos mortos
                    }
                    if (map[p->posY][p->posX+1] < 0){
                        enemyList[-(map[p->posY][p->posX+1]) - 1].hp = enemyList[-(map[p->posY][p->posX+1]) - 1].hp - ((((rand()%p->weapon)+1)*3) - (rand()%(enemyList[-(map[p->posY][p->posX+1]) - 1].armor+1)));
                        if (enemyList[-(map[p->posY][p->posX+1]) - 1].hp <= 0) enemyList[-(map[p->posY][p->posX+1]) - 1].isDead++; // Se matar inimigo incrementa quantidade de inimgos mortos
                    }
                    p->sp = p->sp - SP_HABILITY_COST;
                }

                else if (strcmp(p->chClass, "Paladin") == 0 && p->sp >= SP_HABILITY_COST){ // Habilidade do paladino
                        if (p->hp>0 && p->hp <=hpMax - 15)p->hp = p->hp + 15;
                        else if (p->hp > hpMax - 15) p->hp = 100;
                        else p->hp = p->hp + 0;
                        p->sp = p->sp - SP_HABILITY_COST;
                    }


                else if (strcmp(p->chClass, "Barbarian") == 0 && p->sp >= (2*SP_HABILITY_COST)){ // Habilidade do bárbaro
                            berserkMode += 1;
                            hpMax = hpMax + 30;
                            p->hp = p->hp + 30;
                            p->armor = p->armor + 10;
                            p->weapon= p->weapon + 5;
                            p->sp = p->sp - (2*SP_HABILITY_COST);
                }
            }


            // Essa parte agora trata das poções de cura
            if (input == KEY_hp_potion || input == KEY_HP_POTION){
                if(playerHPPotions > 0 && berserkMode == 0){
                    if (p->hp>0 && p->hp <= (hpMax - 20))p->hp = p->hp + 20;
                    else if (p->hp > (hpMax-20)) p->hp = hpMax;
                    else p->hp = p->hp + 0;
                    playerHPPotions--;
                }
            }

            //Essa parte trata das poções de SP
            if (input == KEY_sp_potion || input == KEY_SP_POTION){
                if(playerSPPotions > 0 && berserkMode == 0){
                    if (p->sp>=0 && p->sp <= (spMax - 5))p->sp = p->sp + 5;
                    else if (p->sp > (spMax-5)) p->sp = spMax;
                    else p->sp = p->sp + 0;
                    playerSPPotions--;
                }
            }

            /*
            if (input == 'r' || input == 'R'){ // Reset - for debug
                    createEnemies();
                    createMap();
                    actualLevel = actualLevel + 10;
                    spMax = spMax + 10;
                    p->sp = spMax;
                    p->weapon = p->weapon + 30;
                    p->armor = p->armor + 30;
            }


            if (input == 'x' || input == 'X'){ // Reset - for debug
                    createEnemies();
                    createMap();
                    actualLevel = actualLevel + 1;
                    p->weapon = p->weapon + 30;
                    p->armor = p->armor + 30;
            }
            */


        map[p->posY][p->posX] = p->code; //Aloca a nova posição do jogador no mapa
        }
    }
    void updateEnemies(Char *p){ // Função responsável por mover os inimigos baseando-se na posição do jogador

        int i; // Variável auxiliar do For
        int canMove; // Variável auxiliar da movimentação dos inimigos
        int actualDamage; // Variável que guarda o dano causado nesse tick
        int alreadyMoved;


        for (i=0 ; i<quantEnemies; i++){
            // Destruição dos inimigos
            if (enemyList[i].hp <= 0){
                if (enemyList[i].isDead == 1){
                    enemiesDefeated++;
                    enemyList[i].isDead++;
                }
                map[enemyList[i].posY][enemyList[i].posX] = modelMap[enemyList[i].posY][enemyList[i].posX];
                enemyList[i].code = 0;
                enemyList[i].weapon = 0;
                enemyList[i].posX = i;
                enemyList[i].posY = DIMENSION_Y - 1;
            }
            // Movimentação dos inimigos

            map[enemyList[i].posY][enemyList[i].posX] = modelMap[enemyList[i].posY][enemyList[i].posX]; // Limpa a antiga posição do inimigo no mapa
            canMove = (rand()%100)+1; // Determina a chance de se mover nessa atualização entre 1% e 100%

            if(enemyList[i].isBurning == 1){// Caso o inimigo esteja queimando
                if ((rand()%10) >= 8){
                    enemyList[i].isBurning = 0;
                }
                else enemyList[i].hp = enemyList[i].hp - (rand()%4+1);
                if (enemyList[i].hp <=0){
                        enemyList[i].isDead = enemyList[i].isDead++;
                        enemyList[i].isBurning = 0;
                }

            }

            // Caso o inimigo já esteja ao redor do jogador, ele não se move nessa atualização
            if (enemyList[i].posX == (p->posX + 1)&& enemyList[i].posY == p->posY) canMove = 100;
            if (enemyList[i].posX == (p->posX - 1)&& enemyList[i].posY == p->posY) canMove = 100;
            if (enemyList[i].posY == (p->posY + 1)&& enemyList[i].posX == p->posX) canMove = 100;
            if (enemyList[i].posY == (p->posY - 1)&& enemyList[i].posX == p->posX) canMove = 100;
            // Off

            // Caso o inimigo possa se mover, ele se move (duh!). Existe 65% de chance de o inimigo se mover.
            if (canMove <= 65){

                // Rotina de andar no chão comum
                if(enemyList[i].posX > p->posX && map[enemyList[i].posY][enemyList[i].posX - 1] == CODE_BLANKSPACE) enemyList[i].posX--;
                if(enemyList[i].posY > p->posY && map[enemyList[i].posY - 1][enemyList[i].posX] == CODE_BLANKSPACE) enemyList[i].posY--;
                if(enemyList[i].posX < p->posX && map[enemyList[i].posY][enemyList[i].posX + 1] == CODE_BLANKSPACE) enemyList[i].posX++;
                if(enemyList[i].posY < p->posY && map[enemyList[i].posY + 1][enemyList[i].posX] == CODE_BLANKSPACE) enemyList[i].posY++;

                // Rotina de andar na água
                if(enemyList[i].posX > p->posX && map[enemyList[i].posY][enemyList[i].posX - 1] == CODE_WATER && canMove%2 == 0) enemyList[i].posX--;
                if(enemyList[i].posY > p->posY && map[enemyList[i].posY - 1][enemyList[i].posX] == CODE_WATER && canMove%2 == 0) enemyList[i].posY--;
                if(enemyList[i].posX < p->posX && map[enemyList[i].posY][enemyList[i].posX + 1] == CODE_WATER && canMove%2 == 0) enemyList[i].posX++;
                if(enemyList[i].posY < p->posY && map[enemyList[i].posY + 1][enemyList[i].posX] == CODE_WATER && canMove%2 == 0) enemyList[i].posY++;


                //Rotina de andar no fogo(eles são muito burros :D)
                if(enemyList[i].posX > p->posX && map[enemyList[i].posY][enemyList[i].posX - 1] == CODE_FIRE){
                        enemyList[i].isBurning = 1;
                        enemyList[i].hp = enemyList[i].hp - (rand()%4+1);
                        enemyList[i].posX--;
                }
                if(enemyList[i].posY > p->posY && map[enemyList[i].posY - 1][enemyList[i].posX] == CODE_FIRE){
                        enemyList[i].isBurning = 1;
                        enemyList[i].hp = enemyList[i].hp - (rand()%4+1);
                        enemyList[i].posY--;
                }
                if(enemyList[i].posX < p->posX && map[enemyList[i].posY][enemyList[i].posX + 1] == CODE_FIRE){
                        enemyList[i].isBurning = 1;
                        enemyList[i].hp = enemyList[i].hp - (rand()%4+1);
                        enemyList[i].posX++;
                }
                if(enemyList[i].posY < p->posY && map[enemyList[i].posY + 1][enemyList[i].posX] == CODE_FIRE){
                        enemyList[i].isBurning = 1;
                        enemyList[i].hp = enemyList[i].hp - (rand()%4+1);
                        enemyList[i].posY++;
                }
            }
            // Off


            if(enemyList[i].isBurning == 1){// Caso o inimigo esteja queimando
                if ((rand()%10) >= 8){
                    enemyList[i].isBurning = 0;
                }
                else enemyList[i].hp = enemyList[i].hp - (rand()%4+1);
                if (enemyList[i].hp <=0){
                        enemiesDefeated++;
                        enemyList[i].isDead = enemyList[i].isDead+2;
                        enemyList[i].isBurning = 0;
                }

            }

            // Dano causado pelo inimigo
            // Padrão:  p->hp = p->hp - ((rand()%enemyList[i].weapon + 1) - p->armor);

            if (enemyList[i].posX == (p->posX + 1)&& enemyList[i].posY == p->posY){
                    actualDamage = (rand()%enemyList[i].weapon) + 1;
                    if((actualDamage - p->armor) > 0)p->hp = p->hp - (actualDamage - p->armor);
                    else p->hp = p->hp - actualDamage%2;
            }
            if (enemyList[i].posX == (p->posX - 1)&& enemyList[i].posY == p->posY){
                    actualDamage = (rand()%enemyList[i].weapon) + 1;
                    if((actualDamage - p->armor) > 0)p->hp = p->hp - (actualDamage - p->armor);
                    else p->hp = p->hp - actualDamage%2;
            }
            if (enemyList[i].posY == (p->posY + 1)&& enemyList[i].posX == p->posX){
                    actualDamage = (rand()%enemyList[i].weapon) + 1;
                    if((actualDamage - p->armor) > 0)p->hp = p->hp - (actualDamage - p->armor);
                    else p->hp = p->hp - actualDamage%2;
            }
            if (enemyList[i].posY == (p->posY - 1)&& enemyList[i].posX == p->posX){
                    actualDamage = (rand()%enemyList[i].weapon) + 1;
                    if((actualDamage - p->armor) > 0)p->hp = p->hp - (actualDamage - p->armor);
                    else p->hp = p->hp - actualDamage%2;
            }
            // Off

            map[enemyList[i].posY][enemyList[i].posX] = enemyList[i].code; // Aloca a nova posição do inimigo no mapa
        }
    }
    void updateMap(Char *p){ // Função responável por atualizar o mapa. Também trata da situação de vencer o jogo
        int i; // Variável auxiliar do For
        int quantChest, posXChest, posYChest;


        if (enemiesDefeated >= quantEnemies){ // Quando matar todos os inimigos:

            if (canExistChests == 0){
                //Geram-se os baús de recompensa...
                quantChest = rand()%CHESTS; // Dá a chance de gerar de 0 à 3 baús

                for(i=0; i<quantChest; i++){

                    beginChestGeneration:

                    posXChest = (rand()%DIMENSION_X) - 2;
                    posYChest = (rand()%DIMENSION_Y) - 1;

                    if(map[posYChest][posXChest] == 0 ){
                      map[posYChest][posXChest] = CODE_CHEST+i;
                      chestsContents[i] = (rand()%5)+1;
                    }
                    else goto beginChestGeneration;
                }
            }

             canExistChests = 1; // Não se deve mais gerar baús

            // ... E abrem-se os portões para o próximo nível.
            map[(DIMENSION_Y/2)-2][DIMENSION_X-2] = CODE_WALL;
            map[(DIMENSION_Y/2)+1][DIMENSION_X-2] = CODE_WALL;
            map[(DIMENSION_Y/2)-1][DIMENSION_X-1] = CODE_BLANKSPACE;
            map[(DIMENSION_Y/2)][DIMENSION_X-1] = CODE_BLANKSPACE;
            map[(DIMENSION_Y/2)-1][DIMENSION_X-2] = CODE_STAIR;
            map[(DIMENSION_Y/2)][DIMENSION_X-2] = CODE_STAIR;
            modelMap[(DIMENSION_Y/2)-1][DIMENSION_X-2] = CODE_STAIR;
            modelMap[(DIMENSION_Y/2)][DIMENSION_X-2] = CODE_STAIR;
            map[(DIMENSION_Y/2)-1][DIMENSION_X-1] = CODE_STAIR;
            map[(DIMENSION_Y/2)][DIMENSION_X-1] = CODE_STAIR;


            // Caso o jogador passe pelo portão, ele passa para o próximo andar
            if(p->posX == DIMENSION_X-1 &&(p->posY == (DIMENSION_Y/2)+1 || p->posY == (DIMENSION_Y/2) || p->posY == (DIMENSION_Y/2)-1)){
                if (actualLevel > 99){// Caso o jogador chegue ao 100º andar, significa que ele ganhou o jogo
                    system("cls");
                    printf("\n\n\n\n\n");
                    printf("    #===========================================================#\n");
                    printf("   || Thanks adventurer!                                        ||\n");
                    printf("   ||                                                           ||\n");
                    printf("   || Because of their hard work and dedication, the necromancer||\n");
                    printf("   ||  was finally destroyed and Demeror got rid of a very evil ||\n");
                    printf("   || that plagued this land.                                   ||\n");
                    printf("   ||                                                           ||\n");
                    printf("   || But do not think that evil was defeated. How many other   ||\n");
                    printf("   || towers full of horrors may be hidden by these lands? How  ||\n");
                    printf("   || How many dens are yet to be created? Only you can say.    ||\n");
                    printf("   ||                                                           ||\n");
                    printf("   ||              Go to the fight, adventurer!                 ||\n");
                    printf("    #===========================================================#\n");
                    getch();
                    printf ("\n\n\n\t\tCongratulations! You won the game!\n ");
                    getch();
                    exit(0);
                }

                else{
                    if (berserkMode >= 1){
                        hpMax = hpMax - berserkMode*30;
                        if (p->hp <= 30) p->hp = (rand()%3)+1;
                        else p->hp = p->hp - berserkMode*20;
                        p->armor = p->armor - berserkMode*10;
                        p->weapon = p->weapon - berserkMode*5;
                        berserkMode = 0;
                    }

                    enemiesDefeated = 0; // Reseta a quantidade de inimigos derrotados na sala
                    createMap(); // Gera novo mapa
                    createEnemies(); // Gera novos inimigos
                    p->posX = 1; // Move o jogador para o inicio da sala
                    p->posY = DIMENSION_Y/2; // Move o jogador para o inicio da sala
                    canExistChests = 0; // Reseta se os chests podem ser gerados novamente
                    actualLevel++; // Incrementa o andar subido
                    if(p->sp<spMax) p->sp ++; // Caso o personagem não esteja com seu sp cheio, ele recupera 1 de SP ao passar para o próximo nível

                }

            }
        }
    }
    void updateGame(Char *p){ // Função responsável por atualizar os elementos do jogo

        char input = getch();// Espera e guarda o input do player
        system("cls"); // Limpa a tela para ser desenhada de novo

        updateMap(p);
        updatePlayer(p, input);
        updateEnemies(p);

    }

    // FUNÇÕES DE ATUALIZAÇÃO :: OFF

    // FUNÇÕES DE APRESENTAÇÃO :: ON
    void loadingScreen(){ // Barra de loading
    long int i;
    printf("\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\t\t\t    Loading...\n");
    printf("\t\t        [");
    for(i=0; i<1500000000; i++){
        if (i%100000000 == 0) printf("|");
        else printf("");
    }
    printf("]\n");
    printf("\n\n\n\n\t\t      Please press any key.");
    getchar();
}
    void introGame(){ // Tela estilo "Press Start"
        printf("    #==========================================================#\n");
        printf("  |S|              #=*=*=*=*=*=*=*=*=*=*=*=*=*#                |S|\n");
        printf("  |s|      ~o~O~o~||  The Demeror Chronicles  ||~o~O~o~        |s|\n");
        printf("  |o| 	           #*=*=*=*=*=*=*=*=*=*=*=*=*=#                |o|\n");
        printf("  |S| 	                _____ _                                |S|\n");
        printf("  |s|                  /__   \\ |__   ___                       |s|\n");
        printf("  |o|                    / /\\/ '_ \\ / _ \\                      |o|\n");
        printf("  |S|                   / /  | | | |  __/                      |S|\n");
        printf("  |s|       __          \\/   |_| |_|\\___|              _       |s|\n");
        printf("  |o|    /\\ \\ \\___  ___ _ __ ___  _ __ ___   __ _ _ __ | |_    |o|\n");
        printf("  |S|   /  \\/ / _ \\/ __| '__/ _ \\| '_ ` _ \\ / _` | '_ \\| __|   |S|\n");
        printf("  |s|  / /\\  /  __/ (__| | | (_) | | | | | | (_| | | | | |_    |s|\n");
        printf("  |o|  \\_\\ \\/ \\___|\\___|_|  \\___/|_| |_| |_|\\__,_|_| |_|\\__|   |o|\n");
        printf("  |S|              _____                                       |S|\n");
        printf("  |s|             /__   \\_____      _____ _ __                 |s|\n");
        printf("  |o|               / /\\/ _ \\ \\ /\\ / / _ \\ '__|                |o|\n");
        printf("  |S|              / / | (_) \\ V  V /  __/ |                   |S|\n");
        printf("  |s|              \\/   \\___/ \\_/\\_/ \\___|_|                   |s|\n");
        printf("  |o|                                                          |o|\n");
        printf("    #==========================================================#\n");


        printf("\n\n\n                       Please press any key\n");
        printf("\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\t\t      Paulo Henrique da Silva Ferreira - 2014");
        getch();
        system("cls");
    }
    void firstScreen(Char *p){ // Primeira tela do Jogo
        char chosenClass;


        printf("  #================================================================#\n");
        printf(" ||~~~~~~~~~~~~~~~~~~~~~~~~~~~# PRELUDE #~~~~~~~~~~~~~~~~~~~~~~~~~~||\n");
        printf(" ||                           ===========                          ||\n");
        printf(" ||                                                                ||\n");
        printf(" || By nightfall, could be heard from every corner of Demeror the  ||\n");
        printf(" ||  creaking doors of the Necromancer's Tower being opened again, ||\n");
        printf(" || releasing its horrors to the world and defying anyone to enter ||\n");
        printf(" ||                      into its domain.                          ||\n");
        printf(" ||  But tonight the power of the necromancer will be challenged.  ||\n");
        printf(" || Indistinguishable, the adventurous only becomes fully visible  ||\n");
        printf(" ||    when arriving on the ghostly light emanating from doors,    ||\n");
        printf(" ||    revealing the features and the typical equipment of a ...   ||\n");
        printf(" ||                                                                ||\n");
        printf(" ||~~~~~~~~~~~~~~~~~~~~~~~~ CHOOSE A CLASS:~~~~~~~~~~~~~~~~~~~~~~~~||\n");
        printf(" ||  W - WARRIOR: Can execute a powerful spin attack using SP;     ||\n");
        printf(" ||  P - PALADIN: Can cure yourself using SP;                      ||\n");
        printf(" ||  B - BARBARIAN: Can enter in 'Berserk Mode' using SP;          ||\n");
        printf("  #================================================================# \n");

        beginChooseClass:
        printf ("   ");
        scanf("%c",&chosenClass);

        if (chosenClass == 'w' || chosenClass == 'W'){
            p->sprite = '@';
            strcpy(p->chClass, "Warrior");
        }
        else if (chosenClass == 'p' || chosenClass == 'P'){
            p->sprite = 157;
            strcpy(p->chClass, "Paladin");
        }
        else if (chosenClass == 'b' || chosenClass == 'B'){
            p->sprite = 203;
            strcpy(p->chClass, "Barbarian");
        }
        else{
            printf("   Please choose a valid class.\n");
            goto beginChooseClass;
        }

        printf("  #================================================================# \n");
        printf(" ||                                                                ||\n");
        if (strcmp(p->chClass, "Warrior") == 0) printf(" || ...a Warrior. And once he get in the front of the door, a deep ||\n");
        else if(strcmp(p->chClass, "Paladin") == 0) printf(" || ...a Paladin. And once he get in the front of the door, a deep ||\n");
        else printf(" || ...a Barbarian. And once he get in the front of the door,a deep||\n");

        printf(" || voice that seemed out of the bowels of the earth bellowed full ||\n");
        printf(" || of anger  and rancor: 'WHO DARES TO INVADE THE TOWER OF THE    ||\n");
        printf(" || NECROMANCER?', and so the adventurous promptly replied: 'I am  ||\n");
        printf(" || the one who will rid Demeror of your vile existence, and my    ||\n");
        printf(" ||                           name is ...'                         ||\n");
        printf(" ||~~~~~~~~~~~~~~~~~~~~~~~~~TYPE YOUR NAME:~~~~~~~~~~~~~~~~~~~~~~~~||\n");
        printf(" ||                                                                ||\n");
        printf("  #================================================================# \n");
        printf("   ");
        scanf("%s",&p->name);
        printf("  #================================================================# \n");
        printf(" ||                                                                ||\n");
        if (strcmp(p->chClass, "Warrior") == 0)  printf(" || And so the Warrior entered the tower, with a mission to defeat ||\n");
        else if(strcmp(p->chClass, "Paladin") == 0)  printf(" || And so the Paladin entered the tower, with a mission to defeat ||\n");
        else  printf(" ||And so the Barbarian entered the tower, with a mission to defeat||\n");
        printf(" ||          the Necromancer and rid Demeror of this evil.         ||\n");
        printf("  #================================================================# \n");
                getch();

        system("cls");
    }
    void drawHUD(Char *p){ // Função que cria a HUD (Heads Up Display) e a desenha na tela. Também trata da situação de game-over
        Char tempUp, tempDown, tempLeft, tempRight;
        int i;

        // HUD do jogador
        if (p->hp > 0){
            printf("\t\t    *=====* NAME: %s, the %s\n", p->name, p->chClass);
            if (p->isBurning == 1) printf("\t\t    |B    | HP: %d/%d\n", p->hp, hpMax);
            else printf("\t\t    |     | HP: %d/%d\n", p->hp, hpMax);
            printf("\t\t    |  %c  | SP: %d/%d\n",p->sprite, p->sp, spMax);
            printf("\t\t    |     | ARMOR: %d\n", p->armor);
            printf("\t\t    *=====* WEAPON: %d\n", p->weapon);
            printf("\t\t    HP Pots: %d\n", playerHPPotions);
            printf("\t\t    SP Pots: %d\n", playerSPPotions);
        }

        else{ // Caso o personagem tenha morrido, trata da tela de game over
            printf("  ________                        ________                     \n");
            printf(" /  _____/_____    _____   ____   \\_____  \\___  __ ___________ \n");
            printf("/   \\  ___\\__  \\  /     \\_/ __ \\   /   |   \\  \\/ // __ \\_  __ \\\n");
            printf("\\    \\_\\  \\/ __ \\|  Y Y  \\  ___/  /    |    \\   /\\  ___/|  | \\/\n");
            printf(" \\______  (____  /__|_|  /\\___  > \\_______  /\\_/  \\___  >__|   \n");
            printf("        \\/     \\/      \\/     \\/          \\/          \\/       \n");
            printf("\n\nWould like to start a new quest?(Y/N)\n");

            char rAux = getch();
            if(rAux == 'y' || rAux == 'Y'){
                system("cls");
                createPlayer(p);
                firstScreen(p);
                loadingScreen();
                createMap();
                createEnemies();
            }
            else{
                printf ("\n           Thanks for playing!");
                getch();
                system("cls");
                exit(0);
            }

        }

        /*
        Mais um teste
        for(i=0; i<quantEnemies; i++){
           printf("%s: [%d,%d]\n", enemyList[i].name,enemyList[i].posX, enemyList[i].posY); // Teste
        }
        */


        // Caso haja um inimigo a direita, esquerda, acima ou abaixo do jogador, mostrar suas informações
        if (map[p->posY-1][p->posX] < 0 || map[p->posY+1][p->posX] < 0 || map[p->posY][p->posX-1] < 0 || map[p->posY][p->posX+1] < 0){

            tempUp = enemyList[-(map[p->posY-1][p->posX]) - 1];
            tempDown = enemyList[-(map[p->posY+1][p->posX]) - 1];
            tempLeft = enemyList[-(map[p->posY][p->posX-1]) - 1];
            tempRight = enemyList[-(map[p->posY][p->posX+1]) - 1];

            if (map[p->posY-1][p->posX] < 0){ // Caso haja um inimigo acima do jogador

                printf("\t\t    o=====o NAME:%s\n", tempUp.name);
                if (tempUp.isBurning == 1) printf("\t\t    |B    | HP: %d\n", tempUp.hp);
                else printf("\t\t    |     | HP:%d\n", tempUp.hp);
                printf("\t\t    |  %c  | ARMOR:%d\n", tempUp.sprite, tempUp.armor);
                printf("\t\t    |     | WEAPON:%d\n",tempUp.weapon);
                printf("\t\t    o=====o\n");
            }

            if (map[p->posY+1][p->posX] < 0){

                printf("\t\t    o=====o NAME:%s\n", tempDown.name);
                if (tempDown.isBurning == 1) printf("\t\t    |B    | HP: %d\n", tempDown.hp);
                else printf("\t\t    |     | HP:%d\n", tempDown.hp);
                printf("\t\t    |  %c  | ARMOR:%d\n", tempDown.sprite, tempDown.armor);
                printf("\t\t    |     | WEAPON:%d\n",tempDown.weapon);
                printf("\t\t    o=====o\n");
            }

            if (map[p->posY][p->posX+1] < 0){

                printf("\t\t    o=====o NAME:%s\n", tempRight.name);
                if (tempRight.isBurning == 1) printf("\t\t    |B    | HP: %d\n", tempRight.hp);
                else printf("\t\t    |     | HP:%d\n", tempRight.hp);
                printf("\t\t    |  %c  | ARMOR:%d\n", tempRight.sprite, tempRight.armor);
                printf("\t\t    |     | WEAPON:%d\n",tempRight.weapon);
                printf("\t\t    o=====o\n");
            }

            if (map[p->posY][p->posX-1] < 0){

                printf("\t\t    o=====o NAME:%s\n", tempLeft.name);
                if (tempLeft.isBurning == 1) printf("\t\t    |B    | HP: %d\n", tempLeft.hp);
                else printf("\t\t    |     | HP:%d\n", tempLeft.hp);
                printf("\t\t    |  %c  | ARMOR:%d\n", tempLeft.sprite, tempLeft.armor);
                printf("\t\t    |     | WEAPON:%d\n",tempLeft.weapon);
                printf("\t\t    o=====o\n");
            }

        }
}
    void drawMap(Char *p){ // Função responsável por desenhar o mapa e seus elementos
        int i, j, k; // Variáveis auxiliares dos For
        int actualWall = actualLevel/10;

        printf("\n");
        if (actualLevel < 10){
                printf("\t\t            #===========#\n");
                printf("\t\t           ||  LEVEL: %d ||\n", actualLevel);
        }
        else if (actualLevel >= 10 && actualLevel < 100){
                printf("\t\t           #============#\n");
                printf("\t\t          ||  LEVEL: %d ||\n", actualLevel);
        }
        else{
                printf("\t\t          #=============#\n");
                printf("\t\t         ||  LEVEL: %d ||\n", actualLevel);
        }

        for(i=0; i<DIMENSION_Y; i++){
            printf("\t\t     ");
            for(j=0; j<DIMENSION_X; j++){
                for (k=0; k<MAX_ENEMIES; k++){
                    if (i == enemyList[k].posY && j == enemyList[k].posX && enemyList[k].code < 0) printf ("%c", enemyList[k].sprite); // Desenha os inimigos na tela
                }
                if (i == p->posY && j == p->posX) printf("%c", p->sprite); // Desenha o jogador na tela
                else if (map[i][j] == CODE_BLANKSPACE) printf(" "); // Desenha os epaços livres
                else if (map[i][j] == CODE_WALL){
                    // Desenha as paredes
                    if (actualWall == 0) printf("%c", 35);
                    else if (actualWall == 1) printf("%c", 19);
                    else if (actualWall == 2) printf("%c", 219);
                    else if (actualWall == 3) printf("%c", 206);
                    else if (actualWall == 4) printf("%c", 244);
                    else if (actualWall == 5) printf("%c", 254);
                    else if (actualWall == 6) printf("%c", 186);
                    else if (actualWall == 7) printf("%c", 176);
                    else if (actualWall == 8) printf("%c", 177);
                    else if (actualWall == 9) printf("%c", 178);
                    else printf ("%c", 15);
                }
                else if (map[i][j] >= CODE_CHEST) printf("="); // Desenha os baús
                else if (map[i][j] == CODE_WATER) printf("~"); // Desenha a área com água
                else if (map[i][j] == CODE_FIRE){
                    int flame = rand()%2;
                    if (flame == 0) printf("s"); // Desenha as áreas com fogo
                    else printf("S"); // Desenha as áreas com fogo
                }
                else if (map[i][j] == CODE_STAIR) printf("|");

//                printf ("%d", map[i][j]);
            }
        printf("\n");
        }
    }
    void drawScreen(Char *p){ // Função responsável por desenhar os elementos do jogo

        drawMap(p);
        drawHUD(p);
    }

    // FUNÇÕES DE APRESENTAÇÃO :: OFF

// FUNÇÕES :: OFF

// MAIN :: ON
main(){
    srand(time(NULL)); // Gera a semente dos números aleatóreos
    int gameLoop = 1;
    Char player;

    introGame();
    createPlayer(&player); // Cria o jogador
    firstScreen(&player); // Mostra ao jogador a tela de início

    loadingScreen(); // Loading. Esse loading eh necessário para gerar uma boa seed para o 1º mapa
    createMap(); // Cria o mapa
    createEnemies(); // Cria os inimigos

    while (gameLoop == 1){
        updateGame(&player);
        drawScreen(&player);
    }

}
// MAIN :: OFF
