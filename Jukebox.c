#include <uart.h>
#include <reg932.h>
#include <simon2.h>

char globalMode;
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

int main()
{
	uart_init();
	P2M1 = 0x00;
	
	globalMode = modeSelect();

	//"Hello, World!" has 13 characters
	uart_write("Hello, World!", 13);

	while(1)
	{

	}
	return 0;
}

char modeSelect(void)
{
	//Dummy value
	return KEYBOARD_MODE;
}

void jukeboxMode(void)
{
	return;
}

void gameMode(void)
{
	return;
}
