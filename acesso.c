// GRR20190372 Jorge Lucas Vicilli Jabczenski

#include <stdio.h>
#include <stdlib.h>
#include "acesso.h"

void ler_cabecalho(FILE *ENTRADA, audio_t *audio)
{
    fread(&audio->RIFF, sizeof(RIFF_t), 1, ENTRADA);
    fread(&audio->fmt, sizeof(fmt_t), 1, ENTRADA);
    fread(&audio->data, sizeof(data_t), 1, ENTRADA);
}

void ler_dados(FILE *ENTRADA, audio_t *audio)
{
    /*  Calcula a quantidade de samples do áudio */
    audio->tamanho = audio->data.SubChunk2Size / (audio->fmt.BitsPerSample / 8);

    audio->dados = malloc(audio->data.SubChunk2Size);
    if (!audio->dados)
    {
        perror("MALLOC FAILED");
        exit(1);
    }

    fread(audio->dados, sizeof(int16_t), audio->tamanho, ENTRADA);
}

audio_t *ler_audio(FILE *ENTRADA)
{
    audio_t *audio;

    audio = malloc(sizeof(audio_t));
    if (!audio)
    {
        perror("MALLOC FAILED");
        exit(1);
    }

    ler_cabecalho(ENTRADA, audio);
    ler_dados(ENTRADA, audio);

    return audio;
}

void enviar_audio(FILE *SAIDA, audio_t *audio)
{
    fwrite(&audio->RIFF, sizeof(RIFF_t), 1, SAIDA);
    fwrite(&audio->fmt, sizeof(fmt_t), 1, SAIDA);
    fwrite(&audio->data, sizeof(data_t), 1, SAIDA);
    fwrite(audio->dados, sizeof(int16_t), audio->tamanho, SAIDA);
}

void fechar_streams(FILE *ENTRADA, FILE *SAIDA)
{
    //if (ENTRADA != stdin)
    fclose(ENTRADA);
    //if (SAIDA != stdout)
    fclose(SAIDA);
}

void liberar_audio(audio_t *audio)
{
    free(audio->dados);
    free(audio);
}