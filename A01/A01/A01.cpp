// A01.cpp : This file contains the 'main' function. Program execution begins and ends there.
//
//Tests the stack class

#include "pch.h"
#include "queue.h"
#include <iostream>
using namespace std;

int main()
{
	Queue <int> queue;

	//Add items into the queue
	queue.Push(1);
	queue.Push(20);
	queue.Push(1002);

	//Test Print
	cout << "Size: " << queue.GetSize() << endl;
	queue.Print();

	//Test copy constructor
	Queue <int> queue2(queue);
	queue2.Pop();
	queue2.Print();
	queue.Print();

	cout << endl;

	//Test copy assignment
	Queue<int> queue3 = queue2;
	queue3.Pop();
	queue.Push(224);
	queue3.Push(23021);
	queue.Print();
	queue2.Print();
	queue3.Print();
	
	return 0;
}


