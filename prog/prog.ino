//#include <avr/power.h>
//#include <avr/sleep.h>
//#include <avr/interrupt.h>
#include "RTClib.h"

RTC_DS1307 rtc;


#define X0 1
#define X1 6
#define X2 7
#define X3 10

#define ConvVCC 5
#define PntL 17
#define PntR 0
#define NeonL 8

#define Frst /*15*/ 16
#define Scnd /*14*/ 2
#define Thrd /*2*/  14
#define Frth /*16*/ 15

#define FBtn 3
#define SBtn 4

#define PWM_pin 9

int ShwnTm = 2;
int Shw = 0;                     
int btn = 0;
bool slp = false;
uint8_t HRS = 0;
uint8_t MNTS = 0;
uint8_t Sec = 0;
uint8_t SecPrv = 0;
uint8_t DateShow = 0;

uint8_t Month = 0;
uint8_t Day = 0;
bool ChkSec = false;

int FrstL = 0, ScndL = 0, ThrdL = 0, FrthL = 0;

uint8_t Settings = 0;
bool chk = false;

uint8_t DateShowPRV = 0;

uint8_t BlinkSettings = 0;
bool ChkAntiPois = false;



void setup() {
 /* ADCSRA &= ~(1 << ADEN); 
  power_usart0_disable();
  power_twi_disable();  */
 // attachInterrupt(Btn, ButInter, RISING);   
  rtc.begin();  
  
  TCCR1A = 0b00000001;  // 8bit
  TCCR1B = 0b00000001;

  pinMode(FBtn, INPUT);    
  pinMode(SBtn, INPUT);  
  
  pinMode(Frst, OUTPUT);  
  pinMode(Scnd, OUTPUT);
  pinMode(Thrd, OUTPUT);
  pinMode(Frth, OUTPUT);

  pinMode(PntL, OUTPUT);
  pinMode(PntR, OUTPUT);
  pinMode(NeonL, OUTPUT);

  pinMode(X0, OUTPUT);
  pinMode(X1, OUTPUT); 
  pinMode(X2, OUTPUT); 
  pinMode(X3, OUTPUT); 
  pinMode(ConvVCC, OUTPUT);  
  pinMode(PWM_pin, OUTPUT);  

  analogWrite(PWM_pin, 175);
  digitalWrite(ConvVCC, HIGH);
  digitalWrite(Frst, HIGH); 
  digitalWrite(Scnd, LOW); 
  digitalWrite(Thrd, LOW);  
  digitalWrite(Frth, LOW);  

  digitalWrite(NeonL, HIGH);  
 
  
  //set_sleep_mode(SLEEP_MODE_PWR_SAVE);
  //sleep_enable();
  //sleep_mode(); 
}

void tabl(int numr = 0){ //ДАННАЯ ТАБЛИЦА ИСТИННОСТИ ВЕРНА ТОЛЬКО ДЛЯ ДАННОЙ ПЛАТЫ ЧАСОВ
  switch(numr){
    case 0: 
      digitalWrite(X0, LOW); 
      digitalWrite(X1, LOW); 
      digitalWrite(X2, LOW); 
      digitalWrite(X3, LOW);      
    break;
    case 9:
      digitalWrite(X0, HIGH); 
      digitalWrite(X1, LOW); 
      digitalWrite(X2, LOW); 
      digitalWrite(X3, LOW);   
    break;
    case 1:
      digitalWrite(X0, LOW); 
      digitalWrite(X1, HIGH); 
      digitalWrite(X2, LOW); 
      digitalWrite(X3, LOW); 
    break;
    case 4:
      digitalWrite(X0, HIGH); 
      digitalWrite(X1, HIGH); 
      digitalWrite(X2, LOW); 
      digitalWrite(X3, LOW); 
    break;
    case 7:
      digitalWrite(X0, LOW); 
      digitalWrite(X1, LOW); 
      digitalWrite(X2, HIGH); 
      digitalWrite(X3, LOW); 
    break;
    case 8:
      digitalWrite(X0, HIGH); 
      digitalWrite(X1, LOW); 
      digitalWrite(X2, HIGH); 
      digitalWrite(X3, LOW); 
    break;
    case 6:
      digitalWrite(X0, LOW); 
      digitalWrite(X1, HIGH); 
      digitalWrite(X2, HIGH); 
      digitalWrite(X3, LOW); 
    break;
    case 5:
      digitalWrite(X0, HIGH); 
      digitalWrite(X1, HIGH); 
      digitalWrite(X2, HIGH); 
      digitalWrite(X3, LOW); 
    break;
    case 2:
      digitalWrite(X0, LOW); 
      digitalWrite(X1, LOW); 
      digitalWrite(X2, LOW); 
      digitalWrite(X3, HIGH); 
    break;
    case 3:
      digitalWrite(X0, HIGH); 
      digitalWrite(X1, LOW); 
      digitalWrite(X2, LOW); 
      digitalWrite(X3, HIGH); 
    break;   
  }  
}

void TIME_SHOW(int a = 0){
  for(int i = 0; i < a; i++)
    {
      digitalWrite(Frst, HIGH);
      tabl(FrstL);
      delay(1); 
      digitalWrite(Frst, LOW);
      delay(4);  

      digitalWrite(Scnd, HIGH);
      tabl(ScndL);
      delay(2); 
      digitalWrite(Scnd, LOW);
      delay(1);  

      digitalWrite(Thrd, HIGH);
      tabl(ThrdL);
      delay(1); 
      digitalWrite(Thrd, LOW);
      delay(1);  

      digitalWrite(Frth, HIGH);
      tabl(FrthL);
      delay(1); 
      digitalWrite(Frth, LOW);
      delay(1);  

    } 
}

void Blink(){
  if(Sec != SecPrv){
        SecPrv = Sec;
        if(ChkSec == false){
          ChkSec = true;
          digitalWrite(NeonL, HIGH); 
        }
        else{
          ChkSec = false;
          digitalWrite(NeonL, LOW); 
        }
      }
}

void BlnkSttngs(uint8_t dl = 0){
  if(BlinkSettings >= dl){
    BlinkSettings = 0;
    if(digitalRead(NeonL) == LOW)
      digitalWrite(NeonL, HIGH); 
    else
      digitalWrite(NeonL, LOW); 
  }
  BlinkSettings++;  
}

void MonthChk(){
  switch(Month){
    case 1:
      if(Day >= 32)
        Day = 0;
      break;
    case 2:
      if(Day >= 29)
        Day = 0;
      break;  
    case 3:
      if(Day >= 32)
        Day = 0;
      break; 
    case 4:
      if(Day >= 31)
        Day = 0;
      break; 
    case 5:
      if(Day >= 32)
        Day = 0;
      break; 
    case 6:
      if(Day >= 31)
        Day = 0;
      break; 
    case 7:
      if(Day >= 32)
        Day = 0;
      break; 
    case 8:
      if(Day >= 32)
        Day = 0;
      break; 
    case 9:
      if(Day >= 31)
        Day = 0;
      break; 
    case 10:
      if(Day >= 32)
        Day = 0;
      break; 
    case 11:
      if(Day >= 31)
        Day = 0;
      break; 
    case 12:
      if(Day >= 32)
        Day = 0;
      break;     
  }
}

void Lamp_Antipoisoning(){
    for(uint8_t i = 0; i < 10; i++){
      FrstL = i;
      ScndL = i;
      ThrdL = i;
      FrthL = i;
      TIME_SHOW(3);
    }
}

void loop() { 
  
  DateTime now = rtc.now();

  if(digitalRead(FBtn) == HIGH){
    delay(500);
    Settings++;
  }
  if(Settings != 0){
    digitalWrite(NeonL, LOW); 
    if(Settings >= 5){
      Settings = 0;
      rtc.adjust(DateTime(2021, Month, Day, HRS, MNTS, 0));
    }
    if(HRS == 24)
      HRS = 0;
    if(MNTS == 60)
      MNTS = 0;
    MonthChk();
    if(Month == 13)
      Month = 0;    
    if(digitalRead(SBtn) == HIGH){
      delay(200);
      chk = true;
    }        
    switch(Settings){
      case 1:
        if(chk == true){          
          HRS++;                                     
        }
        FrstL = HRS / 10;
        ScndL = HRS % 10; 
        ThrdL = MNTS / 10;
        FrthL = MNTS % 10;
        BlnkSttngs(3);                              
        break;
      case 2:   
        if(chk == true){          
          MNTS++;          
        }   
        ThrdL = MNTS / 10;
        FrthL = MNTS % 10;
        BlnkSttngs(5);               
        break;
      case 4:  
        if(chk == true){           
          Day++;          
        }    
        FrstL = Day / 10;
        ScndL = Day % 10; 
        ThrdL = Month / 10;
        FrthL = Month % 10;  
        BlnkSttngs(8);   
        break;
      case 3: 
        if(chk == true){           
          Month++;           
        } 
        FrstL = Day / 10;
        ScndL = Day % 10;  
        ThrdL = Month / 10;
        FrthL = Month % 10;   
        BlnkSttngs(11);     
        break;
    }
    chk = false; 
  }
    
  if(Settings == 0){
    HRS = now.hour();
    MNTS = now.minute();
    Sec = now.second();
    Day = now.day();
    Month = now.month();
    
    if(digitalRead(SBtn) == HIGH){
      delay(500);
      DateShow++;
      if(DateShow == 3){
        DateShow = 0;
        analogWrite(PWM_pin, 175);
      }
      Lamp_Antipoisoning();

    }

    if(DateShow == 2){
      analogWrite(PWM_pin, 0);   
    }
     
    if(DateShow == false){
      FrstL = HRS / 10;
      ScndL = HRS % 10;
      ThrdL = MNTS / 10;
      FrthL = MNTS % 10;
      Blink();
    }
    else{
      FrstL = Day / 10;
      ScndL = Day % 10;
      ThrdL = Month / 10;
      FrthL = Month % 10;  
      digitalWrite(NeonL, HIGH); 
    }
      if(DateShow != 2){
        switch(MNTS){
        case 10:
          DateShow = 1;
          if (ChkAntiPois == false){
            Lamp_Antipoisoning();
            ChkAntiPois = true;
          }
          if(Sec >= 10){
            DateShow = 0;
          }
          break;    
        case 11:
          ChkAntiPois = false;
          break;    
        /*case 25:
          DateShow = 1;
          if(Sec >= 10){
            DateShow = 0;
          } 
          break;*/
        case 40:
          DateShow = 1;
          if(Sec >= 10){
            DateShow = 0;
          } 
          break; 
        /*case 55:
          DateShow = 1;
          if(Sec >= 10){
            DateShow = 0;
          } 
          break; */
      } 
    }
  }
  
  TIME_SHOW(ShwnTm);    
}

 
