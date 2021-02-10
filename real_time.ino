
void settime_minutes(void) {

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
    // hours += minutes / 60; //12 + (96 / 60) = 13
    minutes %= 60; //minutes = 36

    //Do we display 12 hour or 24 hour time?
    /* if (TwelveHourMode == true) {
       //In 12 hour mode, hours go from 12 to 1 to 12.
       while (hours > 12) hours -= 12;
      }
      else {
       //In 24 hour mode, hours go from 0 to 23 to 0.
       while (hours > 23) hours -= 24;


      }
    */
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

      if (buttonHold < 10 ) { //10 = 2 seconds

        minutes++;
      }
      else {
        //Advance the minutes faster because you're holding the button for 10 seconds
        //Start advancing on the tens digit. Floor the single minute digit.
        minutes /= 10; //minutes = 46 / 10 = 4
        minutes *= 10; //minutes = 4 * 10 = 40
        minutes += 10;  //minutes = 40 + 10 = 50

      }
      //------------------
    }
    else
      buttonHold = 0;

    idleMiliseconds += 200;
  }
}




void settime_hours(void) {

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

      if (buttonHold >= 0 ) { //10 = 2 seconds

        hours++; //Advance the minutes
        if (hours > 23) {
          hours = 0;
        }
      } /*
      else {
        //Advance the minutes faster because you're holding the button for 10 seconds
        //Start advancing on the tens digit. Floor the single minute digit.
        minutes /= 10; //minutes = 46 / 10 = 4
        minutes *= 10; //minutes = 4 * 10 = 40
        minutes += 10;  //minutes = 40 + 10 = 50
       if (minutes < 10) {
          minutes = 10;
        }
        else if (minutes < 30) {
          minutes = 30;
        } else if (minutes <= 59) {
          minutes = 50;
        }
     }*/
      //------------------
    }
    else
      buttonHold = 0;

    idleMiliseconds += 200;
  }
}




void settime_alarm(void) {//ALARM

  int idleMiliseconds = 0;
  //2초간 버튼을 제어하지 않으면 셋타임을 탈출함

  int buttonHold = 0;
  //This counts the number of times you are holding the button down consecutively
  //Once we notice you're really holding the button down a lot, we will speed up the minute counter

  while (idleMiliseconds < 2000) {//2초보다 작을 때만 실시


    //for code


    int combinedTime = (A_hours * 100) + A_minutes; //Combine the hours and minutes

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

      if (buttonHold <= 6 ) { //10 = 2 seconds

        A_minutes--; //Advance the minutes
        if (A_minutes < 0) {
          A_hours--;
          if (A_hours < 0) {
            A_hours = 23;
          }
          A_minutes = 59;
        }
      }
      else {
        //Advance the minutes faster because you're holding the button for 10 seconds
        //Start advancing on the tens digit. Floor the single minute digit.
        A_minutes++;
        A_minutes /= 10; //minutes = 46 / 10 = 4
        A_minutes *= 10; //minutes = 4 * 10 = 40
        A_minutes += 10;  //minutes = 40 + 10 = 50
        if (A_minutes > 59) {
          A_hours++;
          if (A_hours > 23) {
            A_hours = 0;
          }
          A_minutes = 0;
        }
      }
      //------------------
    }
    else
      buttonHold = 0;

    idleMiliseconds += 200;
  }
}
