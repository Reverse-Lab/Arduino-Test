void start_update()
{
  footState = digitalRead(footSW);

    if (footState == HIGH)
    {
      if (footFlag == 0)
      {
        footFlag = 1; delay(5);
      }
    }
    
    if (footState == LOW)
    {
      if (footFlag == 1)
      {
        startAddr = read_ds(0xEF); delay(5);
        startAddr = 1 - startAddr;
        write_ds (0xEF, startAddr); // 자동 모드, 정지 모드
        delay(5);
      }
    }
    
    startAddr = read_ds(0xEF); //delay(5);
    if (startAddr == 1)
    {
      mode_update();
      jig_update();
      pos_update();
      shape_update();

      if (mode = 1)
      {
        cycleStart();
      } else {
        homePos();
      }
    }
}
void homePos()
{
  if (start_pos == 1) {
    sqieez.OFF();
    left.OFF();
    right.ON();
    
  } else if (start_pos == 0) {
    sqieez.OFF();
    right.OFF();
    left.ON();
    
  }
}
