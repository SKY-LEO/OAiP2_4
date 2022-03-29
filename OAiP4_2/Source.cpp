#include <iostream>
#include <ctime>
#include <conio.h>
#include <iomanip>

using namespace std;

const int VALUE_OF_STRING = 20;
const int NUMBER_OF_VARIABLES_FOR_ARRAY = 5;

struct Input_Stack
{
	char symbol;
	Input_Stack* next;
};

struct Output_Stack
{
	double number;
	Output_Stack* next;
};

Input_Stack* pushStack(Input_Stack* begin, char symbol);
Input_Stack* viewStack(Input_Stack* begin);
void deleteStack(Input_Stack*& begin);
void popStack(Input_Stack*& begin, char& symbol);
Output_Stack* pushStack(Output_Stack* begin, double number);
Output_Stack* viewStack(Output_Stack* begin);
void deleteStack(Output_Stack*& begin);
void popStack(Output_Stack*& begin, double& number);
double correctInputDouble();
int correctInputInt();
void getMemoryForArray(char*& my_string, int& size);


void checkInputString(char*&, int&);

int checkStringLength(char*);


int checkPriority(char symbol);
void rewriteToPolish(char*& input_string, char*& output_string, Input_Stack*& begin);
void calculateResult(char*& output_string, Output_Stack*& begin);
void pushBackArray(char*& my_string, int& size, char symbol);

int main()
{
	int size_of_string = VALUE_OF_STRING;
	Input_Stack* input_begin = NULL;
	Output_Stack* output_begin = NULL;
	int code;
	char* input_string = NULL;
	char* output_string = NULL;
	int length;
	while (true)
	{
		do
		{
			cout << "\n Enter input string - 1\n Create Polish record - 2\n Enter variables and calculate - 3\n EXIT - 0\n";
			code = correctInputInt();
		} while (code < 0 || code > 3);
		switch (code)
		{
		case 1:cout << "Enter string:" << endl;
			input_string = new char[size_of_string];
			checkInputString(input_string, size_of_string);
			break;
		case 2:
			if (input_string)
			{
				length = checkStringLength(input_string);
				output_string = new char[length];
				rewriteToPolish(input_string, output_string, input_begin);
				for (int i = 0; output_string[i] != '\0'; i++)
				{
					cout << output_string[i];
				}
				//delete[]input_string;
			}
			else
			{
				cout << "Enter a string first!" << endl;
			}
			break;
		case 3:
			if (output_string)
			{
				calculateResult(output_string, output_begin);
				//delete[]output_string;
			}
			else
			{
				cout << "Create a Polish record first!" << endl;
			}
			break;
		case 0:
			cout << "Safe exit..." << endl;
			if (input_begin) deleteStack(input_begin);
			if (output_begin) deleteStack(output_begin);
			delete[]input_string;
			delete[]output_string;
			system("pause");
			return 0;
		}
	}
}

void calculateResult(char*& output_string, Output_Stack*& begin)
{
	char symbol;
	double number;
	double var1, var2;
	double result = 0.;
	bool zero_denominator = false;
	int size_of_array = NUMBER_OF_VARIABLES_FOR_ARRAY;
	char* variables = new char[size_of_array];
	for (int i = 0; i < size_of_array; i++) variables[i] = '\0';
	for (int i = 0; output_string[i] != '\0' && !zero_denominator; i++)
	{
		symbol = output_string[i];
		if (symbol >= 'a' && symbol <= 'z')
		{
			bool flag = false;
			for (int j = 0; variables[j] != '\0'; j++)
			{
				if (variables[j] == symbol)
				{
					flag = true;
					break;
				}
			}
			if (!flag)
			{
				cout << "Enter " << symbol << ": ";
				number = correctInputDouble();
				begin = pushStack(begin, number);
				pushBackArray(variables, size_of_array, symbol);
			}
		}
		else
		{
			popStack(begin, var1);
			popStack(begin, var2);
			switch (symbol)
			{
			case '+':result = var2 + var1;
				break;
			case '-':result = var2 - var1;
				break;
			case '*':result = var2 * var1;
				break;
			case '/':
				if (var1 != 0.)
				{
					result = var2 / var1;
				}
				else {
					cout << "Denominator is 0! Error!" << endl;
					zero_denominator = true;
				}
				break;
			case '^':result = pow(var2, var1);
				break;
			default: cout << "ERROR";
			}
			begin = pushStack(begin, result);
		}
	}
	if (!zero_denominator)
	{
		cout << "Result: " << result << endl;
	}
	deleteStack(begin);
	delete[]variables;
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

void rewriteToPolish(char*& input_string, char*& output_string, Input_Stack*& begin)
{
	char symbol, next_symbol, variable;
	int j = 0;
	bool flag = false;
	bool flagn = false;
	for (int i = 0; input_string[i] != '\0'; i++)
	{
		symbol = input_string[i];
		next_symbol = input_string[i + 1];
		if (symbol >= 'a' && symbol <= 'z')
		{
			if (next_symbol >= 'a' && next_symbol <= 'z')
			{
				cout << "There is 2 operands in a row!" << endl;
				//break;
				return;
			}
			output_string[j] = symbol;
			j++;
		}
		else if (symbol == '(')
		{
			begin = pushStack(begin, symbol);
		}
		else if (symbol == ')')
		{
			/*while ((begin->symbol) != '(')
			{
				popStack(begin, variable);
				if (!begin)
				{
					variable = '\0';
				}
				output_string[j] = variable;
				j++;
			}*/
			
			while (begin)
			{
				if (begin->symbol == '(')
				{
					flag = true;
					break;
				}
				popStack(begin, variable);
				//if (!begin)
				//{
					//variable = '\0';
				//}
				output_string[j] = variable;
				j++;
			}
			if (!flag)
			{
				cout << "No balance!" << endl;
				return;
			}
			popStack(begin, variable);
		}
		else if (symbol == '/' || symbol == '*' || symbol == '+' || symbol == '-' || symbol == '^')
		{
			if (next_symbol == '/' || next_symbol == '*' || next_symbol == '+' || next_symbol == '-' || next_symbol == '^')
			{
				cout << "There is 2 operators in a row!" << endl;
				break;
			}
			while (begin)
			{
				if (checkPriority(begin->symbol) >= checkPriority(symbol))
				{
					popStack(begin, variable);
					output_string[j] = variable;
					j++;
					flagn = true;
				}
			}
			if (!flagn)
			{
				cout << "No balance!1" << endl;
				return;
			}
			begin = pushStack(begin, symbol);
		}
	}
	while (begin)
	{
		popStack(begin, variable);
		if (variable == '(')
		{
			cout << "error" << endl;
			return;
		}
		output_string[j] = variable;
		j++;
	}
	output_string[j] = '\0';
}

Input_Stack* pushStack(Input_Stack* begin, char symbol)
{
	Input_Stack* temp = new Input_Stack;
	temp->symbol = symbol;
	temp->next = begin;
	return temp;
}

void popStack(Input_Stack*& begin, char& symbol)
{
	Input_Stack* temp = begin;
	symbol = begin->symbol;
	begin = begin->next;
	delete temp;
}

Input_Stack* viewStack(Input_Stack* begin)
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

void deleteStack(Input_Stack*& begin)//по адресу
{
	while (begin)
	{
		Input_Stack* temp = begin;
		begin = begin->next;
		delete temp;
	}
	cout << "Input stack deleted successfully" << endl;
}

Output_Stack* pushStack(Output_Stack* begin, double number)
{
	Output_Stack* temp = new Output_Stack;
	temp->number = number;
	temp->next = begin;
	return temp;
}

void popStack(Output_Stack*& begin, double& number)
{
	Output_Stack* temp = begin;
	number = begin->number;
	begin = begin->next;
	delete temp;
}

Output_Stack* viewStack(Output_Stack* begin)
{
	if (begin)
	{
		cout << begin->number << '\t';
		begin = begin->next;
		return viewStack(begin);
	}
	cout << endl;
	return begin;
}

void deleteStack(Output_Stack*& begin)
{
	while (begin)
	{
		Output_Stack* temp = begin;
		begin = begin->next;
		delete temp;
	}
	cout << "Output stack deleted successfully" << endl;
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
			cout << "Error, please write Double numbers!\n" << "Try again!" << endl;
		}
	}
	return a;
}

int correctInputInt()
{
	int a;
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
			|| (my_string[i] >= 'a' && my_string[i] <= 'z')))
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

void pushBackArray(char*& my_string, int& size, char symbol)
{
	int length = checkStringLength(my_string);
	if (length + 1 == size)
	{
		getMemoryForArray(my_string, size);
	}
	my_string[length] = symbol;
}

int checkStringLength(char* my_string)
{
	int length = 0;
	for (length; my_string[length] != '\0'; length++);
	return length;
}






