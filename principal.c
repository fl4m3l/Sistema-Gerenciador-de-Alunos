#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <string.h>

#include "arvore.h"

/* run this program using the console pauser or add your own getch, system("pause") or input loop */

void
menu(arvore *arv)
{
	int op;

	printf 	       ("**************************************************************************************\n"
			"|1. Carregar os alunos para memoria a partir da base de dados.                       |\n"
			"**************************************************************************************\n"
			"|2. Inserir um novo aluno.                                                           |\n"
			"**************************************************************************************\n"
			"|3. Listar os dados de todos os alunos por ordem de matricula.                       |\n"
			"**************************************************************************************\n"
			"|4. Listar os dados de todos os alunos cujas matriculas estao contidas em um arquivo.|\n"
			"**************************************************************************************\n"
			"|5. Consultar dados do aluno a partir da matricula.                                  |\n"
			"**************************************************************************************\n"
			"|6. Alterar os dados de nome, email e telefone a partir da matricula.                |\n"
			"**************************************************************************************\n"
			"|7. Remover um aluno a partir da matricula.                                          |\n"
			"**************************************************************************************\n"
			"|8. Remover os dados de todos os alunos cujas matriculas estao contidas em um arquivo.\n"
			"**************************************************************************************\n"
			"|9. Remover todos os alunos.                                                         |\n"
			"**************************************************************************************\n"
			"|10. Salvar os alunos na base de dados.                                              |\n"
			"**************************************************************************************\n"
			"\nEscolha uma opcao: ");
	scanf("%d", &op);

	while (op < 1 || op > 10)
	{
		printf("\nValor incorreto. Digite novamente: ");
		scanf("%d", &op);
	}

	fflush(stdin);

	if (op == 1)
	{
		char base[30];
		printf("\nNome da base de dados: ");
		carregar_pelo_arquivo(arv, gets(base));
	}
	else if(op == 2)
	{
		char nome[30];
		char email[30];
		char telefone[11];
		printf("\nNome: ");		gets(nome);
		printf("Email: ");		gets(email);		
		printf("Telefone: ");	gets(telefone);
		inserir_novo_aluno(arv, nome, email, telefone);
	}
	else if(op == 3)
		imprimir(arv);
	else if(op == 4)
	{
		char arq[30];
		printf("\nNome do arquivo: ");		imprimir_alunos_arquivo(arv, gets(arq));
	}
	else if(op == 5)
	{
		int matricula;
		fflush(stdin);
		printf("\nMatricula: ");		scanf("%d", &matricula);
		buscar(arv, matricula);
	}
	else if(op == 6)
	{
		int matricula;
		fflush(stdin);
		printf("\nMatricula: ");	scanf("%d", &matricula);
		alterar_dados(arv, matricula);
	}
	else if(op == 7)
	{
		int matricula;
		fflush(stdin);
		printf("\nMatricula: ");	scanf("%d", &matricula);
		remover(arv, matricula);
	}
	else if(op == 8)
	{
		char arq[30];
		printf("\nNome do arquivo: ");		remover_pelo_arquivo(arv, gets(arq));
	}
	else if(op == 9)
		remover_todos(arv);
	else
	{
		char nome_arq[30];
		printf("Nome do arquivo: ");	gets(nome_arq);
		salvar_em_arquivo(arv, nome_arq);
	}
	printf("\n[1] - REPETIR ou [0] - SAIR: ");
	scanf("%d", &op);
	while (op < 0 || op > 1)
	{
		printf("\nValor incorreto. Digite novamente: ");
		scanf("%d", &op);
	}
	op == 1 ? system("cls"), menu(arv) : exit(1);
}


int main(int argc, char *argv[]) {
	arvore *arv = criar();
	menu(arv);
	destruir(arv);
	system("pause");
	return 0;
}
