#include <stdio.h>
#include <mos_api.h>
#include <sys/timers.h>

#define CPU_FREQUENCY 50000000	// eZ80F92 clock frequency
#define UART_BASE_FREQUENCY 18432000	// eZ80F92 UARTs base clock frequency

#define PC_DR       0x009E		// Port C output data register
#define PC_DDR      0x009F		// Port C direction register
#define PC_ALT1	0x00A0		// Port C Alternate register 1
#define PC_ALT2     0x00A1         // Port C alternate register 2

#define UART1_BRG_L 0x00D0		// Bit Rate Generation L register
#define UART1_BRG_H 0x00D1		// Bit Rate Generation H register
#define UART1_LCTL  0x00D3		// bit 7=1 => Access to BRG registers (above)
#define UART1_MCTL  0x00D4		// nRTS and nDTE bits cleared (don't apply flow control).

#define UART1_THR   0x00D0		// Tx register with data to transmit (write-only).
#define UART1_RBR   0x00D0		// Rx register with data to receive (read-only).
#define UART1_LSR   0x00D5		// UART1 Line Status Register.
#define UART1_FCTL  0x00D2		// UART1 FIFO Control register.

typedef char i8;
typedef unsigned char u8;
typedef int i16;
typedef unsigned int  u16;
typedef long i32;
typedef unsigned long u32;


void uart1_begin(u32 bitrate)
{								//		     PC7            PC0
	mos_port_write(PC_DR, 0xff);		// 0x00 ??? -> %0 0 0 0  0    0     0   0
	mos_port_write(PC_DDR, 0xff);		// 0xeb ??? -> %1 1 1 0  1    0     1   1
	mos_port_write(PC_ALT1, 0x00);	// 0x00 ??? -> %0 0 0 0  0    0     0   0
	mos_port_write(PC_ALT2, 0x03);	// 0x03 ??? -> %0 0 0 0  0    0     1   1
								// Mode     ->  2 2 2 1  2    1     7   7
								//                      CTS1 RTS1  Rx1 Tx1
								//				     in  out

	u16 internalValue = UART_BASE_FREQUENCY / (16 * bitrate);
	u8  brLvalue = internalValue & 0x00ff;
	u8  brHvalue = (internalValue & 0xff00) >> 8;

	// Configuring UART1 for 9,600 bps operation
	mos_port_write(UART1_LCTL, 0x80);			// Enable UART1 BRG registers.
	mos_port_write(UART1_BRG_L, brLvalue);
	mos_port_write(UART1_BRG_H, brHvalue);
	mos_port_write(UART1_LCTL, 0x03);			// Disable UART1 BRG registers; 8 bits+1 stop bit
	mos_port_write(UART1_FCTL, 0x00);			// Disable Rx and Tx FIFO
}


u8 uart1_isTxEmpty()
{
	return mos_port_read(UART1_LSR & 0x20);		// 1 if empty / 0 if not empty.
}

u8 uart1_isRxEmpty()
{
	return mos_port_read(~(UART1_LSR & 0x01));	// 1 if empty / 0 if not empty.
}


u8 uart1_getc()
{
	//while(uart1_isRxEmpty() != 1);
	return mos_port_read(UART1_RBR);
}


u8 uart1_getc_blocked()
{
	while(uart1_isRxEmpty() != 1);
	return mos_port_read(UART1_RBR);
}


void uart1_putc(u8 c)
{
	//while(uart1_isTxEmpty() == 0);
	mos_port_write(UART1_THR, c);
}
long dd;

int main()
{
	printf("Enabling UART1 @ 9,600 [bps]...\n\n");
	
	uart1_begin(9600);
	
	printf("Sending bytes through UART1 port (pins PC0<->TxD & PC1 <-> RxD)...\n\n");
	u8 p = 0;
	while(1)
	{
		printf("%d\n", p);
		
		uart1_putc(p++);
		
		printf("char sent.\n\n");
		
		for(long pause = 1; pause <= 100000; pause++)
			dd +=(u8)pause;
	}
	
	return 0;
}
