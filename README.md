# Piped console
Did u ever need to log some messages to the console from another process?

Maybe You were unable to `AllocConsole` by yourself and had to visualize it in some other way?

Probably not...

But I did :)

# Preview
![image](https://github.com/user-attachments/assets/0156edce-6a4e-48aa-99a7-94d6ce6b46a4)


# Required
- CMake
- Clang/MSVC - cxx_23
- Windows

# Optional:
- Ninja
- Make

# Usage(as library)
- Get library in your prefered way, eg.
```
FetchContent_Declare(
    piped_console
    GIT_REPOSITORY https://github.com/shv187/piped_console.git
    GIT_TAG 7307304cbd0cff8c4000f69070fb3d26cf908cab)
FetchContent_MakeAvailable(piped_console)
```
- Link against it, eg.
```
target_link_libraries(your_target PRIVATE piped_console)
```
- Use it:

<details><summary> Emitter </summary>
  
```cpp
#include <iostream>
#include <stdexcept>
#include <string>

#include <windows.h>
#include <fmt/core.h>

#include "piped_console/piped_console.hpp"

int main() {
    SetConsoleTitleA("Emitter");

    try {
        piped_console::Configuration config{ "\\\\.\\pipe\\piped_console" };
        piped_console::Emitter emitter(config);

        fmt::println("[+] Connected to console");

        while (true) {
            fmt::print("Message: ");
            std::string message{};
            std::getline(std::cin, message);
            emitter.sendMessage(message);
        }
    } catch (const std::runtime_error& e) {
        MessageBoxA(nullptr, e.what(), "Error", MB_OK);
        return 1;
    }
}
```

</details>

<details><summary> Console </summary>
  
```cpp
#include <chrono>
#include <thread>

#include <windows.h>
#include <fmt/core.h>
#include <piped_console/piped_console.hpp>
#include <winuser.h>

int main() {
    SetConsoleTitleA("Console");

    try {
        piped_console::Configuration config{ "\\\\.\\pipe\\piped_console" };
        piped_console::Listener listener(config);

        while (true) {
            fmt::println("[+] Waiting for client connection...");

            if (listener.connectToNamedPipe()) {
                fmt::println("[+] Client connected");

                while (listener.isConnected()) {
                    const auto message = listener.getMessage();
                    if (!message || message.value().empty()) {
                        break;
                    }

                    fmt::println("[message] {}", message.value());
                }

                fmt::println("[?] Client disconnected");
                listener.disconnectFromNamedPipe();

                std::this_thread::sleep_for(std::chrono::milliseconds(100));
            } else {
                fmt::println("[!] Failed to connect to client");
                std::this_thread::sleep_for(std::chrono::seconds(1));
            }
        }
    } catch (const std::exception& e) {
        MessageBoxA(nullptr, e.what(), "Error", MB_OK);
        return 1;
    }
}
```

</details>

<details><summary> (Optional) Dynamic library :P </summary>

```cpp
#include <chrono>
#include <cstdint>
#include <cstdlib>
#include <thread>

#include <windows.h>
#include <fmt/core.h>
#include <libloaderapi.h>
#include <piped_console/piped_console.hpp>
#include <winuser.h>

uint32_t main_thread(HMODULE module_handle) {
    try {
        piped_console::Configuration config{ "\\\\.\\pipe\\piped_console" };
        piped_console::Emitter emitter(config);

        fmt::println("[+] Connected to console");

        while (!GetAsyncKeyState(VK_HOME)) {
            const auto message =
                std::format("Main module base address: 0x{:X}", reinterpret_cast<uintptr_t>(GetModuleHandle(nullptr)));
            emitter.sendMessage(message);

            std::this_thread::sleep_for(std::chrono::milliseconds(100));
        }
    } catch (const std::runtime_error& e) {
        MessageBoxA(nullptr, e.what(), "Error", MB_OK);
    }

    FreeLibraryAndExitThread(module_handle, EXIT_SUCCESS);
}

BOOL WINAPI DllMain(HINSTANCE module_handle, DWORD call_reason, LPVOID) {
    if (call_reason != DLL_PROCESS_ATTACH) {
        return TRUE;
    }

    if (const auto handle = CreateThread(nullptr, 0, reinterpret_cast<LPTHREAD_START_ROUTINE>(main_thread),
                                         module_handle, 0, nullptr)) {
        CloseHandle(handle);
    }

    return TRUE;
}
```

</details>

# Usage(if you want is to build and run examples)(Optional requirements are _required_ here, to simplify the process)

`git clone https://github.com/shv187/piped_console.git`

`cd piped_console`

`mkdir build`

`make run`

# Notes
Console supports emitter's disconnection and connection of a new one.

However one emitter won't connect to a new console if something (bad?) happens to the old one.
