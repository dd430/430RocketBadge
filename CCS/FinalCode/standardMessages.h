/*
 * standardMessages.h
 *
 *  Created on: Mar 18, 2014
 *      Author: Christopher
 *
 *      This file sets the location for these constants in memory -
 *      the same location we write to if we have a new message.
 *      This allows us to flash new messages on the device.
 */

#ifndef STANDARDMESSAGES_H_
#define STANDARDMESSAGES_H_


#define START_EE	0xEBE0
#define START0	0xEFE0
#define START1	0xF3E0
#define START2	0xF7E0
#define STARTTMP	0xFBE0

#pragma RETAIN(Message1)
#pragma location=START0  //43oh.com rocks!
const char Message1[1024]=
			{0x00,0x50,
			 0x07,0x04,0x04,0x1f,0x00,0x11,0x15,0x15,
			 0x1f,0x00,0x1c,0x14,0x1c,0x00,0x1f,0x04,
			 0x1c,0x00,0x18,0x18,0x00,0x1f,0x11,0x11,
			 0x11,0x00,0x1f,0x11,0x11,0x1f,0x00,0x1f,
			 0x02,0x04,0x02,0x1f,0x00,0x00,0x00,0x1f,
			 0x05,0x0d,0x17,0x00,0x1f,0x11,0x11,0x1f,
			 0x00,0x1f,0x11,0x11,0x11,0x00,0x1f,0x04,
			 0x0a,0x11,0x00,0x17,0x15,0x15,0x1d,0x00,
			 0x00,0x17,0x00,0x00,0x17,0x00,0x00,0x17,
			 0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00};


#pragma RETAIN(Message2)
#pragma location=START1
const char Message2[1024]= //MSP430 - WHAT HAVE YOU LAUNCHED LATELY ??
		{0x00,0xbb,
		 0x1f,0x02,0x04,0x02,0x1f,0x00,0x17,0x15,0x15,
		 0x1d,0x00,0x1f,0x05,0x05,0x07,0x00,0x07,0x04,
		 0x04,0x1f,0x00,0x11,0x15,0x15,0x1f,0x00,0x1f,
		 0x11,0x11,0x1f,0x00,0x00,0x04,0x04,0x04,0x04,
		 0x00,0x00,0x1f,0x08,0x04,0x08,0x1f,0x00,0x1f,
		 0x04,0x04,0x1f,0x00,0x1f,0x05,0x05,0x1f,0x00,
		 0x01,0x01,0x1f,0x01,0x01,0x00,0x00,0x1f,0x04,
		 0x04,0x1f,0x00,0x1f,0x05,0x05,0x1f,0x00,0x03,
		 0x0c,0x10,0x0c,0x03,0x00,0x1f,0x15,0x15,0x11,
		 0x00,0x00,0x01,0x02,0x1c,0x02,0x01,0x00,0x1f,
		 0x11,0x11,0x1f,0x00,0x1f,0x10,0x10,0x1f,0x00,
		 0x00,0x1f,0x10,0x10,0x10,0x00,0x1f,0x05,0x05,
		 0x1f,0x00,0x1f,0x10,0x10,0x1f,0x00,0x1f,0x02,
		 0x04,0x08,0x1f,0x00,0x1f,0x11,0x11,0x11,0x00,
		 0x1f,0x04,0x04,0x1f,0x00,0x1f,0x15,0x15,0x11,
		 0x00,0x1f,0x11,0x11,0x0e,0x00,0x00,0x1f,0x10,
		 0x10,0x10,0x00,0x1f,0x05,0x05,0x1f,0x00,0x01,
		 0x01,0x1f,0x01,0x01,0x00,0x1f,0x15,0x15,0x00,
		 0x1f,0x10,0x10,0x10,0x01,0x02,0x1c,0x02,0x01,
		 0x00,0x00,0x01,0x15,0x05,0x06,0x00,0x00,0x01,
		 0x15,0x05,0x06,0x00,0x00,0x00,0x00};


/*
 * //MSP430 - LOW POWER - CAP TOUCH
{0x00,0x8a,
		 0x00,0x00,0x00,0x1f,0x02,0x04,0x02,0x1f,0x00,
		 0x17,0x15,0x15,0x1d,0x00,0x1f,0x05,0x05,0x07,
		 0x00,0x07,0x04,0x04,0x1f,0x00,0x11,0x15,0x15,
		 0x1f,0x00,0x1f,0x11,0x11,0x1f,0x00,0x00,0x04,
		 0x04,0x04,0x04,0x00,0x00,0x1f,0x10,0x10,0x10,
		 0x00,0x1f,0x11,0x11,0x1f,0x00,0x1f,0x08,0x04,
		 0x08,0x1f,0x00,0x00,0x00,0x1f,0x05,0x05,0x07,
		 0x00,0x1f,0x11,0x11,0x1f,0x00,0x1f,0x08,0x04,
		 0x08,0x1f,0x00,0x1f,0x15,0x15,0x11,0x00,0x1f,
		 0x05,0x0d,0x17,0x00,0x04,0x04,0x04,0x04,0x00,
		 0x1f,0x11,0x11,0x11,0x00,0x1f,0x05,0x05,0x1f,
		 0x00,0x1f,0x05,0x05,0x07,0x00,0x00,0x00,0x01,
		 0x01,0x1f,0x01,0x01,0x00,0x1f,0x11,0x11,0x1f,
		 0x00,0x1f,0x10,0x10,0x1f,0x00,0x1f,0x11,0x11,
		 0x11,0x00,0x1f,0x04,0x04,0x1f,0x00,0x00,0x00,
		 0x00,0x00,0x00};*/

#pragma RETAIN(Message3)
#pragma location=START2
const char Message3[1024]= //ENERGIA - WIRING FOR MSP430
		{0x00,0x7e,
		 0x1f,0x15,0x15,0x11,0x00,0x1f,0x02,0x04,0x1f,
		 0x00,0x1f,0x15,0x15,0x11,0x00,0x1f,0x05,0x0d,
		 0x17,0x00,0x1f,0x11,0x15,0x1d,0x00,0x11,0x11,
		 0x1f,0x11,0x11,0x00,0x1f,0x05,0x05,0x1f,0x00,
		 0x04,0x04,0x04,0x04,0x00,0x00,0x1f,0x08,0x04,
		 0x08,0x1f,0x00,0x11,0x11,0x1f,0x11,0x11,0x00,
		 0x1f,0x05,0x0d,0x17,0x00,0x11,0x11,0x1f,0x11,
		 0x11,0x00,0x1f,0x02,0x04,0x1f,0x00,0x1f,0x11,
		 0x15,0x1d,0x00,0x00,0x1f,0x05,0x05,0x01,0x00,
		 0x1f,0x11,0x11,0x1f,0x00,0x1f,0x05,0x0d,0x17,
		 0x00,0x00,0x1f,0x02,0x04,0x02,0x1f,0x00,0x17,
		 0x15,0x15,0x1d,0x00,0x1f,0x05,0x05,0x07,0x00,
		 0x07,0x04,0x04,0x1f,0x00,0x11,0x15,0x15,0x1f,
		 0x00,0x1f,0x11,0x11,0x1f,0x00,0x00,0x00,0x00};

#pragma RETAIN(MessageEE)
#pragma location=START_EE
const char MessageEE[1024]=
		{0x00,0xa0,
		 0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,
		 0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,
		 0x01,0x01,0x02,0x03,0x02,0x03,0x02,0x03,0x02,
		 0x03,0x02,0x03,0x02,0x02,0x02,0x02,0x02,0x02,
		 0x02,0x02,0x02,0x02,0x04,0x06,0x04,0x06,0x04,
		 0x06,0x04,0x06,0x04,0x06,0x04,0x04,0x04,0x04,
		 0x04,0x04,0x04,0x04,0x04,0x04,0x08,0x0c,0x08,
		 0x0c,0x08,0x0c,0x08,0x0c,0x08,0x0c,0x08,0x08,
		 0x08,0x08,0x08,0x08,0x08,0x08,0x08,0x08,0x10,
		 0x18,0x10,0x18,0x10,0x18,0x10,0x18,0x10,0x18,
		 0x10,0x10,0x10,0x10,0x10,0x10,0x10,0x10,0x10,
		 0x10,0x08,0x18,0x08,0x18,0x08,0x18,0x08,0x18,
		 0x08,0x18,0x08,0x08,0x08,0x08,0x08,0x08,0x08,
		 0x08,0x08,0x08,0x04,0x0c,0x04,0x0c,0x04,0x0c,
		 0x04,0x0c,0x04,0x0c,0x04,0x04,0x04,0x04,0x04,
		 0x04,0x04,0x04,0x04,0x04,0x02,0x06,0x02,0x06,
		 0x02,0x06,0x02,0x06,0x02,0x06,0x02,0x02,0x02,
		 0x02,0x02,0x02,0x02,0x02,0x02,0x02};

#endif /* STANDARDMESSAGES_H_ */
