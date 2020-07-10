
/****************************************************/
/*    Encoder interface IC LS7166    */
/****************************************************/

/* LS7166 commands */
#define  MASTER_RESET     0x20    /* master reset command */
#define  INPUT_SETUP      0x68    /* setup counter input mode */
#define  QUAD_X1       0xC1    /* quadrature multiplier to 1 */
#define  QUAD_X2       0xC2    /* quadrature multiplier to 2 */
#define  QUAD_X4       0xC3    /* quadrature multiplier to 4 */
#define  ADDR_RESET      0x01    /* reset address pointer */
#define  LATCH_CNTR      0x02    /* latch counter */
#define  CNTR_RESET      0x04    /* reset counter */
#define  PRESET_CTR      0x08    /* tranfer preset to counter */


void LS7166_init(void)
{

 LS7166CS_port = 0;   /* 1 = disable, 0 = select */
 LS7166CD_port = 1;   /* 1 = Command, 0 = Data */
  Sleep_10micro(1);
 outp7166(MASTER_RESET);  /* master reset command */
  Sleep_10micro(1);
 outp7166(INPUT_SETUP);  
  Sleep_10micro(1);
 outp7166(QUAD_X4);   /* quadrature encoder pulse (4 times multiplied value) */
  Sleep_10micro(1);
 outp7166(CNTR_RESET);  /* counter reset command */
  Sleep_10micro(1);
 outp7166(ADDR_RESET);  /* address pointer reset command */
  Sleep_10micro(1); 
 
 LS7166_latch();    /* LS7166 output latch command */

 LS7166CD_port = 1;
 LS7166CS_port = 1;

}

void LS7166_latch(void)
{

 LS7166CS_port = 0;
 LS7166CD_port = 1;   /* select command */

 
 outp7166(LATCH_CNTR);  /* counter latch command */
 
 LS7166CD_port = 0;
 LS7166CS_port = 1;
}

long LS7166_read(void)
{
 long position=0, tempk;
 
 LS7166CS_port = 0;
 LS7166CD_port = 1;   /* select command */
 
 outp7166(ADDR_RESET); /* the address reset and the latch command */

 outp7166(LATCH_CNTR);

 
 LS7166CD_port = 0;   /* select data */

 
 position |=(long)((inp7166()&0x000000ff));  /* read first byte */
 position |=(long)((inp7166()&0x000000ff)<<8); /* read second byte */
 position |=(long)((inp7166()&0x000000ff)<<16); /* read third byte */

 LS7166CD_port = 1;   /* select command */
 LS7166CS_port = 1;
 
 return (position&0x0000ffff);
}

void outp7166(UCHAR port_data)
{
 LS7166PortSetOutput();

 LS7166_OP = port_data;  /* write 8 bits */

 LS7166RD_port = 1;
 LS7166CS_port = 0;
 LS7166CD_port = 1;  /* Command */
  
 LS7166WR_port = 0;
 LS7166WR_port = 1; 
 
 LS7166CS_port = 1;
 LS7166PortSetInput();
}

UCHAR inp7166(void)
{
 UCHAR port_InData=0;
 
 LS7166WR_port = 1;
 LS7166CS_port = 0;
 LS7166CD_port = 0;
 
 
 LS7166RD_port = 0;
 LS7166RD_port = 1; 

 LS7166PortSetInput();

 port_InData = LS7166_IP;  /* read 8 bits from Port */
 
 LS7166PortSetInput();
 
 LS7166CD_port = 1;
 LS7166CS_port = 1;
 
 return port_InData; 
}
