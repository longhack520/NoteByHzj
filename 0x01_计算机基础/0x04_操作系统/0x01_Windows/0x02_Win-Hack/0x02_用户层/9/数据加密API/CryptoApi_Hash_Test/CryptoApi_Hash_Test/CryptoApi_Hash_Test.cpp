// CryptoApi_Hash_Test.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include <Windows.h>


void ShowError(char *pszText)
{
	char szErr[MAX_PATH] = { 0 };
	::wsprintf(szErr, "%s Error[%d]\n", pszText, ::GetLastError());
#ifdef _DEBUG
	::MessageBox(NULL, szErr, "ERROR", MB_OK | MB_ICONERROR);
#endif
}


BOOL GetFileData(char *pszFilePath, BYTE **ppFileData, DWORD *pdwFileDataLength)
{
	BOOL bRet = TRUE;
	BYTE *pFileData = NULL;
	DWORD dwFileDataLength = 0;
	HANDLE hFile = NULL;
	DWORD dwTemp = 0;

	do
	{
		hFile = ::CreateFile(pszFilePath, GENERIC_READ | GENERIC_WRITE,
			FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, OPEN_EXISTING,
			FILE_ATTRIBUTE_ARCHIVE, NULL);
		if (INVALID_HANDLE_VALUE == hFile)
		{
			bRet = FALSE;
			ShowError("CreateFile");
			break;
		}

		dwFileDataLength = ::GetFileSize(hFile, NULL);

		pFileData = new BYTE[dwFileDataLength];
		if (NULL == pFileData)
		{
			bRet = FALSE;
			ShowError("new");
			break;
		}
		::RtlZeroMemory(pFileData, dwFileDataLength);

		::ReadFile(hFile, pFileData, dwFileDataLength, &dwTemp, NULL);

		// 返回
		*ppFileData = pFileData;
		*pdwFileDataLength = dwFileDataLength;

	} while (FALSE);

	if (hFile)
	{
		::CloseHandle(hFile);
	}

	return bRet;
}


BOOL CalculateHash(BYTE *pData, DWORD dwDataLength, ALG_ID algHashType, BYTE **ppHashData, DWORD *pdwHashDataLength)
{
	HCRYPTPROV hCryptProv = NULL;
	HCRYPTHASH hCryptHash = NULL;
	BYTE *pHashData = NULL;
	DWORD dwHashDataLength = 0;
	DWORD dwTemp = 0;
	BOOL bRet = FALSE;


	do
	{
		// 获得指定CSP的密钥容器的句柄
		bRet = ::CryptAcquireContext(&hCryptProv, NULL, NULL, PROV_RSA_AES, CRYPT_VERIFYCONTEXT);
		if (FALSE == bRet)
		{
			ShowError("CryptAcquireContext");
			break;
		}

		// 创建一个HASH对象, 指定HASH算法
		bRet = ::CryptCreateHash(hCryptProv, algHashType, NULL, NULL, &hCryptHash);
		if (FALSE == bRet)
		{
			ShowError("CryptCreateHash");
			break;
		}

		// 计算HASH数据
		bRet = ::CryptHashData(hCryptHash, pData, dwDataLength, 0);
		if (FALSE == bRet)
		{
			ShowError("CryptHashData");
			break;
		}

		// 获取HASH结果的大小
		dwTemp = sizeof(dwHashDataLength);
		bRet = ::CryptGetHashParam(hCryptHash, HP_HASHSIZE, (BYTE *)(&dwHashDataLength), &dwTemp, 0);
		if (FALSE == bRet)
		{
			ShowError("CryptGetHashParam");
			break;
		}

		// 申请内存
		pHashData = new BYTE[dwHashDataLength];
		if (NULL == pHashData)
		{
			bRet = FALSE;
			ShowError("new");
			break;
		}
		::RtlZeroMemory(pHashData, dwHashDataLength);

		// 获取HASH结果数据
		bRet = ::CryptGetHashParam(hCryptHash, HP_HASHVAL, pHashData, &dwHashDataLength, 0);
		if (FALSE == bRet)
		{
			ShowError("CryptGetHashParam");
			break;
		}

		// 返回数据
		*ppHashData = pHashData;
		*pdwHashDataLength = dwHashDataLength;

	} while (FALSE);

	// 释放关闭
	if (FALSE == bRet)
	{
		if (pHashData)
		{
			delete[]pHashData;
			pHashData = NULL;
		}
	}
	if (hCryptHash)
	{
		::CryptDestroyHash(hCryptHash);
	}
	if (hCryptProv)
	{
		::CryptReleaseContext(hCryptProv, 0);
	}

	return bRet;
}


int _tmain(int argc, _TCHAR* argv[])
{
	BYTE *pData = NULL;
	DWORD dwDataLength = 0;
	DWORD i = 0;
	BYTE *pHashData = NULL;
	DWORD dwHashDataLength = 0;
	
	// 读取文件数据
	GetFileData("C:\\Users\\DemonGan\\Desktop\\520.exe", &pData, &dwDataLength);

	// MD5
	CalculateHash(pData, dwDataLength, CALG_MD5, &pHashData, &dwHashDataLength);
	printf("MD5[%d]\n", dwHashDataLength);
	for (i = 0; i < dwHashDataLength; i++)
	{
		printf("%x", pHashData[i]);
	}
	printf("\n\n", dwHashDataLength);
	if (pHashData)
	{
		delete[]pHashData;
		pHashData = NULL;
	}

	// SHA1
	CalculateHash(pData, dwDataLength, CALG_SHA1, &pHashData, &dwHashDataLength);
	printf("SHA1[%d]\n", dwHashDataLength);
	for (i = 0; i < dwHashDataLength; i++)
	{
		printf("%x", pHashData[i]);
	}
	printf("\n\n", dwHashDataLength);
	if (pHashData)
	{
		delete[]pHashData;
		pHashData = NULL;
	}

	// SHA256
	CalculateHash(pData, dwDataLength, CALG_SHA_256, &pHashData, &dwHashDataLength);
	printf("SHA256[%d]\n", dwHashDataLength);
	for (i = 0; i < dwHashDataLength; i++)
	{
		printf("%x", pHashData[i]);
	}
	printf("\n\n", dwHashDataLength);
	if (pHashData)
	{
		delete[]pHashData;
		pHashData = NULL;
	}

	system("pause");
	return 0;
}

