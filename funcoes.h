#ifndef FUNCOES_H
#define FUNCOES_H

typedef struct dadosThreads1{
    int linhaInicial;
    int linhaFinal;
    int largura;
    int altura;
    int maxInteracao;
    int *bufferCru;
    unsigned char *buffer;
}dadosThreads1;

int validaArgumento(int argc, char const *argv[], int vet[], int n);
int calculaInteracoes(int coluna, int linha, int largura, int altura, int max_interacoes);
void rodaSerial(int largura, int altura, int max_interacoes, int *bufferCru, unsigned char *buffer, FILE *time);
void rodaOpenMP(int largura, int altura, int max_interacoes, int *bufferCru, unsigned char *buffer, int num_threads, FILE *time);
void rodaPthreads1(int largura, int altura, int max_interacoes, int *bufferCru, unsigned char *buffer, int num_threads, FILE *time);
void rodaPthreads2();
void *tarefa(void *dados);


#endif
