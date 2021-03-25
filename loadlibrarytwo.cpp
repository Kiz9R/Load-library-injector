// loadlibrarytwo.cpp : This file contains the 'main' function. Program execution begins and ends there.
//


#include <iostream>
#include <windows.h>
#include <string>
#include <thread>
#include<libloaderapi.h>


using namespace std;

void get_id(const char* window_title, DWORD& process_id)
{
    GetWindowThreadProcessId(FindWindow(NULL, window_title), &process_id);
}

void error(const char* error_title, const char* error_message)
{
    MessageBox(NULL, error_message, error_title, NULL);
    exit(-1);
}


bool file_exists(string file_name)
{
    struct stat buffer;
    return (stat(file_name.c_str(), &buffer) == 0);
}



int main()
{
    DWORD process_id = NULL;
    char dll_path[MAX_PATH];
    const char* dll_name = "cheat.dll";
    const char* window_title = "Team Fortress 2";

    if (!file_exists(dll_name))
    {
        error("file_name", ("file doesn't exists"));
    }

    if (!GetFullPathName(dll_name, MAX_PATH, dll_path, nullptr))
    {
        error("GetFullPathName", "failed to get path");
    }

    get_id(window_title, process_id);
    if (process_id == NULL)
    {
        error("get_id", "fail to get id");
    }

    HANDLE h_process = OpenProcess(PROCESS_ALL_ACCESS, NULL, process_id);
    if (!h_process)
    {
        error("OpenProcess", "Failed to open a hanble to process");
    }

    void* allocate_memory = VirtualAllocEx(h_process, nullptr, MAX_PATH, MEM_RESERVE | MEM_COMMIT, PAGE_READWRITE);
    if (!allocate_memory)
    {
        error("VirtualAllocEx", "failed to allocate memory");
    }

    if (!WriteProcessMemory(h_process, allocate_memory, dll_path, MAX_PATH, nullptr))
    {
        error("WriteProcessMemory", "failed to write process memory");
    }

    HANDLE h_thread = CreateRemoteThread(h_process, nullptr, NULL, LPTHREAD_START_ROUTINE(LoadLibraryA), allocate_memory, NULL, nullptr);
    if (!h_thread)
    {
        error("CrateRemoteThread", "failed to create remote thread");
    }

    CloseHandle(h_process);
    VirtualFreeEx(h_process, allocate_memory, NULL, MEM_RELEASE);
    MessageBox(0, "Successfully Injected!", "Success", 0);

}

// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file
