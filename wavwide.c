// GRR20190372 Jorge Lucas Vicilli Jabczenski

#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>
#include "acesso.h"
#include "funwav.h"

void tratar_argumentos(int argc, char **argv, FILE *ENTRADA, FILE *SAIDA, float *k);
int conferir_estereo(audio_t *audio);
void estereo_amplificado(audio_t *audio, float k);

int main(int argc, char **argv)
{
    FILE *ENTRADA = stdin, *SAIDA = stdout;
    audio_t *audio;
    float k = 1.0;

    tratar_argumentos(argc, argv, ENTRADA, SAIDA, &k);
    audio = ler_audio(ENTRADA);

    estereo_amplificado(audio, k);

    enviar_audio(SAIDA, audio);
    fechar_streams(ENTRADA, SAIDA);

    liberar_audio(audio);

    return 0;
}

/* Função para tratar os argumentos da linha de comando */
void tratar_argumentos(int argc, char **argv, FILE *ENTRADA, FILE *SAIDA, float *k)
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
            *k = atof(optarg);
            if (*k < 0.0 || *k > 10.0)
            {
                fprintf(stderr, "K fora dos limites (0.0 < k < 10.0)\n");
                exit(1);
            }
            break;

        default:
            fprintf(stderr, "Usage: ./wavwide -i [FILE] -o [FILE] -l k \n");
            exit(1);
        }
    }
}

/* Confere se o áudio é estereo */
int conferir_estereo(audio_t *audio)
{
    if (audio->fmt.NrChannels == 2)
        return 1;
    return 0;
}

/* Aplica um efeito de estéreo amplificado, que dá uma sensação de ambientes abertos */
void estereo_amplificado(audio_t *audio, float k)
{

    /* Verifica se é possivel aplicar o efeito no áudio */
    if (!conferir_estereo(audio))
    {
        fprintf(stderr, "O áudio deve ser estéreo (possuir 2 canais)");
        exit(1);
    }

    int16_t diff;

    /* Podemos incrementar de dois em dois pois assume-se que em um áudio estéreo o número de amostras */
    /* é sempre par */
    for (int i = 0; i < audio->tamanho; i += 2)
    {
        diff = op_com_limite(SUBT, audio->dados[i + 1], audio->dados[i], VOLMAX);         /* Diferença entre os canais */
        audio->dados[i + 1] = op_com_limite(SOMA, audio->dados[i + 1], k * diff, VOLMAX); /* Canal direito  */
        audio->dados[i] = op_com_limite(SUBT, audio->dados[i], k * diff, VOLMAX);         /* Canal esquerdo */

        /* O método acima é examante igual ao de baixo porém evita o clipping */
        // diff = audio->dados[i + 1] -  audio->dados[i];
        // audio->dados[i + 1] = audio->dados[i + 1] + (k * diff);
        // audio->dados[i]     = audio->dados[i] -  (k * diff);
    }
}