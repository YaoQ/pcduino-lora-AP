/*
*  Copyright (C) 2012 Libelium Comunicaciones Distribuidas S.L.
*  http://www.libelium.com
*
*  This program is free software: you can redistribute it and/or modify
*  it under the terms of the GNU General Public License as published by
*  the Free Software Foundation, either version 3 of the License, or
*  (at your option) any later version.
*
*  This program is distributed in the hope that it will be useful,
*  but WITHOUT ANY WARRANTY; without even the implied warranty of
*  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
*  GNU General Public License for more details.
*
*  You should have received a copy of the GNU General Public License
*  along with this program.  If not, see <http://www.gnu.org/licenses/>.
*
*  Version 1.5 (For Raspberry Pi Rev2)
*  Author: Anartz Nuin Jiménez
* 
*  Version 1.6 (For pcDuino8 Uno)
*  Author: Anartz Nuin Jiménez
*/
#ifndef pcduino_h 
#define pcduino_h

#include <errno.h>
#include <fcntl.h>
#include <netinet/in.h>
#include <signal.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/socket.h>
#include <sys/time.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/mman.h>
#include <string.h>
#include <time.h>
#include <termios.h> 
#include <ctype.h>
#include <sys/ioctl.h>
#include <limits.h>
#include <algorithm>
#include <limits.h>
#include <pthread.h>
#include <poll.h>

#define PAGESIZE 4096
#define BLOCK_SIZE 4096

/// Defines for SPI
#define CS		10
#define MOSI	11
#define MISO	12
#define SCK		13


static int REV = 0;

namespace unistd {
	//All functions of unistd.h must be called like this: unistd::the_function()
    #include <unistd.h>
}


enum Representation{
	BIN,
	OCT,
	DEC,
	HEX,
	BYTE
};

typedef enum {
	INPUT,
	OUTPUT
}Pinmode;

typedef enum {
	LOW = 0,
	HIGH = 1,
	RISING = 2,
	FALLING = 3,
	BOTH = 4
}Digivalue;

typedef bool boolean;
typedef unsigned char byte;

struct ThreadArg{
    void (*func)();
    int pin;
};



///* Some useful arduino functions */
void delay(long millis);
void delayMicroseconds(long micros);
long millis();
#endif
