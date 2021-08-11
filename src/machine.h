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
    byte *framebuffer;
} Machine;

//make a new machine struct and destroy an existing one
Machine *newMachine();
void destroyMachine(Machine *m);

//process CPU instructions until the next frame
void stepFrame(Machine *m);

#endif //_MACHINE_H