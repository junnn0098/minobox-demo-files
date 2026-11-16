/* QuoteViewer_Setup.exe - harmless minoBox demo file ("an installer that came by email").
 *
 * It installs nothing. It prints where it is running (computer and user name) and leaves a short report
 * in the isolated VM's Output folder, so the report can be exported back to the PC through minoBox's
 * Defender check. The computer name in the report shows the program ran in the VM, not on the PC.
 *
 * Output folder: the newest C:\RunnerWorkspace\Sessions\session-*\runtime-*\generation-*\output
 * (the Runner's session Output; override the Sessions root with DEMO_SESSIONS_ROOT for testing).
 * If there is none, the report is written to the current folder.
 *
 * Build (MSYS2 UCRT64):  gcc -O2 -s -static -o QuoteViewer_Setup.exe QuoteViewer_Setup.c
 */
#include <windows.h>
#include <stdio.h>
#include <string.h>

static int newest_child(const char *dir, const char *pattern, char *out, size_t size) {
    char query[MAX_PATH];
    WIN32_FIND_DATAA fd;
    FILETIME best = {0, 0};
    int found = 0;
    snprintf(query, sizeof query, "%s\\%s", dir, pattern);
    HANDLE h = FindFirstFileA(query, &fd);
    if (h == INVALID_HANDLE_VALUE) return 0;
    do {
        if (!(fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)) continue;
        if (!found || CompareFileTime(&fd.ftLastWriteTime, &best) > 0) {
            best = fd.ftLastWriteTime;
            snprintf(out, size, "%s\\%s", dir, fd.cFileName);
            found = 1;
        }
    } while (FindNextFileA(h, &fd));
    FindClose(h);
    return found;
}

static int find_output(char *out, size_t size) {
    char root[MAX_PATH] = "C:\\RunnerWorkspace\\Sessions", a[MAX_PATH], b[MAX_PATH], c[MAX_PATH];
    DWORD n = GetEnvironmentVariableA("DEMO_SESSIONS_ROOT", root, sizeof root);
    if (n == 0 || n >= sizeof root) strcpy(root, "C:\\RunnerWorkspace\\Sessions");
    if (!newest_child(root, "session-*", a, sizeof a)) return 0;
    if (!newest_child(a, "runtime-*", b, sizeof b)) return 0;
    if (!newest_child(b, "generation-*", c, sizeof c)) return 0;
    snprintf(out, size, "%s\\output", c);
    DWORD attr = GetFileAttributesA(out);
    return attr != INVALID_FILE_ATTRIBUTES && (attr & FILE_ATTRIBUTE_DIRECTORY);
}

int main(void) {
    char computer[MAX_COMPUTERNAME_LENGTH + 1] = "?", user[256] = "?", folder[MAX_PATH], report[MAX_PATH];
    DWORD cn = sizeof computer, un = sizeof user;
    SYSTEMTIME t;
    GetComputerNameA(computer, &cn);
    GetUserNameA(user, &un);
    GetLocalTime(&t);

    printf("QuoteViewer Setup 2.1 (minoBox demo file - installs nothing)\n");
    for (int i = 1; i <= 5; i++) {               /* a few seconds of "installing" so the run is visible */
        printf("  installing... %d/5\n", i);
        fflush(stdout);
        Sleep(1000);
    }
    printf("Running on computer : %s\n", computer);
    printf("Running as user     : %s\n", user);

    int in_vm = find_output(folder, sizeof folder);
    if (!in_vm) GetCurrentDirectoryA(sizeof folder, folder);
    snprintf(report, sizeof report, "%s\\quote_check_report.txt", folder);
    FILE *f = fopen(report, "w");
    if (!f) {
        printf("Could not write the report to %s\n", report);
        return 1;
    }
    fprintf(f, "QuoteViewer Setup - minoBox demo report\n");
    fprintf(f, "Time     : %04d-%02d-%02d %02d:%02d:%02d\n", t.wYear, t.wMonth, t.wDay, t.wHour, t.wMinute, t.wSecond);
    fprintf(f, "Computer : %s\n", computer);
    fprintf(f, "User     : %s\n", user);
    fprintf(f, "\nThis file was written by QuoteViewer_Setup.exe, an unsigned program that arrived by email.\n");
    fprintf(f, "minoBox ran it in an isolated VM. The computer name above is the VM's, not your PC's.\n");
    fprintf(f, "Nothing was installed on your PC. This report came back only after the Defender check.\n");
    fclose(f);
    printf("Report written       : %s\n", report);
    printf(in_vm ? "Report is in the Output folder: ask minoBox to export it.\n"
                 : "No Runner Output folder here (not in a minoBox VM): report left in the current folder.\n");
    return 0;
}
