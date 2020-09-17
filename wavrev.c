// GRR20190372 Jorge Lucas Vicilli Jabczenski

#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>
#include "wav.h"
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
            fprintf(stderr, "Usage: ./wavrev -i [FILE] -o [FILE]\n");
            exit(1);
        }
    }
}

/* Inverte o local de dois samples de audio */
void trocar_samples(int16_t *sampleA, int16_t *sampleB)
{
    int16_t aux;

    aux = *sampleA;
    *sampleA = *sampleB;
    *sampleB = aux;
}

/* Reverte todo o audio, levando em conta o número de canais */
void reverter_audio(audio_t *audio)
{
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

/* Os cálculos utilizados para achar os indices foram achados primeiramente utilizando um canal
 e depois montando tabelas verdades para 2 e 3 canais. Observando as tabelas foi possível encontrar
 uma relação matemática entre os indices corretos e o número de canais */