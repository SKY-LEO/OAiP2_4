#include <iostream>
#include <conio.h>

using namespace std;

const int START_VALUE_OF_STRING = 20;
const int NUMBER_OF_VARIABLES_FOR_ARRAY = 26;

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

struct Num_Of_Sym_Stack
{
	int letters = 0;
	int operators = 0;
	Num_Of_Sym_Stack* next;
};

struct Value_Of_Symbols
{
	char symbol;
	double number;
};

Input_Stack* pushStack(Input_Stack* begin, char symbol);
void deleteStack(Input_Stack*& begin);
void popStack(Input_Stack*& begin, char& symbol);
Output_Stack* pushStack(Output_Stack* begin, double number);
void deleteStack(Output_Stack*& begin);
void popStack(Output_Stack*& begin, double& number);
Num_Of_Sym_Stack* refreshNumOfSymAndLet(Num_Of_Sym_Stack* begin, bool is_letters);
Num_Of_Sym_Stack* pushStack(Num_Of_Sym_Stack* begin);
void deleteStack(Num_Of_Sym_Stack*& begin);
void popStack(Num_Of_Sym_Stack*& begin, int& letters, int& operators);
void getMemoryForArray(char*& my_string, int& size);
void checkInputString(char*& my_string, int& size);
void calculateResult(char*& output_string, Output_Stack*& begin);
int checkStringLength(char* my_string);
int checkPriority(char symbol);
int correctInputInt();
double correctInputDouble();
bool checkLetterAndOperatorBalance(int letters, int operators);
bool rewriteToPolish(char*& input_string, char*& output_string, Input_Stack*& begin);

int main()
{
	int size_of_string = START_VALUE_OF_STRING, code, length;
	Input_Stack* input_begin = NULL;
	Output_Stack* output_begin = NULL;
	char* input_string = NULL, * output_string = NULL;
	bool is_input_deleted = false, is_output_deleted = false, is_error = false;
	while (true)
	{
		do
		{
			cout << "\n Enter input string and create Polish record - 1\n Enter variables and calculate - 2\n EXIT - 0\n";
			code = correctInputInt();
		} while (code < 0 || code > 2);
		switch (code)
		{
		case 1:cout << "Enter string:" << endl;
			input_string = new char[size_of_string];
			checkInputString(input_string, size_of_string);
			length = checkStringLength(input_string);
			output_string = new char[length + 1];
			is_error = rewriteToPolish(input_string, output_string, input_begin);
			delete[]input_string;
			is_input_deleted = true;
			if (!is_error)
			{
				cout << "\nPolish record:" << endl;
				for (int i = 0; output_string[i] != '\0'; i++)
				{
					cout << output_string[i];
				}
				cout << endl;
			}
			break;
		case 2:
			if (output_string == NULL || !is_error)
			{
				calculateResult(output_string, output_begin);
				is_output_deleted = true;
				delete[]output_string;
			}
			else
			{
				cout << "Enter a string first!" << endl;
			}
			break;
		case 0:
			cout << "Safe exit..." << endl;
			if (input_begin) deleteStack(input_begin);
			if (output_begin) deleteStack(output_begin);
			if (!is_input_deleted)delete[]input_string;
			if (!is_output_deleted)delete[]output_string;
			system("pause");
			return 0;
		}
	}
}

bool rewriteToPolish(char*& input_string, char*& output_string, Input_Stack*& begin)
{
	char symbol, next_symbol, variable;
	int j = 0, letters, operators;
	bool is_error = false, flag;
	Num_Of_Sym_Stack* begin_num_of_sym_stack = NULL;
	begin_num_of_sym_stack = pushStack(begin_num_of_sym_stack);
	for (int i = 0; input_string[i] != '\0'; i++)
	{
		symbol = input_string[i];
		next_symbol = input_string[i + 1];
		if (symbol >= 'a' && symbol <= 'z')
		{
			if (next_symbol >= 'a' && next_symbol <= 'z')
			{
				cout << "\nThere is 2 operands in a row!" << endl;
				is_error = true;
			}
			output_string[j] = symbol;
			j++;
			refreshNumOfSymAndLet(begin_num_of_sym_stack, true);
		}
		else if (symbol == '(')
		{
			begin = pushStack(begin, symbol);
			begin_num_of_sym_stack = pushStack(begin_num_of_sym_stack);
		}
		else if (symbol == ')')
		{
			flag = false;
			while (begin)
			{
				if ((begin->symbol) == '(')
				{
					flag = true;
					break;
				}
				popStack(begin, variable);
				output_string[j] = variable;
				j++;
				refreshNumOfSymAndLet(begin_num_of_sym_stack, false);
			}
			if (!flag)//если не нашлась открывающая скобка
			{
				cout << "\nThere are more close brackets than open!" << endl;
				is_error = true;
				break;
			}
			popStack(begin_num_of_sym_stack, letters, operators);
			if (checkLetterAndOperatorBalance(letters, operators))//если баланс внутри скобок нарушен
			{
				cout << "\nBad letter and operator balance in brackets!" << endl;
				is_error = true;
				break;
			}
			else
			{
				popStack(begin, variable);
			}
		}
		else if (symbol == '/' || symbol == '*' || symbol == '+' || symbol == '-' || symbol == '^')
		{
			if (next_symbol == '/' || next_symbol == '*' || next_symbol == '+' || next_symbol == '-' || next_symbol == '^')
			{
				cout << "\nThere is 2 operators in a row!" << endl;
				is_error = true;
				break;
			}
			while (begin && checkPriority(begin->symbol) >= checkPriority(symbol))
			{
				popStack(begin, variable);
				output_string[j] = variable;
				j++;
				refreshNumOfSymAndLet(begin_num_of_sym_stack, false);
			}
			begin = pushStack(begin, symbol);
		}
	}
	if (!is_error)
	{
		while (begin)
		{
			popStack(begin, variable);
			if (variable == '(')
			{
				is_error = true;
				cout << "\nThere are more open brackets than close!" << endl;
				break;
			}
			output_string[j] = variable;
			refreshNumOfSymAndLet(begin_num_of_sym_stack, false);
			j++;
		}
		if (!is_error)
		{
			popStack(begin_num_of_sym_stack, letters, operators);
			if (checkLetterAndOperatorBalance(letters, operators))//если не равно общее число операндов и операторов
			{
				cout << "\nBad letter and operator balance!" << endl;
				is_error = true;
			}
			else
			{
				output_string[j] = '\0';
			}
		}
	}
	if (begin_num_of_sym_stack)deleteStack(begin_num_of_sym_stack);
	return is_error;
}

void calculateResult(char*& output_string, Output_Stack*& begin)
{
	char symbol;
	double var1, var2;
	double result = 0.;
	bool is_error = false, flag;
	int k = 0;
	Value_Of_Symbols array_of_struct[NUMBER_OF_VARIABLES_FOR_ARRAY];
	for (int i = 0; output_string[i] != '\0' && !is_error; i++)
	{
		symbol = output_string[i];
		if (symbol >= 'a' && symbol <= 'z')
		{
			flag = false;
			for (int j = 0; j < k && k < NUMBER_OF_VARIABLES_FOR_ARRAY; j++)
			{
				if (array_of_struct[j].symbol == symbol)
				{
					flag = true;
					begin = pushStack(begin, array_of_struct[j].number);
					break;
				}
			}
			if (!flag)
			{
				cout << "Enter " << symbol << ": ";
				array_of_struct[k].symbol = symbol;
				array_of_struct[k].number = correctInputDouble();
				begin = pushStack(begin, array_of_struct[k].number);
				k++;
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
				else
				{
					cout << "Denominator is 0! Error!" << endl;
					is_error = true;
				}
				break;
			case '^':
				if (var1 == 0. && var2 == 0.)
				{
					cout << "Error! 0^0 is uncertainty!" << endl;
					is_error = true;
				}
				else if (var2 < 0. && (int)var1 != var1)//отрицательное число может быть возведено только в цклую степень
				{
					cout << "For negative number only integer power!" << endl;
					is_error = true;
				}
				else
				{
					result = pow(var2, var1);
				}
				break;
			}
			begin = pushStack(begin, result);
		}
	}
	if (!is_error)
	{
		cout << "Result: " << result << endl;
	}
}

int checkPriority(char symbol)
{
	switch (symbol)
	{
	case '+':
	case '-': return 2;
	case '/':
	case '*': return 3;
	case '(': return 1;
	case '^': return 4;
	default:return 0;
	}
}

bool checkLetterAndOperatorBalance(int letters, int operators)
{
	return letters != operators + 1;
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

void deleteStack(Input_Stack*& begin)
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

Num_Of_Sym_Stack* pushStack(Num_Of_Sym_Stack* begin)
{
	Num_Of_Sym_Stack* temp = new Num_Of_Sym_Stack;
	temp->next = begin;
	return temp;
}

void popStack(Num_Of_Sym_Stack*& begin, int& letters, int& operators)
{
	Num_Of_Sym_Stack* temp = begin;
	letters = begin->letters;
	operators = begin->operators;
	begin = begin->next;
	delete temp;
}

void deleteStack(Num_Of_Sym_Stack*& begin)
{
	while (begin)
	{
		Num_Of_Sym_Stack* temp = begin;
		begin = begin->next;
		delete temp;
	}
	cout << "Temp stack deleted successfully" << endl;
}

Num_Of_Sym_Stack* refreshNumOfSymAndLet(Num_Of_Sym_Stack* begin, bool is_letters)
{
	if (begin)
	{
		if (is_letters)
		{
			begin->letters += 1;
		}
		else
		{
			begin->operators += 1;
		}
		begin = begin->next;
		return refreshNumOfSymAndLet(begin, is_letters);
	}
	return begin;
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
			cin.ignore(256, '\n');
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
			cin.ignore(256, '\n');
			cout << "Error, please write Integer numbers!\n" << "Try again!" << endl;
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
		my_string[i] = (char)_getch();
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

int checkStringLength(char* my_string)
{
	int length;
	for (length = 0; my_string[length] != '\0'; length++);
	return length;
}
