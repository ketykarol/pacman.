#define LIN 30
#define COL 80

struct pacman{
	int lin, col;
	int corpo[3][3];
};

struct fantasma{
	int lin, col;
	int corpo[3][3];
	int id;
};

void inicializa_ncurses(void);
void inicializa_mapa(int mapa[LIN][COL]);
void inicializa_pacman(struct pacman *player);
void inicializa_fantasmas(struct fantasma *blinky, struct fantasma *pinky, struct fantasma *inky, struct fantasma *clyde);
void printa_mapa(int mapa[LIN][COL]);
void printa_pontuacao(int pontos);
void printa_nivel(int nivel);
void atualiza_pacman(struct pacman *player, int mapa[LIN][COL]);
void atualiza_fantasmas(struct fantasma *blinky, struct fantasma *pinky, struct fantasma *inky, struct fantasma *clyde, int mapa[LIN][COL]);
int move_direita(struct pacman *player, int mapa[LIN][COL], int pontos);
int move_esquerda(struct pacman *player, int mapa[LIN][COL], int pontos);
int move_cima(struct pacman *player, int mapa[LIN][COL], int pontos);
int move_baixo(struct pacman *player, int mapa[LIN][COL], int pontos);
int move_fantasma(struct fantasma *fantasma, int mapa[LIN][COL], int comeco);
void move_fantasma_esquerda(struct fantasma *fantasma, int mapa[LIN][COL]);
void move_fantasma_direita(struct fantasma *fantasma, int mapa[LIN][COL]);
void move_fantasma_baixo(struct fantasma *fantasma, int mapa[LIN][COL]);
void move_fantasma_cima(struct fantasma *fantasma, int mapa[LIN][COL]);
int verifica_laterais_fantasma(struct fantasma *fantasma, int mapa[LIN][COL], int comeco);
void modifica_corpo(struct pacman *player, int tipo);
void game_over(int mapa[LIN][COL], int pontuacao, int nivel);
void game_won(int mapa[LIN][COL], int pontuacao, int nivel);
int verifica_ganhador(int mapa[LIN][COL]);
void level_up();
void animacao_inicial(void);
void regras();
int verifica_terminal(void);
void fim_jogo(void)



