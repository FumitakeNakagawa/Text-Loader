#pragma once
#include <stack>
#include <map>
#include <vector>

class ReversePolishNotation
{
private:
	std::string formulaString;
	std::stack<char> formula;

	std::map<std::string, float> variableMaps;

	bool isConvert;


	template<typename type>
	void Sort(std::stack<type> *out, std::stack<type>* in);


	template<typename type>
	type Pop(std::stack<type>* stack);


	void Push(std::stack<char>* stack, std::string* str);

	//変換に使う関数群
	void Plus(std::stack<char>* mainStack, std::stack<char>* subStack, std::string* str, char ch);
	void Minus(std::stack<char>* mainStack, std::stack<char>* subStack, std::string* str, const char* infixNotation, int index);
	void Asterisk(std::stack<char>* mainStack, std::stack<char>* subStack, std::string*str, char ch);
	void Slash(std::stack<char>* mainStack, std::stack<char>* subStack, std::string* str, char ch);
	void Carat(std::stack<char>* mainStack, std::stack<char>* subStack, std::string* str, char ch);
	void Brackets(std::stack<char>* mainStack, const char* formula, int* index, const int length);
	void Sin(std::stack<char>* mainStack, const char* formula, int* index);
	void Cos(std::stack<char>* mainStack, const char* formula, int* index);
	void Tan(std::stack<char>* mainStack, const char* formula, int* index);
	void Equal(std::stack<char>* mainStack, std::stack<char>*subStack, std::string* str);

	//計算に使う関数群
	//加算
	void Addition(std::stack<float>* stack);

	//減算
	void Subtraction(std::stack<float>* stack);

	//乗算
	void Multiply(std::stack<float>* stack);

	//除算
	void Division(std::stack<float>* stack);

	//代入
	void Substitution(std::stack<float>* stack, std::string* str);

	//累乗
	void Power(std::stack<float>* stack);
	//sin関数
	void Sinf(std::stack<float>* stack);
	//cos関数
	void Cosf(std::stack<float>* stack);
	//tan関数
	void Tanf(std::stack<float>* stack);

public:

	ReversePolishNotation();
	~ReversePolishNotation();

	const char* GetFormulaString() { return formulaString.c_str(); }

	//一般式から逆ポーランド記法へ変換
	void ConvertToReversePorland(char* infixNotation);
	void ConvertToReversePorland(const char* infixNotation);

	//逆ポーランド記法を元に計算
	float Calculation(const char* infix);
	float Calculation(char* infix);


	void Reset();
	void OutputToFile(char* filename, const int mode = std::ios::out | std::ios::binary);

	void GetVariables(char* key[], float* values[]);
	std::map<std::string, float>* GetVariableMaps() { return &variableMaps; }
};