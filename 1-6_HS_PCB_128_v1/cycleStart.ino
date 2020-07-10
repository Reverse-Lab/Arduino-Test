
void cycleStart()
{
  switch (mode){
    case 1:
      homePos();
      if (start_pos == 0) {
        sqieez.ON();
        left.OFF();
        right.ON();
        sqieez.OFF();
        right.OFF();
        left.ON();
      } else {
        sqieez.ON();
        right.OFF();
        left.ON();
        sqieez.OFF();
        left.OFF();
        right.ON();
      }
      FND++;
      write_ds(0xEF, 0); // 반자동 모드인 경우 작업개시 초기화
    break;

    case 2:
      homePos();
      if (start_pos == 0) {
        sqieez.ON();
        left.OFF();
        right.ON();
        sqieez.OFF();
        right.OFF();
        left.ON();
      } else {
        sqieez.ON();
        right.OFF();
        left.ON();
        sqieez.OFF();
        left.OFF();
        right.ON();
      }
      FND++;
    break;
    
    default:
    break;  
  }
}
