// GRR20190372 Jorge Lucas Vicilli Jabczenski

#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>
#include "wav.h"
#include "funwav.h"

void tratar_argumentos(int argc, char **argv, FILE *SAIDA);
void concatenar_audios(audio_t *a, audio_t *b);

int main(int argc, char **argv)
{
    FILE *ENTRADA = stdin, *SAIDA = stdout;
    audio_t *audio;

    tratar_argumentos(argc, argv, SAIDA);

    audio = tratar_varios_arquivos(argc, argv, concatenar_audios);

    enviar_audio(SAIDA, audio);
    
    fechar_streams(ENTRADA, SAIDA);
    liberar_audio(audio);

    return 0;
}

/* Função para tratar os argumentos da linha de comando */
void tratar_argumentos(int argc, char **argv, FILE *SAIDA)
{
    int opt;
    while ((opt = getopt(argc, argv, "o:")) != -1)
    {
        switch (opt)
        {
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
            fprintf(stderr, "Usage: ./wavcat -o [FILE] [file1] [file2] ... [fileN] \n");
            exit(1);
        }
    }
}

/* Concatena dois audios e retorna o resultado no audio A */
void concatenar_audios(audio_t *a, audio_t *b)
{
    int novoTamanho;

    /* Calcula o tamanho do novo audio */
    novoTamanho = a->tamanho + b->tamanho;

    /* Aloca o espaço extra necessário no audioA */
    a->dados = realloc(a->dados, sizeof(int16_t) * novoTamanho);
    if (!a->dados)
    {
        fprintf(stderr, "REALLOC FAILED");
        liberar_audio(a);
        liberar_audio(b);
        exit(1);
    }

    /* Concatena os audios */
    for (int i = 0; i < b->tamanho; i++)
        a->dados[a->tamanho + i] = b->dados[i];

    /* Arruma os parâmetros do cabeçalho */
    a->RIFF.ChunkSize += b->data.SubChunk2Size;
    a->data.SubChunk2Size += b->data.SubChunk2Size;
    a->tamanho = novoTamanho;
}