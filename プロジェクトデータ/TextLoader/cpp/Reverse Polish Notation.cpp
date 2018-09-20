#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>
#include <fstream>
#include <iomanip>
#include "..\header\Reverse Polish Notation.h"

ReversePolishNotation::ReversePolishNotation()
{
	isConvert = false;
}

ReversePolishNotation::~ReversePolishNotation()
{
	//中身を空に
	while (!formula.empty())
		formula.pop();
}

// ソート関数※
// 引数
// out:受け取り用スタック(受け取り用なのではじめに中身を空にしてある)
// in :ソート元データ(ソート後中身は無くなる)
// ※inの逆順に格納しなおす関数
// 参考)0 1 2 3	→ 3 2 1 0
template<typename type>
void ReversePolishNotation::Sort(
	std::stack<type> *out,
	std::stack<type> *in)
{
	// in と out が別の参照先であれば
	//outを空にしてoutにinを格納する
	if (out != in)
	{
		while (!out->empty())
			out->pop();

		while (!in->empty())
			out->push(Pop(in));
	}
	//同じ参照先であればoutを空にするとinも空になってしまうので
	//コピー用変数tmpに格納してからそれをoutへ代入
	else
	{
		std::stack<type> tmp;
		while (!in->empty())
			tmp.push(Pop(in));

		*out = tmp;
	}
}


//先頭データ取得後にポップする
template<typename type>
type ReversePolishNotation::Pop(std::stack<type>* stack)
{
	type top = stack->top();
	stack->pop();
	return top;
}

void ReversePolishNotation::Push(
	std::stack<char>* stack,
	std::string* str)
{
	//文字列に入っている数字を1文字ずつ格納
	for (size_t i = 0; i < str->size(); i++)
	{
		if (str->at(i) == ' ')
			continue;
		stack->push(str->at(i));
	}
	//値の変わり目の目印記号
	stack->push(' ');

	//読込文字列リセット
	*str = "";
}


// '+'記号時の処理
void ReversePolishNotation::Plus(
	std::stack<char>* mainStack,
	std::stack<char>* subStack,
	std::string* str,
	char ch)
{
	char op = 0;
	//メインスタックに値を格納
	if (!str->empty())
		Push(mainStack, str);

	//サブスタックに中身があれば '+' か '-'までの記号を取得
	while (!subStack->empty())
	{
		op = subStack->top();

		if (op == ' ')
			Pop(subStack);
		//直前の記号が'*' , '/', '^' ,'-'の場合はそっちの方が優先順位が高いので
		//メインスタックに直前の演算子を移し替える
		if (op == '*' || op == '/' || op == '^' || op == '-')
		{
			//直前の演算子はすでに取得済みなのでそのままポップしている
			subStack->pop();

			//メインスタックに直前の演算子を移し替えた
			mainStack->push(op);
			mainStack->push(' ');
		}
		// '+', '-'のときループ抜ける
		else
			break;
	}

	//サブスタックに最新の演算子を格納
	subStack->push(ch);
}


// '-'記号時の処理
void ReversePolishNotation::Minus(
	std::stack<char>* mainStack,
	std::stack<char>* subStack,
	std::string * str,
	const char* infixNotation,
	int index)
{
	if (index == 0)
	{
		*str += "-";
		return;
	}

	for (int reverseIndex = index - 1; reverseIndex > -1; reverseIndex--)
	{
		char ch = infixNotation[reverseIndex];
		//空白はスルー
		if (ch == ' ')
			continue;

		// '-' の直前が数字かまたは括弧式の終わりかどうかか初めか

		if ('0' <= ch && ch <= '9' || ch == ')' || ch == 0 || ('a' <= ch && ch <= 'z') || ('A' <= ch && ch <= 'Z'))
		{
			// 演算子としての-の場合も+とは変わらないため
			// +の時に呼び出すメソッドをここで呼び出している
			Plus(mainStack, subStack, str, infixNotation[index]);
			return;
		}
		else
		{
			//マイナス値としての '-' 記号
			*str += '-';
			return;
		}
	}
}

// '*' 記号
void ReversePolishNotation::Asterisk(
	std::stack<char>* mainStack,
	std::stack<char>* subStack,
	std::string * str,
	char ch)
{
	if (!str->empty())
		Push(mainStack, str);

	while (!subStack->empty())
	{
		char op = subStack->top();
		// 累乗の方が優先順位が高いので'^'を先にメインスタックへ入れてる
		if (op == '^' || op == '/')
		{
			mainStack->push(Pop(subStack));
			mainStack->push(' ');
		}
		else
			break;
	}

	subStack->push(ch);
}

// '/' 記号
void ReversePolishNotation::Slash(
	std::stack<char>* mainStack,
	std::stack<char>* subStack,
	std::string * str,
	char ch)
{
	Asterisk(mainStack, subStack, str, ch);
}

// '^'記号(累乗)
void ReversePolishNotation::Carat(
	std::stack<char>* mainStack,
	std::stack<char>* subStack,
	std::string* str,
	char ch)
{
	if (!str->empty())
		Push(mainStack, str);
	subStack->push(ch);
}

// 括弧部の処理(再帰関数なので少し危険かもしれない)
// 引数
// mainStack:全体式のスタック
// formula  :変換前の式(不変)
// index	:式の読み取り場所のインデックス
// (括弧から抜けるまでこの関数で一気に読み込んでしまうため抜けた後の次参照を記憶させるためポインターにしてある)
// ※引数は値渡しで返値に次インデックスを返してあげてもよかったが…
void ReversePolishNotation::Brackets(
	std::stack<char>* parentStack,
	const char* formula,
	int* index,
	const int length)
{
	std::stack<char> mStack;//括弧内の式格納用メインスタック
	std::stack<char> sStack;//括弧内の演算子格納用サブスタック
	std::string str = "";	//文字列読込用変数

	*index += 1;
	char ch = formula[*index];
	while (ch != ')')
	{
		switch (ch)
		{
		case '+':
			Plus(&mStack, &sStack, &str, ch);
			break;
		case '-':
			Minus(&mStack, &sStack, &str, formula, *index);
			break;
		case '*':
			Asterisk(&mStack, &sStack, &str, formula[*index]);
			break;
		case '/':
			Slash(&mStack, &sStack, &str, formula[*index]);
			break;
		case '(':
			//再帰呼び出し
			//(多重括弧の時に入り')'までくるとひとつ前の階層へ戻る)
			Brackets(&mStack, formula, index, length);
			break;
		case '^':
			Carat(&mStack, &sStack, &str, formula[*index]);
			break;
		case '.':
			str += ch;
			break;
		default:
			str += ch;
			break;
		}

		//todo 時機にやる
		/*if (str == "sin")
			Sin(&mStack, formula, index);
		else if (str == "cos")
			Cos(&mStack, formula, index);
		else if (str == "tan")
			Tan(&mStack, formula, index);
		else*/

		*index += 1;

		if (*index >= length)
			break;

		ch = formula[*index];
	}

	//')'終了直前の値の格納
	if (!str.empty())
		Push(&mStack, &str);

	//サブスタックの中身をメインスタック(括弧内の式への連結)へ
	while (!sStack.empty())
	{
		mStack.push(Pop(&sStack));
		mStack.push(' ');
	}

	//ソート実行
	Sort(&mStack, &mStack);

	//親スタックへデータを渡す
	while (!mStack.empty())
		parentStack->push(Pop(&mStack));
}

void ReversePolishNotation::Sin(std::stack<char>* mainStack, const char * formula, int * index)
{
	std::string str;
	char ch;

	*index += 1;
	ch = formula[*index];

	do
	{
		str += ch;
		*index += 1;
		ch = formula[*index];
	} while ('0' <= ch && ch <= '9' || ch == '.');

	Push(mainStack, &str);
}

void ReversePolishNotation::Cos(std::stack<char>* mainStack, const char * formula, int * index)
{
	Sin(mainStack, formula, index);
}

void ReversePolishNotation::Tan(std::stack<char>* mainStack, const char * formula, int * index)
{
	Sin(mainStack, formula, index);
}

void ReversePolishNotation::Equal(std::stack<char>* mainStack, std::stack<char>* subStack, std::string* str)
{
	Push(mainStack, str);
	subStack->push('=');
}

// 中置記法から逆ポーランド記法への変換関数
// 引数(infixNotation):式の文字列
void ReversePolishNotation::ConvertToReversePorland(char* infixNotation)
{
	if (isConvert)
		return;

	//mainStack:式全体を格納するスタック,subStack:演算子を格納するスタック
	std::stack<char> mainStack, subStack;

	//式の文字数取得
	int len = strlen(infixNotation);
	std::string str;

	for (int i = 0; i < len;)
	{
		switch (infixNotation[i])
		{
		case '+':
			Plus(&mainStack, &subStack, &str, infixNotation[i]);
			break;
		case '-':
			Minus(&mainStack, &subStack, &str, infixNotation, i);
			break;
		case '*':
			Asterisk(&mainStack, &subStack, &str, infixNotation[i]);
			break;
		case '/':
			Slash(&mainStack, &subStack, &str, infixNotation[i]);
			break;
		case '=':
			Equal(&mainStack, &subStack, &str);
			break;
		case '(':
			Brackets(&mainStack, infixNotation, &i, len);
			break;
		case '^':
			Carat(&mainStack, &subStack, &str, infixNotation[i]);
			break;
		default:
			str += infixNotation[i];
			break;
		}

		//todo 時機にやる
		/*if (str == "sin")
			Sin(&mainStack, infixNotation, &i);
		else if (str == "cos")
			Cos(&mainStack, infixNotation, &i);
		else if (str == "tan")
			Tan(&mainStack, infixNotation, &i);
		else*/
		i++;
	}


	//最終項をスタックに格納
	if (!str.empty())
		Push(&mainStack, &str);

	//サブスタックに残っている演算子をすべてスタックへ
	while (!subStack.empty())
	{
		mainStack.push(Pop(&subStack));
		mainStack.push(' ');
	}

	//ソート実行
	Sort(&formula, &mainStack);

	auto tmp = formula;
	str = "";
	while (!tmp.empty())
		str += Pop(&tmp);

	formulaString = str;

	isConvert = true;
}

void ReversePolishNotation::ConvertToReversePorland(const char* infixNotation)
{
	ConvertToReversePorland((char*)infixNotation);
}
//*******************************************************************************
//
//		計算部分
//
//*******************************************************************************
float ReversePolishNotation::Calculation(const char * infix)
{
	return Calculation((char*)infix);
}

float ReversePolishNotation::Calculation(char * infix)
{
	ConvertToReversePorland(infix);

	std::stack<float> stack;
	std::string str = "";
	std::string key = "";
	float ans = 0.0f;
	bool isVariable = true;
	while (!formula.empty())
	{
		char op = Pop(&formula);

		switch (op)
		{
		case '+':
			Addition(&stack);
			break;
		case '-':
			Subtraction(&stack);
			break;
		case '*':
			Multiply(&stack);
			break;
		case '/':
			Division(&stack);
			break;
		case '^':
			Power(&stack);
			break;
		case '=':
			Substitution(&stack, &key);
			break;
		case ' ':
			if (str.empty())
				break;
			if (!isVariable)
			{
				stack.push(atof(str.c_str()));
				isVariable = true;
			}
			else
			{
				//検索してなければ新規登録
				auto it = variableMaps.find(str);
				if (it == variableMaps.end() || key.empty())
				{
					variableMaps.emplace(str, 0);
					key = str;
				}
				else
					stack.push(it->second);

			}
			str = "";
			break;
		default:
			//頭文字が数字なら値
			if (str.empty() && '0' <= op && op <= '9')
				isVariable = false;
			str += op;
			break;
		}


		if (str == "sin")
		{
			Sinf(&stack);
			str = "";
		}
		else if (str == "cos")
		{
			Cosf(&stack);
			str = "";
		}
		else if (str == "tan")
		{
			Tanf(&stack);
			str = "";
		}

	}

	if (!stack.empty())
		ans = Pop(&stack);
	isConvert = false;
	return ans;
}

//**************************************************************************************************************
//			加算
void ReversePolishNotation::Addition(std::stack<float>* stack)
{
	float left, right;

	right = Pop(stack);
	left = Pop(stack);

	stack->push(left + right);
}

//***********************************************************************************************
//			減算
void ReversePolishNotation::Subtraction(std::stack<float>* stack)
{
	float left, right;
	char op = formula.top();

	//次文字が' 'であれば演算子としての'-'ということなる
	if (op != ' ')
	{
		std::string str = "-";
		while (op != ' ')
		{
			op = Pop(&formula);
			str += op;
		}

		float val = (float)atof(str.c_str());
		stack->push(val);
	}
	else
	{
		right = Pop(stack);
		left = Pop(stack);
		stack->push(left - right);
	}
}

//******************************************************************************************
//			乗算
void ReversePolishNotation::Multiply(std::stack<float>* stack)
{
	float left, right;

	right = Pop(stack);
	left = Pop(stack);

	stack->push(left * right);
}

//*****************************************************************************************
void ReversePolishNotation::Division(std::stack<float>* stack)
{
	float left, right;

	right = Pop(stack);
	left = Pop(stack);

	//ゼロ除算エラー
	if (right == 0)
		stack->push(0);

	stack->push(left / right);
}


//累乗
void ReversePolishNotation::Power(std::stack<float>* stack)
{
	float base, exponent;

	exponent = Pop(stack);
	base = Pop(stack);

	stack->push(pow(base, exponent));
}

//todo
//サイン
void ReversePolishNotation::Sinf(std::stack<float>* stack)
{
	stack->push(sinf(Pop(stack)));
}

//todo
//コサイン
void ReversePolishNotation::Cosf(std::stack<float>* stack)
{
	stack->push(cosf(Pop(stack)));
}

//todo
//タンジェント
void ReversePolishNotation::Tanf(std::stack<float>* stack)
{
	stack->push(tanf(Pop(stack)));
}

//代入
void ReversePolishNotation::Substitution(std::stack<float>* stack, std::string* str)
{
	auto& it = variableMaps.find(*str);
	it->second = stack->top();
}


//変数リセット
void ReversePolishNotation::Reset()
{
	variableMaps.clear();
}


//ファイルに出力
void ReversePolishNotation::OutputToFile(char* filename, const int mode)
{
	std::ofstream file(filename, mode);
	if (file)
	{
		std::string str = "";
		for (auto it : variableMaps)
		{
			str = it.first;
			str += " = ";
			char buf[16];
			sprintf(buf, "%15.4f", it.second);
			str += buf;
			str += "\n";
			file.write(str.c_str(), str.size());
		}
	}
}


//todo 未完成
void ReversePolishNotation::GetVariables(char* key[], float* values[])
{
	variableMaps.size();

	int index = 0;

	auto vkey = std::vector<char*>(variableMaps.size());
	auto vvalues = std::vector<float>(variableMaps.size());

	for (auto it : variableMaps)
	{
		int size = strlen(it.first.c_str());

		vkey[index] = new char[size];

		strcpy(vkey[index], it.first.c_str());

		vvalues[index] = it.second;
		index++;
	}

	key = vkey.data();
	*values = vvalues.data();
}

