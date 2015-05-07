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

int main()
{
	uart_init();
	P2M1 = 0x00;

	//"Hello, World!" has 13 characters
	uart_write("Hello, World!", 13);

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
	return;
}

void jukeboxMode(void)
{
	return;
}

void gameMode(void)
{
	return;
}
