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
#define A4 4189
#define A5 2095

char modeSelect(void);
void keyboardMode(void);
void jukeboxMode(void);
void gameMode(void);
void delay(long millis);
void playNote(long pitch, long duration);

volatile long reload_value;

int main()
{
	uart_init();
	gpio_init();

	LED1_RED = 1;
	delay(400);
	LED1_RED = 0;
	delay(400);
	LED1_RED = 1;
	delay(400);
	LED1_RED = 0;

	//"Hello, World!" has 13 characters
	uart_write("Hello, World!\n");

	global_mode = KEYBOARD_MODE;
	while(1)
	{
		if(global_mode == KEYBOARD_MODE)
			keyboardMode();
		if(global_mode == PLAYBACK_MODE)
			jukeboxMode();
		if(global_mode == GAME_MODE)
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
			playNote(A4, 10);
			uart_write("Playing note A440\r\n");
		
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

void playNote(long pitch, long duration)
{

	long i = 0;
	reload_value = pitch;

	TMOD = 0x01;  //Timer 0, Mode 1
	//Preload Values
	TH0 = pitch >> 8;
	TL0 = pitch;

	//Enable timer
	TF0 = 0;
	IT0 = 1;
	TR0 = 1;

	
	//Wait for note to stop
	for(i = 0; i < duration; i++)
	{
		if(!MODE_SWITCH_BUTTON)
			return;
		delay(1);
	}

	//Disable timer
	IT0 = 0;
	TR0 = 0;
	TF0 = 0;
	return;
}
