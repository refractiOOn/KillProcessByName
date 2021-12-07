#include <Windows.h>
#include <stdio.h>
#include <psapi.h>

void KillProcessByName(DWORD processID, TCHAR name[])
{
    TCHAR processName[] = L"<unknown>";
    HANDLE process = OpenProcess(PROCESS_ALL_ACCESS, FALSE, processID);

    if (process != NULL)
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
    for (size_t i = 0; i < sizeof(name) / (sizeof(TCHAR) * 2); ++i)
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
            printf("Could not terminate process with the chosen name\n");
        }
        else
        {
            printf("Process terminated successfully\n");
        }
    }

    CloseHandle(process);
}

int main()
{
    DWORD processes[1024], bytesReturned, processesAmount;
    TCHAR processToKill[] = L"Evernote.exe";

    if (!EnumProcesses(processes, sizeof(processes), &bytesReturned))
    {
        printf("Could not get processes ids");
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