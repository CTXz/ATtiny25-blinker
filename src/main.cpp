/*

 * Copyright (C) 2019 Patrick Pedersen <ctx.xda@gmail.com>

 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.

 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.

  * Author: Patrick Pedersen <ctx.xda@gmail.com> 

 * Description:
 * A simple ATtiny25 based two-LED blinker light for bicycles, scooters and other types of vehicles   

 */

#include <avr/io.h>
#include <util/delay.h>

/* GPIO */

// LEDs
#define LED_L_MASK    (1 << PB3)
#define LED_R_MASK    (1 << PB4)
#define LED_L_R_MASK  LED_L_MASK | LED_R_MASK

// Mode Switch
#define MODE_SW_MASK  (1 << PB2)

/* Parameters */

#define DELAY_MS          500

/* Blinking Modes */

// get_mode
// ------------------------------------
// Fetches the state of the mode switch
// ------------------------------------
inline bool get_mode(uint8_t mode_sw_mask)
{
  return PINB & MODE_SW_MASK;
}

// set_mode
//----------------------------------------------------------
// Prepares the LEDs for toggling
// When mode = true, the LEDs will toggle simultaneously
// When mode = false, the LEDs will switch with every toggle
// ---------------------------------------------------------
inline void set_mode(bool mode, uint8_t led_l_mask, uint8_t led_r_mask, uint8_t led_l_r_mask)
{
  if (mode) {
    PORTB &= ~led_l_r_mask;
  } else {
    PORTB |= led_l_mask;
    PORTB &= ~led_r_mask;
  }
}

/* MAIN */

int main(void)
{
  DDRB |= LED_L_R_MASK;
  DDRB &= ~MODE_SW_MASK;

  PORTB &= ~(LED_L_R_MASK);
  
  bool mode = get_mode(MODE_SW_MASK);
  set_mode(mode, LED_L_MASK, LED_R_MASK, LED_L_R_MASK);

  while(true) {

    PORTB ^= LED_L_R_MASK; // Toggle LEDs
    
    for (uint16_t ms_counter = 0; ms_counter < DELAY_MS; ms_counter += 10) {
      _delay_ms(10);

      // Check if the mode has changed every 10 seconds
      if (get_mode(LED_L_R_MASK) != mode) {
        mode = !mode;
        set_mode(mode, LED_L_MASK, LED_R_MASK, LED_L_R_MASK);
        break;
      }

    }
  }
}
