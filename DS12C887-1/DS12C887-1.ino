
//DS12C887     Arduino UNO
//DS12C887     UNO Pin NO.
//      4       4
//      5       5
//      6       6
//      7       7
//      8       8
//      9       9
//     10       10
//     11       11
//     12       GND
//     13       14（A0）
//     14       15（A1）
//     15       16（A2）
//     17       17（A3）
//     18       VCC
//     19       12
//     24       VCC


const int dscs=14;
const int dsas=15;
const int dsrw=16;
const int dsds=17;
const int dsirq=12;
//define bidirectional address-data bus(定义双向地址总线)
const int ad[8]={4,5,6,7,8,9,10,11};
//寄存器A B C D的地址
#define regA 0x0A
#define regB 0x0B
#define regC 0x0C
#define regD 0x0D
//函数声明
void bus2out();
void bus2in();
void write_ds(byte add,byte date);
byte read_ds(byte add);
void set_time();
void set_alarm(byte ashi,byte afen,byte amiao);
byte miao,fen,shi,ri,yue,nian,xq;
int i;
void setup()
{
  Serial.begin(9600);
  pinMode(dscs, OUTPUT);
  pinMode(dsas, OUTPUT);
  pinMode(dsrw, OUTPUT);
  pinMode(dsds, OUTPUT);
  pinMode(dsirq, OUTPUT);
//芯片初始化
//  write_ds(regA,0x20);      // 控制A寄存器来开启晶体振荡器且保持时钟运行。
//  write_ds(regB,0x26);        //设置24小时模式，数据二进制模式，开启闹铃中断。
//时间初始化
//  set_time();//设置初始时间，运行一次即可注释掉
//  set_alarm(13,30,0);//设置闹钟
}
void loop()
{
  //读取时间
  miao=read_ds(0);
  fen=read_ds(2);
  shi=read_ds(4);
  ri=read_ds(7);
  yue=read_ds(8);
  nian=read_ds(9);
  xq=read_ds(6);
//  miao=(miao&0x0f)+((miao&0xf0)>>4)*10;//若使用BCD码需要进行位运算处理
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
  Serial.print("  星期：");
  Serial.println(xq);
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
void write_ds(byte add,byte date)    //芯片写指令
{
  bus2out();  //set address pins to output
  digitalWrite(dscs,0);
  digitalWrite(dsas,1);
  digitalWrite(dsds,1);
  digitalWrite(dsrw,1);
  //set address on bus(写地址)
  for(i=0;i<8;i++)
  {digitalWrite(ad[i],bitRead(add,i));}
  digitalWrite(dsas,0);
  digitalWrite(dsrw,0);
  //set byte to write(写数据)
  for(i=0;i<8;i++)
  {digitalWrite(ad[i],bitRead(date,i));}
  digitalWrite(dsrw,1);
  digitalWrite(dsas,1);
  digitalWrite(dscs,1);
}
byte read_ds(byte add)         //芯片读指令
{
  byte readb=0;
  bus2out();  //set address pins to output
  digitalWrite(dscs,0);
  digitalWrite(dsas,1);
  digitalWrite(dsds,1);
  digitalWrite(dsrw,1);
  //set address on bus(写地址)
  for(i=0;i<8;i++)
  {digitalWrite(ad[i],bitRead(add,i));}
  digitalWrite(dsas,0);
  digitalWrite(dsds,0);
  bus2in();
  for(i=0;i<8;i++){digitalWrite(ad[i],1);}
  readb = digitalRead(ad[0])|(digitalRead(ad[1])<<1)|(digitalRead(ad[2])<<2)|(digitalRead(ad[3])<<3)|(digitalRead(ad[4])<<4)|(digitalRead(ad[5])<<5)|(digitalRead(ad[6])<<6)|(digitalRead(ad[7])<<7);
  digitalWrite(dsds,1);
  digitalWrite(dsas,1);
  digitalWrite(dscs,1);
  return readb;
}
void set_time()//需要自行调整时间，修改第二个参数
{
  write_ds(0,10);//秒
  write_ds(2,19);//分
  write_ds(4,13);//时
  write_ds(7,26);//日
  write_ds(8,2);//月
  write_ds(9,18);//年
  write_ds(6,1);//星期
}
void set_alarm(byte ashi,byte afen,byte amiao) //闹钟设初值
{
  write_ds(1,amiao);
  write_ds(3,afen);
  write_ds(5,ashi);
}
//————————————————
//版权声明：本文为CSDN博主「XPLab」的原创文章，遵循 CC 4.0 BY-SA 版权协议，转载请附上原文出处链接及本声明。
//原文链接：https://blog.csdn.net/Xinpengmaker/java/article/details/79374770
