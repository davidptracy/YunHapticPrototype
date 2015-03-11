/* 
Code for haptic wrist device by David P Tracy - davidptracy@gmail.com - davidptracy.com

This code is for a circuit with TLC 5940 PWM driver with 10+ 5V vibration motors.

Each motor requires a separate circuit with the following components: 
10k ohm pull-up resistor (from each TLC pin out)
2N3904 Transistor Base: 1k ohm resistor from TLC pin out channel 
2N3904 Transistor Collector: Connect to 5V
2N3904 Transistor Emitter: Vibe Motor (other lead goes to ground)

For user with Arduino Yun + Node.js with Node WS (websockets)

**** CREATE GITHUB REPO FOR TLC 5940 + Leonardo ****
TLC 5940 must be configured for Leonardo - see github repo 

*/

#include <Bridge.h>
#include <Process.h>
#include <Tlc5940.h>

// make a new Process for calling Node
Process node;

int motor_count = 10;

//Three axes values
int alpha;
int beta;
int gamma;

void setup() {
 
  // initialize bridge and serial
  Serial.begin(9600);
  
  delay(120000);
  
  Bridge.begin();
  
  Serial.println("========== PROCESS INITIATED ==========");
  
  // launch the echo.js script asynchronously:
  node.runShellCommandAsynchronously("killall node");
  node.runShellCommandAsynchronously("killall nodejs");
  
  //wait 1 seconds
  delay(1000);
  node.runShellCommandAsynchronously("node /mnt/sda1/arduino/node/SocketIOClient.js");
  
  // Call Tlc.init() to setup the tlc.
  // You can optionally pass an initial PWM value (0 - 4095) for all channels. 
  Tlc.init(); 

}

void loop() {
  // put your main code here, to run repeatedly:
  
  String a = node.readStringUntil(',');
  String b = node.readStringUntil(',');
  String c = node.readStringUntil('\n');
  
  alpha = map(a.toInt(), 0, 360, 5, 175);
  beta = map(b.toInt(), -90, 90, 5, 175);
  gamma = map(c.toInt(), -180, 180, 5, 175); 
  
  // convert the string into an integer
//  int sensorVal = sensorStr.toInt();
  
  Serial.println(beta);
  
  intensity(beta);
     
}

void intensity(int rate){

  rate = map(rate, 5, 175, 0, 3000);

  Serial.println(rate);  
  
  for (int row = 0; row < 5; row ++){
    
    Tlc.clear();
    
    for (int channel = 0; channel < 10; channel ++){
      if (channel == row || channel == row+5){
        Tlc.set(channel, rate);
      } else {
        Tlc.set(channel, rate);
      } 
    }
    
    Tlc.update();
    
//    delay(rate);
    
  }
    
}
