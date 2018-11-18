#pragma once

template <class Data>
class Queue {
	struct Node {
		Node(const Data& value, Node* prev = nullptr, Node* next = nullptr)
			: value(value)
			, prev(prev)
			, next(next)
		{}

		Data value;
		Node* prev;
		Node* next;
	};
	Node* head;
	Node* tail;

public:

	Queue()
		: head(nullptr)
		, tail(nullptr)
	{}

	Queue(const Queue& other) {
		copyFrom(other);
	}

	Queue& operator=(const Queue& other) {
		if (this != &other) {
			clean();
			copyFrom(other);
		}
		return *this;
	}

	~Queue() {
		clean();
	}

	bool empty() const {
		return !head;
	}

	Data& front() {
		return head->value;
	}

	const Data& front() const {
		return head->value;
	}

	Data& back() {
		return tail->value;
	}

	const Data& back() const {
		return tail->value;
	}

	void pushBack(const Data& value) {
		if (empty()) {
			tail = head = new Node(value);
		}
		else {
			tail->next = new Node(value, tail);
			tail = tail->next;
		}
	}

	void popBack() {
		Node* node = tail;
		if (tail && tail->prev) {
			tail = tail->prev;
			tail->next = nullptr;
		}
		else {
			tail = head = nullptr;
		}
		delete node;
	}

	void popFront() {
		Node* node = head;
		if (head && head->next) {
			head = head->next;
			head->prev = nullptr;
		}
		else {
			head = tail = nullptr;
		}
		delete node;
	}

	void append(Queue& other) {

		if (empty()) {
			head = other.head;
			tail = other.tail;
		}
		else if(!other.empty()) {
			tail->next = other.head;
			other.head->prev = tail;
			tail = other.tail;
		}
		other.head = other.tail = nullptr;
	}

	template<class UnaryPredicate>
	Queue* split_if(UnaryPredicate p) {
		for (Node* i = head; i; i = i->next) {
			if (p(i->value)) {

				if (i == head) {
					Node* newQueueTail = tail;
					head = tail = nullptr;
					return new Queue(i, newQueueTail);
				}

				Node* newQueueTail = tail;
				tail = i->prev;
				tail->next = i->prev = nullptr;
				return new Queue(i, newQueueTail);
			}
		}
		return nullptr;
	}

	template<class T, class BinaryPredicate>
	Queue* split_if(T value, BinaryPredicate p) {
		for (Node* i = head; i; i = i->next) {
			if (p(i->value, value)) {

				if (i == head) {
					Node* newQueueTail = tail;
					head = tail = nullptr;
					return new Queue(i, newQueueTail);
				}

				Node* newQueueTail = tail;
				tail = i->prev;
				tail->next = i->prev = nullptr;
				return new Queue(i, newQueueTail);
			}
		}
		return nullptr;
	}
	
	void for_each(void(*f)(const Data&)) const {
		for (Node* i = head; i; i = i->next) {
			f(i->value);
		}
	}

private:

	Queue(Node* head, Node* tail)
		: head(head)
		, tail(tail)
	{}

	void clean() {
		while (head) {
			Node* node = head;
			head = head->next;
			delete node;
		}
	}

	void copyFrom(const Queue& other) {

		head = tail = nullptr;

		for (Node* i = other.head; i; i = i->next) {
			pushBack(i->value);
		}
	}
};