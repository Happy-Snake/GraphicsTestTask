#include <bitset>
#include <map>
#include <utility>
#include <iterator>
#include <cstdio>

#include "List.h"

template <class T>
union un {
	T value;
	std::bitset<sizeof(T) * 8> bytes;

	un() { value = NULL; }
};

template <typename T>
char* make_str(T &var) {
	un<T> v;
	v.value = var;
	char* str = new char[v.bytes.size()];
	for (int i = sizeof(var) - 1; i >= 0; i--)
		for (int j = 7; j >= 0; j--)
			if (v.bytes[i * 8 + j])
				str[(sizeof(var) - i - 1) * 8 + 7 - j] = '1';
			else
				str[(sizeof(var) - i - 1) * 8 + 7 - j] = '0';
	return str;
};

template <typename T>
T make_var(std::string str, size_t size)
{
	un<T> v;
	for (int i = size - 1; i >= 0; i--)
		for (int j = 7; j >= 0; j--)
			if (str[i * 8 + j] == '1') {
				v.bytes.set((size - 1 - i) * 8 + 7 - j);
			}
			else {
				v.bytes.reset((size - 1 - i) * 8 + 7 - j);
			}
	return v.value;
}

List::~List() {
	//TODO bidirectional movement 
	for (int i = 0; i < count; i++) {
		ListNode* tmp = head->next;
		delete head;
		head = tmp;
	}
}

void List::Serialize(FILE * file) {
	// Сначала пишем общее число элементов
	char* out;
	out = make_str(count);
	fwrite(out, 1, sizeof(out) * 8, file);

	// Записываем элементы. В качестве идентификаторов используем адреса
	ListNode* current = head;
	for (int i = 0; i < count; i++) {
		// Размер адреса элемента и сам элемент
		int currentNodeSize = sizeof(current);
		out = make_str(currentNodeSize);
		fwrite(out, 1, sizeof(out) * 8, file);
		out = make_str(current);
		fwrite(out, 1, currentNodeSize * 8, file);

		// Размер адреса рандомного элемента и сам элемент
		ListNode* randNode = current->rand;
		int randNodeSize = sizeof(randNode);
		out = make_str(randNodeSize);
		fwrite(out, 1, sizeof(randNodeSize) * 8, file);
		out = make_str(randNode);
		fwrite(out, 1, randNodeSize * 8, file);

		// Размер данных и сами данные
		int dataSize = current->data.size();
		out = make_str(dataSize);
		fwrite(out, 1, sizeof(out) * 8, file);
		std::string data = current->data;
		for (char& c : current->data) {
			out = make_str(c);
			fwrite(out, 1, sizeof(c) * 8, file);
		}

		current = current->next;
	}
}

void List::Deserialize(FILE * file) {
	this->clearData();

	fseek(file, 0, SEEK_END);
	long lSize = ftell(file) + 1;
	rewind(file);

	char * buffer = (char*)malloc(sizeof(char) * lSize);
	fgets(buffer, lSize, file);

	std::string str(buffer, lSize);
	std::map< std::string /* id элемента */, std::pair<ListNode* /* указатель на элемент */, std::string /* id рандомного элемента */> > listMap;
	// Читаем
	int elementsNumber = make_var<int>(str.substr(0, sizeof(int) * 8), sizeof(int));
	str = str.substr(sizeof(int) * 8, str.length() - sizeof(int));
	for (int i = 0; i < elementsNumber; i++) {
		// Размер адреса элемента и сам элемент
		int elementIdSize = make_var<int>(str.substr(0, sizeof(int) * 8), sizeof(int));
		str = str.substr(sizeof(int) * 8, str.length() - sizeof(int));
		std::string elementId = str.substr(0, elementIdSize * 8);
		str = str.substr(elementIdSize * 8, str.length() - elementIdSize);

		// Размер адреса рандомного элемента и сам элемент
		int randNodeIdSize = make_var<int>(str.substr(0, sizeof(int) * 8), sizeof(int));
		str = str.substr(sizeof(int) * 8, str.length() - sizeof(int));
		std::string randNodeId = str.substr(0, randNodeIdSize * 8);
		str = str.substr(randNodeIdSize * 8, str.length() - randNodeIdSize);

		// Размер данных и сами данные
		int dataSize = make_var<int>(str.substr(0, sizeof(int) * 8), sizeof(int));
		str = str.substr(sizeof(int) * 8, str.length() - sizeof(int));

		std::string data = "";
		for (int i = 0; i < dataSize; i++) {
			data += make_var<char>(str.substr(0, 8), 1);
			str = str.substr(8, str.length() - 8);
		}

		ListNode* newNode = new ListNode(data);
		this->appendNode(newNode);

		listMap[elementId] = { newNode, randNodeId };
	}

	// Восстанавливаем связь элементов с рандомными элементами
	for (std::pair < std::string, std::pair<ListNode*, std::string>> element : listMap) {
		ListNode* currentElement = element.second.first;
		ListNode* randElement = listMap[element.second.second].first;
		currentElement->rand = randElement;
	}
}

ListNode * List::getNode(int pos) {
	if (pos >= count)
		return NULL;

	//TODO bidirectional movement 
	ListNode* current = head;
	for (int i = 0; i < pos; i++) 
		current = head->next;
	
	return current;
}

void List::appendNode(ListNode * node) {
	count++;

	if (head == NULL) {
		head = node;
		head->next = tail;
		tail = node;
		tail->prev = head;
		return;
	}

	tail->next = node;
	node->prev = tail;
	tail = node;
}

void List::clearData()
{
	for (int i = 0; i < count; i++) {
		ListNode* tmp = head->next;
		head->data.clear();
		delete head;
		head = tmp;
	}
	head = NULL;
	tail = NULL;
	count = 0;
}
