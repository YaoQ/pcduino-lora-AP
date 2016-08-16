#ifndef SPI_BASH_H
#define SPI_BASH_H

#include <stdint.h>

#ifdef RASPBERRY2
//#include "arduPi_pi2.h"
#else
//#include "arduPi.h"
#endif

#ifndef inttypes_h
//	#include <inttypes.h>
#endif

#define   cs_pin   10
#define   mosi_pin  11
#define   miso_pin  12
#define   clk_pin     13
#define   reset_pin  8

#undef MAX_GPIO_NUM
#define MAX_GPIO_NUM 23

#undef MAX_GPIO_MODE_NUM
#define MAX_GPIO_MODE_NUM 8

#define INPUT 0x0
#define OUTPUT 0x1

#define LOW  0x0
#define HIGH 0x1

#define GPIO_MODE_DIR  "/sys/devices/virtual/misc/gpio/mode/"
#define GPIO_PIN_DIR     "/sys/devices/virtual/misc/gpio/pin/"
#define GPIO_IF_PREFIX  "gpio"

#define   SPIx_CS_LOW          gp_digitalWrite(cs_pin, LOW)
#define   SPIx_CS_HIGH         gp_digitalWrite(cs_pin, HIGH)
#define   SPIx_SCLK_LOW       gp_digitalWrite(clk_pin, LOW)
#define   SPIx_SCLK_HIGH      gp_digitalWrite(clk_pin, HIGH)

#define   SPIx_DOUT_LOW      gp_digitalWrite(mosi_pin, LOW)
#define   SPIx_DOUT_HIGH     gp_digitalWrite(mosi_pin, HIGH)


//#define READ_ACCESS		0x00
//#define WRITE_ACCESS	0x80

//! MACROS //
#define bitRead(value, bit) (((value) >> (bit)) & 0x01)  // read a bit
#define bitSet(value, bit) ((value) |= (1UL << (bit)))    // set bit to '1'
#define bitClear(value, bit) ((value) &= ~(1UL << (bit))) // set bit to '0'

//namespace  spi_some{
class spi_bash{
	private:
		int write_to_file(int fd,char *str,int len);
		int config_pin_mode(unsigned char pin,unsigned char mode);
		void delay_s(unsigned int  us);
		unsigned char Spi_Read(void);
		void Spi_Write(unsigned char data);
	public:
		int init_spi_config(void);
		int gp_digitalWrite(unsigned char pin, unsigned char value);
		int gp_digitalRead(unsigned char pin);
		int  read_buf_spi(unsigned char addr,unsigned int num,unsigned char *ptr);
		int write_buf_spi(unsigned char addr,unsigned int num,unsigned char *ptr);
};

//}

#endif
