#include <mcp_can.h>  // library for MCP2515 ic
#include <SPI.h>      // library for SPI communication

long unsigned int rxId;  // storage for can data
unsigned char len = 0;   // storage for can data
unsigned char rxBuf[8];  // storage for can data

#define CAN0_INT 2  // Set INT to pin 2
#define CAN1_INT 3
MCP_CAN CAN0(10);   // set CS pin to 10r
MCP_CAN CAN1(9); 


bool button1;
bool button2;
bool button3;
bool button4;
bool button5;
bool button6;
bool button7;
bool button8;
bool button9;
bool button10;
bool button11;
bool button12;

byte out1 = 4;
byte out2 = 5;
byte out3 = 6;
byte out4 = 7;



unsigned long KAinterval = 150;              // 50ms interval for keep aliv frame
unsigned long ButtonInfoInterval = 30;      // 30ms interval for button info frame
unsigned long KAintervalMillis = 0;         // storage for millis counter
unsigned long ButtonInfoIntervalMillis = 0; // storage for millis counter



void setup() {
  // start serial port an send a message with delay for starting
  Serial.begin(115200);
  Serial.println("Haltech 3x5 keypad ID B emulator");
  delay(50);

  // initialize canbus with 1000kbit and 16mhz xtal
  if (CAN0.begin(MCP_ANY, CAN_1000KBPS, MCP_16MHZ) == CAN_OK)
    Serial.println("MCP2515 CAN0 Initialized Successfully!");
  else Serial.println("Error Initializing MCP2515 CAN0...");

    // initialize canbus with 1000kbit and 16mhz xtal
  if (CAN1.begin(MCP_ANY, CAN_500KBPS, MCP_16MHZ) == CAN_OK)
    Serial.println("MCP2515 CAN1 Initialized Successfully!");
  else Serial.println("Error Initializing MCP2515 CAN1...");

  // Set operation mode to normal so the MCP2515 sends acks to received data.
  CAN0.setMode(MCP_NORMAL);
  CAN1.setMode(MCP_NORMAL);

  pinMode(CAN0_INT, INPUT);      // set INT pin to be an input
  digitalWrite(CAN0_INT, HIGH);  // set INT pin high to enable interna pullup

  pinMode(CAN1_INT, INPUT);      // set INT pin to be an input
  digitalWrite(CAN1_INT, HIGH);  // set INT pin high to enable interna pullup

  pinMode(4, INPUT);
  digitalWrite(4, HIGH);

  KPstart();


  Serial.println("All OK");  // all ready to go !
}


void KPstart() {

  byte KeepAliveblink[2];
  KeepAliveblink[0] = { 0x01 };   
  KeepAliveblink[1] = { 0x15 };                          // frame data is 0X05 for byte 0
  CAN1.sendMsgBuf(0x00, 0, 2, KeepAliveblink);            // send the frame at 70D 
}

void loop() {

  unsigned long currentMillis = millis();  // Get current time in milliseconds

  // Execute keepalive frame every 50 ms
  if (currentMillis - KAintervalMillis >= KAinterval) {
    KAintervalMillis = currentMillis;
    SendKeepAlive();
  }

  // Execute buttoninfo frame every 30 ms
  if (currentMillis - ButtonInfoIntervalMillis >= ButtonInfoInterval) {
    ButtonInfoIntervalMillis = currentMillis;
    SendButtonInfo();
  }

  // read can buffer when interrupted and jump to canread for processing.
  if (!digitalRead(CAN0_INT))  // If CAN0_INT pin is low, read receive buffer
  {
    CAN0.readMsgBuf(&rxId, &len, rxBuf);  // Read data: len = data length, buf = data byte(s)
    canRead(); // execute canRead function to negotiate with ecu
  }
    // read can buffer when interrupted and jump to canread for processing.
  if (!digitalRead(CAN1_INT))  // If CAN0_INT pin is low, read receive buffer
  {
    CAN1.readMsgBuf(&rxId, &len, rxBuf);  // Read data: len = data length, buf = data byte(s)
    canRead1();

  }
}


void canRead1() {
  if (rxId == 0x715 )
  {KPstart();}



  if (rxId == 0x195) 
    {
        button1  = bitRead(rxBuf[0], 0);
        button2  = bitRead(rxBuf[0], 1);
        button3  = bitRead(rxBuf[0], 2);
        button4  = bitRead(rxBuf[0], 3);
        button5  = bitRead(rxBuf[0], 4);
        button6  = bitRead(rxBuf[0], 5);
        button7  = bitRead(rxBuf[0], 6);
        button8  = bitRead(rxBuf[0], 7);
        button9  = bitRead(rxBuf[1], 0);
        button10 = bitRead(rxBuf[1], 1);
        button11 = bitRead(rxBuf[1], 2);
        button12 = bitRead(rxBuf[1], 3);

       digitalWrite(out1, button6);
       digitalWrite(out2, button10);
       digitalWrite(out3, button11);
       digitalWrite(out4, button12);

        byte ledstate[8];

        
        // bitWrite(ledstate[1], 4, !button1);
        // bitWrite(ledstate[1], 5, !button2);
        // bitWrite(ledstate[1], 6, !button3);
        // bitWrite(ledstate[1], 7, !button4);


        // bitWrite(ledstate[2], 0, !button5);
        // bitWrite(ledstate[2], 1, !button6);
        // bitWrite(ledstate[2], 2, !button7);
        // bitWrite(ledstate[2], 3, !button8);
        // bitWrite(ledstate[2], 4, !button9);
        // bitWrite(ledstate[2], 5, !button10);
        // bitWrite(ledstate[2], 6, !button11);
        // bitWrite(ledstate[2], 7, !button12);

        ledstate[0] = 0;
        ledstate[1] = 0;
        ledstate[2] = 0;
        ledstate[3] = 0;
        ledstate[4] = 0;
        ledstate[5] = 0;
        ledstate[6] = 0;
        ledstate[7] = 0;




        CAN1.sendMsgBuf(0x215, 0, 8, ledstate);


    }

}



void canRead() {
  // CAN Input from Haltech Keypad
  int b0;
  int b1;
  int b2;
  int b3;
  int b4;
  int b5;
  int b6;
  int b7;

  // Keypad Configuration Section

  if (rxId == 0X60B) {
    if ((rxBuf[0]) == 34) {
      b0 = 96;
      b1 = (rxBuf[1]);
      b2 = (rxBuf[2]);
      b3 = (rxBuf[3]);
      b4 = 0;
      b5 = 0;
      b6 = 0;
      b7 = 0;

    } else if ((rxBuf[0]) == 66) {
      b0 = 67;
      b1 = (rxBuf[1]);
      b2 = (rxBuf[2]);
      b3 = (rxBuf[3]);
      if ((b1 == 24) && (b2 == 16) && (b3 == 1)) {
        b4 = 7;
        b5 = 4;
        b6 = 0;
        b7 = 0;
      } else if ((b1 == 24) && (b2 == 16) && (b3 == 2)) {
        b4 = 75;
        b5 = 51;
        b6 = 0;
        b7 = 0;
      } else if ((b1 == 24) && (b2 == 16) && (b3 == 3)) {
        b4 = 1;
        b5 = 0;
        b6 = 0;
        b7 = 0;
      } else if ((b1 == 24) && (b2 == 16) && (b3 == 4)) {
        b4 = 207;
        b5 = 184;
        b6 = 25;
        b7 = 12;
      } else if ((b1 == 0) && (b2 == 24) && (b3 == 1)) {
        b4 = 139;
        b5 = 1;
        b6 = 0;
        b7 = 64;
      } else {
        b4 = 0;
        b5 = 0;
        b6 = 0;
        b7 = 0;
      }

    } else if (((rxBuf[0]) == 0) && ((rxBuf[7]) == 200)) {
      b0 = 128;
      b1 = 0;
      b2 = 0;
      b3 = 0;
      b4 = 1;
      b5 = 0;
      b6 = 4;
      b7 = 5;

    }
    byte txBuf[8] = { b0, b1, b2, b3, b4, b5, b6, b7 };

    CAN0.sendMsgBuf(0x58B, 0, 8, txBuf);
  }
}

void SendButtonInfo() {
  byte ButtonInfo[3];                                 // declare an array for 2 bytes used for key pressed information
  bitWrite(ButtonInfo[0], 0, button1);                      // byte 0, bit 0, button 1 
  bitWrite(ButtonInfo[0], 1, button2);                      // byte 0, bit 0, button 2
  bitWrite(ButtonInfo[0], 2, button3);                      // byte 0, bit 0, button 3
  bitWrite(ButtonInfo[0], 3, button4);                      // byte 0, bit 0, button 4
  bitWrite(ButtonInfo[0], 4, button5);                      // byte 0, bit 0, button 5
  bitWrite(ButtonInfo[0], 5, button6);                      // byte 0, bit 0, button 6
  bitWrite(ButtonInfo[0], 6, button7);                      // byte 0, bit 0, button 7
  bitWrite(ButtonInfo[0], 7, button8);                      // byte 0, bit 0, button 8



  ButtonInfo[1] = 0;                                  // byte 2 filled with 0
  CAN0.sendMsgBuf(0x18B, 0, 2, ButtonInfo);           // send the 2 byte data buffer at adres 18D
}


void SendKeepAlive() {                                // send keep alive frame
  byte KeepAlive[1] = { 5 };                          // frame data is 0X05 for byte 0
  CAN0.sendMsgBuf(0x70B, 0, 1, KeepAlive);            // send the frame at 70D 
}
