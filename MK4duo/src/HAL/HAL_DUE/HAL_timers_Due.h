/**
 * MK4duo 3D Printer Firmware
 *
 * Based on Marlin, Sprinter and grbl
 * Copyright (C) 2011 Camiel Gubbels / Erik van der Zalm
 * Copyright (C) 2013 - 2016 Alberto Cotronei @MagoKimbra
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program. If not, see <http://www.gnu.org/licenses/>.
 *
 */

/**
 * This is the main Hardware Abstraction Layer (HAL).
 * To make the firmware work with different processors and toolchains,
 * all hardware related code should be packed into the hal files.
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program. If not, see <http://www.gnu.org/licenses/>.
 *
 *
 * Description: HAL for Arduino Due and compatible (SAM3X8E)
 *
 * Contributors:
 * Copyright (c) 2014 Bob Cousins bobcousins42@googlemail.com
 *                    Nico Tonnhofer wurstnase.reprap@gmail.com
 *
 * Copyright (c) 2015 - 2016 Alberto Cotronei @MagoKimbra
 *
 * ARDUINO_ARCH_SAM
 */

#ifndef _HAL_TIMERS_DUE_H
#define _HAL_TIMERS_DUE_H

// --------------------------------------------------------------------------
// Includes
// --------------------------------------------------------------------------

#include <stdint.h>

// --------------------------------------------------------------------------
// Types
// --------------------------------------------------------------------------

typedef uint32_t HAL_TIMER_TYPE;

typedef struct {
  Tc          *pTimerRegs;
  uint16_t    channel;
  IRQn_Type   IRQ_Id;
  uint8_t     priority;
} tTimerConfig;

// --------------------------------------------------------------------------
// Defines
// --------------------------------------------------------------------------

#define NUM_HARDWARE_TIMERS 9

#define STEPPER_TIMER 2
#define STEPPER_TIMER_PRESCALE 2.0
#define HAL_STEPPER_TIMER_RATE  ((F_CPU) / STEPPER_TIMER_PRESCALE)    // 42 MHz
#define STEPPER_TIMER_TICKS_PER_US (HAL_STEPPER_TIMER_RATE / 1000000) // 42

#define TEMP_TIMER 3
#define TEMP_TIMER_FREQUENCY 2000

#define BEEPER_TIMER 4
#define BEEPER_TIMER_COUNTER TC1
#define BEEPER_TIMER_CHANNEL 1

#define ENABLE_STEPPER_DRIVER_INTERRUPT()   HAL_timer_enable_interrupt (STEPPER_TIMER)
#define DISABLE_STEPPER_DRIVER_INTERRUPT()  HAL_timer_disable_interrupt (STEPPER_TIMER)

#define ENABLE_TEMP_INTERRUPT()             HAL_timer_enable_interrupt (TEMP_TIMER)
#define DISABLE_TEMP_INTERRUPT()            HAL_timer_disable_interrupt (TEMP_TIMER)

#define HAL_TIMER_SET_STEPPER_COUNT(count)  HAL_timer_set_count(STEPPER_TIMER, count);
#define HAL_TIMER_SET_TEMP_COUNT(count)     HAL_timer_set_count(TEMP_TIMER, count);

#define HAL_STEP_TIMER_ISR    void TC2_Handler()
#define HAL_TEMP_TIMER_ISR    void TC3_Handler()
#define HAL_BEEPER_TIMER_ISR  void TC4_Handler()

#define _ENABLE_ISRs() \
    do { \
      if (thermalManager.in_temp_isr) DISABLE_TEMP_INTERRUPT(); \
      else ENABLE_TEMP_INTERRUPT(); \
      ENABLE_STEPPER_DRIVER_INTERRUPT(); \
    } while(0)

#define CLI_ENABLE_TEMP_INTERRUPT() \
    in_temp_isr = false; \
    ENABLE_TEMP_INTERRUPT();

// Clock speed factor
#define CYCLES_PER_US ((F_CPU) / 1000000UL) // 84
// Stepper pulse duration, in cycles
#define STEP_PULSE_CYCLES ((MINIMUM_STEPPER_PULSE) * CYCLES_PER_US)

// --------------------------------------------------------------------------
// Public Variables
// --------------------------------------------------------------------------

// --------------------------------------------------------------------------
// Private Variables
// --------------------------------------------------------------------------

static constexpr tTimerConfig TimerConfig [NUM_HARDWARE_TIMERS] = {
  { TC0, 0, TC0_IRQn, 0 },  // 0 - [servo timer5]
  { TC0, 1, TC1_IRQn, 0 },  // 1
  { TC0, 2, TC2_IRQn, 2 },  // 2 - stepper
  { TC1, 0, TC3_IRQn, 15 }, // 3 - temperature
  { TC1, 1, TC4_IRQn, 0 },  // 4 - beeper
  { TC1, 2, TC5_IRQn, 0 },  // 5 - [servo timer3]
  { TC2, 0, TC6_IRQn, 0 },  // 6
  { TC2, 1, TC7_IRQn, 0 },  // 7
  { TC2, 2, TC8_IRQn, 0 },  // 8
};

// --------------------------------------------------------------------------
// Public functions
// --------------------------------------------------------------------------

void HAL_timer_start(const uint8_t timer_num, const uint32_t frequency);

static FORCE_INLINE void HAL_timer_set_count (uint8_t timer_num, uint32_t count) {
  const tTimerConfig *pConfig = &TimerConfig[timer_num];

  pConfig->pTimerRegs->TC_CHANNEL[pConfig->channel].TC_RC = count;
}

static FORCE_INLINE HAL_TIMER_TYPE HAL_timer_get_count (uint8_t timer_num) {
  const tTimerConfig *pConfig = &TimerConfig[timer_num];

  return pConfig->pTimerRegs->TC_CHANNEL[pConfig->channel].TC_RC;
}

static FORCE_INLINE uint32_t HAL_timer_get_current_count(uint8_t timer_num) {
  const tTimerConfig *pConfig = &TimerConfig[timer_num];

  return pConfig->pTimerRegs->TC_CHANNEL[pConfig->channel].TC_CV;
}

void HAL_timer_enable_interrupt(const uint8_t timer_num);
void HAL_timer_disable_interrupt(const uint8_t timer_num);

static FORCE_INLINE void HAL_timer_isr_prologue(uint8_t timer_num) {
  const tTimerConfig *pConfig = &TimerConfig[timer_num];

  // Reading the status register clears the interrupt flag
  pConfig->pTimerRegs->TC_CHANNEL[pConfig->channel].TC_SR;
}

// Tone
inline void HAL_timer_isr_status(Tc* tc, uint32_t channel) {
  tc->TC_CHANNEL[channel].TC_SR; // clear status register
}

void tone(uint8_t pin, int frequency, unsigned long duration);
void noTone(uint8_t pin);

#endif // _HAL_TIMERS_DUE_H
