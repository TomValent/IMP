/* Header file with all the essential definitions for a given type of MCU */
#include "MK60D10.h"
#include "letters.h"

#define TWO_DISPLAYS 16

/* Macros for bit-level registers manipulation */
#define GPIO_PIN_MASK	0x1Fu
#define GPIO_PIN(x)		(((1)<<(x & GPIO_PIN_MASK)))


/* Constants specifying delay loop duration */
#define	tdelay1			10000
#define tdelay2 		20

#define BTN_SW2 0x400     // Port E, bit 10
#define BTN_SW3 0x1000    // Port E, bit 12
#define BTN_SW4 0x8000000 // Port E, bit 27
#define BTN_SW5 0x4000000 // Port E, bit 26
#define BTN_SW6 0x800     // Port E, bit 11


/* Configuration of the necessary MCU peripherals */
void SystemConfig() {
	/* Turn on all port clocks */
	SIM->SCGC5 = SIM_SCGC5_PORTA_MASK | SIM_SCGC5_PORTE_MASK;


	/* Set corresponding PTA pins (column activators of 74HC154) for GPIO functionality */
	PORTA->PCR[8] = ( 0|PORT_PCR_MUX(0x01) );  // A0
	PORTA->PCR[10] = ( 0|PORT_PCR_MUX(0x01) ); // A1
	PORTA->PCR[6] = ( 0|PORT_PCR_MUX(0x01) );  // A2
	PORTA->PCR[11] = ( 0|PORT_PCR_MUX(0x01) ); // A3

	/* Set corresponding PTA pins (rows selectors of 74HC154) for GPIO functionality */
	PORTA->PCR[26] = ( 0|PORT_PCR_MUX(0x01) );  // R0
	PORTA->PCR[24] = ( 0|PORT_PCR_MUX(0x01) );  // R1
	PORTA->PCR[9] = ( 0|PORT_PCR_MUX(0x01) );   // R2
	PORTA->PCR[25] = ( 0|PORT_PCR_MUX(0x01) );  // R3
	PORTA->PCR[28] = ( 0|PORT_PCR_MUX(0x01) );  // R4
	PORTA->PCR[7] = ( 0|PORT_PCR_MUX(0x01) );   // R5
	PORTA->PCR[27] = ( 0|PORT_PCR_MUX(0x01) );  // R6
	PORTA->PCR[29] = ( 0|PORT_PCR_MUX(0x01) );  // R7

	/* Set corresponding PTE pins (output enable of 74HC154) for GPIO functionality */
	PORTE->PCR[28] = ( 0|PORT_PCR_MUX(0x01) ); // #EN


	/* Change corresponding PTA port pins as outputs */
	PTA->PDDR = GPIO_PDDR_PDD(0x3F000FC0);

	/* Change corresponding PTE port pins as outputs */
	PTE->PDDR = GPIO_PDDR_PDD( GPIO_PIN(28) );
}


/* Variable delay loop */
void delay(int t1, int t2)
{
	int i, j;

	for(i=0; i<t1; i++) {
		for(j=0; j<t2; j++);
	}
}


/* Conversion of requested column number into the 4-to-16 decoder control.  */
void column_select(unsigned int col_num)
{
	unsigned i, result, col_sel[4];

	for (i =0; i<4; i++) {
		result = col_num / 2;	  // Whole-number division of the input number
		col_sel[i] = col_num % 2;
		col_num = result;

		switch(i) {

			// Selection signal A0
		    case 0:
				((col_sel[i]) == 0) ? (PTA->PDOR &= ~GPIO_PDOR_PDO( GPIO_PIN(8))) : (PTA->PDOR |= GPIO_PDOR_PDO( GPIO_PIN(8)));
				break;

			// Selection signal A1
			case 1:
				((col_sel[i]) == 0) ? (PTA->PDOR &= ~GPIO_PDOR_PDO( GPIO_PIN(10))) : (PTA->PDOR |= GPIO_PDOR_PDO( GPIO_PIN(10)));
				break;

			// Selection signal A2
			case 2:
				((col_sel[i]) == 0) ? (PTA->PDOR &= ~GPIO_PDOR_PDO( GPIO_PIN(6))) : (PTA->PDOR |= GPIO_PDOR_PDO( GPIO_PIN(6)));
				break;

			// Selection signal A3
			case 3:
				((col_sel[i]) == 0) ? (PTA->PDOR &= ~GPIO_PDOR_PDO( GPIO_PIN(11))) : (PTA->PDOR |= GPIO_PDOR_PDO( GPIO_PIN(11)));
				break;

			// Otherwise nothing to do...
			default:
				break;
		}
	}
}

void row_select(unsigned int rowNum)
{
	switch(rowNum) {
		case 0:
			PTA->PDOR |= GPIO_PDOR_PDO( GPIO_PIN(26));
			PTA->PDOR &= ~GPIO_PDOR_PDO( GPIO_PIN(24));
			PTA->PDOR &= ~GPIO_PDOR_PDO( GPIO_PIN(9));
			PTA->PDOR &= ~GPIO_PDOR_PDO( GPIO_PIN(25));
			PTA->PDOR &= ~GPIO_PDOR_PDO( GPIO_PIN(28));
			PTA->PDOR &= ~GPIO_PDOR_PDO( GPIO_PIN(7));
			PTA->PDOR &= ~GPIO_PDOR_PDO( GPIO_PIN(27));
			PTA->PDOR &= ~GPIO_PDOR_PDO( GPIO_PIN(29));
			break;

		case 1:
			PTA->PDOR &= ~GPIO_PDOR_PDO( GPIO_PIN(26));
			PTA->PDOR |= GPIO_PDOR_PDO( GPIO_PIN(24));
			PTA->PDOR &= ~GPIO_PDOR_PDO( GPIO_PIN(9));
			PTA->PDOR &= ~GPIO_PDOR_PDO( GPIO_PIN(25));
			PTA->PDOR &= ~GPIO_PDOR_PDO( GPIO_PIN(28));
			PTA->PDOR &= ~GPIO_PDOR_PDO( GPIO_PIN(7));
			PTA->PDOR &= ~GPIO_PDOR_PDO( GPIO_PIN(27));
			PTA->PDOR &= ~GPIO_PDOR_PDO( GPIO_PIN(29));
			break;

		case 2:
			PTA->PDOR &= ~GPIO_PDOR_PDO( GPIO_PIN(26));
			PTA->PDOR &= ~GPIO_PDOR_PDO( GPIO_PIN(24));
			PTA->PDOR |= GPIO_PDOR_PDO( GPIO_PIN(9));
			PTA->PDOR &= ~GPIO_PDOR_PDO( GPIO_PIN(25));
			PTA->PDOR &= ~GPIO_PDOR_PDO( GPIO_PIN(28));
			PTA->PDOR &= ~GPIO_PDOR_PDO( GPIO_PIN(7));
			PTA->PDOR &= ~GPIO_PDOR_PDO( GPIO_PIN(27));
			PTA->PDOR &= ~GPIO_PDOR_PDO( GPIO_PIN(29));
			break;

		case 3:
			PTA->PDOR &= ~GPIO_PDOR_PDO( GPIO_PIN(26));
			PTA->PDOR &= ~GPIO_PDOR_PDO( GPIO_PIN(24));
			PTA->PDOR &= ~GPIO_PDOR_PDO( GPIO_PIN(9));
			PTA->PDOR |= GPIO_PDOR_PDO( GPIO_PIN(25));
			PTA->PDOR &= ~GPIO_PDOR_PDO( GPIO_PIN(28));
			PTA->PDOR &= ~GPIO_PDOR_PDO( GPIO_PIN(7));
			PTA->PDOR &= ~GPIO_PDOR_PDO( GPIO_PIN(27));
			PTA->PDOR &= ~GPIO_PDOR_PDO( GPIO_PIN(29));
			break;

		case 4:
			PTA->PDOR &= ~GPIO_PDOR_PDO( GPIO_PIN(26));
			PTA->PDOR &= ~GPIO_PDOR_PDO( GPIO_PIN(24));
			PTA->PDOR &= ~GPIO_PDOR_PDO( GPIO_PIN(9));
			PTA->PDOR &= ~GPIO_PDOR_PDO( GPIO_PIN(25));
			PTA->PDOR |= GPIO_PDOR_PDO( GPIO_PIN(28));
			PTA->PDOR &= ~GPIO_PDOR_PDO( GPIO_PIN(7));
			PTA->PDOR &= ~GPIO_PDOR_PDO( GPIO_PIN(27));
			PTA->PDOR &= ~GPIO_PDOR_PDO( GPIO_PIN(29));
			break;

		case 5:
			PTA->PDOR &= ~GPIO_PDOR_PDO( GPIO_PIN(26));
			PTA->PDOR &= ~GPIO_PDOR_PDO( GPIO_PIN(24));
			PTA->PDOR &= ~GPIO_PDOR_PDO( GPIO_PIN(9));
			PTA->PDOR &= ~GPIO_PDOR_PDO( GPIO_PIN(25));
			PTA->PDOR &= ~GPIO_PDOR_PDO( GPIO_PIN(28));
			PTA->PDOR |= GPIO_PDOR_PDO( GPIO_PIN(7));
			PTA->PDOR &= ~GPIO_PDOR_PDO( GPIO_PIN(27));
			PTA->PDOR &= ~GPIO_PDOR_PDO( GPIO_PIN(29));
			break;

		case 6:
			PTA->PDOR &= ~GPIO_PDOR_PDO( GPIO_PIN(26));
			PTA->PDOR &= ~GPIO_PDOR_PDO( GPIO_PIN(24));
			PTA->PDOR &= ~GPIO_PDOR_PDO( GPIO_PIN(9));
			PTA->PDOR &= ~GPIO_PDOR_PDO( GPIO_PIN(25));
			PTA->PDOR &= ~GPIO_PDOR_PDO( GPIO_PIN(28));
			PTA->PDOR &= ~GPIO_PDOR_PDO( GPIO_PIN(7));
			PTA->PDOR |= GPIO_PDOR_PDO( GPIO_PIN(27));
			PTA->PDOR &= ~GPIO_PDOR_PDO( GPIO_PIN(29));

			break;

		case 7:
			PTA->PDOR &= ~GPIO_PDOR_PDO( GPIO_PIN(26));
			PTA->PDOR &= ~GPIO_PDOR_PDO( GPIO_PIN(24));
			PTA->PDOR &= ~GPIO_PDOR_PDO( GPIO_PIN(9));
			PTA->PDOR &= ~GPIO_PDOR_PDO( GPIO_PIN(25));
			PTA->PDOR &= ~GPIO_PDOR_PDO( GPIO_PIN(28));
			PTA->PDOR &= ~GPIO_PDOR_PDO( GPIO_PIN(7));
			PTA->PDOR &= ~GPIO_PDOR_PDO( GPIO_PIN(27));
			PTA->PDOR |= GPIO_PDOR_PDO( GPIO_PIN(29));
			break;

		default:
			break;
		}
}

void select_spot(unsigned int x, unsigned int y)
{
	if (x >= 0 && x < TWO_DISPLAYS && y >= 0 && y < TWO_DISPLAYS) {
		column_select(x);
		row_select(y);
	}

	delay(180, 1);
}

void clear()
{
	PTA->PDOR = 0x00;
}

void showField(char field[HEIGHT][MAX], int length, int z)
{
	for (int k = 0; k < length; k++) {
		for (int i = 0; i < HEIGHT; i++) {
			if (field[i][k] == '1')
				select_spot(z-7-k, 7-i);
		}
	}
	clear();
}

void createString(char field[HEIGHT][MAX], char* str, int strLength)
{
	int fieldLength = 0;
	for (int i=0; i < strLength; i++) {
		switch(str[i]) {
			case ' ':
				addSpace(field, fieldLength);
				break;
			case 'A':
				addA(field, fieldLength);
				break;
			case 'B':
				addB(field, fieldLength);
				break;
			case 'C':
				addC(field, fieldLength);
				break;
			case 'D':
				addD(field, fieldLength);
				break;
			case 'E':
				addE(field, fieldLength);
				break;
			case 'F':
				addF(field, fieldLength);
				break;
			case 'G':
				addG(field, fieldLength);
				break;
			case 'H':
				addH(field, fieldLength);
				break;
			case 'I':
				addI(field, fieldLength);
				break;
			case 'J':
				addJ(field, fieldLength);
				break;
			case 'K':
				addK(field, fieldLength);
				break;
			case 'L':
				addL(field, fieldLength);
				break;
			case 'M':
				addM(field, fieldLength);
				break;
			case 'N':
				addN(field, fieldLength);
				break;
			case 'O':
				addO(field, fieldLength);
				break;
			case 'P':
				addP(field, fieldLength);
				break;
			case 'Q':
				addQ(field, fieldLength);
				break;
			case 'R':
				addR(field, fieldLength);
				break;
			case 'S':
				addS(field, fieldLength);
				break;
			case 'T':
				addT(field, fieldLength);
				break;
			case 'U':
				addU(field, fieldLength);
				break;
			case 'V':
				addV(field, fieldLength);
				break;
			case 'W':
				addW(field, fieldLength);
				break;
			case 'X':
				addX(field, fieldLength);
				break;
			case 'Y':
				addY(field, fieldLength);
				break;
			case 'Z':
				addZ(field, fieldLength);
				break;
		}
		fieldLength += 8;
	}
}

void MCUInit(void)
{
    MCG_C4 |= ( MCG_C4_DMX32_MASK | MCG_C4_DRST_DRS(0x01) );
    SIM_CLKDIV1 |= SIM_CLKDIV1_OUTDIV1(0x00);
    WDOG_STCTRLH &= ~WDOG_STCTRLH_WDOGEN_MASK;
}

void initButtonPorts()
{
    PORTE->PCR[10] = PORT_PCR_MUX(0x01); // SW2
    PORTE->PCR[12] = PORT_PCR_MUX(0x01); // SW3
    PORTE->PCR[27] = PORT_PCR_MUX(0x01); // SW4
    PORTE->PCR[26] = PORT_PCR_MUX(0x01); // SW5
    PORTE->PCR[11] = PORT_PCR_MUX(0x01); // SW6
}


int main(void)
{
	MCUInit();
	SystemConfig();
	initButtonPorts();
	char field[HEIGHT][MAX];
	int maxLen;
	int forLoopLimit;
	int strLen;

    for (;;) {
    	if (!(GPIOE_PDIR & BTN_SW2)) {
    		char *displayText = "HELLO";
    		strLen = 5;
    		maxLen = strLen * 8;
    		forLoopLimit = maxLen * 2 + 20;
    		createString(field, displayText, strLen);
    		for (int i = 0; i < forLoopLimit; i++) {
    			for (int k = 0; k < 12; k++) {
    				showField(field, maxLen, i);
    			}
    		}
    	} else if (!(GPIOE_PDIR & BTN_SW3)) {
    		char *displayText = "WORLD";
    		strLen = 5;
    		maxLen = strLen * 8;
    		forLoopLimit = maxLen * 2 + 20;
    		createString(field, displayText, strLen);
    		for (int i = 0; i < forLoopLimit; i++) {
    			for (int k = 0; k < 12; k++) {
    				showField(field, maxLen, i);
    			}
    		}
    	} else if (!(GPIOE_PDIR & BTN_SW5)) {
    		char *displayText = "IMP";
    		strLen = 3;
    		maxLen = strLen * 8;
    		forLoopLimit = maxLen * 2 + 20;
    		createString(field, displayText, strLen);
    		for (int i = 0; i < forLoopLimit; i++) {
    			for (int k = 0; k < 12; k++) {
    				showField(field, maxLen, i);
    			}
    		}
		} else if (!(GPIOE_PDIR & BTN_SW4)) {
    		char *displayText = "FIT VUT";
    		strLen = 7;
    		maxLen = strLen * 8;
    		forLoopLimit = maxLen * 2 + 20;
    		createString(field, displayText, strLen);
    		for (int i = 0; i < forLoopLimit; i++) {
    			for (int k = 0; k < 12; k++) {
    				showField(field, maxLen, i);
    			}
    		}
		} else if (!(GPIOE_PDIR & BTN_SW6)) {
    		char *displayText = "BRNO";
    		strLen = 4;
    		maxLen = strLen * 8;
    		forLoopLimit = maxLen * 2 + 20;
    		createString(field, displayText, strLen);
    		for (int i = 0; i < forLoopLimit; i++) {
    			for (int k= 0; k < 12; k++) {
    				showField(field, maxLen, i);
    			}
    		}
		}
    }
    /* Never leave main */
    return 0;
}
