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

// Gets the mode from the user
// Possible outputs: KEYBOARD_MODE
//                   PLAYBACK_MODE
//                   GAME_MODE
char modeSelect(void);
void keyboardMode(void);
void jukeboxMode(void);
void gameMode(void);
void delay(long millis);

int main()
{
	uart_init();
	P2M1 = 0x00;


	LED1_RED = 1;
	delay(400);
	LED1_RED = 0;
	delay(400);
	LED1_RED = 1;
	delay(400);
	LED1_RED = 0;

	//"Hello, World!" has 13 characters
	uart_write("Hello, World!\n");


	while(1)
	{
		global_mode = modeSelect();
		if(global_mode == KEYBOARD_MODE)
			keyboardMode();
		if(global_mode == PLAYBACK_MODE)
			jukeboxMode();
		if(global_mode == GAME_MODE)
			gameMode();
	}
	return 0;
}

char modeSelect(void)
{
	//Dummy value
	
	return KEYBOARD_MODE;
}

void keyboardMode(void)
{
	uart_write("Keyboard Mode Selected\n");
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
