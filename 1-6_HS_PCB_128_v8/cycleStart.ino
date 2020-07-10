
void cycleStart()
{
 if (jig == 0){
  unsigned long currentMillis = millis();
  switch (mode){
    case 1:         // 반자동 모드
      if (start_pos == 0) {                 // 후 인쇄 모드 LED ON 필요
        while((digitalRead(updownSensor) == LOW) && (digitalRead(leftSensor) == HIGH))
          {sqieez.ON(); break;}
        while((digitalRead(updownSensor) == HIGH) && (digitalRead(leftSensor) == HIGH))
          {left.OFF(); right.ON();break;}
        while((digitalRead(updownSensor) == HIGH) && (digitalRead(rightSensor) == HIGH))
          {sqieez.OFF(); break;}
        while((digitalRead(updownSensor) == LOW) && (digitalRead(rightSensor) == HIGH))
          {right.OFF(); left.ON(); footFlag = 0; write_ds(0xEF, 0);FND++; //2020-06-13 풋스위치 업데이트 추가
          break;}
           
      } else if (start_pos == 1){  // 전 인쇄 모드 LED OFF 필요
        while((digitalRead(updownSensor) == LOW) && (digitalRead(leftSensor) == HIGH))
          {left.OFF(); right.ON();break;}
        while((digitalRead(updownSensor) == LOW) && (digitalRead(rightSensor) == HIGH))
          {sqieez.ON(); break;}
        while((digitalRead(updownSensor) == HIGH) && (digitalRead(rightSensor) == HIGH))
          {right.OFF(); left.ON();break;}
        while((digitalRead(updownSensor) == HIGH) && (digitalRead(leftSensor) == HIGH))
          {sqieez.OFF(); footFlag = 0; write_ds(0xEF, 0); FND++;    //2020-06-13 풋스위치 업데이트 추가
          break;}        
      } foot_Update(); 
    break;

    case 2: // 자동모드
      if (currentMillis - previousMillis >= CT){  
        if (start_pos == 0) {
        while((digitalRead(updownSensor) == LOW) && (digitalRead(leftSensor) == HIGH))
          {sqieez.ON(); foot_Update(); break;}
        while((digitalRead(updownSensor) == HIGH) && (digitalRead(leftSensor) == HIGH))
          {left.OFF(); right.ON();foot_Update(); break;}
        while((digitalRead(updownSensor) == HIGH) && (digitalRead(rightSensor) == HIGH))
          {sqieez.OFF(); foot_Update(); break;}
        while((digitalRead(updownSensor) == LOW) && (digitalRead(rightSensor) == HIGH))
          {right.OFF(); left.ON(); foot_Update(); FND++;  
          previousMillis = currentMillis; break;} 
        } 
        
        else if (start_pos == 1){    
        while((digitalRead(updownSensor) == LOW) && (digitalRead(leftSensor) == HIGH))
          {left.OFF(); right.ON();foot_Update(); break;}
        while((digitalRead(updownSensor) == LOW) && (digitalRead(rightSensor) == HIGH))
          {sqieez.ON(); foot_Update(); break;}
        while((digitalRead(updownSensor) == HIGH) && (digitalRead(rightSensor) == HIGH))
          {right.OFF(); left.ON();foot_Update(); break;}
        while((digitalRead(updownSensor) == HIGH) && (digitalRead(leftSensor) == HIGH))
          {sqieez.OFF(); foot_Update(); FND++;    
          previousMillis = currentMillis; break;}   
        }
      } 
    break;
    
    default:
    break;  
    }
  }

  if (jig == 1){
  unsigned long currentMillis = millis();
  
  switch (mode){
    case 1:         // 반자동 모드
      if (start_pos == 0) {                 // 후 인쇄 모드 LED ON 필요
        while((digitalRead(updownSensor) == LOW) && (digitalRead(leftSensor) == HIGH))
          {sqieez.ON(); acOut.Update(); break;}
        while((digitalRead(updownSensor) == HIGH) && (digitalRead(leftSensor) == HIGH))
          {left.OFF(); right.ON(); break;}
        while((digitalRead(updownSensor) == HIGH) && (digitalRead(rightSensor) == HIGH))
          {sqieez.OFF(); acOut.Update(); break;}
        while((digitalRead(updownSensor) == LOW) && (digitalRead(rightSensor) == HIGH))
          {right.OFF(); left.ON(); acOut.Update(); footFlag = 0; write_ds(0xEF, 0);FND++; //2020-06-13 풋스위치 업데이트 추가
          break;}
           
      } else if (start_pos == 1){  // 전 인쇄 모드 LED OFF 필요
        while((digitalRead(updownSensor) == LOW) && (digitalRead(leftSensor) == HIGH))
          {left.OFF(); right.ON(); acOut.Update(); break;}
        while((digitalRead(updownSensor) == LOW) && (digitalRead(rightSensor) == HIGH))
          {sqieez.ON(); break;}
        while((digitalRead(updownSensor) == HIGH) && (digitalRead(rightSensor) == HIGH))
          {right.OFF(); left.ON(); break;}
        while((digitalRead(updownSensor) == HIGH) && (digitalRead(leftSensor) == HIGH))
          {sqieez.OFF(); acOut.Update(); footFlag = 0; write_ds(0xEF, 0); FND++;    //2020-06-13 풋스위치 업데이트 추가
          break;}        
      } foot_Update(); 
    break;

    case 2: // 자동모드
      if (currentMillis - previousMillis >= CT){  
        if (start_pos == 0) {
        while((digitalRead(updownSensor) == LOW) && (digitalRead(leftSensor) == HIGH))
          {sqieez.ON(); acOut.Update(); foot_Update(); break;}
        while((digitalRead(updownSensor) == HIGH) && (digitalRead(leftSensor) == HIGH))
          {left.OFF(); right.ON();  foot_Update(); break;}
        while((digitalRead(updownSensor) == HIGH) && (digitalRead(rightSensor) == HIGH))
          {sqieez.OFF(); acOut.Update(); foot_Update(); break;}
        while((digitalRead(updownSensor) == LOW) && (digitalRead(rightSensor) == HIGH))
          {right.OFF(); left.ON(); acOut.Update(); foot_Update(); FND++;  
          previousMillis = currentMillis; break;} 
        } 
        
        else if (start_pos == 1){    
        while((digitalRead(updownSensor) == LOW) && (digitalRead(leftSensor) == HIGH))
          {left.OFF(); right.ON(); acOut.Update(); foot_Update(); break;}
        while((digitalRead(updownSensor) == LOW) && (digitalRead(rightSensor) == HIGH))
          {sqieez.ON(); foot_Update(); break;}
        while((digitalRead(updownSensor) == HIGH) && (digitalRead(rightSensor) == HIGH))
          {right.OFF(); left.ON(); foot_Update(); break;}
        while((digitalRead(updownSensor) == HIGH) && (digitalRead(leftSensor) == HIGH))
          {sqieez.OFF(); acOut.Update(); foot_Update(); FND++;    
          previousMillis = currentMillis; break;}   
        }
      } 
    break;
    
    default:
    break;  
    }
  }
}
