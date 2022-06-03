// PICOCAM - Camera IF using RP2040
// this project communicates OV5642(or parallel 8-bit DVP-IF Image sensor with SCCB control) using Raspberry Pi PICO
// Need: A Raspberry Pi PICO, An IoT SRAM(APS1604M-3SQR or compatible), A DVP Camera(OV5642)
// Connection:
// ----- with OV Camera -----
// (PICO <-> OV5642/OV2640)
// GP0 ---> XCLK(24MHz Clock IN)
// GP1 <--- D2
// GP2 <--- D3
// GP3 <--- D4
// GP4 <--- D5
// GP5 <--- D6
// GP6 <--- D7
// GP7 <--- D8
// GP8 <--- D9
// GP9 <--- VSYNC
// GP10 <-- HREF(HSYNC)
// GP11 <-- PCLK(Pixel Clock OUT)

// GP12~GP15 : reserved for future use. (SPI1)

// GP26 --> SDA(SCCB DATA/I2C)
// GP27 --> SCL(SCCB CLK/I2C)

// ----- with IoT SRAM -----
// (PICO <-> APS1604M)
// GP16 <-> D0
// GP17 <-> D1
// GP18 <-> D2
// GP19 <-> D3
// GP20 --> xCS
// GP21 --> SCLK

#include <stdio.h>
#include <stdlib.h>
#include "pico/stdlib.h"
#include "hardware/gpio.h"
#include "cam.h"

#define BOARD_LED           (25)

void setup() {
    // system init
    set_sys_clock_khz(SYS_CLK_KHZ, true);
    stdio_init_all();

    // INIT LED
    gpio_init(BOARD_LED);
    gpio_set_dir(BOARD_LED, GPIO_OUT);
    gpio_put(BOARD_LED, 1);
    //gpio_pull_up(BOARD_LED);

}


int main() {

    setup();

    init_cam(DEV_OV5642);
   
    config_cam_buffer();    // config buffer
    start_cam();            // start streaming
    //uartout_cam();          // data via USB-UART

    // need more still?    
    /*
    uartout_cam();
    uartout_cam(); 
    uartout_cam(); 
    uartout_cam(); 
    uartout_cam(); 
    // ... 
    //
    */

    // or you have Raspberry Pi3 or Pi4 and MATLAB?
    // you can use SPI Slave. MATLAB Host program is in 'matlab/comm_raspi_spi.c'
    // caution: this is not stable. you may add 'check data and timeout' logic. 
    while(true) {
        spiout_cam();
        printf("OK\r\n");
    }
    

    // end
    free_cam();

    while (true);
}


