const int lightPin = 5;

const int redLEDPin = 11;
const int blueLEDPin = 10;
const int greenLEDPin = 9;

unsigned long previousTime = 0;

long secInterval = 1000;

int seconds = 0;
int minutes = 0;
int hours = 0;

int setTime[3] = {11,49,50}; //hours-minutes-seconds | current time uses 24hour clock then resets.
int preAlarmTime[3] = {11, 50, 0};
int wakeupTime[3] = {11, 55, 0};//hours-minutes-seconds | time to wake up.

bool clockState = false;
bool alarmActive = false;
bool preAlarmActive = false;

float sineWave = 0;
float colorValue = 0;

void setup() {
    Serial.begin(9600);

    pinMode(lightPin, OUTPUT);

    pinMode(redLEDPin, OUTPUT);
    pinMode(blueLEDPin, OUTPUT);
    pinMode(greenLEDPin, OUTPUT);

    hours = setTime[0];
    minutes = setTime[1];
    seconds = setTime[2];
}

void loop() {
  clock();
}

void clock(){
  unsigned long currentTime = millis();
  preAlarm();
  alarm();

  if(currentTime - previousTime > secInterval){
    previousTime = currentTime;
    getTime();
    seconds++;
  }

  if(seconds > 60){
    minutes++;
    seconds = 0;
  }

  if(minutes > 60){
    hours++;
    minutes = 0;
  }

  if(hours > 24){
    hours = 0;
  }
}

void getTime (){
  Serial.print(hours);
  Serial.print(" : ");
  Serial.print(minutes);
  Serial.print(" : ");
  Serial.println(seconds);
}

void preAlarm(){
  if(hours == preAlarmTime[0] && minutes == preAlarmTime[1] && seconds == preAlarmTime[2]){
    Serial.println("Pre-Alarm Active");
    preAlarmActive = true;
  }

  if(preAlarmActive && seconds < 5){
    colorFade(0.001, 0, 50, redLEDPin);
    colorFade(0.001, 0, 255, blueLEDPin);
    colorFade(0.001, 0, 255, greenLEDPin);
    preAlarmTime[1] = preAlarmTime[1] + 1;
  }

  if(seconds >= 5){
    preAlarmActive = false;
  }
}


void alarm(){
  if(hours == wakeupTime[0] && minutes == wakeupTime[1] && seconds == wakeupTime[2]){
    Serial.println("Alarm sound");
    alarmActive = true;
  }

  if(alarmActive){
    colorFade(0.001, 0, 50, redLEDPin);
    colorFade(0.001, 0, 255, blueLEDPin);
    colorFade(0.001, 0, 255, greenLEDPin);
  }
}

void switchState(bool state){
  if(state){
      state = false;
    } else {
      state = true;
    }
}

//rate can only be a value > 0; minRange is the min brightness; maxRange is maxBrightness, pin, is what pin to fade to
void colorFade(float rate, int minRange, int maxRange, int pin){
  sineWave = (0.5 * sin(millis() * rate)) + 0.5;//I JUST CREATED A SINE WAVE OMG SYNTHESIS HERE WE COME!
  colorValue = mapf(sineWave, 0, 1, minRange, maxRange);
  analogWrite(pin, colorValue);
}

int pitchLFO(float rate, int minRange, int maxRange){
  sineWave = (0.5 * sin(millis() * rate)) + 0.5;
  int lfoWave = mapf(sineWave, 0, 1, minRange, maxRange);
  return lfoWave;
}

float mapf(float x, float in_min, float in_max, float out_min, float out_max){
  return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}
