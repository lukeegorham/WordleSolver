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
void recurseDeleteList(node* n);

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
	//void insert(int loc, int data);
	//int remove(int pos);
};
list::~list() {
	if (this->size != 0 && this->head != nullptr) {
		recurseDeleteList(this->head);
	}
}
// Enqueue = Add to end (aka append)
void list::enq(int i) {
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
// Append = Add to end (same as enq())
void list::app(int i) {
	this->enq(i);
}
// Dequeue = Remove from front
int list::deq() {
	if (this->size == 0) {
		return -1;
	}
	int n = this->head->data;
	node* n_addr = this->head;
	if (this->size == 1) {
		free(n_addr);
		this->head == nullptr;
		this->tail == nullptr;
		this->size = 0;
		return n;
	}
	this->head->next->prev = nullptr;
	this->head = this->head->next;
	free(n_addr);
	this->size--;
	return n;
}
// Push = Add to front
void list::push(int i) {
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
// Prepend = Add to front (same as push())
void list::prep(int i) {
	this->push(i);
}
// Pop = Remove from front (same as deq())
int list::pop() {
	return this->deq();
}
// Remove End = Remove from end (NOT same as pop())
int list::rme() {
	if (this->size == 0) {
		return -1;
	}
	node* n_addr = this->tail;
	int n = n_addr->data;
	if (this->size == 1) {
		free(n_addr);
		this->head == nullptr;
		this->tail == nullptr;
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


void recurseDeleteList(node* n) {
	if (n->next == nullptr)
		return;
	recurseDeleteList(n->next);
	free(n);
}
