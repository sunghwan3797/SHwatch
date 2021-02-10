
void updown_show() {
  long startTime = millis();
  while ( (millis() - startTime) < 3000) {//cds time show : 5 sec
    displayNumber_for_updown(number, false); //Each call takes about 8ms, display the colon
  }
}
#ifdef PROTO_TYPE
//--------------------------------gps and ble-----------
void gps_func() {
  ss.begin(GPSBAUD);

  while (!(digitalRead(theButton) == LOW)) {
    get_gps();
  }
  ss.end();
}
void get_gps() {
  ss.listen();
  // This sketch displays information every time a new sentence is correctly encoded.
  while (ss.available() > 0)
    if (gps.encode(ss. read()))
      displayInfo();
}

void displayInfo()
{
  BluetoothWatch.print(F("Location: "));
  if (gps.location.isValid())
  {
    BluetoothWatch.print(gps.location.lat(), 6);
    BluetoothWatch.print(F(","));
    BluetoothWatch.print(gps.location.lng(), 6);
  }
  else
  {
    BluetoothWatch.print(F("INVALID"));
  }

  BluetoothWatch.println();
}
//--------------------------------gps and ble-----------
#endif
void on_time_showTime() {
  //this is show time very long.
  int cmbTime = (hours * 100) + minutes; //Combine the hours and minutes

  //Now show the time for a certain length of time
  long startTime = millis();
  while ( (millis() - startTime) < 10000) {//cds time show : 5 sec
    displayNumber(cmbTime, true); //Each call takes about 8ms, display the colon
  }
}

void light_time() {
  int cmbTime = (hours * 100) + minutes; //Combine the hours and minutes

  //Now show the time for a certain length of time
  long startTime = millis();
  while ( (millis() - startTime) < 100) {//cds time show : 5 sec
    displayNumber(cmbTime, true); //Each call takes about 8ms, display the colon
  }
}
void showyear() {//show year on display
  bool buttonhit = false;
  int show_year = year * 1;
  long nowprev = millis();
  while ((millis() - nowprev) <= 500) {
    displayNumber(show_year, false);

    if (digitalRead(theButton) == LOW) {
      buttonhit = true;
    }

    else if (buttonhit == true && digitalRead(theButton) == HIGH) {
      return;
    }
  }
}


void showday_name() {

  //Now show the letters for a certain length of time
  change_number_to_day(day_name);
  long startTime = millis();
  while ( (millis() - startTime) < 500) {
    displayLetters(name_day); //Each call takes about 8ms, display the colon

    //After the letters are displayed, the segments are turned off
    //We control the brightness by modifying how long we wait between re-paints of the display
    delayMicroseconds(display_brightness);
  }
}
void setyear(void) {

  int idleMiliseconds = 0;
  //2초간 버튼을 제어하지 않으면 셋타임을 탈출함

  int buttonHold = 0;
  //This counts the number of times you are holding the button down consecutively
  //Once we notice you're really holding the button down a lot, we will speed up the minute counter

  while (idleMiliseconds < 2000) {//2초보다 작을 때만 실시
    int set_year = year; //Combine the hours and minutes

    for (int x = 0 ; x < 10 ; x++) {
      displayNumber(set_year, true); //Each call takes about 8ms, display the colon for about 100ms
      delayMicroseconds(display_brightness); //Wait before we paint the display again
    }
    for (int x = 0 ; x < 10 ; x++) {
      displayNumber(set_year, false); //Each call takes about 8ms, turn off the colon for about 100ms
      delayMicroseconds(display_brightness); //Wait before we paint the display again
    }

    //If you're still hitting the button, then increase the time and reset the idleMili timeout variable
    if (digitalRead(theButton) == LOW) {
      idleMiliseconds = 0;

      buttonHold++;
      //------------------------
      if (buttonHold < 5) //10 = 2 seconds
        year++; //Advance the minutes
      else {
        //Advance the minutes faster because you're holding the button for 10 seconds
        //Start advancing on the tens digit. Floor the single minute digit.
        year--;
      }
      //------------------
    }
    else
      buttonHold = 0;

    idleMiliseconds += 200;
  }
}

void showdate() {//show date on display
  bool buttonhit = false;

  int mix_date = (month * 100) + day;
  long prev = millis();
  while ((millis() - prev) <= 500) {
    displayNumber(mix_date, false);
    if (digitalRead(theButton) == LOW) {
      buttonhit = true;
    }

    else if (buttonhit == true && digitalRead(theButton) == HIGH) {
      return;
    }

  }
}

void showsec() {
  bool buttonhit = false;

  int combinedsec = (minutes * 100) + seconds; //For testing, combine the minutes and seconds
  long prev = millis();
  while ((millis() - prev) <= show_time_length) {
    displayNumber(combinedsec, false);

    if (digitalRead(theButton) == LOW) {
      buttonhit = true;
    }

    else if (buttonhit == true && digitalRead(theButton) == HIGH) {
      return;
    }
  }
}

void showTime() {

  //Here is where we display the time and PWM the segments
  //Display brightness changes based on color, red is too bright, blue is very dull
  //By painting the time, then turning off for a number of microseconds, we can control how bright the display is

  //Each digit is on for a certain amount of microseconds
  //Then it is turned off and we go to the next digit
  //We loop through each digit until we reach the show_time_length (usually about 2 seconds)

  //For the default red display:
  //Let's define a variable called display_brightness that varies from:
  //5000 blindingly bright (15.7mA current draw per digit)
  //2000 shockingly bright (11.4mA current draw per digit)
  //1000 pretty bright (5.9mA)
  //500 normal (3mA)
  //200 dim but readable (1.4mA)
  //50 dim but readable (0.56mA)
  //5 dim but readable (0.31mA)
  //1 dim but readable in dark (0.28mA)
  //#ifdef ampm_mode
  // int combinedTime = (newh * 100) + minutes; //Combine the hours and minutes
  //#else
  int combinedTime = (hours * 100) + minutes; //Combine the hours and minutes
  //#endif
  boolean buttonPreviouslyHit = false;

  //Now show the time for a certain length of time
  long startTime = millis();
  while ( (millis() - startTime) < show_time_length) {
    //#ifdef ampm_mode
    //   displayNumber_AMPM(combinedTime, true, ampm_want); //Each call takes about 8ms, display the colon
    //#else
    displayNumber(combinedTime, true); //Each call takes about 8ms, display the colon
    //#endif
    //After the time is displayed, the segments are turned off
    //We control the brightness by modifying how long we wait between re-paints of the display
    //delayMicroseconds(display_brightness);

    //If you have hit and released the button while the display is on, start the IR off sequence
    if (digitalRead(theButton) == LOW) {
      buttonPreviouslyHit = true;
    }
    else if ( (buttonPreviouslyHit == true) && (digitalRead(theButton) == HIGH) ) {

      return;//호출된 곳으로
    }
  }

}
#ifdef ENABLE_TVBGONE
void tv_b_gone() {
  //unsigned long delay_time=millis();
  //Serial.println(delay_time,DEC);
#ifdef ENABLE_TVBGONE
  //Disable TIMER2 for IR control
  TCCR2A = 0x00;
  TCCR2B = 0;
  ASSR = 0;
  TIMSK2 = 0; //Enable the timer 2 interrupt

  //Serial.println(delay_time,DEC);
  //Turn off all the things!
  sendAllCodes();

  //Setup TIMER2
  TCCR2A = 0x00;
  //TCCR2B = (1<<CS22)|(1<<CS20); //Set CLK/128 or overflow interrupt every 1s
  TCCR2B = (1 << CS22) | (1 << CS21) | (1 << CS20); //Set CLK/1024 or overflow interrupt every 8s
  ASSR = (1 << AS2); //Enable asynchronous operation
  TIMSK2 = (1 << TOIE2); //Enable the timer 2 interrupt

  quickflashLEDx(2); //Blink Colons twice letting us know it's done
#endif

  //Serial.println(delay_time,DEC);
  //업데이트
  seconds = seconds + 45;
}
//Displays a string on the display
//Used to indicate which code is loaded onto the watch
#endif
void showColor(char *colorName) {

  //Now show the letters for a certain length of time
  long startTime = millis();
  while ( (millis() - startTime) < show_time_length) {
    displayLetters(colorName); //Each call takes about 8ms, display the colon

    //After the letters are displayed, the segments are turned off
    //We control the brightness by modifying how long we wait between re-paints of the display
    delayMicroseconds(display_brightness);
  }
}

//Displays a string on the display
//Used to indicate which code is loaded onto the watch
void showmsg(char *colorName) {

  //Now show the letters for a certain length of time
  long startTime = millis();
  while ( (millis() - startTime) < counttime) {
    displayLetters(colorName); //Each call takes about 8ms, display the colon

    //After the letters are displayed, the segments are turned off
    //We control the brightness by modifying how long we wait between re-paints of the display
    delayMicroseconds(display_brightness);
  }
}

void shownum(int num) {
  long prev = millis();
  while ((millis() - prev) <= 500) {
    displayNumber(num, false);
  }
}
/*
  void updown_game() {

  int count = 0;//저장기능 카운트
  int set_count = 0;//기회 소진시 랜덤 함수 바꿈
  if (save_and_continue == false) {//저장되어 있지 않을 때
    quiz = 0;//랜덤 퀴즈 값 초기화
    number = 0;//유저 값 초기화

    showColor("easy");
    if (ver_easy == true) shownum(1);
    else shownum(2);

    if (digitalRead(theButton) == LOW) {//난이도 선택
      if (ver_easy == true)
      {
        ver_easy = false;
        shownum(2);
      }
      else
      {
        ver_easy = true;
        shownum(1);
      }

    }

    showColor("hard");
    if (hard == true) shownum(2);
    else shownum(1);
    if (digitalRead(theButton) == LOW) {
      if (hard == true) {
        hard = false;
        showColor("soft");
      }
      else {
        hard = true;
        showColor("hard");
      }
    }


    if (ver_easy == true) quiz = random(0, 100);//99까지 랜덤
    else quiz = random(0, 9999);//9999까지 랜덤
  }
  showColor("----");  //ready
  save_and_continue = false;
  while (number != quiz &&  save_and_continue == false  ) { //긴급종료 및 저장, 난이도 3 : 일정횟수 못맞출시 랜덤 함수 초기화 반복(무한 난이도)
    if (set_count > 25) {//리셋 카운트가 15회 이상이면 답을 바꿈
      if (ver_easy == true) quiz = random(0, 100);
      else quiz = random(0, 9999);
      set_count = 0;//리셋 카운트 제로
    }
    if (number > quiz ) {
      showColor("down");
      if (hard == true) {
        set_count++; //리셋 카운터
      }
      int anum = number;//자동저장을 위한 넘버의 이전 값
      setupdown_for_m();
      int bnum = number;//자동저장을 위한 넘버의 이후 값(아무것도 건들지 않으면 이전,이후 값이 같음)
      if (anum == bnum) { //아무것도 안하고 3번 카운트를 셀 때
        count++;//긴급탈출 세이브 카운트 증가
        if (count == 3) {
          save_and_continue = true;//긴급 종료 및 number 값 저장
        }
      }
    }
    else {
      showColor("  up");
      if (hard == true) set_count++;//리셋 카운터
      setupdown_for_p();
    }
  }
  showColor(" win");
  }
*/
//This routine occurs when you hold the button down
//The colon blinks indicating we are in this mode
//Holding the button down will increase the time (accelerates)
//Releasing the button for more than 2 seconds will exit this mode
void setTime(void) {

  int idleMiliseconds = 0;
  //2초간 버튼을 제어하지 않으면 셋타임을 탈출함

  int buttonHold = 0;
  //This counts the number of times you are holding the button down consecutively
  //Once we notice you're really holding the button down a lot, we will speed up the minute counter

  while (idleMiliseconds < 2000) {//2초보다 작을 때만 실시

    cli(); //We don't want the interrupt changing values at the same time we are!

    //Update the minutes and hours variables
    minutes += seconds / 60; //Example: seconds = 2317, minutes = 58 + 38 = 96
    seconds %= 60; //seconds = 37
    hours += minutes / 60; //12 + (96 / 60) = 13
    minutes %= 60; //minutes = 36

    //Do we display 12 hour or 24 hour time?
    if (TwelveHourMode == true) {
      //In 12 hour mode, hours go from 12 to 1 to 12.
      while (hours > 12) hours -= 12;
    }
    else {
      //In 24 hour mode, hours go from 0 to 23 to 0.
      while (hours > 23) hours -= 24;


    }

    sei(); //Resume interrupts

    int combinedTime = (hours * 100) + minutes; //Combine the hours and minutes

    for (int x = 0 ; x < 10 ; x++) {
      displayNumber(combinedTime, true); //Each call takes about 8ms, display the colon for about 100ms
      delayMicroseconds(display_brightness); //Wait before we paint the display again
    }
    for (int x = 0 ; x < 10 ; x++) {
      displayNumber(combinedTime, false); //Each call takes about 8ms, turn off the colon for about 100ms
      delayMicroseconds(display_brightness); //Wait before we paint the display again
    }

    //If you're still hitting the button, then increase the time and reset the idleMili timeout variable
    if (digitalRead(theButton) == LOW) {
      idleMiliseconds = 0;

      buttonHold++;
      //------------------------

      if (buttonHold < 3 ) { //10 = 2 seconds
        if (minutes > 59) {
          minutes = 59;
        }
        minutes--; //Advance the minutes
      }
      else {
        //Advance the minutes faster because you're holding the button for 10 seconds
        //Start advancing on the tens digit. Floor the single minute digit.
        minutes /= 10; //minutes = 46 / 10 = 4
        minutes *= 10; //minutes = 4 * 10 = 40
        minutes += 10;  //minutes = 40 + 10 = 50
        /*  if (minutes < 10) {
            minutes = 10;
          }
          else if (minutes < 30) {
            minutes = 30;
          } else if (minutes <= 59) {
            minutes = 50;
          }
        */
      }
      //------------------
    }
    else
      buttonHold = 0;

    idleMiliseconds += 200;
  }
}

//This is a not-so-accurate delay routine
//Calling fake_msdelay(100) will delay for about 100ms
//Assumes 8MHz clock
/*void fake_msdelay(int x){
  for( ; x > 0 ; x--)
  fake_usdelay(1000);
  }*/

//This is a not-so-accurate delay routine
//Calling fake_usdelay(100) will delay for about 100us
//Assumes 8MHz clock
/*void fake_usdelay(int x){
  for( ; x > 0 ; x--) {
  __asm__("nop\n\t");
  __asm__("nop\n\t");
  __asm__("nop\n\t");
  __asm__("nop\n\t");
  __asm__("nop\n\t");
  __asm__("nop\n\t");
  __asm__("nop\n\t");
  }
  }*/

//Given 1022, we display "10:22"
//Each digit is displayed for ~2000us, and cycles through the 4 digits
//After running through the 4 numbers, the display is turned off
void displayNumber(int toDisplay, boolean displayColon) {

#define DIGIT_ON  HIGH
#define DIGIT_OFF  LOW

  for (int digit = 4 ; digit > 0 ; digit--) {

    //Turn on a digit for a short amount of time
    switch (digit) {
      case 1:
        digitalWrite(digit1, DIGIT_ON);
        break;
      case 2:
        digitalWrite(digit2, DIGIT_ON);
        if (displayColon == true) digitalWrite(colons, DIGIT_ON); //When we update digit 2, let's turn on colons as well
        break;
      case 3:
        digitalWrite(digit3, DIGIT_ON);
        break;
      case 4:
        digitalWrite(digit4, DIGIT_ON);
        break;
    }

    //Now display this digit
    if (digit > 1)
      lightNumber(toDisplay % 10); //Turn on the right segments for this digit
    else if (digit == 1) { //Special case on first digit, don't display 02:11, display 2:11
      if ( (toDisplay % 10) != 0) //If we are on the first digit, and it's not zero . 0과 같지 않을 때
        lightNumber(toDisplay % 10); //Turn on the right segments for this digit
    }//0일때는 조건문을 탈출하여 표시를 하지 않음.

    /*
       base=count-((t*1000)+(s*100)+(f*10));
    */

    toDisplay /= 10;  // a = a / 10 ; -> 숫자를 분리해서 한 자리 씩 출력하는 공식.;-> 1의 자리에서 부터 출력

    delayMicroseconds(2000); //Display this digit for a fraction of a second (between 1us and 5000us, 500-2000 is pretty good)
    //If you set this too long, the display will start to flicker. Set it to 25000 for some fun.

    //Turn off all segments
    lightNumber(10);

    //Turn off all digits
    digitalWrite(digit1, DIGIT_OFF);
    digitalWrite(digit2, DIGIT_OFF);
    digitalWrite(digit3, DIGIT_OFF);
    digitalWrite(digit4, DIGIT_OFF);
    digitalWrite(colons, DIGIT_OFF);
    //    digitalWrite(ampm, DIGIT_OFF);
  }

}
/*
  void displayNumber_AMPM(int toDisplay, boolean displayColon, bool mode) {

  #define DIGIT_ON  HIGH
  #define DIGIT_OFF  LOW

  for (int digit = 4 ; digit > 0 ; digit--) {

    //Turn on a digit for a short amount of time
    switch (digit) {
      case 1:
        digitalWrite(digit1, DIGIT_ON);
        break;
      case 2:
        digitalWrite(digit2, DIGIT_ON);
        if (displayColon == true) digitalWrite(colons, DIGIT_ON); //When we update digit 2, let's turn on colons as well
        if (mode == true) digitalWrite(ampm, HIGH);
        else digitalWrite(ampm, LOW);
        break;
      case 3:
        digitalWrite(digit3, DIGIT_ON);
        break;
      case 4:
        digitalWrite(digit4, DIGIT_ON);
        break;
    }

    //Now display this digit
    if (digit > 1)
      lightNumber(toDisplay % 10); //Turn on the right segments for this digit
    else if (digit == 1) { //Special case on first digit, don't display 02:11, display 2:11
      if ( (toDisplay % 10) != 0) //If we are on the first digit, and it's not zero . 0과 같지 않을 때
        lightNumber(toDisplay % 10); //Turn on the right segments for this digit
    }//0일때는 조건문을 탈출하여 표시를 하지 않음.


    base = count - ((t * 1000) + (s * 100) + (f * 10));

    toDisplay /= 10;  // a = a / 10 ; -> 숫자를 분리해서 한 자리 씩 출력하는 공식.;-> 1의 자리에서 부터 출력

    delayMicroseconds(2000); //Display this digit for a fraction of a second (between 1us and 5000us, 500-2000 is pretty good)
    //If you set this too long, the display will start to flicker. Set it to 25000 for some fun.

    //Turn off all segments
    lightNumber(10);

    //Turn off all digits
    digitalWrite(digit1, DIGIT_OFF);
    digitalWrite(digit2, DIGIT_OFF);
    digitalWrite(digit3, DIGIT_OFF);
    digitalWrite(digit4, DIGIT_OFF);
    digitalWrite(colons, DIGIT_OFF);
    digitalWrite(ampm, DIGIT_OFF);
  }

  }
*/
void setupdown_for_p(void) {

  int idleMiliseconds = 0;
  //2초간 버튼을 제어하지 않으면 셋타임을 탈출함

  int buttonHold = 0;
  //This counts the number of times you are holding the button down consecutively
  //Once we notice you're really holding the button down a lot, we will speed up the minute counter

  while (idleMiliseconds < 2000) {//2초보다 작을 때만 실시


    for (int x = 0 ; x < 10 ; x++) {
      displayNumber_for_updown(number, true); //Each call takes about 8ms, display the colon for about 100ms
      delayMicroseconds(display_brightness); //Wait before we paint the display again
    }
    for (int x = 0 ; x < 10 ; x++) {
      displayNumber_for_updown(number, false); //Each call takes about 8ms, turn off the colon for about 100ms
      delayMicroseconds(display_brightness); //Wait before we paint the display again
    }

    //If you're still hitting the button, then increase the time and reset the idleMili timeout variable
    if (digitalRead(theButton) == LOW) {
      idleMiliseconds = 0;

      buttonHold++;
      //------------------------
      if (buttonHold < 10) //10 = 2 seconds
        number++; //Advance the minutes
      else {
        //Advance the minutes faster because you're holding the button for 10 seconds
        //Start advancing on the tens digit. Floor the single minute digit.

        if (ver_easy == true) {

          number /= 10; //minutes = 46 / 10 = 4
          number *= 10; //minutes = 4 * 10 = 40
          number += 10;  //minutes = 40 + 10 = 50
        }
        else {
          number /= 100; //minutes = 46 / 10 = 4
          number *= 100; //minutes = 4 * 10 = 40
          number += 100;  //minutes = 40 + 10 = 50
        }
      }
      //------------------
    }
    else
      buttonHold = 0;

    idleMiliseconds += 200;
  }
}
void setupdown_for_m(void) {

  int idleMiliseconds = 0;
  //2초간 버튼을 제어하지 않으면 셋타임을 탈출함

  int buttonHold = 0;
  //This counts the number of times you are holding the button down consecutively
  //Once we notice you're really holding the button down a lot, we will speed up the minute counter

  while (idleMiliseconds < 2000) {//2초보다 작을 때만 실시


    for (int x = 0 ; x < 10 ; x++) {
      displayNumber_for_updown(number, true); //Each call takes about 8ms, display the colon for about 100ms
      delayMicroseconds(display_brightness); //Wait before we paint the display again
    }
    for (int x = 0 ; x < 10 ; x++) {
      displayNumber_for_updown(number, false); //Each call takes about 8ms, turn off the colon for about 100ms
      delayMicroseconds(display_brightness); //Wait before we paint the display again
    }

    //If you're still hitting the button, then increase the time and reset the idleMili timeout variable
    if (digitalRead(theButton) == LOW) {
      idleMiliseconds = 0;

      buttonHold++;
      //------------------------
      if (buttonHold < 10) //10 = 2 seconds
        number--; //Advance the minutes
      else {
        //Advance the minutes faster because you're holding the button for 10 seconds
        //Start advancing on the tens digit. Floor the single minute digit.
        if (ver_easy == true) {

          number /= 10; //minutes = 46 / 10 = 4
          number *= 10; //minutes = 4 * 10 = 40
          number -= 10;  //minutes = 40 + 10 = 50
        }
        else {
          number /= 100; //minutes = 46 / 10 = 4
          number *= 100; //minutes = 4 * 10 = 40
          number -= 100;  //minutes = 40 + 10 = 50
        }
      }
      //------------------
    }
    else
      buttonHold = 0;

    idleMiliseconds += 200;
  }
}

void setupdown_ten(void) {

  int idleMiliseconds = 0;
  //2초간 버튼을 제어하지 않으면 셋타임을 탈출함

  int buttonHold = 0;
  //This counts the number of times you are holding the button down consecutively
  //Once we notice you're really holding the button down a lot, we will speed up the minute counter

  while (idleMiliseconds < 2000) {//2초보다 작을 때만 실시


    for (int x = 0 ; x < 10 ; x++) {
      displayNumber_for_updown(number, true); //Each call takes about 8ms, display the colon for about 100ms
      delayMicroseconds(display_brightness); //Wait before we paint the display again
    }
    for (int x = 0 ; x < 10 ; x++) {
      displayNumber_for_updown(number, false); //Each call takes about 8ms, turn off the colon for about 100ms
      delayMicroseconds(display_brightness); //Wait before we paint the display again
    }

    //If you're still hitting the button, then increase the time and reset the idleMili timeout variable
    if (digitalRead(theButton) == LOW) {
      idleMiliseconds = 0;

      //buttonHold++;
      //------------------------
      if (buttonHold < 10) //10 = 2 seconds
        number++; //Advance the minutes
      /* else {
         //Advance the minutes faster because you're holding the button for 10 seconds
         //Start advancing on the tens digit. Floor the single minute digit.
         number /= 100; //minutes = 46 / 10 = 4
         number *= 100; //minutes = 4 * 10 = 40
         number += 100;  //minutes = 40 + 10 = 50
        }*/
      //------------------
    }
    else
      buttonHold = 0;

    idleMiliseconds += 200;
  }
}
void displayNumber_for_numcursor(int toDisplay, boolean displayColon) {

#define DIGIT_ON  HIGH
#define DIGIT_OFF  LOW




  for (int digit = 4 ; digit > 0 ; digit--) {

    //Turn on a digit for a short amount of time
    switch (digit) {
      case 1:
        digitalWrite(digit1, DIGIT_ON);//천의 자리
        break;
      case 2:
        digitalWrite(digit2, DIGIT_ON);
        if (displayColon == true) digitalWrite(colons, DIGIT_ON); //When we update digit 2, let's turn on colons as well
        break;
      case 3:
        digitalWrite(digit3, DIGIT_ON);
        break;
      case 4:
        digitalWrite(digit4, DIGIT_ON);//일의 자리
        break;
    }

    //Now display this digit

    lightNumber(toDisplay % 10);//we want all segment umber display.


    toDisplay /= 10;  // a = a / 10 ; -> 숫자를 분리해서 한 자리 씩 출력하는 공식.;-> 1의 자리에서 부터 출력

    delayMicroseconds(2000); //Display this digit for a fraction of a second (between 1us and 5000us, 500-2000 is pretty good)
    //If you set this too long, the display will start to flicker. Set it to 25000 for some fun.

    //Turn off all segments
    lightNumber(10);

    //Turn off all digits
    digitalWrite(digit1, DIGIT_OFF);
    digitalWrite(digit2, DIGIT_OFF);
    digitalWrite(digit3, DIGIT_OFF);
    digitalWrite(digit4, DIGIT_OFF);
    digitalWrite(colons, DIGIT_OFF);
    //  digitalWrite(ampm, DIGIT_OFF);
  }

}

void displayNumber_for_updown(int toDisplay, boolean displayColon) {

#define DIGIT_ON  HIGH
#define DIGIT_OFF  LOW




  for (int digit = 4 ; digit > 0 ; digit--) {

    //Turn on a digit for a short amount of time
    switch (digit) {
      case 1:
        digitalWrite(digit1, DIGIT_ON);//천의 자리
        break;
      case 2:
        digitalWrite(digit2, DIGIT_ON);
        if (displayColon == true) digitalWrite(colons, DIGIT_ON); //When we update digit 2, let's turn on colons as well
        break;
      case 3:
        digitalWrite(digit3, DIGIT_ON);
        break;
      case 4:
        digitalWrite(digit4, DIGIT_ON);//일의 자리
        break;
    }

    //Now display this digit

    lightNumber(toDisplay % 10);//we want all segment umber display.


    toDisplay /= 10;  // a = a / 10 ; -> 숫자를 분리해서 한 자리 씩 출력하는 공식.;-> 1의 자리에서 부터 출력

    delayMicroseconds(2000); //Display this digit for a fraction of a second (between 1us and 5000us, 500-2000 is pretty good)
    //If you set this too long, the display will start to flicker. Set it to 25000 for some fun.

    //Turn off all segments
    lightNumber(10);

    //Turn off all digits
    digitalWrite(digit1, DIGIT_OFF);
    digitalWrite(digit2, DIGIT_OFF);
    digitalWrite(digit3, DIGIT_OFF);
    digitalWrite(digit4, DIGIT_OFF);
    digitalWrite(colons, DIGIT_OFF);
    // digitalWrite(ampm, DIGIT_OFF);
  }

}



//Takes a string like "gren" and displays it, left justified
//We don't use the colons, or AMPM dot, so they are turned off
void displayLetters(char *colorName) {
#define DIGIT_ON  HIGH
#define DIGIT_OFF  LOW

  digitalWrite(digit4, DIGIT_OFF);
  digitalWrite(colons, DIGIT_OFF);
  // digitalWrite(ampm, DIGIT_OFF);

  for (int digit = 0 ; digit < 4 ; digit++) {
    //Turn on a digit for a short amount of time
    switch (digit) {
      case 0:
        digitalWrite(digit1, DIGIT_ON);
        break;
      case 1:
        digitalWrite(digit2, DIGIT_ON);
        break;
      case 2:
        digitalWrite(digit3, DIGIT_ON);
        break;
      case 3:
        digitalWrite(digit4, DIGIT_ON);
        break;
    }

    //Now display this letter
    lightNumber(colorName[digit]); //Turn on the right segments for this letter

    delayMicroseconds(2000); //Display this digit for a fraction of a second (between 1us and 5000us, 500-2000 is pretty good)
    //If you set this too long, the display will start to flicker. Set it to 25000 for some fun.

    //Turn off all segments
    lightNumber(10);

    //Turn off all digits
    digitalWrite(digit1, DIGIT_OFF);
    digitalWrite(digit2, DIGIT_OFF);
    digitalWrite(digit3, DIGIT_OFF);
  }
}

//Given a number, turns on those segments
//If number == 10, then turn off all segments
void lightNumber(int numberToDisplay) {

#define SEGMENT_ON  LOW
#define SEGMENT_OFF HIGH

  /*
    Segments
    -  A
    F / / B
    -  G
    E / / C
    -  D
  */

  switch (numberToDisplay) {

    case 0:
      digitalWrite(segA, SEGMENT_ON);
      digitalWrite(segB, SEGMENT_ON);
      digitalWrite(segC, SEGMENT_ON);
      digitalWrite(segD, SEGMENT_ON);
      digitalWrite(segE, SEGMENT_ON);
      digitalWrite(segF, SEGMENT_ON);
      break;

    case 1:
      digitalWrite(segB, SEGMENT_ON);
      digitalWrite(segC, SEGMENT_ON);
      break;

    case 2:
      digitalWrite(segA, SEGMENT_ON);
      digitalWrite(segB, SEGMENT_ON);
      digitalWrite(segD, SEGMENT_ON);
      digitalWrite(segE, SEGMENT_ON);
      digitalWrite(segG, SEGMENT_ON);
      break;

    case 3:
      digitalWrite(segA, SEGMENT_ON);
      digitalWrite(segB, SEGMENT_ON);
      digitalWrite(segC, SEGMENT_ON);
      digitalWrite(segD, SEGMENT_ON);
      digitalWrite(segG, SEGMENT_ON);
      break;

    case 4:
      digitalWrite(segB, SEGMENT_ON);
      digitalWrite(segC, SEGMENT_ON);
      digitalWrite(segF, SEGMENT_ON);
      digitalWrite(segG, SEGMENT_ON);
      break;

    case 5:
      digitalWrite(segA, SEGMENT_ON);
      digitalWrite(segC, SEGMENT_ON);
      digitalWrite(segD, SEGMENT_ON);
      digitalWrite(segF, SEGMENT_ON);
      digitalWrite(segG, SEGMENT_ON);
      break;

    case 6:
      digitalWrite(segA, SEGMENT_ON);
      digitalWrite(segC, SEGMENT_ON);
      digitalWrite(segD, SEGMENT_ON);
      digitalWrite(segE, SEGMENT_ON);
      digitalWrite(segF, SEGMENT_ON);
      digitalWrite(segG, SEGMENT_ON);
      break;

    case 7:
      digitalWrite(segA, SEGMENT_ON);
      digitalWrite(segB, SEGMENT_ON);
      digitalWrite(segC, SEGMENT_ON);
      break;

    case 8:
      digitalWrite(segA, SEGMENT_ON);
      digitalWrite(segB, SEGMENT_ON);
      digitalWrite(segC, SEGMENT_ON);
      digitalWrite(segD, SEGMENT_ON);
      digitalWrite(segE, SEGMENT_ON);
      digitalWrite(segF, SEGMENT_ON);
      digitalWrite(segG, SEGMENT_ON);
      break;

    case 9:
      digitalWrite(segA, SEGMENT_ON);
      digitalWrite(segB, SEGMENT_ON);
      digitalWrite(segC, SEGMENT_ON);
      digitalWrite(segD, SEGMENT_ON);
      digitalWrite(segF, SEGMENT_ON);
      digitalWrite(segG, SEGMENT_ON);
      break;

    case 10:
      digitalWrite(segA, SEGMENT_OFF);
      digitalWrite(segB, SEGMENT_OFF);
      digitalWrite(segC, SEGMENT_OFF);
      digitalWrite(segD, SEGMENT_OFF);
      digitalWrite(segE, SEGMENT_OFF);
      digitalWrite(segF, SEGMENT_OFF);
      digitalWrite(segG, SEGMENT_OFF);
      break;

    /*
      Segments
      -  A
      F / / B
      -  G
      E / / C
      - D
    */

    //Letters
    case 'b': //cdefg
      digitalWrite(segC, SEGMENT_ON);
      digitalWrite(segD, SEGMENT_ON);
      digitalWrite(segE, SEGMENT_ON);
      digitalWrite(segF, SEGMENT_ON);
      digitalWrite(segG, SEGMENT_ON);
      break;
    case 'v': //cdefg
      digitalWrite(segC, SEGMENT_ON);
      digitalWrite(segD, SEGMENT_ON);
      digitalWrite(segE, SEGMENT_ON);
      digitalWrite(segF, SEGMENT_ON);
      digitalWrite(segB, SEGMENT_ON);
      break;

    case 's': //cdefg
      digitalWrite(segC, SEGMENT_ON);
      digitalWrite(segD, SEGMENT_ON);
      digitalWrite(segA, SEGMENT_ON);
      digitalWrite(segF, SEGMENT_ON);
      digitalWrite(segG, SEGMENT_ON);
      break;
    case 'h': //cefg
      digitalWrite(segC, SEGMENT_ON);
      digitalWrite(segE, SEGMENT_ON);
      digitalWrite(segF, SEGMENT_ON);
      digitalWrite(segG, SEGMENT_ON);
      break;

    case 'L':
    case 'l': //def
      digitalWrite(segD, SEGMENT_ON);
      digitalWrite(segE, SEGMENT_ON);
      digitalWrite(segF, SEGMENT_ON);
      break;
    case 'u': //cde
      digitalWrite(segC, SEGMENT_ON);
      digitalWrite(segD, SEGMENT_ON);
      digitalWrite(segE, SEGMENT_ON);
      break;

    case 'g': //abcdfg
      digitalWrite(segA, SEGMENT_ON);
      digitalWrite(segB, SEGMENT_ON);
      digitalWrite(segC, SEGMENT_ON);
      digitalWrite(segD, SEGMENT_ON);
      digitalWrite(segF, SEGMENT_ON);
      digitalWrite(segG, SEGMENT_ON);
      break;
    case 'r': //eg
      digitalWrite(segE, SEGMENT_ON);
      digitalWrite(segG, SEGMENT_ON);
      break;
    case 'n': //ceg
      digitalWrite(segC, SEGMENT_ON);
      digitalWrite(segE, SEGMENT_ON);
      digitalWrite(segG, SEGMENT_ON);
      break;

    //case r
    case 'e': //adefg
      digitalWrite(segA, SEGMENT_ON);
      digitalWrite(segD, SEGMENT_ON);
      digitalWrite(segE, SEGMENT_ON);
      digitalWrite(segF, SEGMENT_ON);
      digitalWrite(segG, SEGMENT_ON);
      break;
    case 'd': //bcdeg
      digitalWrite(segB, SEGMENT_ON);
      digitalWrite(segC, SEGMENT_ON);
      digitalWrite(segD, SEGMENT_ON);
      digitalWrite(segE, SEGMENT_ON);
      digitalWrite(segG, SEGMENT_ON);
      break;
    case 'f': //bcdeg
      digitalWrite(segA, SEGMENT_ON);
      digitalWrite(segF, SEGMENT_ON);
      digitalWrite(segE, SEGMENT_ON);
      digitalWrite(segG, SEGMENT_ON);
      break;
    case '-': //special text
      digitalWrite(segG, SEGMENT_ON);
      break;
    case ' ': //None
      digitalWrite(segA, SEGMENT_OFF);
      digitalWrite(segB, SEGMENT_OFF);
      digitalWrite(segC, SEGMENT_OFF);
      digitalWrite(segD, SEGMENT_OFF);
      digitalWrite(segE, SEGMENT_OFF);
      digitalWrite(segF, SEGMENT_OFF);
      digitalWrite(segG, SEGMENT_OFF);
      break;

    case 'y': //bcdfg
      digitalWrite(segB, SEGMENT_ON);
      digitalWrite(segC, SEGMENT_ON);
      digitalWrite(segD, SEGMENT_ON);
      digitalWrite(segF, SEGMENT_ON);
      digitalWrite(segG, SEGMENT_ON);
      break;
    //case e
    //case L
    case 'o': //cdeg
      digitalWrite(segC, SEGMENT_ON);
      digitalWrite(segD, SEGMENT_ON);
      digitalWrite(segE, SEGMENT_ON);
      digitalWrite(segG, SEGMENT_ON);
      break;
    case 't': //cdeg
      digitalWrite(segF, SEGMENT_ON);
      digitalWrite(segG, SEGMENT_ON);
      digitalWrite(segE, SEGMENT_ON);
      digitalWrite(segD, SEGMENT_ON);
      break;
    case 'i': //cdeg
      digitalWrite(segC, SEGMENT_ON);
      digitalWrite(segB, SEGMENT_ON);
      break;

    case 'c': //deg
      digitalWrite(segD, SEGMENT_ON);
      digitalWrite(segE, SEGMENT_ON);
      digitalWrite(segG, SEGMENT_ON);
      break;
    case 'a': //cdeg
      digitalWrite(segC, SEGMENT_ON);
      digitalWrite(segA, SEGMENT_ON);
      digitalWrite(segB, SEGMENT_ON);
      digitalWrite(segE, SEGMENT_ON);
      digitalWrite(segF, SEGMENT_ON);
      digitalWrite(segG, SEGMENT_ON);
      break;
    case 'k': //cdeg
      digitalWrite(segC, SEGMENT_ON);
      digitalWrite(segA, SEGMENT_ON);
      digitalWrite(segE, SEGMENT_ON);
      digitalWrite(segF, SEGMENT_ON);
      digitalWrite(segG, SEGMENT_ON);
      break;
    case 'w': //cdeg
      digitalWrite(segC, SEGMENT_ON);
      digitalWrite(segA, SEGMENT_ON);
      digitalWrite(segE, SEGMENT_ON);
      digitalWrite(segD, SEGMENT_ON);
      break;
    case 'm': //cdeg
      digitalWrite(segC, SEGMENT_ON);
      digitalWrite(segA, SEGMENT_ON);
      digitalWrite(segE, SEGMENT_ON);
      digitalWrite(segG, SEGMENT_ON);
      break;
    case 'p': //cdeg
      digitalWrite(segB, SEGMENT_ON);
      digitalWrite(segA, SEGMENT_ON);
      digitalWrite(segF, SEGMENT_ON);
      digitalWrite(segE, SEGMENT_ON);
      digitalWrite(segG, SEGMENT_ON);
      break;

      //---------------------------------

  }
}


//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=

//and gate

/*
  //test function
  void set_and_move_cursor(void) {

  int idleMiliseconds = 0;
  //2초간 버튼을 제어하지 않으면 셋타임을 탈출함

  int buttonHold = 0;
  //This counts the number of times you are holding the button down consecutively
  //Once we notice you're really holding the button down a lot, we will speed up the minute counter

  while (idleMiliseconds < 2000) {//2초보다 작을 때만 실시

    //여백


    //int combinedTime = (hours * 100) + minutes; //Combine the hours and minutes
    for (int y = 0; y < 2; y++) {
      for (int x = 0 ; x < 5 ; x++) {
        displayNumber_for_cursor(number, cursor, true); //Each call takes about 8ms, display the colon for about 100ms
        delayMicroseconds(display_brightness); //Wait before we paint the display again
      }
      for (int x = 0 ; x < 5 ; x++) {
        displayNumber_for_cursor(number, cursor, false); //Each call takes about 8ms, turn off the colon for about 100ms
        delayMicroseconds(display_brightness); //Wait before we paint the display again
      }
    }
    //-------------------------------
    int originDigitNum = 0;
    if (cursor == 4)   ///일의 자리일때
      originDigitNum = number % 10;  //완전체분해
    else if (cursor == 3)
      originDigitNum = number / 10 % 10;
    else if (cursor == 2)
      originDigitNum = number / 100 % 10;
    else if (cursor == 1)
      originDigitNum = number / 1000;
    newDigitNum = originDigitNum;
    //---------------------------------
    //If you're still hitting the button, then increase the time and reset the idleMili timeout variable
    if (digitalRead(theButton) == LOW) {
      idleMiliseconds = 0;
      buttonHold++;

      //------------------------
      if (buttonHold < 11) { //10 = 2 seconds  2초 아레로 짧게 누를 때
        newDigitNum++;
        if (newDigitNum > 9) {
          newDigitNum = 0  ;//커서 숫자 올리는 거 까지
        }

        if (cursor == 1)
          number = (number - originDigitNum * 1000) + newDigitNum * 1000;
        else if (cursor == 2)
          number = (number - originDigitNum * 100) + newDigitNum * 100;
        else if (cursor == 3)
          number = (number - originDigitNum * 10) + newDigitNum * 10;
        else if (cursor == 4)
          number = (number - originDigitNum ) + newDigitNum;
      }//Advance the minutes

      else {

        cursor--;
        if (cursor <= 0) {
          cursor = 4;


        }//버튼을 길게 누를 시 자릿수 이동을 실행한다.
        //그 이상으로 2초 이상 누를 때 트리거
      }



      //------------------
    }
    else
      buttonHold = 0;

    //--------------------------


    idleMiliseconds += 200;
  }
  }
*/

void displayNumber_for_cursor(int toDisplay, int cursors, boolean power) {
#define DIGIT_ON  HIGH
#define DIGIT_OFF  LOW

  for (int digit = 4 ; digit > 0 ; digit--) {

    //Turn on a digit for a short amount of time
    switch (digit) {
      case 1:
        digitalWrite(digit1, DIGIT_ON);//천의자리
        if (cursors == 1)
          if (power == false) lightNumber(10); //When we update digit 2, let's turn on colons as well
        break;
      case 2:
        digitalWrite(digit2, DIGIT_ON);
        if (cursors == 2)
          if (power == false) lightNumber(10) ;//When we update digit 2, let's turn on colons as well
        break;
      case 3:
        digitalWrite(digit3, DIGIT_ON);
        if (cursors == 3)
          if (power == false) lightNumber(10); //When we update digit 2, let's turn on colons as well
        break;
      case 4:
        digitalWrite(digit4, DIGIT_ON);//일의자리
        if (cursors == 4)
          if (power == false) lightNumber(10); //When we update digit 2, let's turn on colons as well
        break;
    }

    //Now display this digit
    if (power == true)
      lightNumber(toDisplay % 10); //Turn on the right segments for this digit
    else if (power == false) { //Special case on first digit, don't display 02:11, display 2:11
      if ( digit != cursors) //If we are on the first digit, and it's not zero . 0과 같지 않을 때
        lightNumber(toDisplay % 10); //Turn on the right segments for this digit
    }//0일때는 조건문을 탈출하여 표시를 하지 않음.

    /*
       base=count-((t*1000)+(s*100)+(f*10));
    */

    toDisplay /= 10;  // a = a / 10 ; -> 숫자를 분리해서 한 자리 씩 출력하는 공식.;-> 1의 자리에서 부터 출력

    delayMicroseconds(2000); //Display this digit for a fraction of a second (between 1us and 5000us, 500-2000 is pretty good)
    //If you set this too long, the display will start to flicker. Set it to 25000 for some fun.

    //Turn off all segments
    lightNumber(10);

    //Turn off all digits
    digitalWrite(digit1, DIGIT_OFF);
    digitalWrite(digit2, DIGIT_OFF);
    digitalWrite(digit3, DIGIT_OFF);
    digitalWrite(digit4, DIGIT_OFF);
    digitalWrite(colons, DIGIT_OFF);
    // digitalWrite(ampm, DIGIT_OFF);
  }



}


//--------------------------



void setdate(void) {

  int idleMiliseconds = 0;
  //2초간 버튼을 제어하지 않으면 셋타임을 탈출함

  int buttonHold = 0;
  //This counts the number of times you are holding the button down consecutively
  //Once we notice you're really holding the button down a lot, we will speed up the minute counter

  while (idleMiliseconds < 2000) {//2초보다 작을 때만 실시
    //--------------------
    int date = (month * 100) + day; //Combine the hours and minutes

    for (int x = 0 ; x < 10 ; x++) {
      displayNumber(date, true); //Each call takes about 8ms, display the colon for about 100ms
      delayMicroseconds(display_brightness); //Wait before we paint the display again
    }
    for (int x = 0 ; x < 10 ; x++) {
      displayNumber(date, false); //Each call takes about 8ms, turn off the colon for about 100ms
      delayMicroseconds(display_brightness); //Wait before we paint the display again
    }

    //If you're still hitting the button, then increase the time and reset the idleMili timeout variable
    if (digitalRead(theButton) == LOW) {
      idleMiliseconds = 0;

      buttonHold++;
      //------------------------
      if (buttonHold < 10) { //10 = 2 seconds
        day++; //Advance the minutes
        if (day > MONTH_DAYS[month - 1]) {
          if (!(month == 2 && day == 29)) {
            month++;
            day = 1;
          }
        }
        if (month > 12) {//만약 달이 12월보다 크면 연을 높이고 달은 1로 초기화
          // year++;
          month = 1;
        }
      }
      else {
        //Advance the minutes faster because you're holding the button for 10 seconds
        //Start advancing on the tens digit. Floor the single minute digit.

        day /= 10; //minutes = 46 / 10 = 4
        day *= 10; //minutes = 4 * 10 = 40
        day += 10;  //minutes = 40 + 10 = 50
        if ( day > 30)  {// 만약 날짜가 해당 달의 맥스 날짜를 넘어가면 달 상승,날짜 초기화
          month++;
          day = 1;

          // bDone = false;//3월에서 bDone 초기화
        }
        if (month > 12) {//만약 달이 12월보다 크면 연을 높이고 달은 1로 초기화
          // year++;
          month = 1;

        }

      }
      //------------------

    }
    //--------------------
    else
      buttonHold = 0;

    idleMiliseconds += 200;
  }
}

//-----
void setday_name(void) {

  int idleMiliseconds = 0;
  //2초간 버튼을 제어하지 않으면 셋타임을 탈출함

  int buttonHold = 0;
  //This counts the number of times you are holding the button down consecutively
  //Once we notice you're really holding the button down a lot, we will speed up the minute counter

  while (idleMiliseconds < 2000) {//2초보다 작을 때만 실시


    //    int combinedTime = (hours * 100) + minutes; //Combine the hours and minutes
    for (int y = 0; y < 2; y++) {
      for (int x = 0 ; x < 5 ; x++) {
        displayLetters(name_day); //Each call takes about 8ms, display the colon for about 100ms
        delayMicroseconds(display_brightness); //Wait before we paint the display again
      }
      for (int x = 0 ; x < 5 ; x++) {
        displayLetters(name_day); //Each call takes about 8ms, turn off the colon for about 100ms
        delayMicroseconds(display_brightness); //Wait before we paint the display again
      }
    }

    //If you're still hitting the button, then increase the time and reset the idleMili timeout variable
    if (digitalRead(theButton) == LOW) {
      idleMiliseconds = 0;

      buttonHold++;
      //------------------------
      if (buttonHold >= 0) //10 = 2 seconds
        day_name++; //Advance the minutes
      if (day_name > 7) {
        day_name = 1;
      }
      change_number_to_day(day_name);
      //------------------
    }
    else
      buttonHold = 0;

    idleMiliseconds += 200;
  }
}




//test function
//비밀번호 만들기
//무브커서로 시계 설정
void change_number_to_day(int day_name) {
  switch (day_name) {
    case 1:
      name_day = " sun";
      break;
    case 2:
      name_day = " mon";
      break;
    case 3:
      name_day = " tue";
      break;
    case 4:
      name_day = " wed";
      break;
    case 5:
      name_day = " thu";
      break;
    case 6:
      name_day = " fri";
      break;
    case 7:
      name_day = " sat";
      break;
  }
}
/*
  void stopwatch() {

  int count = 0;
  bool start = true;
  bool ready = true;
  int countmin = 0;
  int countsec = 0;
  int mix = 0;
  while ( start == true) {//진입
    showColor("----");
    while (digitalRead(theButton) == LOW) {//대기 : 버튼 계속 누르고 있을 시
      ready = false;
    }
    while (digitalRead(theButton) == HIGH && ready == false)//시작
    {
      unsigned long previousMillis = 0;
      unsigned long currentMillis = millis();

      if (currentMillis - previousMillis >= 1000) {//1초면 진입
        previousMillis = currentMillis;
        count++;//카운트 증가
        if (count >= 100) {      //카운트가 만약 100이면
          countsec++;            //1초증가
          count = 0;             //카운트는 0
          if (countsec > 59) {   //초가 60이면
            countmin++;          //1분 증가
            countsec = 0;        //초는 0
            if (countmin > 59) { //60분이면
              countsec = 0;      //초,분 0으로 초기화
              countmin = 0;
            }
          }
        }
        mix = (countmin * 100) + countsec;
        displayNumber(mix, false);

        if (digitalRead(theButton) == LOW) {
          ready = true;
          start = false;
        }
      }
    }
  }
  shownum(mix);
  }
*/
/*
  void timer() {

  int count = 0;
  bool start = true;
  bool ready = true;
  int countmin = 0;
  int countsec = 0;
  int mix = 0;
  settimer();
  shownum(timeer_num);
  while ( timer_num!=cur_num) {//진입

      unsigned long previousMillis = 0;
      unsigned long currentMillis = millis();

      if (currentMillis - previousMillis >= 1000) {//1초면 진입
        previousMillis = currentMillis;
        count++;//카운트 증가
        if (count >= 100) {      //카운트가 만약 100이면
          countmin--;            //1초증가
          if (countsec == 0) {
            countsec = 59;
          }
          countsec--;
        }
        mix = (countmin * 100) + countsec;
        displayNumber(mix, false);

        if (digitalRead(theButton) == LOW) {
          ready = true;
          start = false;
        }
      }

  }
  shownum(mix);
  }
*/





/*
   랜덤 숫자 지정
   각 자리 중복 확인

   숫자 옮기고
   숫자 증감


*/

//랜덤 찐 중복확인


//-------------------------------------------------------------------------------------------------


int set_and_move_cursor_for_ballgame(  int user_answer, bool move_mode ) {

  int idleMiliseconds = 0;
  //2초간 버튼을 제어하지 않으면 셋타임을 탈출함

  int buttonHold = 0;
  //This counts the number of times you are holding the button down consecutively
  //Once we notice you're really holding the button down a lot, we will speed up the minute counter

  while (idleMiliseconds < 2000) {//2초보다 작을 때만 실시

    //여백

    for (int y = 0; y < 2; y++) {
      for (int x = 0; x < 5; x++) {
        displayNumber_for_cursor(user_answer, cursor, true); //Each call takes about 8ms, display the colon for about 100ms
        delayMicroseconds(display_brightness); //Wait before we paint the display again
      }
      for (int x = 0; x < 5; x++) {
        displayNumber_for_cursor(user_answer, cursor, false); //Each call takes about 8ms, turn off the colon for about 100ms
        delayMicroseconds(display_brightness); //Wait before we paint the display again
      }
    }
    //-------------------------------
    int originDigitNum = 0;
    if (cursor == 4)   ///일의 자리일때
      originDigitNum = user_answer % 10;  //완전체분해
    else if (cursor == 3)
      originDigitNum = user_answer / 10 % 10;
    else if (cursor == 2)
      originDigitNum = user_answer / 100 % 10;
    else if (cursor == 1)
      originDigitNum = user_answer / 1000;
    int newDigitNum = originDigitNum;
    //---------------------------------
    //If you're still hitting the button, then increase the time and reset the idleMili timeout variable

    if (digitalRead(theButton) == LOW)  {
      idleMiliseconds = 0;
      buttonHold++;
      //      cursormove++;
      //------------------------
      if (move_mode == false && buttonHold >= 1) { //10 = 2 seconds  2초 아레로 짧게 누를 때
        newDigitNum++;
        if (newDigitNum > 9) {
          newDigitNum = 0;
        }
        //-------------------------
        if (cursor == 1)
          user_answer = (user_answer - originDigitNum * 1000) + newDigitNum * 1000;
        else if (cursor == 2)
          user_answer = (user_answer - originDigitNum * 100) + newDigitNum * 100;
        else if (cursor == 3)
          user_answer = (user_answer - originDigitNum * 10) + newDigitNum * 10;
        else if (cursor == 4)
          user_answer = (user_answer - originDigitNum) + newDigitNum;
        //Advance the minutes
      }
      else if (move_mode == true && buttonHold >= 1 ) {
        cursor--;
        if (cursor < 1) {
          cursor = 4;
        }
      }
      //------------------
    }
    else
      buttonHold = 0;
    //--------------------------

    idleMiliseconds += 200;
  }
  return user_answer;
}

void displayNumber_strike_and_ball(int strike, int ball) {

#define DIGIT_ON  HIGH
#define DIGIT_OFF  LOW

  char* SB[2] = {'s', 'b'}; //display strike and ball

  for (int digit = 4 ; digit > 0 ; digit--) {//4가 일의 자리,나머지 이용으로 인해 4부터 반복문 시작

    //Turn on a digit for a short amount of time
    switch (digit) {
      case 1:
        digitalWrite(digit1, DIGIT_ON);
        break;
      case 2:
        digitalWrite(digit2, DIGIT_ON);
        //  if (displayColon == true) digitalWrite(colons, DIGIT_ON); //When we update digit 2, let's turn on colons as well
        break;
      case 3:
        digitalWrite(digit3, DIGIT_ON);
        break;
      case 4:
        digitalWrite(digit4, DIGIT_ON);
        break;
    }

    //"," is s and b


    switch (digit) {
      case 1:
        if (strike != 0)
          lightNumber(strike);
        break;
      case 2:
        if (strike != 0)
          lightNumber(SB[0]);
        break;
      case 3:
        if (ball != 0)lightNumber(ball);
        break;
      case 4:
        if (ball != 0)lightNumber(SB[1]);
        break;
    }

    delayMicroseconds(2000); //Display this digit for a fraction of a second (between 1us and 5000us, 500-2000 is pretty good)
    //If you set this too long, the display will start to flicker. Set it to 25000 for some fun.

    //Turn off all segments
    lightNumber(10);

    //Turn off all digits
    digitalWrite(digit1, DIGIT_OFF);
    digitalWrite(digit2, DIGIT_OFF);
    digitalWrite(digit3, DIGIT_OFF);
    digitalWrite(digit4, DIGIT_OFF);
    digitalWrite(colons, DIGIT_OFF);
    //digitalWrite(ampm, DIGIT_OFF);
  }

}

void ball_game() {//main ball
#ifdef DEBUG

#endif
  int answer_ball = 0; //user
  int user_ball[4] = {0};
  int user_answer = 0;
  int prev_one_time_num[4] = {0};
  int num[4] = {0};
  bool move_mode = false;
  bool pass = false;
  int save = 0;
  bool save_mode = false;
  //------------  this is random number  -----//

  for (int i = 0; i <= 3; i++) //0,1,2,3
  {

    //  srand(time(NULL));
    int one_time_num = random(1, 9) ; //난수 생성
    prev_one_time_num[i] = one_time_num;//검사용
    num[i] = one_time_num;//합계
    for (int j = 0; j < i; j++)
    { //데이터 중복 검사 - 일의 자리면 반복 1, 십의 자리면 반복 2..
      if (one_time_num == prev_one_time_num[j])//데이터 중복 검사 랜덤 값과 각 자리
      {
        i--;
      }
    }
  }

  //---------  welcome to main loop  ------//

  showmsg("----");//ready

  int random_ball = (num[0] * 1000) + (num[1] * 100) + (num[2] * 10) + (num[3]); //1,10,100,1000

  Serial.println(random_ball);

  while ( pass == false && save_mode == false) { //정답을 맞추었거나 저장모드일 때
    if ( move_mode == true ) showmsg("move");
    else showmsg("plus");

    if (digitalRead(theButton) == LOW)
    {
      if (move_mode == true )
      {
        move_mode = false;
      }
      else
      {
        move_mode = true;
      }
    }
    int before = user_answer;
    answer_ball = set_and_move_cursor_for_ballgame(user_answer, move_mode);
    user_answer = answer_ball;
    int after = answer_ball;

    Serial.print("이전 값 : "); Serial.println(before);
    Serial.print("현재 값 : "); Serial.println(after);

    if (before == after) {
      save++;
      Serial.print("save :");
      Serial.println(save);

      if (save == 3) {
        save_mode = true;

      }
    } else {
      save = 0;
    }
    //------------------ user num -> array user num[] --------------------//
    for (int i = 0; i < 4; i++) {
      user_ball[i] = answer_ball % 10;//일의 자리부터 분해
      answer_ball = answer_ball / 10;//몫을 저장
    }
    //------------  Give me " 1 strike & 1 ball "  --------------------//
    int b_num = 0;
    int s_num = 0;
    for (int i = 0; i < 4; i++) {
      for (int j = 0; j < 4; j++) {
        if ( user_ball[i] == num[j] )
        {
          if (i == j) {
            s_num++; //자리가 맞는 스트라이크 증가
            if (s_num == 4) {
              pass = true;
            }
          }
          else b_num++;//숫자가 맞는 볼 증가
        }
      }
    } if (s_num == 0 && b_num == 0)
    {
      showmsg(" out");
    }
    else {
      show_strike_and_ball(s_num, b_num);
    }
  }
  if (save_mode == true)showmsg("save"); //save
  else showmsg(" win");//P
}



void show_strike_and_ball(int s, int b) {//

  long prev = millis();
  while ((millis() - prev) <= 1000) {//1초간 숫자야구 결과를 디스플레이
    displayNumber_strike_and_ball(s, b);

  }
}
