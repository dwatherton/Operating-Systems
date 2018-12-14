/*
THIS COPYRIGHT HEADER IS INCLUDED TO INDICATE THAT A PORTION OF THIS CODE WAS PROVIDED AS PART OF A COURSE THAT I TOOK AT UNIVERSITY. THE ORIGINAL CODE IS PART OF THE NACHOS OS FROM THE UNIVERSITY OF CALIFORNIA, I SIMPLY MADE MODIFICATIONS TO THE PROVIDED CODE TO IMPLEMENT THE BOUNDED-BUFFER PROBLEM. I DO NOT CLAIM TO BE THE INITIAL AUTHOR OF THIS CODE. -Daniel Atherton

Copyright (c) 1992-1993 The Regents of the University of California.
All rights reserved.

Permission to use, copy, modify, and distribute this software and its documentation for any purpose, without fee, and without written agreement is hereby granted, provided that the above copyright notice and the following two paragraphs appear in all copies of this software.

IN NO EVENT SHALL THE UNIVERSITY OF CALIFORNIA BE LIABLE TO ANY PARTY FOR DIRECT, INDIRECT, SPEICAL, INCIDENTAL, OR CONSEQUENTIAL DAMAGES ARISING OUT OF THE USE OF THIS SOFTWARE AND ITS DOCUMENTATION, EVEN IF THE UNIVERSITY OF CALIFORNIA HAS BEEN ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

THE UNIVERSITY OF CALIFORNIA SPECIFICALLY DISCLAIMS ANY WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE. THE SOFTWARE PROVIDED HEREUNDER IS ON AN "AS IS" BASIS, AND THE UNIVERSITY OF CALIFORNIA HAS NO OBLIGATION TO PROVIDE MAINTENANCE, SUPPORT, UPDATES, ENHANCEMENTS, OR MODIFICATIONS.
*/

#include "ring.h"

Slot::Slot(int id, int num)
{
	thread_id = id;
	number = num;
}

Ring::Ring(int sz) : Monitor("Monitor")
{
	if (sz < 1)
	{
		std::cout << "Error, the ring's size is too small!" << std::endl;
		exit(1);
	}

	name = Monitor::getName();

	size = sz;

	in = 0;
	out = 0;
	current = 0;

	buffer = new Slot[size];

	notfull = new Condition_H("notfull", this);
	notempty = new Condition_H("notempty", this);
}

Ring::~Ring()
{
	delete[] buffer;
	delete notfull;
	delete notempty;
}

void Ring::Put(Slot* message)
{
	Entry();
	if (current == size)
	{
		notfull->Wait();
	}

	buffer[in].thread_id = message->thread_id;
	buffer[in].number = message->number;
	in = (in + 1) % size;
	current++;

	notempty->Signal();
	Exit();
}

void Ring::Get(Slot* message)
{
	Entry();
	if (current == 0)
	{
		notempty->Wait();
	}

	message->thread_id = buffer[out].thread_id;
	message->number = buffer[out].number;
	out = (out + 1) % size;
	current--;

	notfull->Signal();
	Exit();
}