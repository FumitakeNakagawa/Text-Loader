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

	std::cout << "ファイル名を入力してください:";
	std::cin >> filename;

	TextLoader infix((char*)filename.c_str());

	while (!infix.IsEndOfFile())
	{
		char* str = infix.GetString();
		if (!strlen(str))
			break;

		std::cout << "式:";
		std::cout << str << std::endl;

		auto ans = infix.GetReversePolishNotation()->Calculation(str);

		std::cout << "逆ポーランド記法:";
		std::cout << infix.GetReversePolishNotation()->GetFormulaString() << std::endl;


		std::cout << "答え:";
		std::cout << ans << std::endl;

		std::cout << std::endl;
	}

	//計算結果ファイルに出力
	infix.GetReversePolishNotation()->OutputToFile("DATA\\Variable List.dat", std::ios::out);
	infix.GetReversePolishNotation()->Reset();
	infix.Close();

	*execute = false;
}

void CalculationFromString(bool* execute)
{
	ReversePolishNotation rpn;
	std::cout << std::endl;
	std::cout << "キーボード入力モードを開始します。" << std::endl;
	std::cout << "Ctrl+Zで終了します" << std::endl << std::endl;;

	while (1)
	{
		std::string infix;
		std::string porland;


		std::cout << "式:";

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
		std::cout << "逆ポーランド記法:" << rpn.GetFormulaString() << std::endl;;

		auto ans = rpn.Calculation(line);
		std::cout << "計算結果:" << ans << std::endl;
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
	std::cout << "キーボード入力モードを開始します。" << std::endl;
	std::cout << "Ctrl+Zで終了します" << std::endl << std::endl;;

	while (1)
	{
		std::string infix;
		std::string str = "";
		std::string porland;

		std::cout << "式:";


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
		std::cout << "逆ポーランド記法:" << rpn.GetFormulaString() << std::endl;;

		auto ans = rpn.Calculation(str.c_str());
		std::cout << "計算結果:" << ans << std::endl;
		std::cout << std::endl;

	}

	rpn.GetVariables(key, values);

	//for (auto it = rpn.GetVariableMaps()->begin(); it != rpn.GetVariableMaps()->end();++it)

	*isExecute = false;
}


void main()
{
	//待機モード
	std::cout << "Escapeキーを押すと終了します。" << std::endl;;

	std::thread thread;
	bool isExecute = false;

	while (1)
	{
		//今実行中かどうか
		if (!isExecute)
		{
			//前回のモードのスレッドが入っているときは管理を手放している(解放)
			if (thread.get_id() != std::thread::id::id())
				thread.detach();

			std::cout << "0:終了" << std::endl;
			std::cout << "1:キーボード入力して計算" << std::endl;
			std::cout << "2:ファイルから読込んで計算" << std::endl;
			std::cout << "モードを選択してください:";


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


		//Escapeキーを押して強制終了
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