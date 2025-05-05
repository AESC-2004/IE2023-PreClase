/*
 * PreClase.c
 *
 * Created: 30/04/2025 04:06:09 a. m.
 * Author : ang50
 */ 

/*********************************************************************************************************************************************/
// Header (Libraries)
#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdint.h>
#include <string.h>
#include "m328pusart.h"

/*********************************************************************************************************************************************/
// Function prototypes
void SETUP();

/*********************************************************************************************************************************************/
// Main Function
int main(void)
{
	SETUP();
	while (1)
	{
		if ((usart_get_received_length() > 0) && ((usart_get_received_byte((uint8_t)(usart_get_received_length() - 1))) == '\r'))
		{
			// Setting a new buffer, sized as the RX buffer to be cautious, for using the "receive_string" function
			char msg[USART_RX_BUFFER_SIZE];
			usart_receive_string(msg);
			
			if (!strcmp(msg, "1"))
			{
				PORTB |= (1 << PORTB0);
				usart_transmit_string("LED turned ON\n\n");
				
			} else if (!strcmp(msg, "0"))
			{
				PORTB &= ~(1 << PORTB0);
				usart_transmit_string("LED turned OFF\n\n");
			} else
			{
				usart_transmit_string("Non valid function\n\n");
			}
			
			// Flushing the rx buffer for new incoming data
			usart_rx_buffer_flush();
		}
	}
}

/*********************************************************************************************************************************************/
// NON-Interrupt subroutines
void SETUP()
{
	// Disabling interrupts
	cli();
	// Setting PORTB for outputting the USART-controlled LED
	DDRB	|= (1 << DDB0);
	// Setting PORTD for outputting the button-controlled LED
	DDRD	|= (1 << DDD7);
	// Setting PORTC for lecturing the button (Pull-up enabled)
	DDRC	&= ~(1 << DDC4);
	PORTC	|= (1 << PORTC4);
	// Enabling PC interrupts for PC4
	PCICR	|= (1 << PCIE1);
	PCMSK1	|= (1 << PCINT12);
	//2X UART 8b, no parity, 1 stop bit, 9600baud rate
	uart_init(USART_SPEED_DOUBLE, USART_CHARACTER_SIZE_8b, USART_PARITY_MODE_DISABLED, USART_STOP_BIT_1b, USART_MULTIPROCESSOR_COMMUNICATION_MODE_DISABLED, 207);
	usart_data_register_empty_interrupt_enable();
	usart_rx_interrupt_enable();
	sei();
}

/*********************************************************************************************************************************************/
// Interrupt routines

ISR(USART_UDRE_vect)
{
	usart_load_next_byte();
}

ISR(USART_RX_vect)
{
	usart_receive_bytes();
}

ISR(PCINT1_vect)
{
	if (!(PINC & (1 << PINC4))) PORTD |= (1 << PORTD7);
	else PORTD &= ~(1 << PORTD7);
}
