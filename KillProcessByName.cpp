#include <iostream>
#include <Windows.h>
#include <stdio.h>
#include <psapi.h>

void KillProcessByName(DWORD processID, TCHAR name[])
{
    TCHAR processName[] = L"<unknown>";
    HANDLE process = OpenProcess(PROCESS_ALL_ACCESS, FALSE, processID);

    if (process)
    {
        HMODULE mod;
        DWORD cbNeeded;

        if (EnumProcessModules(process, &mod, sizeof(mod),
            &cbNeeded))
        {
            GetModuleBaseName(process, mod, processName,
                sizeof(processName) / sizeof(TCHAR));
        }
    }

    bool neededProcess = false;
    for (size_t i = 0; i < sizeof(processName) / (sizeof(TCHAR) * 2); ++i)
    {
        if (name[i] != processName[i])
        {
            neededProcess = false;
            break;
        }
        else
        {
            neededProcess = true;
        }
        
    }
    if (neededProcess)
    {
        BOOL endProcess = TerminateProcess(
            process,
            1
        );
        if (!endProcess)
        {
            std::cout << "Could not terminate process with the chosen name" << std::endl;
        }
        else
        {
            std::cout << "Process terminated successfully" << std::endl;
        }
    }

    CloseHandle(process);
}

int main()
{
    DWORD processes[1024], bytesReturned, processesAmount;
    TCHAR processToKill[MAX_PATH];
    std::wcin >> processToKill;

    if (!EnumProcesses(processes, sizeof(processes), &bytesReturned))
    {
        std::cout << "Could not get processes ids" << std::endl;
    }

    processesAmount = bytesReturned / sizeof(DWORD);

	for (size_t i = 0; i < processesAmount; ++i)
	{
        if (processes[i] != 0)
        {
            KillProcessByName(processes[i], processToKill);
        }
	}
}