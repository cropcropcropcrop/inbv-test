#include "inbv_display.h"
#include <stdexcept>
#include <string>
#include <winternl.h>
#include <ntstatus.h>

// INBV function pointers
INBV_DISPLAY_STRING_FN InbvDisplayString = nullptr;
INBV_SET_TEXT_COLOR_FN InbvSetTextColor = nullptr;
INBV_SET_PROGRESS_BAR_SUBSET_FN InbvSetProgressBarSubset = nullptr;
INBV_SET_PROGRESS_BAR_POSITION_FN InbvSetProgressBarPosition = nullptr;
INBV_GET_DISPLAY_STATE_FN InbvGetDisplayState = nullptr;
INBV_ACQUIRE_DISPLAY_OWNERSHIP_FN InbvAcquireDisplayOwnership = nullptr;
INBV_SET_DISPLAY_OWNERSHIP_FN InbvSetDisplayOwnership = nullptr;
INBV_SET_SCROLL_REGION_FN InbvSetScrollRegion = nullptr;

// Function typedefs
typedef NTSTATUS(NTAPI* _RtlGetVersion)(PRTL_OSVERSIONINFOW);

extern "C" {
    // Forward declare NtDeviceIoControlFile
    NTSYSAPI NTSTATUS NTAPI NtDeviceIoControlFile(
        HANDLE FileHandle,
        HANDLE Event,
        PIO_APC_ROUTINE ApcRoutine,
        PVOID ApcContext,
        PIO_STATUS_BLOCK IoStatusBlock,
        ULONG IoControlCode,
        PVOID InputBuffer,
        ULONG InputBufferLength,
        PVOID OutputBuffer,
        ULONG OutputBufferLength
    );

    // Forward declare RtlInitUnicodeString
    NTSYSAPI VOID NTAPI RtlInitUnicodeString(
        PUNICODE_STRING DestinationString,
        PCWSTR SourceString
    );
}

// NT function pointers
static _RtlGetVersion pRtlGetVersion = nullptr;

// INBV function names
constexpr const char* INBV_DISPLAY_STRING_FN_NAME = "InbvDisplayString";
constexpr const char* INBV_SET_TEXT_COLOR_FN_NAME = "InbvSetTextColor";
constexpr const char* INBV_SET_PROGRESS_BAR_SUBSET_FN_NAME = "InbvSetProgressBarSubset";
constexpr const char* INBV_SET_PROGRESS_BAR_POSITION_FN_NAME = "InbvSetProgressBarPosition";
constexpr const char* INBV_GET_DISPLAY_STATE_FN_NAME = "InbvGetDisplayState";
constexpr const char* INBV_ACQUIRE_DISPLAY_OWNERSHIP_FN_NAME = "InbvAcquireDisplayOwnership";
constexpr const char* INBV_SET_DISPLAY_OWNERSHIP_FN_NAME = "InbvSetDisplayOwnership";
constexpr const char* INBV_SET_SCROLL_REGION_FN_NAME = "InbvSetScrollRegion";

// Helper function to get INBV function pointer
template<typename T>
static T GetInbvFunction(HMODULE hModule, const char* functionName) {
    if (!hModule) return nullptr;
    return reinterpret_cast<T>(GetProcAddress(hModule, functionName));
}

// Internal function to load INBV functions
bool InbvDisplay::LoadInbvFunctions() {
    // Try to load win32kbase.sys or win32kfull.sys
    HMODULE hInbv = LoadLibraryExW(L"win32kbase.sys", NULL, DONT_RESOLVE_DLL_REFERENCES);
    if (!hInbv) {
        hInbv = LoadLibraryExW(L"win32kfull.sys", NULL, DONT_RESOLVE_DLL_REFERENCES);
        if (!hInbv) {
            return false;
        }
    }

    // Get INBV function pointers
    InbvDisplayString = GetInbvFunction<INBV_DISPLAY_STRING_FN>(hInbv, "InbvDisplayString");
    InbvSetTextColor = GetInbvFunction<INBV_SET_TEXT_COLOR_FN>(hInbv, "InbvSetTextColor");
    InbvSetProgressBarSubset = GetInbvFunction<INBV_SET_PROGRESS_BAR_SUBSET_FN>(hInbv, "InbvSetProgressBarSubset");
    InbvSetProgressBarPosition = GetInbvFunction<INBV_SET_PROGRESS_BAR_POSITION_FN>(hInbv, "InbvSetProgressBarPosition");
    InbvGetDisplayState = GetInbvFunction<INBV_GET_DISPLAY_STATE_FN>(hInbv, "InbvGetDisplayState");
    InbvAcquireDisplayOwnership = GetInbvFunction<INBV_ACQUIRE_DISPLAY_OWNERSHIP_FN>(hInbv, "InbvAcquireDisplayOwnership");
    InbvSetDisplayOwnership = GetInbvFunction<INBV_SET_DISPLAY_OWNERSHIP_FN>(hInbv, "InbvSetDisplayOwnership");
    InbvSetScrollRegion = GetInbvFunction<INBV_SET_SCROLL_REGION_FN>(hInbv, "InbvSetScrollRegion");

    // We only require the display string function to be available
    return (InbvDisplayString != nullptr);
}

InbvDisplay::InbvDisplay() 
    : hInbv(nullptr),
      initialized(false) {
}

InbvDisplay::~InbvDisplay() {
    // No need to free ntdll
    hInbv = nullptr;
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

    // Try to acquire display ownership if possible
    if (InbvAcquireDisplayOwnership) {
        InbvAcquireDisplayOwnership();
    }

    // Set default text color (white on black)
    if (InbvSetTextColor) {
        InbvSetTextColor(0x0F);
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
        // Call the real INBV function
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
