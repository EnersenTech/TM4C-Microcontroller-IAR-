// 0. Documentation Section

// 1. Pre-processor Directives Section
#include  "tm4c123gh6pm.h"

// 2. Global Declarations Section 
void DisableInterrupts(void); // Disable interrupts
void EnableInterrupts(void); // Enable interrupts 



// 3. Subroutines Section 


/*
1. Make the LED flash at 10 Hz. In other words, make it turn on for 0.05 sec, and then turn off for 0.05 sec. 
*/

void PortF_Init(void){
	volatile unsigned long delay; // 0) System Clock Gate Open Delay 
	SYSCTL_RCGCGPIO_R |= (0x01U << 5); // 1) Activate clock for PortF_Init F 
	delay = SYSCTL_RCGCGPIO_R; // 2) allow time for clock to start 
	GPIO_PORTF_LOCK_R = 0x4C4F434BU; // 3) Unlock GPIO Port F  enables write access to the GPIOCR register = Commit
	GPIO_PORTF_CR_R = 0x1FU; // 4) allow changes to PF4-0 
	//  Only PF0 needs to be unlocked, other bits can't be locked. PF0 is special consideration in GPIOPCTL
	GPIO_PORTF_AMSEL_R = 0x00U; // 5) Disable analog on PF 
	GPIO_PORTF_PCTL_R = 0x00U; // 6) PCTL GPIO on PF4-0 No Alternatve Function used
	GPIO_PORTF_DIR_R = 0x0EU; // 7) PF4, PF0 in , PF[3:1] out 
	GPIO_PORTF_AFSEL_R = 0x00U; // 8) Disable alt func on PF[7:0]
	GPIO_PORTF_PUR_R = 0x11U; // 9) Enable pull-up on PF0 and PF4 
	GPIO_PORTF_DEN_R = 0x1F; // 10) Enable digital I/O on PF[4:0]
}

// Initialize SysTick with busy wait running at bus clock
void SysTick_Init(void){
	NVIC_ST_CTRL_R = 0; // Disable SysTick during setup
	NVIC_ST_RELOAD_R = 0x00FFFFFFU; // Max reload value
	NVIC_ST_CURRENT_R = 0; // Any write to current clears it
	NVIC_ST_CTRL_R = 0x05U; // Enable SysTick with core clock
}


void Delay(void){
	unsigned long volatile time;
	time = 80000; // 0.05 sec 
	while(time--);
}


int main(void){
	// First data point is wrong, the other 24 will be correct 
	unsigned long Time[25];
	// You must leave the Data array defined exactly as it is 
	unsigned long Data[25];

	unsigned long LED;
	unsigned long i,last,now; 
	PortF_Init(); // Initialize PF1 to output
	SysTick_Init(); // Initialize SysTick, runs at 16 MHz 
	i = 0; // Array index; 
	last = NVIC_ST_CURRENT_R;
	// EnableInterrupts(); // Enable interrupts
	while(1){
		LED = GPIO_PORTF_DATA_R; // Read previous 
		LED = LED^0x02U; // Toggle red LED 
		GPIO_PORTF_DATA_R = LED; // Output 
		if(i<25){
			now = NVIC_ST_CURRENT_R; 
			Time[i] = (last-now)&0x00FFFFFF; // 24-bit time difference
			Data[i] = GPIO_PORTF_DATA_R&0x02U; // Record PF1
			last = now; 
			i++;
		}
		Delay();
	}
}
