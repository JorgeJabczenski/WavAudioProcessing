// GRR20190372 Jorge Lucas Vicilli Jabczenski

#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>
#include "wav.h"
#include "funwav.h"

void tratar_argumentos(int argc, char **argv, FILE *ENTRADA, FILE *SAIDA);
void normalizar_volume(audio_t *audio);

int main(int argc, char **argv)
{
    FILE *ENTRADA = stdin, *SAIDA = stdout;
    audio_t *audio;

    tratar_argumentos(argc, argv, ENTRADA, SAIDA);

    audio = ler_audio(ENTRADA);

    normalizar_volume(audio);

    enviar_audio(SAIDA, audio);

    fechar_streams(ENTRADA, SAIDA);
    liberar_audio(audio);

    return 0;
}

/* Tratar os argumentos da linha de comando */
void tratar_argumentos(int argc, char **argv, FILE *ENTRADA, FILE *SAIDA)
{
    int opt;
    while ((opt = getopt(argc, argv, "i:o:")) != -1)
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

        default:
            fprintf(stderr, "Usage: ./wavnorm -i [FILE] -o [FILE]\n");
            exit(1);
        }
    }
}

/* Aumenta o volume para seu máximo sem causar clipping */
void normalizar_volume(audio_t *audio)
{
    int16_t maior = abs(audio->dados[0]);
    float level;

    /* Encontra o maior valor entre os samples */
    for (int i = 1; i < audio->tamanho; i++)
        if (abs(audio->dados[i]) > maior)
            maior = abs(audio->dados[i]);

    level = (float)VOLMAX / (float)maior;
    alterar_volume(audio, level);
}