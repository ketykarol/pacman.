struct nodo{
	int lin, col;
	char corpo[3][3];
	int id; /* Cada fastama terá um id, cada id formará um fantasma com caracteres diferentes */
	struct nodo *prox, *prev;
};

struct lista{
	struct nodo *ini, *fim, *atual;
	int tamanho;
};

void cria_lista(struct lista *l);
int lista_vazia(struct lista *l);
int insere_inicio(struct lista *l, int lin, int col, int id);
int remove_item(struct lista *l, int id);
void inicializa_fantasma(struct nodo *nodo, int id);