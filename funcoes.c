#include <stdio.h>
#include <stdlib.h>
#include "funcoes.h"
#include <time.h>
#include <pthread.h>

#define MIN_REAL -2.0
#define MAX_REAL 1.0
#define MIN_IMAG -1.5
#define MAX_IMAG 1.5

int validaArgumento(int argc,char const **argv, int vet[], int n){
    int valor;
    char *endptr;

    for(int i=0; i<n; i++){
        valor = strtol(argv[i+1], &endptr, 10); 

        
        if(valor > 0 && *endptr == '\0') vet[i] = valor;
        
        else{
            fprintf(stderr, "Erro: Argumento invalido, use apenas numeros\n");
            return 1;
        } 
    }

    return 0;
}

int calculaInteracoes(int coluna, int linha, int largura, int altura, int max_interacoes){
    double a_c = MIN_REAL + ((double) coluna / (largura - 1)) * (MAX_REAL - MIN_REAL);
    double b_c = MIN_IMAG + ((double) linha / (altura - 1)) * (MAX_IMAG - MIN_IMAG);

    double a = 0.0, b = 0.0, A, B;
    int interacao = 0;

    while((a*a + b*b <= 4) && (interacao < max_interacoes)){
        A = a*a - b*b + a_c;
        B = 2 * a * b + b_c;
        a = A;
        b = B;
        interacao += 1;
    }

    return interacao;
}

void rodaSerial(int largura, int altura, int max_interacoes, int *bufferCru, unsigned char *buffer, FILE *time){
    struct timespec antes, depois;
    double tempoDeExecucao;
    int interacoes, intensidade;

    clock_gettime(CLOCK_MONOTONIC, &antes);

    for(int linha = 0; linha < altura; linha++){
        for(int coluna = 0; coluna < largura; coluna++){

            interacoes = calculaInteracoes(coluna, linha, largura, altura, max_interacoes);
            bufferCru[linha * largura + coluna] = interacoes;
            intensidade = ((double) interacoes / max_interacoes) * 255;
            buffer[linha * largura + coluna] = intensidade;
        }
    }

    clock_gettime(CLOCK_MONOTONIC, &depois);
    tempoDeExecucao = (depois.tv_sec - antes.tv_sec) + (depois.tv_nsec - antes.tv_nsec) / 1000000000.0;

    fprintf(time, "Tempo serial: %f \n", tempoDeExecucao);

    FILE *arq = fopen("mandelbrot_lcw_serial.pgm", "w");

    if(arq == NULL){
        fprintf(stderr, "Erro: Nao foi possivel abrir o arquivo\n");
        exit(1);
    }


    for(int linha = 0; linha < altura; linha++){
        for(int coluna = 0; coluna < largura; coluna++){
            fprintf(arq, "%d ", buffer[linha * largura + coluna]);
        }
        fprintf(arq, "\n");
    }
}

void rodaOpenMP(int largura, int altura, int max_interacoes, int *bufferCru, unsigned char *buffer, int num_threads, FILE *time){
    struct timespec antes, depois;
    double tempoDeExecucao;
    int interacoes, intensidade;

    clock_gettime(CLOCK_MONOTONIC, &antes);

    #pragma omp parallel for num_threads(num_threads)

    for(int linha = 0; linha < altura; linha++){
        for(int coluna = 0; coluna < largura; coluna++){

            interacoes = calculaInteracoes(coluna, linha, largura, altura, max_interacoes);
            bufferCru[linha * largura + coluna] = interacoes;
            intensidade = ((double) interacoes / max_interacoes) * 255;
            buffer[linha * largura + coluna] = intensidade;
        }
    }

    clock_gettime(CLOCK_MONOTONIC, &depois);
    tempoDeExecucao = (depois.tv_sec - antes.tv_sec) + (depois.tv_nsec - antes.tv_nsec) / 1000000000.0;

    fprintf(time, "Tempo OpenMP: %f \n", tempoDeExecucao);

    FILE *arq = fopen("mandelbrot_lcw_openmp.pgm", "w");

    if(arq == NULL){
        fprintf(stderr, "Erro: Nao foi possivel abrir o arquivo\n");
        exit(1);
    }


    for(int linha = 0; linha < altura; linha++){
        for(int coluna = 0; coluna < largura; coluna++){
            fprintf(arq, "%d ", buffer[linha * largura + coluna]);
        }
        fprintf(arq, "\n");
    }
}

void *tarefa(void *argc){
    dadosThreads1 dados = *(dadosThreads1*) argc;
    int interacoes, intensidade;
    
    for(int linha = dados.linhaInicial; linha <= dados.linhaFinal; linha++){
        for(int coluna = 0; coluna<dados.largura; coluna++){

            interacoes = calculaInteracoes(coluna, linha, dados.largura, dados.altura, dados.maxInteracao);
            dados.bufferCru[linha * dados.largura + coluna] = interacoes;
            intensidade = ((double) interacoes / dados.maxInteracao) * 255;
            dados.buffer[linha * dados.largura + coluna] = intensidade;
        }
    }
    return NULL;
}

void rodaPthreads1(int largura, int altura, int max_interacoes, int *bufferCru, unsigned char *buffer, int num_threads, FILE *time){
    struct timespec antes, depois;
    double tempoDeExecucao;
    pthread_t thread[num_threads];
    dadosThreads1 vetThread[num_threads];

    int linhaPorThreads = altura / num_threads;

    clock_gettime(CLOCK_MONOTONIC, &antes);

    for(int i=0; i<num_threads; i++){

        vetThread[i].linhaInicial = linhaPorThreads * i;
        vetThread[i].linhaFinal = linhaPorThreads * (i + 1) - 1;
        vetThread[i].altura = altura;
        vetThread[i].largura = largura;
        vetThread[i].buffer = buffer;
        vetThread[i].bufferCru = bufferCru;
        vetThread[i].maxInteracao = max_interacoes;
        
        pthread_create(&thread[i], NULL, tarefa, &vetThread[i]);
    }

    for(int i=0; i<num_threads; i++){
        pthread_join(thread[i], NULL);
    }

    clock_gettime(CLOCK_MONOTONIC, &depois);
    tempoDeExecucao = (depois.tv_sec - antes.tv_sec) + (depois.tv_nsec - antes.tv_nsec) / 1000000000.0;

    fprintf(time, "Tempo Pthreads1: %f \n", tempoDeExecucao);
    FILE *arq = fopen("mandelbrot_lcw_pthreads1.pgm", "w");

    if(arq == NULL){
        fprintf(stderr, "Erro: Nao foi possivel abrir o arquivo\n");
        exit(1);
    }

    for(int linha = 0; linha < altura; linha++){
        for(int coluna = 0; coluna < largura; coluna++){
            fprintf(arq, "%d ", buffer[linha * largura + coluna]);
        }
        fprintf(arq, "\n");
    }

    fclose(arq);
}

void *tarefa2(void *argc){
    dadosThreads2 dados = *(dadosThreads2*) argc;
    int interacoes, intensidade;
    
    for(int linha = dados.linhaInicial; linha < dados.altura; linha += dados.numThread){
        for(int coluna = 0; coluna<dados.largura; coluna++){

            interacoes = calculaInteracoes(coluna, linha, dados.largura, dados.altura, dados.maxInteracao);
            dados.bufferCru[linha * dados.largura + coluna] = interacoes;
            intensidade = ((double) interacoes / dados.maxInteracao) * 255;
            dados.buffer[linha * dados.largura + coluna] = intensidade;
        }
    }
    return NULL;
}

void rodaPthreads2(int largura, int altura, int max_interacoes, int *bufferCru, unsigned char *buffer, int num_threads, FILE *time){
    struct timespec antes, depois;
    double tempoDeExecucao;
    pthread_t thread[num_threads];
    dadosThreads2 vetThread[num_threads];

    clock_gettime(CLOCK_MONOTONIC, &antes);

    for(int i=0; i<num_threads; i++){

        vetThread[i].linhaInicial = i;
        vetThread[i].numThread = num_threads;
        vetThread[i].altura = altura;
        vetThread[i].largura = largura;
        vetThread[i].buffer = buffer;
        vetThread[i].bufferCru = bufferCru;
        vetThread[i].maxInteracao = max_interacoes;
        
        pthread_create(&thread[i], NULL, tarefa2, &vetThread[i]);
    }

    for(int i=0; i<num_threads; i++){
        pthread_join(thread[i], NULL);
    }

    clock_gettime(CLOCK_MONOTONIC, &depois);
    tempoDeExecucao = (depois.tv_sec - antes.tv_sec) + (depois.tv_nsec - antes.tv_nsec) / 1000000000.0;

    fprintf(time, "Tempo Pthreads2: %f \n", tempoDeExecucao);
    FILE *arq = fopen("mandelbrot_lcw_pthreads2.pgm", "w");

    if(arq == NULL){
        fprintf(stderr, "Erro: Nao foi possivel abrir o arquivo\n");
        exit(1);
    }

    for(int linha = 0; linha < altura; linha++){
        for(int coluna = 0; coluna < largura; coluna++){
            fprintf(arq, "%d ", buffer[linha * largura + coluna]);
        }
        fprintf(arq, "\n");
    }

    fclose(arq);
}