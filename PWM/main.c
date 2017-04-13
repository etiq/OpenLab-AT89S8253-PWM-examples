/*  Name     : main.c
 *  Purpose  : Main file for generating pwm using AT89S8253 vary dutycle thus control the speed of motor
 *  Author   : Etiq Technologies
 *  Date     : 25/04/2016 
 *  Revision : None
 */
#include <REG8253.H>


sbit PWM_Pin = P2^0;		   // Pin P2.0 is named as PWM_Pin

// Function declarations
void port_init(void);
void InitTimer0(void);
void InitPWM(void);


/*
 
 */



// Main Function
int main(void)
{
	 unsigned  int dutycycle=50;
   port_init();   	       // Make all ports zero except port0
   InitPWM();              // Start PWM
	                          // Rest is done in Timer0 interrupt
 
   
  while(1)               
   {
	if((P0_0==0)&&(dutycycle<250))   // if P0_0 pressed value of dutycycle variable increased results in speed reduction
	{
	while(!P0_0);
	dutycycle+=20;                    
	}
	
	if((P0_1==0)&&(dutycycle>50))  //if P0_1 is pressed value of dutycycle variable is reduced results in speed increase
	{
	while(!P0_1);
	dutycycle-=20;	
	}
	
	/*
	if pwm pin is high then TH0 is modified so that for the next overload new value will be loaded  
	and the timer will run for the rest of the period with pwm pin low which is made in ISR. Similarly
	opposite carried out 
	for pwm pin low.*/
	
	if(PWM_Pin==1)           
	TH0=255-dutycycle;       
	
	if(PWM_Pin==0)
	TH0=dutycycle;
 }
	 
 }
// Init CCT function
void port_init(void)
{
	P0=0xFF;  
  P1=0x00;
  P2=0x00;	
	P3=0x00;
	
}

// Timer0 initialize
void InitTimer0(void)
{
	TMOD &= 0xF0;    // Clear 4bit field for timer0
	TMOD |= 0x02;    // Set timer0 in mode 2
	
	
	ET0 = 1;         // Enable Timer0 interrupts
	EA  = 1;         // Global interrupt enable
	
	TR0 = 1;         // Start Timer 0
}

// PWM initialize
void InitPWM(void)
{
	
	PWM_Pin=1;
	InitTimer0();    // Initialize timer0 to start generating interrupts
					
}
// PWM generation code is written inside the Timer0 ISR
// Timer0 ISR

void Timer0_ISR (void) interrupt 1   
{

	if(PWM_Pin)	   // if PWM_Pin is high
	{
		PWM_Pin = 0;
	}
	else	         // if PWM_Pin is low
	{
		PWM_Pin = 1;
		
	}

	TF0 = 0;     // Clear the interrupt flag

}

