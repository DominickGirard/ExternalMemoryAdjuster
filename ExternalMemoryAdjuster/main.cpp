#include <Windows.h>

int main(int argc, char** argv) {
    /*
        To use ReadProcessMemory and WriteProcessMemory, we require a handle to the Wesnoth process.

        To get this handle, we require a process id. The quickest way to get a process id for a particular
        process is to use GetWindowThreadProcessId.

        GetWindowThreadProcessId requires a window handle (different than a process handle). To get this
        window handle, we use FindWindow.
    */

    // Find our Wesnoth window. Depending on your language settings, this might be different.
    HWND wesnoth_window = FindWindow(NULL, L"The Battle for Wesnoth - 1.14.9");

    // Get the process id for the Wesnoth process. GetWindowThreadProcessId does not return a process id, but
    // rather fills a provided variable with its value, hence the &.
    DWORD process_id = 0;
    GetWindowThreadProcessId(wesnoth_window, &process_id);

    // Open our Wesnoth process. PROCESS_ALL_ACCESS means we can both read and write to the process. However,
    // it also means that this program must be executed as an administrator to work.
    HANDLE wesnoth_process = OpenProcess(PROCESS_ALL_ACCESS, true, process_id);

    // Read the value at 0x017EED18 and place its value into the variable gold_value. 
    DWORD gold_value = 0;
    SIZE_T bytes_read = 0;
    ReadProcessMemory(wesnoth_process, (void*)0x017EED18, &gold_value, 4, &bytes_read);

    gold_value += 0xA90;
    ReadProcessMemory(wesnoth_process, (void*)gold_value, &gold_value, 4, &bytes_read);

    gold_value += 4;

    DWORD new_gold_value = 155;
    SIZE_T bytes_written = 0;
    WriteProcessMemory(wesnoth_process, (void*)gold_value, &new_gold_value, 4, &bytes_written);

    return 0;
}