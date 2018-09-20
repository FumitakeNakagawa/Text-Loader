#pragma once
#include <fstream>
#include "Reverse Polish Notation.h"

using namespace std;

class TextLoader
{
private:

	ReversePolishNotation rpn;//逆ポーランド記法で計算するためのクラス
	fstream file;

	fstream::pos_type lastPos; //最終位置
	fstream::pos_type firstPos;//初期位置
	fstream::pos_type nowPos;  //現在位置

	string strBuffer;		   //テキストデータ受け取り用変数

	char charBuffer; //読み込み直前の値
	char latestChar; //最後に読み込んだ値

	bool isOpened;

	//取得した文字に対しての条件分岐
	bool CompareChar();

	//コメント文か比較
	void CompareComment();
	
public:

	TextLoader();

	//コンストラクタ(ファイルオープン込)
	TextLoader(char* filename);

	//デストラクタ(ファイルクローズ込)
	~TextLoader();

	//ファイルオープン
	bool Open(char* filename);

	//ファイルクローズ
	void Close();

	//long型の値取得
	long GetLong();

	//float型の値取得
	float GetFloat();

	//int型の値取得
	int GetInt();

	//文字列取得
	char* GetString();

	//char型の値(文字)取得
	char GetChar();

	//1行取得
	char* GetLine();

	char* GetAllData();
	//ファイルの最後に到達したか判定
	bool IsEndOfFile();

	ReversePolishNotation* GetReversePolishNotation() { return &rpn; }
};

