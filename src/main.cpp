#include "inbv_display.h"
#include <iostream>
#include <string>
#include <thread>
#include <chrono>

void RunCommand(const std::string& cmd, InbvDisplay& display) {
    display.DisplayString(("Executing: " + cmd + "\n").c_str());
    
    // In a real implementation, you would use CreateProcess or system()
    // For demonstration, we'll just simulate command execution
    for (int i = 0; i <= 10; ++i) {
        display.SetProgress(i * 10);
        std::this_thread::sleep_for(std::chrono::milliseconds(200));
    }
    
    display.DisplayString("Command completed.\n");
}

void ShowMenu(InbvDisplay& display) {
    display.DisplayString("\n=== INBV Command Runner ===\n");
    display.DisplayString("1. Run system info\n");
    display.DisplayString("2. Show disk information\n");
    display.DisplayString("3. Display network info\n");
    display.DisplayString("4. Exit\n");
    display.DisplayString("Select an option: ");
}

int main() {
    InbvDisplay display;
    
    // Initialize INBV display
    if (!display.Initialize()) {
        std::cerr << "Failed to initialize INBV display. Make sure you're running with appropriate privileges.\n";
        return 1;
    }
    
    display.SetTextColor(INBV_COLOR_WHITE | INBV_COLOR_BRIGHT);
    display.DisplayString("=== INBV Command Runner Initialized ===\n");
    
    bool running = true;
    while (running) {
        display.SetTextColor(INBV_COLOR_WHITE);
        ShowMenu(display);
        
        // In a real application, you would read input from the user
        // For this example, we'll simulate user input
        int choice = 4; // Default to exit
        // Simulate user input (in a real app, use GetAsyncKeyState or similar)
        std::this_thread::sleep_for(std::chrono::seconds(2));
        
        display.DisplayString("4\n"); // Simulate user entering '4'
        
        switch (choice) {
            case 1:
                display.SetTextColor(INBV_COLOR_CYAN);
                RunCommand("systeminfo", display);
                break;
            case 2:
                display.SetTextColor(INBV_COLOR_GREEN);
                RunCommand("wmic diskdrive list brief", display);
                break;
            case 3:
                display.SetTextColor(INBV_COLOR_YELLOW);
                RunCommand("ipconfig /all", display);
                break;
            case 4:
                display.SetTextColor(INBV_COLOR_RED);
                display.DisplayString("\nExiting INBV Command Runner...\n");
                running = false;
                break;
            default:
                display.SetTextColor(INBV_COLOR_RED);
                display.DisplayString("\nInvalid option!\n");
        }
        
        if (running) {
            display.DisplayString("\nPress any key to continue...");
            // Wait for key press in a real application
            std::this_thread::sleep_for(std::chrono::seconds(1));
        }
    }
    
    return 0;
}
