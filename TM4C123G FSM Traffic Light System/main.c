// 1. Pre-processor Directives Section
#include "tm4c123gh6pm.h"
#include <stdint.h>


#define SENSOR  (*((volatile uint32_t *)0x400243FC)) // GPIO PORT E Data register
#define LIGHT   (*((volatile uint32_t *)0x400053FC)) // GPIO PORT B Data register

/* States */
#define goN 0
#define waitN 1
#define goE 2
#define waitE 3


typedef struct State {
	uint32_t Out;
	uint32_t Wait;
	uint32_t Next[4]; // goN, waitN, goE, waitE
} State_t;


// 3. Subroutines Section 

void PortBE_Init(void){
	volatile unsigned long delay;
	SYSCTL_RCGCGPIO_R |= ((0x01U << 1) | (0x01U <<  4));      // B E

  delay = SYSCTL_RCGCGPIO_R;      // no need to unlock

	GPIO_PORTE_LOCK_R |= 0x4C4F434BU;
	GPIO_PORTE_CR_R = 0x03U; // PE[1:0]
	GPIO_PORTE_AMSEL_R = 0x00U;
	GPIO_PORTE_PCTL_R = 0x00U;
	GPIO_PORTE_DIR_R |= 0x00U;   // inputs on PE1-0
	GPIO_PORTE_AFSEL_R |= 0x00U;
	GPIO_PORTE_PDR_R |= 0x03U;
  GPIO_PORTE_DEN_R |= 0x03U;    // enable digital on PE1-0

  GPIO_PORTB_DIR_R |= 0x3FU;    // outputs on PB5-0
  GPIO_PORTB_DEN_R |= 0x3FU;    // enable digital on PB5-0
}
	

// Initialize SysTick with busy wait running at bus clock
void SysTick_Init(void){
	NVIC_ST_CTRL_R = 0; // Disable SysTick during setup
	NVIC_ST_RELOAD_R = 0x00FFFFFFU; // Max reload value
	NVIC_ST_CURRENT_R = 0; // Any write to current clears it
	NVIC_ST_CTRL_R = 0x05U; // Enable SysTick with core clock
}

void SysTick_wait10ms(uint32_t delay){
		volatile uint32_t elapsedTime;
		uint32_t startTime = NVIC_ST_CURRENT_R;
		do{
			elapsedTime = (startTime-NVIC_ST_CURRENT_R)&0x00FFFFFF;
		}while(elapsedTime <= delay);
}		


void SysTick_Wait(uint32_t delay){
		uint32_t i;
		for(i=0; i< delay; i++){
			SysTick_wait10ms(160000); 
		}
}


int main(void){
		State_t FSM[4] = {
		{0x21, 300, {goN, waitN, goN, waitN}},
		{0x22, 200, {goE, goE, goE, goE}},
		{0x0C, 300, {goE, goE, waitE, waitE}},
		{0x14, 200, {goN, goN, goN, goN}},
	};
		
	uint32_t S; // index to the current state 
	uint32_t Input;
	
	PortBE_Init();
	SysTick_Init(); // Initialize SysTick, runs at 16 MHz 
	
	S = goN;
	
	while(1){
		LIGHT = FSM[S].Out; // Set lights 
		Input = SENSOR;
		S = FSM[S].Next[Input];
		SysTick_Wait(FSM[S].Wait);
	}
}
