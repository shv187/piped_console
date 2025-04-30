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
