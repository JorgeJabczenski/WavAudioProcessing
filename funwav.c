// GRR20190372 Jorge Lucas Vicilli Jabczenski

#include "funwav.h"
#include "acesso.h"
#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>

/* Realiza uma operação respeitando um valor máximo abs(limite)  */
int16_t op_com_limite(int op, float a, float b, int16_t limite)
{
    /* Os parâmetros são todos float por conta da 'promoção de variáveis' */

    switch (op)
    {
    case SOMA:
        if ((a + b) > limite)
            return (limite);
        if ((a + b) < -limite)
            return (-limite);
        return (a + b);
        break;

    case SUBT:
        if ((a - b) > limite)
            return (limite);
        if ((a - b) < -limite)
            return (-limite);
        return (a - b);
        break;

    case MULT:
        if ((a * b) > limite)
            return (limite);
        if ((a * b) < -limite)
            return (-limite);
        return (a * b);
        break;
    default:
        return 0;
        break;
    }
}

/* Altera os samples de um audio para sample[i]*level  */
void alterar_volume(audio_t *audio, float level)
{
    for (int i = 0; i < audio->tamanho; i++)
        audio->dados[i] = op_com_limite(MULT, audio->dados[i], level, VOLMAX);
}

/* Retorna 1 caso os arquivos de audio são compatíveis, 0 caso contrário */
static int conferir_compatibilidade(audio_t *a, audio_t *b)
{
    /* Começa assumindo que os audios são compatíveis */
    int compativeis = 1;

    if ((a->fmt.AudioFormat != b->fmt.AudioFormat) ||
        (a->fmt.NrChannels != b->fmt.NrChannels) ||
        (a->fmt.SampleRate != b->fmt.SampleRate) ||
        (a->fmt.ByteRate != b->fmt.ByteRate) ||
        (a->fmt.BlockAling != b->fmt.BlockAling) ||
        (a->fmt.BitsPerSample != b->fmt.BitsPerSample))
    {
        compativeis = 0;
    }

    return (compativeis);
}

/* Aplica uma função em todos os arquivos que vierem por 
argumento da linha de comando e retorna o resultado final*/
audio_t *tratar_varios_arquivos(int argc, char **argv, void(func)(audio_t *, audio_t *))
{
    FILE *ENTRADA;
    audio_t *audioA, *audioB;

    /* Confere se existem arquivos o suficiente para gerar uma concatenação */
    if ((argc - optind) < 2)
    {
        fprintf(stderr, "Numero insuficiente de arquivos\n");
        exit(1);
    }

    /* Como existem pelo menos 2 arquivos, pegar o primeiro deles 
    e armazenar em audioA */
    ENTRADA = fopen(argv[optind], "r");
    if (!ENTRADA)
    {
        fprintf(stderr, "Erro ao abrir arquivo de música\n");
        exit(1);
    }
    audioA = ler_audio(ENTRADA);

    /* Ler arquivo por arquivo e ir aplicando a função */
    for (int index = optind + 1; index < argc; index++)
    {
        ENTRADA = freopen(argv[index], "r", ENTRADA);
        if (!ENTRADA)
        {
            fprintf(stderr, "Erro ao abrir arquivo de música\n");
            exit(1);
        }
        audioB = ler_audio(ENTRADA);

        /* Conferir a compatibilidade antes de aplicar a função */
        if (!conferir_compatibilidade(audioA, audioB))
        {
            fprintf(stderr, "O arquivo [%s] não é compatível \n", argv[index]);
            fclose(ENTRADA);
            liberar_audio(audioA);
            liberar_audio(audioB);
            exit(1);
        }

        /* Aplica a função nos dois audios e retorna o resultado em audioA */
        func(audioA, audioB);

        /* Feito dentro do loop para evitar memory leaks            */
        /* já que a função "ler_audio" utiliza malloc e nao realloc */
        liberar_audio(audioB);
    }
    fclose(ENTRADA);
    return (audioA);
}

/* Libera os espaços alocados pelo audio */
void liberar_audio(audio_t *audio)
{
    free(audio->dados);
    free(audio);
}

/* Fecha as streams de dados */
void fechar_streams(FILE *ENTRADA, FILE *SAIDA)
{
    if (ENTRADA != stdin)
        fclose(ENTRADA);
    if (SAIDA != stdout)
        fclose(SAIDA);
}
