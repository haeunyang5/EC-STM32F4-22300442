# EC-STM32F4-22300442

repository for Embedded Controller, HGU

Documentation for HAL functions

Written by: Haeun Yang

Course: Embedded Controller

Program: C/C++

IDE/Compiler: VS Code (PlatformIO)

OS: Win10

MCU: STM32F411RE (Nucleo-64)



# GPIO Digital In/Out

### Header File

```c
#include "ecGPIO2.h"
```

```c
#ifndef __ECGPIO2_H
#define __ECGPIO2_H

#include "stm32f411xe.h"
#include "ecRCC2.h"
#include "ecPinNames.h"

// MODER
#define INPUT  0x00
#define OUTPUT 0x01
#define AF     0x02
#define ANALOG 0x03

// IDR & ODR
#define HIGH 1
#define LOW  0

// OSPEED
#define LOW_SPEED    0x00
#define MEDIUM_SPEED 0x01
#define FAST_SPEED   0x02
#define HIGH_SPEED   0x03

// OTYPER
#define PUSH_PULL  0
#define OPEN_DRAIN 1

// PUDR
#define NO_PUPD  0x00
#define PU       0x01
#define PD       0x02
#define RESERVED 0x03

#define FND_NUM        (int)(       4)
#ifdef __cplusplus
 extern "C" {
#endif /* __cplusplus */
	 
void GPIO_init(PinName_t pinName, uint32_t mode);     
void GPIO_mode(PinName_t pinName, uint32_t mode);
void GPIO_ospeed(PinName_t pinName, uint32_t speed);
void GPIO_otype(PinName_t pinName, uint32_t type);
void GPIO_pupd(PinName_t pinName, uint32_t pupd);
void GPIO_write(PinName_t pinName, uint32_t Output);
uint32_t  GPIO_read(PinName_t pinName);

// ARDUINO Syntax  (Optional)
void pinMode(PinName_t pin, uint32_t mode);
void digitalWrite(PinName_t pin, uint32_t value);
uint32_t  digitalRead(PinName_t pin);

// FND for JKIT board 
extern int numberFND[12][8];
void FND_init(PinName_t *pinFND) ;
void FND_select_init(PinName_t *selectFND) ;
void FND_display(PinName_t *pinFND, uint8_t num) ;
void FND_select(uint8_t digit) ;

void led_toggle(PinName_t pinName) ;
#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif // __ECGPIO2_H

```

### **GPIO_init()**

Enables the clock of the port that the given pin belongs to, and sets the GPIO mode of the pin.

```c
void GPIO_init(PinName_t pinName, uint32_t mode)
```

#### Parameters

- `pinName`: Pin to initialize (e.g., `PA_5`)
- `mode`: GPIO mode. `INPUT`, `OUTPUT`, `AlterFunc`, or `Analog`

```c
void GPIO_init(PinName_t pinName, uint32_t mode){     
	GPIO_TypeDef * Port;
	unsigned int pin;
	ecPinmap(pinName, &Port, &pin);

	if (Port == GPIOA)
		RCC_GPIOA_enable();
	if (Port == GPIOB)
		RCC_GPIOB_enable();
	if (Port == GPIOC)
		RCC_GPIOC_enable();
	if (Port == GPIOD)
		RCC_GPIOD_enable();
	if (Port == GPIOE)
		RCC_GPIOE_enable();
	if (Port == GPIOH)
		RCC_GPIOH_enable();

	GPIO_mode(pinName, mode);
}
```

#### Example code

```c
GPIO_init(PA_5, OUTPUT);
```

### **GPIO_write()**

Writes a HIGH or LOW value to the given output pin.

```c
void GPIO_write(PinName_t pinName, uint32_t Output)
```

#### Parameters

- `pinName`: Pin to write to
- `Output`: Output value. `0` (LOW) or `1` (HIGH)

```c
void GPIO_write(PinName_t pinName, uint32_t Output){
	GPIO_TypeDef *Port;
	unsigned int pin;
	ecPinmap(pinName, &Port, &pin);

	Port->ODR &= ~(1UL<<pin) ;
	Port->ODR |= (Output<<pin) ;
}
```

#### Example code

```c
GPIO_write(PA_5, HIGH);   // set PA_5 to HIGH
GPIO_write(PA_5, LOW);    // set PA_5 to LOW
```

### **GPIO_read()**

Reads the current input level (0 or 1) of the given pin.

```c
uint32_t GPIO_read(PinName_t pinName)
```

#### Parameters

- `pinName`: Pin to read

```c
uint32_t GPIO_read(PinName_t pinName){
	GPIO_TypeDef * Port;
	unsigned int pin;
	ecPinmap(pinName,&Port,&pin);

	unsigned int bit;
	bit = (Port->IDR>>pin) & 1 ;
	return bit; 
}
```

#### Example code

```c
uint32_t state = GPIO_read(BUTTON_PIN);
```

### **GPIO_mode()**

Sets the operating mode of the given pin (input, output, alternate function, or analog).

```c
void GPIO_mode(PinName_t pinName, uint32_t mode)
```

#### Parameters

- `pinName`: Pin to configure
- `mode`: `00` = Input, `01` = Output, `10` = Alternate function, `11` = Analog

```c
// GPIO Mode : Input(00), Output(01), AlterFunc(10), Analog(11)
void GPIO_mode(PinName_t pinName, uint32_t mode){
   GPIO_TypeDef * Port;
   unsigned int pin;
   ecPinmap(pinName,&Port,&pin);

   Port->MODER &= ~(3UL<<(2*pin));     // Clear '00' 
   Port->MODER |= mode<<(2*pin);       // Set mode
}
```

#### Example code

```c
GPIO_mode(PA_5, OUTPUT);
```

### **GPIO_ospeed()**

Sets the output speed (slew rate) of the given pin.

```c
void GPIO_ospeed(PinName_t pinName, uint32_t speed)
```

#### Parameters

- `pinName`: Pin to configure
- `speed`: `00` = Low, `01` = Medium, `10` = Fast, `11` = High speed

```c
// GPIO Speed : Low speed (00), Medium speed (01), Fast speed (10), High speed (11)
void GPIO_ospeed(PinName_t pinName, uint32_t speed){
	GPIO_TypeDef * Port;
	unsigned int pin;
	ecPinmap(pinName,&Port,&pin);

	Port->OSPEEDR &= ~(3UL<<(2*pin)) ;    // Clear '00' 
	Port->OSPEEDR |= speed<<(2*pin) ;
}
```

#### Example code

```c
GPIO_ospeed(PA_5, MEDIUM_SPEED);
```

### **GPIO_otype()**

Sets the output type of the given pin (push-pull or open-drain).

```c
void GPIO_otype(PinName_t pinName, uint32_t type)
```

#### Parameters

- `pinName`: Pin to configure
- `type`: `0` = Push-pull, `1` = Open-drain

```c
// GPIO Output Type: Output push-pull (0, reset), Output open drain (1)
void GPIO_otype(PinName_t pinName, uint32_t type){
	GPIO_TypeDef * Port;
	unsigned int pin;
	ecPinmap(pinName,&Port,&pin);

	Port->OTYPER &= ~(1UL<<(pin)) ;    // Clear '0' 
	Port->OTYPER |= (type<<pin) ;
}
```

#### Example code

```c
GPIO_otype(PA_5, PUSH_PULL);
```

### **GPIO_pupd()**

Sets the internal pull-up / pull-down resistor of the given pin.

```c
void GPIO_pupd(PinName_t pinName, uint32_t pupd)
```

#### Parameters

- `pinName`: Pin to configure
- `pupd`: `00` = No pull-up/pull-down, `01` = Pull-up, `10` = Pull-down, `11` = Reserved

```c
// GPIO Pull-up/Pull-down : No pull-up, pull-down (00), Pull-up (01), Pull-down (10), Reserved (11)
void GPIO_pupd(PinName_t pinName, uint32_t pupd){
	GPIO_TypeDef * Port;
	unsigned int pin;
	ecPinmap(pinName,&Port,&pin);

	Port->PUPDR &= ~(3UL<<(2*pin)) ;    // Clear '00' 
	Port->PUPDR |= pupd<<(2*pin) ;
}
```

#### Example code

```c
GPIO_pupd(PA_5, PU);
```

---



# GPIO 7segment

### Header File

```c
#include "ecGPIO2.h"
```

```c
#ifndef __ECGPIO2_H
#define __ECGPIO2_H

#include "stm32f411xe.h"
#include "ecRCC2.h"
#include "ecPinNames.h"

// MODER
#define INPUT  0x00
#define OUTPUT 0x01
#define AF     0x02
#define ANALOG 0x03

// IDR & ODR
#define HIGH 1
#define LOW  0

// OSPEED
#define LOW_SPEED    0x00
#define MEDIUM_SPEED 0x01
#define FAST_SPEED   0x02
#define HIGH_SPEED   0x03

// OTYPER
#define PUSH_PULL  0
#define OPEN_DRAIN 1

// PUDR
#define NO_PUPD  0x00
#define PU       0x01
#define PD       0x02
#define RESERVED 0x03

#define FND_NUM        (int)(       4)
#ifdef __cplusplus
 extern "C" {
#endif /* __cplusplus */
	 
void GPIO_init(PinName_t pinName, uint32_t mode);     
void GPIO_mode(PinName_t pinName, uint32_t mode);
void GPIO_ospeed(PinName_t pinName, uint32_t speed);
void GPIO_otype(PinName_t pinName, uint32_t type);
void GPIO_pupd(PinName_t pinName, uint32_t pupd);
void GPIO_write(PinName_t pinName, uint32_t Output);
uint32_t  GPIO_read(PinName_t pinName);

// ARDUINO Syntax  (Optional)
void pinMode(PinName_t pin, uint32_t mode);
void digitalWrite(PinName_t pin, uint32_t value);
uint32_t  digitalRead(PinName_t pin);

// FND for JKIT board 
extern int numberFND[12][8];
void FND_init(PinName_t *pinFND) ;
void FND_select_init(PinName_t *selectFND) ;
void FND_display(PinName_t *pinFND, uint8_t num) ;
void FND_select(uint8_t digit) ;

void led_toggle(PinName_t pinName) ;
#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif // __ECGPIO2_H

```

### **FND_init()**

Initializes the segment pins (a–g, dp) of the 7-segment display as digital outputs.

```c
void FND_init(PinName_t *pinFND)
```

#### Parameters

- `pinFND`: Array of the 8 segment pins (e.g., `{PB_0, PB_1, ..., PB_7}`)

#### Example code

```c
PinName_t pinFND[8] = {PB_0, PB_1, PB_2, PB_3, PB_4, PB_5, PB_6, PB_7};
FND_init(pinFND);
```

### **FND_select_init()**

Initializes the four digit-select pins (FND0–FND3) as digital outputs.

```c
void FND_select_init(PinName_t *selectFND)
```

#### Parameters

- `selectFND`: Array of the 4 digit-select pins (e.g., `{PA_10, PA_11, PC_4, PC_3}`)

#### Example code

```c
PinName_t selectFND[4] = {PA_10, PA_11, PC_4, PC_3};
FND_select_init(selectFND);
```

### **FND_display()**

Converts a single digit from 0 to 9 into its segment pattern and displays it on the 7-segment display.

```c
void FND_display(PinName_t *pinFND, uint8_t num)
```

#### Parameters

- `pinFND`: Array of the 8 segment pins
- `num`: Digit to display (valid range: 0–9)

#### Example code

```c
FND_display(pinFND, 7);   // display '7'
```

### **FND_select()**

Selects and activates one of the four digits (FND0–FND3).

```c
void FND_select(uint8_t digit)
```

#### Parameters

- `digit`: Index of the digit to activate (0–3)

#### Example code

```c
FND_select(2);   // activate the third digit (FND2)
```

---



# EXTI

### Header File

```c
#include "ecEXTI2.h"
```

```c
#ifndef __EC_EXTI2_H
#define __EC_EXTI2_H

#include "stm32f411xe.h"
#include "ecPinNames.h"

#define FALL 0
#define RISE 1
#define BOTH 2

#ifdef __cplusplus
 extern "C" {
#endif /* __cplusplus */

void EXTI_init(PinName_t pinName, uint32_t trig_type,uint32_t priority);
void EXTI_enable(PinName_t pinName);
void EXTI_disable(PinName_t pinName);
uint32_t is_pending_EXTI(PinName_t pinName);
void clear_pending_EXTI(PinName_t pinName);

#ifdef __cplusplus
}
#endif /* __cplusplus */
	 
#endif // __EC_EXTI2_H

```

### **EXTI_init()**

Function to connect the given pin to an external interrupt (EXTI) line, select the trigger edge, and enable the interrupt in the NVIC.

```c
void EXTI_init(PinName_t pinName, uint32_t trig_type, uint32_t priority){
	GPIO_TypeDef  *Port;
	unsigned int pin;
	ecPinmap(pinName,&Port,&pin);

	// Enable EXTI Clock with SYSCFG controller clock
	RCC->APB2ENR |= RCC_APB2ENR_SYSCFGEN;

	// Connect EXTI to the GPIO Port_y Pin_x
	uint32_t EXTICR_port = 0;
	if      (Port == GPIOA) EXTICR_port = 0 ;
	else if (Port == GPIOB) EXTICR_port = 1 ;
	else if (Port == GPIOC) EXTICR_port = 2 ;
	else if (Port == GPIOD) EXTICR_port = 3 ;
	else                    EXTICR_port = 4 ;

	SYSCFG->EXTICR[pin / 4] &= ~(0xF<<((pin % 4) * 4)) ;          // clear 4 bits
	SYSCFG->EXTICR[pin / 4] |= (EXTICR_port<<((pin % 4) * 4)) ;   // set 4 bits

	// Select Trigger edge (RISE, FALL, BOTH)
	if (trig_type == FALL)      EXTI->FTSR |= 1UL << pin ;
	else if (trig_type == RISE) EXTI->RTSR |= 1UL << pin ;
	else if (trig_type == BOTH) {
		EXTI->RTSR |= 1UL << pin ;
		EXTI->FTSR |= 1UL << pin ;
	}

	// Not-mask (Enable) EXTIx
	EXTI->IMR |= 1UL << pin;

	// NVIC(IRQ) Setting
	uint32_t EXTI_IRQn = 0;
	if (pin < 5)       EXTI_IRQn = (6 + pin);
	else if (pin < 10) EXTI_IRQn = EXTI9_5_IRQn;
	else               EXTI_IRQn = EXTI15_10_IRQn;

	NVIC_SetPriority(EXTI_IRQn, priority);
	NVIC_EnableIRQ(EXTI_IRQn);
}
```

#### Parameters

- `PinName_t pinName` : Pin to use as the external interrupt source
- `uint32_t trig_type` : Trigger edge (`FALL`, `RISE`, `BOTH`)
- `uint32_t priority` : NVIC interrupt priority (0 = highest)

#### Example code

```c
EXTI_init(PC_13, FALL, 0);   // Button B1, falling edge, priority 0
```

---

### **EXTI_enable()**

Function to enable (unmask) the external interrupt of the given pin.

```c
void EXTI_enable(PinName_t pinName) {
	GPIO_TypeDef  *Port;
	unsigned int pin;
	ecPinmap(pinName,&Port,&pin);

	EXTI->IMR |= 1 << pin;     // not masked (i.e., Interrupt enabled)
}
```

#### Parameters

- `PinName_t pinName` : Pin whose interrupt is enabled

#### Example code

```c
EXTI_enable(PC_13);
```

---

### **EXTI_disable()**

Function to disable (mask) the external interrupt of the given pin.

```c
void EXTI_disable(PinName_t pinName) {
	GPIO_TypeDef  *Port;
	unsigned int pin;
	ecPinmap(pinName,&Port,&pin);

	EXTI->IMR &= ~(1 << pin);  // masked (i.e., Interrupt disabled)
}
```

#### Parameters

- `PinName_t pinName` : Pin whose interrupt is disabled

#### Example code

```c
EXTI_disable(PC_13);
```

---

### **is_pending_EXTI()**

Function to check whether an interrupt has occurred on the given pin by reading its pending bit.

```c
uint32_t is_pending_EXTI(PinName_t pinName) {
	GPIO_TypeDef  *Port;
	unsigned int pin;
	ecPinmap(pinName,&Port,&pin);

	uint32_t EXTI_PRx = (EXTI->PR>>pin & 1);   // read PRx, EXTI pending bit
	return EXTI_PRx;                           // Return only 0 or 1
}
```

#### Parameters

- `PinName_t pinName` : Pin whose pending bit is checked

#### Example code

```c
if (is_pending_EXTI(PC_13)) {
	// interrupt occurred
}
```

---

### **clear_pending_EXTI()**

Function to clear the pending bit of the given pin by writing '1'. It is called at the end of the interrupt handler.

```c
void clear_pending_EXTI(PinName_t pinName){
	GPIO_TypeDef *Port;
	unsigned int pin;
	ecPinmap(pinName, &Port, &pin);

	EXTI->PR |= (1 << pin);    // clear by writing 1 to the pending bit
}
```

#### Parameters

- `PinName_t pinName` : Pin whose pending bit is cleared

#### Example code

```c
void EXTI15_10_IRQHandler(void) {
	if (is_pending_EXTI(PC_13)) {
		stepper_stop();
		clear_pending_EXTI(PC_13);
	}
}
```



# Stepper Motor

### Header File

```c
ecStepper.h
```

```c
#include "stm32f411xe.h"
#include "ecPinNames.h"
#include "ecRCC2.h"
#include "ecGPIO2.h"
#include "ecSysTick2.h"
			
#ifndef __EC_STEPPER2_H
#define __EC_STEPPER2_H

#ifdef __cplusplus
 extern "C" {
#endif /* __cplusplus */

// Stepper Mode
#define HALF 0
#define FULL 1	 
#define DIR_CW  1
#define DIR_CCW 0


//State number 
typedef enum {
	S0, S1, S2, S3, S4, S5, S6, S7
} StateNum;

// FULL stepping sequence  - FSM
typedef struct {	
  uint32_t next[2];
  uint8_t out[4];
} StateStepper_t;

// Initialization of Stepper Motor
void stepper_init(uint32_t mode, PinName_t *pinStepper);

// Return step per revolution, always return 1 rpm
uint32_t stepper_stepPerRev(void) ;

// Converts Motor [rpm] to step delay in [msec]
void stepper_speed (uint32_t speedRPM);

// Run Stepper Motor for given steps and direction
void stepper_step(uint32_t steps, uint32_t direction) ;

// Stepper Motor Output for given state
void stepper_out(uint32_t state);

// Stop Stepper Motor
void stepper_stop(void);


#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif // __EC_STEPPER2_H

```

### **stepper_init()**

Selects the FSM table and the number of steps per revolution according to the drive mode (Full/Half), and initializes the four output pins (A, B, A', B') as GPIO outputs.

```c
void stepper_init(uint32_t mode, PinName_t *pinStepper)
```

#### Parameters

- `mode`: Drive mode. `FULL` or `HALF`
- `pinStepper`: Array of pins connected in the order A, B, A', B'

```c
void stepper_init(uint32_t mode, PinName_t *pinStepper){
	if (mode == HALF) {
		fsm = fsm_Half;
		STEP_PER_REV = 4096 ;
	}
	else{
		fsm = fsm_Full;
		STEP_PER_REV = 2048 ;
	}

	for (int i=0; i<4; i++){
		stepperPins[i] = pinStepper[i];
		GPIO_init(stepperPins[i], OUTPUT) ;
		GPIO_ospeed(stepperPins[i], FAST_SPEED) ;
	} 
}
```

#### Example code

```c
PinName_t pinStepper[4] = {PB_10, PB_4, PB_5, PB_3};   // A, B, A', B'
stepper_init(FULL, pinStepper);
```

### **stepper_stepPerRev()**

Returns the number of steps required for one revolution of the output shaft in the current mode. (Full: 2048, Half: 4096)

```c
uint32_t stepper_stepPerRev(void)
```

#### Parameters

- None

```c
uint32_t stepper_stepPerRev(void) {
	return STEP_PER_REV ;
}
```

#### Example code

```c
uint32_t rev   = 1;
uint32_t steps = stepper_stepPerRev() * rev;
```

### **stepper_speed()**

Converts the motor speed [rpm] into the delay time per step [ms] and sets it.

```c
void stepper_speed(uint32_t speedRPM)
```

#### Parameters

- `speedRPM`: Target rotation speed [rpm]

```c
void stepper_speed (uint32_t speedRPM){
	_step_delay = 60*1000/(STEP_PER_REV*speedRPM) ;   // [msec]
}
```

#### Example code

```c
stepper_speed(2);   // 2 rpm
```

### **stepper_step()**

Rotates the motor by the given number of steps in the given direction. At each step, the next state is determined from the present state and the direction, the output of that state is applied, and then the function waits for `_step_delay`.

```c
void stepper_step(uint32_t steps, uint32_t direction)
```

#### Parameters

- `steps`: Number of steps to rotate
- `direction`: Rotation direction. `DIR_CW` (clockwise) or `DIR_CCW` (counterclockwise)

```c
void stepper_step(uint32_t steps, uint32_t direction){
	if (fsm == 0) return ;
	_steps = steps ;

	for (uint32_t i = 0; i < _steps; i++){
		if (direction == DIR_CCW) state = fsm[state].next[0] ; 
		else state = fsm[state].next[1] ;

		stepper_out(state) ;
		delay_ms(_step_delay) ;
	}
}
```

#### Example code

```c
stepper_step(stepper_stepPerRev(), DIR_CW);   // one revolution, clockwise
```

### **stepper_out()**

Reads the output values of the given state from the FSM table and writes them to the A, B, A', B' pins.

```c
void stepper_out(uint32_t state)
```

#### Parameters

- `state`: FSM state number to output (Full: S0–S3, Half: S0–S7)

```c
void stepper_out(uint32_t state){
	for (int i = 0; i < 4; i++) {
		GPIO_write(stepperPins[i], fsm[state].out[i]);
	}
}
```

#### Example code

```c
stepper_out(S0);   // Full: A=1, B=1, A'=0, B'=0
```

### **stepper_stop()**

Sets the remaining step count to 0 so that the rotation loop in `stepper_step()` ends, and sets all coil outputs to LOW to stop the motor.

```c
void stepper_stop(void)
```

#### Parameters

- None

```c
void stepper_stop(void) {
    _steps = 0;    
	for (int i=0; i<4; i++) GPIO_write(stepperPins[i], 0) ;
}
```

#### Example code

```c
void EXTI15_10_IRQHandler(void) {  
	if (is_pending_EXTI(BUTTON_PIN)) {
		stepper_stop();
		clear_pending_EXTI(BUTTON_PIN);
	}
}
```
# USART

### Header File

```c
ecUART2.h
```

```c

#ifndef __EC_USART2_STUDENT_H
#define __EC_USART2_STUDENT_H

#include <stdio.h>
#include <math.h>
#include "stm32f411xe.h"
#include "ecGPIO2.h"
#include "ecRCC2.h"


#define POL 0
#define INT 1

// You can modify this
#define BAUD_9600	9600
#define BAUD_19200	19200
#define BAUD_38400  38400
#define BAUD_57600	57600
#define BAUD_115200 115200
#define BAUD_921600 921600


// ********************** USART 2 (USB) ***************************
	// PA_2 = USART2_TX
	// PA_3 = USART2_RX
	// Alternate function(AF7), High Speed, Push pull, Pull up
	// APB1
// **********************************************************

// ********************** USART 1 ***************************
	// PA_9 = USART1_TX (default)	// PB_6  (option)
	// PA_10 = USART1_RX (default)	// PB_3 (option)
	// APB2
// **********************************************************

// ********************** USART 6 ***************************
	// PA_11 = USART6_TX (default)	// PC_6  (option)
	// PA_12 = USART6_RX (default)	// PC_7 (option)
	// APB2
// **********************************************************


/*================ EXERCISE ================*/
/*================ General Configuration ================*/
// General configuration
void USART_init(USART_TypeDef* USARTx, PinName_t pin_GPIO_TX, PinName_t pin_GPIO_RX, uint32_t baud);	// copy of USART_setting
void USART_baud(USART_TypeDef* USARTx, uint32_t baud);
void USART_pins(USART_TypeDef* USARTx, PinName_t pin_GPIO_TX, PinName_t pin_GPIO_RX);

void USART1_baud(uint32_t baud);
void USART2_baud(uint32_t baud);


/*================ PROVIDED  ================*/
/*================ Configuration for each USART ================*/
// Configuration UART 1, 2 using default settings
void USART1_init(void);
void USART2_init(void);

// USART write & read
void USART1_write(uint8_t* buffer, uint32_t nBytes);
void USART2_write(uint8_t* buffer, uint32_t nBytes);
uint8_t USART1_read(void);										
uint8_t USART2_read(void);	

// RX Inturrupt Flag USART1,2
uint32_t is_USART1_RXNE(void);
uint32_t is_USART2_RXNE(void);


/*================ General Configuration ================*/
void USART_clock_enable(USART_TypeDef* USARTx);
void USART_write(USART_TypeDef* USARTx, uint8_t* buffer, uint32_t nBytes);
uint8_t USART_read(USART_TypeDef * USARTx);										
uint32_t is_USART_RXNE(USART_TypeDef * USARTx);
void USART_delay(uint32_t us);  

								

#endif // __EC_USART2_STUDENT_H


```

### USART_init()

Initializes the selected USART: configures the TX/RX pins, enables the USART clock, sets the frame format (no parity, 8-bit data, 1 stop bit, oversampling x16) and baud rate, and enables the RXNE interrupt.

```c
void USART_init(USART_TypeDef* USARTx, PinName_t pin_GPIO_TX, PinName_t pin_GPIO_RX, uint32_t baud)
```

**Parameters**

- `USARTx` : USART peripheral (`USART1`, `USART2`, `USART6`)
- `pin_GPIO_TX` : TX pin (e.g., `PA_9`)
- `pin_GPIO_RX` : RX pin (e.g., `PA_10`)
- `baud` : Baud rate (e.g., `9600`)

**Example code**

```c
USART_init(USART1, PA_9, PA_10, 9600);   // USART1 : TX = PA9, RX = PA10, 9600 bps
```

### USART_baud()

Sets the baud rate of the selected USART by calculating USARTDIV from the APB clock and writing the mantissa and fraction to the BRR register.

```c
void USART_baud(USART_TypeDef* USARTx, uint32_t baud)
```

**Parameters**

- `USARTx` : USART peripheral (`USART1`, `USART2`, `USART6`)
- `baud` : Baud rate (e.g., `9600`, `38400`, `115200`)

**Example code**

```c
USART_baud(USART2, 38400);   // change USART2 baud rate to 38400 bps
```

### USART_pins()

Configures the TX/RX pins for USART: alternate function mode, push-pull, no pull-up/pull-down, high speed, and sets the AF number (AF7 for USART1/2, AF8 for USART6).

```c
void USART_pins(USART_TypeDef* USARTx, PinName_t pin_GPIO_TX, PinName_t pin_GPIO_RX)
```

**Parameters**

- `USARTx` : USART peripheral (`USART1`, `USART2`, `USART6`)
- `pin_GPIO_TX` : TX pin (e.g., `PA_2`)
- `pin_GPIO_RX` : RX pin (e.g., `PA_3`)

**Example code**

```c
USART_pins(USART2, PA_2, PA_3);   // PA2 = USART2_TX, PA3 = USART2_RX (AF7)
```

### USART1_baud()

Sets the baud rate of USART1. Calls `USART_baud(USART1, baud)`.

```c
void USART1_baud(uint32_t baud)
```

**Parameters**

- `baud` : Baud rate (e.g., `9600`)

**Example code**

```c
USART1_baud(9600);   // USART1 baud rate = 9600 bps
```

### USART2_baud()

Sets the baud rate of USART2. Calls `USART_baud(USART2, baud)`.

```c
void USART2_baud(uint32_t baud)
```

**Parameters**

- `baud` : Baud rate (e.g., `9600`)

**Example code**

```c
USART2_baud(9600);   // USART2 baud rate = 9600 bps
```
