/*----------------------------------------------------------------\
@ Embedded Controller by Young-Keun Kim - Handong Global University
Author           : Haeun Yang
Created          : 05-03-2021
Modified         : 09-20-2026
Language/ver     : C++ in VS Code

Description      : Toggle multi LEDs with a button
/----------------------------------------------------------------*/

#include "ecRCC2.h"
#include "ecGPIO2.h"

#define LED_0 PB_12
#define LED_1 PB_13
#define LED_2 PB_14
#define LED_3 PB_15

#define BUTTON_PIN_ PA_4

// Initialization
void setup(void) ;
void LEDsetup(uint32_t LED) ; 


int main(void){
    setup() ;
    int buttonState = 1 ;
    int prevButtonState = 1 ; 
    int flag = 0 ;

    while(1){
        // check if the pushbutton is pressed. 
        // change like: LED0 -> LED1 -> LED2 -> LED3 -> LED0 (Active high)
        buttonState = GPIO_read(BUTTON_PIN_) ;
        if (buttonState == 0 && prevButtonState == 1) {
            if (flag == 0){
                GPIO_write(LED_0, HIGH) ;  // BUTTON -> Active Low(0 일 때가 눌렸을 때)
                GPIO_write(LED_1, LOW) ;
                GPIO_write(LED_2, LOW) ;
                GPIO_write(LED_3, LOW) ;
                flag = 1 ;
            }
            else if (flag == 1){
                GPIO_write(LED_0, LOW) ;  // BUTTON -> Active Low(0 일 때가 눌렸을 때)
                GPIO_write(LED_1, HIGH) ;
                GPIO_write(LED_2, LOW) ;
                GPIO_write(LED_3, LOW) ;
                flag = 2 ;
            }
            else if (flag == 2){
                GPIO_write(LED_0, LOW) ;  // BUTTON -> Active Low(0 일 때가 눌렸을 때)
                GPIO_write(LED_1, LOW) ;
                GPIO_write(LED_2, HIGH) ;
                GPIO_write(LED_3, LOW) ;
                flag = 3 ;
            }
            else if (flag == 3){
                GPIO_write(LED_0, LOW) ;  // BUTTON -> Active Low(0 일 때가 눌렸을 때)
                GPIO_write(LED_1, LOW) ;
                GPIO_write(LED_2, LOW) ;
                GPIO_write(LED_3, HIGH) ;
                flag = 0 ;
            }
        }
        prevButtonState = buttonState ;
        delay_ms_HSI(100) ;
    }
}

void setup(void){
    RCC_HSI_init();
    GPIO_init(BUTTON_PIN_, INPUT) ;  // Initialize the pushbutton pin as an input:
    GPIO_pupd(BUTTON_PIN_, PU) ;
    
    LEDsetup(LED_0) ;
    LEDsetup(LED_1) ;
    LEDsetup(LED_2) ;
    LEDsetup(LED_3) ;
}

void LEDsetup(uint32_t  LED){
    GPIO_init(LED, OUTPUT) ;   // Initialze the LED pin as an output
    GPIO_pupd(LED, PU) ;
    GPIO_otype(LED, PUSH_PULL) ;
    GPIO_ospeed(LED, MEDIUM_SPEED) ;
}