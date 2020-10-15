//#include <avr/power.h>
//#include <avr/sleep.h>
//#include <avr/interrupt.h>
#include <OneWire.h>
#include <DallasTemperature.h>

#define X0 3
#define X1 6
#define X2 5
#define X3 10
#define ConvVCC 7
#define PntL 1
#define PntR 17

#define Frst 0
#define Scnd 16
#define Thrd 15

#define Btn 2

#define DS_Sens 4

#define PWM_pin 9

int temp = 0;
int ShwnTm = 120;
int Shw = 0;                     
int btn = 0;
bool slp = false;

int FrstL = 0, ScndL = 0, ThrdL = 0;

long lastUpdateTime = 0;
const int TEMP_UPDATE_TIME = 1000;
OneWire DS0(DS_Sens);
DallasTemperature TempSensor(&DS0);
bool Chk = false;


void setup() {
 /* ADCSRA &= ~(1 << ADEN); 
  power_usart0_disable();
  power_twi_disable();  */
 // attachInterrupt(Btn, ButInter, RISING); 
  
TCCR1A = 0b00000001;  // 8bit
TCCR1B = 0b00000001;
  pinMode(Frst, OUTPUT);  
  pinMode(Scnd, OUTPUT);
  pinMode(Thrd, OUTPUT);

  pinMode(X0, OUTPUT);
  pinMode(X1, OUTPUT); 
  pinMode(X2, OUTPUT); 
  pinMode(X3, OUTPUT); 
  pinMode(ConvVCC, OUTPUT);  
  pinMode(PWM_pin, OUTPUT);  

 analogWrite(PWM_pin, 170);
 digitalWrite(ConvVCC, HIGH);
 digitalWrite(Frst, HIGH); 
 digitalWrite(Scnd, LOW); 
 digitalWrite(Thrd, LOW);  
 
  TempSensor.begin();
  TempSensor.setResolution(10);
  
  //set_sleep_mode(SLEEP_MODE_PWR_SAVE);
  //sleep_enable();
  //sleep_mode(); 
}

void tabl(int numr = 0){ //ДАННАЯ ТАБЛИЦА ИСТИННОСТИ ВЕРНА ТОЛЬКО ДЛЯ ДАННОЙ ПЛАТЫ ТЕРМОМЕТРА
  switch(numr){
    case 1: 
      digitalWrite(X0, LOW); 
      digitalWrite(X1, LOW); 
      digitalWrite(X2, LOW); 
      digitalWrite(X3, LOW);      
    break;
    case 0:
      digitalWrite(X0, HIGH); 
      digitalWrite(X1, LOW); 
      digitalWrite(X2, LOW); 
      digitalWrite(X3, LOW);   
    break;
    case 4:
      digitalWrite(X0, LOW); 
      digitalWrite(X1, HIGH); 
      digitalWrite(X2, LOW); 
      digitalWrite(X3, LOW); 
    break;
    case 8:
      digitalWrite(X0, HIGH); 
      digitalWrite(X1, HIGH); 
      digitalWrite(X2, LOW); 
      digitalWrite(X3, LOW); 
    break;
    case 2:
      digitalWrite(X0, LOW); 
      digitalWrite(X1, LOW); 
      digitalWrite(X2, HIGH); 
      digitalWrite(X3, LOW); 
    break;
    case 6:
      digitalWrite(X0, HIGH); 
      digitalWrite(X1, LOW); 
      digitalWrite(X2, HIGH); 
      digitalWrite(X3, LOW); 
    break;
    case 5:
      digitalWrite(X0, LOW); 
      digitalWrite(X1, HIGH); 
      digitalWrite(X2, HIGH); 
      digitalWrite(X3, LOW); 
    break;
    case 9:
      digitalWrite(X0, HIGH); 
      digitalWrite(X1, HIGH); 
      digitalWrite(X2, HIGH); 
      digitalWrite(X3, LOW); 
    break;
    case 3:
      digitalWrite(X0, LOW); 
      digitalWrite(X1, LOW); 
      digitalWrite(X2, LOW); 
      digitalWrite(X3, HIGH); 
    break;
    case 7:
      digitalWrite(X0, HIGH); 
      digitalWrite(X1, LOW); 
      digitalWrite(X2, LOW); 
      digitalWrite(X3, HIGH); 
    break;   
  }  
}

void TEMP(int a = 0){
  for(int i = 0; i < a; i++)
    {
      digitalWrite(Frst, HIGH);
      tabl(FrstL);
      delay(2); 
      digitalWrite(Frst, LOW);
      delay(2);  
             
      digitalWrite(Scnd, HIGH);
      if(Chk == false)
        digitalWrite(PntR, HIGH);
      tabl(ScndL);
      delay(2); 
      digitalWrite(Scnd, LOW);
      if(Chk == false)
        digitalWrite(PntR, LOW);
      delay(2); 
      
      digitalWrite(Thrd, HIGH);
      if(Chk == true)
        digitalWrite(PntR, HIGH);
      tabl(ThrdL);
      delay(2); 
      digitalWrite(Thrd, LOW);
      if(Chk == true)
        digitalWrite(PntR, LOW);
      delay(2); 
    } 
}

/*void ButInter(){    
    btn++;   
  }*/
int o = 0;
void loop() { 

  /*if(slp){     
    sleep_mode();
    slp = false;
  }  */
  if(millis() - lastUpdateTime > TEMP_UPDATE_TIME){
    lastUpdateTime = millis();
    TempSensor.requestTemperatures();
    float a = TempSensor.getTempCByIndex(0);
    if(a >= 100){
      temp = a;
      Chk = true;
    }
    else{
      temp = a * 10;
      Chk = false;
    }
  }
  FrstL = temp / 100;
  ScndL = (temp % 100)/10;
  ThrdL = temp % 10;

 /* if(Shw == 0){    
    
    digitalWrite(ConvVCC, HIGH);          
    analogWrite(PWM_pin, 155); // Переменная ШИМ
    delay(500);     
       
  }  */     

 // if(Shw == 1){             
    TEMP(ShwnTm);    
    /*analogWrite(5, 0);
    digitalWrite(0, LOW); 
    TmShw = 0; 
    Chk = false; 
    btn = 0; 
    sleep_mode();  */
 // } 

 
}
 
