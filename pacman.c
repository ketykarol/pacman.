#include <stdio.h>
#include <stdlib.h>
#include <ncurses.h>
#include "pacman.h"
#include <unistd.h>


/* Funções de movimentação */

int move_direita(struct pacman *player, int mapa[LIN][COL], int pontos){
	int i, cont = 0;
	modifica_corpo(player, 1); /* Para cada movimento o Pacman tem uma "posição" diferente */
	for(i = 0;i < 3;i++){
		if(mapa[i+player->lin][player->col+3] == 0) /* Verifico a coluna direita ao lado do pacman */
			cont++; /* Para cada espaço livre, conto uma vez */
		if(mapa[i+player->lin][player->col+3] == 2){ /* Se o valor nessa coluna for 2 (uma moeda) são adicionados pontos */
			cont++;
			pontos = pontos + 10;
		}
		if(mapa[i+player->lin][player->col+3] == -1){ /* Pastilha especial dá 50 pontos bonus */
			cont++;
			pontos = pontos + 50; 
		}
		if(mapa[i+player->lin][player->col+3] == 1) /* Se o valor for 1 (uma parede) ele para de se mover imediatamente */
			return pontos;
		if(mapa[i+player->lin][player->col+3] >= 6) /* Se o valor for maior ou igual a 6 (fantasmas) o jogador perde */
			return 1;
	}
	if(cont == 3){ /* Se no total houverem 3 espaços livres, o pacman pode mover-se para a direita*/
		player->col++; /* Incremento em 1 a coordenada das colunas */
		for(i = 0;i < 3;i++)
			mapa[player->lin+i][player->col-1] = 0; /* Apago do mapa onde o pacman estava anteriormente */
	}
	return pontos; /* Atualizo a pontuação */
}

int move_esquerda(struct pacman *player, int mapa[LIN][COL], int pontos){
	int i, cont = 0;
	modifica_corpo(player, 2);
	for(i = 0;i < 3;i++){
		if(mapa[i+player->lin][player->col-1] == 0)
			cont++;
		if(mapa[i+player->lin][player->col-1] == 2){
			cont++;
			pontos = pontos + 10;
		}
		if(mapa[i+player->lin][player->col-1] == -1){
			cont++;
			pontos = pontos + 50;
		}
		if(mapa[i+player->lin][player->col-1] == 1)
			return pontos;
		if(mapa[i+player->lin][player->col-1] >= 6)
			return 1;
	}
	if(cont == 3){
		player->col--;
		for(i = 0;i < 3;i++)
			mapa[player->lin+i][player->col+3] = 0;
	}
	return pontos;
}

int move_cima(struct pacman *player, int mapa[LIN][COL], int pontos){
	int j, cont = 0;
	modifica_corpo(player, 4);
	for(j = 0;j < 3;j++){
		if(mapa[player->lin-1][player->col+j] == 0)
			cont++;
		if(mapa[player->lin-1][player->col+j] == 2){
			cont++;
			pontos = pontos + 10;
		}
		if(mapa[player->lin-1][player->col+j] == -1){
			cont++;
			pontos = pontos + 50;
		}
		if(mapa[player->lin-1][player->col+j] == 1)
			return pontos;
		if(mapa[player->lin-1][player->col+j] >= 6)
			return 1;
	}
	if(cont == 3){
		player->lin--;
		for(j = 0;j < 3;j++)
			mapa[player->lin+3][player->col+j] = 0;
	}
	return pontos;
}

int move_baixo(struct pacman *player, int mapa[LIN][COL], int pontos){
	int j, cont = 0;
	modifica_corpo(player, 3);
	for(j = 0;j < 3;j++){
		if(mapa[player->lin+3][player->col+j] == 0)
			cont++;
		if(mapa[player->lin+3][player->col+j] == 2){
			cont++;
			pontos = pontos + 10;
		}
		if(mapa[player->lin+3][player->col+j] == -1){
			cont++;
			pontos = pontos + 50;
		}
		if(mapa[player->lin+3][player->col+j] == 1)
			return pontos;
		if(mapa[player->lin+3][player->col+j] >= 6)
			return 1;
	}
	if(cont == 3){
		player->lin++;
		for(j = 0;j < 3;j++)
			mapa[player->lin-1][player->col+j] = 0;
	}
	return pontos;
}

/* Movimentação dos fantasmas */

int move_fantasma(struct fantasma *fantasma, int mapa[LIN][COL], int comeco){
	int ok; /* Função que checa as laterais do fantasma, e então, se for possível, move no sentido mandado para a função */
	ok = verifica_laterais_fantasma(fantasma, mapa, comeco);
	switch(ok){ /* A varivável começo é onde o programa começará a verificação (se começará por baixo, cima, etc) */
		case 0: /* Casos 0-3 são erros onde a lateral não está livre */
			return 2;
		case 1:
			return 3;
		case 2:
			return 4;
		case 3:
			return 5;
		case 4: /* Casos 4-7 é possível fazer o movimento para o lado selecionado */
			move_fantasma_esquerda(fantasma, mapa);
			return 1;
		case 5:
			move_fantasma_baixo(fantasma, mapa);
			return 1;
		case 6:
			move_fantasma_direita(fantasma, mapa);
			return 1;
		case 7:
			move_fantasma_cima(fantasma, mapa);
			return 1;
		case 8: /* Caso a função de verificação retornar 8, houve colisão com pacman */
			return 6;
	}
	return 0;
}

int verifica_laterais_fantasma(struct fantasma *fantasma, int mapa[LIN][COL], int comeco){
	int i, j, cont = 0; /* Variável começo funciona como prioridade para verificação, se a prioridade é pra baixo, começa verificando em baixo*/
	if(comeco == 0){ /* Verifica lado esquerdo */
		for(i = 0;i < 3;i++){
			if(mapa[i+fantasma->lin][fantasma->col-1] == 0) /* 0, 2 e -1 são terrenos livres */
				cont++; /* A variável cont conta os espaços vazios à frente */
			if(mapa[i+fantasma->lin][fantasma->col-1] == 2)
				cont++;
			if(mapa[i+fantasma->lin][fantasma->col-1] == -1)
				cont++;
			if(mapa[i+fantasma->lin][fantasma->col-1] == 1) /* 1 é parede, impossivel passar */
				return 0;
			if(mapa[i+fantasma->lin][fantasma->col-1] >= 3) /* Entre 3-6 são caracteres de um dos "corpos" do pacman */
				if(mapa[i+fantasma->lin][fantasma->col-1] <= 6) /* ou seja, uma colisão */
					return 8;
		}
		if(cont == 3) /* Se houverem 3 espaços vazios, a linha ou coluna à frente está livre */
			return 4;
		else
			return 0;
	}
	if(comeco == 1){ /* Verifica lado direito */
		for(j = 0;j < 3;j++){
			if(mapa[fantasma->lin+3][fantasma->col+j] == 0)
				cont++;
			if(mapa[fantasma->lin+3][fantasma->col+j] == 2)
				cont++;
			if(mapa[fantasma->lin+3][fantasma->col+j] == -1)
				cont++;
			if(mapa[fantasma->lin+3][fantasma->col+j] == 1)
				return 1;
			if(mapa[fantasma->lin+3][fantasma->col+j] >= 3)
				if(mapa[fantasma->lin+3][fantasma->col+j] <= 6)
					return 8;
		}
		if(cont == 3)
			return 5;
		else
			return 1;
	}
	if(comeco == 2){ /* Verifica em baixo */
		for(i = 0;i < 3;i++){
			if(mapa[i+fantasma->lin][fantasma->col+3] == 0)
				cont++;
			if(mapa[i+fantasma->lin][fantasma->col+3] == 2)
				cont++;
			if(mapa[i+fantasma->lin][fantasma->col+3] == -1)
				cont++;
			if(mapa[i+fantasma->lin][fantasma->col+3] == 1)
				return 2;
			if(mapa[i+fantasma->lin][fantasma->col+3] >= 3)
				if(mapa[i+fantasma->lin][fantasma->col+3] <= 6)
					return 8;
		}
		if(cont == 3)
			return 6;
		else
			return 2;
	}
	if(comeco == 3){ /* Verifica em cima */
		for(j = 0;j < 3;j++){
			if(mapa[fantasma->lin-1][fantasma->col+j] == 0)
				cont++;
			if(mapa[fantasma->lin-1][fantasma->col+j] == 2)
				cont++;
			if(mapa[fantasma->lin-1][fantasma->col+j] == -1)
				cont++;
			if(mapa[fantasma->lin-1][fantasma->col+j] == 1)
				return 3;
			if(mapa[fantasma->lin-1][fantasma->col+j] >= 3)
				if(mapa[fantasma->lin-1][fantasma->col+j] <= 6)
					return 8;
		}
		if(cont == 3)
			return 7;
		else
			return 3;
	}
	return 0;
}

void move_fantasma_esquerda(struct fantasma *fantasma, int mapa[LIN][COL]){
	int i;
	fantasma->col--; /* Para movimentar, a linha ou coluna da estrutura do fantasma é incrementada ou decrementada */
	for(i = 0;i < 3;i++) /* dependendo do movimento */
		if(mapa[fantasma->lin+i][fantasma->col-1] == 0 || mapa[fantasma->lin+i][fantasma->col-1] == 2 || mapa[fantasma->lin+i][fantasma->col-1] == -1)
			mapa[fantasma->lin+i][fantasma->col+3] = mapa[fantasma->lin+i][fantasma->col-1];
		else /* Após passar por um local, o fantasma copia o que vê a frente e coloca por onde ele passou */
			mapa[fantasma->lin+i][fantasma->col+3] = 0;	/* Por exemplo, se à frente é vazio, atrás dele surgirá um vazio */
} /* No caso, apenas copia se for 0 ou 2 ou -1, se for 1 (parede) à sua frente, atrás dele surgirá um espaço vazio, assim os fantasmas */
 /* não criam paredes do nada */
void move_fantasma_direita(struct fantasma *fantasma, int mapa[LIN][COL]){
	int i;
	fantasma->col++;
	for(i = 0;i < 3;i++)
		if(mapa[fantasma->lin+i][fantasma->col+3] == 0 || mapa[fantasma->lin+i][fantasma->col+3] == 2 || mapa[fantasma->lin+i][fantasma->col+3] == -1)
			mapa[fantasma->lin+i][fantasma->col-1] = mapa[fantasma->lin+i][fantasma->col+3];
		else
			mapa[fantasma->lin+i][fantasma->col-1] = 0;
}

void move_fantasma_baixo(struct fantasma *fantasma, int mapa[LIN][COL]){
	int j;
	fantasma->lin++;
	for(j = 0;j < 3;j++)
		if(mapa[fantasma->lin+3][fantasma->col+j] == 0 || mapa[fantasma->lin+3][fantasma->col+j] == 2 || mapa[fantasma->lin+3][fantasma->col+j] == -1)
			mapa[fantasma->lin-1][fantasma->col+j] = mapa[fantasma->lin+3][fantasma->col+j];
		else
			mapa[fantasma->lin-1][fantasma->col+j] = 0;
}

void move_fantasma_cima(struct fantasma *fantasma, int mapa[LIN][COL]){
	int j;
	fantasma->lin--;
	for(j = 0;j < 3;j++)
		if(mapa[fantasma->lin-1][fantasma->col+j] == 0 || mapa[fantasma->lin-1][fantasma->col+j] == 2 || mapa[fantasma->lin-1][fantasma->col+j] == -1)
			mapa[fantasma->lin+3][fantasma->col+j] = mapa[fantasma->lin-1][fantasma->col+j];
		else
			mapa[fantasma->lin+3][fantasma->col+j] = 0;
}

/* Funções de inicialização */

void inicializa_ncurses(void){
	initscr();              /* inicia a tela */
    cbreak();               /* desabilita o buffer de entrada */
    noecho();               /* não mostra os caracteres digitados */
    nodelay(stdscr, TRUE);  /* faz com que getch não aguarde a digitação */
    curs_set(FALSE);        /* não mostra cursor na tela */
    keypad(stdscr, TRUE);   /* permite a leitura das setas */
    start_color();          /* permite o uso de cores */
    init_pair(1, COLOR_BLUE, COLOR_BLACK); /* inicializo os pares de cores que serão utilizados */
    init_pair(2, COLOR_YELLOW, COLOR_BLACK);
    init_pair(3, COLOR_RED, COLOR_BLACK);
    init_pair(4, COLOR_MAGENTA, COLOR_BLACK);
    init_pair(5, COLOR_CYAN, COLOR_BLACK);
    init_pair(6, COLOR_YELLOW, COLOR_BLUE);
    init_pair(7, COLOR_GREEN, COLOR_BLACK);
    bkgd(COLOR_PAIR(1)); /* Cor padrão são caracteres azuis e funco preto */
}

void inicializa_pacman(struct pacman *player){ /* Atribuo valores ao corpo do pacman para que ele seja mostrado no mapa */
	int i, j;
	for(i = 0;i < 3;i++)
		for(j = 0;j < 3;j++)
			player->corpo[i][j] = 3;
	player->corpo[1][2] = 0;
	player->lin = 9; /* Coordenadas onde o pacman iniciará no jogo */
	player->col = 39;
}

void inicializa_fantasmas(struct fantasma *blinky, struct fantasma *pinky, struct fantasma *inky, struct fantasma *clyde){
	int i, j;
	for(i = 0;i < 3;i++)
		for(j = 0;j < 3;j++)
			blinky->corpo[i][j] = 7;
	blinky->corpo[2][1] = 0;
	blinky->lin = 1; /* Exatamente igual à função inicia pacman, mas com os quatro fantasmas */
	blinky->col = 76;
	blinky->id = 1;
	for(i = 0;i < 3;i++)
		for(j = 0;j < 3;j++)
			inky->corpo[i][j] = 8;
	inky->corpo[2][1] = 0;
	inky->lin = 25; 
	inky->col = 76;
	inky->id = 2;
	for(i = 0;i < 3;i++)
		for(j = 0;j < 3;j++)
			pinky->corpo[i][j] = 10;
	pinky->corpo[2][1] = 0;
	pinky->lin = 1; 
	pinky->col = 1;
	pinky->id = 3;
	for(i = 0;i < 3;i++)
		for(j = 0;j < 3;j++)
			clyde->corpo[i][j] = 9;
	clyde->corpo[2][1] = 0;
	clyde->lin = 25; 
	clyde->col = 1;
	clyde->id = 4;
}

void inicializa_mapa(int mapa[LIN][COL]){
	int i, j;
	for(i = 0;i < LIN;i++) /* Primeiro o campo é inicializado totalmente vazio, apenas 0's */
		for(j = 0;j < COL;j++)
			mapa[i][j] = 0;
	/* Bordas verticais */
	for(i = 0;i < LIN-1;i++){ 
		mapa[i][0] = 1;
		mapa[i][COL - 1] = 1;
	}
	/* E horizontais */
	for(j = 0;j < COL;j++){ 
		mapa[0][j] = 1;
		mapa[LIN - 2][j] = 1;
	}
	/* Moedas no mapa */
	for(i = 0;i <= LIN-2;i++)
		for(j = 0;j <= COL;j+=2)
			if(mapa[i][j] == 0)
				mapa[i][j] = 2;
	mapa[LIN-1][0] = 0;
    /* "Caixa" central */
	for(i = 12;i <= 16;i++)
		for(j = 33;j <= 47;j++)
			mapa[i][j] = 1;
	for(i = 13;i <= 15;i++)
		for(j = 34;j <= 46;j++)
			mapa[i][j] = 0;
	/* Caixas superiores */
	for(i = 4;i <= 8;i++) /* Direita */
		for(j = 55;j <= 61;j++)
			mapa[i][j] = 1;
	for(i = 5;i <= 7;i++)
		for(j = 56;j <= 60;j++)
			mapa[i][j] = 0;
	for(i = 4;i <= 8;i++)
		for(j = 69;j <= 75;j++)
			mapa[i][j] = 1;
	for(i = 5;i <= 7;i++)
		for(j = 70;j <= 74;j++)
			mapa[i][j] = 0;
	for(i = 4;i <= 8;i++) /* Esquerda */
		for(j = 4;j <= 10;j++)
			mapa[i][j] = 1;
	for(i = 5;i <= 7;i++)
		for(j = 5;j <= 9;j++)
			mapa[i][j] = 0;
	for(i = 4;i <= 8;i++)
		for(j = 18;j <= 25;j++)
			mapa[i][j] = 1;
	for(i = 5;i <= 7;i++)
		for(j = 19;j <= 24;j++)
			mapa[i][j] = 0;
	/* Caixas laterais */
	for(i = 12;i <= 16;i++) /* Direita */
		for(j = 55;j <= 75;j++)
			mapa[i][j] = 1;
	for(i = 13;i <= 15;i++)
		for(j = 56;j <= 74;j++)
			mapa[i][j] = 0;
	for(i = 12;i <= 16;i++) /* Esquerda */
		for(j = 4;j <= 25;j++)
			mapa[i][j] = 1;
	for(i = 13;i <= 15;i++)
		for(j = 5;j <= 24;j++)
			mapa[i][j] = 0;
	/* Caixas inferiores */
	for(i = 20;i <= 24;i++) /* Direita */
		for(j = 55;j <= 61;j++)
			mapa[i][j] = 1;
	for(i = 21;i <= 23;i++)
		for(j = 56;j <= 60;j++)
			mapa[i][j] = 0;
	for(i = 20;i <= 24;i++)
		for(j = 69;j <= 75;j++)
			mapa[i][j] = 1;
	for(i = 21;i <= 23;i++)
		for(j = 70;j <= 74;j++)
			mapa[i][j] = 0;
	for(i = 20;i <= 24;i++) /* Esquerda */
		for(j = 4;j <= 10;j++)
			mapa[i][j] = 1;
	for(i = 21;i <= 23;i++)
		for(j = 5;j <= 9;j++)
			mapa[i][j] = 0;
	for(i = 20;i <= 24;i++)
		for(j = 18;j <= 25;j++)
			mapa[i][j] = 1;
	for(i = 21;i <= 23;i++)
		for(j = 19;j <= 24;j++)
			mapa[i][j] = 0;
	/* Paredes do mapa */
	/* Traços horizontais em cima e em baixo da caixa central */
	for(j = 33;j <= 47;j++)
		mapa[20][j] = 1;
	for(j = 33;j <= 47;j++)
		mapa[8][j] = 1;
	/* Traços horizontais em cima e em baixo */
	for(j = 44;j <= 51;j++) /* Em cima */
		mapa[4][j] = 1;
	for(j = 36;j >= 29;j--)
		mapa[4][j] = 1;
	for(j = 44;j <= 51;j++) /* Em baixo */
		mapa[24][j] = 1;
	for(j = 36;j >= 29;j--)
		mapa[24][j] = 1;
	/* Traços verticais em cima e em baixo */
	for(i = 1;i <= 4;i++) /* Em cima */
		mapa[i][40] = 1;
	for(i = 1;i <= 4;i++)
		mapa[i][65] = 1;
	for(i = 1;i <= 4;i++)
		mapa[i][14] = 1;
	for(i = 28;i >= 24;i--) /* Em baixo */
		mapa[i][40] = 1;
	for(i = 28;i >= 24;i--)
		mapa[i][65] = 1;
	for(i = 28;i >= 24;i--)
		mapa[i][14] = 1;
	/* Traços verticais no meio */
	for(i = 12;i >= 8;i--)
		mapa[i][51] = 1;
	for(i = 12;i >= 8;i--)
		mapa[i][29] = 1;
	for(i = 20;i >= 16;i--)
		mapa[i][51] = 1;
	for(i = 20;i >= 16;i--)
		mapa[i][29] = 1;
	/* Espaço vazio onde o Pacman iniciará */
	for(i = 9;i <= 11;i++)
		for(j = 39;j <= 41;j++)
			mapa[i][j] = 0;
	/* Pastilhas especiais */
	for(i = 25;i <= 26;i++)
		for(j = 6;j <= 7;j++)
			mapa[i][j] = -1;
	for(i = 25;i <= 26;i++)
		for(j = 71;j <= 72;j++)
			mapa[i][j] = -1;
}

/* Funções para printar na tela */

void printa_mapa(int mapa[LIN][COL]){
	int i, j;
	move(1, 0);
	for(i = 0;i < LIN;i++){
		for(j = 0;j < COL;j++){
			switch(mapa[i][j]){ /* Para cada valor no mapa é printado um caractere diferente com uma cor diferente */
				case -1:
					attron(COLOR_PAIR(7));
					printw("*");
					break;
				case 0:
					printw(" "); /* Exceto espaços vazios e paredes, que por padrão são pretos e azuis */
					break;
				case 1:
					printw("#");
					break;
				case 2:
					attron(COLOR_PAIR(2));
					printw(".");
					break;
				case 3:
					attron(COLOR_PAIR(2));
					printw("<");
					break;
				case 4:
					attron(COLOR_PAIR(2));
					printw(">");
					break;	
				case 5:
					attron(COLOR_PAIR(2));
					printw("^");
					break;
				case 6:
					attron(COLOR_PAIR(2));
					printw("v");
					break;
				case 7:
					attron(COLOR_PAIR(3));
					printw("M");
					break;
				case 8:
					attron(COLOR_PAIR(5));
					printw("W");
					break;
				case 9:
					attron(COLOR_PAIR(2));
					printw("A");
					break;
				case 10:
					attron(COLOR_PAIR(4));
					printw("U");
					break;
			}
			attroff(COLOR_PAIR(2));
			attroff(COLOR_PAIR(3));
			attroff(COLOR_PAIR(4));
			attroff(COLOR_PAIR(5));
			attroff(COLOR_PAIR(7));
		}
		printw("\n");
	}
}

void printa_pontuacao(int pontos){
	move(0,0);
	attron(COLOR_PAIR(6));
	printw("PONTUCAO: %d", pontos);
	attroff(COLOR_PAIR(6));
}

void printa_nivel(int nivel){
	move(0,20);
	attron(COLOR_PAIR(6));
	printw("NIVEL: %d", nivel);
	attroff(COLOR_PAIR(6));
}

/* Funções de funcionamento geral */

void atualiza_pacman(struct pacman *player, int mapa[LIN][COL]){
	int i, j;
	for(i = 0;i < 3;i++){
		for(j = 0;j < 3;j++){ /* O pacman é colocado no mapa nas coordenadas que foram passadas */
			mapa[player->lin+i][player->col+j] = player->corpo[i][j];
		}
	}
}

void atualiza_fantasmas(struct fantasma *blinky, struct fantasma *pinky, struct fantasma *inky, struct fantasma *clyde, int mapa[LIN][COL]){
	int i, j;
	for(i = 0;i < 3;i++){
		for(j = 0;j < 3;j++){ 
			mapa[blinky->lin+i][blinky->col+j] = blinky->corpo[i][j]; /* Os fantasmas são colocados no mapa de acordo com suas coordenadas */
		}
	}
	for(i = 0;i < 3;i++){
		for(j = 0;j < 3;j++){ 
			mapa[pinky->lin+i][pinky->col+j] = pinky->corpo[i][j];
		}
	}	
	for(i = 0;i < 3;i++){
		for(j = 0;j < 3;j++){ 
			mapa[inky->lin+i][inky->col+j] = inky->corpo[i][j];
		}
	}
	for(i = 0;i < 3;i++){
		for(j = 0;j < 3;j++){ 
			mapa[clyde->lin+i][clyde->col+j] = clyde->corpo[i][j];
		}
	}
}

void modifica_corpo(struct pacman *player, int tipo){
	int i, j;
	switch(tipo){
		case 1:
			for(i = 0;i < 3;i++){
				for(j = 0;j < 3;j++){
					player->corpo[i][j] = 3;
				}
			}
			player->corpo[1][2] = 0;
			break;
		case 2:
			for(i = 0;i < 3;i++){
				for(j = 0;j < 3;j++){
					player->corpo[i][j] = 4;
				}
			}
			player->corpo[1][0] = 0;
			break;
		case 3:
			for(i = 0;i < 3;i++){
				for(j = 0;j < 3;j++){
					player->corpo[i][j] = 5;
				}
			}
			player->corpo[2][1] = 0;
			break;
		case 4:
			for(i = 0;i < 3;i++){
				for(j = 0;j < 3;j++){
					player->corpo[i][j] = 6;
				}
			}
			player->corpo[0][1] = 0;
			break;
	}
	
}

void game_over(int mapa[LIN][COL], int pontuacao, int nivel){
	char key;
	key = getch();
	while(key != 'q'){
		printa_mapa(mapa);
		printa_pontuacao(pontuacao);
		printa_nivel(nivel);
		move(0, 35);
		attron(COLOR_PAIR(6));
		printw("GAME OVER!!!");
		move(0, 47);
		printw("Pressione 'q' para sair...");
		refresh();
		usleep(100000);
		clear();
		key = getch();
		attroff(COLOR_PAIR(6));
	}
	endwin();
	exit(0);
}

void game_won(int mapa[LIN][COL], int pontuacao, int nivel){
	char key;
	key = getch();
	while(key != 'q'){
		printa_mapa(mapa);
		printa_pontuacao(pontuacao);
		printa_nivel(nivel);
		move(0, 35);
		attron(COLOR_PAIR(6));
		printw("VOCE GANHOU!!!");
		move(0, 55);
		printw("Pressione 'q' para sair...");
		refresh();
		usleep(100000);
		clear();
		key = getch();
		attroff(COLOR_PAIR(6));
	}
	endwin();
	exit(0);
}

int verifica_ganhador(int mapa[LIN][COL]){
	int i, j;
	for(i = 0;i <= LIN;i++)
		for(j = 0;j <= COL;j++)
			if(mapa[i][j] == 2)
				return 0;
	return 1;
}

void level_up(){
	clear();
	printw("/\\ \"-.\\ \\   /\\  ___\\   /\\_\\_\\_\\   /\\__  _\\    /\\ \\       /\\  ___\\   /\\ \\ / /  /\\  ___\\   /\\ \\       \n");
	printw("\\ \\ \\-.  \\  \\ \\  __\\   \\/_/\\_\\/_  \\/_/\\ \\/    \\ \\ \\____  \\ \\  __\\   \\ \\ \' /   \\ \\  __\\   \\ \\ \\____  \n");
	printw(" \\  \\_\\\"\\_\\  \\ \\_____\\   /\\_\\/\\_\\    \\ \\_\\     \\ \\_____\\  \\ \\_____\\  \\ \\__|    \\ \\_____\\  \\ \\_____\n");
	printw("  \\/_/ \\/_/   \\/_____/   \\/_/\\/_/     \\/_/      \\/_____/   \\/_____/   \\/_/      \\/_____/   \\/_____/ \n");
	refresh();
	sleep(4);
}

void animacao_inicial(void){
	printw("8b,dPPYba,  ,adPPYYba,  ,adPPYba, 88,dPYba,,adPYba,  ,adPPYYba, 8b,dPPYba,  \n"); refresh(); usleep(150000);
	printw("88P'    '8a ''     `Y8 a8'     '' 88P'  '88'    '8a ''     `Y8 88P'   `'8a  \n"); refresh(); usleep(150000);
	printw("88       d8 ,adPPPPP88 8b         88     88      88 ,adPPPPP88 88       88  \n"); refresh(); usleep(150000);
	printw("88b,   ,a8 '88,   , 88  8a,   ,aa 88     88      88 88,    ,88 88       88  \n"); refresh(); usleep(150000);
	printw("88`YbbdP'  `8bbdP   88  `Ybbd8a   88     88      88 `8bbdPY8   88       88  \n"); refresh(); usleep(150000);
	printw("88                                                                          \n"); refresh(); usleep(150000);
	printw("88                                                                          \n"); refresh(); usleep(150000);
	usleep(2000000);
	clear();
}

void regras(){
	char key;
	key = getch();
	while(key != ' '){
		mvprintw(0, 65, "Bem vindo ao jogo Pac-Man!!!");
		mvprintw(2, 30, "Como jogar:");
		mvprintw(4, 30, ">Utilize as setas do teclado para mover o Pac-Man");
		mvprintw(5, 30, ">Seu objetivo e comer todas as pastilhas espalhadas no mapa");
		mvprintw(6, 30, ">Pastilhas especiais sao maiores e te dao pontos extras!");
		mvprintw(7, 30, ">Quando comer todas as pastilhas, sobe de nivel");
		mvprintw(8, 30, ">A cada nivel que subir, o Pac-Man fica mais rapido");
		mvprintw(9, 30, ">Voce nao pode encostar nos fantasmas, caso voce enconste neles ou eles em voce, voce perde");
		mvprintw(10, 30, ">Voce pode sair do jogo a qualquer momento apertando a tecla 'F1'");
		mvprintw(11, 30, ">Aperte barra de espaco para continuar...");
		key = getch();
	}
}

int verifica_terminal(void){
	int nlin, ncol;
	getmaxyx(stdscr, nlin, ncol);
	if(nlin < 30 || ncol < 80) /* Terminal deve ser pelo menos 38x100 para que o jogo funcione */
		return 0;
	return 1;
}

void animacao_inicial(void){
	printw(/* $$$$$$$$\ $$\   $$\ $$$$$$$\         $$$$$$\   $$$$$$\  $$\      $$\ $$$$$$$$\  */\n"); refresh(); usleep(150000);
	printw(/* $$  _____|$$$\  $$ |$$  __$$\       $$  __$$\ $$  __$$\ $$$\    $$$ |$$  _____| */\n"); refresh(); usleep(150000);
	printw(/* $$ |      $$$$\ $$ |$$ |  $$ |      $$ /  \__|$$ /  $$ |$$$$\  $$$$ |$$ |       */\n"); refresh(); usleep(150000);
	printw(/* $$ |      $$$$\ $$ |$$ |  $$ |      $$ /  \__|$$ /  $$ |$$$$\  $$$$ |$$ |       */\n"); refresh(); usleep(150000);
	printw(/* $$  __|   $$ \$$$$ |$$ |  $$ |      $$ |\_$$ |$$  __$$ |$$ \$$$  $$ |$$  __|    */\n"); refresh(); usleep(150000);
	printw(/* $$ |      $$ |\$$$ |$$ |  $$ |      $$ |  $$ |$$ |  $$ |$$ |\$  /$$ |$$ |       */\n"); refresh(); usleep(150000);
	printw(/* $$ |      $$ |\$$$ |$$ |  $$ |      $$ |  $$ |$$ |  $$ |$$ |\$  /$$ |$$ |       */\n"); refresh(); usleep(150000);
    printw(/* $$$$$$$$\ $$ | \$$ |$$$$$$$  |      \$$$$$$  |$$ |  $$ |$$ | \_/ $$ |$$$$$$$$\  */\n"); refresh(); usleep(150000);
    printw(/* \________|\__|  \__|\_______/        \______/ \__|  \__|\__|     \__|\________| */\n"); refresh(); usleep(150000);
	usleep(2000000);
	clear();








