#include "tm4c123gh6pm.h"

void PortE_Init(void);
void PortF_Init(void);


int main(void){
	unsigned long SW0, SW1, SW2;
	unsigned long Out3, Out4, Out5;
	
	PortE_Init();
	PortF_Init();
	
	while(1){
		SW0 = (GPIO_PORTE_DATA_R & (0x01U << 0));
		SW1 = (GPIO_PORTE_DATA_R & (0x01U << 1));
		SW2 = (GPIO_PORTE_DATA_R & (0x01U << 2));
		Out3 = (GPIO_PORTE_DATA_R & (0x01U << 3));		
		Out4 = (GPIO_PORTE_DATA_R & (0x01U << 4));
		Out5 = (GPIO_PORTE_DATA_R & (0x01U << 5));
		
		
		if(SW0){
			GPIO_PORTF_DATA_R |= (0x01U << 3);
			GPIO_PORTE_DATA_R |= (0x01U << 3);
		}else{
			GPIO_PORTF_DATA_R &= ~(0x01U << 3);
			GPIO_PORTE_DATA_R &= ~(0x01U << 3);
		}
		if(SW1){
			GPIO_PORTF_DATA_R |= (0x01U << 2);
			GPIO_PORTE_DATA_R |= (0x01U << 4);
		}else{
			GPIO_PORTF_DATA_R &= ~(0x01U << 2);
			GPIO_PORTE_DATA_R &= ~(0x01U << 4);
		}
		if(SW2){
			GPIO_PORTF_DATA_R |= (0x01U << 1);
			GPIO_PORTE_DATA_R |= (0x01U << 5);
		}else{
			GPIO_PORTF_DATA_R &= ~(0x01U << 1);
			GPIO_PORTE_DATA_R &= ~(0x01U << 5);
		}
	}
	
	
	
}




void PortE_Init(void){
	unsigned long volatile delay; 
	SYSCTL_RCGCGPIO_R |= (0x01U << 4);
	delay = SYSCTL_RCGCGPIO_R;
	
	// PE[2:0] input, PE[5:3] output 
	GPIO_PORTE_DIR_R |= 0x38U; 
	
	GPIO_PORTE_DEN_R |= 0x3FU;
}


void PortF_Init(void){
	unsigned long volatile delay;
	SYSCTL_RCGCGPIO_R |= (0x01U << 5);
	delay = SYSCTL_RCGCGPIO_R;
	
	GPIO_PORTF_LOCK_R |= 0x4C4F434BU; 
	GPIO_PORTF_CR_R |= 0x1FU;
	GPIO_PORTF_AMSEL_R |= 0x00U;
	GPIO_PORTF_PCTL_R |= 0x00U;
	GPIO_PORTF_AFSEL_R |= 0x00U;
	GPIO_PORTF_PDR_R |= 0x11U;
	
	// PF4, PF0 input, PF[3:1] output 
	GPIO_PORTF_DIR_R |= 0x0EU;
	
	
	GPIO_PORTF_DEN_R |= 0x1FU;
}