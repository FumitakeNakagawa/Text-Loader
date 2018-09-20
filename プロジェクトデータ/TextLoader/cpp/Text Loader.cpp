#include <Windows.h>
#include "Text Loader.h"

TextLoader::TextLoader()
{

	strBuffer = "";
	latestChar = 0;
	charBuffer = 0;
}

//コンストラクタ
TextLoader::TextLoader(char* filename)
{
	Open(filename);
}

//デストラクタ
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

	//ロード成功かチェック
	if (file)
	{
		std::string str = filename;
		str += " is opened.\n";
		OutputDebugStringA(str.c_str());

		//初期場所の登録
		firstPos = nowPos = file.tellg();

		//末尾へ移動後登録
		file.seekg(0, ios::end);
		lastPos = file.tellg();

		//先頭へ戻す
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

//取得した文字に対しての条件分岐
bool TextLoader::CompareChar()
{
	strBuffer.clear();
	while (1)
	{
		charBuffer = latestChar;
		GetChar();
		if (IsEndOfFile())
			break;
		//コメント文か判定
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
	//文字列格納領域にデータがあれば
	if (strBuffer != "")
		return true;
	return false;
}

//コメント文か比較
void TextLoader::CompareComment()
{
	//一文字目が '/' のとき 
	if (latestChar == '/')
	{
		fstream::pos_type pos = file.tellg();
		char tmp = latestChar;
		GetChar();

		//2文字目が '/' のとき
		if (latestChar == '/')
		{
			//改行文字まで読み飛ばし
			while (GetChar() != '\n')
			{
				//何もしない
			}
		}

		//'*' のとき
		else if (latestChar == '*')
		{
			char mark = 0;
			//　"*/"まで読み飛ばし
			while (!(mark == '*' && latestChar == '/'))
			{
				//最新文字取得前の文字代入
				mark = latestChar;
				GetChar();
			}

			GetChar();
		}
		//それ以外
		else
		{
			//元の場所に戻す
			file.seekg(pos, ios::beg);
			latestChar = tmp;
		}
	}
}

//long型の値取得
long TextLoader::GetLong()
{
	bool flg = CompareChar();
	long val = LONG_MAX;
	if (flg)
		val = (long)atol(strBuffer.c_str());
	return val;
}

//float型の値取得
float TextLoader::GetFloat()
{
	bool flg = CompareChar();
	float val = FLT_MAX;

	val = rpn.Calculation(strBuffer.c_str());

	return val;
}

//int型の値取得
int TextLoader::GetInt()
{
	bool flg = CompareChar();
	int val = INT_MAX;
	val = (int)rpn.Calculation(strBuffer.c_str());
	return val;
}

//文字列取得
char* TextLoader::GetString()
{
	bool flg = CompareChar();

	if (flg)
		return (char*)strBuffer.c_str();
	else
		return "";
}

//1文字取得
char TextLoader::GetChar()
{
	if (!IsEndOfFile())
	{
		//ファイルから文字取得
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