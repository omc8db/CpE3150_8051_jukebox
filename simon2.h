/*--------------------------------------------------------------------------
simon2.h

Header file for the Simon Board

--------------------------------------------------------------------------*/

#ifndef SIMON_2
#define SIMON_2

sbit BUZZER P1^.7

sbit LED1_RED = P2^4;
sbit LED2_AMB = P0^6;
sbit LED3_YEL = P2^5;
sbit LED4_YEL = P0^5;
sbit LED5_RED = P1^6;
sbit LED6_GRN = P0^7;
sbit LED7_GRN = P2^7;
sbit LED8_RED = P0^4;
sbit LED9_AMB = P2^6;

sbit SW1 = P2^0;
sbit SW2 = P0^2;
sbit SW3 = P2^1;
sbit SW4 = P0^1;
sbit SW5 = P1^4;
sbit SW6 = P0^3;
sbit SW7 = P2^3;
sbit SW8 = P0^0;
sbit SW9 = P2^2;

#endif //SIMON_2
