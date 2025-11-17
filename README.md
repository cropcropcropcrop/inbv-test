# INBV Command Runner

A Windows application that demonstrates interaction with the Initial Boot Video (INBV) subsystem for low-level display operations during system boot.

## Features

- Display text during Windows boot sequence
- Show progress bars with customizable appearance
- Control text colors and display regions
- Native Windows NT system calls for maximum compatibility

## Prerequisites

- Windows 10/11 SDK (10.0.19041.0 or later)
- Visual Studio 2019 or later with C++ workload
- CMake 3.15 or later
- Git

## Building Locally

1. Clone the repository:
   ```bash
   git clone https://github.com/yourusername/inbv-command-runner.git
   cd inbv-command-runner
   ```

2. Configure the build:
   ```bash
   mkdir build
   cd build
   cmake .. -A x64
   ```

3. Build the project:
   ```bash
   cmake --build . --config Release
   ```

The built executable will be in the `build/bin/Release` directory.

## Building with Visual Studio

1. Open the project folder in Visual Studio 2019 or later
2. Select "x64-Release" configuration
3. Build the solution (Ctrl+Shift+B)

## GitHub Actions

This project includes a GitHub Actions workflow that automatically builds the application on Windows. The workflow:

- Builds the application using MSVC
- Runs tests (if any)
- Uploads the built executable as an artifact

To use the GitHub Actions workflow:

1. Push your code to a GitHub repository
2. Navigate to the "Actions" tab in your repository
3. The workflow will run automatically on push to main/master or on pull requests

## Usage

1. Run the application with administrative privileges:
   ```
   inbv_runner.exe
   ```

2. Follow the on-screen instructions to interact with the INBV display

## License

This project is licensed under the MIT License - see the [LICENSE](LICENSE) file for details.

## Contributing

Contributions are welcome! Please feel free to submit a Pull Request.
