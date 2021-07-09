// Caclulator.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include <iostream>
#include <string>
#include <vector>
#include <sstream>
#include <stack>

using namespace std;
//字符串转数字
template <class Type>
Type stringToNum(string s) {
    istringstream iss(s);
    Type num;
    iss >> num;
    return num;
}

int main()
{
    string s = "5+3*5-88*(5-5/(2*8))";

    vector<int> vecNum;
    vector<char> vecOper;
    stack<int> stackNum;
    stack<char> stackOper;
    
    auto isNumber = [](char c) {
        if (c >= '0' && c <= '9') {
            return true;
        }
        return false;
    };
    auto isOper = [](char c) {
        switch (c)
        {
        case '+': return true;
        case '-': return true;
        case '*': return true;
        case '/': return true;
        }
        return false;
    };
 

    string lastNum;
    bool lastIsOper = false;
    for (int i = 0; i < s.size(); ++i) {

        if (isNumber(s[i])) {
            lastNum.push_back(s[i]);
            lastIsOper = false;
        }
        else if (isOper(s[i])) {
            if (lastIsOper) {
                cout << "fmt is error\n";
                break;
            }
            //vecNum.push_back(stringToNum<int>(lastNum));
            vecNum.push_back(stoi(lastNum));
            stackNum.push(stoi(lastNum));
            lastNum.clear();
            vecOper.push_back(s[i]);
            stackOper.push(s[i]);
            lastIsOper = true;
        }
        else if (s[i] == '(') {

        }
        else if (')' == s[i]) {

        }
        else {
            if (s[i] != ' ') {
                return false;
            }
        }
    }
    if (lastNum.size() > 0) {
        vecNum.push_back(stoi(lastNum));
        stackNum.push(stoi(lastNum));
    }
    for (auto num : vecNum) {
        cout << num << " ";
    }
    cout << "\n";
    for (auto oper : vecOper) {
        cout << oper << " ";
    }
    cout << "\n";

    while (stackNum.size()) {
		cout << stackNum.top() << " ";
		stackNum.pop();
    }

	cout << "\n";
	while (stackOper.size()) {
		cout << stackOper.top() << " ";
        stackOper.pop();
	}
	cout << "\n";
    std::cout << "Hello World!\n";
}

// 运行程序: Ctrl + F5 或调试 >“开始执行(不调试)”菜单
// 调试程序: F5 或调试 >“开始调试”菜单

// 入门使用技巧: 
//   1. 使用解决方案资源管理器窗口添加/管理文件
//   2. 使用团队资源管理器窗口连接到源代码管理
//   3. 使用输出窗口查看生成输出和其他消息
//   4. 使用错误列表窗口查看错误
//   5. 转到“项目”>“添加新项”以创建新的代码文件，或转到“项目”>“添加现有项”以将现有代码文件添加到项目
//   6. 将来，若要再次打开此项目，请转到“文件”>“打开”>“项目”并选择 .sln 文件
