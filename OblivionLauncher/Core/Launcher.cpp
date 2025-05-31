#include <Windows.h>
#include <ShlObj_core.h>
#include <winuser.h>
#include <direct.h>

#include <iostream>

#include <d3d9.h>
#pragma comment(lib, "d3d9.lib")

#include <mmsystem.h>
#pragma comment(lib, "winmm.lib")

#include <thread>

#include "Shared/Shared.h"

#include "Resources/resource.h"

LRESULT __stdcall main_window_proc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
        if (uMsg != WM_DESTROY)
                return DefWindowProcA(hwnd, uMsg, wParam, lParam);
        PostQuitMessage(0);
        return 0;
}

auto create_window_class() -> void
{
        std::cout<< "creating window class..." << "\n";
        Oblivion::Window::MAIN_WINDOW_CLASS.cbSize = 48;
        Oblivion::Window::MAIN_WINDOW_CLASS.style = 3;
        Oblivion::Window::MAIN_WINDOW_CLASS.lpfnWndProc = (WNDPROC)main_window_proc;
        Oblivion::Window::MAIN_WINDOW_CLASS.cbClsExtra = 0;
        Oblivion::Window::MAIN_WINDOW_CLASS.cbWndExtra = 0;
        Oblivion::Window::MAIN_WINDOW_CLASS.hInstance = Oblivion::INSTANCE;
        Oblivion::Window::MAIN_WINDOW_CLASS.hIcon = LoadIconA(Oblivion::INSTANCE, (LPCSTR)0x11B);
        Oblivion::Window::MAIN_WINDOW_CLASS.hCursor = LoadCursorA(0, (LPCSTR)0x7F00);
        Oblivion::Window::MAIN_WINDOW_CLASS.hbrBackground = (HBRUSH)6;
        Oblivion::Window::MAIN_WINDOW_CLASS.lpszMenuName = 0;
        Oblivion::Window::MAIN_WINDOW_CLASS.lpszClassName = Oblivion::STRING_TABLE.at(109);
        Oblivion::Window::MAIN_WINDOW_CLASS.hIconSm = LoadIconA(Oblivion::INSTANCE, (LPCSTR)0xE4);
}

auto install_path_exists() -> bool
{
        HKEY registryKey;

        LSTATUS openKey = RegOpenKeyExA(
                HKEY_LOCAL_MACHINE,
                Oblivion::REGISTRY_SUBKEY,
                0,
                KEY_QUERY_VALUE | KEY_WOW64_32KEY,
                &registryKey);

        std::cout << "install_path_exists | RegOpenKeyExA: " << openKey << "\n";
        
        if (openKey != ERROR_SUCCESS)
                return false;  // failed to open key
        
        LSTATUS queryValue = RegQueryValueExA(
                registryKey,
                "Installed Path",
                0,
                0,
                0,
                0);
                
        std::cout << "install_path_exists | RegQueryValueExA: " << queryValue << "\n";
        
        RegCloseKey(registryKey);
        
        return (queryValue == ERROR_SUCCESS);
}

auto get_install_path(OUT LPBYTE* install_path) -> bool
{
        bool success = false;
        bool installPathExists = install_path_exists();
        HKEY registryKey;
        DWORD buffer = 260; // buffer size for path
        
        if (installPathExists)
        {
                // allocate memory for the install path buffer
                *install_path = (LPBYTE)operator new(buffer);

                // open the registry key
                LSTATUS open_key = RegOpenKeyExA(
                        HKEY_LOCAL_MACHINE,
                        Oblivion::REGISTRY_SUBKEY,
                        0,
                        KEY_QUERY_VALUE | KEY_WOW64_32KEY,
                        &registryKey
                        );

                // if the key opened successfully
                if (open_key == ERROR_SUCCESS)
                {
                        // success equals (if the value exists and was read)
                        success = RegQueryValueExA(
                                registryKey,
                                "Installed Path",
                                0,
                                0,
                                *install_path,
                                &buffer
                                ) == ERROR_SUCCESS;
                        
                        RegCloseKey(registryKey);
                }
        }

        // free the allocated buffer if failed
        if (!success)
        {
                operator delete(*install_path);
                *install_path = nullptr;
        }
        
        return success;
}

#ifndef GCL_HICON
#define GCL_HICON (-14) //weird fuckery in winuser
#endif

bool initialized = false;
auto dialog_callback(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) -> INT_PTR __stdcall
{

        INT_PTR result = 0;
        LONG windowLong;
        HWND dialogItem;
        HICON icon;
        if (uMsg > WM_HOTKEY)
        {
        }
        else if (uMsg == WM_HOTKEY)  // 272 decimal
        {
                if (initialized == false)
                {
                        LONG style = GetWindowLongA(hWnd, GWL_EXSTYLE);
                        SetWindowLongA(hWnd, GWL_EXSTYLE, style | WS_EX_LAYERED);
                        SetLayeredWindowAttributes(hWnd, 0, 128, LWA_ALPHA);
                        initialized = true;
                }

                RECT windowRect;
                GetWindowRect(hWnd, &windowRect);

                HWND child = GetDlgItem(hWnd, 1033);
                RECT childRect;
                GetWindowRect(child, &childRect);

                // calculate scale ratios (float casts)
                //*(float*)&dword_4178C0 = (float)(childRect.right - childRect.left) / (windowRect.right - windowRect.left);
                //*(float*)&dword_4179CC = (float)(childRect.bottom - childRect.top) / (windowRect.bottom - windowRect.top);

                // center the child window in the parent
                MoveWindow(
                    hWnd,
                    windowRect.left + (windowRect.right - windowRect.left - (childRect.right - childRect.left)) / 2,
                    windowRect.top + (windowRect.bottom - windowRect.top - (childRect.bottom - childRect.top)) / 2,
                    childRect.right - childRect.left,
                    childRect.bottom - childRect.top,
                    TRUE);

                ShowWindow(hWnd, SW_SHOW);

                HICON icon = LoadIconA(Oblivion::INSTANCE, MAKEINTRESOURCEA(5));
                SetClassLongPtrA(hWnd, GCL_HICON, (LONG_PTR)icon);

                return TRUE;
        }
        else
        {
                switch (uMsg)
                {

                case WM_ACTIVATE: // 6u:
                        //sub_401860(0);
                        UpdateWindow(hWnd);
                        break;
                        
                case WM_SETFOCUS: // 7u:
                        SendMessageA(hWnd, 6u, 0, 0);
                        break;
                
                case WM_KEYDOWN: //0x100u:
                        if (wParam != VK_ESCAPE)  // 27
                                break;
                        PostQuitMessage(0);
                        break;

                default:
                        break;
                }       
        }
        return result;
 
}

auto create_main_dialog(int nCmdShow) -> int
{
        // play the launcher music
        std::cout << "playing music\n"; 
        PlaySoundA("LauncherMusic.wav", 0, 1u);

        // check if the install path exists
        bool installPathExists = install_path_exists();
        std::cout<< "installPathExists: " << installPathExists << "\n";

        // buffer for executable file
        FILE* setupExecutable;
        
        char* installPath;
        // if the install path exists determines if the game is installed or not
        if (installPathExists)
        {
                get_install_path((LPBYTE*)&installPath);
                std::cout<< "installPath: " << installPath << "\n";
                _chdir(installPath);
        }
        else // if it isn't then run the setup / installer executable
        {
                std::cout << "running game installer\n"; 
                setupExecutable = fopen("setup.exe", "r");
                if (setupExecutable)
                {
                        ShellExecuteA(0, "open", "setup.exe", 0, 0, 0);
                }
        }

        DWORD lastError = GetLastError();

        std::cout << "creating dialog param\n"; 
        HWND dialogParam = CreateDialogParamA(
                Oblivion::INSTANCE,
                MAKEINTRESOURCEA(102),
                0,
                dialog_callback,
                0);

        Oblivion::Window::MAIN_WINDOW = dialogParam;
        
        bool timeout = false;

        if (!dialogParam) {
                DWORD error = GetLastError();
                char buf[256];
                FormatMessageA(
                    FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
                    NULL,
                    error,
                    0,
                    buf,
                    sizeof(buf),
                    NULL);
                MessageBoxA(NULL, buf, "CreateDialogParamA failed", MB_OK | MB_ICONERROR);
                return -1;  // or handle error
        }
        
        if (dialogParam)
        {
                std::cout << "setting window text\n"; 
                SetWindowTextA(dialogParam, "The Elder Scrolls IV: Oblivion");

                if (timeout == true)
                {
                        std::cout << "dialog timeout\n"; 
                        SetTimer(dialogParam, 1u, 0x1Eu, 0);
                }
                std::cout<< "showing window...\n";
                ShowWindow(dialogParam, nCmdShow);
                std::cout<< "updating window...\n";
                UpdateWindow(dialogParam);

                MSG msg;
                while (GetMessage(&msg, NULL, 0, 0)) {
                        if (!IsDialogMessage(dialogParam, &msg)) {
                                TranslateMessage(&msg);
                                DispatchMessage(&msg);
                        }
                }
        }
        
        return 1;
}

auto main_thread(HINSTANCE hInstance = GetModuleHandle(nullptr), int nCmdShow = SW_SHOWNORMAL) -> int
{

        // get user documents path
        SHGetFolderPathA(0, CSIDL_COMMON_DOCUMENTS, 0, 0, Oblivion::USER_DOCUMENTS);

        // set registry key
        strcpy(Oblivion::REGISTRY_SUBKEY, "Software\\Bethesda Softworks\\Oblivion");

        // concat my games to user document path
        strcat(Oblivion::USER_DOCUMENTS, "\\My Games");
        CreateDirectoryA(Oblivion::USER_DOCUMENTS, 0);

        // concat documents to user document path
        strcat(Oblivion::USER_DOCUMENTS, "\\Oblivion\\");
        CreateDirectoryA(Oblivion::USER_DOCUMENTS, 0);

        // get local app data path
        SHGetFolderPathA(0, CSIDL_LOCAL_APPDATA, 0, 0, Oblivion::LOCAL_APP_DATA);

        // concat oblivion to local app data path
        strcat(Oblivion::LOCAL_APP_DATA, "\\Oblivion\\");
        CreateDirectoryA(Oblivion::LOCAL_APP_DATA, 0);

        std::cout << "USER_DOCUMENTS: " << Oblivion::USER_DOCUMENTS << "\n"; 
        std::cout << "LOCAL_APP_DATA: " << Oblivion::LOCAL_APP_DATA << "\n"; 

        strcat(Oblivion::INI_FILE, Oblivion::USER_DOCUMENTS);
        strcat(Oblivion::INI_FILE, "Oblivion.ini");
        std::cout << "INI_FILE: " << Oblivion::INI_FILE << "\n";

        strcat(Oblivion::PLUGINS_TXT, Oblivion::USER_DOCUMENTS);
        strcat(Oblivion::PLUGINS_TXT, "Plugins.txt");
        std::cout << "PLUGINS_TXT: " << Oblivion::PLUGINS_TXT << "\n"; 

        std::cout << Oblivion::STRING_TABLE.at(103) << "\n";

        std::cout << "creating window class\n"; 
        create_window_class();

        std::cout << "creating main dialog\n"; 
        Oblivion::RETURN_CODE = create_main_dialog(nCmdShow);

        return Oblivion::RETURN_CODE;
}

#ifdef _CONSOLE

auto main(int argc, char* argv[]) -> int
{
        std::cout<< "running oblivion on subsystem: CONSOLE" << "\n";
        return main_thread();
}

#else

int __stdcall WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
        std::cout<< "running oblivion on subsystem: WINDOWS" << "\n";
        return main_thread(hInstance, nCmdShow);
}

#endif