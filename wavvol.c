// GRR20190372 Jorge Lucas Vicilli Jabczenski

#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>
#include "wav.h"
#include "funwav.h"

void tratar_argumentos(int argc, char **argv, FILE *ENTRADA, FILE *SAIDA, float *level);

int main(int argc, char **argv)
{
    FILE *ENTRADA = stdin, *SAIDA = stdout;
    audio_t *audio;
    float level = 1.0;

    tratar_argumentos(argc, argv, ENTRADA, SAIDA, &level);

    audio = ler_audio(ENTRADA);

    alterar_volume(audio, level);

    enviar_audio(SAIDA, audio);

    fechar_streams(ENTRADA, SAIDA);
    liberar_audio(audio);

    return 0;
}

/* Tratar os argumentos da linha de comando */
void tratar_argumentos(int argc, char **argv, FILE *ENTRADA, FILE *SAIDA, float *level)
{
    int opt;
    while ((opt = getopt(argc, argv, "i:o:l:")) != -1)
    {
        switch (opt)
        {
        case 'i':
            // Alterar ENTRADA
            ENTRADA = freopen(optarg, "r", ENTRADA);
            if (!ENTRADA)
            {
                fprintf(stderr, "Não foi possível encontrar o arquivo de entrada\n");
                exit(1);
            }
            break;

        case 'o':
            // Alterar SAIDA
            SAIDA = freopen(optarg, "w", SAIDA);
            if (!SAIDA)
            {
                fprintf(stderr, "Não foi possível encontrar o arquivo de saída\n");
                exit(1);
            }
            break;

        case 'l':
            // Alterar volume
            *level = atof(optarg);
            if (*level < 0.0 || *level > 10.0)
            {
                fprintf(stderr, "Level fora dos limites (0.0 < l < 10.0)\n");
                exit(1);
            }
            break;

        default:
            fprintf(stderr, "Usage: ./wavvol -i [FILE] -o [FILE] -l LEVEL\n");
            exit(1);
        }
    }
}
