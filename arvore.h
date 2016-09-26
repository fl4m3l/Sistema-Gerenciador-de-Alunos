#ifndef _ARVORE_H
#define _ARVORE_H

typedef struct arvore arvore;
typedef struct aluno aluno;

arvore	*criar();
void	destruir(arvore *);

void	inserir(arvore *, aluno *);
void	inserir_novo_aluno(arvore *, char[], char[], char[]);
void	remover(arvore *, int);
void	carregar_pelo_arquivo(arvore *, char[]);
void	alterar_dados(arvore *, int);
void	remover_pelo_arquivo(arvore *, char[]);
void	remover_todos(arvore *);
void	salvar_em_arquivo(arvore *, char[]);

void	imprimir(arvore *);
void	buscar(arvore *, int);
void	imprimir_alunos_arquivo(arvore *, char[]);

#endif /*_ARVORE_H*/
