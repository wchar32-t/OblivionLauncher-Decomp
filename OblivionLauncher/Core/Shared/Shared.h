#pragma once
#include <unordered_map>

namespace Oblivion
{
    static char USER_DOCUMENTS[255];
    static char LOCAL_APP_DATA[255];
    
    static char INI_FILE[255];
    static char PLUGINS_TXT[255];

    static char REGISTRY_SUBKEY[255];
    
    static int RETURN_CODE = 0;

    HINSTANCE INSTANCE = GetModuleHandleA(nullptr);

    static std::unordered_map<int, const char*> STRING_TABLE
    {
        {103, "Oblivion Launcher"},
        {104, "Master File"},
        {105, "Player File"},
        {106, "Unable to find any data files.  Your Oblivion installation appears to be damaged.  Please uninstall Oblivion\nand reinstall it from the original disc(s)."},
        {107, "This is not a valid Oblivion data file."},
        {108, "Master Files Required:"},
        {109, "OblivionLauncher"},
        {110, "Data Files:"},
        {111, "Created by:"},
            
        {112, "Created on:"},
        {113, "Last modified:"},
        {114, "Readme"},
        {115, "System Info"},
        {116, "Contacting 2K"},
        {117, "Warnings"},
        {118, "The Oblivion Launcher could not find the DirectX diagnostic tool (dxdiag.exe) on your system."},
        {119, "\r\n\r\nSystem information unavailable."},
        {120, "Could not find Readme.txt."},
        {121, "Invalid File"},
        {122, "This is not a valid Oblivion plugin or master file."},
        {123, "V. Sync"},
        {124, "Video settings have been set for Low Quality."},
        {125, "Video settings have been set for Medium Quality."},
        {126, "Video settings have been set for High Quality."},
        {127, "Video settings have been set for Ultra High Quality."},

        {128, "Oblivion will now detect your video hardware and set video options accordingly."},
        {129, "Video hardware unrecognized.  Video quality settings defaulting to Medium Quality."},
        {130, "Are you sure?"},

        {284, "HDR and antialiasing cannot be enabled at the same time.  Disabling antialiasing."},
        {285, "HDR and antialiasing cannot be enabled at the same time.  Disabling HDR."},
        {286, "(highest quality)"},
        {287, "None (best performance)"},

        {288, "samples"},
        {289, "Adapter:"},
        {290, "Resolution:"},
        {291, "Graphics Adapter and Resolution"},
        {292, "Antialiasing:"},
        {293, "Mode"},
        {294, "Windowed"},
        {295, "Fullscreen"},
        {296, "Screen Effects"},
        {297, "None"},
        {298, "Bloom"},
        {299, "HDR"},
        {300, "Distant Rendering"},
        {301, "Distant Landscape"},
        {302, "Distant Buildings"},
        {303, "Distant Trees"},

        {304, "Reset to Defaults"},
        {305, "Cancel"},
        {306, "OK"},
        {307, "(widescreen)"},
        {308, "HDR (unsupported)"},
        {309, "Unable to find file Oblivion.ini.  Your Oblivion installation appears to be damaged.  Please uninstall Oblivion\nand reinstall it from the original disc(s)."},
        {310, "Corrupted Installation"},
        {311, "Error: Unable to find current resolution in Oblivion.ini.  Using default."},
        {312, "Error: Unable to find current adapter in Oblivion.ini.  Using default."},
        {313, "Could not find Bethesda.txt."},
        {314, "\r\n\r\nWarnings.TXT was too large to display here.\r\n\r\nUse the \"Open Notepad\" button below to view this file.\r\n"},
        {315, "No warnings."},
        {316, "The Oblivion Launcher could not find the Oblivion game executable.  Please reinstall Oblivion."},
        {317, "The Oblivion Launcher could not find the uninstall program.\nRun the setup.exe on the Oblivion install disc to uninstall."},
    };
    
    namespace Resource
    {
        

        
    }
    
    namespace Window
    {

        static HWND MAIN_WINDOW;
        static WNDCLASSEXA MAIN_WINDOW_CLASS;

        
    }

    namespace Render
    {

        static IDirect3DDevice9* D3D_DEVICE;
        
    }
    
}
