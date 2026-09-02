#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "funcoes.h"
#include <time.h>
#include <omp.h>

int main(int argc, char const *argv[]){
    int n = 4;
    int valorConvertido[n], valor;

    if(argc != 5){
        fprintf(stderr ,"USO: ./mandelbrot [largura] [altura] [max_iteracoes] [num_threads]\n");
        return 1;
    } 
    
    if(validaArgumento(argc, argv, valorConvertido, n) != 0){
        return 1;
    }

    // Separação dos argumentos
    int largura = valorConvertido[0];
    int altura = valorConvertido[1];
    int max_interacoes = valorConvertido[2];
    int num_threads = valorConvertido[3];

    // Alocacao de Buffer
    int numDePosicoes = largura * altura;
    int bytes = sizeof(unsigned char);
    int qtdDeBytes = numDePosicoes * bytes;
    unsigned char *buffer;
    buffer = malloc(qtdDeBytes);

    if(buffer == NULL){
        fprintf(stderr, "Erro: Memoria nao alocada\n");
        return 1;
    }


    int interacoes, intensidade;
    int *bufferCru = malloc(sizeof(int) * numDePosicoes);

    if(bufferCru == NULL){
        fprintf(stderr, "Erro: Memoria nao alocada\n");
        return 1;
    }

    FILE *time = fopen("times.txt", "w");

    if(time == NULL){
        fprintf(time, "Erro ao abrir arquivo de time\n");
        return 1;
    }

    rodaSerial(largura, altura, max_interacoes, bufferCru, buffer, time);
    rodaOpenMP(largura, altura, max_interacoes, bufferCru, buffer, num_threads, time);
    rodaPthreads1(largura, altura, max_interacoes, bufferCru, buffer, num_threads, time);
    rodaPthreads2(largura, altura, max_interacoes, bufferCru, buffer, num_threads, time);

    fclose(time);
    free(buffer);
    free(bufferCru);
    return 0;
}
