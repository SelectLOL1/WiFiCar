import paho.mqtt.client as mqtt
import keyboard
import time

getDirectionTopic = "car/recieve"
getSpeedTopic = "car/speed"
ipAddressOfBroker = "192.168.25.27" #where the mqtt broker is
defaultSend = "0000"
mylistkeyboard = ["w","s","d","a"]
nokeyboardpressed = 1

client = mqtt.Client()
client.connect(ipAddressOfBroker,1883,60)

while (1) : 
    defaultSend = "0000"
    nokeyboardpressed = 1
    for i in range(4):
        if(keyboard.is_pressed(mylistkeyboard[i])):
            nokeyboardpressed = 0
            defaultSend = list(defaultSend)
            defaultSend[i] = "1"
            defaultSend = "".join(defaultSend)
    client.publish(getDirectionTopic, defaultSend)
    time.sleep(0.03)
    if nokeyboardpressed == 1:
        client.publish(getDirectionTopic, defaultSend)
    print(defaultSend)
    time.sleep(0.03)
    
client.disconnect()
