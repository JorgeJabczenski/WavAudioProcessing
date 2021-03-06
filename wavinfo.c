// GRR20190372 Jorge Lucas Vicilli Jabczenski

#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>
#include "wav.h"
#include "funwav.h"

void tratar_argumentos(int argc, char **argv, FILE *ENTRADA);
void imprimir_cabecalho(audio_t *audio);

int main(int argc, char **argv)
{
    FILE *ENTRADA = stdin, *SAIDA = stdout;
    audio_t *audio;

    audio = malloc(sizeof(audio_t));
    if (!audio)
    {
        perror("MALLOC FAILED");
        exit(1);
    }

    tratar_argumentos(argc, argv, ENTRADA);

    ler_cabecalho(ENTRADA, audio);
    imprimir_cabecalho(audio);

    fechar_streams(ENTRADA, SAIDA);
    liberar_audio(audio);

    return 0;
}

/* Tratar os argumentos da linha de comando */
void tratar_argumentos(int argc, char **argv, FILE *ENTRADA)
{
    int opt;
    while ((opt = getopt(argc, argv, "i:")) != -1)
    {
        switch (opt)
        {
        case 'i':
            ENTRADA = freopen(optarg, "r", ENTRADA);
            if (!ENTRADA)
            {
                fprintf(stderr, "Não foi possível encontrar o arquivo\n");
                exit(1);
            }
            break;

        default:
            fprintf(stderr, "Usage: ./wavinfo -i [FILE]\n");
            exit(1);
        }
    }
}

/* Imprime os Chuncks RIFF, fmt e data */
void imprimir_cabecalho(audio_t *audio)
{
    printf("riff tag       (4 bytes): \"%.4s\"\n", audio->RIFF.ChunkID);
    printf("riff size      (4 bytes): %" PRIu32 "\n", audio->RIFF.ChunkSize);
    printf("wave tag       (4 bytes): \"%.4s\"\n", audio->RIFF.Format);

    printf("form tag       (4 bytes): \"%.4s\"\n", audio->fmt.SubChunk1ID);
    printf("fmt_size       (4 bytes): %" PRIu32 "\n", audio->fmt.SubChunk1Size);
    printf("audio_format   (2 bytes): %" PRIu16 "\n", audio->fmt.AudioFormat);
    printf("num_channels   (2 bytes): %" PRIu16 "\n", audio->fmt.NrChannels);
    printf("sample_rate    (4 bytes): %" PRIu32 "\n", audio->fmt.SampleRate);
    printf("byte_rate      (4 bytes): %" PRIu32 "\n", audio->fmt.ByteRate);
    printf("block_align    (2 bytes): %" PRIu16 "\n", audio->fmt.BlockAling);
    printf("bits_per_sample(2 bytes): %" PRIu16 "\n", audio->fmt.BitsPerSample);

    printf("data tag       (4 bytes): \"%.4s\"\n", audio->data.SubChunk2ID);
    printf("data size      (4 bytes): %" PRIu32 "\n", audio->data.SubChunk2Size);

    printf("bytes per sample        : %" PRIu32 "\n", audio->fmt.BitsPerSample / 8);
    printf("samples per channel     : %" PRIu32 "\n", (audio->data.SubChunk2Size / audio->fmt.NrChannels) / (audio->fmt.BitsPerSample / 8));
}