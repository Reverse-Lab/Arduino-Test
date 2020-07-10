

//===================================================================
// 코드비젼
//===================================================================
// #pragma opt-    //코드 최적화 금지
//===================================================================
//#include <mega128.h> 
#include <stdio.h>
//#include <delay.h> 
//===================================================================
//#define U_C     unsigned char
//#define U_I     unsigned int
//===================================================================
//Port - C Bit Assign
//===================================================================
// rtc data bus
#define    PC_DDR        0x00    // Port-C Direction
#define    PC_INI        0xFF    // Port-C Init
#define    RTC_IN        0x13 //PINC
#define    RTC_OUT       0x15 // PORTC
//
#define    PD_DDR        0x0F    // Port-D Direction
#define    PD_INI        0x0D    // Port-D Init
#define    RTC_CS        10 //PORTD.0 // Lo Active
#define    RTC_AS        11 //PORTD.1 // Hi Active
#define    RTC_WR        12 //PORTD.2 // Lo Active
#define    RTC_RD        13  //PORTD.3 // Lo Active
//===================================================================
#define Rtc_Sec        0x00 // DS12C887 RTC
#define Rtc_SEC_ALM    0x01
#define Rtc_Min        0x02
#define Rtc_MIN_ALM    0x03
#define Rtc_Hour       0x04
#define Rtc_Hour_ALM   0x05
#define Rtc_WEEKDay    0x06
#define Rtc_Date       0x07
#define Rtc_Mon        0x08
#define Rtc_Year       0x09
#define Rtc_A         //uip,dv2,dv1,dv0,rs3,rs2,rs1,rs0
#define Rtc_B         //set,pie,aie,uie,sqwe,DM=0,24=1,dse
#define Rtc_C         //irqf,pf,af,uf,0,0,0,0
#define Rtc_D         //VRT,0,0,0,0,0,0,0
//#define Rtc_FLAG    0x0F//User ram 'G' 체크용
//#define Rtc_CHKYY   0x0E//User ram 년도 체크용
//#define Rtc_10Year  0x32//세기년도
//===================================================================
//===================================================================
unsigned char U_C; 
unsigned char Year;
unsigned char Mon;
unsigned char Date;
unsigned char Hour;
unsigned char Min;
unsigned char Sec;
unsigned int U_C Time[7];
unsigned int lcd_buf[20];
//===================================================================
void RTC_Write(U_C Add, U_C Data);
U_C RTC_Read(U_C Add);
void Init_Time(void);
void Get_Time(void);
void Save_Time(void);
//===================================================================
 
void RTC_Write(U_C Add, U_C Data){
    DDRC=0xFF;    RTC_CS=0;    RTC_OUT=Add;    RTC_AS=1;    RTC_AS=0;
    RTC_OUT=Data;    RTC_WR=0;    RTC_WR=1;    RTC_CS=1;    DDRC=0;
}
//===================================================================
U_C RTC_Read(U_C Add){ U_C Data;
    DDRC=0xFF;    RTC_CS=0;    RTC_OUT=Add;    RTC_AS=1;    RTC_AS=0;
    DDRC=0;       RTC_RD=0;    Data=RTC_IN;    RTC_RD=1;    RTC_CS=1;
    return Data;
}
//===================================================================
void Init_Time(void){
    Year=9;    Mon=5;    Date=0x20;    Hour=0x12;    Min=0;
    Save_Time();
}
//===================================================================
void Get_Time(void){
    Year=RTC_Read(Rtc_Year);    Mon =RTC_Read(Rtc_Mon);
    Date=RTC_Read(Rtc_Date);    Hour=RTC_Read(Rtc_Hour);
    Min =RTC_Read(Rtc_Min);     Sec =RTC_Read(Rtc_Sec);
 
    if(Year>0x99){ Init_Time(); }
    else if(((Mon & 0xf0)>0x10)||((Mon & 0xf)>0x9)){ Init_Time(); }
    else if(((Date& 0xf0)>0x30)||((Date& 0xf)>0x9)){ Init_Time(); }
    else if(((Hour& 0xf0)>0x20)||((Hour& 0xf)>0x9)){ Init_Time(); }
    else if(((Min & 0xf0)>0x50)||((Min & 0xf)>0x9)){ Init_Time(); };
    
    Time[0]=Year; Time[1]=Mon; Time[2]=Date;
    Time[3]=Hour; Time[4]=Min; Time[5]=Sec;
}
//===================================================================
void Save_Time(void){
    RTC_Write(Rtc_Year,Year);   RTC_Write(Rtc_Mon,Mon);
    RTC_Write(Rtc_Date,Date);   RTC_Write(Rtc_Hour,Hour);
    RTC_Write(Rtc_Min, Min);    RTC_Write(Rtc_Sec, Sec);
}
//===================================================================
//===================================================================
void main(void){
    PORTC=PC_INI; PORTD=PD_INI; DDRC=PC_DDR;  DDRD=PD_DDR;
    RTC_Write(Rtc_B,0x82); RTC_Write(Rtc_A,0x20); RTC_Write(Rtc_B,0x02); //RTC Init
    while(1){
        Get_Time();
        sprintf(lcd_buf," %02x/%02x %02x:%02x:%02x ",Mon,Date,Hour,Min,Sec);
    };
}


void setup() {
  // put your setup code here, to run once:

}

void loop() {
  // put your main code here, to run repeatedly:

}
