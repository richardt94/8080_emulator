#ifndef _MACHINE_H
#define _MACHINE_H

#include <stdint.h>
#include "cpu.h"

/*
    Header that defines the interface for the Space Invaders arcade machine
    core. This is a platform-independent layer that generates interrupts to 
    and reads video outputs from the CPU and memory.
*/

typedef struct Machine {
    CPUState *cs;
    uint16_t shift_reg;
    int shift_amt;
    uint8_t *framebuffer;
    uint8_t button_inputs[3];
} Machine;

//make a new machine struct and destroy an existing one
Machine *newMachine();
void destroyMachine(Machine *m);

//process CPU instructions until the next frame
void stepFrame(Machine *m);

//handle button presses (fed to input ports on the CPU)
typedef enum button {
    COIN,
    START1P,
    START2P,
    FIREP1,
    FIREP2,
    LEFTP1,
    LEFTP2,
    RIGHTP1,
    RIGHTP2,
    UNDEF_BUTTON
} button;

static const int PRESS = 1;
static const int RELEASE = 0;
void keyPressRelease(button b, Machine *m, int press);

#endif //_MACHINE_H