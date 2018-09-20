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
	//���g�����
	while (!formula.empty())
		formula.pop();
}

// �\�[�g�֐���
// ����
// out:�󂯎��p�X�^�b�N(�󂯎��p�Ȃ̂ł͂��߂ɒ��g����ɂ��Ă���)
// in :�\�[�g���f�[�^(�\�[�g�㒆�g�͖����Ȃ�)
// ��in�̋t���Ɋi�[���Ȃ����֐�
// �Q�l)0 1 2 3	�� 3 2 1 0
template<typename type>
void ReversePolishNotation::Sort(
	std::stack<type> *out,
	std::stack<type> *in)
{
	// in �� out ���ʂ̎Q�Ɛ�ł����
	//out����ɂ���out��in���i�[����
	if (out != in)
	{
		while (!out->empty())
			out->pop();

		while (!in->empty())
			out->push(Pop(in));
	}
	//�����Q�Ɛ�ł����out����ɂ����in����ɂȂ��Ă��܂��̂�
	//�R�s�[�p�ϐ�tmp�Ɋi�[���Ă��炻���out�֑��
	else
	{
		std::stack<type> tmp;
		while (!in->empty())
			tmp.push(Pop(in));

		*out = tmp;
	}
}


//�擪�f�[�^�擾��Ƀ|�b�v����
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
	//������ɓ����Ă��鐔����1�������i�[
	for (size_t i = 0; i < str->size(); i++)
	{
		if (str->at(i) == ' ')
			continue;
		stack->push(str->at(i));
	}
	//�l�̕ς��ڂ̖ڈ�L��
	stack->push(' ');

	//�Ǎ������񃊃Z�b�g
	*str = "";
}


// '+'�L�����̏���
void ReversePolishNotation::Plus(
	std::stack<char>* mainStack,
	std::stack<char>* subStack,
	std::string* str,
	char ch)
{
	char op = 0;
	//���C���X�^�b�N�ɒl���i�[
	if (!str->empty())
		Push(mainStack, str);

	//�T�u�X�^�b�N�ɒ��g������� '+' �� '-'�܂ł̋L�����擾
	while (!subStack->empty())
	{
		op = subStack->top();

		if (op == ' ')
			Pop(subStack);
		//���O�̋L����'*' , '/', '^' ,'-'�̏ꍇ�͂������̕����D�揇�ʂ������̂�
		//���C���X�^�b�N�ɒ��O�̉��Z�q���ڂ��ւ���
		if (op == '*' || op == '/' || op == '^' || op == '-')
		{
			//���O�̉��Z�q�͂��łɎ擾�ς݂Ȃ̂ł��̂܂܃|�b�v���Ă���
			subStack->pop();

			//���C���X�^�b�N�ɒ��O�̉��Z�q���ڂ��ւ���
			mainStack->push(op);
			mainStack->push(' ');
		}
		// '+', '-'�̂Ƃ����[�v������
		else
			break;
	}

	//�T�u�X�^�b�N�ɍŐV�̉��Z�q���i�[
	subStack->push(ch);
}


// '-'�L�����̏���
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
		//�󔒂̓X���[
		if (ch == ' ')
			continue;

		// '-' �̒��O���������܂��͊��ʎ��̏I��肩�ǂ��������߂�

		if ('0' <= ch && ch <= '9' || ch == ')' || ch == 0 || ('a' <= ch && ch <= 'z') || ('A' <= ch && ch <= 'Z'))
		{
			// ���Z�q�Ƃ��Ă�-�̏ꍇ��+�Ƃ͕ς��Ȃ�����
			// +�̎��ɌĂяo�����\�b�h�������ŌĂяo���Ă���
			Plus(mainStack, subStack, str, infixNotation[index]);
			return;
		}
		else
		{
			//�}�C�i�X�l�Ƃ��Ă� '-' �L��
			*str += '-';
			return;
		}
	}
}

// '*' �L��
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
		// �ݏ�̕����D�揇�ʂ������̂�'^'���Ƀ��C���X�^�b�N�֓���Ă�
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

// '/' �L��
void ReversePolishNotation::Slash(
	std::stack<char>* mainStack,
	std::stack<char>* subStack,
	std::string * str,
	char ch)
{
	Asterisk(mainStack, subStack, str, ch);
}

// '^'�L��(�ݏ�)
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

// ���ʕ��̏���(�ċA�֐��Ȃ̂ŏ����댯��������Ȃ�)
// ����
// mainStack:�S�̎��̃X�^�b�N
// formula  :�ϊ��O�̎�(�s��)
// index	:���̓ǂݎ��ꏊ�̃C���f�b�N�X
// (���ʂ��甲����܂ł��̊֐��ň�C�ɓǂݍ���ł��܂����ߔ�������̎��Q�Ƃ��L�������邽�߃|�C���^�[�ɂ��Ă���)
// �������͒l�n���ŕԒl�Ɏ��C���f�b�N�X��Ԃ��Ă����Ă��悩�������c
void ReversePolishNotation::Brackets(
	std::stack<char>* parentStack,
	const char* formula,
	int* index,
	const int length)
{
	std::stack<char> mStack;//���ʓ��̎��i�[�p���C���X�^�b�N
	std::stack<char> sStack;//���ʓ��̉��Z�q�i�[�p�T�u�X�^�b�N
	std::string str = "";	//������Ǎ��p�ϐ�

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
			//�ċA�Ăяo��
			//(���d���ʂ̎��ɓ���')'�܂ł���ƂЂƂO�̊K�w�֖߂�)
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

		//todo ���@�ɂ��
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

	//')'�I�����O�̒l�̊i�[
	if (!str.empty())
		Push(&mStack, &str);

	//�T�u�X�^�b�N�̒��g�����C���X�^�b�N(���ʓ��̎��ւ̘A��)��
	while (!sStack.empty())
	{
		mStack.push(Pop(&sStack));
		mStack.push(' ');
	}

	//�\�[�g���s
	Sort(&mStack, &mStack);

	//�e�X�^�b�N�փf�[�^��n��
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

// ���u�L�@����t�|�[�����h�L�@�ւ̕ϊ��֐�
// ����(infixNotation):���̕�����
void ReversePolishNotation::ConvertToReversePorland(char* infixNotation)
{
	if (isConvert)
		return;

	//mainStack:���S�̂��i�[����X�^�b�N,subStack:���Z�q���i�[����X�^�b�N
	std::stack<char> mainStack, subStack;

	//���̕������擾
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

		//todo ���@�ɂ��
		/*if (str == "sin")
			Sin(&mainStack, infixNotation, &i);
		else if (str == "cos")
			Cos(&mainStack, infixNotation, &i);
		else if (str == "tan")
			Tan(&mainStack, infixNotation, &i);
		else*/
		i++;
	}


	//�ŏI�����X�^�b�N�Ɋi�[
	if (!str.empty())
		Push(&mainStack, &str);

	//�T�u�X�^�b�N�Ɏc���Ă��鉉�Z�q�����ׂăX�^�b�N��
	while (!subStack.empty())
	{
		mainStack.push(Pop(&subStack));
		mainStack.push(' ');
	}

	//�\�[�g���s
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
//		�v�Z����
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
				//�������ĂȂ���ΐV�K�o�^
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
			//�������������Ȃ�l
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
//			���Z
void ReversePolishNotation::Addition(std::stack<float>* stack)
{
	float left, right;

	right = Pop(stack);
	left = Pop(stack);

	stack->push(left + right);
}

//***********************************************************************************************
//			���Z
void ReversePolishNotation::Subtraction(std::stack<float>* stack)
{
	float left, right;
	char op = formula.top();

	//��������' '�ł���Ή��Z�q�Ƃ��Ă�'-'�Ƃ������ƂȂ�
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
//			��Z
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

	//�[�����Z�G���[
	if (right == 0)
		stack->push(0);

	stack->push(left / right);
}


//�ݏ�
void ReversePolishNotation::Power(std::stack<float>* stack)
{
	float base, exponent;

	exponent = Pop(stack);
	base = Pop(stack);

	stack->push(pow(base, exponent));
}

//todo
//�T�C��
void ReversePolishNotation::Sinf(std::stack<float>* stack)
{
	stack->push(sinf(Pop(stack)));
}

//todo
//�R�T�C��
void ReversePolishNotation::Cosf(std::stack<float>* stack)
{
	stack->push(cosf(Pop(stack)));
}

//todo
//�^���W�F���g
void ReversePolishNotation::Tanf(std::stack<float>* stack)
{
	stack->push(tanf(Pop(stack)));
}

//���
void ReversePolishNotation::Substitution(std::stack<float>* stack, std::string* str)
{
	auto& it = variableMaps.find(*str);
	it->second = stack->top();
}


//�ϐ����Z�b�g
void ReversePolishNotation::Reset()
{
	variableMaps.clear();
}


//�t�@�C���ɏo��
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


//todo ������
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

