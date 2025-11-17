cmake_minimum_required(VERSION 3.15)
project(INBV_Command_Runner
    VERSION 1.0
    DESCRIPTION "INBV Command Runner Application"
    LANGUAGES CXX
)

# Set C++ standard and properties
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Set output directories
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)

# Platform-specific settings
if(WIN32)
    add_definitions(-D_WIN32_WINNT=0x0A00)  # Target Windows 10
    add_compile_definitions(UNICODE _UNICODE)
    
    # Set Windows SDK version (if available)
    if(MSVC)
        set(CMAKE_VS_WINDOWS_TARGET_PLATFORM_VERSION "10.0.19041.0" CACHE STRING "Windows SDK version to use")
        set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")
    endif()
    
    # Add Windows-specific source files if needed
    set(PLATFORM_SOURCES 
        src/inbv_display.cpp
    )
else()
    message(WARNING "This application is primarily designed for Windows platform")
    set(PLATFORM_SOURCES 
        src/inbv_display_stub.cpp
    )
endif()

# Add executable
add_executable(${PROJECT_NAME}
    src/main.cpp
    ${PLATFORM_SOURCES}
)

# Include directories
target_include_directories(${PROJECT_NAME} PRIVATE 
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>
)

# Platform-specific linking
if(WIN32)
    target_link_libraries(${PROJECT_NAME} PRIVATE
        user32
        kernel32
        gdi32
        ntdll
    )
    
    # Set Windows subsystem (console)
    set_target_properties(${PROJECT_NAME} PROPERTIES
        WIN32_EXECUTABLE TRUE
    )
    
    # Post-build event to copy required DLLs if needed
    add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E echo "Build complete: $<TARGET_FILE:${PROJECT_NAME}>"
    )
endif()

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
