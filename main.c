#include <lgpio.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "config.h"

// SSD1306 Commands
#define SSD1306_SETCONTRAST 0x81
#define SSD1306_DISPLAYALLON_RESUME 0xA4
#define SSD1306_DISPLAYALLON 0xA5
#define SSD1306_OSCFREQUENCY 0x80
#define SSD1306_NORMALDISPLAY 0xA6
#define SSD1306_INVERTDISPLAY 0xA7
#define SSD1306_DISPLAYOFF 0xAE
#define SSD1306_DISPLAYON 0xAF
#define SSD1306_SETDISPLAYOFFSET 0xD3
#define SSD1306_SETCOMPINS 0xDA
#define SSD1306_SETVCOMDETECT 0xDB
#define SSD1306_SETDISPLAYCLOCKDIV 0xD5
#define SSD1306_SETPRECHARGE 0xD9
#define SSD1306_SETMULTIPLEX 0xA8
#define SSD1306_SETMUXRATIO 0x3F
#define SSD1306_SETLOWCOLUMN 0x00
#define SSD1306_SETHIGHCOLUMN 0x10
#define SSD1306_SETENABLECHARGEBUMP 0x14
#define SSD1306_SETSTARTLINE 0x40
#define SSD1306_MEMORYMODE 0x20
#define SSD1306_COLUMNADDR 0x21
#define SSD1306_PAGEADDR 0x22
#define SSD1306_COMSCANINC 0xC0
#define SSD1306_COMSCANDEC 0xC8
#define SSD1306_SEGREMAP 0xA0
#define SSD1306_CHARGEPUMP 0x8D
#define SSD1306_EXTERNALVCC 0x01
#define SSD1306_SWITCHCAPVCC 0x02

// Scrolling constants
#define SSD1306_ACTIVATE_SCROLL 0x2F
#define SSD1306_DEACTIVATE_SCROLL 0x2E
#define SSD1306_SET_VERTICAL_SCROLL_AREA 0xA3
#define SSD1306_RIGHT_HORIZONTAL_SCROLL 0x26
#define SSD1306_LEFT_HORIZONTAL_SCROLL 0x27
#define SSD1306_VERTICAL_AND_RIGHT_HORIZONTAL_SCROLL 0x29
#define SSD1306_VERTICAL_AND_LEFT_HORIZONTAL_SCROLL 0x2A

#define SSD1306_CMD 0x00
#define SSD1306_RAM 0x40


#define OLED_0in96_WIDTH  64//OLED width
#define OLED_0in96_HEIGHT 128 //OLED height



int GPIO_Handle;
int I2C_Handle;


void send_command(uint8_t cmd){
  lgI2cWriteI2CBlockData(I2C_Handle,SSD1306_CMD,(char *)&cmd,1);
}

void send_data(uint8_t data){
  lgI2cWriteI2CBlockData(I2C_Handle,SSD1306_RAM,(char *)&data,1);
}



int main(){

  int i;

  GPIO_Handle = lgGpiochipOpen(0);    // open /dev/gpiochip0
  if (GPIO_Handle < 0){
    printf("Could not open /dev/gpiochip0\n");
    return GPIO_Handle;
  }


  I2C_Handle = lgI2cOpen(I2C_ID, I2C_DEVICE, 0);      // Open the I2C device
  if (I2C_Handle < 0){
    printf("Could not open I2C Device\n");
    return I2C_Handle;
  }


  // Send commands to SSD1306. Commands are sent into register 0x00
  send_command(SSD1306_DISPLAYOFF);
  send_command(SSD1306_SETDISPLAYCLOCKDIV);
  send_command(SSD1306_OSCFREQUENCY);
  send_command(SSD1306_SETMULTIPLEX);
  send_command(SSD1306_SETMUXRATIO);
  send_command(SSD1306_SETDISPLAYOFFSET);
  send_command(0x0);
  send_command((SSD1306_SETSTARTLINE | 0x0));
  send_command(SSD1306_CHARGEPUMP);
  send_command(SSD1306_SETENABLECHARGEBUMP);
  send_command(SSD1306_MEMORYMODE);
  send_command(0x0);
  send_command((SSD1306_SEGREMAP | 0x1));
  send_command(SSD1306_COMSCANDEC);
  send_command(SSD1306_SETCOMPINS);
  send_command(0x12);
  send_command(SSD1306_SETCONTRAST);
  send_command(0xCF);
  send_command(SSD1306_SETPRECHARGE);
  send_command(0xF1);
  send_command(SSD1306_SETVCOMDETECT);
  send_command(0x40);
  send_command(SSD1306_DISPLAYALLON_RESUME);
  send_command(SSD1306_NORMALDISPLAY);



  // 200ms delay, then turn on display
  usleep(200 * 1000);
  send_command(0xaf);


  // Clear screen
  send_command(SSD1306_COLUMNADDR);
  send_command(0x0);
  send_command((OLED_0in96_HEIGHT -1));
  send_command(SSD1306_PAGEADDR);
  send_command(0x0);
  send_command((OLED_0in96_WIDTH/8 -1));

  // Clean screen
  for(i=0; i<1024; i++){
    send_data(0x00);
  }


  //Write blank screen
  for(i=0; i<1024; i++){
    send_data(0xFF);
  }

  //Write random pixel
  while(1){
    for(i=0; i<1024; i++){
      send_data(rand() % 255);
    }
  }




  // Close I2C device and GPIO
  lgI2cClose(I2C_Handle);
  lgGpiochipClose(GPIO_Handle);

  return 0;
}
