//------------------------------------
// Note Definitions
//------------------------------------
// Fclock = 7.373Mhz
// Machine cycle time = (1 / Fclock)(2 clocks / 1 cycle ) = 2.71 x 10^-7 seconds

// 440 / (Machine cycle time) = 8378 cycles
// 50% duty cycle -> 8378 / 2 = 4189
//440 Hz

#define	C0	-112737	
#define	D0	-100450			
#define	E0	-89478			
#define	F0	-84437			
#define	G0	-75235			
#define	A0	-67027			
#define	B0	-59710			
#define	C1	-56369			
#define	D1	-50211			
#define	E1	-44739			
#define	F1	-42228			
#define	G1	-37617			
#define	A1	-33514			
#define	B1	-29855			
#define	C2	-28180			
#define	D2	-25106			
#define	E2	-22367			
#define	F2	-21112			
#define	G2	-18809			
#define	A2	-16757			
#define	B2	-14929			
#define	C3	-14091			
#define	D3	-12554			
#define	E3	-11184			
#define	F3	-10556			
#define	G3	-9404			
#define	A3	-8378			
#define	B3	-7464			
#define	C4	-7045			
#define	D4	-6277			
#define	E4	-5592			
#define	F4	-5278			
#define	G4	-4702			
#define	A4	-4189			
#define	B4	-3732			
#define	C5	-3523			
#define	D5	-3138			
#define	E5	-2796			
#define	F5	-2639			
#define	G5	-2351			
#define	A5	-2095			
#define	B5	-1866			
#define	C6	-1761			
#define	D6	-1569			
#define	E6	-1398			
#define	F6	-1320			
#define	G6	-1176			
#define	A6	-1047			
#define	B6	-933			
#define	C7	-881			
#define	D7	-785			
#define	E7	-699			
#define	F7	-660			
#define	G7	-588			
#define	A7	-524			
#define	B7	-467			
#define	C8	-440			
#define	D8	-392			
#define	E8	-349			
#define	F8	-330			
#define	G8	-294			
#define	A8	-262			
#define	B8	-233			

#define REST 0


//------------------------------------
// Tunes stored in code memory
//------------------------------------

//Number of notes in tune 1 * 2. Used for array bounds checking
#define TUNE_1_LENGTH 40

//Length of beats in milliseconds
//125 = 120bpm
#define TEMPO_1 30

//{note1, time1, note2, time2...}
//times are specified in beats. Here, four beats to a quarter note

code short int BANJOS_A[] = {
	REST, 24,
	D4, 3,
	REST, 1,
	D4, 3,
	REST, 1,
	
	D4, 7,
	REST, 1,
	E4, 8,
	D4, 8,
	REST, 8,

	REST, 24,
	D3, 3,
	REST, 1,
	D3, 3,
	REST, 1,
	
	D3, 7,
	REST, 1,
	E3, 8,
	D3, 3,
	G3, 5,
	REST, 8,

	REST, 24,
	B4, 4,
	C5, 4,
	D5, 8,
	B4, 8,
	C5, 8,
	A4, 8,
	B4, 8,
	G4, 8,
	A4, 16,

	REST, 32,

	REST, 24,
	B4, 4,
	C5, 4,
	D5, 8,
	B4, 8,
	C5, 8,
	A4, 8,
	B4, 8,
	G4, 8,
	A4, 16,

	REST, 32,

	REST, 24,
	G4, 3,
	REST, 1,
	G4, 3,
	REST, 1,
	G4, 7,
	REST, 1,
	A4, 7,
	REST, 1,
	B4, 7,
	REST, 1,
	C5, 8,
	D5, 8,
	C5, 8,
	B4, 8,
	REST, 8

};


