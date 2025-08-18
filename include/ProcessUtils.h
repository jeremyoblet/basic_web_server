#pragma once

#include <stdexcept>
#include <memory>
#include <vector>
#include <stdexcept>

#ifdef _WIN32
    #include <windows.h>
#endif


// voir https://learn.microsoft.com/fr-fr/windows/win32/procthread/creating-a-child-process-with-redirected-input-and-output?redirectedfrom=MSDN
std::string exec(const std::vector<std::string>& args, const std::string& input) {
    std::string command;
    for (const auto& arg : args)
    {
        command += arg + " ";
    }

    // Remove the trailing space
    if (!command.empty())
    {
        command.pop_back();
    }

    // Create pipes for the process's STDOUT, STDERR, and STDIN
    HANDLE hStdOutRead, hStdOutWrite;
    HANDLE hStdErrRead, hStdErrWrite;
    HANDLE hStdInRead, hStdInWrite;

    SECURITY_ATTRIBUTES saAttr = { sizeof(SECURITY_ATTRIBUTES), NULL, TRUE };
    if (!CreatePipe(&hStdOutRead, &hStdOutWrite, &saAttr, 0) ||
        !CreatePipe(&hStdErrRead, &hStdErrWrite, &saAttr, 0) ||
        !CreatePipe(&hStdInRead, &hStdInWrite, &saAttr, 0))
    {
        return "";
    }

    // Set the write handle to be inheritable by the child process
    SetHandleInformation(hStdOutRead, HANDLE_FLAG_INHERIT, 0);
    SetHandleInformation(hStdErrRead, HANDLE_FLAG_INHERIT, 0);
    SetHandleInformation(hStdInWrite, HANDLE_FLAG_INHERIT, 0);

    PROCESS_INFORMATION pi;
    STARTUPINFOA si = { sizeof(STARTUPINFOA), nullptr, nullptr, nullptr, 0,
                        0, 0, 0, 0, 0, 0,
                STARTF_USESTDHANDLES, 0, 0, nullptr, hStdInRead,
                hStdOutWrite, hStdErrWrite };

    if (!CreateProcessA(nullptr, const_cast<char*>(command.c_str()), nullptr, nullptr, TRUE, 0, nullptr, nullptr, &si, &pi))
    {
        return "";
    }

    // Write the input to the stdin pipe
    DWORD bytesWritten;
    if (!WriteFile(hStdInWrite, input.c_str(), input.size(), &bytesWritten, nullptr))
    {
        throw std::runtime_error("Failed to write to stdin");
    }

    // Close the write ends of the pipes
    CloseHandle(hStdOutWrite);
    CloseHandle(hStdErrWrite);
    CloseHandle(hStdInRead);
    CloseHandle(hStdInWrite);

    // Read the output
    std::string output;
    CHAR buffer[128];
    memset(buffer, 0, sizeof(buffer));
    DWORD bytesRead;
    BOOL success = FALSE;

    for (;;)
    {
        success = ReadFile(hStdOutRead, buffer, sizeof(buffer) - 1, &bytesRead, NULL);
        if (!success || bytesRead == 0) break;
        buffer[bytesRead] = '\0';
        output += buffer;
    }

    // for (;;)
    // {
    //     success = ReadFile(hStdErrRead, buffer, sizeof(buffer) - 1, &bytesRead, NULL);
    //     if (!success || bytesRead == 0) break;
    //     buffer[bytesRead] = '\0';
    //     output += buffer;
    // }

    // Wait until child process exits
    WaitForSingleObject(pi.hProcess, INFINITE);

    // Close handles
    CloseHandle(pi.hProcess);
    CloseHandle(pi.hThread);
    CloseHandle(hStdOutRead);
    CloseHandle(hStdErrRead);

    return output;
}
#else
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <array>
std::string exec(const std::vector<std::string>& args, const std::string& input) {
    int stdout_pipe[2];
    int stderr_pipe[2];
    int stdin_pipe[2];

    if (pipe(stdout_pipe) == -1 || pipe(stderr_pipe) == -1 || pipe(stdin_pipe) == -1) {
        throw std::runtime_error("Failed to create pipes");
    }

    pid_t pid = fork();
    if (pid == -1) {
        throw std::runtime_error("Failed to fork process");
    } else if (pid == 0) {
        // Child process
        close(stdout_pipe[0]);
        dup2(stdout_pipe[1], STDOUT_FILENO);
        close(stdout_pipe[1]);

        close(stderr_pipe[0]);
        dup2(stderr_pipe[1], STDERR_FILENO);
        close(stderr_pipe[1]);

        close(stdin_pipe[1]);
        dup2(stdin_pipe[0], STDIN_FILENO);
        close(stdin_pipe[0]);

        std::vector<char*> cargs;
        for (const auto& arg : args) {
            cargs.push_back(const_cast<char*>(arg.c_str()));
        }
        cargs.push_back(nullptr);

        execvp(cargs[0], cargs.data());
        _exit(EXIT_FAILURE); // If execvp fails
    } else {
        // Parent process
        close(stdout_pipe[1]);
        close(stderr_pipe[1]);
        close(stdin_pipe[0]);

        // Write the input to the stdin pipe
        if (write(stdin_pipe[1], input.c_str(), input.size()) == -1) {
            throw std::runtime_error("Failed to write to stdin pipe");
        }
        close(stdin_pipe[1]);

        // Read the output from the stdout and stderr pipes
        std::array<char, 128> buffer;
        std::string result;
        ssize_t count;

        while ((count = read(stdout_pipe[0], buffer.data(), buffer.size())) > 0) {
            result.append(buffer.data(), count);
        }
        close(stdout_pipe[0]);

        while ((count = read(stderr_pipe[0], buffer.data(), buffer.size())) > 0) {
            result.append(buffer.data(), count);
        }
        close(stderr_pipe[0]);

        // Wait for the child process to exit
        int status;
        waitpid(pid, &status, 0);

        return result;
    }
}

#endif

