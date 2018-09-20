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

	//�ϊ��Ɏg���֐��Q
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

	//�v�Z�Ɏg���֐��Q
	//���Z
	void Addition(std::stack<float>* stack);

	//���Z
	void Subtraction(std::stack<float>* stack);

	//��Z
	void Multiply(std::stack<float>* stack);

	//���Z
	void Division(std::stack<float>* stack);

	//���
	void Substitution(std::stack<float>* stack, std::string* str);

	//�ݏ�
	void Power(std::stack<float>* stack);
	//sin�֐�
	void Sinf(std::stack<float>* stack);
	//cos�֐�
	void Cosf(std::stack<float>* stack);
	//tan�֐�
	void Tanf(std::stack<float>* stack);

public:

	ReversePolishNotation();
	~ReversePolishNotation();

	const char* GetFormulaString() { return formulaString.c_str(); }

	//��ʎ�����t�|�[�����h�L�@�֕ϊ�
	void ConvertToReversePorland(char* infixNotation);
	void ConvertToReversePorland(const char* infixNotation);

	//�t�|�[�����h�L�@�����Ɍv�Z
	float Calculation(const char* infix);
	float Calculation(char* infix);


	void Reset();
	void OutputToFile(char* filename, const int mode = std::ios::out | std::ios::binary);

	void GetVariables(char* key[], float* values[]);
	std::map<std::string, float>* GetVariableMaps() { return &variableMaps; }
};