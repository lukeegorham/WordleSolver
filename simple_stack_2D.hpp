#pragma once

#include <array>
#include "wordle_ll.h"

struct node_2D {
	int data[2];
	node_2D* next = nullptr;
	node_2D* prev = nullptr;

	node_2D(int, int);
};
node_2D::node_2D(int i1, int i2) {
	this->data[0] = i1;
	this->data[1] = i2;
}

// Required Function to Delete List
void recurseDeleteList(node_2D* n) {
	if (n->next == nullptr)
		return;
	recurseDeleteList(n->next);
	free(n);
}

struct list_2D {
	node_2D* head = nullptr;
	node_2D* tail = nullptr;
	int size = 0;

	~list_2D();
	void enq(int, int);
	void app(int, int);
	int deq();
	void push(int, int);
	void prep(int, int);
	int pop();
	int rme();
	void clear();
	int remove(node_2D* n);
};
list_2D::~list_2D() {				// Default Deconstructor
	if (this->size != 0 && this->head != nullptr) {
		recurseDeleteList(this->head);
	}
}
void list_2D::enq(int i1, int i2) {		// Enqueue = Add to end (aka append)
	node_2D* new_node_2D = (node_2D*) new node_2D(i1, i2);
	if (this->size == 0) {
		this->head = new_node_2D;
		this->tail = new_node_2D;
		this->size++;
		return;
	}
	else {
		new_node_2D->prev = this->tail;
		this->tail->next = new_node_2D;
		this->tail = new_node_2D;
	}
}
void list_2D::app(int i1, int i2) {		// Append = Add to end (same as enq())
	this->enq(i1, i2);
}
int list_2D::deq() {			// Dequeue = Remove from front
	if (this->size <= 0) {
		return -1;
	}
	int n = this->head->data[0];
	node_2D* n_addr = this->head;
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
void list_2D::push(int i1, int i2) {	// Push = Add to front
	node_2D* new_node_2D = (node_2D*) new node_2D(i1, i2);
	if (this->size == 0) {
		this->tail = new_node_2D;
	}
	else {
		this->head->prev = new_node_2D;
	}
	new_node_2D->next = this->head;
	this->head = new_node_2D;
	this->size++;
}
void list_2D::prep(int i1, int i2) {	// Prepend = Add to front (same as push())
	this->push(i1, i2);
}
int list_2D::pop() {			// Pop = Remove from front (same as deq())
	return this->deq();
}
int list_2D::rme() {			// Remove End = Remove from end (NOT same as pop())
	if (this->size == 0) {
		return -1;
	}
	node_2D* n_addr = this->tail;
	int n = n_addr->data[0];
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
void list_2D::clear() {
	if (this->size != 0) {
		recurseDeleteList(this->head);
	}
	this->size = 0;
	this->head = nullptr;
	this->tail = nullptr;
}
int list_2D::remove(node_2D* n) {
	int return_value;
	if (this->size <= 0) {
		std::cout << "ERROR: Called remove on empty list_2D!\n";
		return -1;
	}
	else if (n == this->head || n->prev == nullptr) {
		return this->deq();
	}
	else if (n == this->tail || n->next == nullptr) {
		return this->rme();
	}
	else if (this->size > 0) {
		return_value = n->data[0];
		n->prev->next = n->next;
		n->next->prev = n->prev;
		free(n);
		this->size--;
		return return_value;
	}
}
