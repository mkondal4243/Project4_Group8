# SecureLink – Smart Home Security System

SecureLink is a C++ and Qt-based smart home security system. It provides a graphical user interface to monitor and control simulated smart devices like door locks, garage, lights, and cameras. The app connects to a server using TCP sockets, handles user login, transfers log files, and displays motion alerts in real-time.

---

## Features

- Login system using socket-based authentication
- Dashboard to control and monitor smart devices
- Motion alert simulation and log export
- Log file transfer from server to client
- Server status indicator (live, reconnecting, offline)
- SQLite database to store access logs and login attempts
- Built with Qt for a modern GUI experience

---

## Requirements (macOS)

Make sure these are installed before running:

- Qt 5.15 (install using Homebrew: `brew install qt@5`)
- CMake
- Xcode Command Line Tools (`xcode-select --install`)
- C++ compiler (AppleClang or compatible)

---

## How to Build

```bash
cd Project4_Group8

# Clean previous build (optional)
rm -rf build

# Configure with CMake
cmake -S . -B build -DCMAKE_PREFIX_PATH="/opt/homebrew/Cellar/qt@5/5.15.16_1/lib/cmake"

# Build the project
cmake --build build
```

---

## How to Run

Open **two terminals** and run the following:

### Terminal 1: Launch the UI

```bash
./build/SecureLinkUI/SecureLinkUI.app/Contents/MacOS/SecureLinkUI
```

### Terminal 2: Start the server

```bash
./build/server
```

---

## Default Login

- **Username:** `admin`
- **Password:** `password123`

---

## Logs & Database

- Logs are saved under the `logs/` folder
- The SQLite database (`securelink.db`) is generated in the project root

---

## Team Members

- Mohammed Sujahat Ali  
- Mehak Kondal  
- Nonso Ekpunobi

---

## Notes

- Tested on macOS (M1/M2 chip)
- Project built using Qt Creator and VS Code
- Compatible with standard CMake-based workflows
