// GRR20190372 Jorge Lucas Vicilli Jabczenski

#include <stdio.h>
#include <stdlib.h>
#include "acesso.h"

/* Lê o cabeçalho de um audio de uma stream ENTRADA */
void ler_cabecalho(FILE *ENTRADA, audio_t *audio)
{
    /* Após cada leitura, testa o erro */
    int bytes_lidos;

    bytes_lidos = fread(&audio->RIFF, sizeof(RIFF_t), 1, ENTRADA);
    if (bytes_lidos == 0 && ferror(ENTRADA))
    {
        fprintf(stderr, "Erro ao realizar a leitura do RIFF");
        exit(1);
    }

    bytes_lidos = fread(&audio->fmt, sizeof(fmt_t), 1, ENTRADA);
    if (bytes_lidos == 0 && ferror(ENTRADA))
    {
        fprintf(stderr, "Erro ao realizar a leitura do fmt");
        exit(1);
    }

    bytes_lidos = fread(&audio->data, sizeof(data_t), 1, ENTRADA);
    if (bytes_lidos == 0 && ferror(ENTRADA))
    {
        fprintf(stderr, "Erro ao realizar a leitura do data");
        exit(1);
    }
}

/* Lê os dados de um audio de uma stream ENTRADA */
void ler_dados(FILE *ENTRADA, audio_t *audio)
{
    int bytes_lidos;

    /*  Calcula a quantidade de samples do áudio */
    audio->tamanho = audio->data.SubChunk2Size / (audio->fmt.BitsPerSample / 8);

    audio->dados = malloc(audio->data.SubChunk2Size);
    if (!audio->dados)
    {
        perror("MALLOC FAILED");
        exit(1);
    }

    bytes_lidos = fread(audio->dados, sizeof(int16_t), audio->tamanho, ENTRADA);
    if (bytes_lidos == 0 && ferror(ENTRADA))
    {
        fprintf(stderr, "Erro ao realizar a leitura do audio data");
        exit(1);
    }
}

/* Lê primeiro o cabeçalho e depois dos dados de um audio de uma stream ENTRADA */
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

/* Envia o audio para a SAIDA */
void enviar_audio(FILE *SAIDA, audio_t *audio)
{
    /* Após cada envio, verifica se ele foi bem sucedido */
    int bytes_enviados;

    bytes_enviados = fwrite(&audio->RIFF, sizeof(RIFF_t), 1, SAIDA);
    if (bytes_enviados == 0)
    {
        fprintf(stderr, "Erro ao realizar a escrita do RIFF");
        exit(1);
    }

    bytes_enviados = fwrite(&audio->fmt, sizeof(fmt_t), 1, SAIDA);
    if (bytes_enviados == 0)
    {
        fprintf(stderr, "Erro ao realizar a escrita do fmt");
        exit(1);
    }

    bytes_enviados = fwrite(&audio->data, sizeof(data_t), 1, SAIDA);
    if (bytes_enviados == 0)
    {
        fprintf(stderr, "Erro ao realizar a escrita do data");
        exit(1);
    }

    bytes_enviados = fwrite(audio->dados, sizeof(int16_t), audio->tamanho, SAIDA);
    if (bytes_enviados == 0)
    {
        fprintf(stderr, "Erro ao realizar a escrita do audio data");
        exit(1);
    }
}
