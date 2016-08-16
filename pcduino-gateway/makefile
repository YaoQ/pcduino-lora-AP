#CFLAGS=-DRADIO_RFM92_95
CFLAGS=-DRADIO_INAIR9B
#CFLAGS=-DRADIO_20DBM



pcduino-gateway: lora_gateway.o pcduino.o  SX1272.o spi_bash.o
	g++ -o $@ $^    -lrt -lpthread

%.o : %.cpp
	g++ -c -o $@ $<   -lrt -lpthread

clean:
	rm -f *.o  pcduino-gateway	
