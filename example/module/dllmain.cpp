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
