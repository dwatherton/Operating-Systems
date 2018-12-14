/*
THIS COPYRIGHT HEADER IS INCLUDED TO INDICATE THAT A PORTION OF THIS CODE WAS PROVIDED AS PART OF A COURSE THAT I TOOK AT UNIVERSITY. THE ORIGINAL CODE IS PART OF THE NACHOS OS FROM THE UNIVERSITY OF CALIFORNIA, I SIMPLY MADE MODIFICATIONS TO THE PROVIDED CODE TO IMPLEMENT THE BOUNDED-BUFFER PROBLEM. I DO NOT CLAIM TO BE THE INITIAL AUTHOR OF THIS CODE. -Daniel Atherton

Copyright (c) 1992-1993 The Regents of the University of California.
All rights reserved.

Permission to use, copy, modify, and distribute this software and its documentation for any purpose, without fee, and without written agreement is hereby granted, provided that the above copyright notice and the following two paragraphs appear in all copies of this software.

IN NO EVENT SHALL THE UNIVERSITY OF CALIFORNIA BE LIABLE TO ANY PARTY FOR DIRECT, INDIRECT, SPEICAL, INCIDENTAL, OR CONSEQUENTIAL DAMAGES ARISING OUT OF THE USE OF THIS SOFTWARE AND ITS DOCUMENTATION, EVEN IF THE UNIVERSITY OF CALIFORNIA HAS BEEN ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

THE UNIVERSITY OF CALIFORNIA SPECIFICALLY DISCLAIMS ANY WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE. THE SOFTWARE PROVIDED HEREUNDER IS ON AN "AS IS" BASIS, AND THE UNIVERSITY OF CALIFORNIA HAS NO OBLIGATION TO PROVIDE MAINTENANCE, SUPPORT, UPDATES, ENHANCEMENTS, OR MODIFICATIONS.
*/

#include <stdio.h>
#include <fstream>
#include "Synch.h"
#include "Ring.h"

#define BUFF_SIZE 2  // the size of the round buffer
#define N_PROD    2  // the number of producers
#define N_CONS    2  // the number of consumers
#define N_MESSG   3  // the number of messages produced by each producer
#define MAX_NAME  16 // the maximum lengh of a name

#define MAXLEN  48
#define LINELEN 24

Thread *producers[N_PROD]; //array of pointers to the producer
Thread *consumers[N_CONS];  //and consumer threads;

char prod_names[N_PROD][MAX_NAME];  //array of charater string for prod names
char cons_names[N_CONS][MAX_NAME];  //array of charater string for cons names

Ring *ring;  //the ring buffer for producer/consumer problem

//----------------------------------------------------------------------
// Producer
//      Loop N_MESSG times, produce a message and put it in the
//      shared ring buffer each time.
//      "which" is simply a number identifying the producer thread.
//
//----------------------------------------------------------------------

void Producer(int which)
{
	int num;
	Slot *message = new Slot(0, 0);

	//  This loop is to generate N_MESSG messages to put into the ring buffer
	//   by calling ring->Put(message). Each message carries a message id
	//   which is represened by integer "num". It also carries the id
	//   of the producer thread in the "thread_id" field so that
	//   consumer threads can know which producer generates the message.

	for (num = 0; num < N_MESSG; num++)
	{
		message->thread_id = which;
		message->value = num;

		ring->Put(message);

		currentThread->Yield();
	}
}

//----------------------------------------------------------------------
// Consumer
//      endless loop to fetch messages from the ring buffer and
//      record these message in the corresponding file.
//
//----------------------------------------------------------------------

void Consumer(int which)
{
	char str[MAXLEN];
	char fname[LINELEN];
	std::ofstream fout;

	Slot *message = new Slot(0, 0);

	// create the file name based on the consumer id
	sprintf_s(fname, "tmp_%d", which);

	// create/open the file to store output
	fout.open(fname);

	for (; ; ) {

		ring->Get(message);

		// form a string to record the message
		sprintf_s(str, "producer id --> %d; Message number --> %d;\n",
			message->thread_id,
			message->value);

		// write the string to the file
		fout << str;

		currentThread->Yield();
	}
}

//----------------------------------------------------------------------
// ProdCons
//      Set up the bounded buffer ring, and create and fork the
//      producer and consumer threads with respective function args
//----------------------------------------------------------------------

void ProdCons()
{
	int i;

	// create the bounded buffer ring
	ring = new Ring(BUFF_SIZE);

	// create and fork N_PROD of producer threads
	for (i = 0; i < N_PROD; i++)
	{
		sprintf_s(prod_names[i], "producer_%d", i);
		producers[i] = new Thread(prod_names[i]);
		producers[i]->Fork(Producer, i);
	};

	// create and fork N_CONS of consumer threads
	for (i = 0; i < N_CONS; i++)
	{
		sprintf_s(cons_names[i], "consumer_%d", i);
		consumers[i] = new Thread(cons_names[i]);
		consumers[i]->Fork(Consumer, i);
	};
}