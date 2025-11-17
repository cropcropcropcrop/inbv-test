#ifndef INBV_DISPLAY_H
#define INBV_DISPLAY_H

#define WIN32_LEAN_AND_MEAN
#include <windows.h>

// INBV function prototypes
typedef VOID (NTAPI *INBV_DISPLAY_STRING_FN)(PCHAR String);
typedef VOID (NTAPI *INBV_SET_TEXT_COLOR_FN)(ULONG Color);
typedef VOID (NTAPI *INBV_SET_PROGRESS_BAR_SUBSET_FN)(ULONG SubsetIndex, ULONG MaximumValue);
typedef VOID (NTAPI *INBV_SET_PROGRESS_BAR_POSITION_FN)(ULONG Position);
typedef BOOLEAN (NTAPI *INBV_GET_DISPLAY_STATE_FN)(VOID);
typedef VOID (NTAPI *INBV_ACQUIRE_DISPLAY_OWNERSHIP_FN)(VOID);
typedef VOID (NTAPI *INBV_SET_DISPLAY_OWNERSHIP_FN)(BOOLEAN OwnDisplay);
typedef VOID (NTAPI *INBV_SET_SCROLL_REGION_FN)(ULONG x1, ULONG y1, ULONG x2, ULONG y2);

// Global function pointers
extern INBV_DISPLAY_STRING_FN InbvDisplayString;
extern INBV_SET_TEXT_COLOR_FN InbvSetTextColor;
extern INBV_SET_PROGRESS_BAR_SUBSET_FN InbvSetProgressBarSubset;
extern INBV_SET_PROGRESS_BAR_POSITION_FN InbvSetProgressBarPosition;
extern INBV_GET_DISPLAY_STATE_FN InbvGetDisplayState;
extern INBV_ACQUIRE_DISPLAY_OWNERSHIP_FN InbvAcquireDisplayOwnership;
extern INBV_SET_DISPLAY_OWNERSHIP_FN InbvSetDisplayOwnership;
extern INBV_SET_SCROLL_REGION_FN InbvSetScrollRegion;

// Define necessary types and constants that would normally come from WDK
#ifndef NT_SUCCESS
#define NT_SUCCESS(Status) ((NTSTATUS)(Status) >= 0)
#endif

// NTSTATUS codes
#ifndef STATUS_SUCCESS
#define STATUS_SUCCESS ((NTSTATUS)0x00000000L)
#endif

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

class InbvDisplay {
private:
    HMODULE hInbv;
    bool initialized;
    
    bool LoadInbvFunctions();
    
public:
    InbvDisplay();
    ~InbvDisplay();
    
    bool Initialize();
    void DisplayString(const char* message);
    void SetProgress(ULONG percent);
    void SetTextColor(ULONG color);
    bool IsInitialized() const { return initialized; }
    
    // Disable copy and move
    InbvDisplay(const InbvDisplay&) = delete;
    InbvDisplay& operator=(const InbvDisplay&) = delete;
    InbvDisplay(InbvDisplay&&) = delete;
    InbvDisplay& operator=(InbvDisplay&&) = delete;
};

// NTSTATUS codes
#ifndef STATUS_SUCCESS
#define STATUS_SUCCESS ((NTSTATUS)0x00000000L)
#endif

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
