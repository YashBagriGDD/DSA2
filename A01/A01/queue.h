#pragma once

template <class T>
class Queue {
public:
	//constructor
	Queue() {
		top = 0;
	}
	//destructor
	~Queue() {
		//pop queue until it is empty
		while (IsEmpty() == false) {
			Pop();
		}
		IsEmpty();
	}

	Queue(const Queue& source); //copy constructor
	Queue<T> operator=(const Queue& source); //copy constructor

	//public functions
	void Push(const T& object);
	void Pop();
	bool IsEmpty();
	int GetSize();
	void Print();

private: 
	//linked list to hold data and reference other nodes
	struct Node {
		T data; // data in this node
		Node *next; //the next node in the stack

		//Node constructor
		Node(const T& data, Node *next) {
			this->data = data;
			this->next = next;
		}
	};

	Node *top; //top of the queue
};

//Function definitions

//copy constructor
template <class T>
inline Queue<T>::Queue(const Queue & source) {
	//Get reference to the list passed in
	Node *current = source.top;

	//Copy over the data
	Node *copy = new Node(current->data, 0);

	//Keep track of first in queue
	this->top = copy;

	//First element is copied so move to next before loop
	current = current->next;

	//Loop until there are no more elements
	while (current != 0) {
		//allocate next node
		copy = copy->next = new Node(current->data, 0);

		//Advance current
		current = current->next;
	}
}

//copy assignment
template <class T>
inline Queue<T> Queue<T>::operator=(const Queue & source) {
	//Check for self assignment
	if (&source == this)
		return *this;

	//Copy the data
	//Get reference to the list passed in
	Node *current = source.top;

	//Copy over the data
	Node *copy = new Node(current->data, 0);

	//Keep track of first in queue
	this->top = copy;

	//First element is copied sso move to next before loop
	current = current->next;

	//Loop until there are no more elements
	while (current != 0) {
		//allocate next node
		copy = copy->next = new Node(current->data, 0);

		//Advance current
		current = current->next;
	}
	return *this;
}

//Adds data to the queue
template<class T>
void Queue<T>::Push(const T & data) {
	//Check if the top is empty
	if (top == 0) {
		//If top is empty, set top to the data
		top = new Node(data, 0);
	}
	else {
		Node *currentQueue = top; //Save the node to iterate through the queue

		//Check if the next spot is empty
		while (currentQueue->next != 0) {
			//Set the current spot to the next spot if it is full
			currentQueue = currentQueue->next;
		}

		currentQueue->next = new Node(data, 0);
	}
}

//Deletes the top of the queue
template<class T>
void Queue<T>::Pop() {
	//Check if queue is empty
	if (!IsEmpty()) {
		//Delete top of queue and set the top of the queue to the next node
		Node *queueTop = top;
		top = top->next;
		delete queueTop;
	}
	else {
		throw std::invalid_argument("List is empty.");
	}
}

//Check to see if the queue is empty
template<class T>
bool Queue<T>::IsEmpty() {
	return top == 0;
}

//Iterates through the queue to get the size
template<class T>
int Queue<T>::GetSize() {
	
	int count = 0;
	Node *queueTop = top; //Saves the top to iterate through the list

	//Loop through while the queueTop exists
	while (queueTop != 0) {
		queueTop = queueTop->next;
		count++;
	}

	return count;
}

//Iterates through the queue to print the data
template<class T>
void Queue<T>::Print() {
	Node *queueTop = top; // top of the queue

	while (queueTop != 0) {
		std::cout << queueTop->data << " ";
		queueTop = queueTop->next;
	}

	std::cout << endl;
}