#include "timer.h"

void timer_init()
{
	TCCR0A = 1 << WGM01; //CTC mode
	TCCR0B = 1 << CS00;  //no prescaling
	OCR0A = TONE_PRESC - 1;
	DDRB |= 1 << 0;
}

void set_tone_presc(uint8_t presc)
{
	OCR0A = presc;
}

void enable_timer_interrupt()
{
	sei();
	TIMSK |= 1 << OCIE0A;
}

void disable_timer_interrupt()
{
	TIMSK &= ~(1 << OCIE0A);
}

uint16_t out_cnt, dyn_cnt;
uint16_t out_border, dyn_border;
uint8_t out_on;
uint8_t dyn_on;


void dyn_tone_on(uint16_t hz)
{
	//DYN_ON;
	enable_timer_interrupt();
	dyn_on = 1;
	dyn_border = HZ_TO_BORD(hz);
}

void dyn_tone_off()
{
	dyn_on = 0;
	//DYN_ON;
	if(!(dyn_on || out_on)) disable_timer_interrupt();
}

void out_tone_off()
{
	out_on = 0;
	//OUT_MUTE;
	if(!(dyn_on || out_on)) disable_timer_interrupt();
}

void out_tone_on(uint16_t hz)
{
	//OUT_ON;
	enable_timer_interrupt();
	out_on = 1;
	out_border = HZ_TO_BORD(hz);
}

void tone_init()
{
	dyn_cnt = 0;
	dyn_on = 0;
	out_cnt = 0;
	out_on = 0;
}

note* out_mel;
note* dyn_mel;
note out_cur_note;
note dyn_cur_note;
uint16_t out_dur_cnt, dyn_dur_cnt;
uint8_t dyn_note_ind, dyn_note_maxind, out_note_ind, out_note_maxind, dyn_rep, out_rep;

inline void dyn_note_switch()
{
	dyn_dur_cnt++;
	if(dyn_mel && dyn_dur_cnt >= dyn_cur_note.duration)
	{
		dyn_dur_cnt = 0;
		dyn_note_ind = dyn_note_ind < dyn_note_maxind ? dyn_note_ind + 1 : 0;
		if(! dyn_rep && ! dyn_note_ind) 
			dyn_mel = 0;
		NOTE_GET(dyn_cur_note, dyn_mel, dyn_note_ind);
		dyn_border = dyn_cur_note.period;
		PIN_SET_0(DYN_PIN);
	}
}


void dyn_mel_start(const note* melody, uint8_t numnotes, uint8_t rep)
{
	dyn_on = 1;
	//DYN_ON;
	dyn_mel = melody;
	dyn_note_maxind = numnotes - 1;
	dyn_rep = rep;
	dyn_note_ind = 0;
	enable_timer_interrupt();
}

void dyn_mel_stop()
{
	dyn_mel = 0;
	dyn_tone_off();
}

inline void out_note_switch()
{
	out_dur_cnt++;
	if(out_mel && out_dur_cnt >= out_cur_note.duration)
	{
		out_dur_cnt = 0;
		out_note_ind = out_note_ind < out_note_maxind ? out_note_ind + 1 : 0;
		if(!out_rep && !out_note_ind) 
			out_mel = 0;
		NOTE_GET(out_cur_note, out_mel, out_note_ind);
		out_border = out_cur_note.period;
		PIN_SET_0(OUT_PIN);
	}
}


void out_mel_start(const note* melody, uint8_t numnotes, uint8_t rep)
{
	out_on = 1;
	OUT_ON;
	out_mel = melody;
	out_note_maxind = numnotes;
	out_rep = rep;
	out_note_ind = 0;
	enable_timer_interrupt();
}

void out_mel_stop()
{
	out_mel = 0;
	out_tone_off();
}

uint8_t dyn_mel_is_playing()
{
	return dyn_mel != 0;
}
uint8_t out_mel_is_playing()
{
	return out_mel != 0;
}


ISR(TIM0_COMPA_vect)
{
	out_cnt++;
	dyn_cnt++;
	dyn_note_switch();
	out_note_switch();
	if(out_cnt >= out_border && out_on)
	{
		out_cnt = 0;
		if(PIN_IS_0(OUT_PIN)) PIN_SET_1(OUT_PIN)
		else PIN_SET_0(OUT_PIN);
	}
	if(dyn_cnt >= dyn_border &&  dyn_on)
	{
		dyn_cnt = 0;
		if(PIN_IS_0(DYN_PIN)) PIN_SET_1(DYN_PIN)
		else PIN_SET_0(DYN_PIN);
	}
}