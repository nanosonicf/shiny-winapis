
#include <Windows.h>
#include <stdio.h>
#include <Psapi.h>

void PrintProcID(DWORD pID) {
	TCHAR szProcessName[MAX_PATH] = TEXT("*");
	HANDLE hProcess= OpenProcess(PROCESS_ALL_ACCESS |
		PROCESS_VM_READ,
		FALSE, pID);
	DWORD ProcessVersion, ProcessOnCore, ProcessID = pID;
	BOOL Wow64;
	TCHAR *ProcArch = TEXT("");
	ProcessVersion = GetProcessVersion(ProcessID);
	ProcessOnCore = GetCurrentProcessorNumber();
	IsWow64Process(hProcess, &Wow64);

	switch (Wow64) {
	case true: ProcArch = TEXT("Wow64"); break;
	case false: ProcArch = TEXT("Native"); break;
	}
	
	if (NULL != hProcess)
	{
		HMODULE hMod;
		DWORD cbNeeded;
		
		if (EnumProcessModulesEx(hProcess, &hMod, sizeof(hMod), &cbNeeded, LIST_MODULES_ALL) )
		{
			GetModuleBaseName(hProcess, hMod, szProcessName,
				sizeof(szProcessName) / sizeof(TCHAR));
		}
		/*if (EnumProcessModules(hProcess, &hMod, sizeof(hMod),
			&cbNeeded))
		{
			GetModuleBaseName(hProcess, hMod, szProcessName,
				sizeof(szProcessName) / sizeof(TCHAR));
		}
		*/
	}
	if (szProcessName[0]!='*') { //Ismi alinamayan processler * olarak kaldigindan yazdirilmasin.
		wprintf(L"%u %4s %5d.%d %4d %7s -- %llu \n",
			ProcessID,
			ProcArch,
			HIWORD(ProcessVersion), LOWORD(ProcessVersion),
			ProcessOnCore,
			szProcessName,
			hProcess
			);
	}
	else;
	CloseHandle(hProcess); //Process Handleleri birakiliyor.
}

int main() {
	HANDLE hProcess = GetCurrentProcess();
	DWORD ProcessVersion, ProcessOnCore, ProcessID = GetCurrentProcessId();
	BOOL Wow64;
	TCHAR *ProcArch = TEXT("");

	// Get the list of process identifiers.

	DWORD aProcesses[1024], cbNeeded, cProcesses;
	unsigned int i;
	
	if (!EnumProcesses(aProcesses, sizeof(aProcesses), &cbNeeded))
	{
		return 1;
	}


	// Kac tane process ID dondugunu hesapla.
	cProcesses = cbNeeded / sizeof(DWORD);

	printf(" PID-MODULE---VER--CPU----EXE--HANDLE DEGERI \n");
	for (i = 0; i < cProcesses; i++)
	{
		if (aProcesses[i] != 0)
		{
			PrintProcID(aProcesses[i]); //pIDleri methoda gonder
		}
	}
	
	return 0;

}