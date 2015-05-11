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
sbit AUX_BUZZER = P1^1;

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


void keyboardMode(void);
void jukeboxMode(void);
void gameMode(void);
void delay(long millis);

void startNoteMain(long note);
void silenceMain(void);
void startNoteAux(long note);
void silenceAux(void);

void debounce(void);

static long T0reload;
static long T1reload;

void Tune1(void);
void Tune2(void);

//Number of notes in tune 1 * 2. Used for array bounds checking
#define TUNE_1_LENGTH 10
//Length of beats in milliseconds
//125 = 120bpm
#define TEMPO_1 125

//{note1, time1, note2, time2...}
//times are specified in beats
code long TUNE_1_NOTES[] = {C4, 1, E4, 2, G4, 3, C5, 2, E5, 3};

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
	//This counter represents which beat we're on
	uart_write("Jukebox Mode Selected\r\n");
	debounce();
	while(1)
	{
		if(!SW1)
			Tune1();
		if(!SW4)
			Tune2();
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
	long i;
	long j;

	for(i = 0; i < millis; i++)
	{
		for(j = 0; j < 64; j++);
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


void timer1ISR() interrupt 3{

	AUX_BUZZER = ~AUX_BUZZER;
	
	//Disable timer interrupt
	TF1 = 0;
	TR1 = 0;

	//Preload value
	TH1 = T1reload >> 8;
	TL1 = T1reload;

	//Restart timer
	TF1 = 0;
	TR1 = 1;
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


void startNoteAux(long note)
{
	T1reload = note;
	TMOD = 0x10;
	TH1 = note >> 8; //Preload note
	TL1 = note;

	TF1 = 0;
	ET1 = 1;         //Enable interrupt

	TR1 = 1;
}

void silenceAux(void)
{
	TR1 = 0;
	ET1 = 0;
}


void debounce(void)
{
	delay(100);
	while(!MODE_SWITCH_BUTTON);
	delay(100);
}

void Tune1(void)
{
	long counter;
	long next_note_time;
	long next_note_val;
	char current_note_index;

	uart_init();
	uart_write("Playing Tune 1");
	counter = 0;
	current_note_index = 0;
	next_note_val = TUNE_1_NOTES[current_note_index];
	current_note_index++;
	next_note_time = TUNE_1_NOTES[current_note_index] + counter;
	
	while(current_note_index < TUNE_1_LENGTH)
	{
		if(counter >= next_note_time)	
		{
			//Play the next note
			if(next_note_val == 0)
				silenceMain();
			else
				startNoteMain(next_note_val);

			//Get the next note. Increment twice to skip over time
			current_note_index ++;
			next_note_val = TUNE_1_NOTES[current_note_index];
			current_note_index ++;		
			next_note_time = TUNE_1_NOTES[current_note_index] + counter;
		}
		if(!MODE_SWITCH_BUTTON)
			goto leaveTune1;

		counter++;
		delay(TEMPO_1);
	}

leaveTune1:
	silenceMain();
	return;
}

void Tune2(void)
{
	return;
}
