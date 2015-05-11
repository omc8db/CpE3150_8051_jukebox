#include <uart.h>
#include <reg932.h>
#include <simon2.h>

char global_mode;
#define KEYBOARD_MODE 	0
#define PLAYBACK_MODE 	1
#define GAME_MODE     	2

//-------------------------------------
// Hardware Resources Allocation
//-------------------------------------
// Timer 0: Music
// Timer 1: Serial
// Interrupt 0:
// Interrupt 1:
// Interrupt 2:
// Interrupt 3:

//------------------------------------
// Button and LED assignments
//------------------------------------
#define MODE_SWITCH_BUTTON SW9

// Possible Modes  : KEYBOARD_MODE
//                   PLAYBACK_MODE
//                   GAME_MODE
void gpio_init(void);

//Predefined delay values for common notes
// Fclock = 7.373Mhz
// Machine cycle time = (1 / Fclock)(2 clocks / 1 cycle ) = 2.71 x 10^-7 seconds

// 440 / (Machine cycle time) = 8378 cycles
// 50% duty cycle -> 8378 / 2 = 4189
#define A4 -4189
#define A5 -2095

char modeSelect(void);
void keyboardMode(void);
void jukeboxMode(void);
void gameMode(void);
void delay(long millis);
void playNote(long pitch, long duration);
void T0M1Delay(long cycles);

volatile long reload_value;

int main()
{
	uart_init();
	gpio_init();

	//"Hello, World!" has 13 characters
	uart_write("Hello, World!\n");

	//Infinite loop through all modes
	//When the mode switch button is pressed, the function returns
	//and the program advances to the next mode
	while(1)
	{
		keyboardMode();
		jukeboxMode();
		gameMode();
	}
	return 0;
}

void gpio_init(void)
{
	P0M1 = 0x00;
	P1M1 = 0x00;
	P2M1 = 0x00;
}

char modeSelect(void)
{
	//Dummy value
	
	return KEYBOARD_MODE;

}

void keyboardMode(void)
{
	uart_write("Keyboard Mode Selected\n");
	while(1)
	{
		if(!SW1)
		{
			LED1_RED = 0;
			//uart_write("Playing note A440\r\n");
			playNote(A4, 100);
			LED1_RED = 1;
		}
		if(!SW4)
		{
			LED4_YEL = 0;
			//uart_write("Playing note A880\r\n");	
			playNote(A5, 100);
			LED4_YEL = 1;
		}
		if(!SW7)
		{
			LED7_GRN = 0;
			uart_write("Playing note D4\r\n");
			LED7_GRN = 1;
		}
		if(!MODE_SWITCH_BUTTON)
		{
			return;
		}
	}
	return;
}

void jukeboxMode(void)
{
	uart_write("Jukebox Mode Selected\n");
	return;
}

void gameMode(void)
{
	uart_write("Game Mode Selected\n");
	return;
}

void delay(long millis)
{
	char j;
	for(; millis != 0; millis--)
	{
		for(j = 255; j != 0; j--);
	}
	return;
}
/*
void timer0ISR() interrupt 1{

	BUZZER = ~BUZZER;
	
	//Disable timer interrupt
	TF0 = 0;
	TR0 = 0;
	IT0 = 0;
	//Clear timer flags

	//Preload value
	TH0 = reload_value >> 8;
	TL0 = reload_value;

	//Restart timer
	TF0 = 1;
	TR0 = 1;
	//Return
	return;
}//end function timer0ISR()
*/
void playNote(long pitch, long duration)
{
	long i;
	//Wait for note to stop
	for(i = 0; i < duration; i++)
	{
		BUZZER = ~BUZZER;
		if(!MODE_SWITCH_BUTTON)
		{
			//Disable timer
			IT0 = 0;
			TR0 = 0;
			TF0 = 0;
		}
		T0M1Delay(pitch);
	}

	//Disable timer
	IT0 = 0;
	TR0 = 0;
	TF0 = 0;
	return;
}

void T0M1Delay(long cycles)
{
	TMOD = 0x01;
	TH0 = cycles >> 8;
	TL0 = cycles;
	TR0 = 1;
	while(!TF0);
	TR0 = 0;
	TF0 = 0;
}
