// GRR20190372 Jorge Lucas Vicilli Jabczenski

#ifndef __ACESSO__
#define __ACESSO__

#include <stdio.h>
#include "estruturas.h"

/* Lê o cabeçalho de um audio de uma stream ENTRADA */
void ler_cabecalho(FILE *ENTRADA, audio_t *audio);

/* Lê os dados de um audio de uma stream ENTRADA */
void ler_dados(FILE *ENTRADA, audio_t *audio);

/* Lê primeiro o cabeçalho e depois dos dados de um audio de uma stream ENTRADA */
audio_t *ler_audio(FILE *ENTRADA);

/* Envia o audio para a SAIDA */
void enviar_audio(FILE *SAIDA, audio_t *audio);

/* Fecha as streams de dados */
void fechar_streams(FILE *ENTRADA, FILE *SAIDA);

#endif