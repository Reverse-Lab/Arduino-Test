const int grayPin[8]={2,3,4,5,6,7,8,9};

uint8_t Binary2Gray(uint8_t data)
 {
   int n_data=(data>>1);
   n_data=(data ^ n_data);
   
  return n_data;
 }

void grayPinSet(){
  for(int8_t i=0;i<8;i++)
  {
    pinMode(grayPin[i], INPUT);
  }
}

 uint8_t Gray2Binary(uint8_t data)
 {
  grayPinSet();
  byte b[8]={0,0,0,0,0,0,0,0};
  int8_t i=0;
  
  for(i=0;i<8;i++)
  {
    //b[i]=(data & (1 << i ))>>i;
    b[i]=(data & (1 << digitalRead(grayPin[i])))>>digitalRead(grayPin[i]);
  }
  
  uint8_t n_data=0;
  n_data= (b[7]<<7);
  for(i=6;i>=0;i--)  
  {
    if( (b[i+1]+b[i])==0 || (b[i+1]+b[i])==2 )
    {
      n_data|=(0<<i);
    }else if( (b[i+1]+b[i])==1 || (b[i+1]+b[i])==3)
    {
      n_data|=(1<<i);
    }
  }
 return n_data;
}
void setup() {
  // put your setup code here, to run once:
Serial.begin(4800);
Serial.println(" Gray binary conversion test");
grayPinSet();
  
}

void loop() {
  byte encVal;
  int8_t i=0;
  for( i=0;i<8;i++)
  {
    encVal[i] ^= digitalRead(grayPin[i]);
  }
  Serial.println(encVal[i]);
}
