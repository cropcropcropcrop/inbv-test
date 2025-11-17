#ifndef INBV_DISPLAY_H
#define INBV_DISPLAY_H

#define WIN32_NO_STATUS
#include <windows.h>
#include <winternl.h>
#include <ntstatus.h>
#include <ntddk.h>
#include <ntddvdeo.h>
#include <inbv.h>

// Undefine the status macros to avoid conflicts
#undef STATUS_SUCCESS
#undef STATUS_UNSUCCESSFUL

// Native NT function pointers
typedef NTSTATUS (NTAPI *PNT_QUERY_SYSTEM_INFORMATION)(
    IN SYSTEM_INFORMATION_CLASS SystemInformationClass,
    OUT PVOID SystemInformation,
    IN ULONG SystemInformationLength,
    OUT PULONG ReturnLength OPTIONAL
);

typedef VOID (NTAPI *RTL_INIT_UNICODE_STRING)(PUNICODE_STRING, PCWSTR);
typedef NTSTATUS (NTAPI *ZW_DEVICE_IO_CONTROL_FILE)(
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

class InbvDisplay {
private:
    // Function pointers for native APIs
    PNT_QUERY_SYSTEM_INFORMATION NtQuerySystemInformation;
    RTL_INIT_UNICODE_STRING RtlInitUnicodeString;
    ZW_DEVICE_IO_CONTROL_FILE ZwDeviceIoControlFile;
    
    // INBV function pointers
    VOID (NTAPI *InbvAcquireDisplayOwnership)();
    VOID (NTAPI *InbvNotifyDisplayOwnershipChange)(IN PVOID Callback, IN PVOID Context);
    VOID (NTAPI *InbvSetTextColor)(ULONG Color);
    VOID (NTAPI *InbvSetScrollRegion)(ULONG x1, ULONG y1, ULONG x2, ULONG y2);
    VOID (NTAPI *InbvSetDisplayOwnership)(BOOLEAN OwnDisplay);
    VOID (NTAPI *InbvDisplayString)(PCHAR String);
    BOOLEAN (NTAPI *InbvGetDisplayState)();
    VOID (NTAPI *InbvSetProgressBarSubset)(ULONG SubsetIndex, ULONG MaximumValue);
    VOID (NTAPI *InbvSetProgressBarPosition)(ULONG Position);
    
    // Module handles
    HMODULE hNtDll;
    HMODULE hInbv;
    
    bool initialized;
    
public:
    InbvDisplay();
    ~InbvDisplay();
    
    bool Initialize();
    void DisplayString(const char* message);
    void SetProgress(ULONG percent);
    void SetTextColor(ULONG color);
    bool IsInitialized() const { return initialized; }
};

// Color definitions for INBV
#define INBV_COLOR_BLACK     0x00
#define INBV_COLOR_BLUE      0x01
#define INBV_COLOR_GREEN     0x02
#define INBV_COLOR_CYAN      0x03
#define INBV_COLOR_RED       0x04
#define INBV_COLOR_MAGENTA   0x05
#define INBV_COLOR_YELLOW    0x06
#define INBV_COLOR_WHITE     0x07
#define INBV_COLOR_BRIGHT    0x08

#endif // INBV_DISPLAY_H
