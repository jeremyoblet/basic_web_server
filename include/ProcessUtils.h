#pragma once

#include <string>
#include <vector>
#include <stdexcept>

#ifdef _WIN32
  #include <windows.h>

  inline std::string exec(const std::vector<std::string>& args, const std::string& input) {
      // Concatène la ligne de commande (simple, sans guillemets avancés)
      std::string cmd;
      for (auto& a : args) { cmd += a; cmd += ' '; }
      if (!cmd.empty()) cmd.pop_back();

      SECURITY_ATTRIBUTES sa{ sizeof(SECURITY_ATTRIBUTES), nullptr, TRUE };
      HANDLE inR=nullptr,inW=nullptr,outR=nullptr,outW=nullptr;
      if (!CreatePipe(&inR,&inW,&sa,0)) throw std::runtime_error("CreatePipe stdin");
      if (!SetHandleInformation(inW, HANDLE_FLAG_INHERIT, 0)) throw std::runtime_error("SetHandleInformation stdin");
      if (!CreatePipe(&outR,&outW,&sa,0)) throw std::runtime_error("CreatePipe stdout");
      if (!SetHandleInformation(outR, HANDLE_FLAG_INHERIT, 0)) throw std::runtime_error("SetHandleInformation stdout");

      STARTUPINFOA si{};
      si.cb = sizeof(si);
      si.hStdInput  = inR;
      si.hStdOutput = outW;
      si.hStdError  = outW;
      si.dwFlags    = STARTF_USESTDHANDLES;

      PROCESS_INFORMATION pi{};
      std::string cmdCopy = cmd;
      if (!CreateProcessA(nullptr, cmdCopy.data(), nullptr, nullptr, TRUE, 0, nullptr, nullptr, &si, &pi)) {
          CloseHandle(inR); CloseHandle(inW); CloseHandle(outR); CloseHandle(outW);
          throw std::runtime_error("CreateProcess failed");
      }

      CloseHandle(inR);   // le processus lit sur inR
      DWORD written=0;
      if (!input.empty()) {
          WriteFile(inW, input.data(), (DWORD)input.size(), &written, nullptr);
      }
      CloseHandle(inW);   // fin d'entrée

      std::string out;
      char buf[4096];
      DWORD read=0;
      for (;;) {
          if (!ReadFile(outR, buf, sizeof(buf), &read, nullptr) || read==0) break;
          out.append(buf, buf+read);
      }
      CloseHandle(outR);
      CloseHandle(outW);

      WaitForSingleObject(pi.hProcess, INFINITE);
      CloseHandle(pi.hThread);
      CloseHandle(pi.hProcess);
      return out;
  }

#else
  #include <unistd.h>
  #include <sys/wait.h>
  #include <fcntl.h>

  inline std::string exec(const std::vector<std::string>& args, const std::string& input) {
      int inPipe[2], outPipe[2];
      if (pipe(inPipe)  == -1) throw std::runtime_error("pipe in");
      if (pipe(outPipe) == -1) { close(inPipe[0]); close(inPipe[1]); throw std::runtime_error("pipe out"); }

      pid_t pid = fork();
      if (pid < 0) { close(inPipe[0]); close(inPipe[1]); close(outPipe[0]); close(outPipe[1]); throw std::runtime_error("fork"); }

      if (pid == 0) {
          dup2(inPipe[0], STDIN_FILENO);
          dup2(outPipe[1], STDOUT_FILENO);
          dup2(outPipe[1], STDERR_FILENO);
          close(inPipe[1]); close(outPipe[0]);

          std::vector<char*> argv; argv.reserve(args.size()+1);
          for (auto& s : args) argv.push_back(const_cast<char*>(s.c_str()));
          argv.push_back(nullptr);
          execvp(argv[0], argv.data());
          _exit(127);
      }

      close(inPipe[0]); close(outPipe[1]);
      if (!input.empty()) write(inPipe[1], input.data(), input.size());
      close(inPipe[1]);

      std::string out;
      char buf[4096];
      ssize_t n;
      while ((n = read(outPipe[0], buf, sizeof buf)) > 0) out.append(buf, buf+n);
      close(outPipe[0]);
      int status=0; waitpid(pid, &status, 0);
      return out;
  }
#endif
