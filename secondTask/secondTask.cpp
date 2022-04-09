#include <iostream>

#include "List.h"

#pragma warning(disable : 4996)

int main() {
	
	List* list = new List();

	// Заполняем структуру
	ListNode* arr[5];
	for (int i = 0; i < 4; i++) {
		ListNode* newNode = new ListNode("Element #" + std::to_string(i));
		list->appendNode(newNode);
		arr[i] = newNode;
	}

	//arr[0]->rand = arr[1];
	arr[1]->rand = arr[2];
	arr[2]->rand = arr[3];
	arr[3]->rand = arr[0];
	//arr[4]->rand = arr[0];

	// Выводим структуру
	ListNode* current = list->getHead();
	for (int i = 0; i < list->getCount(); i++) {
		std::cout << std::endl << current->data << " --> ";
		if (current->rand)
			std::cout << current->rand->data;
		current = current->next;
	}
	std::cout << std::endl;

	// Открываем файл
	FILE* file = std::fopen("./serialize", "wb");
	if (!file) {
		std::perror("File opening failed");
		return 1;
	}

	list->Serialize(file);

	std::fclose(file);

	// Открываем файл
	file = std::fopen("./serialize", "rb");
	if (!file) {
		std::perror("File opening failed");
		return 1;
	}

	list->Deserialize(file);

	// завершение работы
	fclose(file);

	// Выводим структуру
	current = list->getHead();
	for (int i = 0; i < list->getCount(); i++) {
		std::cout << std::endl << current->data << " --> ";
		if (current->rand)
			std::cout << current->rand->data;
		current = current->next;
	}

	std::cout << std::endl;
	system("pause");
	return 0;
}