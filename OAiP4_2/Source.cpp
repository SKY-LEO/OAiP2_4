#include <iostream>
#include <conio.h>

using namespace std;

const int VALUE_OF_STRING = 15;
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

struct Value_Of_Symbols
{
	char symbol;
	double number;
};

struct Number_Of_Symbols
{
	int letters;
	int operators;
};

Input_Stack* pushStack(Input_Stack* begin, char symbol);
void deleteStack(Input_Stack*& begin);
void popStack(Input_Stack*& begin, char& symbol);
Output_Stack* pushStack(Output_Stack* begin, double number);
void deleteStack(Output_Stack*& begin);
void popStack(Output_Stack*& begin, double& number);
void getMemoryForArray(char*& my_string, int& size);
void checkInputString(char*& my_string, int& size);
void calculateResult(char*& output_string, Output_Stack*& begin);
int checkStringLength(char* my_string);
int checkPriority(char symbol);
int correctInputInt();
double correctInputDouble();
bool checkLetterAndOperatorBalance(int letters, int operators);
bool rewriteToPolish(char*& input_string, char*& output_string, Input_Stack*& begin, int size_input_string);

int main()
{
	int size_of_string = VALUE_OF_STRING, code, length;
	Input_Stack* input_begin = NULL;
	Output_Stack* output_begin = NULL;
	char* input_string = NULL, * output_string = NULL;
	bool is_error;
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
			is_error = rewriteToPolish(input_string, output_string, input_begin, size_of_string);
			if (!is_error)
			{
				cout << "\nPolish record:"<<endl;
				for (int i = 0; output_string[i] != '\0'; i++)
				{
					cout << output_string[i];
				}
				cout << endl;
			}
			break;
		case 2:
			if (output_string)
			{
				if (!is_error)calculateResult(output_string, output_begin);
				else cout << "Enter a right string first!" << endl;
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

bool rewriteToPolish(char*& input_string, char*& output_string, Input_Stack*& begin, int size_input_string)
{
	char symbol, next_symbol, variable;
	int j = 0, count_of_open_brackets = 1, count_of_close_brackets = 1;
	bool flag = false, is_error = false, is_open_bracket = false;
	Number_Of_Symbols* arr = new Number_Of_Symbols[size_input_string];//для подсчета баланса операндов и операторов
	for (int i = 0; i < size_input_string; i++)			//нулевая ячейка для подсчета всех, другие - только для внутренних скобок 
	{
		arr[i].letters = 0;
		arr[i].operators = 0;
	}
	for (int i = 0; input_string[i] != '\0' && !is_error; i++)
	{
		symbol = input_string[i];
		next_symbol = input_string[i + 1];
		if (symbol >= 'a' && symbol <= 'z')
		{
			if (next_symbol >= 'a' && next_symbol <= 'z')
			{
				cout << "There is 2 operands in a row!" << endl;
				is_error = true;
			}
			output_string[j] = symbol;
			j++;
			arr[0].letters += 1;
			if (is_open_bracket)
			{
				for (int k = 1; k < count_of_open_brackets && k < size_input_string; k++)//нулевая ячейка используется 
																			//для подсчета всех операторов и операндов
				{
					arr[k].letters += 1;
				}
			}
		}
		else if (symbol == '(')
		{
			begin = pushStack(begin, symbol);
			is_open_bracket = true;
			count_of_open_brackets++;
		}
		else if (symbol == ')')
		{
			count_of_close_brackets++;
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
				arr[0].operators += 1;
				for (int z = 1; z < count_of_open_brackets && z < size_input_string; z++)
				{
					arr[z].operators += 1;
				}
			}
			if (!flag)//если не нашлась открывающая скобка
			{
				cout << "Bad brackets balance!" << endl;
				is_error = true;
			}
			else if (checkLetterAndOperatorBalance(arr[count_of_open_brackets - 1].letters, arr[count_of_open_brackets - 1].operators))
			{//если баланс внутри скобок нарушен
				cout << "Bad letter and operator balance in brackets!" << endl;
				is_error = true;
			}
			else
			{
				count_of_close_brackets--;
				arr[count_of_open_brackets - 1].letters = 0;
				arr[count_of_open_brackets - 1].operators = 0;
				count_of_open_brackets--;
				if (count_of_open_brackets == 1 && count_of_close_brackets == 1)
				{
					is_open_bracket = false;
				}
				popStack(begin, variable);
			}
		}
		else if (symbol == '/' || symbol == '*' || symbol == '+' || symbol == '-' || symbol == '^')
		{
			if (next_symbol == '/' || next_symbol == '*' || next_symbol == '+' || next_symbol == '-' || next_symbol == '^')
			{
				cout << "There is 2 operators in a row!" << endl;
				is_error = true;
				break;
			}
			while (begin && checkPriority(begin->symbol) >= checkPriority(symbol))
			{
				popStack(begin, variable);
				output_string[j] = variable;
				j++;
				arr[0].operators += 1;
				if (is_open_bracket)
				{
					for (int z = 1; z < count_of_open_brackets && z < size_input_string; z++)
					{
						arr[z].operators += 1;
					}
				}
			}
			begin = pushStack(begin, symbol);
		}
	}
	while (begin)
	{
		popStack(begin, variable);
		arr[0].operators += 1;
		output_string[j] = variable;
		j++;
	}
	if (count_of_close_brackets != count_of_open_brackets)
	{
		cout << "Bad brackets balance!" << endl;
		is_error = true;
	}
	else if (checkLetterAndOperatorBalance(arr[0].letters, arr[0].operators))//если не равно общее число операндов и операторов
	{
		cout << "Bad letter and operator balance!" << endl;
		is_error = true;
	}
	else
	{
		output_string[j] = '\0';
	}
	delete[]arr;
	return is_error;
}

void calculateResult(char*& output_string, Output_Stack*& begin)
{
	char symbol;
	double var1, var2;
	double result = 0.;
	bool is_error = false;
	int k = 0;
	Value_Of_Symbols array_of_struct[NUMBER_OF_VARIABLES_FOR_ARRAY];
	for (int i = 0; output_string[i] != '\0' && !is_error; i++)
	{
		symbol = output_string[i];
		if (symbol >= 'a' && symbol <= 'z')
		{
			bool flag = false;
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
	case '^': return 4;
	case '/':
	case '*': return 3;
	case '+':
	case '-': return 2;
	case '(': return 1;
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
	int length = 0;
	for (length; my_string[length] != '\0'; length++);
	return length;
}
