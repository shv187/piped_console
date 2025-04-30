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
