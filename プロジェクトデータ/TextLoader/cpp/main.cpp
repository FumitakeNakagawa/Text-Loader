#include <windows.h>
#include <iostream>
#include <string>
#include <thread>
#include <strsafe.h>
#include <objbase.h>
#include "..\header\Text Loader.h"

#ifdef EXE
void CalculationFromFile(bool* execute)
{
	std::cout << std::endl;
	std::string filename;

	std::cout << "�t�@�C��������͂��Ă�������:";
	std::cin >> filename;

	TextLoader infix((char*)filename.c_str());

	while (!infix.IsEndOfFile())
	{
		char* str = infix.GetString();
		if (!strlen(str))
			break;

		std::cout << "��:";
		std::cout << str << std::endl;

		auto ans = infix.GetReversePolishNotation()->Calculation(str);

		std::cout << "�t�|�[�����h�L�@:";
		std::cout << infix.GetReversePolishNotation()->GetFormulaString() << std::endl;


		std::cout << "����:";
		std::cout << ans << std::endl;

		std::cout << std::endl;
	}

	//�v�Z���ʃt�@�C���ɏo��
	infix.GetReversePolishNotation()->OutputToFile("DATA\\Variable List.dat", std::ios::out);
	infix.GetReversePolishNotation()->Reset();
	infix.Close();

	*execute = false;
}

void CalculationFromString(bool* execute)
{
	ReversePolishNotation rpn;
	std::cout << std::endl;
	std::cout << "�L�[�{�[�h���̓��[�h���J�n���܂��B" << std::endl;
	std::cout << "Ctrl+Z�ŏI�����܂�" << std::endl << std::endl;;

	while (1)
	{
		std::string infix;
		std::string porland;


		std::cout << "��:";

		for (; EOF == getchar();)
		{
		}
		char line[128] = {0};
		for (int i = 0; i < 127; i++)
		{
			line[i] = getchar();
			if (line[i] == '\n')
			{
				line[i] = '\0';
				break;

			}
			else if (line[i] == EOF)
			{
				*execute = false;
				return;
			}

		}
		rpn.ConvertToReversePorland(line);
		std::cout << "�t�|�[�����h�L�@:" << rpn.GetFormulaString() << std::endl;;

		auto ans = rpn.Calculation(line);
		std::cout << "�v�Z����:" << ans << std::endl;
		std::cout << std::endl;

	}


	*execute = false;
}

void GatVariables(bool* isExecute)
{
	char** key;
	float** values;
	ReversePolishNotation rpn;

	std::cout << std::endl;
	std::cout << "�L�[�{�[�h���̓��[�h���J�n���܂��B" << std::endl;
	std::cout << "Ctrl+Z�ŏI�����܂�" << std::endl << std::endl;;

	while (1)
	{
		std::string infix;
		std::string str = "";
		std::string porland;

		std::cout << "��:";


		char ch[256] = {0};
		std::cin >> ch;

		if (ch[0] == '\0')
			break;

		for (int i = 0; ch[i] != 0; i++)
		{
			str += ch[i];
		}
		if (str.empty())
			continue;

		rpn.ConvertToReversePorland(str.c_str());
		std::cout << "�t�|�[�����h�L�@:" << rpn.GetFormulaString() << std::endl;;

		auto ans = rpn.Calculation(str.c_str());
		std::cout << "�v�Z����:" << ans << std::endl;
		std::cout << std::endl;

	}

	rpn.GetVariables(key, values);

	//for (auto it = rpn.GetVariableMaps()->begin(); it != rpn.GetVariableMaps()->end();++it)

	*isExecute = false;
}


void main()
{
	//�ҋ@���[�h
	std::cout << "Escape�L�[�������ƏI�����܂��B" << std::endl;;

	std::thread thread;
	bool isExecute = false;

	while (1)
	{
		//�����s�����ǂ���
		if (!isExecute)
		{
			//�O��̃��[�h�̃X���b�h�������Ă���Ƃ��͊Ǘ���������Ă���(���)
			if (thread.get_id() != std::thread::id::id())
				thread.detach();

			std::cout << "0:�I��" << std::endl;
			std::cout << "1:�L�[�{�[�h���͂��Čv�Z" << std::endl;
			std::cout << "2:�t�@�C������Ǎ���Ōv�Z" << std::endl;
			std::cout << "���[�h��I�����Ă�������:";


			char ch = getchar();

			switch (ch)
			{
			case '0':
				return;
			case '1':
				thread = std::thread(CalculationFromString, &isExecute);
				isExecute = true;
				break;
			case '2':
				thread = std::thread(CalculationFromFile, &isExecute);
				isExecute = true;
				break;
			case '3':
				thread = std::thread(GatVariables, &isExecute);
				isExecute = true;
				break;
			default:
				std::cout << std::endl;
				continue;
			}


		}


		//Escape�L�[�������ċ����I��
		if (GetKeyState(VK_ESCAPE) < 0)
			break;
	}

	thread.detach();
	return;
}


#elif DLL

BOOL APIENTRY DllMain(HMODULE hModule,
	DWORD  ul_reason_for_call,
	LPVOID lpReserved
)
{

	switch (ul_reason_for_call)
	{
	case DLL_PROCESS_ATTACH:
	case DLL_THREAD_ATTACH:
	case DLL_THREAD_DETACH:
	case DLL_PROCESS_DETACH:
		break;
	}
	return TRUE;
}

#endif