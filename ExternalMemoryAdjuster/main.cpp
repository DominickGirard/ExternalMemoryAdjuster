#include <Windows.h>
#include <iostream>
int main(int argc, char** argv) {
    
    // Find our Wesnoth Window.
    HWND wesnoth_window = FindWindow(NULL, L"The Battle for Wesnoth - 1.14.9");

    // Gets the process id for the window specified above. (note the second argument return a pointer hence the & )
    DWORD process_id = 0;
    GetWindowThreadProcessId(wesnoth_window, &process_id);

    //if process id fails to be found we exit the program
    if (process_id == 0)
    {
        std::cout << "failed to find process exiting program";
        exit(0);
    }

    //open the process. PROCESS_ALL_ACCESS means we can both read and write to the process. However,
    // it also means that this program must be executed as an administrator to work.
    HANDLE wesnoth_process = OpenProcess(PROCESS_ALL_ACCESS, true, process_id);

    // Read the value at 0x017EED18 and place its value into the variable gold_value. 
    //0x017EED18 is the base pointer before the offsets are added to get the dynamic memory adress.
    DWORD gold_value = 0;
    SIZE_T bytes_read = 0;
    ReadProcessMemory(wesnoth_process, (void*)0x017EED18, &gold_value, 4, &bytes_read);

    // first offset is added
    // self not the => means in need to be read in memory (check the engine for pointer instructions)
    gold_value += 0xA90;
    ReadProcessMemory(wesnoth_process, (void*)gold_value, &gold_value, 4, &bytes_read);

    //second offset but does not need to be read by memory
    gold_value += 4;
    
    // set this to the value the memory address references.
    DWORD new_gold_value = 155;
    //outputdd
    SIZE_T bytes_written = 0;
    WriteProcessMemory(wesnoth_process, (void*)gold_value, &new_gold_value, 4, &bytes_written);

    return 0;
}