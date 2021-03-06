
/*
 * noise.h
 *
 * Created: 12/5/2019 3:01:13 PM
 *  Author: Anna
 */ 
/*
 * lab8_4this.c
 *
 * Created: 11/19/2019 2:01:31 PM
 *  Author: annah
 */ 
#include <avr/io.h>
#include <avr/interrupt.h>
extern void clock_init(void);

int16_t tab[256] =
{
	0x800,0x832,0x864,0x896,0x8c8,0x8fa,0x92c,0x95e,
	0x98f,0x9c0,0x9f1,0xa22,0xa52,0xa82,0xab1,0xae0,
	0xb0f,0xb3d,0xb6b,0xb98,0xbc5,0xbf1,0xc1c,0xc47,
	0xc71,0xc9a,0xcc3,0xceb,0xd12,0xd39,0xd5f,0xd83,
	0xda7,0xdca,0xded,0xe0e,0xe2e,0xe4e,0xe6c,0xe8a,
	0xea6,0xec1,0xedc,0xef5,0xf0d,0xf24,0xf3a,0xf4f,
	0xf63,0xf76,0xf87,0xf98,0xfa7,0xfb5,0xfc2,0xfcd,
	0xfd8,0xfe1,0xfe9,0xff0,0xff5,0xff9,0xffd,0xffe,
	0xfff,0xffe,0xffd,0xff9,0xff5,0xff0,0xfe9,0xfe1,
	0xfd8,0xfcd,0xfc2,0xfb5,0xfa7,0xf98,0xf87,0xf76,
	0xf63,0xf4f,0xf3a,0xf24,0xf0d,0xef5,0xedc,0xec1,
	0xea6,0xe8a,0xe6c,0xe4e,0xe2e,0xe0e,0xded,0xdca,
	0xda7,0xd83,0xd5f,0xd39,0xd12,0xceb,0xcc3,0xc9a,
	0xc71,0xc47,0xc1c,0xbf1,0xbc5,0xb98,0xb6b,0xb3d,
	0xb0f,0xae0,0xab1,0xa82,0xa52,0xa22,0x9f1,0x9c0,
	0x98f,0x95e,0x92c,0x8fa,0x8c8,0x896,0x864,0x832,
	0x800,0x7cd,0x79b,0x769,0x737,0x705,0x6d3,0x6a1,
	0x670,0x63f,0x60e,0x5dd,0x5ad,0x57d,0x54e,0x51f,
	0x4f0,0x4c2,0x494,0x467,0x43a,0x40e,0x3e3,0x3b8,
	0x38e,0x365,0x33c,0x314,0x2ed,0x2c6,0x2a0,0x27c,
	0x258,0x235,0x212,0x1f1,0x1d1,0x1b1,0x193,0x175,
	0x159,0x13e,0x123,0x10a,0xf2,0xdb,0xc5,0xb0,
	0x9c,0x89,0x78,0x67,0x58,0x4a,0x3d,0x32,
	0x27,0x1e,0x16,0xf,0xa,0x6,0x2,0x1,
	0x0,0x1,0x2,0x6,0xa,0xf,0x16,0x1e,
	0x27,0x32,0x3d,0x4a,0x58,0x67,0x78,0x89,
	0x9c,0xb0,0xc5,0xdb,0xf2,0x10a,0x123,0x13e,
	0x159,0x175,0x193,0x1b1,0x1d1,0x1f1,0x212,0x235,
	0x258,0x27c,0x2a0,0x2c6,0x2ed,0x314,0x33c,0x365,
	0x38e,0x3b8,0x3e3,0x40e,0x43a,0x467,0x494,0x4c2,
	0x4f0,0x51f,0x54e,0x57d,0x5ad,0x5dd,0x60e,0x63f,
	0x670,0x6a1,0x6d3,0x705,0x737,0x769,0x79b,0x7cd,
};



void dac_init(void){
	//CTRLB channel 0 default used
	
	//#define DAC_CH0EN_bm  0x04  /* Channel 0 Output Enable bit mask. */
	DACA.CTRLA = DAC_ENABLE_bm|DAC_CH1EN_bm;	//enable DAC mod and CH0, output enable pin for ch 1 enabled
	
	//DAC_CHSEL_SINGLE_gc = (0x00<<5),  /* Single channel operation (Channel 0 only) */
	//DAC_CH0TRIG_bm  0x01  /* Channel 0 Event Trig Enable bit mask. */
	DACA.CTRLB = DAC_CHSEL_SINGLE1_gc|DAC_CH1TRIG_bm; //single channel operation for channel 0 only
	//CTRLC, set Internal 1.00V - default

	//DAC_REFSEL_AREFB_gc = (0x03<<3),  /* External reference on AREF on PORTB */
	DACA.CTRLC =  DAC_REFSEL_AREFB_gc;
	//EVCTRL controls interrupts
	//DAC_EVSEL_0_gc
	//DAC_EVSEL0_bm
	DACA.EVCTRL = DAC_EVSEL_0_gc;
	
	//DACA.CH0DATA = 0x37;
}

void timer_initnoise(){
	//1000Hz
	//500(PER)=(32e6/64)*.001
	TCC0.PER = 71;
	TCC0.CTRLA = TC_CLKSEL_DIV1_gc;	//prescaler == 64
	//TCC0.INTCTRLA = TC_OVFINTLVL_LO_gc;
	//PMIC.CTRL = PMIC_LOLVLEN_bm;
	//sei();
}

void dma_init(void){
	DMA.CTRL |= DMA_RESET_bm;
	
	//ADDRCTRL reg- configures how data is transfered from src to destination
	//src addr corresponds to 1st index of arr
	//reload bitfield, when does src add reload to OG config
	//every triggere add inc to next element, at end reset to beg @end of block
	//after transcmtting, inc to get to next byte
	//DESTINATIONRELOAD is noe, its the same DAC output register- FIXED dest addr mode
	DMA.CH0.ADDRCTRL = DMA_CH_SRCRELOAD_BLOCK_gc|DMA_CH_SRCDIR_INC_gc|
	DMA_CH_DESTRELOAD_BURST_gc|DMA_CH_DESTDIR_INC_gc;

	
	//trigger src reg trigger off of other peripherals- DMA burst trigger off of other periph (TCC0 ovf flag)
	DMA.CH0.TRIGSRC = DMA_CH_TRIGSRC_EVSYS_CH0_gc;
	//transfer count 16 bit reg determines how many bytes per block
	DMA.CH0.TRFCNT = (uint16_t)(sizeof(tab));	//bytes in array - 256 elements*2bytes
	//int i = (uint16_t)(sizeof(tab));	//evaluates to 512
	//ulimited repets by setting REPCNT to zero
	DMA.CH0.REPCNT = 0x00;

	
	//printf("%d", i);
	//init src and dest addr
	//if reg greater than 8 bits load low byte 1st
	//SRCADDR 24 bit value
	DMA.CH0.SRCADDR0 = (uintptr_t)tab;	//arith with pointers, cast to 8 bit src addr 0 is 8 bits
	DMA.CH0.SRCADDR1 = ( ((uintptr_t)tab) >> 8);
	DMA.CH0.SRCADDR2 = ( (uint32_t)((uintptr_t)tab) >> 16);
	
	//dest
	DMA.CH0.DESTADDR0 = (uint8_t)(uintptr_t)&DACA.CH1DATA;	//& returns address of register
	DMA.CH0.DESTADDR1 = (uint8_t)( ((uintptr_t)&DACA.CH1DATA) >> 8);
	DMA.CH0.DESTADDR2 = (uint8_t)( (uint32_t)((uintptr_t)&DACA.CH1DATA) >> 16);
	//choose channel 0
	//DMA.CH0.CTRLA = DMA_CH_SINGLE_bm|DMA_CH_BURSTLEN_2BYTE_gc|DMA_CH_SINGLE_bm;	//const 1.5 volts
	DMA.CH0.CTRLA = DMA_CH_BURSTLEN_2BYTE_gc | DMA_CH_REPEAT_bm | DMA_CH_SINGLE_bm;
	
	DMA.CH0.CTRLA |= DMA_CH_ENABLE_bm;
	
	//enable entire DMA
	DMA.CTRL |= DMA_ENABLE_bm;	//or equal to not overwrite bits

}
