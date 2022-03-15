// Radio
#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>

RF24 radio(7, 8); // CE, CSN
const byte addresses[][6] = {"00001", "00002"};

//**************remote control****************
struct RECEIVE_DATA_STRUCTURE{
  //put your variable definitions here for the data you want to send
  //THIS MUST BE EXACTLY THE SAME ON THE OTHER ARDUINO
    int16_t menuDown;  
    int16_t Select;    
    int16_t menuUp;  
    int16_t toggleBottom;  
    int16_t toggleTop; 
    int16_t toggle1;
    int16_t toggle2;
    int16_t mode;  
    int16_t RLR;
    int16_t RFB;
    int16_t RT;
    int16_t LLR;
    int16_t LFB;
    int16_t LT;
};

RECEIVE_DATA_STRUCTURE mydata_remote;

int RLR = 0;
int RFB = 0;
int RFBa = 0;
int RT = 0;
int LLR = 0;
int LFB = 0;
int LT = 0;

int output1;
int output1a;
int output2;
int output2a;
int output3;
int output3a;

unsigned long currentMillis;
long previousMillis = 0;    // set up timers
long interval = 10;        // time constant for timer


void setup() {

    // initialize serial communication
    Serial.begin(115200);
    
    radio.begin();
    radio.openWritingPipe(addresses[0]); // 00002
    radio.openReadingPipe(1, addresses[1]); // 00001
    radio.setPALevel(RF24_PA_MIN);
    radio.startListening();

    pinMode(2,OUTPUT);
    pinMode(3,OUTPUT);
    pinMode(4,OUTPUT);
    pinMode(5,OUTPUT);
    pinMode(10,OUTPUT);
    pinMode(11,OUTPUT);
   
}   // end of setup

// ********************* MAIN LOOP *******************************

void loop() {  

      
        currentMillis = millis();
        if (currentMillis - previousMillis >= 10) {  // start timed event
          
            previousMillis = currentMillis;


            // check for radio data
            if (radio.available()) {
                    radio.read(&mydata_remote, sizeof(RECEIVE_DATA_STRUCTURE));   
            }             

            else {
              Serial.println("no data");
            }


            // threshold remote data
            // some are reversed based on stick wiring in remote
            RFB = thresholdStick(mydata_remote.RFB)/2;   
            RLR = thresholdStick(mydata_remote.RLR)/2;
            LT = thresholdStick(mydata_remote.LT)/2; 

            Serial.print(RFB);
            Serial.print(" , ");
            Serial.print(RLR);
            Serial.print(" , ");
            Serial.print(LT);
            Serial.println();

            output1 = RFB - (RLR * 0.66) - LT;
            output2 = RFB + (RLR * 0.66) + LT;
            output3 = RLR - LT;

            output1 = constrain(output1,-255,255);
            output2 = constrain(output2,-255,255);
            output3 = constrain(output3,-255,255);

            // track 1
            if (output1 > 0) {
              analogWrite(3, output1);
              analogWrite(2, 0);
            }

            else if (output1 < 0) {
              output1a = abs(output1);
              analogWrite(2, output1a);
              analogWrite(3, 0);
            }

            else {
              analogWrite(3, 0);
              analogWrite(2, 0);
            }

            // track 2
            if (output2 > 0) {
              analogWrite(4, output2);
              analogWrite(5, 0);
            }

            else if (output2 < 0) {
              output2a = abs(output2);
              analogWrite(5, output2a);
              analogWrite(4, 0);
            }

            else {
              analogWrite(4, 0);
              analogWrite(5, 0);
            }

            //track3
            if (output3 > 0) {
              analogWrite(11, output3);
              analogWrite(10, 0);
            }

            else if (output3 < 0) {
              output3a = abs(output3);
              analogWrite(10, output3a);
              analogWrite(11, 0);
            }

            else {
              analogWrite(10, 0);
              analogWrite(11, 0);
            }
            

            


            
        }     // end of timed loop         

   
}       // end  of main loop
