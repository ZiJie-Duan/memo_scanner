#include "memo_scanner.h"

/* open_process
 * 打开进程
 * 参数: processId 进程ID
 * 返回值: 进程句柄
 * 默认请求 内存读写权限，查询权限，进程信息权限
*/
HANDLE open_process(DWORD processId) {
    info_print(INFO, "running..", "open_process");

    DWORD desiredAccess = PROCESS_VM_READ | PROCESS_VM_WRITE | 
                          PROCESS_VM_OPERATION | PROCESS_QUERY_INFORMATION ;

    HANDLE hProcess = OpenProcess(desiredAccess, FALSE, processId);
    if (hProcess == NULL) {
        error(ERROR_SERIOUS, "OpenProcess");
    } else {
        printf("OpenProcess success\n");
    }
    return hProcess;
}

/* close_process
 * 关闭进程
 * 参数: hProcess 进程句柄
 * 返回值: 无
*/
void close_process(HANDLE hProcess) {
    info_print(INFO, "closing..", "close_process");
    CloseHandle(hProcess);
}
