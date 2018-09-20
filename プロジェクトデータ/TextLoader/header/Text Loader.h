#pragma once
#include <fstream>
#include "Reverse Polish Notation.h"

using namespace std;

class TextLoader
{
private:

	ReversePolishNotation rpn;//�t�|�[�����h�L�@�Ōv�Z���邽�߂̃N���X
	fstream file;

	fstream::pos_type lastPos; //�ŏI�ʒu
	fstream::pos_type firstPos;//�����ʒu
	fstream::pos_type nowPos;  //���݈ʒu

	string strBuffer;		   //�e�L�X�g�f�[�^�󂯎��p�ϐ�

	char charBuffer; //�ǂݍ��ݒ��O�̒l
	char latestChar; //�Ō�ɓǂݍ��񂾒l

	bool isOpened;

	//�擾���������ɑ΂��Ă̏�������
	bool CompareChar();

	//�R�����g������r
	void CompareComment();
	
public:

	TextLoader();

	//�R���X�g���N�^(�t�@�C���I�[�v����)
	TextLoader(char* filename);

	//�f�X�g���N�^(�t�@�C���N���[�Y��)
	~TextLoader();

	//�t�@�C���I�[�v��
	bool Open(char* filename);

	//�t�@�C���N���[�Y
	void Close();

	//long�^�̒l�擾
	long GetLong();

	//float�^�̒l�擾
	float GetFloat();

	//int�^�̒l�擾
	int GetInt();

	//������擾
	char* GetString();

	//char�^�̒l(����)�擾
	char GetChar();

	//1�s�擾
	char* GetLine();

	char* GetAllData();
	//�t�@�C���̍Ō�ɓ��B����������
	bool IsEndOfFile();

	ReversePolishNotation* GetReversePolishNotation() { return &rpn; }
};

