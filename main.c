#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "funcoes.h"

int main(int argc, char const *argv[]){
    int n = 4;
    int valorConvertido[n], valor;

    if(argc != 5){
        printf("USO: ./mandelbrot [largura] [altura] [max_iteracoes] [num_threads]\n");
        return 1;
    } 
    
    if(validaArgumento(argc, argv, valorConvertido, n) != 0){
        return 1;
    }

    int largura = valorConvertido[0];
    int altura = valorConvertido[1];
    int max_interacoes = valorConvertido[2];
    int num_threads = valorConvertido[3];

    int numDePosicoes = largura * altura;
    int bytes = sizeof(unsigned char);
    int qtdDeBytes = numDePosicoes * bytes;
    unsigned char *buffer;
    buffer = malloc(qtdDeBytes);

    if(buffer == NULL){
        printf("Erro: Memoria nao alocada\n");
    }


    int interacoes, intensidade;
    int *bufferCru = malloc(sizeof(int) * numDePosicoes);

    if(bufferCru == NULL){
        printf("Erro: Memoria nao alocada\n");
    }

    for(int linha = 0; linha < altura; linha++){
        for(int coluna = 0; coluna < largura; coluna++){
            interacoes = calculaInteracoes(coluna, linha, largura, altura, max_interacoes);
            bufferCru[linha * largura + coluna] = interacoes;
            intensidade = ((double) interacoes / max_interacoes) * 255;
            buffer[linha * largura + coluna] = intensidade;
        }
    }

    FILE *arq = fopen("mandelbrot_lcw_serial.pgm", "w");

    if(arq == NULL){
        fprintf(stderr, "Erro: Nao foi possivel abrir o arquivo\n");
        return 1;
    }

    for(int linha = 0; linha < altura; linha++){
        for(int coluna = 0; coluna < largura; coluna++){
            fprintf(arq, "%d ", buffer[linha * largura + coluna]);
        }
        fprintf(arq, "\n");
    }

    fclose(arq);

    return 0;
}
