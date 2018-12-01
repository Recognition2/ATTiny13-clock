//
// Created by gregory on 23/11/18.
//

#ifndef LEDCLOCK_PIN_CONFIG_H
#define LEDCLOCK_PIN_CONFIG_H

// CPU frequency defined as an unsigned long
#define F_CPU (8UL*1000UL*1000UL)

// Which pin corresponds to the blink pin
#define BLINK_PIN       1

// The two pins used for I2C communication
#define I2C_CLK_PIN     4 // SCL
#define I2C_DATA_PIN     5 // SDA

// Sanity placeholders for NOP's
#if defined(__arm__)
# define NOP __asm__ __volatile__ ("nop\n");
#else
#  define NOP __asm__ __volatile__ ("cp r0,r0\n");
#endif

#define NOP2 NOP NOP
#define NOP4 NOP2 NOP2


#endif //LEDCLOCK_PIN_CONFIG_H
