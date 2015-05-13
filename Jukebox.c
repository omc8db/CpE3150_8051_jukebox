#include <uart.h>
#include <reg932.h>
#include <simon2.h>
#include <music.h>

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
sbit AUX_BUZZER = P0^7;

// Possible Modes  : KEYBOARD_MODE
//                   PLAYBACK_MODE
//                   GAME_MODE
void gpio_init(void);


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
void Tune3(void);
void playTune(short* trackA, short* trackB, short length, char tempo);

int main()
{

	gpio_init();
	silenceAux();
	silenceMain();

	uart_init();
	//"Hello, World!" has 13 characters
	uart_write("UART initialized\r\n");

	//Infinite loop through all modes
	//When the mode switch button is pressed, the function returns
	//and the program advances to the next mode
	while(1)
	{
		keyboardMode();
		jukeboxMode();
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
		if(!SW2)
		{
			LED2_AMB = 0;
			startNoteAux(C4);
			while(!SW2);
			silenceAux();
			LED2_AMB = 1;
		}
		if(!SW5)
		{
			LED5_RED = 0;
			startNoteAux(E4);
			while(!SW5);
			silenceAux();
			LED5_RED = 1;
		}
		if(!SW8)
		{
			LED8_RED = 0;
			startNoteAux(G4);
			while(!SW8);
			silenceAux();
			LED8_RED = 1;
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
		if(!SW7)
			Tune3();
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

	//Timer 0, mode 1. Does not affect other timer.
	TMOD = 0x11;

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

	//Timer 1, mode 1. Does not affect other timer.
	TMOD = 0x11;
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
	uart_init();
	uart_write("Playing Dueling Banjos");
	playTune(BANJOS_A, BANJOS_B, sizeof(BANJOS_A)/sizeof(short), TEMPO_1);
	return;
}

void Tune2(void)
{
	uart_init();
	uart_write("Playing Mario Brothers Theme");
	playTune(MARIO_A, MARIO_A, sizeof(MARIO_A)/sizeof(short), TEMPO_2);
	return;
}

void Tune3(void)
{
	uart_init();
	uart_write("Playing Take5\r\n");
	playTune(TAKE5_B, TAKE5_A, sizeof(TAKE5_B)/sizeof(short), TEMPO_3);
}

void playTune(short* trackA, short* trackB, short length, char tempo)
{
	short counter;

	short next_noteA_time;
	short next_noteA_val;
	short current_noteA_index;

	short next_noteB_time;
	short next_noteB_val;
	short current_noteB_index;

	counter = 0;
	current_noteA_index = 0;
	current_noteB_index = 0;

	next_noteA_val = trackA[current_noteA_index];
	current_noteA_index++;
	next_noteA_time = trackA[current_noteA_index] + counter;

	next_noteB_val = trackB[current_noteB_index];
	current_noteB_index++;
	next_noteB_time = trackB[current_noteB_index] + counter;
	
	while(current_noteA_index < length)
	{
		if(counter >= next_noteA_time)	
		{
			//Play the next note
			if(next_noteA_val == 0)
				silenceMain();
			else
				startNoteMain(next_noteA_val);

			//Get the next note. Increment twice to skip over time
			current_noteA_index ++;
			next_noteA_val = trackA[current_noteA_index];
			current_noteA_index ++;		
			next_noteA_time = trackA[current_noteA_index] + counter;
		}
		if(counter >= next_noteB_time)	
		{
			//Play the next note
			if(next_noteB_val == 0)
				silenceAux();
			else
				startNoteAux(next_noteB_val);

			//Get the next note. Increment twice to skip over time
			current_noteB_index ++;
			next_noteB_val = trackB[current_noteB_index];
			current_noteB_index ++;		
			next_noteB_time = trackB[current_noteB_index] + counter;
		}
		if(!MODE_SWITCH_BUTTON)
			goto leaveTune;

		counter++;
		delay(tempo);
	}

leaveTune:
	silenceMain();
	silenceAux();
	return;
}


