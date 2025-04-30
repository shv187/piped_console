#include "piped_console/piped_console.hpp"

#include <stdexcept>
#include <string>
#include <string_view>

#include <windows.h>

namespace piped_console {
    Listener::Listener(const Configuration& configuration) {
        if (!createNamedPipe(configuration.m_pipe_name)) {
            throw std::runtime_error("Listener couldn't create named pipe");
        }
    }

    Listener::~Listener() {
        if (isConnected()) {
            disconnectFromNamedPipe();
        }

        destroyNamedPipe();
    }

    bool Listener::createNamedPipe(std::string_view pipe_name) {
        m_pipe_handle =
            CreateNamedPipeA(pipe_name.data(), PIPE_ACCESS_INBOUND, PIPE_WAIT, 1, 0, 1024, 60 * 60 * 1000, NULL);
        return m_pipe_handle != INVALID_HANDLE_VALUE;
    }

    void Listener::destroyNamedPipe() {
        CloseHandle(m_pipe_handle);
    }

    bool Listener::connectToNamedPipe() {
        m_connected = ConnectNamedPipe(m_pipe_handle, NULL);
        return m_connected;
    }

    bool Listener::disconnectFromNamedPipe() {
        const auto status = DisconnectNamedPipe(m_pipe_handle);
        if (status) {
            m_connected = false;
        }
        return status;
    }

    std::optional<std::string> Listener::getMessage() {
        char buffer[1024]{};
        DWORD dwRead{};
        if (ReadFile(m_pipe_handle, buffer, sizeof(buffer) - 1, &dwRead, NULL)) {
            return std::string(buffer);
        }
        return {};
    }

    /* --- */

    Emitter::Emitter(const Configuration& configuration) {
        if (!connectToNamedPipe(configuration.m_pipe_name)) {
            throw std::runtime_error("Emitter couldn't connect to named pipe");
        }
    }

    Emitter::~Emitter() {
        disconnectFromNamedPipe();
    }

    bool Emitter::connectToNamedPipe(std::string_view pipe_name) {
        m_pipe_handle = CreateFileA(pipe_name.data(), GENERIC_WRITE, 0, NULL, OPEN_EXISTING, 0, NULL);
        return m_pipe_handle != INVALID_HANDLE_VALUE;
    }

    bool Emitter::disconnectFromNamedPipe() {
        return CloseHandle(m_pipe_handle);
    }

    bool Emitter::sendMessage(const std::string& message) {
        DWORD written{};
        WriteFile(m_pipe_handle, message.data(), message.size(), &written, NULL);
        return written == message.size();
    }
} // namespace piped_console
