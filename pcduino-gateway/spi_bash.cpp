//#include <iostream>
#include <string.h>
#include <stdint.h>		/* C99 types */
#include <stdio.h>		/* printf fprintf */
#include <stdlib.h>		/* malloc free */
#include <unistd.h>		/* lseek, close */
#include <fcntl.h>		

#include <sys/ioctl.h>
#include <linux/spi/spidev.h>
#include  "spi_bash.h"


//static unsigned char  cs_pin = 10;
//static unsigned char  mosi_pin = 11;
//static unsigned char  miso_pin = 12;
//static unsigned char  clk_pin = 13;
//static unsigned char  reset_pin = 8;

int gpio_pin_fd[MAX_GPIO_NUM+1];
int gpio_mode_fd[MAX_GPIO_NUM+1];

int spi_bash::write_to_file(int fd,char *str,int len)
{
    int ret = -1;

    lseek(fd, 0, SEEK_SET);
    ret = write(fd, str, len);
    if ( ret <= 0 )
    {
        printf("write %d failed\n", fd);
        return -1;
    }
    return ret;
}
int spi_bash::gp_digitalRead(unsigned char pin)
{
//    char path[128];
    char buf[4];    
    int ret = -1;
//    int fd = -1;     
        
        memset((void *)buf, 0, sizeof(buf));
        lseek(gpio_pin_fd[pin], 0, SEEK_SET);
        ret = read(gpio_pin_fd[pin], buf, sizeof(buf));

        if ( ret <= 0 )
        {
            fprintf(stderr, "read gpio %d failed\n", pin);
            exit(-1);
        }
        
        ret = buf[0] - '0';
        switch( ret )
        {
            case LOW:
            case HIGH:
                break;
            default:
                ret = -1;
                break;
        }

    
    return ret;

}
int spi_bash::gp_digitalWrite(unsigned char pin, unsigned char value)
{
    char buf[4]; 
     int ret = -1;
	 
	memset((void *)buf, 0, sizeof(buf));
	sprintf(buf, "%d", value);
	 ret = write_to_file(gpio_pin_fd[pin], buf, sizeof(buf));
	if ( ret <= 0 )
	{
	  fprintf(stderr, "write gpio %d  failed\n", pin);
	  exit(-1);
	}
	
	   return 0;
}
int spi_bash::config_pin_mode(unsigned char pin,unsigned char mode)
{
//	int fd;
	char buf[4];
	int ret = -1;

	memset((void *)buf, 0, sizeof(buf));
	sprintf(buf, "%d", mode);
	ret = write_to_file(gpio_mode_fd[pin], buf, sizeof(buf));
	if ( ret <= 0 )
	{
		fprintf(stderr, "write gpio %d mode failed\n", pin);
		exit(-1);
	}
	return 0;
}
int spi_bash::init_spi_config(void)
{
	int i;
	char path[1024];

       for(i = 10; i< 14; i++){
              // open pin dir
		memset(path, 0, sizeof(path));
		sprintf(path, "%s%s%d", GPIO_PIN_DIR, GPIO_IF_PREFIX, i);
//		printf("open pin %s \n",path);
	       gpio_pin_fd[i] = open(path, O_RDWR);
	         if ( gpio_pin_fd[i] < 0 )
	         {
	             printf("open pin  000 = %s failed = %d \n", path,gpio_pin_fd[i]);
	             return -1;
	         }

	       //open mode dir
		memset(path, 0, sizeof(path));
		sprintf(path, "%s%s%d", GPIO_MODE_DIR, GPIO_IF_PREFIX, i);
//		printf("open mode %s \n",path);
		gpio_mode_fd[i] = open(path, O_RDWR);
		if(gpio_mode_fd[i]<0){
 			printf("open mode 000 = %s failed\n", path);
			return -1;
		}
		
       }
       
       // reset pin
	memset(path, 0, sizeof(path));
	sprintf(path, "%s%s%d", GPIO_PIN_DIR, GPIO_IF_PREFIX, reset_pin);
//	printf("cs open pin %s \n",path);
	gpio_pin_fd[reset_pin] = open(path, O_RDWR);
      if ( gpio_pin_fd[reset_pin] < 0 )
      {
         printf("open pin  000 = %s failed = %d \n", path,gpio_pin_fd[i]);
         return -1;
      }
	//open mode dir
	memset(path, 0, sizeof(path));
	sprintf(path, "%s%s%d", GPIO_MODE_DIR, GPIO_IF_PREFIX, reset_pin);
//	printf("cs open mode %s \n",path);
	gpio_mode_fd[reset_pin] = open(path, O_RDWR);
	if(gpio_mode_fd[reset_pin]<0){
 			printf("open mode 000 = %s failed\n", path);
			return -1;
		}
	
       // config mode
	config_pin_mode(cs_pin,OUTPUT);
	config_pin_mode(mosi_pin,OUTPUT);
	config_pin_mode(clk_pin,OUTPUT);
	config_pin_mode(miso_pin,INPUT);
	
        config_pin_mode(reset_pin,OUTPUT);
	gp_digitalWrite(reset_pin,LOW);
	usleep(10);
	gp_digitalWrite(reset_pin,HIGH);
	usleep(100);
	gp_digitalWrite(reset_pin,LOW);
	usleep(100);

	return 0;
	
}
void delay_s(unsigned int  us)
{  
     usleep(us);
}
void spi_bash::Spi_Write(unsigned char data)
{
	unsigned char k;
//	SPIx_SCLK_LOW;
	for (k=0;k<8;k++)
	{        SPIx_SCLK_LOW;
		 if((data&0x80)==0X80)
	             SPIx_DOUT_HIGH;
	         else
		     SPIx_DOUT_LOW;
		  data <<=1;
		  SPIx_SCLK_HIGH;
	 }
	SPIx_SCLK_LOW;
	SPIx_DOUT_LOW;
}
unsigned char spi_bash::Spi_Read(void)
{
	unsigned char k;
	unsigned char data=0;

	for (k=0;k<8;k++)
	{   SPIx_SCLK_HIGH;
	     data <<=1;
	     if (gp_digitalRead(miso_pin))
		    data |=0x01;  
	     SPIx_SCLK_LOW;

	}
	return data;
}
int  spi_bash::read_buf_spi(unsigned char addr,unsigned int num,unsigned char *ptr)
{      unsigned int i;
	//write addr
	SPIx_CS_LOW;
	bitClear(addr, 7);
	Spi_Write(addr);
	for(i=0;i<num;i++){
           *ptr ++ = Spi_Read();
	}
	SPIx_CS_HIGH;
	return 0;
}
int spi_bash::write_buf_spi(unsigned char addr,unsigned int num,unsigned char *ptr)
{
        unsigned int i;
        unsigned char wr_dat;
	wr_dat = *ptr++;
	//write addr
	SPIx_CS_LOW;
	bitSet(addr, 7);
	Spi_Write(addr);
//	usleep(1);
	for(i=0;i<num;i++){
            Spi_Write(wr_dat);
	    wr_dat =  *ptr ++;
	}
	SPIx_CS_HIGH;

	return 0;
}


#define        REG_VERSION	  					0x42

#if   0
int main()
{
  spi_bash   spi_lora;

  spi_lora.init_spi_config();
//   spi_lora.gp_digitalWrite(reset_pin,HIGH);
//   spi_lora.gp_digitalWrite(10,LOW);

	unsigned char  version =0;
	spi_lora.read_buf_spi(REG_VERSION,1,&version);
	printf("version = %x \n",version);
	while(1){


	}
}
#endif 

