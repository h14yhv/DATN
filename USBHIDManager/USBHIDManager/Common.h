#pragma once
#include "Windows.h"
#include "tchar.h"
#include "string"

//////////////////////////////////////////////////////////////////////////
// Buffer len
#define TIN_SIZE		256
#define SMA_SIZE		(TIN_SIZE * 4)	//1024
#define MED_SIZE		(SMA_SIZE * 4)	//4096 
#define BIG_SIZE		(MED_SIZE * 4)	//16384
#define HUG_SIZE		(BIG_SIZE * 4)	//65536
#define GIA_SIZE		(HUG_SIZE * 4)	//262144

//////////////////////////////////////////////////////////////////////////
// Utils macros
#define MEMSET(addr, value, size_in_byte)	memset((addr), (value), (size_in_byte))
#define MEMSETW(addr, value, size_in_word)	memset((addr), (value), ((size_in_word) * sizeof(WCHAR)))
#define ALLOC(size_in_byte)			malloc(size_in_byte) //VirtualAlloc(NULL, (size_in_byte), MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE)

#define CLOSE_HANDLE(handle)		{if (handle != NULL)  { CloseHandle(handle); handle = NULL;}}

#define FREE(x)						{if ((x) != NULL) {free(x); x = NULL;}} //{VirtualFree(x, size_in_byte, MEM_COMMIT | MEM_RESERVE); x = NULL;}}
#define FREE2(x, y)					{FREE((x)); FREE((y));}
#define FREE3(x, y, z)				{FREE((x)); FREE((y)); FREE((z));}

#define wsizeof(szPath)				(wcslen(szPath) * sizeof(WCHAR))
#define RET_LABEL					RET_STATUS
#define RET_THIS_STATUS(stvar, nts)	{stvar = nts; goto RET_LABEL;}
#define RET_THIS					goto RET_LABEL
#define TEST_FLAG(val, flag)		((val & flag) == flag)

#define DBG_PRINT_FILE
#ifdef DBG_PRINT_FILE
#define DBGPRINTF0(format)							{TCHAR szInfo[TIN_SIZE]; memset(szInfo, 0, TIN_SIZE * sizeof(TCHAR)); _stprintf_s(szInfo, TIN_SIZE, format); WriteLog(szInfo);OutputDebugString(szInfo);}
#define DBGPRINTF1(format, arg)						{TCHAR szInfo[TIN_SIZE]; memset(szInfo, 0, TIN_SIZE * sizeof(TCHAR)); _stprintf_s(szInfo, TIN_SIZE, format, arg); WriteLog(szInfo);OutputDebugString(szInfo);}
#define DBGPRINTF2(format, arg1, arg2)				{TCHAR szInfo[TIN_SIZE]; memset(szInfo, 0, TIN_SIZE * sizeof(TCHAR)); _stprintf_s(szInfo, TIN_SIZE, format, arg1, arg2); WriteLog(szInfo);OutputDebugString(szInfo);}
#define DBGPRINTF3(format, arg1, arg2, arg3)		{TCHAR szInfo[TIN_SIZE]; memset(szInfo, 0, TIN_SIZE * sizeof(TCHAR)); _stprintf_s(szInfo, TIN_SIZE, format, arg1, arg2, arg3); WriteLog(szInfo);OutputDebugString(szInfo);}
#define DBGPRINTF4(format, arg1, arg2, arg3, arg4)	{TCHAR szInfo[TIN_SIZE]; memset(szInfo, 0, TIN_SIZE * sizeof(TCHAR)); _stprintf_s(szInfo, TIN_SIZE, format, arg1, arg2, arg3, arg4); WriteLog(szInfo);OutputDebugString(szInfo);}
#endif // DBG_PRINT_FILE

#define DBG_PRINT

#ifdef _DEBUG 
#define DBGPRINT0(format)							{TCHAR szInfo[TIN_SIZE]; memset(szInfo, 0, TIN_SIZE * sizeof(TCHAR)); _stprintf_s(szInfo, TIN_SIZE, format); OutputDebugString(szInfo);}
#define DBGPRINT1(format, arg)						{TCHAR szInfo[TIN_SIZE]; memset(szInfo, 0, TIN_SIZE * sizeof(TCHAR)); _stprintf_s(szInfo, TIN_SIZE, format, arg); OutputDebugString(szInfo);}
#define DBGPRINT2(format, arg1, arg2)				{TCHAR szInfo[TIN_SIZE]; memset(szInfo, 0, TIN_SIZE * sizeof(TCHAR)); _stprintf_s(szInfo, TIN_SIZE, format, arg1, arg2); OutputDebugString(szInfo);}
#define DBGPRINT3(format, arg1, arg2, arg3)			{TCHAR szInfo[TIN_SIZE]; memset(szInfo, 0, TIN_SIZE * sizeof(TCHAR)); _stprintf_s(szInfo, TIN_SIZE, format, arg1, arg2, arg3); OutputDebugString(szInfo);}
#define DBGPRINT4(format, arg1, arg2, arg3, arg4)	{TCHAR szInfo[TIN_SIZE]; memset(szInfo, 0, TIN_SIZE * sizeof(TCHAR)); _stprintf_s(szInfo, TIN_SIZE, format, arg1, arg2, arg3, arg4); OutputDebugString(szInfo);}
#else
#ifdef DBG_PRINT
#define DBGPRINT0(format)							{TCHAR szInfo[TIN_SIZE]; memset(szInfo, 0, TIN_SIZE * sizeof(TCHAR)); _stprintf_s(szInfo, TIN_SIZE, format); OutputDebugString(szInfo);}
#define DBGPRINT1(format, arg)						{TCHAR szInfo[TIN_SIZE]; memset(szInfo, 0, TIN_SIZE * sizeof(TCHAR)); _stprintf_s(szInfo, TIN_SIZE, format, arg); OutputDebugString(szInfo);}
#define DBGPRINT2(format, arg1, arg2)				{TCHAR szInfo[TIN_SIZE]; memset(szInfo, 0, TIN_SIZE * sizeof(TCHAR)); _stprintf_s(szInfo, TIN_SIZE, format, arg1, arg2); OutputDebugString(szInfo);}
#define DBGPRINT3(format, arg1, arg2, arg3)			{TCHAR szInfo[TIN_SIZE]; memset(szInfo, 0, TIN_SIZE * sizeof(TCHAR)); _stprintf_s(szInfo, TIN_SIZE, format, arg1, arg2, arg3); OutputDebugString(szInfo);}
#define DBGPRINT4(format, arg1, arg2, arg3, arg4)	{TCHAR szInfo[TIN_SIZE]; memset(szInfo, 0, TIN_SIZE * sizeof(TCHAR)); _stprintf_s(szInfo, TIN_SIZE, format, arg1, arg2, arg3, arg4); OutputDebugString(szInfo);}
#else
#define DBGPRINT0(format)							
#define DBGPRINT1(format, arg)						
#define DBGPRINT2(format, arg1, arg2)				
#define DBGPRINT3(format, arg1, arg2, arg3)			
#define DBGPRINT4(format, arg1, arg2, arg3, arg4)
#endif // DBG_PRINT
#endif // _DEBUG


// Define

#define MAX_WHITE_LIST_PROCESS 32
#define MAX_PATH 260

typedef enum _TYPE_MESSAGE
{
	Protect,
	UnProtect,
}TYPE_MESSAGE;

typedef enum _RIGHT_ACCESS
{
	Read = 1,
	Write = 2,
}RIGHT_ACCESS;

typedef enum _TYPE_SOURCE
{
	File,
	Key,
	Value,
	Process,
	Thread,
	Memory,
	Services,
	AllProtectedResources = 100,
}TYPE_SOURCE;

typedef struct _WHITE_LIST_PROCESS
{
	WCHAR szProcessPath[MAX_PATH];
	BOOLEAN bAllowChildPath;
}WHITE_LIST_PROCESS, *PWHITE_LIST_PROCESS;

typedef struct _DB_ENTRY
{
	TYPE_SOURCE ulType;
	BOOLEAN bProtectChildPath;
	WCHAR szPath[MAX_PATH];
	RIGHT_ACCESS ulRightAccess;
	WHITE_LIST_PROCESS listWhiteProcessPath[MAX_WHITE_LIST_PROCESS];
}DB_ENTRY, *PDB_ENTRY;

void DebugPrintW(__in LPCWSTR pszFormat, ...);
void DebugPrint(__in LPCSTR pszFormat, ...);
BOOL ErrorExit(LPSTR lpszFunction);
BOOLEAN PrintError(__in LPCSTR pszFormat, ...);

