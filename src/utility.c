#include "memo_scanner.h"

/* error
 * 打印错误信息
 * 参数: mode 错误模式
 *       msg 错误信息
 * 返回值: 错误代码
 * mode 错误模式，0为简单模式，1为严重模式
 * msg 错误信息
*/
int error(int mode, char *msg) {
    info_print(INFO_NECESSARY, msg, "err_scanner");
    int err = GetLastError();
    if (mode == ERROR_SIMPLE) {
        printf("Error: %s\n", msg);
        printf("ERROR CODE: %d\n", err);
        return err;
    } else {
        printf("SERIOUS Error: %s\n", msg);
        printf("ERROR CODE: %d\n", err);
        getchar();
        getchar();
        exit(1);
    }
}


BOOL SetDebugPrivilege() {
    HANDLE hToken;
    LUID luid;
    TOKEN_PRIVILEGES tkp;

    // 打开进程令牌
    if (!OpenProcessToken(GetCurrentProcess(), TOKEN_ADJUST_PRIVILEGES | TOKEN_QUERY, &hToken)) {
        printf("OpenProcessToken error: %lu\n", GetLastError());
        return FALSE;
    }

    // 获取LUID
    if (!LookupPrivilegeValue(NULL, SE_DEBUG_NAME, &luid)) {
        printf("LookupPrivilegeValue error: %lu\n", GetLastError());
        return FALSE;
    }

    tkp.PrivilegeCount = 1;
    tkp.Privileges[0].Luid = luid;
    tkp.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED;

    // 调整令牌权限
    if (!AdjustTokenPrivileges(hToken, FALSE, &tkp, sizeof(TOKEN_PRIVILEGES), NULL, NULL)) {
        printf("AdjustTokenPrivileges error: %lu\n", GetLastError());
        return FALSE;
    }

    if (GetLastError() == ERROR_NOT_ALL_ASSIGNED) {
        printf("The token does not have the specified privilege.\n");
        return FALSE;
    }

    CloseHandle(hToken);
    return TRUE;
}
