#include "stm32f411xe.h"
#include "ecRCC2.h"
#include "ecGPIO2.h"
#include "ecSysTick2.h"  

#define MOTOR_DIR PB_3  
#define MOTOR_PULSE PA_10

void setup(void)
{
	RCC_PLL_init();                
    SysTick_init(); 
    GPIO_init(MOTOR_DIR, OUTPUT);      
    GPIO_init(MOTOR_PULSE, OUTPUT);      
}


int main(void) {	    
	setup();
    // Set Dir high (CW)
    GPIO_write(MOTOR_DIR, HIGH);     
	while(1){
          for(int x = 0; x < 1000; x++) 
		  {
            GPIO_write(MOTOR_PULSE, HIGH);        
        	delay_ms(1);
	        GPIO_write(MOTOR_PULSE, LOW);        
	        delay_ms(1);
          }
        delay_ms(1000);
	}
}

