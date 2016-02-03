#include <stdio.h>
#include <stdlib.h>
#include "littleWire.h"
#include "littleWire_util.h"

int main()
{
	unsigned char i=0;
	unsigned char iterator = 0;
	unsigned char version;
	unsigned char myBuffer[8];
	littleWire *lw = NULL;

	lw = littleWire_connect();

	if(lw == NULL){
		printf("> Little Wire could not be found!\n");
		exit(EXIT_FAILURE);
	}

	version = readFirmwareVersion(lw);
	printf("> Little Wire firmware version: %d.%d\n",((version & 0xF0)>>4),(version&0x0F));
	if(version<=0x12)
	{
		printf("> This example requires the new 1.3 version firmware. Please update soon.\n");
		return 0;
	}

	i2c_init(lw);
	i2c_updateDelay(lw,5);

	/*printf("> Address search ... \n");
	for(i=0;i<128;i++)
	{
		rc = i2c_start(lw,i,WRITE);
		if(rc==1)
			printf("> Found device at %3d\n",i);
	}
	printf("> Adress search end ");
	//delay(2000);
	 */

	i = i2c_start(lw, 0x15 ,WRITE);
	myBuffer[0] = 25;
	myBuffer[1] = 0xAA;
	myBuffer[2] = 0x55;
	myBuffer[3] = 0xAA;
	i2c_write(lw,myBuffer,4,END_WITH_STOP);
	printf("> write end\n");


	iterator = 10;
	while (iterator)
	{
		if (i2c_start(lw,0x15,WRITE) == 1)
		{
			break;
		}
		iterator--;
	}
	myBuffer[0] = 23;
	myBuffer[1] = 0xAA;
	myBuffer[2] = 0x55;
	i2c_write(lw,myBuffer,3,END_WITH_STOP);
	printf("> write end\n");

	iterator = 10;
	while (iterator)
	{
		if (i2c_start(lw,0x15,WRITE) == 1)
		{
			break;
		}
		iterator--;
	}
	myBuffer[0] = 20;
	i2c_write(lw,myBuffer,1,END_WITH_STOP);
	printf("> write 2 end\n");

	while (iterator)
	{
		if (i2c_start(lw,0x15,READ) == 1)
		{
			break;
		}
		iterator--;
	}
	/* Get the standard response */
	for (i = 0; i < 10; i++)
	{
		i2c_read(lw,myBuffer,1,NO_STOP);
		printf("> read 0x%X\n", myBuffer[0]);
	}
	i2c_read(lw,myBuffer,1,END_WITH_STOP);

	return 0;
}
