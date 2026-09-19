/*----------------------------------------------------------------\
@ Embedded Controller by Young-Keun Kim - Handong Global University
Author           : Haeun Yang
Created          : 05-03-2021
Modified         : 09-14-2026
Language/ver     : C++ in VS Code

Description      : Toggle multi LEDs with a button
/----------------------------------------------------------------*/

#include "ecRCC2.h"
#include "ecGPIO2.h"

#define LED_0 PB_12
#define LED_1 PB_13
#define LED_2 PB_14
#define LED_3 PB_15

#define BUTTON_PIN PA_4

// Initialization
void LEDsetup(LED){
    GPIO_init(LED, OUTPUT) ;   // Initialze the LED pin as an output
    GPIO_ospeed(LED, MEDIUM_SPEED) ;
    GPIO_pupd(LED, PU) ;
    GPIO_otype(LED, OPEN_DRAIN) ;
}
void setup(void){
    RCC_HSI_init();
    GPIO_init(BUTTON_PIN, INPUT) ;  // Initialize the pushbutton pin as an input:
    GPIO_pupd(BUTTON_PIN, PU) ;
    
    LEDsetup(LED_0) ;
    LEDsetup(LED_1) ;
    LEDsetup(LED_2) ;
    LEDsetup(LED_3) ;
}

int main(void){
    setup();
    int buttonState = 0 ;

    while(1){
        // check if the pushbutton is pressed. 
        // change like: LED0 -> LED1 -> LED2 -> LED3 -> LED0
        buttonState = GPIO_read(BUTTON_PIN) ;
        for (int i=0; i<4; i++){
            if(buttonState == 1) GPIO_write(LED_0, HIGH) ;
            if(buttonState == 1) GPIO_write(LED_1, HIGH) ;
            if(buttonState == 1) GPIO_write(LED_2, HIGH) ;
            if(buttonState == 1) GPIO_write(LED_3, HIGH) ;
        }
        if(buttonState == 1) GPIO_write(LED_0, HIGH) ;
        else  GPIO_write(LED_PIN, HIGH) ;
        delay_ms_HSI(100) ;
    }
}