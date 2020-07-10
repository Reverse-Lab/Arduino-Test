
//DS12C887     Arduino UNO
//DS12C887     UNO Pin NO.
//      4       4   ++ 37
//      5       5   ++ 36
//      6       6   ++ 35
//      7       7   ++ 34
//      8       8   ++ 33
//      9       9   ++ 32
//     10       10  ++ 31
//     11       11  ++ 30
//     12       GND
//     13       14（dscs）++ 4
//     14       15（dsas）++ 2
//     15       16（dsrw）++ 4
//     17       17（dsds）++ 5
//     18       VCC
//     19       12  (dsirq)   ++ 6
//     24       VCC


const int dscs=43;
const int dsas=52;
const int dsrw=42;
const int dsds=44;
const int dsirq=51;
//define bidirectional address-data bus(양방향 주소 버스 정의)

const int ad[8]={28,29,30,31,32,33,34,35};
//등록 A B C D 주소

#define regA 0x0A
#define regB 0x0B
#define regC 0x0C
#define regD 0x0D
//함수 선언
void bus2out();
void bus2in();
void write_ds(byte add,byte date);
byte read_ds(byte add);
void set_time();
void set_alarm(byte ashi,byte afen,byte amiao);
byte miao,fen,shi,ri,yue,nian,xq,s_arm,m_arm;

// ds12887 메모리 제어
unsigned short Count = 65535;
unsigned short count;

//int countHi, countLow;
//byte hiByte = 0xE0;
//byte loByte = hiByte + 1;

int i;

void setup()
{
  Serial.begin(9600);
  pinMode(dscs, OUTPUT);
  pinMode(dsas, OUTPUT);
  pinMode(dsrw, OUTPUT);
  pinMode(dsds, OUTPUT);
  pinMode(dsirq, OUTPUT);
  
//칩 초기화
  write_ds(regA,0x20);      // 수정 발진기를 켜고 클럭을 계속 작동 시키려면 A 레지스터를 제어하십시오.
  write_ds(regB,0x26);        //24 시간 모드, 데이터 바이너리 모드를 설정하고 알람 인터럽트를 활성화하십시오

//시간 초기화
//set_time();//초기 시간 설정, 한 번 주석 처리

//알람 초기화
 //set_alarm(0,0,170);//알람 설정 (시,분,초)

//카운트 셋업
set_count();
}
void loop()
{
  //읽는 시간
  miao=read_ds(0);
  s_arm=read_ds(1);
  fen=read_ds(2);
  m_arm=read_ds(3);
  shi=read_ds(4);
  ri=read_ds(7);
  yue=read_ds(8);
  nian=read_ds(9);
  xq=read_ds(6);
  
  // ds12887 메모리 읽기
  byte counthi = read_ds(0xE0);
  byte countlow = read_ds(0xE1);

  count = word(counthi, countlow);

  //  byte count = word(countHi, countLow);
  
//  miao=(miao&0x0f)+((miao&0xf0)>>4)*10;//BCD 코드를 사용하는 경우 비트 산술 처리가 필요합니다
  Serial.print(nian);
  Serial.print("/");
  Serial.print(yue);
  Serial.print("/");
  Serial.print(ri);
  Serial.print("  ");
  Serial.print(shi);
  Serial.print("：");
  Serial.print(fen);
  Serial.print("：");
  Serial.print(miao);
  Serial.print("  주：");
  Serial.print(xq);
  Serial.print(" Ararm: ");
  Serial.print(m_arm);Serial.print(":");Serial.print(s_arm);
  Serial.print(" Count : ");Serial.print(count);
  Serial.println();
  delay(500);
}
void bus2out(){
  for(i=0;i<8;i++)
  {
    pinMode(ad[i], OUTPUT);
  }
}
void bus2in(){
  for(i=0;i<8;i++)
  {
    pinMode(ad[i], INPUT);
  }
}
void write_ds(byte add,byte date)    //칩 쓰기 명령
{
  bus2out();  //set address pins to output
  digitalWrite(dscs,0);
  digitalWrite(dsas,1);
  digitalWrite(dsds,1);
  digitalWrite(dsrw,1);
  
  //set address on bus(주소 쓰기)
    for(i=0;i<8;i++)
  {digitalWrite(ad[i],bitRead(add,i));}
  digitalWrite(dsas,0);
  digitalWrite(dsrw,0);
  
  //set byte to write(데이터 쓰기)
  for(i=0;i<8;i++)
  {digitalWrite(ad[i],bitRead(date,i));}
  digitalWrite(dsrw,1);
  digitalWrite(dsas,1);
  digitalWrite(dscs,1);
}

byte read_ds(byte add)         //칩 읽기 명령
{
  byte readb=0;
  bus2out();  //set address pins to output
  digitalWrite(dscs,0);
  digitalWrite(dsas,1);
  digitalWrite(dsds,1);
  digitalWrite(dsrw,1);
  
  //set address on bus(주소 쓰기)
  for(i=0;i<8;i++)
  {digitalWrite(ad[i],bitRead(add,i));}
  digitalWrite(dsas,0);
  digitalWrite(dsds,0);
  
  bus2in(); // Input Mode 전환
  for(i=0;i<8;i++){digitalWrite(ad[i],1);}
  readb = digitalRead(ad[0])|(digitalRead(ad[1])<<1)|(digitalRead(ad[2])<<2)|(digitalRead(ad[3])<<3)|(digitalRead(ad[4])<<4)|(digitalRead(ad[5])<<5)|(digitalRead(ad[6])<<6)|(digitalRead(ad[7])<<7);
  digitalWrite(dsds,1);
  digitalWrite(dsas,1);
  digitalWrite(dscs,1);
  return readb;
}
void set_time()//시간을 직접 조정하고 두 번째 매개 변수를 수정해야합니다
{
  write_ds(0,40);//초
  write_ds(2,37);//분
  write_ds(4,17);//시간
  write_ds(7,10);//일
  write_ds(8,5);//월
  write_ds(9,20);//년
  write_ds(6,1);//주
}

void set_count()
{
  
  write_ds(0xE0, highByte(Count));
  write_ds(0xE1, lowByte(Count));
  
}

void set_alarm(byte ashi,byte afen,byte amiao) //알람 시계의 초기 값 설정
{
  write_ds(1,amiao); //초
  write_ds(3,afen); //분
  write_ds(5,ashi); //시
}
//————————————————
//版权声明：本文为CSDN博主「XPLab」的原创文章，遵循 CC 4.0 BY-SA 版权协议，转载请附上原文出处链接及本声明。
//原文链接：https://blog.csdn.net/Xinpengmaker/java/article/details/79374770
