/*
 * main.c
 *
 *  Copyright 2013 flycode.pl
 *  Created on: 2013-02-10
 *  Author: Paul Peter Flis <pawel@flycode.pl>
 */

#include <avr/io.h>
#include <avr/eeprom.h>
#include <avr/interrupt.h>
#include <util/atomic.h>
#include <util/delay.h>
#include "keys.h"

#define LED_PORT DDRA
#define LED1 (1<<PA0)
#define LED2 (1<<PA1)
#define LED3 (1<<PA2)
#define LED4 (1<<PA3)
#define LED5 (1<<PA4)
#define LED6 (1<<PA5)

#define KEY1 (1<<PA7)
#define KEY2 (1<<PB1)

#define LEDS_ON  PORTA = (PORTA &= ~0x3F) | leds[state]
#define LEDS_OFF PORTA = (PORTA &= ~0x3F) | leds[0]

#define MAX_STATE 6

int leds[7] = {0x0, 0x1, 0x3, 0x7, 0xF, 0x1F, 0x3F};
unsigned char state = 0;
int time = 0;
unsigned char leds_on = 0;
unsigned char saved = 1;

/* attiny44 prescaler (page 84)
   CS02 CS01 CS00
   0    0    0    stop
   0    0    1    1 no prescaling
   0    1    0    8
   0    1    1    64
   1    0    0    256
   1    0    1    1024
*/

void init() {
  /* Initializacja przyciskow */
  // minus
  DDRA  &= ~KEY1;
  PORTA |= KEY1;

  // plus
  DDRB  &= ~KEY2;
  PORTB |= KEY2;

  /* Porty diod jako wyjscie */
  LED_PORT |= LED1|LED2|LED3|LED4|LED5|LED6;

  // PWN
  TCCR1A |= (1<<COM1A1)|(1<<WGM11);
  TCCR1B |= (1<<CS00)|(1<<CS01)|(1<<WGM12)|(1<<WGM13);
  ICR1 = 256;

  DDRA |= (1<<PA6);
  power_off();
};

void self_test(void) {
  int test_state[7] = {0x0, 0x1, 0x2, 0x4, 0x8, 0x10, 0x20};
  for(int i=0; i<=MAX_STATE; i++) {
    PORTA = (PORTA &= ~0x3F) | test_state[i];
    _delay_ms(100);
  };
  for(int i=MAX_STATE; i>=0; i--) {
    PORTA = (PORTA &= ~0x3F) | test_state[i];
    _delay_ms(100);
  };
  PORTA |= 0x3F;
  _delay_ms(500);
  PORTA = (PORTA &= ~0x3F);
  _delay_ms(500);
};

void save(unsigned char value) {
  eeprom_busy_wait();
  eeprom_write_byte(sizeof(value), value);
};

unsigned char read(void) {
  eeprom_busy_wait();
  return eeprom_read_byte(sizeof(unsigned char));
};

void restore_state(void) {
  unsigned char init_state = read();
  if (init_state < 0 || init_state > 6) {
    state = 1;
  } else {
    state = init_state;
  };
  set_power();
};

void power_on(void) {
  TCCR1A |= (1<<COM1A1);
  PORTA |= (1<<PA6);
};

void power_off(void) {
  TCCR1A &= ~(1<<COM1A1);
  PORTA &= ~(1<<PA6);
};

void plus(void) {
  if(state < MAX_STATE) {
    state++;
    set_power();
  };
  if(state == 1) power_on();
  full_power_leds();
};

void minus(void) {
  if(state > 0) {
    state--;
    set_power();
  };
  if(state == 0) power_off();
  full_power_leds();
};

void full_power_leds(void) {
  LEDS_ON;
  time = 30000;
  leds_on = 1;
};

void render_leds(void) {
  if(time == 0 && leds_on == 0) {
    LEDS_ON;
    time = 10;
    leds_on = 1;
  } else if(time == 0 && leds_on == 1) {
    if(saved == 0) {
      save(state);
      saved=1;
    };
    LEDS_OFF;
    time = 90;
    leds_on = 0;
  };
  time--;
};

void set_power(void) {
  saved = 0;
  if(state == 0)      power_off();
  else if(state >= 1)  power_on();
  OCR1A=state*43;
};

int main(void) {
  init();
  self_test();
  restore_state();
  while(1) {
    key_press(&key1_lock, &PINA, KEY1, minus);
    key_press(&key2_lock, &PINB, KEY2, plus);
    render_leds();
  };
};
