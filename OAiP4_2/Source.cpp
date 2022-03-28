#include <iostream>
#include <ctime>
#include <conio.h>
#include <iomanip>

using namespace std;

struct Stack
{
	char symbol;
	Stack* next;
};

Stack* pushStack(Stack* begin, char symbol);
Stack* viewStack(Stack* begin);
void deleteStack(Stack*& begin);
char popStack(Stack*& begin);
double correctInputDouble();
void getMemoryForArray(char*& my_string, int& size);


using namespace std;
void checkInputString(char*&, int&);

int checkStringLength(char*);


int checkPriority(char symbol);
void rewriteToPolish(char*& input_string, char*& output_string, Stack*& begin);
void calculateResult(char*& output_string, double* array_of_alphabet, Stack*& begin);

int main()
{
	int VALUE_OF_STRING = 1;
	Stack* begin = NULL;
	int max_number;
	char code;
	char* input_string = new char[VALUE_OF_STRING];
	char* output_string;
	int length;
	double alphabet[60];//переделать на динамический
	for (int i = 0; i < 26; i++)
	{
		alphabet[i] = i;
	}
	do
	{
		cout << "Enter string:" << endl;
		checkInputString(input_string, VALUE_OF_STRING);
		length = checkStringLength(input_string);
		output_string = new char[length + 1];
		rewriteToPolish(input_string, output_string, begin);
		for (int i = 0; output_string[i] != '\0'; i++)
		{
			cout << output_string[i];
		}
		calculateResult(output_string, alphabet, begin);
		deleteStack(begin);
		cout << "Do you want to continue?(Y/N)" << endl;
		code = _getch();
	} while (code == 'Y' || code == 'y');
	cout << "Safe exit..." << endl;
	if (begin) deleteStack(begin);
	delete[]input_string;
	delete[]output_string;
	system("pause");
	return 0;
}

void calculateResult(char*& output_string, double* array_of_alphabet, Stack*& begin)
{
	char symbol;
	int number;
	int number_on_alphabet;
	char var1, var2;
	double result = 0.0;
	char g = 'z' + 1;
	//double* array_of_numbers = new double[countDifferentVariables(output_string)];
	for (int i = 0; output_string[i] != '\0'; i++)
	{
		symbol = output_string[i];
		if (symbol >= 'a' && symbol <= 'z')
		{
			number_on_alphabet = symbol - 'a';
			if (array_of_alphabet[number_on_alphabet] == number_on_alphabet)
			{
				cout << "Enter " << symbol << ": ";
				array_of_alphabet[number_on_alphabet] = correctInputDouble();
				begin = pushStack(begin, symbol);
			}
		}
		else if(symbol == '/' || symbol == '*' || symbol == '+' || symbol == '-' || symbol == '^') 
		{
			var1 = popStack(begin);
			var2 = popStack(begin);
			switch (symbol)
			{
			case '+':result = array_of_alphabet[var2 - 'a'] + array_of_alphabet[var1 - 'a'];
				break;
			case '-':result = array_of_alphabet[var2 - 'a'] - array_of_alphabet[var1 - 'a'];
				break;
			case '*':result = array_of_alphabet[var2 - 'a'] * array_of_alphabet[var1 - 'a'];
				break;
			case '/':result = array_of_alphabet[var2 - 'a'] / array_of_alphabet[var1 - 'a'];
				break;
			case '^':
				break;
			default: cout << "ERROR";
			}
			
			array_of_alphabet[g-'a'] = result;
			begin = pushStack(begin, g);
			g++;
		}

	}
	cout << "Result " << result << endl;
}

int checkPriority(char symbol)
{
	switch (symbol)
	{
	case '^': return 4;
	case '/':
	case '*': return 3;
	case '+':
	case '-': return 2;
	case '(': return 1;
	default:return 0;
	}
}

void rewriteToPolish(char*& input_string, char*& output_string, Stack*& begin)
{
	char symbol, variable;
	int j = 0;
	for (int i = 0; input_string[i] != '\0'; i++)
	{
		symbol = input_string[i];
		if (symbol >= 'a' && symbol <= 'z')
		{
			output_string[j] = symbol;
			j++;
		}
		else if (symbol == '(')
		{
			begin = pushStack(begin, symbol);
		}
		else if (symbol == ')')
		{
			while ((begin->symbol) != '(')
			{
				variable = popStack(begin);
				if (!begin)
				{
					variable = '\0';
				}
				output_string[j] = variable;
				j++;
			}
			variable = popStack(begin);
		}
		else if (symbol == '/' || symbol == '*' || symbol == '+' || symbol == '-' || symbol == '^')
		{
			while (begin && checkPriority(symbol) <= checkPriority(begin->symbol))
			{
				variable = popStack(begin);
				output_string[j] = variable;
				j++;
			}
			begin = pushStack(begin, symbol);
		}
	}
	while (begin)
	{
		variable = popStack(begin);
		output_string[j] = variable;
		j++;
	}
	output_string[j] = '\0';
}


Stack* pushStack(Stack* begin, char symbol)
{
	Stack* temp = new Stack;
	temp->symbol = symbol;
	temp->next = begin;
	return temp;
}

char popStack(Stack*& begin)
{
	Stack* temp = begin;
	char out;
	out = begin->symbol;
	begin = begin->next;
	delete temp;
	return out;
}

Stack* viewStack(Stack* begin)
{
	if (begin)
	{
		cout << begin->symbol << '\t';
		begin = begin->next;
		return viewStack(begin);
	}
	cout << endl;
	return begin;
}

void deleteStack(Stack*& begin)//по адресу
{
	while (begin)
	{
		Stack* temp = begin;
		begin = begin->next;
		delete temp;
	}
	cout << "Stack deleted successfully" << endl;
}

double correctInputDouble()
{
	double a;
	while (true)
	{
		cin >> a;
		if (cin.get() == '\n') {
			break;
		}
		else
		{
			cin.clear();
			cin.ignore(numeric_limits<streamsize>::max(), '\n');
			cout << "Error, please write INT numbers!\n" << "Try again!" << endl;
		}
	}
	return a;
}


void checkInputString(char*& my_string, int& size)
{
	int i = 0;
	//while (i < VALUE_OF_STRING - 1)//последний элемент для терминатора
	while (true)
	{
		if (i >= size)
		{
			getMemoryForArray(my_string, size);
		}
		my_string[i] = _getch();
		if (my_string[i] == 13) break;//enter
		if (my_string[i] == 8) {//backspace
			cout << "\b \b";
			i--;
			continue;
		}
		if (!((my_string[i] == '+' || my_string[i] == '-' || my_string[i] == '*' || my_string[i] == '/'
			|| my_string[i] == '^' || my_string[i] == '(' || my_string[i] == ')')
			|| (my_string[i] >= '0' && my_string[i] <= '9') || (my_string[i] >= 'a' && my_string[i] <= 'z')))
		{
			continue;
		}
		cout << my_string[i];
		i++;
	}
	my_string[i] = '\0';
}

void getMemoryForArray(char*& my_string, int& size)
{
	char* my_string_new = new char[size * 2];
	for (int i = 0; i < size; i++)
	{
		my_string_new[i] = my_string[i];
	}
	for (int i = size; i < size * 2; i++)
	{
		my_string_new[i] = '\0';
	}
	delete[]my_string;
	my_string = my_string_new;
	size *= 2;
}

int checkStringLength(char* my_string)
{
	int length = 1;
	for (length; my_string[length] != '\0'; length++);
	return length;
}




