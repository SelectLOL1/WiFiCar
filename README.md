# WiFiCar
Toy car that is controlled over WiFi.

A Toy remote controlled car that I reverse enginnered. I checked which pins of the microcontroller controler Forward,Backward, Left and Right. I made the output of the ESP compatible with the H-Bridge that controls Foward and Backward. Left and Right did not need any modification to the circuit. The Servo that controls Left and Right also has a potentiometer that outputs a value corresponding the current position of the front steering tires.
<br/><br/>
So all in all I have 4 PWM pins for control and 1 Analog input for the steering position.
<br/><br/>

The ESP basically listens to MQTT data on **"car/recieve"**, the data packages that are formatted as follows:

```
x = 1 or 0 (1 means move, 0 means dont)

[x,x,x,x]

[Forwards, Backwards, Right, Left]
```
If you want the car to go forwards you send:
```
[1,0,0,0] to "car/recieve"
```

For the speed the car listens to **"car/speed"**, the default value is 300.

When the car recieves data it sends its current status to **"car/status"**.
<br/><br/>

Over node-red the car can be controlled over the UI, the problem is that you cannot really detect when a key is pressed or released. It can only listen for characters, thats why I also cannot read if multiple keys are pressed. I wanted a fast solution to the browser limitation, so I ditched the browser version later on. The car and its speed can still be controlled over the website.

The code for the node-red version is in the **nodered** folder. Adittional packages must be installed
<br/><br/>

A better way of controlling the car is over python. The code is pretty self explanatory. 

Prerequisites for the python code:
```
import paho.mqtt.client as mqtt #paho
import keyboard #keyboard
```

# Demo

![wificar](gifs/wificardemo.gif)