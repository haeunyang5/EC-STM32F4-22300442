/*----------------------------------------------------------------\
@ Embedded Controller by Young-Keun Kim - Handong Global University
Author           : Haeun Yang
Created          : 05-03-2021
Modified         : 09-17-2026 
Language/ver     : C++ in VS Code

Description      : Tutorial 7 segment
/----------------------------------------------------------------*/

#include "ecSTM32F4v2.h"

void setup(void);
	
int main(void) {	
	// Initialiization --------------------------------------------------------
	setup();
	
	// Inifinite Loop ----------------------------------------------------------
	while(1){
		// LOW -> 켜짐
		GPIO_write(PA_5, HIGH); //왼쪽 아래 (e)
		GPIO_write(PA_6, HIGH);  // 바닥 아래 (d)
		GPIO_write(PA_7, LOW);  // 오른쪽 아래 (c)
		GPIO_write(PB_6, HIGH); // 점 (DP)
		GPIO_write(PC_7, HIGH);  // 가운데 (g)
		GPIO_write(PA_9, HIGH);  // 왼쪽 위 (f)
		GPIO_write(PA_8, LOW);   // 가운데 위 (a)
		GPIO_write(PB_10, LOW);   // 오른쪽 위 (b)
	}
}

void setup(void){
	RCC_HSI_init();
	GPIO_init(PA_5, OUTPUT);
	GPIO_init(PA_6, OUTPUT);
	GPIO_init(PA_7, OUTPUT);
	GPIO_init(PB_6, OUTPUT);
	GPIO_init(PC_7, OUTPUT);
	GPIO_init(PA_9, OUTPUT);
	GPIO_init(PA_8, OUTPUT);
	GPIO_init(PB_10, OUTPUT);
}