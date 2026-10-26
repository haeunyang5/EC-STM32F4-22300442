/*----------------------------------------------------------------\
@ Embedded Controller by Young-Keun Kim - Handong Global University
Author           : Haeun Yang
Created          : 05-03-2021
Modified         : 10-08-2026 
Language/ver     : C++ in VS Code

Description      :  Tutorial FSM Using Mealy (Fan)
/----------------------------------------------------------------*/


#include <stdio.h>

// State definition
#define S0  0
#define S1  1

#define LOW  0
#define HIGH  1

unsigned char state = S0;
unsigned char nextstate = S0;
unsigned char input = LOW;
unsigned char ledOut = LOW;

typedef struct {
	unsigned int next[2];   // nextstate = FSM[state].next[input]
	unsigned int out;    // output = FSM[state].out
} State_t;

State_t FSM[2] = {
  {{S0, S1},LOW},
  {{S1, S0},HIGH}
};

void nextState();
void stateOutput();

int main()
{
    printf("Start\n\r");

    input = LOW;
	nextState();
	stateOutput();

	printf("state=%d,  ledOut=%d \n\r", state, ledOut);

    input = HIGH;
	nextState();
	stateOutput();
	printf("state=%d,  ledOut=%d \n\r", state, ledOut);

    input = LOW;
	nextState();
	stateOutput();
	printf("state=%d,  ledOut=%d \n\r", state, ledOut);

    return 0;
}


void nextState() {
	nextstate = FSM[state].next[input];
	state = nextstate;	
}

void stateOutput() {	
	ledOut = FSM[state].out;
}
