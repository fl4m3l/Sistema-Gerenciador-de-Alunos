#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "arvore.h"

typedef struct no no;

struct aluno
{
	int matricula;
	char nome[21];
	char email[21];
	char telefone[11];
};

struct arvore
{
	int maior_no;
	no *raiz;
};

struct no
{
	int maior_no;
	aluno *info;
	no *pai;
	no *esq;
	no *dir;
};

arvore *
criar()
{
   arvore *a = (arvore *)malloc(sizeof(arvore));
   a->raiz = NULL;
   a->maior_no = 0;
   return a;
}

void
destruir(arvore *a)
{
	free(a);
}

void
pre_ordem(no *raiz)
{
	if (raiz != NULL)
	{
		printf("\n\nMatricula: %d ", raiz->info->matricula);
		printf("\nNome: %s ", raiz->info->nome);
		printf("\nEmail: %s ", raiz->info->email);
		printf("\nTelefone: %s ", raiz->info->telefone);
		pre_ordem(raiz->esq);
		pre_ordem(raiz->dir);
	}
}

void
imprimir(arvore *arv)
{
	pre_ordem(arv->raiz);
	printf("\n");
}

void
inserir(arvore *arv, aluno *aluno)
{
	no *ant = NULL, *raiz = arv->raiz;
	while (raiz != NULL && raiz->info->matricula != aluno->matricula)
	{
		ant = raiz;
		raiz = aluno->matricula < raiz->info->matricula ? raiz->esq : raiz->dir; 
	}
	if (raiz == NULL)
	{
		raiz = (no *)malloc(sizeof(no));
		raiz->info =  aluno;
		raiz->esq = NULL;
		raiz->dir = NULL;
		if (ant != NULL)
		{
			if (aluno->matricula < ant->info->matricula)
				ant->esq = raiz;
			else
				ant->dir = raiz;
		}
		else
			arv->raiz = raiz;
	}	 
}

void
inserir_novo_aluno(arvore *arv, char nome[], char email[], char telefone[])
{
	aluno *alun = (aluno *)malloc(sizeof(aluno));
	alun->matricula = arv->maior_no + 1;
	arv->maior_no = alun->matricula;
	strcpy(alun->nome, nome);
	strcpy(alun->email, email);
	strcpy(alun->telefone, telefone);
	inserir(arv, alun);
}

void buscar(arvore *arv, int matricula)
{
	no* raiz = arv->raiz;
	while (raiz != NULL && raiz->info->matricula != matricula) {
		raiz = matricula < raiz->info->matricula ? raiz->esq : raiz->dir; 
	}
	if (raiz != NULL)
	{
		printf("\n\nMatricula: %d", raiz->info->matricula);
		printf("\nNome: %s", raiz->info->nome);
		printf("\nEmail: %s", raiz->info->email);
		printf("\nTelefone: %s", raiz->info->telefone);
	}
	else
		printf("\n\nMatricula: %d nao encontrada", matricula);
}

void
ajustar_ant(arvore *arv, no *ant, int v, no* no)
{
	if (ant != NULL)
	{
		if (v < ant->info->matricula)
			ant->esq = no;
		else
			ant->dir = no;
	} 
	else
		arv->raiz = no;
}

int
remover_menor(no **praiz)
{
	no *ant = NULL, *raiz = *praiz;
	int me;
	while (raiz->esq != NULL)
	{
		ant = raiz;
		raiz = raiz->esq; 
	}
	if (ant != NULL)
		ant->esq = raiz->dir;
	else
		*praiz = raiz->dir;
	me = raiz->info->matricula;
	free(raiz);
	return me;
}

	void
	remover(arvore *arv, int matricula)
	{
		no* ant = NULL, *raiz = arv->raiz;
		while (raiz != NULL && raiz->info->matricula != matricula)
		{
			ant = raiz;
			raiz = matricula < raiz->info->matricula ? raiz->esq : raiz->dir; 
		}
		if (raiz != NULL)
		{
			if (raiz->esq == NULL && raiz->dir == NULL)		//Folha
			{
				ajustar_ant(arv, ant, matricula, NULL);
				free(raiz);
			}
			else if (raiz->esq != NULL && raiz->dir != NULL)	//Dois filhos
				raiz->info->matricula = remover_menor(&raiz->dir);
			else
			{	//Um filho
				if (raiz->esq != NULL)
						ajustar_ant(arv, ant, matricula, raiz->esq);
				else
					ajustar_ant(arv, ant, matricula, raiz->dir);
				free(raiz);
			}
			if (matricula == arv->maior_no)
					arv->maior_no--;
		}
		else
			printf("\n\nMatricula: %d nao encontrada", matricula);
	}

void
carregar_pelo_arquivo(arvore *arv, char url[])
{
	FILE *arq;
	
	arq = fopen(url, "r");
	if(arq == NULL)
			printf("Erro, nao foi possivel abrir o arquivo\n");
	else
	{
		int matricula;
		char linha[80];
		char nome[21];
		char email[21];
		char telefone[11];
		while (fgets(linha, sizeof(linha) - 1, arq) != NULL)
		{
			aluno *alun = (aluno *)malloc(sizeof(aluno));
			sscanf(linha, "%d | %[^|] | %[^|] | %[^\0]", &matricula, nome, email, telefone);
			nome[strlen(nome) - 1] = '\0';
			email[strlen(email) - 1] = '\0';
			telefone[strlen(telefone) - 1] = '\0';
			alun->matricula = matricula;
			if (alun->matricula > arv->maior_no)
				arv->maior_no = alun->matricula;
			strcpy(alun->nome, nome);
			strcpy(alun->email, email);
			strcpy(alun->telefone, telefone);
			inserir(arv, alun);
		}		
	}
	fclose(arq);
}

void
imprimir_alunos_arquivo(arvore *arv, char url[])
{
	no *raiz = arv->raiz;
	FILE *arq;
	
	arq = fopen(url, "r");
	if(arq == NULL)
			printf("Erro, nao foi possivel abrir o arquivo\n");
	else
	{
		int matricula;
		char linha[9];
		while (fgets(linha, sizeof(linha), arq) != NULL)
		{
			sscanf(linha, "%d", &matricula);
			while (raiz != NULL && raiz->info->matricula != matricula)
				raiz = matricula < raiz->info->matricula ? raiz->esq : raiz->dir; 
			if (raiz != NULL)
			{
				printf("\n\nMatricula: %d", raiz->info->matricula);
				printf("\nNome: %s", raiz->info->nome);
				printf("\nEmail: %s", raiz->info->email);
				printf("\nTelefone: %s", raiz->info->telefone);
			}
			else
				printf("\n\nMatricula: %d nao encontrada", matricula);
			raiz = arv->raiz;
		}		
	}
	fclose(arq);
}

void
alterar_dados(arvore *arv, int matricula)
{
	no* raiz = arv->raiz;
	while (raiz != NULL && raiz->info->matricula != matricula) {
		raiz = matricula < raiz->info->matricula ? raiz->esq : raiz->dir; 
	}
	if (raiz != NULL)
	{
		char nome[21];
		char email[21];
		char telefone[21];
		printf("\n\nNome: ");	strcpy(raiz->info->nome, gets(nome));
		printf("Email: ");	strcpy(raiz->info->email, gets(email));
		printf("Telefone: ");	strcpy(raiz->info->telefone, gets(telefone));
		
	}
	else
		printf("\n\nMatricula: %d nao encontrada", matricula);
}

void
remover_pelo_arquivo(arvore *arv, char url[])
{
	no *raiz = arv->raiz;
	FILE *arq;
	
	arq = fopen(url, "r");
	if(arq == NULL)
			printf("Erro, nao foi possivel abrir o arquivo\n");
	else
	{
		int matricula;
		char linha[9];
		while (fgets(linha, sizeof(linha), arq) != NULL)
		{
			sscanf(linha, "%d", &matricula);
			while (raiz != NULL && raiz->info->matricula != matricula)
				raiz = matricula < raiz->info->matricula ? raiz->esq : raiz->dir; 
			if (raiz != NULL)
				remover(arv, matricula);
			else
				printf("\n\nMatricula: %d nao encontrada", matricula);
			raiz = arv->raiz;
		}		
	}
	fclose(arq);
}
