/*

  HelloWorld.pde
  
  "Hello World!" example code.
  
  >>> Before compiling: Please remove comment from the constructor of the 
  >>> connected graphics display (see below).
  
  Universal 8bit Graphics Library, http://code.google.com/p/u8glib/
  
  Copyright (c) 2012, olikraus@gmail.com
  All rights reserved.

  Redistribution and use in source and binary forms, with or without modification, 
  are permitted provided that the following conditions are met:

  * Redistributions of source code must retain the above copyright notice, this list 
    of conditions and the following disclaimer.
    
  * Redistributions in binary form must reproduce the above copyright notice, this 
    list of conditions and the following disclaimer in the documentation and/or other 
    materials provided with the distribution.

  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND 
  CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, 
  INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF 
  MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE 
  DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR 
  CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, 
  SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT 
  NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; 
  LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER 
  CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, 
  STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) 
  ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF 
  ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.  
  
*/

// This documen is modified to work with the Pax Instruments LCD Test Board v0.2
#define LCD_A0               9
#define LCD_BACKLIGHT_PIN    6    // LCD backlight on pin
#define LCD_CS               7
#define LCD_RST              8
// MISO                     14
// SCK                      15
// MOSI                     16
#define SW_A_pin             4
#define SW_B_pin             5
#define SW_C_pin             A3
#define SW_D_pin             A2
#define SW_E_pin             A1
#define SW_F_pin             A0
#define SD_CS                10

#define LCD_CONTRAST     0x018*7  // Sets the LCD contrast

#define DHTPIN A4     // what pin we're connected to
#define DHTTYPE DHT11   // DHT 11 

#include "U8glib.h"
#include <SdFat.h>
#include "DHT.h"

U8GLIB_PI13264  u8g(LCD_CS, LCD_A0, LCD_RST); // Use HW-SPI

DHT dht(DHTPIN, DHTTYPE);

SdFat sd;
SdFile myFile;
int sd_test_status = 0;

int SW_A_state = 0;
int SW_B_state = 0;
int SW_C_state = 0;
int SW_D_state = 0;
int SW_E_state = 0;
int SW_F_state = 0;

int fadeValue = 0;
int updown = 1; // is the fading going up or down

int sdTest(){
//  Serial.begin(9600);
//  while (!Serial) {}  // wait for Leonardo
//  Serial.println("Type any character to start");
//  while (Serial.read() <= 0) {}
//  delay(400);  // catch Due reset problem
 // if (sd.begin(SD_CS, SPI_HALF_SPEED)) sd.initErrorHalt();
  sd.begin(SD_CS, SPI_HALF_SPEED);
  if (!myFile.open("testFile.txt", O_RDWR | O_CREAT | O_AT_END)) {
    //sd.errorHalt("opening test.txt for write failed");
    return 0;
  }
//  Serial.print("Writing to test.txt...");
  myFile.println("testing 1, 2, 3.");
  myFile.close();
//  Serial.println("done.");
  if (!myFile.open("test.txt", O_READ)) {
   // sd.errorHalt("opening test.txt for read failed");
    return 0;
  }
//  Serial.println("test.txt:");
  int data;
  while ((data = myFile.read()) >= 0) Serial.write(data);
  // close the file:
  myFile.close();
  return 1;
}

void draw(void) {
  // graphic commands to redraw the complete screen should be placed here  
  //u8g.setFont(u8g_font_unifont);
  u8g.setFont(u8g_font_ncenR12);
  u8g.drawStr( 0, 14, "PAX");
  u8g.drawStr( 0, 30, "INSTRUMENTS");
  
  u8g.setFont(u8g_font_5x8);
  u8g.drawStr( 0, 39, "OPENSOURCE LAB INSTRUMENTS");
  u8g.drawStr( 0, 62, "Graphic LCD 132x64 shield");
  u8g.drawStr( 40, 8, "    Temp: ");
  u8g.setPrintPos(90, 8);
  u8g.print(dht.readTemperature(), 0);
  u8g.drawStr( 100, 8, " C");
  u8g.drawStr( 40, 16, "Humidity: ");
  u8g.setPrintPos(90, 16);
  u8g.print(dht.readHumidity(), 0);
  u8g.drawStr( 100, 16, " %");
  
  
  // Draw button stuff
  if (SW_A_state == LOW) {     
    u8g.drawStr( 0, 50, "A");
  } 
  if (SW_B_state == LOW) {     
    u8g.drawStr( 5, 50, "B");
  } 
  if (SW_C_state == LOW) {     
    u8g.drawStr( 10, 50, "C");
  } 
  if (SW_D_state == LOW) {     
    u8g.drawStr( 15, 50, "D");
    if (SW_C_state == LOW){
      if (sd_test_status == 0){
        u8g.drawStr( 75, 50, "SD: bad");
      }
      if (sd_test_status == 1){
        u8g.drawStr( 75, 50, "SD: okay");
      }
    }
  } 
  if (SW_E_state == LOW) {     
    u8g.drawStr( 20, 50, "E");
  } 
  if (SW_F_state == LOW) {     
    u8g.drawStr( 25, 50, "Fading...");
  } 
}

void setup(void) {
  sd_test_status = sdTest();
    // flip screen, if required
  u8g.setRot180();
  u8g.setContrast(LCD_CONTRAST);
  
  pinMode(LCD_BACKLIGHT_PIN, OUTPUT);           
  analogWrite(LCD_BACKLIGHT_PIN, fadeValue);   

  // assign default color value
  if ( u8g.getMode() == U8G_MODE_R3G3B2 ) {
    u8g.setColorIndex(255);     // white
  }
  else if ( u8g.getMode() == U8G_MODE_GRAY2BIT ) {
    u8g.setColorIndex(3);         // max intensity
  }
  else if ( u8g.getMode() == U8G_MODE_BW ) {
    u8g.setColorIndex(1);         // pixel on
  }
  else if ( u8g.getMode() == U8G_MODE_HICOLOR ) {
    u8g.setHiColorByRGB(255,255,255);
  }
  
  // Set up the buttons
    pinMode(SW_A_pin, INPUT);
    pinMode(SW_B_pin, INPUT);
    pinMode(SW_C_pin, INPUT);
    pinMode(SW_D_pin, INPUT);
    pinMode(SW_E_pin, INPUT);
    pinMode(SW_F_pin, INPUT);
    
    dht.begin();
}



void loop(void) {
  SW_A_state = digitalRead(SW_A_pin);
  SW_B_state = digitalRead(SW_B_pin);
  SW_C_state = digitalRead(SW_C_pin);
  SW_D_state = digitalRead(SW_D_pin);
  SW_E_state = digitalRead(SW_E_pin);
  SW_F_state = digitalRead(SW_F_pin);
  
  // fade in from min to max in increments of 5 points:

  if (SW_D_state == false && SW_C_state == false){
   sd_test_status = sdTest(); 
  }
  
  // picture loop
  u8g.firstPage();  
  do {
    draw();
  }
  while( u8g.nextPage() );
  // rebuild the picture after some delay
  delay(50);
  while(SW_F_state == false){
    if (fadeValue >=255){
      updown = 0;
    }
    if (fadeValue <= 0 ){
      updown = 1;
    }
    
    analogWrite(LCD_BACKLIGHT_PIN, fadeValue);         
    delay(3);
    if (updown == true){
      fadeValue +=1;
    }
    if (updown == false){
      fadeValue -=1;
    }
    SW_F_state = digitalRead(SW_F_pin);
  }
}

