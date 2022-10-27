// -------------------------------------------------- //
// This file is autogenerated by pioasm; do not edit! //
// -------------------------------------------------- //

#pragma once

#if !PICO_NO_HARDWARE
#include "hardware/pio.h"
#endif

// ---------- //
// picampinos //
// ---------- //

#define picampinos_wrap_target 0
#define picampinos_wrap 22

static const uint16_t picampinos_program_instructions[] = {
            //     .wrap_target
    0x6020, //  0: out    x, 32                      
    0x6040, //  1: out    y, 32                      
    0x2028, //  2: wait   0 pin, 8                   
    0x20a8, //  3: wait   1 pin, 8                   
    0xa022, //  4: mov    x, y                       
    0x2029, //  5: wait   0 pin, 9                   
    0x20a9, //  6: wait   1 pin, 9                   
    0x20aa, //  7: wait   1 pin, 10                  
    0x4008, //  8: in     pins, 8                    
    0x202a, //  9: wait   0 pin, 10                  
    0x20aa, // 10: wait   1 pin, 10                  
    0x4008, // 11: in     pins, 8                    
    0x202a, // 12: wait   0 pin, 10                  
    0x20aa, // 13: wait   1 pin, 10                  
    0x4008, // 14: in     pins, 8                    
    0x202a, // 15: wait   0 pin, 10                  
    0x20aa, // 16: wait   1 pin, 10                  
    0x4008, // 17: in     pins, 8                    
    0x202a, // 18: wait   0 pin, 10                  
    0xa0d6, // 19: mov    isr, ::isr                 
    0x8020, // 20: push   block                      
    0x0046, // 21: jmp    x--, 6                     
    0x0002, // 22: jmp    2                          
            //     .wrap
};

#if !PICO_NO_HARDWARE
static const struct pio_program picampinos_program = {
    .instructions = picampinos_program_instructions,
    .length = 23,
    .origin = -1,
};

static inline pio_sm_config picampinos_program_get_default_config(uint offset) {
    pio_sm_config c = pio_get_default_sm_config();
    sm_config_set_wrap(&c, offset + picampinos_wrap_target, offset + picampinos_wrap);
    return c;
}

static inline void picampinos_program_init( PIO pio, uint32_t sm, uint32_t offset, uint32_t in_base ,uint32_t in_pin_num )
{
    pio_sm_config c = picampinos_program_get_default_config( offset );
    sm_config_set_set_pins(&c, in_base, in_pin_num);
    sm_config_set_in_pins( &c, in_base );
    sm_config_set_in_shift( &c, false, false, 32); // auto push : false
    sm_config_set_out_shift( &c, false, true, 32); // auto pull : true
    //sm_config_set_fifo_join( &c, PIO_FIFO_JOIN_RX);
    {
        uint32_t pin_offset;
        for ( pin_offset = 0; pin_offset < in_pin_num; pin_offset++ )
        {
            pio_gpio_init( pio, in_base + pin_offset );
        }
    }
    pio_sm_set_consecutive_pindirs( pio, sm, in_base, in_pin_num, false );
    sm_config_set_clkdiv( &c, 1 );
    pio_sm_init( pio, sm, offset, &c );
    pio_sm_set_enabled( pio, sm, true );
}

#endif

