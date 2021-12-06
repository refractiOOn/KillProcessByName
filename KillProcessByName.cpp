#include <iostream>
#include <Windows.h>
#include <stdio.h>
#include <psapi.h>

bool KillProcessByName(DWORD processID, TCHAR name[])
{
    TCHAR processName[] = L"<unknown>";
    HANDLE process = OpenProcess(PROCESS_ALL_ACCESS, FALSE, processID);

    if (!process || !processID)
    {
        return 0;
    }

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
        BOOL endProcess = TerminateProcess(process, 1);
        if (!endProcess)
        {
            return 0;
        }
    }

    if (process)
    {
        CloseHandle(process);
    }

    return 1;
}

int main()
{
    DWORD processes[1024], bytesReturned, processesAmount;
    TCHAR processToKill[MAX_PATH];
    bool result = false;

    std::wcin >> processToKill;

    if (!EnumProcesses(processes, sizeof(processes), &bytesReturned))
    {
        std::cout << "Could not get processes ids" << std::endl;
        return 0;
    }

    processesAmount = bytesReturned / sizeof(DWORD);

	for (size_t i = 0; i < processesAmount; ++i)
	{
        if (processes[i] != 0)
        {
            result = KillProcessByName(processes[i], processToKill);
        }
	}

    if (result)
    {
        std::cout << "Process terminated successfully" << std::endl;
    }
    else
    {
        std::cout << "Something went wrong" << std::endl;
    }
}