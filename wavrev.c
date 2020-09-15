// GRR20190372 Jorge Lucas Vicilli Jabczenski

#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>
#include "acesso.h"
#include "funwav.h"

void tratar_argumentos(int argc, char **argv, FILE *ENTRADA, FILE *SAIDA);
void trocar_samples(int16_t *sampleA, int16_t *sampleB);
void reverter_audio(audio_t *audio);

int main(int argc, char **argv)
{
    FILE *ENTRADA = stdin, *SAIDA = stdout;
    audio_t *audio;

    tratar_argumentos(argc, argv, ENTRADA, SAIDA);
    audio = ler_audio(ENTRADA);

    reverter_audio(audio);

    enviar_audio(SAIDA, audio);
    fechar_streams(ENTRADA, SAIDA);

    liberar_audio(audio);

    return 0;
}

void tratar_argumentos(int argc, char **argv, FILE *ENTRADA, FILE *SAIDA)
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

        default:
            fprintf(stderr, "Usage: ./wavrev -i [FILE] -o [FILE]\n");
            exit(1);
        }
    }
}

void trocar_samples(int16_t *sampleA, int16_t *sampleB)
{
    int16_t aux;

    aux = *sampleA;
    *sampleA = *sampleB;
    *sampleB = aux;
}

void reverter_audio(audio_t *audio)
{
    // switch (audio->fmt.NrChannels)
    // {
    // case 1:
    //     for (int i = 0; i < audio->tamanho / 2; i++)
    //     {
    //         trocar_samples(&audio->dados[i], &audio->dados[audio->tamanho - 1 - i]);
    //     }
    //     break;
    // case 2:
    //     for (int i = 0; i < audio->tamanho / 2; i += 2)
    //     {
    //         trocar_samples(&audio->dados[i], &audio->dados[audio->tamanho - 1 - (i + 1)]); /* Canal esquerdo */
    //         trocar_samples(&audio->dados[i + 1], &audio->dados[audio->tamanho - 1 - (i)]); /* Canal direito  */
    //     }
    //     break;
    // default:
    //     perror("Use apenas áudios com 1 ou 2 canais\n");
    //     exit(1);
    // }

    int nrChannels = audio->fmt.NrChannels;     /*Nr de canais*/
    int tamanho = audio->tamanho;               /*Tamanho do audio*/
    int iteracoes = (tamanho / nrChannels) / 2; /*Quantidade de trocas por canal*/

    for (int i = 0; i < nrChannels; i++)
        for (int j = 0; j < iteracoes; j++)
        {
            /* Calcula os indexes certos para funcionar com qualquer número de canais */
            int index1 = i + (j * nrChannels);
            int index2 = (tamanho - (nrChannels - i)) - (j * nrChannels);

            trocar_samples(&audio->dados[index1], &audio->dados[index2]);
        }
}

