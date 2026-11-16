/* PcCheck.exe - harmless minoBox demo file ("a PC check tool found on GitHub").
 *
 * Needs nothing installed: one static Windows program. It only reads basic facts about the computer it
 * runs on and prints them. It changes nothing, writes no file and uses no network.
 * Run in a minoBox VM, the values are the VM's (e.g. user WDAGUtilityAccount in Windows Sandbox),
 * not the PC's.
 *
 * Build (MSYS2 UCRT64):  gcc -O2 -s -static -o PcCheck.exe PcCheck.c
 */
#include <windows.h>
#include <stdio.h>

typedef LONG (WINAPI *RtlGetVersionFn)(OSVERSIONINFOW *);

int main(void) {
    char computer[MAX_COMPUTERNAME_LENGTH + 1] = "?", user[256] = "?";
    DWORD cn = sizeof computer, un = sizeof user;
    GetComputerNameA(computer, &cn);
    GetUserNameA(user, &un);

    OSVERSIONINFOW os = {sizeof os};
    RtlGetVersionFn get = (RtlGetVersionFn)GetProcAddress(GetModuleHandleA("ntdll.dll"), "RtlGetVersion");
    if (get) get(&os);

    SYSTEM_INFO si;
    GetNativeSystemInfo(&si);
    MEMORYSTATUSEX mem = {sizeof mem};
    GlobalMemoryStatusEx(&mem);
    ULONGLONG up = GetTickCount64() / 1000;

    printf("PC Check 1.0 (minoBox demo file - reads only, changes nothing)\n");
    printf("------------------------------------------------------------\n");
    printf("Computer name : %s\n", computer);
    printf("User          : %s\n", user);
    printf("Windows       : %lu.%lu build %lu\n", os.dwMajorVersion, os.dwMinorVersion, os.dwBuildNumber);
    printf("CPU cores     : %lu\n", si.dwNumberOfProcessors);
    printf("Memory        : %.1f GB total, %.1f GB free\n", mem.ullTotalPhys / 1073741824.0,
           mem.ullAvailPhys / 1073741824.0);
    printf("Running for   : %llu min\n", up / 60);
    printf("------------------------------------------------------------\n");
    printf("%s\n", lstrcmpiA(user, "WDAGUtilityAccount") == 0
                       ? "This is a Windows Sandbox: the tool ran isolated, not on your PC."
                       : "Compare these values with your own PC to see where the tool ran.");
    return 0;
}
