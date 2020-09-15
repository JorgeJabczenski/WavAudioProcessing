// GRR20190372 Jorge Lucas Vicilli Jabczenski

#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>
#include "acesso.h"
#include "funwav.h"

void tratar_argumentos(int argc, char **argv, FILE *SAIDA);
void mixar_audios(audio_t *a, audio_t *b);

int main(int argc, char **argv)
{
    FILE *ENTRADA = stdin, *SAIDA = stdout;
    audio_t *audio;

    tratar_argumentos(argc, argv, SAIDA);

    audio = tratar_varios_arquivos(argc, argv, mixar_audios);

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
            fprintf(stderr, "Usage: ./wavmix -o [FILE] [file1] [file2] ... [fileN] \n");
            exit(1);
        }
    }
}

/* Soma dois audios e retorna o resultado no audioA */
void mixar_audios(audio_t *a, audio_t *b)
{
    int maiorTamanho, menorTamanho;

    /* Compara o tamanho dos audios para as operações futuras */
    if (a->tamanho > b->tamanho)
    {
        maiorTamanho = a->tamanho;
        menorTamanho = b->tamanho;
    }
    else
    {
        maiorTamanho = b->tamanho;
        menorTamanho = a->tamanho;

        /* Aloca o espaço extra necessário no audio A caso necessário */
        /* pois o resultado será colocado nele                        */
        a->dados = realloc(a->dados, sizeof(int16_t) * maiorTamanho);
        if (!a->dados)
        {
            fprintf(stderr, "REALLOC FAILED");
            liberar_audio(a);
            liberar_audio(b);
            exit(1);
        }

        /* Preenche o restante do áudio agora que o espaço já está alocado */
        for (int i = menorTamanho; i < maiorTamanho; i++)
            a->dados[i] = b->dados[i];

        /* Arruma os parâmetros do cabeçalho */
        a->RIFF.ChunkSize = b->RIFF.ChunkSize;
        a->data.SubChunk2Size = b->data.SubChunk2Size;
        a->tamanho = maiorTamanho;
    }

    /* Mixa os audios */
    for (int i = 0; i < menorTamanho; i++)
        a->dados[i] = op_com_limite(SOMA, a->dados[i], b->dados[i], VOLMAX);
}