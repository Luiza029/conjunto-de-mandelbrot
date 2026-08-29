#include <stdio.h>
#include <stdlib.h>
#include "funcoes.h"

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
}

int calculaInteracoes(int coluna, int linha, int largura, int altura, int max_interacoes){
    double a_c = MIN_REAL + (coluna / (largura - 1)) * (MAX_REAL - MIN_REAL);
    double b_c = MIN_IMAG + (linha / (altura - 1)) * (MAX_IMAG - MIN_IMAG);

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