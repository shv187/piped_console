#pragma once

#include <optional>
#include <string>
#include <string_view>

namespace piped_console {
    struct Configuration {
        std::string_view m_pipe_name{ "\\\\.\\pipe\\piped_console" };
    };

    using HANDLE = void*;

    class Listener {
    public:
        Listener(const Configuration& configuration);
        ~Listener();

    private:
        bool createNamedPipe(std::string_view pipe_name);
        void destroyNamedPipe();

    public:
        bool connectToNamedPipe();
        bool disconnectFromNamedPipe();

        bool isConnected() const {
            return m_connected;
        }

        std::optional<std::string> getMessage();

    private:
        HANDLE m_pipe_handle{};
        bool m_connected{};
    };

    class Emitter {
    public:
        Emitter(const Configuration& configuration);
        ~Emitter();

        bool connectToNamedPipe(std::string_view pipe_name);
        bool disconnectFromNamedPipe();

        bool sendMessage(const std::string& message);

    private:
        HANDLE m_pipe_handle{};
    };
} // namespace piped_console
