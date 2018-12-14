/*
THIS COPYRIGHT HEADER IS INCLUDED TO INDICATE THAT A PORTION OF THIS CODE WAS PROVIDED AS PART OF A COURSE THAT I TOOK AT UNIVERSITY. THE ORIGINAL CODE IS PART OF THE NACHOS OS FROM THE UNIVERSITY OF CALIFORNIA, I SIMPLY MADE MODIFICATIONS TO THE PROVIDED CODE TO IMPLEMENT THE BOUNDED-BUFFER PROBLEM. I DO NOT CLAIM TO BE THE INITIAL AUTHOR OF THIS CODE. -Daniel Atherton

Copyright (c) 1992-1993 The Regents of the University of California.
All rights reserved.

Permission to use, copy, modify, and distribute this software and its documentation for any purpose, without fee, and without written agreement is hereby granted, provided that the above copyright notice and the following two paragraphs appear in all copies of this software.

IN NO EVENT SHALL THE UNIVERSITY OF CALIFORNIA BE LIABLE TO ANY PARTY FOR DIRECT, INDIRECT, SPEICAL, INCIDENTAL, OR CONSEQUENTIAL DAMAGES ARISING OUT OF THE USE OF THIS SOFTWARE AND ITS DOCUMENTATION, EVEN IF THE UNIVERSITY OF CALIFORNIA HAS BEEN ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

THE UNIVERSITY OF CALIFORNIA SPECIFICALLY DISCLAIMS ANY WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE. THE SOFTWARE PROVIDED HEREUNDER IS ON AN "AS IS" BASIS, AND THE UNIVERSITY OF CALIFORNIA HAS NO OBLIGATION TO PROVIDE MAINTENANCE, SUPPORT, UPDATES, ENHANCEMENTS, OR MODIFICATIONS.
*/

#ifndef RING_H
#define RING_H

#include <iostream>
#include "Synch.h"

class Slot
{
public:
	Slot(int id, int num);
	Slot() { thread_id = 0; number = 0; };

	int thread_id;
	int number;
};

class Ring : public Monitor
{
public:
	Ring(int sz);
	~Ring();

	const char* getName() { return name; }

	void Put(Slot* message);
	void Get(Slot* message);

private:
	const char* name;

	int size;
	int in, out, current;

	Slot* buffer;

	Condition_H* notfull;
	Condition_H* notempty;
};

#endif // RING_H