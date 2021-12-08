#include <iostream>
#include <windows.h>
#include <stdio.h>
#include <psapi.h>

void KillProcessByName(DWORD processID, const std::wstring& name)
{
    HANDLE process = OpenProcess(PROCESS_ALL_ACCESS, FALSE, processID);
    if (!process)
    {
        return;
    }

    std::wstring processName(MAX_PATH, L'\0');
    /*if (!GetModuleFileNameExW(process, 0, &processName[0], processName.length()))
    {
        return;
    }*/
    DWORD nameSize = GetModuleBaseName(process, 0, &processName[0], processName.length());
    if (!nameSize)
    {
        return;
    }
    processName.resize(nameSize);

    if (processName == name)
    {
        BOOL endProcess = TerminateProcess(process, 1);
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
    DWORD processes[1024], bytesReturned;
    std::wstring processToKill(MAX_PATH, L'\0');
    std::wcin >> processToKill;

    if (!EnumProcesses(processes, sizeof(processes), &bytesReturned))
    {
        std::cout << "Could not get processes ids" << std::endl;
    }

    DWORD processesAmount = bytesReturned / sizeof(DWORD);
    for (size_t i = 0; i < processesAmount; ++i)
    {
        if (processes[i])
        {
            KillProcessByName(processes[i], processToKill);
        }
    }
}