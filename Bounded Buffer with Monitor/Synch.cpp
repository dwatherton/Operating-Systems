/*
THIS COPYRIGHT HEADER IS INCLUDED TO INDICATE THAT A PORTION OF THIS CODE WAS PROVIDED AS PART OF A COURSE THAT I TOOK AT UNIVERSITY. THE ORIGINAL CODE IS PART OF THE NACHOS OS FROM THE UNIVERSITY OF CALIFORNIA, I SIMPLY MADE MODIFICATIONS TO THE PROVIDED CODE TO IMPLEMENT THE BOUNDED-BUFFER PROBLEM. I DO NOT CLAIM TO BE THE INITIAL AUTHOR OF THIS CODE. -Daniel Atherton

Copyright (c) 1992-1993 The Regents of the University of California.
All rights reserved.

Permission to use, copy, modify, and distribute this software and its documentation for any purpose, without fee, and without written agreement is hereby granted, provided that the above copyright notice and the following two paragraphs appear in all copies of this software.

IN NO EVENT SHALL THE UNIVERSITY OF CALIFORNIA BE LIABLE TO ANY PARTY FOR DIRECT, INDIRECT, SPEICAL, INCIDENTAL, OR CONSEQUENTIAL DAMAGES ARISING OUT OF THE USE OF THIS SOFTWARE AND ITS DOCUMENTATION, EVEN IF THE UNIVERSITY OF CALIFORNIA HAS BEEN ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

THE UNIVERSITY OF CALIFORNIA SPECIFICALLY DISCLAIMS ANY WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE. THE SOFTWARE PROVIDED HEREUNDER IS ON AN "AS IS" BASIS, AND THE UNIVERSITY OF CALIFORNIA HAS NO OBLIGATION TO PROVIDE MAINTENANCE, SUPPORT, UPDATES, ENHANCEMENTS, OR MODIFICATIONS.
*/

#include"synch.h"

Semaphore::Semaphore(const char* debugName, int val)
{
	name = debugName;
	value = val;
	queue = new List;
}

Semaphore::~Semaphore()
{
	delete queue;
}

void Semaphore::P()
{
	while (value == 0)
	{
		queue->Append((void *)currentThread);
		currentThread->Sleep();
	}
	value--;
}

void Semaphore::V()
{
	Thread* thread = (Thread *)queue->Remove();
	if (thread != NULL)
	{
		scheduler->ReadyToRun(thread);
	}
	value++;
}

Monitor::Monitor(const char* debugName)
{
	name = debugName;
	next_count = 0;
	next = new Semaphore("next", 0);
	mutex = new Semaphore("mutex", 1);
}

Monitor::~Monitor()
{
	delete next;
	delete mutex;
}

void Monitor::Entry()
{
	mutex->P();
}

void Monitor::Exit()
{
	if (next_count > 0)
	{
		next->V();
	}
	else
	{
		mutex->V();
	}
}

Condition_H::Condition_H(const char* debugName, Monitor* monitor)
{
	name = debugName;
	mon = monitor;
	count = 0;
	sem = new Semaphore(name, 0);
}

Condition_H::~Condition_H()
{
	delete sem;
	delete mon;
}

void Condition_H::Wait()
{
	count++;
	if (mon->next_count > 0)
	{
		mon->next->V();
	}
	else
	{
		mon->mutex->V();
	}
	sem->P();
	count--;
}

void Condition_H::Signal()
{
	if (count > 0)
	{
		(mon->next_count)++;

		sem->V();
		mon->next->P();

		(mon->next_count)--;
	}
}