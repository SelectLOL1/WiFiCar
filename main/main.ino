#define FORWARD 25
#define BACKWARD 32
#define LEFT 26
#define RIGHT 27
#define STEERING_POS 33

#define PWMFREQ 5000
#define PWMCHANNEL_FORWARD 0
#define PWMCHANNEL_BACKWARD 3
#define PWM_bitRES 10

//Trying PWM to control car, not the final code

// the setup function runs once when you press reset or power the board
void setup() {
  // initialize digital pin LED_BUILTIN as an output.
  ledcSetup(PWMCHANNEL_FORWARD, PWMFREQ, PWM_bitRES);
  ledcAttachPin(FORWARD, PWMCHANNEL_FORWARD);

  ledcSetup(PWMCHANNEL_BACKWARD, PWMFREQ, PWM_bitRES);
  ledcAttachPin(BACKWARD, PWMCHANNEL_BACKWARD);

  //pinMode(LEFT, OUTPUT);
  //pinMode(RIGHT, OUTPUT);
  //ledcSetup(PWMCHANNEL_RIGHT, PWMFREQ, PWM_bitRES);
  //ledcSetup(PWMCHANNEL_LEFT, PWMFREQ, PWM_bitRES);
  //ledcAttachPin(RIGHT, PWMCHANNEL_RIGHT);
  //ledcAttachPin(LEFT, PWMCHANNEL_LEFT);
  
  Serial.begin(115200);
}

// the loop function runs over and over again forever
void loop() {
  for(unsigned int letsgo=100;letsgo<700;letsgo++){
    ledcWrite(PWMCHANNEL_FORWARD,letsgo);
    delay(1);
  }
  delay(10);
  for(unsigned int letsgo2=100;letsgo2<700;letsgo2++){
    ledcWrite(PWMCHANNEL_BACKWARD,letsgo2);
    delay(1);
  }
  delay(10);
  // for(unsigned char amount = 0;amount<20;amount++){
  //   ledcWrite(PWMCHANNEL_LEFT, 1000);   
  //   Serial.println(analogRead(STEERING_POS));
  //   delay(10);   
  // }                  
  // for(unsigned char amount = 0;amount<20;amount++){
  //   ledcWrite(PWMCHANNEL_RIGHT, 1000);   
  //   Serial.println(analogRead(STEERING_POS));
  //   delay(10);   
  // }               
}