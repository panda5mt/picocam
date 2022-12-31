#include <stdio.h>
#include <string.h>
#include "pico/stdlib.h"
#include "pico/binary_info.h"
#include "hardware/dma.h"
#include "sfp_hw.h"
/////
uint32_t DMA_SER_WR0;
uint32_t tx_buf_udp[DEF_UDP_BUF_SIZE+1] = {0};
uint8_t  udp_payload[DEF_UDP_PAYLOAD_SIZE] = {0};
uint32_t lp_cnt = 0;
uint32_t offset_sfp = 0;
uint32_t sm_sfp;

volatile bool sfp_initialized = false;
uint32_t c[DEF_UDP_PAYLOAD_SIZE/sizeof(uint32_t)];


void sfp_hw_init(PIO pio) {

    if(true == sfp_initialized) {
        return;
    }
    else {
        sfp_initialized = true;
    }


    gpio_init(HW_PINNUM_SFP0_TXD);
    gpio_set_dir(HW_PINNUM_SFP0_TXD, GPIO_OUT);
    //hw_test();

    // PIO (Serializer) init
    offset_sfp = pio_add_program(pio, &ser_100base_fx_program);
    sm_sfp = 0;//pio_claim_unused_sm(pio, true);
    //printf("sfp:pio=%d, sm = %d, offset=%d\r\n",(uint32_t)pio, sm_sfp, offset_sfp);

    ser_100base_fx_program_init(pio, sm_sfp, offset_sfp, HW_PINNUM_SFP0_TXD);       // for 100BASE-FX 100Mbps

    // UDP
    udp_init();
    sprintf(udp_payload, "a");
    udp_packet_gen(tx_buf_udp, udp_payload);

    // DMA channel setting(SFP0)
    DMA_SER_WR0 = dma_claim_unused_channel(true);
    dma_channel_config c0 = dma_channel_get_default_config(DMA_SER_WR0);
    channel_config_set_dreq(&c0, pio_get_dreq(pio, sm_sfp, true));
    channel_config_set_transfer_data_size(&c0, DMA_SIZE_32);
    channel_config_set_read_increment(&c0, true);
    channel_config_set_write_increment(&c0, false);
    dma_channel_configure (
        DMA_SER_WR0,            // Channel to be configured
        &c0,                    // The configuration we just created
        &pio->txf[0],           // Destination address
        tx_buf_udp,             // Source address
        (DEF_UDP_BUF_SIZE+1),   // Number of transfers
        false                   // Don't start yet
    );

    return;

}

void __time_critical_func(sfp_send)(void* str, uint16_t len) {

    udp_packet_gen(tx_buf_udp, (uint8_t *)str);
    // Wait for DMA
    dma_channel_wait_for_finish_blocking(DMA_SER_WR0);
    sleep_us(2);
    // DMA Start
    dma_channel_set_read_addr(DMA_SER_WR0, tx_buf_udp, true);

}

void __time_critical_func(sfp_send_with_header)(uint32_t header1, uint32_t header2, uint32_t header3, uint32_t header4, void* str, uint16_t len) {


    int32_t *b = (uint32_t*)str;
    b[0] = header1;
    b[1] = header2;
    b[2] = header3;
    b[3] = header4;
    
    //memcpy((uint32_t *) &c[4],(uint32_t *) str,len);
    //memcpy(udp_payload, c, len + sizeof(uint32_t)*4);// length of str + length of header
    //udp_packet_gen(tx_buf_udp, udp_payload);
    udp_packet_gen(tx_buf_udp,(uint8_t *) b);
    
    // Wait for DMA
    dma_channel_wait_for_finish_blocking(DMA_SER_WR0);
    sleep_us(2);
    // DMA Start
    dma_channel_set_read_addr(DMA_SER_WR0, tx_buf_udp, true);

}

void __time_critical_func(sfp_test)() {

    lp_cnt = 0;
    while( true ) {
        
        sprintf(udp_payload, "Hello RP2040 count = %d",lp_cnt);
        udp_packet_gen(tx_buf_udp, udp_payload);
    
        // Wait for DMA
        dma_channel_wait_for_finish_blocking(DMA_SER_WR0);
        
        // DMA Start
        dma_channel_set_read_addr(DMA_SER_WR0, tx_buf_udp, true);
        lp_cnt = lp_cnt + 1;
    }
}