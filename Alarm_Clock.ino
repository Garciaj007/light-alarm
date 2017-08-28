const int redLED = 11;
const int blueLED = 10;
const int greenLED = 9;
const int nightLED = 6;
const int dayLED = 5;
const int btn1 = 8;
const int btn2 = 7;
const int speaker = 3;

int redValue = 0;
int greenValue = 0;
int blueValue = 0;
int nightValue = 0;
int dayValue = 0;

//both buttons activate the menu when either or are pressed
int btn1State = 0; // this button sets the mode 1 - SNOOZE | 2 - STOP ||  hold to select(btn 2 value) ---> [if selected light up 3rd LED Green to set Time; Blue to set Alarm and sound alarm] use this button to confirm settime |OR| setalarm
int btn2State = 0; // this button sets the time 1 - Set Alarm || hold to select(btn 1 value) ---> [if selected light up LED corresponding to DayMode light LEDS fully |OR| NightMODE fade then out] then using this button set the time for every 10 intervals light light and move on to a new one. then select minutes every 10 min intervals
//PRESS BOTH BUTTONS ON THIRD FUNCTION TO SNOOZE -|OR|- TO STOP

int ledState = LOW;
bool menu = false;
bool disabled = false;
bool playSound = false;

unsigned long previousTime = 0;
long interval = 1000;

int seconds = 0;
int minutes = 0;
int hours = 0;

void setup() {
  Serial.begin(9600);

  pinMode(redLED, OUTPUT);
  pinMode(blueLED, OUTPUT);
  pinMode(greenLED, OUTPUT);
  pinMode(nightLED, OUTPUT);
  pinMode(dayLED, OUTPUT);

  pinMode(btn1, INPUT);
  pinMode(btn2, INPUT);

}

void loop() {

  unsigned long currentTime = millis();
  unsigned long xTime = millis()/5000;

    if(currentTime - previousTime > interval){

      previousTime = currentTime;

      if(ledState == LOW){
        //Serial.println("High");
        ledState = HIGH;
      } else {
        //Serial.println("Low");
        ledState = LOW;
      }

      //digitalWrite(nightLED, ledState);
      seconds++;
      Time(seconds);

      if(menu && !disabled){
        digitalWrite(nightLED, ledState);
        digitalWrite(dayLED, ledState);
        mainMenu();
      } else {
        if(btn1State == HIGH || btn2State == HIGH){
            menu = true;
        }
      }

    }
}

void Time (int seconds){
  if(seconds >= 60){
    minutes++;
    seconds = 0;
    Serial.print(minutes);
    Serial.println(" : Minutes");
  }

  if(minutes >= 60){
    hours++;
    minutes = 0;
    Serial.print(hours);
    Serial.println(" : hours");
  }

  if(hours >= 24){
    hours = 0;
  }
}

void Alarm (){
  //wakes up at 10:01 am;
  if(hours == 10 && minutes == 1){
    long internalClock = (0.5 * sin((xTime - 1.5))) + 0.5
    redValue = fade(100.0);
    greenValue = fade(120.0);
    blueValue = fade(128.0);

    if(playSound){
      tone(speaker, 294, 500);
    }
  }
}

void setTime() {
  minutes = 10;
  hours = 0;
  seconds = 0;
}

int mainMenu(){
  if(btn1State == HIGH){
    Serial.println("Press other button to SNOOZE");
    if(btn2State == HIGH){
      //turn off alarm and wake up again int 10 minutes
      disabled = true;
      menu = false;
      return 0;
    }
  }

  if(btn2State == HIGH){
    Serial.println("Press other button to STOP");
    if(btn1State == HIGH){
      //turn off alarm
      disabled = true;
      menu = false;
      return 0;
    }
  }
}

int fade(float range){
  float xTime = millis()/5000.0;
  int value = range + (int)range * sin(xTime*2.0*PI);
  return value;
}
