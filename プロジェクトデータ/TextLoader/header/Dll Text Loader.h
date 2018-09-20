#pragma once

#ifdef DLL

#define DLLTEXTLOADER_EXPORTS
#ifdef DLLTEXTLOADER_EXPORTS
#define DLLTEXTLOADER_API __declspec(dllexport)
#else
#define  DLLTEXTLOADER_API __declspec(dllimport)
#endif

extern "C"
{
	DLLTEXTLOADER_API void CreateInstance();
	DLLTEXTLOADER_API void Open(char* filename);
	DLLTEXTLOADER_API float GetFloat();
	DLLTEXTLOADER_API float GetFloatFromString(char* str);
	DLLTEXTLOADER_API char* GetString();
	DLLTEXTLOADER_API char* GetAllData();
	DLLTEXTLOADER_API void Clear();
	DLLTEXTLOADER_API void ClearAndOutput(char* outputname);
	DLLTEXTLOADER_API bool isEOF();
	DLLTEXTLOADER_API void Close();
	DLLTEXTLOADER_API void DestroyInstance();
	DLLTEXTLOADER_API void SaveFile(char* filename, char* data);
	DLLTEXTLOADER_API void OutputVariables(char* filename);
}

#endif

