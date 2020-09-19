// GRR20190372 Jorge Lucas Vicilli Jabczenski

#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>
#include "wav.h"
#include "funwav.h"

void tratar_argumentos(int argc, char **argv, FILE *ENTRADA, FILE *SAIDA, float *level, int *delay);
void aplicar_eco(audio_t *audio, float level, int delay);

int main(int argc, char **argv)
{
    FILE *ENTRADA = stdin, *SAIDA = stdout;
    audio_t *audio;
    float level = 0.5;
    int delay = 1000;

    tratar_argumentos(argc, argv, ENTRADA, SAIDA, &level, &delay);

    audio = ler_audio(ENTRADA);

    aplicar_eco(audio, level, delay);

    enviar_audio(SAIDA, audio);

    fechar_streams(ENTRADA, SAIDA);
    liberar_audio(audio);

    return 0;
}

/* Função para tratar os argumentos da linha de comando */
void tratar_argumentos(int argc, char **argv, FILE *ENTRADA, FILE *SAIDA, float *level, int *delay)
{
    int opt;
    while ((opt = getopt(argc, argv, "i:o:l:t:")) != -1)
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
            if (*level < 0.0 || *level > 1.0)
            {
                fprintf(stderr, "Level fora dos limites (0.0 < l < 1.0)\n");
                exit(1);
            }
            break;
        case 't':
            // Alterar delay
            *delay = atof(optarg);
            if (*delay < 0)
            {
                fprintf(stderr, "O valor do delay deve ser maior que 0");
                exit(1);
            }
            break;

        default:
            fprintf(stderr, "Usage: ./wavecho -i [FILE] -o [FILE] -l LEVEL -t DELAY \n");
            exit(1);
        }
    }
}

/* Aplica o efeito de eco nos samples */
void aplicar_eco(audio_t *audio, float level, int delay)
{
    int nrChannels = audio->fmt.NrChannels;         /* Nr de Canais */
    int sampleRate = audio->fmt.SampleRate;         /* Sample Rate */
    int samplesPerSecond = sampleRate * nrChannels; /* Samples de audio por segundo levando em conta o número de canais*/

    /* Calcula o início dos ecos */
    int inicio = (delay * samplesPerSecond) / 1000;

    /* Aplica apenas os ecos no próprio canal */
    for (int i = 0; i < nrChannels; i++)
    {
        for (int j = inicio + i; j < audio->tamanho; j += nrChannels)
        {
            int16_t amostraAnterior = op_com_limite(MULT, audio->dados[j - inicio], level, VOLMAX);
            audio->dados[j] = op_com_limite(SOMA, audio->dados[j], amostraAnterior, VOLMAX);
            
            /* O método acima é exatamente igual ao de baixo porém evita o overflow */
            // audio->dados[j] += audio->dados[j - inicio] * level;               
        }
    }
}