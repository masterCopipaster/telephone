/*
 * telephone.c
 *
 * Created: 13.07.2019 1:28:19
 * Author : andro
 */ 

#define F_CPU 1000000

#include <avr/io.h>
#include <util/delay.h>
#include <avr/pgmspace.h>

#define MEL_PGM

#include "iomacro.h"
#include "timer.h"
#include "pitches.h"
#include "melodies.h"


int main(void)
{
    /* Replace with your application code */
	DDRB = 0x0;
	PORTB = 0x0;
	timer_init();
	tone_init();
	
	//uint8_t i;
	//for(i = 0; i < 50; i++)
	//{
		//PIN_SET_1(DYN_PIN);
		//_delay_ms(1);
		//PIN_SET_0(DYN_PIN);
		//_delay_ms(1);
	//}
	dyn_mel_start(supermario, 24, 0);
	while(dyn_mel_is_playing()){}
	dyn_mel_stop();
	
	PIN_SET_IN(DYN_PIN);
	PIN_SET_IN(OUT_PIN);
	PIN_SET_IN_PU(BTN1_PIN);
	PIN_SET_IN_PU(BTN2_PIN);
	
    while (1) 
    {
		if(PIN_IS_0(BTN1_PIN))
		{
			PIN_SET_1(RELAY_PIN);
			dyn_tone_off();
			DYN_MUTE;
			if(PIN_IS_0(BTN2_PIN))
			{
				if(!out_mel_is_playing()) out_mel_start(piroshok, 10, 1);
			}
			else out_mel_stop();
		}
		else
		{
			PIN_SET_0(RELAY_PIN);
			out_tone_off();
			if(PIN_IS_0(BTN2_PIN))
			{
				if(!dyn_mel_is_playing()) dyn_mel_start(pampam, 3, 1);
			}
			else dyn_mel_stop();
		}
    }
}

