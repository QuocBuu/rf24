#include "test_nf24_no_task.h"

#define ADDRESS_ME      (0)
#define ADDRESS_YOU     (1)

/* ring buffer */
uint8_t nf24_buffer_send[NF24_BUFFER_LENGHT];
uint8_t nf24_buffer_recv[NF24_BUFFER_LENGHT];
ring_buffer_init ring_buffer_nf24_send;
ring_buffer_init ring_buffer_nf24_recv;

/* state handware */
static nf24_state_handware_e nf24_handware_state;
static nf24_state_handware_e get_nf24_state_handware();
static void set_nf24_state_handware(nf24_state_handware_e state);
/* state send */
static nf24_send_state_e nf24_send_state;
static nf24_send_state_e get_nf24_send_state();
static void set_nf24_send_state(nf24_send_state_e state);
/* state receive*/
static nf24_recv_state_e nf24_recv_state;
static nf24_recv_state_e get_nf24_recv_state();
static void set_nf24_recv_state(nf24_recv_state_e state);

/* void */
void init_nf24();
void init_ring_buffer_nf24();
void irq_nf24();
void irq_uart2();
void send_data_nf24(uint8_t add, uint8_t* data, uint8_t len);
void recv_data_nf24(ring_buffer_t* buffer, uint8_t* data, uint8_t len);

#define RS_485_UART_2_ENABLE_TX() \
do { \
    io_rs485_dir_high(); \
} while( 0 )

#define RS_485_UART_2_DISABLE_TX() \
do { \
    io_rs485_dir_low(); \
} while( 0 )

static uint8_t nf24_pload_frame_buffer[MAX_PHY_PAYLOAD_LEN];

static uint8_t nf24_src_addr[5] = {0x3c, 0x3c, 0x3c, 0, 0};
static uint8_t nf24_des_addr[5] = {0x3c, 0x3c, 0x3c, 0, 0};

void init_nf24() {
    /* set addr my nf24 */
    nrf_set_static_nwk_addr(ADDRESS_ME);

    /* init physic layer */
        // IO
        /* init io control of nrf24 (CE, NCS, IRQ) */
	    nrf24l01_io_ctrl_init()
        nrf24l01_spi_ctrl_init();

        CE_LOW();
        sys_ctrl_delay_ms(100);

        /* power down */
        hal_nrf_set_power_mode(HAL_NRF_PWR_DOWN);
        /* clear flags interrup */
        hal_nrf_get_clear_irq_flags();
        
        /* First close all radio pipes, Pipe 0 and 1 open by default */
        hal_nrf_close_pipe(HAL_NRF_ALL); 
        /* Open pipe0, without/autoack (autoack) */
		hal_nrf_open_pipe(HAL_NRF_PIPE0, true);

        /* Operates in 16bits CRC mode */
        hal_nrf_set_crc_mode(HAL_NRF_CRC_16BIT); 
        /* Enable auto retransmit */
		hal_nrf_set_auto_retr(5, 750); 

        /* 5 bytes address width */
		hal_nrf_set_address_width(HAL_NRF_AW_5BYTES);
        /* Set device's addresses */
		hal_nrf_set_address(HAL_NRF_TX, (uint8_t*)nrf_get_src_phy_addr()); 
        /* Sets receiveing address on pipe0 */
		hal_nrf_set_address(HAL_NRF_PIPE0, (uint8_t*)nrf_get_src_phy_addr()); 

        /* set recv data */
		hal_nrf_set_operation_mode(HAL_NRF_PRX);
        /* size pload on pipe0 */
		hal_nrf_set_rx_pload_width((uint8_t)HAL_NRF_PIPE0, 32);


		hal_nrf_set_rf_channel(NRF_PHY_CHANEL_CFG);
		hal_nrf_set_output_power(HAL_NRF_0DBM);
		hal_nrf_set_lna_gain(HAL_NRF_LNA_HCURR);
		hal_nrf_set_datarate(HAL_NRF_2MBPS);

		hal_nrf_set_power_mode(HAL_NRF_PWR_UP); /* Power up device */

        /* set interrup on*/
		hal_nrf_set_irq_mode(HAL_NRF_MAX_RT, true);
		hal_nrf_set_irq_mode(HAL_NRF_TX_DS, true);
		hal_nrf_set_irq_mode(HAL_NRF_RX_DR, true);

        /* reset rx, tx*/
		hal_nrf_flush_rx();
		hal_nrf_flush_tx();

		sys_ctrl_delay_ms(2);
		CE_HIGH();

		ENTRY_CRITICAL();
		// phy_state = PHY_STATE_HARDWARE_STARTED;
		EXIT_CRITICAL();
}

void init_ring_buffer_nf24() {
    ring_buffer_init(   &ring_buffer_nf24_send, \
                        &nf24_buffer_send, \
                        NF24_BUFFER_LENGHT, \
                        sizeof(uint8_t));
    ring_buffer_init(   &ring_buffer_nf24_recv, \
                        &nf24_buffer_recv, \
                        NF24_BUFFER_LENGHT, \
                        sizeof(uint8_t));
}

void irq_nf24() {
    if(get_nf24_state_handware() == NF24_STATE_HANDWARE_NONE) {
        return;
    }

    /* read flags and clear flags */
    uint8_t nrf24_irq_mask = hal_nrf_get_clear_irq_flags();

    switch (nrf24_irq_mask) {
        /* not enough Tx fifo */
        case (1 << HAL_NRF_MAX_RT): {
            /* not */
        }
            break;

        /* send frame done */
        case (1 << HAL_NRF_TX_DS): {
            /*status */
            set_nf24_send_state(NF24_SEND_STATE_SLEEP);
        }
            break;

        /* have a frame recv */
        case (1 << HAL_NRF_RX_DR): {
            if (!hal_nrf_rx_fifo_empty()) {
                uint8_t pl_len;
                /* write frame at buffer and read pload len */
                pl_len = hal_nrf_read_rx_pload(nf24_pload_frame_buffer);
                /* frame == 32 byte */
                if (pl_len == MAX_PHY_PAYLOAD_LEN) {
                    /* not */
                }
                /* frame == 1 byte */
                else if (pl_len == 1) {
                    /* read byte and write modbus */
                    recv_data_nf24(&ring_buffer_nf24_recv, &nf24_pload_frame_buffer, pl_len);
                }
                else if (pl_len < MAX_PHY_PAYLOAD_LEN) {
                    // while (pl_len) {}
                }
                else {
                    /* not */
                }

            }
        }
            break;

        default: {
            /* Clear all flags interrup */
            hal_nrf_get_clear_irq_flags();
        }
            break;
    }

    /* ring buffer != 0, nf24 not send */
    if (!ring_buffer_is_empty(ring_buffer_nf24_recv) && get_nf24_send_state() == NF24_SEND_STATE_SLEEP) {
        USART_ITConfig(USART2, USART_IT_TXE, ENABLE);
    }
}

void irq_uart2() {
    /* Recicev*/
    if (USART_GetITStatus(USART2, USART_IT_RXNE) == ENABLE) {
        volatile uint8_t b;
        b = (uint8_t)USART_ReceiveData(USART2);
        ring_buffer_put(&ring_buffer_nf24_send, &b);
    }
    /* send */
    if (USART_GetITStatus(USART2, USART_IT_TXE) == ENABLE) {
        USART_ClearITPendingBit(USART2, USART_IT_TXE);
        /* buffer no data */
        if(ring_buffer_is_empty(&ring_buffer_nf24_recv)){
            USART_ITConfig(USART2, USART_IT_TC, ENABLE);
            USART_ITConfig(USART2, USART_IT_TXE, DISABLE);
        }
        /* buffer have data */
        else {
            volatile uint8_t c;
            ring_buffer_get(&ring_buffer_nf24_recv, &c);
            USART_SendData(USART2, c);
        }
    }
    /* off Tx rs485 */
    if (USART_GetITStatus(USART2, USART_IT_TC) == ENABLE) {
        io_rs485_dir_low();
        USART_ITConfig(USART2, USART_IT_TC, DISABLE);
    }
}

void send_data_nf24(uint8_t add, uint8_t* data, uint8_t len) {
    /* set new address*/
    nrf_set_des_nwk_addr(add);

    /* set address */
    hal_nrf_set_address(HAL_NRF_TX, (uint8_t*)nrf_get_des_phy_addr()); /* Set device's addresses */
    hal_nrf_set_address(HAL_NRF_PIPE0, (uint8_t*)nrf_get_des_phy_addr()); /* Sets receiveing address on pipe0 */
    
    /* write data */
    hal_nrf_write_tx_pload(data, len);
}

void recv_data_nf24(ring_buffer_t* buffer, uint8_t* data, uint8_t len) {
    for(uint8_t i = 0; i < len, i++) {
        ring_buffer_put(buffer, (data + i));
    }
}
/*************************************************************************
 * Set/get status nf24 handware/send/receivee
 *************************************************************************
*/

nf24_state_handware_e get_nf24_state_handware() {
    return nf24_handware_state;
}

nf24_send_state_e get_nf24_send_state() {
    return nf24_send_state;
}

nf24_recv_state_e get_nf24_recv_state() {
    return nf24_recv_state;
}

void set_nf24_state_handware(nf24_state_handware_e state) {
    nf24_handware_state = state;
}

void set_nf24_send_state(nf24_send_state_e state) {
    nf24_send_state = state;
}

void set_nf24_recv_state(nf24_recv_state_e state) {
    nf24_recv_state = state;
}