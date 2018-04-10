#include<iostream>
#include<fstream>
#include<string>
#include<vector>
#include<map>

using namespace std;

enum Value
{
	FALSE,
	TRUE,
	UNDEFINED
};

class parse
{
private:
	int N, M;
	map<char, Value> value;
	map<char, int> number;
	ifstream input;
	vector<string> operation;
public:
	parse(string inpath)
	{
		input.open(inpath);
		value['0'] = FALSE;
		value['1'] = TRUE;
		value['?'] = UNDEFINED;
		input >> N >> M;
		for (int i = 0; i < N; i++)
		{
			char tempName;
			bool tempValue;
			input >> tempName >> tempValue;
			if (tempValue)
				value[tempName] = TRUE;
			else
				value[tempName] = FALSE;
		}
		input.get();
		for (int i = 0; i < M; i++)
		{
			string temp;
			getline(input, temp);
			operation.push_back(temp);
		}
		input.close();
	}

	Value getValue(char ch, int number)
	{
		if (!(value.find(ch) != value.end()))
		{
			for (int i = number; i < M; i++)
			{
				if (operation[i][0] == ch)
				{
					return calculate(operation[i], i);
				}
			}
			return UNDEFINED;
		}
		else
		{
			return value[ch];
		}
	}

	Value calculate(string str, int number)
	{
		Value left, right;
		if (!(str.find('|') != str.npos) & !(str.find('&') != str.npos))
		{
			switch (getValue(str[4], number))
			{
			case FALSE:
				str.replace(4, 1, "0");
				break;
			case TRUE:
				str.replace(4, 1, "1");
				break;
			case UNDEFINED:
				str.replace(4, 1, "?");
				break;
			}
		}
		while (str.find('&') != str.npos)
		{
			left = getValue(str[str.find('&') - 2], number);
			right = getValue(str[str.find('&') + 2], number);
			if ((left == FALSE) | (right == FALSE))
			{
				str.replace(str.find('&') - 2, 5, "0");
			}
			else if ((left == UNDEFINED) | (right == UNDEFINED))
			{
				str.replace(str.find('&') - 2, 5, "?");
			}
			else if ((left == TRUE) & (right == TRUE))
			{
				str.replace(str.find('&') - 2, 5, "1");
			}
		}
		while (str.find('|') != str.npos)
		{
			left = getValue(str[str.find('|') - 2], number);
			right = getValue(str[str.find('|') + 2], number);
			if ((left == TRUE) | (right == TRUE))
			{
				str.replace(str.find('|') - 2, 5, "1");
			}
			else if ((left == UNDEFINED) | (right == UNDEFINED))
			{
				str.replace(str.find('|') - 2, 5, "?");
			}
			else if ((left == FALSE) | (right == FALSE))
			{
				str.replace(str.find('|') - 2, 5, "0");
			}
		}
		switch (str[4])
		{
		case '0':
			value[str[0]] = FALSE;
			return FALSE;
			break;
		case '1':
			value[str[0]] = TRUE;
			return TRUE;
			break;
		case '?':
			value[str[0]] = UNDEFINED;
			return UNDEFINED;
			break;
		}
	}
	void count()
	{
		for (int i = 0; i < M; i++)
		{
			calculate(operation[i], i);
			for (int j = i + 1; j < M; j++)
			{
				if (value.find(operation[j][0]) != value.end())
				{
					value.erase(operation[j][0]);
				}
			}
		}
	}
	void writeResult(string outputh)
	{
		ofstream output(outputh);
		for (int i = 0; i < M; i++)
		{
			switch (value[operation[i][0]])
			{
			case FALSE:
				output << '0' << endl;
				break;
			case TRUE:
				output << '1' << endl;
				break;
			case UNDEFINED:
				output << '?' << endl;
				break;
			}
		}
		output.close();
	}
};


void main(int argc, char* argv[])
{
	setlocale(LC_ALL, "ru");
	if (argc == 3)
	{
		ifstream input(argv[1]);
		if (input.is_open())
		{
			parse file(argv[1]);
			file.count();
			file.writeResult(argv[2]);
		}
		else
		{
			cout << argv[1] << " не найден!";
		}
		input.close();
	}
	else
	{
		cout << "Необходимо указать два аргумента!";
	}
}
