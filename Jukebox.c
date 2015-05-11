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
// Timer 0: Main Buzzer
// Timer 1: Serial / Aux Buzzer
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
//440 Hz
#define A4 -4189
//261 Hz
#define C4 -14124
//329 Hz
#define E4 -11205
//392 Hz
#define G4 -9404

#define C5 -7062

#define E5 -5602

#define G5 -4702

//880Hz
#define A5 -2095



char modeSelect(void);
void keyboardMode(void);
void jukeboxMode(void);
void gameMode(void);
void delay(long millis);

void startNoteMain(long note);
void silenceMain(void);

void debounce(void);

static long T0reload;
static long T1reload;

int main()
{
	uart_init();
	gpio_init();

	//"Hello, World!" has 13 characters
	uart_write("UART initialized\r\n");

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
	uart_write("Keyboard Mode Selected\r\n");
	debounce();
	while(1)
	{
		if(!SW1)
		{
			LED1_RED = 0;
			//uart_write("Playing note A440\r\n");
			startNoteMain(C5);
			while(!SW1); //Wait for button to be released
			silenceMain();
			LED1_RED = 1;
		}
		if(!SW4)
		{
			LED4_YEL = 0;
			//uart_write("Playing note A880\r\n");	
			startNoteMain(E5);
			while(!SW4);
			silenceMain();
			LED4_YEL = 1;
		}
		if(!SW7)
		{
			LED7_GRN = 0;
			startNoteMain(G5);
			while(!SW7);
			silenceMain();
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
	uart_write("Jukebox Mode Selected\r\n");
	debounce();
	while(1)
	{
		if(!MODE_SWITCH_BUTTON)
		{
			return;
		}
	}
	
	return;
}

void gameMode(void)
{
	uart_write("Game Mode Selected\r\n");
	debounce();

	while(1)
	{
		if(!MODE_SWITCH_BUTTON)
		{
			return;
		}
	}
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

	//Preload value
	TH0 = T0reload >> 8;
	TL0 = T0reload;

	//Restart timer
	TF0 = 0;
	TR0 = 1;
	//Return
	return;
}//end function timer0ISR()

void startNoteMain(long note)
{
	T0reload = note;
	TMOD = 0x01;
	TH0 = note >> 8; //Preload note
	TL0 = note;

	TF0 = 0;
	ET0 = 1;         //Enable interrupt

	TR0 = 1;
}

void silenceMain(void)
{
	TR0 = 0;
	ET0 = 0;
}

void debounce(void)
{
	delay(100);
	while(!MODE_SWITCH_BUTTON);
	delay(100);
}

