﻿// Caclulator.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include <iostream>
#include <string>
#include <vector>
#include <sstream>
#include <stack>
#include <deque>
#include <queue>

using namespace std;
////字符串转数字
//template <class Type>
//Type stringToNum(string s) {
//	istringstream iss(s);
//	Type num;
//	iss >> num;
//	return num;
//}
enum DataType {
	DT_Num = 0,
	DT_Oper
};
struct OperData {
	DataType type;
	int data;
};
int getOperPriority(char oper) {
	switch (oper)
	{
	case '+':
	case '-':
		return 0;
	case '*':
	case '/':
		return 1;
	default:
		break;
	}
	return -1;
}
bool isNumber(char c) {
	if (c >= '0' && c <= '9') {
		return true;
	}
	return false;
}

bool isOper(char c) {
	switch (c)
	{
	case '+': return true;
	case '-': return true;
	case '*': return true;
	case '/': return true;
	}
	return false;
}

bool parseToMidOrder(string s, vector<OperData> &midOrder, string &err) {
	
	string lastNum;
	bool lastIsOper = false;
	int leftBracketNum = 0;
	int rightBracketNum = 0;
	for (int i = 0; i < s.size(); ++i) {
		if (isNumber(s[i])) {
			lastNum.push_back(s[i]);
			lastIsOper = false;
		}
		else if (isOper(s[i])) {
			if (lastIsOper) {
				err = "two oper can't back to back";
				midOrder.clear();
				return false;
			}
			if (!lastNum.empty())
			{
				midOrder.push_back({ DT_Num, stoi(lastNum) });
				lastNum.clear();
			}
			midOrder.push_back({ DT_Oper, s[i] });
			lastIsOper = true;
		}
		else if ('(' == s[i]) {
			midOrder.push_back({ DT_Oper, s[i] });
			leftBracketNum++;
			
		}
		else if (')' == s[i]) {
			if (lastIsOper) {
				err = "two oper can't back to back";
				midOrder.clear();
				return false;
			}

			rightBracketNum++;
			if (!lastNum.empty())
			{
				midOrder.push_back({ DT_Num, stoi(lastNum) });
				lastNum.clear();
			}
			midOrder.push_back({ DT_Oper, s[i] });
		}
		else if (s[i] != ' ' || s[i] != '\t') {
			err = "find invalid character";
			midOrder.clear();
			return false;			
		}
	}
	if (leftBracketNum != rightBracketNum) {
		err = "bracket num is not match";
		midOrder.clear();
		return false;
	}
	if (lastNum.size() > 0) {
		midOrder.push_back({ DT_Num, stoi(lastNum) });
	}
	
	return true;
}
double calcTwoNum(char oper, double first, double second) {
	switch (oper)
	{		
	case '+': return first + second;
	case '-': return first - second;
	case '*': return first * second;
	case '/': return first / second;
	}
	return 0;
}
//计算
double calculate(const deque<OperData>& backOrder) {
	stack<double> stackToCalc;
	for (auto data : backOrder) {
		if (DT_Oper == data.type) {
			if (stackToCalc.size() < 2) {
				return 0;
			}
			double secondNum = stackToCalc.top();
			stackToCalc.pop();
			double firstNum = stackToCalc.top();
			stackToCalc.pop();
			double result = calcTwoNum(static_cast<char>(data.data), firstNum, secondNum);
			stackToCalc.push(result);
		}
		else if (DT_Num == data.type) {
			stackToCalc.push(data.data);
		}
	}
	return stackToCalc.top();
}
void transToBackOrder(const vector<OperData>& midOrder, deque<OperData> &backOrder){
	stack<char> stackOperator;
	for (int i = 0; i < midOrder.size(); ++i) {
		if (DT_Num == midOrder[i].type) {
			backOrder.push_back(midOrder[i]);
		}
		else if(DT_Oper == midOrder[i].type){
			if (isOper(static_cast<char>(midOrder[i].data)))
			{
				if (stackOperator.empty()) {
					stackOperator.push(static_cast<char>(midOrder[i].data));
				}
				else {
					if (stackOperator.top() == '(') {
						stackOperator.push(static_cast<char>(midOrder[i].data));
					}
					else {
						while (stackOperator.size()) {
							//栈顶符号比读出的符号优先级高，栈顶出栈，保存到队列中
							if (getOperPriority(stackOperator.top()) >= getOperPriority(static_cast<char>(midOrder[i].data))) {
								backOrder.push_back({ DT_Oper, stackOperator.top() });
								stackOperator.pop();
							}
							else {
								//栈顶符号比读出的符号优先给低，将当前符号压入框中
								stackOperator.push(static_cast<char>(midOrder[i].data));
								break;
							}
						}
						if (0 == stackOperator.size()) {
							stackOperator.push(static_cast<char>(midOrder[i].data));
						}
					}
				}
			}
			else if ('(' == static_cast<char>(midOrder[i].data)) {
				stackOperator.push('(');
			}
			else if (')' == static_cast<char>(midOrder[i].data)) {
				while (stackOperator.size()) {
					if ('(' == stackOperator.top()) {
						stackOperator.pop();
						break;
					}
					backOrder.push_back({ DT_Oper, stackOperator.top() });
					stackOperator.pop();
				}
			}
		}
	}
	while (stackOperator.size()) {
		backOrder.push_back({ DT_Oper, stackOperator.top() });
		stackOperator.pop();
	}
}
int main()
{
	//string s = "(5+3*5-88*(5-5/(2*8)))";
	string s = "(1+1*1-1*(1-1/(1*1)))";
	//5 3 5 * + 88 5 5 2 8 * / - * -
	vector<OperData> midOrder;
	string error;
	bool err = parseToMidOrder(s, midOrder, error);
	if (!err) {
		cout << error << endl;
	}
	for (auto data : midOrder) {
		if (data.type == DT_Num) {
			cout << data.data << ' ';
		}
		else if (data.type == DT_Oper) {
			char oper = data.data;
			cout << oper << ' ';
		}
	}
	std::cout << "\n";

	deque<OperData> backOrder;
	transToBackOrder(midOrder, backOrder);
	for (auto data : backOrder) {
		if (data.type == DT_Num) {
			cout << data.data << ' ';
		}
		else if (data.type == DT_Oper) {
			char oper = data.data;
			cout << oper << ' ';
		}
	}
	std::cout << "\n";

	std::cout << "result:" << calculate(backOrder);
}

