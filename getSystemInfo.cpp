#include <stdio.h>
#include <Windows.h>
#include <VersionHelpers.h>
#include <iostream>

int main() {
	 /*GETPRODUCTINFO ->kernel32 
	 **handler = (handler) GetProcAddress(GetModuleHandle(TEXT("kernel32.dll")),"GetProductInfo");
	  */
/*	OSVERSIONINFOEX osVerInfex;
	DWORDLONG dwlConditionMask = 0;
	int op = VER_GREATER_EQUAL;
	ZeroMemory(&osVerInfex, sizeof(OSVERSIONINFOEX));//ramden structure kadar alan sýfýrlýyor
	osVerInfex.dwOSVersionInfoSize = sizeof(OSVERSIONINFOEX);
	VER_SET_CONDITION(dwlConditionMask, VER_BUILDNUMBER,op);
	VER_SET_CONDITION(dwlConditionMask, VER_MAJORVERSION, op);
	VER_SET_CONDITION(dwlConditionMask, VER_MINORVERSION, op);
	int sum = VerifyVersionInfo(&osVerInfex,VER_BUILDNUMBER|VER_MAJORVERSION|VER_MINORVERSION,dwlConditionMask);
	//std::cout << "SONUC= " << sum << std::endl; // check if verify successful
*/	
	OSVERSIONINFOEX osVerInf;
	ZeroMemory(&osVerInf, sizeof(OSVERSIONINFOEX));//ramden structure kadar alan sýfýrlýyor
	osVerInf.dwOSVersionInfoSize = sizeof(OSVERSIONINFOEX);
	GetVersionEx((OSVERSIONINFO*)&osVerInf);//osversionex to osversioninfo **
	
	//DLL yukleme
	typedef void (WINAPI *PGNSI)(LPSYSTEM_INFO);
	PGNSI pgnsi; //method handler
	SYSTEM_INFO sysInf;//getnativesysteminfo icin structure
	ZeroMemory(&sysInf, sizeof(SYSTEM_INFO)); //fills a block w/ zeroes
	pgnsi = (PGNSI)GetProcAddress(GetModuleHandle(TEXT("Kernel32.dll")),"GetNativeSystemInfo");
	if (NULL != pgnsi) //dll load baþarýsýz ise
	{
		printf("\n DLL basariyla yuklendi...\n");
		pgnsi(&sysInf); //getnativesysteminfo cagiriyilor
	}
	else
	{
		printf("DLL yuklemesi basarisiz.. Eski yontem cagiriliyor..\n");
		GetSystemInfo(&sysInf); //32 bitlik yontemi cagir
	}
#if _WIN64
	printf("Kod 64 Bit Calisiyor\n");
	if(sysInf.wProcessorArchitecture == 9)
		//std::cout << "Mimari x64 / 64 bit" << std::endl;
		printf("Mimari x64 / 64 bit - ");
	else if (sysInf.wProcessorArchitecture == 0)
		printf("Mimari x86 / 32 bit - ");
	else
		printf("Mimari Bilinmiyor - ");
	printf(" %u cekirdek \n", sysInf.dwNumberOfProcessors);
	
#else _WIN32
	printf("Kod 32 Bit Calisiyor\n");
	if (sysInf.wProcessorArchitecture == 9)
		printf("Mimari x64 / 64 bit - ");
	else if (sysInf.wProcessorArchitecture == 0)
		printf("Mimari x86 / 32 bit - ");
	else
		printf("Mimari Bilinmiyor - ");
	printf(" %u cekirdek \n", sysInf.dwNumberOfProcessors);	
#endif

	//Fiziksel RAM gosteriliyor
	unsigned __int64 ramMiktariInKB = 0;
	GetPhysicallyInstalledSystemMemory(&ramMiktariInKB);
	wprintf(L"Ram: %d MB \n", ramMiktariInKB / 1024);
	
	//Windows Surumu
	if (!IsWindows10OrGreater())
		printf("Windows 10 ");
	else {
		if (osVerInf.dwMajorVersion == 10)
			printf("Windows 10 veya Windows Server 2016");//wont work without manifest
		else if (osVerInf.dwMajorVersion == 6)
		{
			switch (osVerInf.dwMinorVersion)
			{
			case 0:
				printf("Windows Vista / Server 2008 ");
				break;
			case 1:
				printf("Windows 7 / Server 2008 R2 ");
				break;
			case 2://windows 10da manifest duzenlenmediginde donen deger
				printf("Windows 8 veya Windows Server 2012");
				break;
			case 3:
				printf("Windows 8.1 veya Windows Server 2012 R2");
				break;
			default:
				printf("Windows XP / 2000 veya Server 2003");
				break;

			}
		}
	}

	printf("\n");

	//MEMORY STATUS
	MEMORYSTATUSEX memStat;
	memStat.dwLength = sizeof(memStat);
	GlobalMemoryStatusEx(&memStat);

	printf("--> %llu KB fiziksel bellek\n",(memStat.ullTotalPhys/1024)/1024);
	printf("--> %llu KB kullanilabilir bellek\n", (memStat.ullAvailPhys / 1024)/1024);
	printf("--> %% %d kadar bellek kullaniliyor\n", memStat.dwMemoryLoad );

	printf("\n");
	return 0;
}



