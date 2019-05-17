#include "Windows.h"
#include "Common.h"
#include "string"

WCHAR g_szLogPath[SMA_SIZE] = L"Setup.txt";

#pragma managed(push, off)

std::string UnicodeToUtf8(LPCWSTR pszUnicode)
{
	LPSTR	pszUtf8;
	int	len, maxLen;
	if (pszUnicode == NULL) return "";
	len = WideCharToMultiByte(CP_UTF8, 0, pszUnicode, -1, NULL, 0, NULL, NULL);
	if (len <= 0) return "";
	maxLen = len;

	pszUtf8 = new char[len + 1];
	if (pszUtf8 == NULL) return "";

	len = WideCharToMultiByte(CP_UTF8, 0, pszUnicode, -1, pszUtf8, len, NULL, NULL);
	if (len > maxLen)
	{
		delete[] pszUtf8;
		return "";
	}
	pszUtf8[len] = 0;
	std::string result = pszUtf8;
	delete[] pszUtf8;
	return result;
}

std::string UnicodeToUtf8(const std::wstring &sUnicode)
{
	return UnicodeToUtf8(sUnicode.c_str());
}

// Convert Unicode string to UTF8 string
std::wstring Utf8ToUnicode(LPCSTR pszUtf8)
{
	LPWSTR	pszUnicode;
	int	len, maxLen;

	if (pszUtf8 == NULL) return L"";
	len = MultiByteToWideChar(CP_UTF8, 0, pszUtf8, -1, NULL, 0);
	if (len <= 0) return L"";
	maxLen = len;

	pszUnicode = new WCHAR[len + 1];
	if (pszUnicode == NULL) return L"";

	len = MultiByteToWideChar(CP_UTF8, 0, pszUtf8, -1, pszUnicode, len);
	if (len > maxLen)
	{
		delete[] pszUnicode;
		return L"";
	}
	pszUnicode[len] = 0;
	std::wstring result = pszUnicode;
	delete[] pszUnicode;
	return result;
}

std::wstring Utf8ToUnicode(const std::string &sUtf8)
{
	return Utf8ToUnicode(sUtf8.c_str());
}

#ifdef _DEBUG
#define _DEBUG_PRINT_LOG
#endif
#ifndef _DEBUG_PRINT_LOG
#define _DEBUG_PRINT_LOG
#endif

void DebugPrintW(__in LPCWSTR pszFormat, ...)
{
#ifdef _DEBUG_PRINT_LOG
	PWCHAR szLog = NULL;
	va_list args;
	SYSTEMTIME *pTime = NULL;

	if (pszFormat == NULL) return;

	szLog = (PWCHAR)ALLOC(MAX_PATH * sizeof(WCHAR));
	pTime = (SYSTEMTIME *)ALLOC(sizeof(SYSTEMTIME));
	if (szLog == NULL || pTime == NULL)
	{
		return;
	}
	memset(szLog, 0, MAX_PATH * sizeof(WCHAR));
	memset(pTime, 0, sizeof(SYSTEMTIME));

	va_start(args, pszFormat);

	if (vswprintf_s(szLog, MAX_PATH, pszFormat, args) > 0)
	{
		std::wstring log = szLog;

		GetLocalTime(pTime);
		swprintf_s(szLog, MAX_PATH, L"\n[%d/%02d/%02d - %02d:%02d:%02d][USBTokenManager] ",
			pTime->wYear, pTime->wMonth, pTime->wDay,
			pTime->wHour, pTime->wMinute, pTime->wSecond);
		log = szLog + log;
		printf("%ws",log.c_str());
	}
	va_end(args);

	FREE(szLog);
	FREE(pTime);
#else
#endif
}

void DebugPrint(__in LPCSTR pszFormat, ...) 
{
#ifdef _DEBUG_PRINT_LOG
	PCHAR szLog = NULL;
	va_list args;
	SYSTEMTIME *pTime = NULL;

	if (pszFormat == NULL) return;

	szLog = (PCHAR)ALLOC(SMA_SIZE * sizeof(CHAR));
	pTime = (SYSTEMTIME *)ALLOC(sizeof(SYSTEMTIME));
	if (szLog == NULL || pTime == NULL)
	{
		return;
	}
	memset(szLog, 0, SMA_SIZE * sizeof(CHAR));
	memset(pTime, 0, sizeof(SYSTEMTIME));

	va_start(args, pszFormat);

	if (vsprintf_s(szLog, SMA_SIZE, pszFormat, args) > 0)
	{
		std::string log = szLog;

		GetLocalTime(pTime);
		sprintf_s(szLog, SMA_SIZE, "\n[%d/%02d/%02d - %02d:%02d:%02d][USBTokenManager] ",
			pTime->wYear, pTime->wMonth, pTime->wDay,
			pTime->wHour, pTime->wMinute, pTime->wSecond);
		log = szLog + log;
		OutputDebugStringA(log.c_str());
		printf("%s", log.c_str());
	}
	va_end(args);

	FREE(szLog);
	FREE(pTime);
#else
#endif
}

BOOLEAN WriteLogFile(__in LPCWSTR pszFormat, ...)
{
	BOOLEAN bStatus = FALSE;
	HANDLE hFile = NULL;
	PWCHAR szBuffer = NULL;
	WCHAR szBufferTemp[SMA_SIZE] = { 0 };
	DWORD dwBytesWritten = 0;
	SYSTEMTIME stTime = { 0 };
	va_list args;

	// Cap phat de tranh thieu stack
	szBuffer = (PWCHAR)ALLOC(MED_SIZE);
	if (szBuffer == NULL)
	{
		DebugPrint("Function %s failed 0x%X at %d in file %s", __FUNCTION__, GetLastError(), __LINE__, __FILE__);
		RET_THIS_STATUS(bStatus, FALSE);
	};
	memset(szBuffer, 0, MED_SIZE);
	va_start(args, pszFormat);
	if (vswprintf_s(szBufferTemp, MAX_PATH, pszFormat, args) <= 0)
	{
		DebugPrint("Function %s failed 0x%X at %d in file %s", __FUNCTION__, GetLastError(), __LINE__, __FILE__);
		RET_THIS_STATUS(bStatus, FALSE);
	}
	va_end(args);

	GetLocalTime(&stTime);
	swprintf_s(szBuffer, MED_SIZE / sizeof(WCHAR), L"[%02d/%02d/%02d - %02d:%02d:%02d][NetForwarderSetup]: %ws \r\n", stTime.wDay, stTime.wMonth, stTime.wYear, stTime.wHour, stTime.wMinute, stTime.wSecond, szBufferTemp);
	OutputDebugStringW(szBuffer);

	hFile = CreateFileW(g_szLogPath,
		FILE_APPEND_DATA,
		0,
		NULL,
		OPEN_ALWAYS,
		FILE_ATTRIBUTE_NORMAL,
		NULL);
	if (hFile == INVALID_HANDLE_VALUE)
	{
		DebugPrint("Function %s failed 0x%X at %d in file %s", __FUNCTION__, GetLastError(), __LINE__, __FILE__);
		RET_THIS_STATUS(bStatus, FALSE);
	}

	bStatus = (BOOLEAN)WriteFile(
		hFile,
		szBuffer,
		(ULONG)wcslen(szBuffer) * sizeof(WCHAR),  // number of bytes to write
		&dwBytesWritten, // number of bytes that were written
		NULL);            // no overlapped structure
	if (bStatus == FALSE)
	{
		DebugPrint("Function %s failed 0x%X at %d in file %s", __FUNCTION__, GetLastError(), __LINE__, __FILE__);
		RET_THIS_STATUS(bStatus, FALSE);
	}
	else
	{
		if (dwBytesWritten != (ULONG)wcslen(szBuffer) * sizeof(WCHAR))
		{
			DebugPrint("Error: dwBytesWritten != dwBytesToWrite\n");
		}
	}
	bStatus = TRUE;

RET_LABEL:
	FREE(szBuffer);
	CLOSE_HANDLE(hFile);
	return bStatus;
}


BOOLEAN PrintError(__in LPCSTR pszFormat, ...)
{
	BOOLEAN bRet = FALSE;
	PCHAR szLog = NULL;
	va_list args;
	SYSTEMTIME *pTime = NULL;

	if (pszFormat == NULL) return FALSE;

	szLog = (PCHAR)ALLOC(MAX_PATH * sizeof(CHAR));
	pTime = (SYSTEMTIME *)ALLOC(sizeof(SYSTEMTIME));
	if (szLog == NULL || pTime == NULL)
	{
		RET_THIS_STATUS(bRet, FALSE);
	}
	memset(szLog, 0, MAX_PATH * sizeof(CHAR));
	memset(pTime, 0, sizeof(SYSTEMTIME));

	va_start(args, pszFormat);
	if (vsprintf_s(szLog, MAX_PATH, pszFormat, args) > 0)
	{
		std::string log = szLog;
		CHAR szError[MAX_PATH] = { 0 };
		GetLocalTime(pTime);
		sprintf_s(szLog, MAX_PATH, "\n[%d/%02d/%02d - %02d:%02d:%02d][USBTokenManager] ",
			pTime->wYear, pTime->wMonth, pTime->wDay,
			pTime->wHour, pTime->wMinute, pTime->wSecond);
		ErrorExit(szError);
		log = szLog + log + ": " +szError;	
		printf("%s", log.c_str());
		OutputDebugStringA(log.c_str());
	}
	va_end(args);

RET_LABEL:
	FREE(szLog);
	FREE(pTime);
	return bRet; 
}

BOOL ErrorExit(LPSTR lpszFunction)
{
	// Retrieve the system error message for the last-error code

	LPVOID lpMsgBuf;
	DWORD dw = GetLastError();

	FormatMessageA(
		FORMAT_MESSAGE_ALLOCATE_BUFFER |
		FORMAT_MESSAGE_FROM_SYSTEM |
		FORMAT_MESSAGE_IGNORE_INSERTS,
		NULL,
		dw,
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
		(LPSTR)&lpMsgBuf,
		0, NULL);

	// Display the error message and exit the process
	sprintf_s(lpszFunction, MAX_PATH, "Error: 0x%X: %s", dw, (LPSTR)lpMsgBuf);
/*	wcscpy_s(lpszFunction, wcslen((LPWSTR)lpMsgBuf), (LPWSTR)lpMsgBuf);*/
//	printf("Error %d: %s\n", dw, lpszFunction);

	LocalFree(lpMsgBuf);
	return TRUE;
}

#pragma managed(pop)
