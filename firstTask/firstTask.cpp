#include <iostream>
#include <string>
#include <conio.h>

using namespace std;

// Объединение для хранения числа и его побитового представления
union un
{
	long value;
	char bytes[sizeof(long)];
};

int main()
{
	while (true) {
		cout << "Enter a signed integer from -2 147 483 647 to +2 147 483 647:" << endl;
		int code = 0;
		long number = 0;
		bool isNegative = false;

		// TODO backspace
		// Считываем знак и/или первую цифру числа (не ноль)
		while (true) {
			code = _getch();

			// Выход на Esc
			if (code == 27)
				return 0;

			// Проверяем знак отрицания
			if (code == 45) {
				cout << '-';
				isNegative = true;
				continue;
			}

			// Если символ не цифра или 0, игнорируем
			if (code <= 48 or code > 59)
				continue;

			cout << code - 48;
			number = code - 48;
			break;
		};

		do {
			code = _getch();

			// Выход на Esc
			if (code == 27)
				return 0;

			if (code < 48 or code > 59)
				continue;

			unsigned long tmp = number * 10 + code - 48;
			if (tmp > 2147483647)
				continue;

			number = tmp;
			cout << code - 48;
		} while (code != 13);

		if (isNegative)
			number *= -1;

		un v;
		v.value = number;
		string str;
		for (int i = sizeof(number) - 1; i >= 0; i--)
		{
			for (int j = 7; j >= 0; j--)
			{
				if ((v.bytes[i] >> j) & 1)
					str += '1';
				else
					str += '0';
			}
		}

		cout << endl << str << endl;

		cout << endl;
	}
};
