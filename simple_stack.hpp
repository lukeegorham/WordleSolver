#pragma once

struct node {
	int data;
	node* next = nullptr;
	node* prev = nullptr;

	node(int);
};
node::node(int i) {
	this->data = i;
	this->next = nullptr;
	this->prev = nullptr;
}

// Required Function to Delete List
void recurseDeleteList(node* n) {
	if (n->next == nullptr)
		return;
	recurseDeleteList(n->next);
	free(n);
}

struct list {
	node* head = nullptr;
	node* tail = nullptr;
	int size = 0;

	~list();
	void enq(int);
	void app(int);
	int deq();
	void push(int);
	void prep(int);
	int pop();
	int rme();
	void clear();
	int remove(node* n);
};
list::~list() {				// Default Deconstructor
	if (this->size != 0 && this->head != nullptr) {
		recurseDeleteList(this->head);
	}
}
void list::enq(int i) {		// Enqueue = Add to end (aka append)
	node* new_node = (node*) new node(i);
	if (this->size == 0) {
		this->head = new_node;
		this->tail = new_node;
		this->size++;
		return;
	}
	else {
		new_node->prev = this->tail;
		this->tail->next = new_node;
		this->tail = new_node;
	}
}
void list::app(int i) {		// Append = Add to end (same as enq())
	this->enq(i);
}
int list::deq() {			// Dequeue = Remove from front
	if (this->size <= 0) {
		return -1;
	}
	int n = this->head->data;
	node* n_addr = this->head;
	if (this->size == 1 || this->head->next == nullptr) {
		free(n_addr);
		this->head = nullptr;
		this->tail = nullptr;
		this->size = 0;
		return n;
	}
	this->head->next->prev = nullptr;
	this->head = this->head->next;
	free(n_addr);
	this->size--;
	return n;
}
void list::push(int i) {	// Push = Add to front
	node* new_node = (node*) new node(i);
	if (this->size == 0) {
		this->tail = new_node;
	}
	else {
		this->head->prev = new_node;
	}
	new_node->next = this->head;
	this->head = new_node;
	this->size++;
}
void list::prep(int i) {	// Prepend = Add to front (same as push())
	this->push(i);
}
int list::pop() {			// Pop = Remove from front (same as deq())
	return this->deq();
}
int list::rme() {			// Remove End = Remove from end (NOT same as pop())
	if (this->size == 0) {
		return -1;
	}
	node* n_addr = this->tail;
	int n = n_addr->data;
	if (this->size == 1) {
		free(n_addr);
		this->head = nullptr;
		this->tail = nullptr;
		this->size = 0;
		return n;
	}
	this->tail->prev->next = nullptr;
	this->tail = this->tail->prev;
	free(n_addr);
	this->size--;
	return n;
}
void list::clear() {
	if (this->size != 0) {
		recurseDeleteList(this->head);
	}
	this->size = 0;
	this->head = nullptr;
	this->tail = nullptr;
}
int list::remove(node* n) {
	int return_value;
	if (this->size <= 0) {
		std::cout << "ERROR: Called remove on empty list!\n";
		return -1;
	}
	else if (n == this->head || n->prev == nullptr) {
		return this->deq();
	}
	else if (n == this->tail || n->next == nullptr) {
		return this->rme();
	}
	else if(this->size > 0) {
		return_value = n->data;
		n->prev->next = n->next;
		n->next->prev = n->prev;
		free(n);
		this->size--;
		return return_value;
	}
}
