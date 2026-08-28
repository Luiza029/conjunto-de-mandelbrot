#include <stdio.h>
#include <stdlib.h>
#include "funcoes.h"

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