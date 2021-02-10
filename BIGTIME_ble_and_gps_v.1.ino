//#include <core_build_options.h>
//#include <swRTC.h>
//숫자야구
/*알람 완료
*/
//풀업 구조 조정 , na eu 싸이클

//카톡이나 문자나, 전화가 오면 손목시계로 알림,gps 데이터를 받아 휴대폰에서 위치 추적
//#define PROTO_TYPE

#include <avr/sleep.h> //Needed for sleep_mode
#include <avr/power.h> //Needed for powering down perihperals such as the ADC/TWI and Timers
#ifdef PROTO_TYPE
  #include <TinyGPS++.h>
  #include <SoftwareSerial.h>
#endif
//#define DEBUG //가끔 우리는 디버그를 필히 할 필요가 있다.
//#define RELEASE//가끔 우리는 디버그를 필히 할 필요가 있다.
//#define ampm_mode//오전오후 기능
//Declaring this will enable IR broadcast when you hit the time button twice
//By default, we don't enable this

//#define ENABLE_TVBGONE
#define ALARM

#ifdef ENABLE_TVBGONE
#include "WORLDcodes.h"
#define putstring_nl(s) Serial.println(s)
#define putstring(s) Serial.print(s)
#define putnum_ud(n) Serial.print(n, DEC)
#define putnum_uh(n) Serial.print(n, HEX)
#endif


#ifdef PROTO_TYPE
//--------------------------------------------
//            G       P        S (conf.)
//--------------------------------------------
TinyGPSPlus gps;
SoftwareSerial ss(3, 100);//rx,tx from board. we need only rx
SoftwareSerial BluetoothWatch(A5, A4); // RX, TX . ble module
#define GPSBAUD           9600 //default
#define BLUETOOTHBAUD     9600 //default
//--------------------------------------------
#endif
//Set the 12hourMode to false for military/world time. Set it to true for American 12 hour time.
int TwelveHourMode = false;//시계내에서 설정 가능하도록 구현

bool lazer_state = false;
//we make debug mode

//Set this variable to change how long the time is shown on the watch face. In milliseconds so 1677 = 1.677 seconds
int show_time_length = 2000;
int show_the_time = false;

//You can set always_on to true and the display will stay on all the time
//This will drain the battery in about 15 hours
int always_on = false;  //시계내에서 설정 가능하도록 구현
int A_hours = 2;
int A_minutes = 0;
//----------
//this is hardware line
int led = A2;
int laser = A4;
int vibe = A3;//not yet


//dual time parameter
int setting_minutes = 0;
int setting_hours = 0;
bool dual_time_active = false;


//----------
//this is time and date line.main
//int newh = 0;

//bool ampm_want = false;

long seconds = 0;
int minutes = 57;
int hours = 1;

int year = 2020;
int month = 9;
int day = 14;


int cursor = 4;
//-----------------------------
//                 0123456
int day_name = 2;//월화수목굼토일
char *name_day ;//default
/*
  day name=요일에 숫자
  name day= 요일 이름
  일=1
  월=2
  화=3
  수=4
  목=5
  금=6
  토=7
*/


const int MONTH_DAYS[12] = { 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 };
bool bLeapYear = (year % 4 == 0 && year % 100 != 0 || year % 400 == 0); //윤년 계산
//                      true          fasle              true
//----------------
//for cal date
int number = 0000;
int quiz = 0;
bool ver_easy = false;
bool once_AMPM_check = false;
bool only_cal_date = false;
bool save_and_continue = false;
bool set_on_time = true;//정각 알림 기능 켰다 끄기
bool once_alm = true;
bool bDone = false;

int button_delay = 3000;//-------------
//for updown
bool set_trigger_time = false;
bool bdone_light_time = false;

bool set_alarm = true;//for snooze
bool alarm_start = false;//for alarm
//-------------
//int cursor = 4;
//int newDigitNum = 0;

//for cursor
//int cursor = 4;//일의 자리일때

bool beforeled = LOW;
bool btn = LOW;
bool ledstate = false;
bool hard = false;
bool cur = false;
int counttime = 500;
bool we_want_cds = true;
int pw = 0;
//Careful messing with the system color, you can damage the display if
//you assign the wrong color. If you're in doubt, set it to red and load the code,
//then see what the color is.
#define RED  1
#define GREEN 2
#define BLUE  3
#define YELLOW  4
int systemColor = BLUE;
int display_brightness = 1; //A larger number makes the display more dim. This is set correctly below.




//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//pin map

//        board          seg
int digit1 = 9; //Display pin 1
int digit2 = 10; //Display pin 2
int digit3 = A0; //Display pin 6
int digit4 = A1; //Display pin 8

int segA = 6; //Display pin 14
int segB = 8; //Display pin 16
int segC = 5; //Display pin 13
int segD = 11; //Display pin 3
int segE = 13; //Display pin 5
int segF = 4; //Display pin 11
int segG = 7; //Display pin 15
//int IRLED = 3;
int colons = 12; //Display pin 4 with lazer
//int ampm = 3; //Display pin 10
//int dp=0;
bool alarm_off = false;
int theButton = 2;
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
#ifdef ENABLE_TVBGONE
/*
  void xmitCodeElement(uint16_t ontime, uint16_t offtime, uint8_t PWM_code );
  void quickflashLEDx(uint8_t x);
  void delay_ten_us(uint16_t us);
  void quickflashLED(void);
  uint8_t read_bits(uint8_t count);
*/


extern const IrCode* const NApowerCodes[] PROGMEM;
extern const IrCode* const EUpowerCodes[] PROGMEM;
extern uint8_t num_NAcodes, num_EUcodes;


/* This is kind of a strange but very useful helper function
  Because we are using compression, we index to the timer table
  not with a full 8-bit byte (which is wasteful) but 2 or 3 bits.
  Once code_ptr is set up to point to the right part of memory,
  this function will let us read 'count' bits at a time which
  it does by reading a byte into 'bits_r' and then buffering it. */

uint8_t bitsleft_r = 0;
uint8_t bits_r = 0;
PGM_P code_ptr;



uint16_t ontime, offtime;
uint8_t i, num_codes;
uint8_t region;
uint8_t startOver;

#endif


//what is that isr,signal
//The very important 32.686kHz interrupt handler
SIGNAL(TIMER2_OVF_vect) {
  seconds += 8; //We sleep for 8 seconds instead of 1 to save more power
  //seconds++; //Use this if we are waking up every second  //a++, a+=, a= a+1

  //Update the minutes and hours variables
  /* min=min+sec/60 */        minutes += seconds / 60; //Example: seconds = 2317, minutes = 58 + 38 = 96
  /* sec = sec % 60 */        seconds %= 60; //seconds = 37
  /* hour= hour + min / 60 */ hours += minutes / 60; //12 + (96 / 60) = 13
  /* min = min % 60 */        minutes %= 60; //minutes = 36

  //Do we display 12 hour or 24 hour time?


  //    while (hours > 23) hours -= 24;  //24 hour mode.
  if (TwelveHourMode == true) {
    //In 12 hour mode, hours go from 12 to 1 to 12.
    while (hours > 12) hours -= 12;
  }
  else {
    //In 24 hour mode, hours go from 0 to 23 to 0.

    while (hours > 23) hours -= 24;

  }
}

//---------------------------------------------------



//The interrupt occurs when you push the button
SIGNAL(INT0_vect) {
  //When you hit the button, we will need to display the time
  //if(show_the_time == false)
  show_the_time = true;

  alarm_start = false;

}

void setup() {
  //To reduce power, setup all pins as inputs with no pullups
  for (int x = 1 ; x < 18 ; x++) {
    //  if (x != 3) {
    pinMode(x, INPUT);
    digitalWrite(x, LOW);
    //}
  }
  pinMode(theButton, INPUT); //This is the main button, tied to INT0
  digitalWrite(theButton, HIGH); //Enable internal pull up on button

  //These pins are used to control the display
  pinMode(segA, OUTPUT);
  pinMode(segB, OUTPUT);
  pinMode(segC, OUTPUT);
  pinMode(segD, OUTPUT);
  pinMode(segE, OUTPUT);
  pinMode(segF, OUTPUT);
  pinMode(segG, OUTPUT);

  pinMode(digit1, OUTPUT);
  pinMode(digit2, OUTPUT);
  pinMode(digit3, OUTPUT);
  pinMode(digit4, OUTPUT);
  pinMode(colons, OUTPUT);
  //  pinMode(ampm, OUTPUT);

  pinMode(led, OUTPUT);
  pinMode(laser, OUTPUT);
  pinMode(vibe, OUTPUT);

  //pinMode(IRLED, OUTPUT);
  //digitalWrite(IRLED, LOW);
  // pinMode(REGIONSWITCH, INPUT_PULLUP);

  // pinMode(cds, INPUT);

  //Power down various bits of hardware to lower power usage
  set_sleep_mode(SLEEP_MODE_PWR_SAVE);
  sleep_enable();

  //Shut off ADC, TWI, SPI, Timer0, Timer1

  ADCSRA &= ~(1 << ADEN); //Disable ADC   //v  .this check is for analogRead
  ACSR = (1 << ACD); //Disable the analog comparator  //v
  DIDR0 = 0x3F; //Disable digital input buffers on all ADC0-ADC5 pins  //v
  DIDR1 = (1 << AIN1D) | (1 << AIN0D); //Disable digital input buffer on AIN1/0

  power_twi_disable();  //v
  power_spi_disable();
  //  power_usart0_disable(); //Needed for serial.print
  //power_timer0_disable(); //Needed for delay and millis()
  power_timer1_disable();
  //power_timer2_disable(); //Needed for asynchronous 32kHz operation

  //Setup TIMER2
  TCCR2A = 0x00;
  //TCCR2B = (1<<CS22)|(1<<CS20); //Set CLK/128 or overflow interrupt every 1s
  TCCR2B = (1 << CS22) | (1 << CS21) | (1 << CS20); //Set CLK/1024 or overflow interrupt every 8s
  ASSR = (1 << AS2); //Enable asynchronous operation
  TIMSK2 = (1 << TOIE2); //Enable the timer 2 interrupt

  //Setup external INT0 interrupt
  EICRA = (1 << ISC01); //Interrupt on falling edge
  EIMSK = (1 << INT0); //Enable INT0 interrupt
  //System clock futzing
  //CLKPR = (1<<CLKPCE); //Enable clock writing
  //CLKPR = (1<<CLKPS3); //Divid the system clock by 256
  Serial.begin(115200);
  Serial.println("BigTime Testing:");
#ifdef PROTO_TYPE
  BluetoothWatch.begin(BLUETOOTHBAUD);
  ss.begin(GPSBAUD);
  ss.end();
#endif

  /*
    //---------------------related tv b gone
    // determine region
    if (digitalRead(REGIONSWITCH)) {
      region = NA;
      DEBUGP(putstring_nl("NA"));
    }
    else {
      region = EU;
      DEBUGP(putstring_nl("EU"));
    }
  */
  // Debug output: indicate how big our database is
#ifdef ENABLE_TVBGONE
  DEBUGP(putstring("\n\rNA Codesize: ");
         putnum_ud(num_NAcodes);
        );
  DEBUGP(putstring("\n\rEU Codesize: ");
         putnum_ud(num_EUcodes);
        );
#endif
  //----------------------------------------

  display_brightness = 1; //Works well for blue displays, uses about 5.5mA
  showColor("cLck");

  sei(); //Enable global interrupts
}
void calendar() {
  if (bLeapYear && bDone == false && month == 2 && day == 28) //만약 윤년에 달은 2월 그리고 날짜는 28 이상을 달리면 29로 환승
  {
    day++;
    bDone = true;
  }
  else
  {
    day++;  //++day
    if (day > MONTH_DAYS[month - 1]) {// 만약 날짜가 해당 달의 맥스 날짜를 넘어가면 달 상승,날짜 초기화
      month++;
      day = 1;
      bDone = false;//3월에서 bDone 초기화
    }
    if (month > 12) {//만약 달이 12월보다 크면 연을 높이고 달은 1로 초기화
      year++;
      month = 1;
    }
  }
  day_name++;
  if (day_name > 7) day_name = 1;
  change_number_to_day(day_name);
}
void loop() {

  //Serial.print(hours); Serial.print("//"); Serial.print(minutes); Serial.print("//"); Serial.println(seconds);
#ifdef ALARM
  //(time && start) || snz
  if (set_alarm == true) {
    if ((A_minutes - 1 == minutes  )  && A_hours == hours) alarm_start = true; //0 min - 1min
  }//정각 알림은 버그 발견
  //we need fix bug
  //on time and alarm
  if (alarm_start == true)
  {
    if (A_minutes == minutes && A_hours == hours ) digitalWrite(vibe, HIGH);
  }
  else digitalWrite(vibe, LOW);
#endif
  //정각 알림 구현 - 1
  if (set_on_time == true) {
    if (minutes == 58) once_alm = true;
  }

  //정각 알림 기능 설정(예외 조건 1)
  //정각 알림기능 설정시 한번만 울리게 하는 설정 , 정각 알림기능 설정 해제시 날짜 계산을 위한 자정 시간 출력
  if (once_alm == true) {//매시간 정각 알림(한번만 알리게 해주기 위한 방법)
    if (minutes == 0 ) {
      if (A_minutes == 0)alarm_start = true;
      once_alm = false;
      on_time_showTime();  //정각 알림
    }

    //----------------------
    if ( hours == 0 && minutes == 0) { //정각 알림 때 12시면 날짜 바꾸는 공식 진입
      calendar();
    }
  }

  if (always_on == false) {  //올웨이즈 변수가 펄스일 경우 꺼져있음(보통의 모드 / 예외 조건을 제외하고선 이 모드의 진입)
    //#ifdef BUG_CATCH
    // Serial.print(A_hours); Serial.print("//"); Serial.print(A_minutes); Serial.print("  ||  ");
    //#else
    //    sleep_mode(); //Stop everything and go to sleep. Wake up if the Timer2 buffer overflows or if you hit the button
    //#endif
  }

  if (show_the_time == true || always_on == true) { //예외 조건 만약 버튼의 트리거가 발생할 시 켜짐

    unsigned long nowtime = millis();
    unsigned long prevTime = nowtime;
    int countSecond = 0;//누른 시간을 보여주는 변수.
    //Debounce
    while (digitalRead(theButton) == LOW) {
      unsigned long curTime = millis();
      if (curTime - prevTime >= counttime)
      {
        countSecond++;
        prevTime = curTime;
      }
      if (countSecond > 0) displayNumber(countSecond, false);//구버전
      //선택 기능이 0 이상일 경우 장시간 디스플레이 해줍니다. 우리는 디스플레이를 기능이름으로 하였으나 그것은 매우 지저분했다. 구버전을 활용하길 바란다.

    } //Wait for you to remove your finger

    delay(100);
    while (digitalRead(theButton) == LOW && countSecond == 0) { //If you want to use this function, press the button quickly.
      // cur = true;
      countSecond = 0;
      digitalWrite(laser, HIGH);
      //Wait for you to remove your finger
      lazer_state = true;
    }
    digitalWrite(laser, LOW);
    //Depending on the length of time that the button is pressed, different functions are entered.
    //The clock, not the user measuring the time, automatically measures and shows the seconds.


    /**
       Depending on the length of time that the button is pressed, different functions are entered.
       The clock, not the user measuring the time, automatically measures and shows the seconds.
    */
    //while (digitalRead(theButton) == LOW)
    //function solution 1


    //We have added a date calculation and modified the perceptual notification function.
    //In case of conditions in seconds, it was not clear, so it was required to identify true lies and implement regular notification.
    //It also requires the date to be calculated by displaying the date at 0 o'clock using the time when the notification is received.


    switch (countSecond)
    {
      case 0:
        if (lazer_state == false) {
          showTime(); //Show the current time for a few seconds
          if (digitalRead(theButton) == LOW)settime_minutes();  //If you are STILL holding the button, then you must want to adjust the time
        }
        lazer_state = false;
        break;
      case 1:
        /* btn = digitalRead(theButton);
          if (btn == true) {
           ledpwr = !ledpwr;  //led 반전 시키는 기능. 상태에 따라 값을 주는 것이 아니라 무조건적으로 반전 시키는 공식.
          }*/
        ledstate = !ledstate;//LOW->HIGH
        digitalWrite(led, ledstate);

        break;
      case 2:
        showyear(); //we show this year.
        if (digitalRead(theButton) == LOW) setyear();
        showdate();//we show today date.
        if (digitalRead(theButton) == LOW) setdate();
        showday_name();//show now day(week)
        if (digitalRead(theButton) == LOW) setday_name();
        break;
      case 3:
        settime_hours();//setting just minutes.
        break;
      /* case 4:
         //updown_game();//up and down game.
         ball_game();
         //   Serial.println(9999);//debug
         break;
      */
      case 5:
        settime_alarm();
        break;
      case 7:
#ifdef PROTO_TYPE
        gps_func();
#endif
        //show lan lon
        //gps is small size and need antenna
        break;

#ifdef ENABLE_TVBGONE
      case 6:
        tv_b_gone();
        break;
#endif
      default:
        display_brightness = 1;
        showColor("----"); //문자열 출력
        break;
    }
    show_the_time = false; //Reset the button variable


    /*
        Serial.print(hours, DEC);
          Serial.print(":");
          Serial.print(minutes, DEC);
          Serial.print(":");
          Serial.println(seconds, DEC);


        //function solution 2
        //This version is an older version of the code that detects the time a button is pressed and selects a function.
        //
       (19.08.11) We developed a function to screen functions by counting machines. Use the new above code.
        if ((millis() - nowtime) < button_delay) {
          showTime(); //Show the current time for a few seconds

          //If you are STILL holding the button, then you must want to adjust the time
          if (digitalRead(theButton) == LOW) setTime();


        }

        else {
          curbtn = digitalRead(btn);
          if (btn == HIGH && LEDstate == false) {
            ledpwr = !ledpwr;
          }
          beforebtn = nowbtn;
          digitalWrite(led, ledpwr);

          //Reset clock button status.

        }

        show_the_time = false; //Reset the button variable
    */

  }

}

#ifdef ENABLE_TVBGONE
//this is for define ENABLE_TVBGONE
// This function delays the specified number of 10 microseconds
// it is 'hardcoded' and is calibrated by adjusting DELAY_CNT
// in main.h Unless you are changing the crystal from 8mhz, dont
// mess with this.
void delay_ten_us(uint16_t us) {
  uint8_t timer;
  while (us != 0) {
    // for 8MHz we want to delay 80 cycles per 10 microseconds
    // this code is tweaked to give about that amount.
    for (timer = 0; timer <= DELAY_CNT; timer++) {
      NOP;
      NOP;
    }
    NOP;
    us--;
  }
}


// This function quickly pulses the visible LED (connected to PB0, pin 5)
// This will indicate to the user that a code is being transmitted
void quickflashLED(void) {
  digitalWrite(colons, HIGH);
  //  delay_ten_us(3000);   // 30 millisec delay
  delay_ten_us(300);   // 3 millisec delay
  digitalWrite(colons, LOW);
}

// This function just flashes the visible LED a couple times, used to
// tell the user what region is selected
void quickflashLEDx(uint8_t x) {
  quickflashLED();
  while (--x) {
    delay_ten_us(15000);     // 150 millisec delay between flahes
    quickflashLED();
  }
}

// we cant read more than 8 bits at a time so dont try!
uint8_t read_bits(uint8_t count)
{
  uint8_t i;
  uint8_t tmp = 0;

  // we need to read back count bytes
  for (i = 0; i < count; i++) {
    // check if the 8-bit buffer we have has run out
    if (bitsleft_r == 0) {
      // in which case we read a new byte in
      bits_r = pgm_read_byte(code_ptr++);
      // and reset the buffer size (8 bites in a byte)
      bitsleft_r = 8;
    }
    // remove one bit
    bitsleft_r--;
    // and shift it off of the end of 'bits_r'
    tmp |= (((bits_r >> (bitsleft_r)) & 1) << (count - 1 - i));
  }
  // return the selected bits in the LSB part of tmp
  return tmp;
}

/* This function is the 'workhorse' of transmitting IR codes.
  Given the on and off times, it turns on the PWM output on and off
  to generate one 'pair' from a long code. Each code has ~50 pairs! */
void xmitCodeElement(uint16_t ontime, uint16_t offtime, uint8_t PWM_code )
{
  TCNT2 = 0;
  if (PWM_code) {
    pinMode(IRLED, OUTPUT);
    // Fast PWM, setting top limit, divide by 8
    // Output to pin 3
    TCCR2A = _BV(COM2A0) | _BV(COM2B1) | _BV(WGM21) | _BV(WGM20);
    TCCR2B = _BV(WGM22) | _BV(CS21);
  }
  else {
    // However some codes dont use PWM in which case we just turn the IR
    // LED on for the period of time.
    digitalWrite(IRLED, HIGH);
  }

  // Now we wait, allowing the PWM hardware to pulse out the carrier
  // frequency for the specified 'on' time
  delay_ten_us(ontime);

  // Now we have to turn it off so disable the PWM output
  TCCR2A = 0;
  TCCR2B = 0;
  // And make sure that the IR LED is off too (since the PWM may have
  // been stopped while the LED is on!)
  digitalWrite(IRLED, LOW);

  // Now we wait for the specified 'off' time
  delay_ten_us(offtime);
}


/****************************** LED AND DELAY FUNCTIONS ********/

void sendAllCodes() {

  //Start_transmission:
  // startOver will become TRUE if the user pushes the Trigger button while transmitting the sequence of all codes
  //startOver = false;

  //-------------------------------------------------------------
  // determine region from REGIONSWITCH: 1 = NA, 0 = EU
  //첫 시작은 TRUE(1)
  int area_switch = 1;
  while (area_switch < 3) {
    switch (area_switch)  {
      case 1:
        region = NA;
        num_codes = num_NAcodes;
        break;
      case 2:
        region = EU;
        num_codes = num_EUcodes;
        break;
    }
    Serial.print("area_switch : ");
    Serial.println(area_switch);
    // for every POWER code in our collection
    for (i = 0 ; i < num_codes; i++) {
      //  for (i=0 ; i < 1 ; i++) {
      PGM_P data_ptr;

      // print out the code # we are about to transmit
      DEBUGP(putstring("\n\r\n\rCode #: ");
             putnum_ud(i));

      // point to next POWER code, from the right database
      if (region == NA) {
        data_ptr = (PGM_P)pgm_read_word(NApowerCodes + i);
      }
      else {
        data_ptr = (PGM_P)pgm_read_word(EUpowerCodes + i);
      }

      // print out the address in ROM memory we're reading
      DEBUGP(putstring("\n\rAddr: ");
             putnum_uh((uint16_t)data_ptr));

      // Read the carrier frequency from the first byte of code structure
      const uint8_t freq = pgm_read_byte(data_ptr++);
      // set OCR for Timer1 to output this POWER code's carrier frequency
      OCR2A = freq;
      OCR2B = freq / 3; // 33% duty cycle

      // Print out the frequency of the carrier and the PWM settings
      DEBUGP(putstring("\n\rOCR1: ");
             putnum_ud(freq);
            );
      DEBUGP(uint16_t x = (freq + 1) * 2;
             putstring("\n\rFreq: ");
             putnum_ud(F_CPU / x);
            );

      // Get the number of pairs, the second byte from the code struct
      const uint8_t numpairs = pgm_read_byte(data_ptr++);
      DEBUGP(putstring("\n\rOn/off pairs: ");
             putnum_ud(numpairs));

      // Get the number of bits we use to index into the timer table
      // This is the third byte of the structure
      const uint8_t bitcompression = pgm_read_byte(data_ptr++);
      DEBUGP(putstring("\n\rCompression: ");
             putnum_ud(bitcompression);
             putstring("\n\r"));

      // Get pointer (address in memory) to pulse-times table
      // The address is 16-bits (2 byte, 1 word)
      PGM_P time_ptr = (PGM_P)pgm_read_word(data_ptr);
      data_ptr += 2;
      code_ptr = (PGM_P)pgm_read_word(data_ptr);

      // Transmit all codeElements for this POWER code
      // (a codeElement is an onTime and an offTime)
      // transmitting onTime means pulsing the IR emitters at the carrier
      // frequency for the length of time specified in onTime
      // transmitting offTime means no output from the IR emitters for the
      // length of time specified in offTime

      /*
        #if 0

        // print out all of the pulse pairs
        for (uint8_t k=0; k<numpairs; k++) {
        uint8_t ti;
        ti = (read_bits(bitcompression)) * 4;
        // read the onTime and offTime from the program memory
        ontime = pgm_read_word(time_ptr+ti);
        offtime = pgm_read_word(time_ptr+ti+2);
        DEBUGP(putstring("\n\rti = ");
        putnum_ud(ti>>2);
        putstring("\tPair = ");
        putnum_ud(ontime));
        DEBUGP(putstring("\t");
        putnum_ud(offtime));
        }
        continue;
        #endif
      */

      // For EACH pair in this code....
      cli();
      for (uint8_t k = 0 ; k < numpairs ; k++) {
        uint16_t ti;

        // Read the next 'n' bits as indicated by the compression variable
        // The multiply by 4 because there are 2 timing numbers per pair
        // and each timing number is one word long, so 4 bytes total!
        ti = (read_bits(bitcompression)) * 4;

        // read the onTime and offTime from the program memory
        ontime = pgm_read_word(time_ptr + ti); // read word 1 - ontime
        offtime = pgm_read_word(time_ptr + ti + 2); // read word 2 - offtime

        // transmit this codeElement (ontime and offtime)
        xmitCodeElement(ontime, offtime, (freq != 0));
      }
      sei();

      //Flush remaining bits, so that next code starts
      //with a fresh set of 8 bits.
      bitsleft_r = 0;

      // if user is pushing Trigger button, stop transmission
      if (digitalRead(theButton) == LOW) {
        while (digitalRead(theButton) == LOW) ; //20ms debounce
        //startOver = TRUE;
        break;
      }

      // delay 205 milliseconds before transmitting next POWER code
      //delay_ten_us(20500);
      delay_ten_us(2000); //20ms

      // visible indication that a code has been output.
      quickflashLED();
    }
    area_switch++;

  } //  if (startOver) goto Start_transmission;
  //while (Loop == 1); //Why is this here?

  // flash the visible LED on PB0 8 times to indicate that we're done
  //  delay_ten_us(65500); // wait maxtime
  //  delay_ten_us(65500); // wait maxtime
  //  quickflashLEDx(8);

}

#endif
