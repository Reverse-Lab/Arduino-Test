/* 7166.h : Header file for 7166 encoder project
 * Thomas Peterson, 4/20/10 */
 
 
/* Output pin defines */
//note: Data bus is on port B
#define NWR LATDbits.LATD4			//Write (active low)
#define NCS LATDbits.LATD3			//Chip select (active low)
//#define NLCTR_NLLTC LATDbits.LATD2	//External load command (active low)
//#define NABGT_NRCTR LATDbits.LATD1	//External cntr reset or a/b enable gate (active low)
//#define NCY	LATCbits.LATC1			//input
//#define NBW LATCbits.LATC2			//input
#define C_ND LATCbits.LATC3			//control or data register
#define NRD LATCbits.LATC4			//Read (active low)


#define OUT 0
#define IN 1

//Fcn definitions
void comm_dir(int dir);
void bus_out(unsigned char byte);
unsigned char bus_in();
void write_ctrl(unsigned char reg, unsigned char byte);
void write_PR(unsigned int value);
unsigned int read_reg(unsigned char reg);
void delay();
void latchWR();
unsigned char latchRD();

//Control registers
#define MCR 0
#define ICR 1
#define OCCR 2
#define QR 3
//Readable registers
#define OL 0
#define OSR 1


/* Comm_dir: Switches tris bits on comm bus to inputs or outputs
			 If the bus is changed from port B to other pins, this fcn
			 will need to be changed   */
void comm_dir(int dir) {
	if (dir==IN) {
		//Set as input
		TRISB |= 0x00FF;	
	}	
	else {
		//set as output
		TRISB &= 0xFF00;	
	}	
} 
 	
/* bus_out : This function sends out a byte on the data bus
			 If the bus pins are changed, this fcn will need
			 to be changed too. 
			 This does NOT handle other control lines. */
void bus_out(unsigned char byte) {
	comm_dir(OUT);
	LATB = byte;
}	

/* bus_in : This function reads a byte on the data bus
		 	If the bus pins are changed, this fcn will need
			to be changed too. 
			This does NOT handle other control lines. */
unsigned char bus_in() {
	comm_dir(IN);
	return (unsigned char)(PORTB & 0x00FF);
}	

void init_7166() {
	write_ctrl(MCR,0x20); 	//Performs master reset
	write_ctrl(MCR,0x04);	//Sub-reset
	write_ctrl(ICR,0x18); 	//Enables A/B, sets up pin 3-4
	write_ctrl(OCCR, 0x34); //Divide by n mode	0x04
	write_ctrl(QR,0x03);	//x4 quadrature mode	0x04
	//0x3840->Pr
	write_PR(0x3840);		//Upper limit
}	

/* write_ctrl: writes a byte to the given control register */
void write_ctrl(unsigned char reg, unsigned char byte){
	NCS = 0;	//Select chip
	NRD = 1;
	C_ND = 1;	//Setup to write to control register
	unsigned char op = (reg << 6) | byte;
	bus_out(op);
	latchWR();	//write value
	NCS = 1;	//Unselect chip	
}	

void latchWR() {
	delay();	//Setup time (???)
	NWR = 0;	//Latch out write
	delay();	//Hold time (???)
	NWR = 1;	//End write	
}	

void write_PR(unsigned int value) {
	unsigned int write_value;
	//Prepare for PR read
	write_ctrl(MCR,0x01);	//Reset ctr
	NCS = 0;	//select chip
	C_ND = 0;	//Writes will be to PR _only_
	write_value = value & 0xff; //Get lowest byte
	bus_out((unsigned char)write_value);
	latchWR();	//Latch out value
	write_value = (value >> 8) & 0xff; //Get next byte
	bus_out((unsigned char)write_value);
	latchWR(); //Latch out value
	write_value = (value >> 16) & 0xff; //Get MSB
	bus_out((unsigned char)write_value);
	latchWR(); //Latch out value
	NCS = 1; 	//Unselect chip
	write_ctrl(MCR,0x08); 	//PR->CNTR
}

unsigned int read_reg(unsigned char reg) {
	unsigned int read_value;
	if (reg == OSR) {
		NCS = 0;	//Select chip
		C_ND = 1;
		NWR = 1;
		read_value = latchRD();
		NCS = 1;	//Deselect chip
		return read_value;	
	}		
	if (reg == OL) {
		write_ctrl(MCR,0x03);	//Prepare for read
		C_ND = 0;
		NRD = 1;
		NWR = 1;
		NCS = 0;	//Select chip
		unsigned char read_byte;
		read_byte = latchRD();	//Read byte 0
		read_value = read_byte;
		delay();
		read_byte = latchRD();	//REad byte 1
		int tmp = read_byte;
		tmp = tmp << 8;
		read_value |= tmp;
		delay();
		read_byte = latchRD(); //Read byte 3
		tmp = read_byte;
		tmp = tmp << 16;
		read_value |= tmp;
		NCS = 1;	//Unselect chip
		return read_value;
	}
	//no other regs can be read	
}	

unsigned char latchRD() {
	NRD = 0;	//Read
	delay();	//???
	unsigned char read_byte = bus_in();
	delay();
	NRD = 1;	//End latch
	return read_byte;
}	

/* Delays for 10 cycles, plus branch time */
void delay() {
	asm("nop");
	asm("nop");
	asm("nop");
	asm("nop");
	asm("nop");
	asm("nop");
	asm("nop");
	asm("nop");
	asm("nop");
	asm("nop");
}
