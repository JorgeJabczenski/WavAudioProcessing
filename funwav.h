// GRR20190372 Jorge Lucas Vicilli Jabczenski

#ifndef __FUNWAV__
#define __FUNWAV__

#include "estruturas.h"

/* Código das operações usadas */
#define SOMA 1
#define SUBT 2
#define MULT 3

/* Realiza uma operação respeitando um valor máximo abs(limite)  */
int16_t op_com_limite(int op, float a, float b, int16_t limite);

/* Altera os samples de um audio para sample[i]*level  */
void alterar_volume(audio_t *audio, float level);

/* Aplica uma função em todos os arquivos que vierem por 
argumento da linha de comando e retorna o resultado final*/
audio_t *tratar_varios_arquivos(int argc, char **argv, void(func)(audio_t *, audio_t *));

/* Libera os espaços alocados pelo audio */
void liberar_audio(audio_t *audio);

#endif 