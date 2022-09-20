#include "raylib.h"
#include <stdlib.h>
#include <time.h>
#include <stdio.h>
#define ARESTA 40
#define LARGURA 1080
#define ALTURA 440
#define COLUNA 27
#define LINHA 11
#define N 100
#define RAIO 20
#define EXPLOSAO 3

typedef struct{
    int xPos;
    int yPos;
    int ultMov;
}JOGADOR;

typedef struct{
    int xPos;
    int yPos;
    int tipo;
}CRIATURA;

typedef struct{
    double tempo;
    int plantada;
    int xPos, yPos;
    int xExp[EXPLOSAO*EXPLOSAO], yExp[EXPLOSAO*EXPLOSAO], estadoExp;
    double tempoExp;
}BOMBA;

void movDir( int *xPos, int *yPos, int tecla); // Altera o valor da posição baseado no valor apertado no teclado

int podeMover( int xPos, int yPos, char matriz[][COLUNA], int tecla, int linha, int coluna, BOMBA bomba);// Retorna 1 se o movimento eh valido

int tempoCmp(double tempo, float tempoPerc); // Marca o passar de um tempo "tempoPerc"

void plantarBomba(BOMBA *bomba, JOGADOR jogador); // Função que executa as funcionalidades iniciais de plantar a bomba

void expBomba(BOMBA *bomba, int raio); // Função que passa as posicoes x,y da explosao

void controleBomba(BOMBA *bomba, char matriz[][COLUNA], int tecla, JOGADOR jogador); // Funcao que executa todos os controles de estado da bomba

int main(void)
{

    char matriz[LINHA][COLUNA] = {
        'W','W','W','W','W','W','W','W','W','W','W','W','W','W','W','W','W','W','W','W','W','W','W','W','W','W','W',
        'W','J',' ',' ','P','P','P','P',' ',' ',' ',' ',' ','P',' ',' ',' ',' ','M','W','W','W','W',' ',' ','K','W',
        'W',' ','W',' ','M',' ',' ',' ','W','W','W','D','W','W','W','D','W','W',' ',' ',' ',' ',' ',' ','W',' ','W',
        'W',' ','W','W','W','D','D','W','W','W','W','P','W','P','W','P','W','W','W','W','W',' ','P','P','P',' ','W',
        'W',' ',' ',' ','K',' ',' ',' ',' ',' ','W',' ','D','P','W',' ',' ',' ','M',' ',' ',' ','P',' ',' ',' ','W',
        'W','W','W','W','W','W','W',' ','W','W','W',' ','W','P','W',' ','W',' ','P','P','P','P','P','P','P','P','W',
        'W','K',' ',' ',' ',' ','D',' ',' ',' ',' ',' ','W','P','W',' ',' ',' ','D',' ',' ',' ',' ',' ',' ','K','W',
        'W',' ','W','W','W','W','W',' ','W','W','W','W','W','W','W','W','W',' ','W',' ','W','W','W','W','W',' ','W',
        'W',' ','M',' ',' ',' ','D',' ','P','P','P','P','P','P','P','P','P',' ','D',' ',' ',' ',' ',' ','W','M','W',
        'W','P','K',' ','W',' ',' ','P','W','W','W','W','W','W','W','W','W','P',' ',' ',' ','W',' ',' ',' ',' ','W',
        'W','W','W','W','W','W','W','W','W','W','W','W','W','W','W','W','W','W','W','W','W','W','W','W','W','W','W',
    };


    //Declaracao de variaveis
    int posX, posY, i, j, x, tecla;
    Color cor[50] = {LIGHTGRAY, RED, GREEN, MAGENTA, SKYBLUE, VIOLET,  BLACK};
    JOGADOR jogador;
    CRIATURA criatura[N];
    BOMBA bomba;

    for(i = 0; i < LINHA; i++){
        for(j = 0; j < COLUNA; j++){
            if(matriz[i][j] == 'J'){
                jogador.xPos = i;
                jogador.yPos = j;
                matriz[i][j] = ' ';
            }
        }
    }


    //Inicializacoes de variaveis e aspectos da raylib
    srand(time(NULL));
    posX = 0;//coordenada x central
    posY = 0;//coordenada y central
    bomba.plantada = 0;
    jogador.ultMov = KEY_RIGHT;
    bomba.estadoExp = 0;
    InitWindow(LARGURA, ALTURA, "Jogo"); // Ajusta o titulo da janela
    SetTargetFPS(60);//Ajusta o jogo para executar a 60 quadros por segundo

    //Loop principal
    while (!WindowShouldClose())// Detecta se deve continuar
    {
        tecla = GetKeyPressed();

        posY = 0;


        // Movimentacao do jogador
        if( tecla == KEY_DOWN || tecla == KEY_UP || tecla == KEY_LEFT || tecla == KEY_RIGHT){
            jogador.ultMov = tecla; // Salvando ultimo movimento apertado
            if(podeMover(jogador.xPos, jogador.yPos, matriz, tecla, LINHA, COLUNA, bomba))
                movDir(&jogador.xPos, &jogador.yPos, tecla);
        }



        // Controle da bomba
        controleBomba(&bomba,matriz,tecla,jogador);




        BeginDrawing();
        ClearBackground(RAYWHITE);//Limpa e define uma cor de fundo


        for(i = 0; i < LINHA; i++){
            for(j = 0; j < COLUNA; j++){

                if(matriz[i][j] == 'W'){
                    DrawRectangle(posX, posY, ARESTA, ARESTA, cor[0]);//Desenha o quadrado na posicao adequada
                    posX += 40;
                }else if(matriz[i][j] == 'M'){
                    DrawRectangle(posX, posY, ARESTA, ARESTA, cor[1]);//Desenha o quadrado na posicao adequada
                    posX += 40;
                }else if(matriz[i][j] == 'K'){
                    DrawRectangle(posX, posY, ARESTA, ARESTA, cor[2]);//Desenha o quadrado na posicao adequada
                    posX += 40;
                }else if(matriz[i][j] == 'P'){
                    DrawRectangle(posX, posY, ARESTA, ARESTA, cor[3]);//Desenha o quadrado na posicao adequada
                    posX += 40;
                }else if(matriz[i][j] == 'D'){
                    DrawRectangle(posX, posY, ARESTA, ARESTA, cor[5]);//Desenha o quadrado na posicao adequada
                    posX += 40;
                }else if(matriz[i][j] == ' '){
                    DrawRectangle(posX, posY, ARESTA, ARESTA, cor[6]);//Desenha o quadrado na posicao adequada
                    posX += 40;
                }
            }

            posX = 0;
            posY += 40;
        }


        DrawRectangle(jogador.xPos * ARESTA, jogador.yPos * ARESTA, ARESTA, ARESTA, cor[4]);

        if(bomba.plantada) // Desenhar a bomba se ela estiver plantada
            DrawCircle(bomba.xPos * ARESTA + RAIO, bomba.yPos * ARESTA + RAIO, RAIO, WHITE);
        if(bomba.estadoExp){
            for(i = 0; i < EXPLOSAO * EXPLOSAO; i++){
                if(matriz[bomba.yExp[i]][bomba.xExp[i]] != 'W')
                    DrawCircle(bomba.xExp[i] * ARESTA + RAIO,bomba.yExp[i] * ARESTA + RAIO, RAIO, RED);
            }
        }
        EndDrawing();
        }

    CloseWindow();// Fecha janela
    return 0;
}
void plantarBomba(BOMBA *bomba, JOGADOR jogador)
{
    bomba -> tempo = GetTime(); // Instante que a bomba foi plantada
    bomba -> plantada = 1;

    movDir(&jogador.xPos, &jogador.yPos, jogador.ultMov); // Proxima posicao da bomba
    bomba -> xPos = jogador.xPos;
    bomba -> yPos = jogador.yPos;
}
int tempoCmp(double tempo, float tempoPerc)
{
    int passouTempo = 0;

    if(GetTime() - tempo > tempoPerc)// Se o tempo for maior que o esperado
        passouTempo = 1;


    return passouTempo;
}

void movDir( int *xPos, int *yPos, int tecla)
{
       switch(tecla){
    case KEY_DOWN:
        *yPos += 1;
        break;
    case KEY_UP:
        *yPos -= 1;
        break;
    case KEY_RIGHT:
        *xPos += 1;
        break;
    case KEY_LEFT:
        *xPos -= 1;
        break;
    }
}

int podeMover( int xPos, int yPos, char matriz[][COLUNA], int tecla, int linha, int coluna, BOMBA bomba)
{
    int flag_mov = 0;

    movDir(&xPos, &yPos, tecla);

    if(xPos >= 0 && xPos < coluna && yPos >= 0 && yPos < linha && matriz[yPos][xPos] != 'W' && matriz[yPos][xPos] != 'D' // Não bater em parede
       && (xPos != bomba.xPos || yPos != bomba.yPos || !bomba.plantada )) // Não for na mesma posicao da bomba
        flag_mov = 1;

    return flag_mov;
}

void expBomba(BOMBA *bomba, int raio)
{
    int i, j, contPos = 0;

    for(i = bomba->xPos - raio/2; i <= bomba->xPos + raio/2; i++){
        for(j = bomba->yPos - raio/2; j <= bomba->yPos + raio/2; j++){
            //Lendo todos os pontos da matriz detro da explosao
            bomba->xExp[contPos] = i;
            bomba->yExp[contPos] = j;
            contPos++;
        }
    }
}

void controleBomba(BOMBA *bomba, char matriz[][COLUNA], int tecla, JOGADOR jogador)
{
    int i;
    // Colocação da Bomba
    if(tecla == KEY_SPACE && !bomba->plantada &&
       podeMover(jogador.xPos, jogador.yPos, matriz, jogador.ultMov, LINHA, COLUNA, *bomba)){ // Bomba so pode ser mandada se o jogador estiver olhando para um ponto livre
        plantarBomba(bomba, jogador);
        bomba->tempoExp = 0;

    } // Tempo de explosao da bomba
    else if(tempoCmp(bomba->tempo, 4) && bomba->plantada) // Se acabar o tempo
    {
        bomba->plantada = 0;
        expBomba(bomba, EXPLOSAO);
        bomba->tempoExp = GetTime();
        bomba->estadoExp = 1;

    } // Explosao da bomba
    else if(tempoCmp(bomba->tempoExp, .15) && bomba->estadoExp){
        bomba->estadoExp = 0;
      for(i = 0; i < 9; i++)
        if(matriz[bomba->yExp[i]][bomba->xExp[i]] == 'D') // Removendo as paredes, talvez isso deva ser feito em outra parte código
              matriz[bomba->yExp[i]][bomba->xExp[i]] = ' ';
    }
}
