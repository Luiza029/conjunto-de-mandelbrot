# Conjunto de Mandelbrot

Programa em C que gera o conjunto de Mandelbrot em quatro implementações: Serial, OpenMP e duas versões com Pthreads (estratégias distintas de paralelização).

## Compilação

```bash
make
```

Para limpar os arquivos compilados:

```bash
make clean
```

## Execução

```bash
./mandelbrot [largura] [altura] [max_iteracoes] [num_threads]
```

Exemplo:

```bash
./mandelbrot 800 600 1000 4
```
