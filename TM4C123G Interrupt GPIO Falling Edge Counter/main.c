#include "tm4c123gh6pm.h"
#include <stdint.h>


volatile uint32_t FallingEdges =  0;

void EdgeCounter_Init(void){
	SYSCTL_RCGCGPIO_R |= 0x01U << 5;
	FallingEdges = 0;
	/* PortF has special function, need to unlock to modify */
	GPIO_PORTF_LOCK_R = 0x4C4F434B; // Unlock commit register
	GPIO_PORTF_CR_R = 0x01; // Make PF0 configurable 
	GPIO_PORTF_LOCK_R = 0; // Lock commit register 
	
	GPIO_PORTF_DIR_R &= ~(0x01U << 0); // Set PF0 as a digital input pin
	GPIO_PORTF_DEN_R |= (0x01U << 0); // Enable digital PF0
	
	GPIO_PORTF_IS_R &= ~(0x01U << 0); // PF0 is edge sensitive
	GPIO_PORTF_IBE_R &= ~(0x01U << 0); // PF0 is not both edges which means trigger controlled by IEV
	GPIO_PORTF_IEV_R &= ~(0x01U << 0); // PF0 falling edge trigger 
	GPIO_PORTF_ICR_R |= (0x01U << 0); // Clear any prior interrupt on PF0
	GPIO_PORTF_IM_R |= (0x01U << 0); // ARM interrupt on PF0 = Unmask interrupt = Current CPU interrupt on 
	
	/* Enable interrupt in NVIC and set priority */
	NVIC_PRI7_R = (NVIC_PRI7_R&0x0FF00FFFF)|0x00A00000; // Set interrupt priority to 5
	NVIC_EN0_R |= (0x01U << 30); // Enable interrupt 30 in NVIC which is Port F handler 
}

void GPIOF_Handler(void){
	GPIO_PORTF_ICR_R = (0x01U << 0); // Clear the interrupt flag
	FallingEdges = FallingEdges + 1; // Increment
}

int main(){
	EdgeCounter_Init();
	while(1){
		// Do nothing and wait for the interrupt to occur
	}
}