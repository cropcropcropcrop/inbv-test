cmake_minimum_required(VERSION 3.10)
project(INBV_Command_Runner VERSION 1.0.0 LANGUAGES CXX)

# Set C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Set output directories
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

# Set Windows-specific options
if(WIN32)
    # Common Windows definitions for native applications
    add_compile_definitions(
        _WINDOWS
        UNICODE
        _UNICODE
        _WIN32_WINNT=0x0A00  # Windows 10
        NTDDI_VERSION=0x0A000000
        _NATIVE_SUBSYSTEM
    )
    
    # Set Windows SDK version (if available)
    if(MSVC)
        set(CMAKE_VS_WINDOWS_TARGET_PLATFORM_VERSION "10.0.19041.0" CACHE STRING "Windows SDK version to use")
        
        # MSVC-specific compiler flags
        add_compile_options(
            /W4
            /WX
            /EHsc
        )
    endif()
    
    # Set native subsystem and entry point for native applications
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} /SUBSYSTEM:NATIVE /ENTRY:mainCRTStartup")
    set(CMAKE_RC_FLAGS "-D_WIN32")
else()
    message(WARNING "This application is primarily designed for Windows platform")
endif()

# Add executable with source files
add_executable(${PROJECT_NAME}
    src/main.cpp
    src/inbv_display.cpp
)

# Set target properties
set_target_properties(${PROJECT_NAME} PROPERTIES
    WIN32_EXECUTABLE FALSE
    MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>"
)

# Include directories
target_include_directories(${PROJECT_NAME} PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)

# Link libraries
if(WIN32)
    target_link_libraries(${PROJECT_NAME} PRIVATE
        ntdll
        user32
        gdi32
    )
    
    # Set Windows subsystem to NATIVE
    set_target_properties(${PROJECT_NAME} PROPERTIES
        LINK_FLAGS "/SUBSYSTEM:NATIVE /ENTRY:mainCRTStartup /NODEFAULTLIB:kernel32.lib"
    )
    
    # Add defines for Windows 10
    target_compile_definitions(${PROJECT_NAME} PRIVATE
        _WIN32_WINNT=0x0A00
        NTDDI_VERSION=0x0A000000
        WINVER=0x0A00
    )
endif()

# Install target
install(TARGETS ${PROJECT_NAME}
    RUNTIME DESTINATION bin
    LIBRARY DESTINATION lib
    ARCHIVE DESTINATION lib
)
# Installation
install(TARGETS ${PROJECT_NAME}
    RUNTIME DESTINATION bin
    LIBRARY DESTINATION lib
    ARCHIVE DESTINATION lib
)

# Enable testing if needed
option(ENABLE_TESTING "Enable testing" OFF)
if(ENABLE_TESTING)
    enable_testing()
    # Add your tests here
    # add_test(NAME test_${PROJECT_NAME} COMMAND ${PROJECT_NAME} --test)
endif()
