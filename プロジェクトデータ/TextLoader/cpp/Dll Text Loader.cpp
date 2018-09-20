#include <strsafe.h>
#include <objbase.h>
#include "Dll Text Loader.h"
#include "Text Loader.h"
#include "..\header\Dll Text Loader.h"

#ifdef DLL

TextLoader *loader;


DLLTEXTLOADER_API void CreateInstance()
{
	loader = new TextLoader();
}

DLLTEXTLOADER_API void Open(char * filename)
{
	loader->Open(filename);
}


DLLTEXTLOADER_API float GetFloat()
{
	auto infix = loader->GetString();
	return loader->GetReversePolishNotation()->Calculation(infix);
}

DLLTEXTLOADER_API char* GetString()
{
	auto infix = loader->GetString();
	const size_t size = strlen(infix) + 1;
	STRSAFE_LPSTR result = (STRSAFE_LPSTR)CoTaskMemAlloc(size);
	StringCchCopyA(result, size, infix);

	return result;
}

DLLTEXTLOADER_API char * GetAllData()
{
	auto infix = loader->GetAllData();
	const size_t size = strlen(infix) + 1;
	STRSAFE_LPSTR result = (STRSAFE_LPSTR)CoTaskMemAlloc(size);
	StringCchCopyA(result, size, infix);
	return result;
}

DLLTEXTLOADER_API float GetFloatFromString(char * str)
{
	return loader->GetReversePolishNotation()->Calculation(str);
}

DLLTEXTLOADER_API void Clear()
{
	loader->GetReversePolishNotation()->Reset();
}

DLLTEXTLOADER_API void ClearAndOutput(char* outputname)
{
	loader->GetReversePolishNotation()->OutputToFile(outputname);
	loader->GetReversePolishNotation()->Reset();
}

DLLTEXTLOADER_API bool isEOF()
{
	return loader->IsEndOfFile();
}

DLLTEXTLOADER_API void Close()
{
	loader->Close();
}

DLLTEXTLOADER_API void DestroyInstance()
{
	delete loader;
}

DLLTEXTLOADER_API void SaveFile(char * filename, char * data)
{
	std::fstream file(filename, std::ios::out | std::ios::trunc);
	if (!file)
		return;

	std::string str = data;
	file.write(str.c_str(), str.size());
	file.close();
}
DLLTEXTLOADER_API void OutputVariables(char * filename)
{
	return DLLTEXTLOADER_API void();
}
#endif