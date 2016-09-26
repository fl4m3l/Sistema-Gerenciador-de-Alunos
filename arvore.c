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
	no *raiz;
};

struct no
{
	aluno *info;
	no *pai;
	no *esq;
	no *dir;
};

arvore *
criar()
{
	arvore *a;
	if ((a = (arvore *)malloc(sizeof(arvore))) == NULL)
	{
		printf("Erro, memoria insuficiente.\n");
		exit(1);
	}
	a->raiz = NULL;
	return a;
}

void
destruir(arvore *a)
{
	free(a);
}

void
in_ordem(no *raiz)
{
	if (raiz != NULL)
	{
		in_ordem(raiz->esq);
		printf("\n\nMatricula: %d ", raiz->info->matricula);
		printf("\nNome: %s ", raiz->info->nome);
		printf("\nEmail: %s ", raiz->info->email);
		printf("\nTelefone: %s ", raiz->info->telefone);
		in_ordem(raiz->dir);
	}
}

void
imprimir(arvore *arv)
{
	in_ordem(arv->raiz);
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
		if ((raiz = (no *)malloc(sizeof(no))) == NULL)
		{
			printf("Erro, memoria insuficiente.\n");
			exit(1);
		}
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

int
maior_no(no *raiz)
{
	if (raiz == NULL)
		return 0;
	if (raiz->dir != NULL)
		return maior_no(raiz->dir);
	return raiz->info->matricula;
}

void
inserir_novo_aluno(arvore *arv, char nome[], char email[], char telefone[])
{
	aluno *alun;
	if ((alun = (aluno *)malloc(sizeof(aluno))) == NULL)
	{
		printf("Erro, memoria insuficiente.\n");
		exit(1);
	}
	alun->matricula = maior_no(arv->raiz) + 1;
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
	}
	else
		printf("\n\nMatricula: %d nao encontrada", matricula);
}

void
carregar_pelo_arquivo(arvore *arv, char url[])
{
	FILE *arq;

	if((arq = fopen(url, "r")) == NULL)
	{
		printf("Erro, nao foi possivel abrir o arquivo\n");
		exit(1);
	}
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
	
	if((arq = fopen(url, "r")) == NULL)
	{
		printf("Erro, nao foi possivel abrir o arquivo\n");
		exit(1);
	}
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
		printf("\n\nNome: ");
		gets(nome);
		strcpy(raiz->info->nome, nome);
		printf("Email: ");
		gets(email);
		strcpy(raiz->info->email, email);
		printf("Telefone: ");
		gets(telefone);
		strcpy(raiz->info->telefone, telefone);
	}
	else
		printf("\n\nMatricula: %d nao encontrada", matricula);
}

void
remover_pelo_arquivo(arvore *arv, char url[])
{
	no *raiz = arv->raiz;
	FILE *arq;
	
	if((arq = fopen(url, "r")) == NULL)
	{
		printf("Erro, nao foi possivel abrir o arquivo\n");
		exit(1);
	}
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

void
remover_todos_rec(no *raiz, arvore *arv)
{ 
	if (raiz != NULL) {
		remover_todos_rec(raiz->esq, arv);
		remover_todos_rec(raiz->dir, arv);
		remover(arv, raiz->info->matricula);	
	}
}

void
remover_todos(arvore *arv)
{ 
	remover_todos_rec(arv->raiz, arv);
}

void
salva_aluno(no *raiz, FILE *arq)
{
	if (raiz != NULL)
	{
		fprintf(arq, "%d | ", raiz->info->matricula);
		fprintf(arq, "%s | ", raiz->info->nome);
		fprintf(arq, "%s | ", raiz->info->email);
		fprintf(arq, "%s\n", raiz->info->telefone);
		salva_aluno(raiz->esq, arq);
		salva_aluno(raiz->dir, arq);
	}
}

void
salvar_em_arquivo(arvore *arv, char url[])
{
	FILE *arq;
	no *raiz = arv->raiz;

	if ((arq = fopen(url, "w")) == NULL)
	{
		printf("Erro, nao foi possivel abrir o arquivo\n");
		exit(1);
	}
	salva_aluno(arv->raiz, arq);
	fclose(arq);
}
