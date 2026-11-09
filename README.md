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

```c
void USART_init(USART_TypeDef* USARTx, PinName_t pin_GPIO_TX, PinName_t pin_GPIO_RX, uint32_t baud){
//1. GPIO Pin for TX and RX
	USART_pins(USARTx, pin_GPIO_TX, pin_GPIO_RX);

//2. USARTx (x=2,1,6) configuration
	USART_clock_enable(USARTx);

	// Disable USARTx.
	USARTx->CR1 &= ~USART_CR1_UE; 	// USART disable

	// No Parity / 8-bit word length / Oversampling x16 / Stop bit =1
	USARTx->CR1 &= ~USART_CR1_PCE;    
	USARTx->CR1 &= ~USART_CR1_M;      
	USARTx->CR1 &= ~USART_CR1_OVER8;   
	USARTx->CR2 &= ~USART_CR2_STOP;    

	// Set Baudrate using APB frequency (42MHz)
	// If oversampling by 16, Tx/Rx baud = f_CK / (16*USARTDIV),
	// If oversampling by 8,  Tx/Rx baud = f_CK / (8*USARTDIV)
	USART_baud(USARTx, baud);

	// Enable TX, RX, and USARTx
	USARTx->CR1 |= (USART_CR1_RE | USART_CR1_TE);   	// Transmitter and Receiver enable
	USARTx->CR1 |= USART_CR1_UE; 						          // USART enable

// 3. RX interrupt (data ready to be read)
  // Enable RXNE interrupt
	USARTx->CR1 |= USART_CR1_RXNEIE;
  // USART_IRQn
	uint32_t USART_IRQ = USART2_IRQn;
  // Set the priority and enable interrupt 
	NVIC_SetPriority(USART_IRQ, 1);
	NVIC_EnableIRQ(USART_IRQ);

}
```

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

```c
void USART_baud(USART_TypeDef* USARTx, uint32_t baud){
	// Disable USARTx.
	USARTx->CR1  &= ~USART_CR1_UE; 						// USART disable
	USARTx->BRR = 0;

	// Configure Baud-rate
	float fCK = EC_SYSCLK;                                   // if(USARTx==USART1 || USARTx==USART6), APB2
	                                                         // (84MHz after RCC_PLL_init(), 16MHz after RCC_HSI_init())
	if(USARTx == USART2) fCK =fCK/2;      // APB1
    float USARTDIV = (float) fCK/16.0/baud;

	// Method 1 : build the two BRR fields explicitly
	//   BRR[15:4] = DIV_Mantissa (integer part of USARTDIV)
	//   BRR[3:0]  = DIV_Fraction (counts 1/16 steps -> multiply the fraction by 16)
	uint32_t mantissa = (uint32_t)USARTDIV;                            
	uint32_t fraction = (uint32_t)((USARTDIV - mantissa) * 16.0f + 0.5f); 
	USARTx->BRR = (mantissa << 4) | (fraction & 0xF);
  
	// Enable TX, RX, and USARTx
	USARTx->CR1 |= USART_CR1_UE;
}
```

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

```c
void USART_pins(USART_TypeDef* USARTx, PinName_t pin_GPIO_TX, PinName_t pin_GPIO_RX){
	GPIO_TypeDef *GPIO_TX;
	unsigned int pinTX;
	ecPinmap(pin_GPIO_TX, &GPIO_TX, &pinTX);

	GPIO_TypeDef *GPIO_RX;
	unsigned int pinRX;
	ecPinmap(pin_GPIO_RX, &GPIO_RX, &pinRX);

	// Alternate Function, Push-Pull, No Pull-up/down, High speed
	// TX
	GPIO_TX->MODER   = (GPIO_TX->MODER   & ~(3UL << (2*pinTX))) | (2UL << (2*pinTX));  // AF (10)
	GPIO_TX->OTYPER &= ~(1UL << pinTX);                                                 // Push-Pull (0)
	GPIO_TX->PUPDR  &= ~(3UL << (2*pinTX));                                             // No PUPD (00)
	GPIO_TX->OSPEEDR |=  (3UL << (2*pinTX));                                            // High speed (11)

	// RX
	GPIO_RX->MODER   = (GPIO_RX->MODER   & ~(3UL << (2*pinRX))) | (2UL << (2*pinRX));
	GPIO_RX->OTYPER &= ~(1UL << pinRX);
	GPIO_RX->PUPDR  &= ~(3UL << (2*pinRX));
	GPIO_RX->OSPEEDR |=  (3UL << (2*pinRX));

	// Set Alternate Function Register for USARTx
	//   AF7 - USART1, USART2
	//   AF8 - USART6
	uint32_t AF_num = (USARTx == USART6) ? 8 : 7;
	if (pinTX < 8) { GPIO_TX->AFR[0] &= ~(0xFUL << (4*pinTX));     GPIO_TX->AFR[0] |= AF_num << (4*pinTX); }
	else           { GPIO_TX->AFR[1] &= ~(0xFUL << (4*(pinTX-8))); GPIO_TX->AFR[1] |= AF_num << (4*(pinTX-8)); }
	
  if (pinRX < 8) {
	GPIO_RX->AFR[0] &= ~(0xFUL << (4*pinRX));         
	GPIO_RX->AFR[0] |= AF_num << (4*pinRX);           
  }
  else{
	GPIO_RX->AFR[1] &= ~(0xFUL << (4*(pinRX-8)));
	GPIO_RX->AFR[1] |= AF_num << (4*(pinRX-8));
  }

}
```

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

```c
void USART1_baud(uint32_t baud){
	USART_baud(USART1, baud);
}
```

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

```c
void USART2_baud(uint32_t baud){
	USART_baud(USART2, baud);
}
```

**Example code**

```c
USART2_baud(9600);   // USART2 baud rate = 9600 bps
```

### Provided functions (used in LAB_USART)

```c
void USART1_init(void);      // USART1: PA_9 (TX), PA_10 (RX), 9600 baud, RXNE interrupt enabled
void USART2_init(void);      // USART2: PA_2 (TX), PA_3 (RX), 9600 baud, RXNE interrupt enabled (ST-Link USB)

void USART1_write(uint8_t* buffer, uint32_t nBytes);   // send nBytes (polling on TXE)
void USART2_write(uint8_t* buffer, uint32_t nBytes);
uint8_t USART1_read(void);                              // read one received byte
uint8_t USART2_read(void);

uint32_t is_USART1_RXNE(void);                          // 1 if a byte has been received
uint32_t is_USART2_RXNE(void);
```

`printf()` is retargeted to USART2, so it prints to the PC terminal (Tera Term).

**Example code**

```c
static volatile uint8_t BT_Data = 0;

void setup(void){
	RCC_PLL_init();
	SysTick_init();
	USART2_init();          // PC
	USART1_init();          // Bluetooth (HC-06) or the other MCU
}

void USART1_IRQHandler(){                   // USART1 RX interrupt
	if (is_USART1_RXNE()){
		BT_Data = USART1_read();            // read received byte
		printf("RX: %c \r\n", BT_Data);    // display on PC (USART2)
	}
}
```

---



# Timer Interrupt

### Header File

```c
#include "ecTIM2.h"
```

```c
#ifndef __EC_TIM2_H
#define __EC_TIM2_H

#include "stm32f411xe.h"
#include "ecPinNames.h"
#include "ecRCC2.h"
#include "ecGPIO2.h"
#include "math.h"

#ifdef __cplusplus
 extern "C" {
#endif /* __cplusplus */

// Timer Update Interrupt Initialization
void TIM_UI_init(TIM_TypeDef* TIMx, uint32_t msec);     // 16-bit  msec < 65,530 msec 
void TIM_UI_init_us(TIM_TypeDef* TIMx, uint32_t usec);  // 16-bit  usec < 65,530 usec 

// Timer Update Interrupt On/Off
void TIM_UI_enable(TIM_TypeDef* TIMx);
void TIM_UI_disable(TIM_TypeDef* TIMx);

// Timer Update Interrupt Flag
uint32_t is_UIF(TIM_TypeDef *TIMx);
void clear_UIF(TIM_TypeDef *TIMx);

// Timer Counter Initialization
void TIM_init(TIM_TypeDef *TIMx, uint32_t msec); 
void TIM_init_us(TIM_TypeDef *TIMx, uint32_t usec); 

// Timer Counter Period Config
void TIM_period(TIM_TypeDef* TIMx, uint32_t msec);		
void TIM_period_us(TIM_TypeDef* TIMx, uint32_t usec);  	

// Timer Counter Auxiliary Functions
uint32_t get_TIM_IRQn(TIM_TypeDef* TIMx);
void TIM_clock_enable(TIM_TypeDef* TIMx);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif // __EC_TIM2_H 
```

### **TIM_UI_init()**

Initializes the selected timer with the given period in milliseconds, enables the update interrupt, and enables the timer's IRQ in the NVIC. An update event (overflow) occurs every `msec` milliseconds.

```c
void TIM_UI_init(TIM_TypeDef* TIMx, uint32_t msec)
```

#### Parameters

- `TIMx`: Timer peripheral (`TIM1` ~ `TIM5`, `TIM9` ~ `TIM11`)
- `msec`: Update period in milliseconds (16-bit, `msec` < 65,530)

```c
void TIM_UI_init(TIM_TypeDef* TIMx, uint32_t msec){
// 1. Initialize Timer	
	TIM_init(TIMx,msec);

// 2. Enable Update Interrupt
	TIM_UI_enable(TIMx);	

// 3. NVIC Setting
	uint32_t IRQn_reg = get_TIM_IRQn(TIMx);
	NVIC_EnableIRQ(IRQn_reg);				
	NVIC_SetPriority(IRQn_reg,2);
}
```

#### Example code

```c
TIM_UI_init(TIM3, 500);   // TIM3 update interrupt every 500 msec
```

### **TIM_UI_init_us()**

Same as `TIM_UI_init()`, but the update period is set in microseconds.

```c
void TIM_UI_init_us(TIM_TypeDef* TIMx, uint32_t usec)
```

#### Parameters

- `TIMx`: Timer peripheral (`TIM1` ~ `TIM5`, `TIM9` ~ `TIM11`)
- `usec`: Update period in microseconds (16-bit, `usec` < 65,530)

```c
void TIM_UI_init_us(TIM_TypeDef* TIMx, uint32_t usec){
// 1. Initialize Timer	
	TIM_init_us(TIMx,usec);
	
// 2. Enable Update Interrupt
	TIM_UI_enable(TIMx);
	
// 3. NVIC Setting
	uint32_t IRQn_reg = get_TIM_IRQn(TIMx);
	NVIC_EnableIRQ(IRQn_reg);				
	NVIC_SetPriority(IRQn_reg,2);
}
```

#### Example code

```c
TIM_UI_init_us(TIM3, 100);   // TIM3 update interrupt every 100 usec
```

### **TIM_UI_enable()**

Enables the update interrupt of the selected timer by setting the UIE bit in the DIER register. The UIF flag is cleared first.

```c
void TIM_UI_enable(TIM_TypeDef* TIMx)
```

#### Parameters

- `TIMx`: Timer peripheral

```c
void TIM_UI_enable(TIM_TypeDef* TIMx){
	TIMx->SR = ~TIM_SR_UIF;
	TIMx->DIER |= TIM_DIER_UIE;				
}
```

#### Example code

```c
TIM_UI_enable(TIM3);   // restart TIM3 update interrupt
```

### **TIM_UI_disable()**

Disables the update interrupt of the selected timer by clearing the UIE bit in the DIER register.

```c
void TIM_UI_disable(TIM_TypeDef* TIMx)
```

#### Parameters

- `TIMx`: Timer peripheral

```c
void TIM_UI_disable(TIM_TypeDef* TIMx){
	TIMx->DIER &= ~(TIM_DIER_UIE);					
}
```

#### Example code

```c
TIM_UI_disable(TIM3);   // stop TIM3 update interrupt
```

### **is_UIF()**

Checks whether an update event has occurred by reading the UIF bit in the SR register.

```c
uint32_t is_UIF(TIM_TypeDef *TIMx)
```

#### Parameters

- `TIMx`: Timer peripheral

```c
uint32_t is_UIF(TIM_TypeDef *TIMx){
	return TIMx->SR & TIM_SR_UIF;
}
```

#### Example code

```c
if (is_UIF(TIM3)) {
	// update event occurred
}
```

### **clear_UIF()**

Clears the UIF bit in the SR register. It must be called inside the IRQ handler, otherwise the interrupt is triggered again immediately.

```c
void clear_UIF(TIM_TypeDef *TIMx)
```

#### Parameters

- `TIMx`: Timer peripheral

```c
void clear_UIF(TIM_TypeDef *TIMx){
	TIMx->SR &= ~TIM_SR_UIF;
}
```

#### Example code

```c
void TIM3_IRQHandler(void){
	if (is_UIF(TIM3)) {
		led_toggle(PA_5);
		clear_UIF(TIM3);
	}
}
```

---



# PWM

### Header File

```c
#include "ecPWM2.h"
```

```c
#ifndef __EC_PWM2_STUDENT_H
#define __EC_PWM2_STUDENT_H

#include "math.h"
#include "stm32f411xe.h"
#include "ecGPIO2.h"
#include "ecTIM2.h"

#ifdef __cplusplus
 extern "C" {
#endif /* __cplusplus */

/* PWM Output Mode : OCxM field in CCMRx (3 bits) */
#define EC_PWM_MODE1	0	// output active while CNT <  CCR  (default)
#define EC_PWM_MODE2	1	// output active while CNT >= CCR  (inverted)

// Internal Helper function: for PWM Initialization
void PWM_init_config(PinName_t pinName, uint32_t usec);
// Duty ratio 0~1.0
void PWM_duty(PinName_t pinName, float duty);

// Low Frequency PWM initialization in msec (below 1kHz)
void PWM_init(PinName_t pinName, uint32_t msec);
// High Frequency PWM initialization in usec (1kHz to 100kHz)
void PWM_init_us(PinName_t pinName, uint32_t usec);

// Set Pulse width in msec
void PWM_pulsewidth(PinName_t pinName, uint32_t pulse_width_ms);
// Set Pulse width in usec
void PWM_pulsewidth_us(PinName_t pinName, uint32_t pulse_width_us);

// Internal Helper Functions
void PWM_mode_config(TIM_TypeDef *TIMx, int chN, uint32_t mode);
void PWM_pinmap(PinName_t pinName, TIM_TypeDef **TIMx, int *chN);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif // __EC_PWM2_STUDENT_H
```

### **PWM_init_config()**

Internal helper function for PWM initialization. It finds the timer and channel connected to the pin, sets the pin as an alternate function with the correct AF number, initializes the timer period, and configures the channel as PWM mode 1 (default duty 50%).

```c
void PWM_init_config(PinName_t pinName, uint32_t usec)
```

#### Parameters

- `pinName`: PWM output pin (e.g., `PA_0`)
- `usec`: PWM period in microseconds

```c
void PWM_init_config(PinName_t pinName, uint32_t usec){
// 0. Match TIMx from given Port_y and Pin_x
	GPIO_TypeDef *port;
	unsigned int pin;
	ecPinmap(pinName, &port, &pin);
	TIM_TypeDef *TIMx;
	int chN;
	PWM_pinmap(pinName, &TIMx, &chN);

// 1. Initialize GPIO port and pin as AF
	GPIO_init(pinName, AF);

// 2. Configure GPIO AFR by Pin x num.
	uint32_t afrIdx   = pin >> 3;				// same as (pin/8)
	uint32_t afrShift = 4 * (pin % 8);
	uint32_t afNum    = 0;

	//  AF1: TIM1,TIM2   AF2: TIM3,TIM4,TIM5   AF3: TIM9,TIM10,TIM11
	if((TIMx == TIM1) || (TIMx == TIM2))                        afNum = 1;
	else if(TIMx == TIM3 || TIMx == TIM4 || TIMx == TIM5)       afNum = 2;
	else if(TIMx == TIM9 || TIMx == TIM10 || TIMx == TIM11)     afNum = 3;

	port->AFR[afrIdx] &= ~(0xFUL << afrShift);
	port->AFR[afrIdx] |=  (afNum  << afrShift);

// 3. Initialize Timer : select CLK_CNT according to PWM period
	if((TIMx == TIM2) || (TIMx == TIM5) || (usec <= 65536)){
		TIM_init_us(TIMx, usec);
	}
	else {
		uint32_t msec = usec / 1000;
		if(msec > 6553) msec = 6553;	// 16-bit ARR has limit of 6.5sec
		TIM_init(TIMx, msec);
	}

// 4. Configure Timer Output mode as PWM
	PWM_mode_config(TIMx, chN, EC_PWM_MODE1);

// 5. Enable Timer Counter
	if(TIMx == TIM1) TIMx->BDTR |= TIM_BDTR_MOE;	// Main output enable (TIM1 only)
	TIMx->EGR |= TIM_EGR_UG;
	TIMx->SR   = ~TIM_SR_UIF;
	TIMx->CR1 |= TIM_CR1_CEN;
}
```

#### Example code

```c
// Called inside PWM_init() and PWM_init_us()
PWM_init_config(PA_0, 1000);   // PA_0 PWM, period 1000 usec
```

### **PWM_init()**

Initializes PWM output on the given pin with a period in milliseconds (low frequency, below 1 kHz).

```c
void PWM_init(PinName_t pinName, uint32_t msec)
```

#### Parameters

- `pinName`: PWM output pin (e.g., `PA_15`)
- `msec`: PWM period in milliseconds (TIM2/TIM5: 1 ~ 4,294,967, TIM1/TIM3/TIM4: 1 ~ 6,553)

```c
void PWM_init(PinName_t pinName, uint32_t msec){
	if(msec == 0) msec = 1;		// minimum 1 msec
	PWM_init_config(pinName, msec * 1000);		
}
```

#### Example code

```c
PWM_init(PA_15, 20);   // PA_15 (TIM2_CH1), period 20 msec (50 Hz, RC servo)
```

### **PWM_init_us()**

Initializes PWM output on the given pin with a period in microseconds (high frequency, 1 kHz ~ 100 kHz).

```c
void PWM_init_us(PinName_t pinName, uint32_t usec)
```

#### Parameters

- `pinName`: PWM output pin (e.g., `PA_0`)
- `usec`: PWM period in microseconds (10 usec and up)

```c
void PWM_init_us(PinName_t pinName, uint32_t usec){	
	if(usec < 10) usec = 10;  // Minimum 10us (100kHz) 
	PWM_init_config(pinName, usec);
}
```

#### Example code

```c
PWM_init_us(PA_0, 100);   // PA_0 (TIM2_CH1), period 100 usec (10 kHz)
```

### **PWM_duty()**

Sets the duty ratio of the PWM output. The duty is limited to 0.0 ~ 1.0, and the compare value is calculated as CCR = (ARR + 1) × duty.

```c
void PWM_duty(PinName_t pinName, float duty)
```

#### Parameters

- `pinName`: PWM output pin
- `duty`: Duty ratio (0.0 ~ 1.0)

```c
void PWM_duty(PinName_t pinName, float duty){
// 0. Match TIMx from  Port and Pin
	GPIO_TypeDef *port;
	unsigned int pin;
	ecPinmap(pinName, &port, &pin);
	TIM_TypeDef *TIMx;
	int chN;
	PWM_pinmap(pinName, &TIMx, &chN);

// 1. Check the duty ratio range from 0.0 to 1.0
	if(duty < 0.0f) duty = 0.0f;
	if(duty > 1.0f) duty = 1.0f;

// 2. Duty ratio -> CCR counts (PWM mode 1)
	uint32_t CCvalue = (uint32_t)((TIMx->ARR + 1) * duty);
	
	if(chN == 1)      { TIMx->CCR1 = CCvalue; }
	else if(chN == 2) { TIMx->CCR2 = CCvalue; }
	else if(chN == 3) { TIMx->CCR3 = CCvalue; }
	else if(chN == 4) { TIMx->CCR4 = CCvalue; }
}
```

#### Example code

```c
PWM_duty(PA_0, 0.5f);   // 50% duty
```

### **PWM_pulsewidth()**

Sets the HIGH pulse width of the PWM output in milliseconds. The pulse width is converted into counter ticks using the counter clock (f_CNT = EC_SYSCLK / (PSC + 1)), then into a duty ratio, and applied with `PWM_duty()`.

```c
void PWM_pulsewidth(PinName_t pinName, uint32_t pulse_width_ms)
```

#### Parameters

- `pinName`: PWM output pin
- `pulse_width_ms`: Pulse width in milliseconds (must be shorter than the PWM period)

```c
void PWM_pulsewidth(PinName_t pinName, uint32_t pulse_width_ms){
// 0. Match TIMx from  Port and Pin 	
	GPIO_TypeDef *port;
	unsigned int pin;	
	ecPinmap(pinName, &port, &pin);	
	TIM_TypeDef *TIMx;
	int chN;		
	PWM_pinmap(pinName, &TIMx, &chN);

	float duty=0.0f;
	uint32_t f_cnt =0;

// 1. Counter clock : CK_CNT = EC_SYSCLK / (PSC+1)
	f_cnt = (uint32_t)EC_SYSCLK / (TIMx->PSC + 1);		// [Hz]

// 2. Convert Pulse_width [msec] -> duty ratio (0~1.0) 
	uint32_t counts = (uint32_t)(((uint64_t)f_cnt * pulse_width_ms) / 1000UL);
	duty = (float)counts / (float)(TIMx->ARR + 1);

// 3. Set duty ratio
	PWM_duty(pinName, duty);
}
```

#### Example code

```c
PWM_pulsewidth(PA_15, 2);   // 2 msec pulse
```

### **PWM_pulsewidth_us()**

Same as `PWM_pulsewidth()`, but the pulse width is set in microseconds.

```c
void PWM_pulsewidth_us(PinName_t pinName, uint32_t pulse_width_us)
```

#### Parameters

- `pinName`: PWM output pin
- `pulse_width_us`: Pulse width in microseconds (must be shorter than the PWM period)

```c
void PWM_pulsewidth_us(PinName_t pinName, uint32_t pulse_width_us){
// 0. Match TIMx from  Port and Pin 	
	GPIO_TypeDef *port;
	unsigned int pin;	
	ecPinmap(pinName, &port, &pin);	
	TIM_TypeDef *TIMx;
	int chN;		
	PWM_pinmap(pinName, &TIMx, &chN);
	
	float duty=0.0f;
	
// 1. Counter clock : CK_CNT = EC_SYSCLK / (PSC+1)
	uint32_t f_cnt = (uint32_t)EC_SYSCLK / (TIMx->PSC + 1);		// [Hz]

// 2. Convert Pulse_width [usec] -> duty ratio (0~1.0) 
	uint32_t counts = (uint32_t)(((uint64_t)f_cnt * pulse_width_us) / 1000000UL);
	duty = (float)counts / (float)(TIMx->ARR + 1);

// 3. Set duty ratio
	PWM_duty(pinName, duty);
}
```

#### Example code

```c
PWM_pulsewidth_us(PA_15, 1500);   // 1.5 msec pulse (servo center position, 90°)
```
