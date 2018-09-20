#include <Windows.h>
#include "Text Loader.h"

TextLoader::TextLoader()
{

	strBuffer = "";
	latestChar = 0;
	charBuffer = 0;
}

//�R���X�g���N�^
TextLoader::TextLoader(char* filename)
{
	Open(filename);
}

//�f�X�g���N�^
TextLoader::~TextLoader()
{
	if (isOpened)
		file.close();

	strBuffer.clear();
	latestChar = 0;
	charBuffer = 0;
}

bool TextLoader::Open(char * filename)
{
	file.open(filename);

	//���[�h�������`�F�b�N
	if (file)
	{
		std::string str = filename;
		str += " is opened.\n";
		OutputDebugStringA(str.c_str());

		//�����ꏊ�̓o�^
		firstPos = nowPos = file.tellg();

		//�����ֈړ���o�^
		file.seekg(0, ios::end);
		lastPos = file.tellg();

		//�擪�֖߂�
		file.clear();
		file.seekg(ios::beg);

		isOpened = true;
		return true;
	}
	else
	{
		std::string str = filename;
		str += " is opened failure.\n";
		OutputDebugStringA(str.c_str());
		return false;
	}
}

void TextLoader::Close()
{
	file.close();
	isOpened = false;
}

//�擾���������ɑ΂��Ă̏�������
bool TextLoader::CompareChar()
{
	strBuffer.clear();
	while (1)
	{
		charBuffer = latestChar;
		GetChar();
		if (IsEndOfFile())
			break;
		//�R�����g��������
		CompareComment();

		switch (latestChar)
		{
		case ' ':
			continue;

		case '\n':
			continue;

		case ',':
			break;

		case ':':
			break;

		case ';':
			break;

		default:
			strBuffer += latestChar;
			continue;
		}
		break;
	}
	//������i�[�̈�Ƀf�[�^�������
	if (strBuffer != "")
		return true;
	return false;
}

//�R�����g������r
void TextLoader::CompareComment()
{
	//�ꕶ���ڂ� '/' �̂Ƃ� 
	if (latestChar == '/')
	{
		fstream::pos_type pos = file.tellg();
		char tmp = latestChar;
		GetChar();

		//2�����ڂ� '/' �̂Ƃ�
		if (latestChar == '/')
		{
			//���s�����܂œǂݔ�΂�
			while (GetChar() != '\n')
			{
				//�������Ȃ�
			}
		}

		//'*' �̂Ƃ�
		else if (latestChar == '*')
		{
			char mark = 0;
			//�@"*/"�܂œǂݔ�΂�
			while (!(mark == '*' && latestChar == '/'))
			{
				//�ŐV�����擾�O�̕������
				mark = latestChar;
				GetChar();
			}

			GetChar();
		}
		//����ȊO
		else
		{
			//���̏ꏊ�ɖ߂�
			file.seekg(pos, ios::beg);
			latestChar = tmp;
		}
	}
}

//long�^�̒l�擾
long TextLoader::GetLong()
{
	bool flg = CompareChar();
	long val = LONG_MAX;
	if (flg)
		val = (long)atol(strBuffer.c_str());
	return val;
}

//float�^�̒l�擾
float TextLoader::GetFloat()
{
	bool flg = CompareChar();
	float val = FLT_MAX;

	val = rpn.Calculation(strBuffer.c_str());

	return val;
}

//int�^�̒l�擾
int TextLoader::GetInt()
{
	bool flg = CompareChar();
	int val = INT_MAX;
	val = (int)rpn.Calculation(strBuffer.c_str());
	return val;
}

//������擾
char* TextLoader::GetString()
{
	bool flg = CompareChar();

	if (flg)
		return (char*)strBuffer.c_str();
	else
		return "";
}

//1�����擾
char TextLoader::GetChar()
{
	if (!IsEndOfFile())
	{
		//�t�@�C�����當���擾
		latestChar = file.get();
		nowPos = file.tellg();
		return latestChar;
	}
	else
		return 0;
}

char * TextLoader::GetLine()
{
	return nullptr;
}

char* TextLoader::GetAllData()
{
	

	while (!IsEndOfFile())
	{
		strBuffer += file.get();
	}

	int last = strBuffer.size() - 1;
	strBuffer[last] = 0;
	return (char*)strBuffer.c_str();
}

bool TextLoader::IsEndOfFile()
{
	return file.eof();
}