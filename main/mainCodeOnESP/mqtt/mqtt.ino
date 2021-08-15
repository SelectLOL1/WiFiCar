#include <WiFi.h>
#include <PubSubClient.h>
#include <string.h>

#define FORWARD 25
#define BACKWARD 32
#define LEFT 26
#define RIGHT 27
#define STEERING_POS 33

#define PWMFREQ 5000
#define PWMCHANNEL_FORWARD 0
#define PWMCHANNEL_BACKWARD 3
#define PWMCHANNEL_LEFT 5
#define PWMCHANNEL_RIGHT 7
#define PWM_bitRES 10

#define LEFT_MAX_ANALOGREAD 2800
#define RIGHT_MAX_ANALOGREAD 1280

const char *ssid = "ssis";
const char *password = "pw";
const int mqttPort = 1883;
const char *where2listen = "car/recieve";
const char *where2listenspeed = "car/speed";
const char *where2send = "car/status";

unsigned long currentMillis = 0;
bool firstTimeGettingCallback = 1;

uint16_t steeringposition = 0;
int ForwardBackwardsGlobalSpeed = 400;

String oldresponse;

IPAddress broker(192,168,25,27); // IP address of your MQTT broker eg. 192.168.1.50

WiFiClient espClient;
PubSubClient client(espClient);

//[Forwards, Backwards, Right, Left]

void callback(char *topic, byte *payload, unsigned int length)
{
  //Serial.print("Message arrived in topic: ");
  //Serial.println(topic);
  //Serial.println("Message:");
  if(firstTimeGettingCallback == 1){
    currentMillis = millis();
    firstTimeGettingCallback = 0;
  }

  currentMillis = millis();

  String response;

  if((strcmp(topic,where2listen)) == 0){

    for (int i = 0; i < length; i++)
    {
      response += (char)payload[i];
      //Serial.print((char)payload[i]);
    }
    //Serial.println("");
    //Serial.println("");
    //Serial.println("-----------------------");
    
    response = response.substring(0,4);
    oldresponse = oldresponse.substring(0,4);

    //Serial.println();
    //Serial.println("-----------------------");
    Serial.println(analogRead(STEERING_POS));

    steeringposition = (analogRead(STEERING_POS));

    if((response != oldresponse) || (steeringposition > RIGHT_MAX_ANALOGREAD) || (steeringposition < LEFT_MAX_ANALOGREAD)){
      if(response[0] == '1'){
        //go forward
        ledcWrite(PWMCHANNEL_FORWARD,ForwardBackwardsGlobalSpeed);
        delay(10);
        client.publish(where2send, "go forward");
      }
      else{
        // set forward low
        ledcWrite(PWMCHANNEL_FORWARD,0);
        delay(1);
        client.publish(where2send, "set forward low");
    }

      if(response[1]=='1'){
        //go backwards
        ledcWrite(PWMCHANNEL_BACKWARD,ForwardBackwardsGlobalSpeed);
        delay(10);
        client.publish(where2send, "go backwards");
      }
      else{
        // set backwards low
        ledcWrite(PWMCHANNEL_BACKWARD,0);
        delay(1);
        client.publish(where2send, "set backwards low");
      }
      if(response[2]=='1'){
        //go right max 1200 analogread
        Serial.println(analogRead(STEERING_POS));
        delay(10);
        if (steeringposition > RIGHT_MAX_ANALOGREAD){
          ledcWrite(PWMCHANNEL_RIGHT,550);
          delay(5);
          client.publish(where2send, "go right");
        }
        else{
          ledcWrite(PWMCHANNEL_RIGHT,0);
          delay(1);
          client.publish(where2send, "set right low");
        }
      }
      else{
        // set right low
        ledcWrite(PWMCHANNEL_RIGHT,0);
        delay(1);
        client.publish(where2send, "set right low");
      }

      if(response[3]=='1'){
        //go left max 2800 analogread
        Serial.println(analogRead(STEERING_POS));
        delay(10);
        if (steeringposition < LEFT_MAX_ANALOGREAD){
          ledcWrite(PWMCHANNEL_LEFT,550);
          delay(5);
          client.publish(where2send, "go left");
        }
        else{
          ledcWrite(PWMCHANNEL_LEFT,0);
          delay(1);
          client.publish(where2send, "set left low");
        }
      }
      else{
        // set left low
        ledcWrite(PWMCHANNEL_LEFT,0);
        delay(1);
        client.publish(where2send, "set left low");
      }
      //client.publish(where2send, "suck it");
    oldresponse = response;
    }
  }
  else if((strcmp(topic, where2listenspeed)) == 0){
    for (int i = 0; i < length; i++)
    {
      response += (char)payload[i];
      //Serial.print((char)payload[i]);
    }
    //Serial.println("");
    //Serial.println("");
    //Serial.println("-----------------------");
  
    ForwardBackwardsGlobalSpeed = response.toInt();
  }
}

void setup()
{
  Serial.begin(115200);

  ledcSetup(PWMCHANNEL_FORWARD, PWMFREQ, PWM_bitRES);
  ledcAttachPin(FORWARD, PWMCHANNEL_FORWARD);

  ledcSetup(PWMCHANNEL_BACKWARD, PWMFREQ, PWM_bitRES);
  ledcAttachPin(BACKWARD, PWMCHANNEL_BACKWARD);

  ledcSetup(PWMCHANNEL_RIGHT, PWMFREQ, PWM_bitRES);
  ledcAttachPin(RIGHT, PWMCHANNEL_RIGHT);

  ledcSetup(PWMCHANNEL_LEFT, PWMFREQ, PWM_bitRES);
  ledcAttachPin(LEFT, PWMCHANNEL_LEFT);

  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    //Serial.println("Connecting to WiFi..");
  }
  //Serial.println("Connected to the WiFi network");

  client.setServer(broker, mqttPort);
  client.setCallback(callback);

  while (!client.connected())
  {
    //Serial.println("Connecting to MQTT...");

    if (client.connect("ESP32Client"))
    {
      //Serial.println("connected");
    }
    else
    {
      //Serial.print("failed with state ");
      //Serial.print(client.state());
      delay(2000);
    }
  }
  client.subscribe(where2listen);
  client.subscribe(where2listenspeed);

  ledcWrite(PWMCHANNEL_LEFT,0);
  ledcWrite(PWMCHANNEL_RIGHT,0);
  ledcWrite(PWMCHANNEL_FORWARD,0);
  ledcWrite(PWMCHANNEL_BACKWARD,0);

}

void loop()
{
  client.loop();
  if((firstTimeGettingCallback == 0) && (millis() - currentMillis) >= 1000){
    Serial.println("Looks like client died");
    ESP.restart();
  }
}
