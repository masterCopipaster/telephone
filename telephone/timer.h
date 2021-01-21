/*
 * timer.h
 *
 * Created: 17.07.2019 0:53:57
 *  Author: andro
 */ 

#define F_CPU 1000000

#ifndef TIMER_H_
#define TIMER_H_

#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/pgmspace.h>

#include "iomacro.h"


void timer_init();
void enable_timer_interrupt();
void disable_timer_interrupt();
void tone_init();
void out_tone_off();
void out_tone_on(uint16_t hz);
void dyn_tone_on(uint16_t hz);
void dyn_tone_off();
uint8_t dyn_mel_is_playing();
uint8_t out_mel_is_playing();

typedef struct  
{
	uint16_t duration;
	uint16_t period;
} note;

void dyn_mel_start(const note* melody, uint8_t numnotes, uint8_t rep);
void dyn_mel_stop();
void out_mel_start(const note* melody, uint8_t numnotes, uint8_t rep);
void out_mel_stop();

#define TONE_PRESC 200
#define HZ_TO_BORD(HZ)	((F_CPU / (2 * TONE_PRESC)) / (HZ))
#define MS_TO_BORD(MS)	(((MS) * F_CPU / (2 * TONE_PRESC)) / 1000)

#define  NOTE_GET_RAM(dest, arr, ind) {dest = arr[ind];}
#define  NOTE_GET_PGM(dest, arr, ind) {cli(); memcpy_P(&dest, &(arr[ind]), sizeof(note)); sei();}
#ifdef MEL_PGM
	#define NOTE_GET NOTE_GET_PGM
#else
	#define NOTE_GET NOTE_GET_RAM
#endif

#endif /* TIMER_H_ */