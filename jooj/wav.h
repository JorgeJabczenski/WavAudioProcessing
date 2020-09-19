// GRR20190372 Jorge Lucas Vicilli Jabczenski

#ifndef __WAV__
#define __WAV__

/* Para poder usar os tipo int16_t e int32_t*/
#include <inttypes.h>
#include <stdio.h>

/**************************************************************/

/* Volume máximo (baseado no range de um inteiro de 16 bits) */
//#define VOLMAX INT16_MAX /* 100% */
#define VOLMAX 32113 /* 98%  */
//#define VOLMAX 31130 /* 95%  */
//#define VOLMAX 29492 /* 90%  */

/**************************************************************/

/* Chunk RIFF (16 bytes) */
struct RIFF_s
{
    char ChunkID[4];
    uint32_t ChunkSize;
    char Format[4];
};
typedef struct RIFF_s RIFF_t;

/* Chunk fmt (24 bytes) */
struct fmt_s
{
    char SubChunk1ID[4];
    uint32_t SubChunk1Size;
    uint16_t AudioFormat;
    uint16_t NrChannels;
    uint32_t SampleRate;
    uint32_t ByteRate;
    uint16_t BlockAling;
    uint16_t BitsPerSample;
};
typedef struct fmt_s fmt_t;

/* Chunk data [sem os samples de áudio](8 bytes) */
struct data_s
{
    char SubChunk2ID[4];
    uint32_t SubChunk2Size;
};
typedef struct data_s data_t;

/* Estrutura do Audio WAV */
struct audio_s
{
    RIFF_t RIFF;
    fmt_t fmt;
    data_t data;
    int16_t *dados;
    int tamanho;
};
typedef struct audio_s audio_t;

/**************************************************************/

/* Lê o cabeçalho de um audio de uma stream ENTRADA */
void ler_cabecalho(FILE *ENTRADA, audio_t *audio);

/* Lê os dados de um audio de uma stream ENTRADA */
void ler_dados(FILE *ENTRADA, audio_t *audio);

/* Lê primeiro o cabeçalho e depois dos dados de um audio de uma stream ENTRADA */
audio_t *ler_audio(FILE *ENTRADA);

/* Envia o audio para a SAIDA */
void enviar_audio(FILE *SAIDA, audio_t *audio);


#endif