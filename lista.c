#include <stdio.h>
#include <stdlib.h>
#include "lista.h"

void cria_lista(struct lista *l){
	l->tamanho = 0;
	l->ini = NULL;
	l->fim = NULL;
}

int lista_vazia(struct lista *l){
	if(l->ini == NULL && l->fim == NULL)
		return 1;
	return 0;
}

int insere_inicio(struct lista *l, int lin, int col, int id){
	struct nodo *aux;
	aux = malloc(sizeof(struct nodo));
	if(aux == NULL)
		return 0;
	if(lista_vazia(l)){ /* No caso de lista vazia, a primeira inserção é especial */
		l->ini = aux; /* Inicio e fim apontam para o único nodo */
		l->fim = aux;
		l->tamanho++;
		aux->prox = NULL; /* Seus ponteiros apontam para null, pois é o único nodo */
		aux->prev = NULL;
		aux->lin = lin; /* Recebe as coordenadas */
		aux->col = col;
		aux->id = id;
		inicializa_fantasma(aux, id);
	}
	aux->prox = l->ini;
	l->ini->prev = aux;
	l->ini = aux;
	l->tamanho++;
	aux->prev = NULL;
	aux->lin = lin;
	aux->col = col;
	aux->id = id;
	inicializa_fantasma(aux, id);
	return 1;
}

int remove_item(struct lista *l, int id){
	if(lista_vazia(l))
		return 0;
	struct nodo *aux;
	aux = l->ini;
	while(aux != NULL){
		if(aux->id == id){
			if(aux->prox == NULL && aux->prev == NULL){ /* Esse nodo é o único no lista */
				l->ini = NULL;
				l->fim = NULL;
				l->tamanho = 0;
			}
			else if(aux->prev == NULL) /* Esse nodo é o primeiro da lista */
				if(aux->prox != NULL){
					aux->prox->prev = NULL;
					l->ini = aux->prox;
					aux->prox = NULL;
					l->tamanho--;
					free(aux);
				}
			else{
				if(aux->prox == NULL){ /* Esse nodo é o último da lista */
					aux->prev->prox = NULL;
					l->fim = aux->prev;
					l->tamanho--;
					free(aux);
				}
				else{ /* No meio da lista */
					aux->prev->prox = aux->prox;
					aux->prox->prev = aux->prev;
					l->tamanho--;
					free(aux);
				}
			}
		}
		aux = aux->prox;
	}
	return 1;
}


void inicializa_fantasma(struct nodo *nodo, int id){
	int i, j;
	switch(id){
		case 1: /* Para cada fantasma será atribuído um carcatere diferente */
			for(i = 0;i < 3;i++)
				for(j = 0;j < 3;j++)
					nodo->corpo[i][j] = 7;
			nodo->corpo[2][1] = 0;
			break;
		case 2:
			for(i = 0;i < 3;i++)
				for(j = 0;j < 3;j++)
					nodo->corpo[i][j] = 8;
			nodo->corpo[2][1] = 0;
			break;
		case 3:
			for(i = 0;i < 3;i++)
				for(j = 0;j < 3;j++)
					nodo->corpo[i][j] = 10;
			nodo->corpo[2][1] = 0;
			break;
		case 4:
			for(i = 0;i < 3;i++)
				for(j = 0;j < 3;j++)
					nodo->corpo[i][j] = 9;
			nodo->corpo[2][1] = 0;
			break;
	}
}
