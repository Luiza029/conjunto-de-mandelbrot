#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int main(int argc, char const *argv[]){
    int n = 4;
    int valorConvertido[n], valor;
    char *endptr;

    if(argc != 5){
        printf("USO: ./mandelbrot [largura] [altura] [max_iteracoes] [num_threads]\n");
        return 1;
    } 

    for(int i=0; i<n; i++){
        valor = strtol(argv[i+1], &endptr, 10); 

        if(valor > 0 && *endptr == '\0') valorConvertido[i] = valor;

        else{
            fprintf(stderr, "Erro: Argumento invalido, use apenas numeros\n");
            return 1;
        } 
    }
    return 0;
}
