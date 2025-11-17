#include "inbv_display.h"
#include <stdexcept>
#include <string>
#include <Windows.h>

// INBV function names
constexpr const char* INBV_DISPLAY_STRING_FN_NAME = "InbvDisplayString";
constexpr const char* INBV_SET_TEXT_COLOR_FN_NAME = "InbvSetTextColor";
constexpr const char* INBV_SET_PROGRESS_BAR_SUBSET_FN_NAME = "InbvSetProgressBarSubset";
constexpr const char* INBV_SET_PROGRESS_BAR_POSITION_FN_NAME = "InbvSetProgressBarPosition";
constexpr const char* INBV_GET_DISPLAY_STATE_FN_NAME = "InbvGetDisplayState";
constexpr const char* INBV_ACQUIRE_DISPLAY_OWNERSHIP_FN_NAME = "InbvAcquireDisplayOwnership";
constexpr const char* INBV_SET_DISPLAY_OWNERSHIP_FN_NAME = "InbvSetDisplayOwnership";
constexpr const char* INBV_SET_SCROLL_REGION_FN_NAME = "InbvSetScrollRegion";

// Helper function to get a function pointer from a module
template<typename T>
static T GetFunctionPointer(HMODULE hModule, const char* functionName) {
    if (!hModule) return nullptr;
    return reinterpret_cast<T>(GetProcAddress(hModule, functionName));
}

// Internal function to load INBV functions
bool InbvDisplay::LoadInbvFunctions() {
    // Try to load win32kbase.sys or win32kfull.sys
    hInbv = LoadLibraryW(L"win32kbase.sys");
    if (!hInbv) {
        hInbv = LoadLibraryW(L"win32kfull.sys");
        if (!hInbv) {
            // Fall back to user32.dll for some functions
            hInbv = LoadLibraryW(L"user32.dll");
            if (!hInbv) {
                return false;
            }
        }
    }

    // Get INBV function pointers using the template helper
    InbvDisplayString = GetFunctionPointer<INBV_DISPLAY_STRING_FN>(hInbv, INBV_DISPLAY_STRING_FN_NAME);
    InbvSetTextColor = GetFunctionPointer<INBV_SET_TEXT_COLOR_FN>(hInbv, INBV_SET_TEXT_COLOR_FN_NAME);
    InbvSetProgressBarSubset = GetFunctionPointer<INBV_SET_PROGRESS_BAR_SUBSET_FN>(hInbv, INBV_SET_PROGRESS_BAR_SUBSET_FN_NAME);
    InbvSetProgressBarPosition = GetFunctionPointer<INBV_SET_PROGRESS_BAR_POSITION_FN>(hInbv, INBV_SET_PROGRESS_BAR_POSITION_FN_NAME);
    InbvGetDisplayState = GetFunctionPointer<INBV_GET_DISPLAY_STATE_FN>(hInbv, INBV_GET_DISPLAY_STATE_FN_NAME);
    InbvAcquireDisplayOwnership = GetFunctionPointer<INBV_ACQUIRE_DISPLAY_OWNERSHIP_FN>(hInbv, INBV_ACQUIRE_DISPLAY_OWNERSHIP_FN_NAME);
    InbvSetDisplayOwnership = GetFunctionPointer<INBV_SET_DISPLAY_OWNERSHIP_FN>(hInbv, INBV_SET_DISPLAY_OWNERSHIP_FN_NAME);
    InbvSetScrollRegion = GetFunctionPointer<INBV_SET_SCROLL_REGION_FN>(hInbv, INBV_SET_SCROLL_REGION_FN_NAME);

    // We only require the display string function to be available
    return (InbvDisplayString != nullptr);
}

InbvDisplay::InbvDisplay() 
    : hInbv(nullptr),
      InbvDisplayString(nullptr),
      InbvSetTextColor(nullptr),
      InbvSetProgressBarSubset(nullptr),
      InbvSetProgressBarPosition(nullptr),
      InbvGetDisplayState(nullptr),
      InbvAcquireDisplayOwnership(nullptr),
      InbvSetDisplayOwnership(nullptr),
      InbvSetScrollRegion(nullptr),
      initialized(false) {
}

InbvDisplay::~InbvDisplay() {
    if (hInbv) {
        FreeLibrary(hInbv);
        hInbv = nullptr;
    }
}

bool InbvDisplay::Initialize() {
    if (initialized) {
        return true;
    }

    // Load INBV functions
    if (!LoadInbvFunctions()) {
        OutputDebugStringA("Failed to load INBV functions\n");
        return false;
    }

    // Try to take ownership of the display if possible
    if (InbvAcquireDisplayOwnership) {
        InbvAcquireDisplayOwnership();
    }

    // Set up display if possible
    if (InbvSetDisplayOwnership) {
        InbvSetDisplayOwnership(TRUE);
    }

    // Set default scroll region if possible
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
    } else {
        // Fallback to OutputDebugString if INBV is not available
        OutputDebugStringA(message);
    }
}

void InbvDisplay::SetProgress(ULONG percent) {
    if (!initialized) return;
    
    if (InbvSetProgressBarSubset && InbvSetProgressBarPosition) {
        // Set the progress bar range (0-100)
        InbvSetProgressBarSubset(0, 100);
        // Set the current position
        InbvSetProgressBarPosition(percent > 100 ? 100 : percent);
    } else {
        // Fallback: Just print the progress to debug output
        char buffer[64];
        sprintf_s(buffer, "Progress: %lu%%\n", percent > 100 ? 100 : percent);
        OutputDebugStringA(buffer);
    }
}

void InbvDisplay::SetTextColor(ULONG color) {
    if (initialized && InbvSetTextColor) {
        InbvSetTextColor(color);
    }
    // No fallback for color setting
}
