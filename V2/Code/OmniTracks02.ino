#include <IBusBM.h>
IBusBM IBus; // IBus object

int ch1;
int ch2;
int ch3;
int ch4;
int ch5;
int ch6;
int ch7;
int ch8;
int ch9;
int ch10;

float drive1;
float drive2;
float drive3;

float drive1Filtered;
float drive2Filtered;
float drive3Filtered;

float out1;
float out2;
float out3;
float out1a;
float out2a;
float out3a;

unsigned long currentMillis;
long previousMillis = 0;    // set up timers


void setup() {
  Serial.begin(115200);   // remove comment from this line if you change the Serial port in the next line
  IBus.begin(Serial3);    // change to Serial1 or Serial2 port when required

  pinMode(2, OUTPUT);
  pinMode(3, OUTPUT);
  pinMode(4, OUTPUT);
  pinMode(5, OUTPUT);
  pinMode(6, OUTPUT);
  pinMode(7, OUTPUT);
}

void loop() {

    ch1 = IBus.readChannel(0); // get latest value for servo channel 0
    ch2 = IBus.readChannel(1); // get latest value for servo channel 1
    ch3 = IBus.readChannel(2); // get latest value for servo channel 3
    ch4 = IBus.readChannel(3); // get latest value for servo channel 4
    ch5 = IBus.readChannel(4); // get latest value for servo channel 5
    ch6 = IBus.readChannel(5); // get latest value for servo channel 6
    ch7 = IBus.readChannel(6); // get latest value for servo channel 7
    ch8 = IBus.readChannel(7); // get latest value for servo channel 8
    ch9 = IBus.readChannel(8); // get latest value for servo channel 9
    ch10 = IBus.readChannel(9); // get latest value for servo channel 10

    currentMillis = millis();
    if (currentMillis - previousMillis >= 10) {  // start timed event
          
        previousMillis = currentMillis;

        drive1 = thresholdStick(ch4);
        drive2 = thresholdStick(ch2);
        drive3 = thresholdStick(ch5);

        drive1Filtered = filter(drive1, drive1Filtered, 30);
        drive2Filtered = filter(drive2, drive2Filtered, 30);
        drive3Filtered = filter(drive3, drive3Filtered, 30);
    
        out1 = drive1 + (drive2 * 0.66) - drive3;
        out2 = drive1 - (drive2 * 0.66) + drive3;
        out3 = drive2 + drive3;

        out1 = constrain(out1, -255,255);
        out2 = constrain(out2, -255,255);
        out3 = constrain(out3, -255,255);

        // ***wheel1

        if (out1 >= 0) {
          analogWrite(2, out1);
          analogWrite(3, 0);
        }

        else if (out1 <= 0) {
          out1a = abs(out1);
          analogWrite(3, out1a);
          analogWrite(2, 0);
        }
        else {
          analogWrite(3, 0);
          analogWrite(2, 0);
        }

        // ***wheel2

        if (out2 >= 0) {
          analogWrite(4, out2);
          analogWrite(5, 0);
        }

        else if (out2 <= 0) {
          out2a = abs(out2);
          analogWrite(5, out2a);
          analogWrite(4, 0);
        }
        else {
          analogWrite(4, 0);
          analogWrite(5, 0);
        }

        // ***wheel3

        if (out3 >= 0) {
          analogWrite(7, out3);
          analogWrite(6, 0);
        }

        else if (out3 <= 0) {
          out3a = abs(out3);
          analogWrite(6, out3a);
          analogWrite(7, 0);
        }
        else {
          analogWrite(6, 0);
          analogWrite(7, 0);
        }

    }


}


