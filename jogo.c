#include <stdio.h>
#include <stdlib.h>
#include <ncurses.h>
#include "pacman.h"
#include <time.h>
#include <unistd.h>

/* gcc -ansi -Wall -o jogo jogo.c pacman.c pacman.h -lncurses */

int main(){
	int mapa[LIN][COL], pontuacao = 0, ganhou = 0, moveu_blinky = 0, moveu_inky = 0, moveu_pinky = 0, moveu_clyde = 0;
	int lado_blinky = 0, lado_inky = 0, lado_pinky = 0, lado_clyde = 0, contador = 0, nivel = 1, tempo = 80000, tamanho_terminal;
	int key;
	struct pacman player;
	struct fantasma blinky, pinky, inky, clyde;
	srand(time(NULL));
	inicializa_ncurses();
	tamanho_terminal = verifica_terminal();
	if(tamanho_terminal == 0){
    	endwin();
    	printf("Tamanho do terminal insuficiente, tente novamente\n");
    	exit(0);
    }
	animacao_inicial();
	regras();
	inicializa_mapa(mapa);
	inicializa_pacman(&player);
	inicializa_fantasmas(&blinky, &pinky, &inky, &clyde);
	key = getch();
	while(key != KEY_F(1)){ /* Tecla para sair do jogo é F1 */
		key = getch();
		switch(key){
			case KEY_RIGHT:
				pontuacao = move_direita(&player, mapa, pontuacao);
				break;
			case KEY_LEFT:
				pontuacao = move_esquerda(&player, mapa, pontuacao);
				break;
			case KEY_UP:
				pontuacao = move_cima(&player, mapa, pontuacao);
				break;
			case KEY_DOWN:
				pontuacao = move_baixo(&player, mapa, pontuacao);
				break;
		}
		if(pontuacao == 1) /* Se a função de movimento retornar 1, colidiu com um fantasma */
			game_over(mapa, pontuacao, nivel); /* Mensagem de fim de jogo */
		while(getch() != ERR) {} /* Limpa o buffer do teclado após a tecla ser lida */
		if(contador % 3 == 0){ /* Os fantasmas se movem a cada 3 ciclos para não serem tão rápidos */
			/* Os if's abaixo precisam estar no programa principal para que os fantasmas consigam se mover de forma "flúida" */
			/* tentei colocar os mesmos em funções, mas quando estão fora da main eles quase não se movem no mapa, */
			/* andando apenas uma ou duas posições antes de mudar de direção, mas dessa forma, eles andam na mesma direção até achar uma parede*/
			/* Movimentação do Blinky */
			moveu_blinky = move_fantasma(&blinky, mapa, lado_blinky); /* A função move_fantasma retorna 6 valores possíveis */
			if(moveu_blinky == 1) /* Se retornar 1 o movimento foi bem sucedido e ele continua na mesma direção */
				moveu_blinky = move_fantasma(&blinky, mapa, lado_blinky); /* Entre 2-5 é erro, então sorteia-se uma nova direção para seguir*/
			if(moveu_blinky >= 2 && moveu_blinky <= 5){
				lado_blinky = rand() % 4;
				moveu_blinky = move_fantasma(&blinky, mapa, lado_blinky);
			}
			if(moveu_blinky == 6) /* Se retornou 6 houve colisão entre o fantasma e o pacman, o jogador perde */
				game_over(mapa, pontuacao, nivel);
			/* Movimentação do Inky */
			moveu_inky = move_fantasma(&inky, mapa, lado_inky); 
			if(moveu_inky == 1) 
				moveu_inky = move_fantasma(&inky, mapa, lado_inky); 
			if(moveu_inky >= 2 && moveu_inky <= 5){
				lado_inky = rand() % 4;
				moveu_inky = move_fantasma(&inky, mapa, lado_inky);
			}
			if(moveu_inky == 6) 
				game_over(mapa, pontuacao, nivel);	
			/* Movimentação da Pinky */
			moveu_pinky = move_fantasma(&pinky, mapa, lado_pinky); 
			if(moveu_pinky == 1) 
				moveu_pinky = move_fantasma(&pinky, mapa, lado_pinky); 
			if(moveu_pinky >= 2 && moveu_pinky <= 5){
				lado_pinky = rand() % 4;
				moveu_pinky = move_fantasma(&pinky, mapa, lado_pinky);
			}
			if(moveu_pinky == 6) 
				game_over(mapa, pontuacao, nivel);
			/* Movimentação do Clyde */
			moveu_clyde = move_fantasma(&clyde, mapa, lado_clyde); 
			if(moveu_clyde == 1) 
				moveu_clyde = move_fantasma(&clyde, mapa, lado_clyde); 
			if(moveu_clyde >= 2 && moveu_clyde <= 5){
				lado_clyde = rand() % 4;
				moveu_clyde = move_fantasma(&clyde, mapa, lado_clyde);
			}
			if(moveu_clyde == 6) 
				game_over(mapa, pontuacao, nivel);
		}
		ganhou = verifica_ganhador(mapa);
		if(ganhou && nivel <= 3){
			level_up();
			inicializa_mapa(mapa);
			inicializa_pacman(&player);
			inicializa_fantasmas(&blinky, &pinky, &inky, &clyde);
			nivel++;
			tempo = tempo - 10000; /* Aumenta velocidade do pacman em aprox. 10% */
			refresh();
		}
		if(ganhou && nivel == 4) /* O jogo tem 3 níveis jogáveis, no 4 o jogador ganhou */
			game_won(mapa, pontuacao, nivel);
		atualiza_pacman(&player, mapa);
		atualiza_fantasmas(&blinky, &pinky, &inky, &clyde, mapa);
		printa_mapa(mapa);
		printa_pontuacao(pontuacao);
		printa_nivel(nivel);
		refresh();
		usleep(tempo);
		clear();
		contador++; /* Conta os ciclos de loop, a cada 3 os fantasmas se movem */
	}
	endwin();
	return 0;
}
