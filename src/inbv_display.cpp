#include "inbv_display.h"
#include <stdexcept>

// Define necessary constants and structures
#define IOCTL_VIDEO_QUERY_SUPPORTED_BRIGHTNESS 0x230410
#define IOCTL_VIDEO_QUERY_DISPLAY_BRIGHTNESS 0x230414
#define IOCTL_VIDEO_SET_DISPLAY_BRIGHTNESS 0x230418

// Internal function to get NtDll function pointers
bool InbvDisplay::LoadNtDllFunctions() {
    hNtDll = LoadLibraryW(L"ntdll.dll");
    if (!hNtDll) {
        return false;
    }

    NtQuerySystemInformation = (PNT_QUERY_SYSTEM_INFORMATION)GetProcAddress(hNtDll, "NtQuerySystemInformation");
    RtlInitUnicodeString = (RTL_INIT_UNICODE_STRING)GetProcAddress(hNtDll, "RtlInitUnicodeString");
    ZwDeviceIoControlFile = (ZW_DEVICE_IO_CONTROL_FILE)GetProcAddress(hNtDll, "NtDeviceIoControlFile");

    return (NtQuerySystemInformation != nullptr && 
            RtlInitUnicodeString != nullptr && 
            ZwDeviceIoControlFile != nullptr);
}

// Internal function to load INBV functions
bool InbvDisplay::LoadInbvFunctions() {
    hInbv = LoadLibraryW(L"win32kbase.sys");
    if (!hInbv) {
        hInbv = LoadLibraryW(L"win32kfull.sys");
        if (!hInbv) {
            return false;
        }
    }

    // Get INBV function pointers
    *(FARPROC*)&InbvAcquireDisplayOwnership = GetProcAddress(hInbv, "InbvAcquireDisplayOwnership");
    *(FARPROC*)&InbvNotifyDisplayOwnershipChange = GetProcAddress(hInbv, "InbvNotifyDisplayOwnershipChange");
    *(FARPROC*)&InbvSetTextColor = GetProcAddress(hInbv, "InbvSetTextColor");
    *(FARPROC*)&InbvSetScrollRegion = GetProcAddress(hInbv, "InbvSetScrollRegion");
    *(FARPROC*)&InbvSetDisplayOwnership = GetProcAddress(hInbv, "InbvSetDisplayOwnership");
    *(FARPROC*)&InbvDisplayString = GetProcAddress(hInbv, "InbvDisplayString");
    *(FARPROC*)&InbvGetDisplayState = GetProcAddress(hInbv, "InbvGetDisplayState");
    *(FARPROC*)&InbvSetProgressBarSubset = GetProcAddress(hInbv, "InbvSetProgressBarSubset");
    *(FARPROC*)&InbvSetProgressBarPosition = GetProcAddress(hInbv, "InbvSetProgressBarPosition");

    return (InbvDisplayString != nullptr && 
            InbvSetTextColor != nullptr &&
            InbvSetProgressBarPosition != nullptr);
}

InbvDisplay::InbvDisplay() 
    : NtQuerySystemInformation(nullptr),
      RtlInitUnicodeString(nullptr),
      ZwDeviceIoControlFile(nullptr),
      InbvAcquireDisplayOwnership(nullptr),
      InbvNotifyDisplayOwnershipChange(nullptr),
      InbvSetTextColor(nullptr),
      InbvSetScrollRegion(nullptr),
      InbvSetDisplayOwnership(nullptr),
      InbvDisplayString(nullptr),
      InbvGetDisplayState(nullptr),
      InbvSetProgressBarSubset(nullptr),
      InbvSetProgressBarPosition(nullptr),
      hNtDll(nullptr),
      hInbv(nullptr),
      initialized(false) {
}

InbvDisplay::~InbvDisplay() {
    if (hInbv) {
        FreeLibrary(hInbv);
        hInbv = nullptr;
    }
    if (hNtDll) {
        FreeLibrary(hNtDll);
        hNtDll = nullptr;
    }
}

bool InbvDisplay::Initialize() {
    if (initialized) {
        return true;
    }

    // Load NT DLL functions
    if (!LoadNtDllFunctions()) {
        return false;
    }

    // Load INBV functions
    if (!LoadInbvFunctions()) {
        if (hNtDll) {
            FreeLibrary(hNtDll);
            hNtDll = nullptr;
        }
        return false;
    }

    // Take ownership of the display
    if (InbvAcquireDisplayOwnership) {
        InbvAcquireDisplayOwnership();
    }

    // Set up display
    if (InbvSetDisplayOwnership) {
        InbvSetDisplayOwnership(TRUE);
    }

    // Set default scroll region (full screen)
    if (InbvSetScrollRegion) {
        InbvSetScrollRegion(0, 0, 79, 24);  // Standard 80x25 text mode
    }

    initialized = true;
    return true;
}

void InbvDisplay::DisplayString(const char* message) {
    if (initialized && InbvDisplayString) {
        // Convert to non-const as the INBV functions expect PCHAR
        InbvDisplayString(const_cast<PCHAR>(message));
    }
}

void InbvDisplay::SetProgress(ULONG percent) {
    if (initialized && InbvSetProgressBarSubset && InbvSetProgressBarPosition) {
        // Set the progress bar range (0-100)
        InbvSetProgressBarSubset(0, 100);
        // Set the current position
        InbvSetProgressBarPosition(percent > 100 ? 100 : percent);
    }
}

void InbvDisplay::SetTextColor(ULONG color) {
    if (initialized && InbvSetTextColor) {
        InbvSetTextColor(color);
    }
}
