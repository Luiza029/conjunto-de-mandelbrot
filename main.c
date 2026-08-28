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

    return 0;
}
