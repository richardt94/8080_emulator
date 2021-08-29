#include <stdlib.h>
#include <stdio.h>
#include "machine.h"
#include "cpu.h"

//timing ratio constant
//60 Hz screen refresh, 2 MHz CPU
const int cycles_per_frame = 33333;

Machine *newMachine() {
    Machine *m = malloc(sizeof(Machine));
    //machine has an 8080 CPU with 16K of memory total
    //(8K is ROM but let's just have it all as RAM)
    CPUState *cs = newState(0x4000);
    //Video RAM starts at 9K
    uint8_t *framebuffer = (uint8_t *) &cs->memory[0x2400];
    //coin slot is port 1, bit 0.
    //start with a coin in the machine.
    cs->ports[0] = 0x80;

    m->cs = cs;
    m->shift_reg = 0;
    m->shift_amt = 0;
    m->framebuffer = framebuffer;

    //set "always 1" bits from computer archaeology
    //and coin dip.
    m->button_inputs[0] = 0x0d;
    m->button_inputs[1] = 0x08;
    m->button_inputs[2] = 0x00;

    return m;
}

void destroyMachine(Machine *m) {
    destroyState(m->cs);
    m->framebuffer = NULL;
    free(m);
}

static void handleOutput(byte port, Machine *m) {
    if (port == 2) m->shift_amt = m->cs->ports[2];
    else if (port == 4) {
        m->shift_reg >>= 8;
        m->shift_reg |= m->cs->ports[4] << 8;
    }
    //clear output port
    m->cs->ports[port] = 0;

    //ignore other outputs for now
    //(will need to deal with sound eventually)
}

void stepFrame(Machine *m) {
    int ncycles = 0;
    while (ncycles < cycles_per_frame / 2) {
        // fire input ports according to button and shift reg state
        for (int inport = 0; inport < 3; inport++)
            m->cs->ports[inport] = m->button_inputs[inport];
        m->cs->ports[3] = (m->shift_reg >> (8 - m->shift_amt)) & 0xff;
        ncycles += stepCPU(m->cs);
        if (m->cs->write_flag >= 0) handleOutput(m->cs->write_flag, m);
    }
    //half screen (RST 1 from computer archaeology space invaders article)
    interruptCPU(m->cs, 0xcf);
    while (ncycles < cycles_per_frame) {
        m->cs->ports[3] = (m->shift_reg << m->shift_amt) >> 8;
        ncycles += stepCPU(m->cs);
        if (m->cs->write_flag >= 0) handleOutput(m->cs->write_flag, m);
    }
    //vblank (RST 2)
    interruptCPU(m->cs, 0xd7);
}

void keyPressRelease(button b, Machine *m, int press) {
    //set/unset bits in the input bus corresponding
    //to button presses
    int port_num = -1;
    uint8_t port_val = 0;
    switch (b) {
        case COIN:
        {
            port_num = 1;
            port_val = 0x01;
            break;
        }
        case START1P:
        {
            port_num = 1;
            port_val = 0x04;
            break;
        }
        case LEFTP1:
        {
            port_num = 1;
            port_val = 0x20;
            break;
        }
        case RIGHTP1:
        {
            port_num = 1;
            port_val = 0x40;
            break;
        }
        case FIREP1:
        {
            port_num = 1;
            port_val = 0x10;
            break;
        }
        default: break;
    }
    if (port_val < 0) return;
    if (press) m->button_inputs[port_num] |= port_val;
    else m->button_inputs[port_num] &= ~port_val;
}

