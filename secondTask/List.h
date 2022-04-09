#pragma once
#include <cstdio>
#include <string>

struct ListNode {
	ListNode *      prev;
	ListNode *      next;
	ListNode *      rand;	// ��������� �� ������������ ������� ������� ������ ���� NULL
	std::string     data;

	ListNode(std::string str) : data(str), prev(NULL), next(NULL), rand(NULL) {};
};

class List {
public:
	List() : count(0), head(NULL), tail(NULL) {}
	~List();

	void Serialize(FILE * file);	// ���������� � ���� (���� ������ � ������� fopen(path, "wb"))
	void Deserialize(FILE * file);  // �������� �� ����� (���� ������ � ������� fopen(path, "rb"))

	ListNode*	getNode(int pos);
	ListNode*	getHead() { return head; }
	int			getCount() { return count;  }

	void appendNode(ListNode* node); 

	void clearData();

private:
	ListNode *  head;
	ListNode *	tail;
	int			count;
};
