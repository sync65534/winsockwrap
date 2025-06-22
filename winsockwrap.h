#ifndef WINSOCKWRAP
#define WINSOCKWRAP
#include <winsock2.h>
#include <ws2tcpip.h>
#include <stdio.h>
#include <stdlib.h>
#include <optional>
class sckman {
    private:
        inline void throw_err(const char* context, int sock_fd, int WSAErrCode){
            fprintf(stderr, "%s failure on socket %d\nWSA Error code: %d\n", context, sock_fd, WSAErrCode);
        }
    public:
        int socket_fd;
        sockaddr_in addr;
        int addrlen = sizeof(addr);
        bool fatal_on_error = false;    // Setting default for entire object
        bool log_on_error = true;          // Setting default for entire object


        int Ssocket(int af, int type, int protocol, std::optional<bool> fatal = std::nullopt, std::optional<bool> log = std::nullopt){
            socket_fd = socket(af, type, protocol);
            if (socket_fd == INVALID_SOCKET){
                if (fatal.value_or(fatal_on_error)) { WSACleanup(); }
                int err_code =  WSAGetLastError();
                if (log.value_or(log_on_error)) {throw_err("Socket", socket_fd, err_code);}
                return err_code;
            }
            return 0;
        }
        int Sbind(std::optional<bool> fatal = std::nullopt, std::optional<bool> log = std::nullopt){    // Fills addr and addrlen from the objects addr
            if (bind(socket_fd, (sockaddr*)&addr, addrlen) == SOCKET_ERROR){
                if (fatal.value_or(fatal_on_error)) {WSACleanup();}
                int err_code = WSAGetLastError();
                if (log.value_or(log_on_error)) {throw_err("Bind", socket_fd, err_code);}
                closesocket(socket_fd);
                return err_code;
            }
            return 0;
        }
        int Slisten(int backlog, std::optional<bool> fatal = std::nullopt, std::optional<bool> log = std::nullopt){
            if (listen(socket_fd, backlog) == SOCKET_ERROR){
                if (fatal.value_or(fatal_on_error)) {WSACleanup();};
                int err_code = WSAGetLastError();
                closesocket(socket_fd);
                if (log.value_or(log_on_error)) {throw_err("Listen", socket_fd, err_code);}
                return err_code;
            }
            return 0;
        }
        int Srecv(char* buf, int len, int flags, std::optional<bool> fatal = std::nullopt, std::optional<bool> log = std::nullopt){
            int value_buf = recv(socket_fd, buf, len, flags);
            if (value_buf == SOCKET_ERROR){
                if (fatal.value_or(fatal_on_error)) {WSACleanup();};
                    int err_code = WSAGetLastError();
                    closesocket(socket_fd);
                    if (log.value_or(log_on_error)) {throw_err("Recv", socket_fd, err_code);}
                    return err_code;
                }
            return value_buf;
        }
        int Ssend(char* buf, int len, int flags, std::optional<bool> fatal = std::nullopt, std::optional<bool> log = std::nullopt){
            int value_buf = send(socket_fd, buf, len, flags); 
            if (value_buf == SOCKET_ERROR) {
                int err_code = WSAGetLastError();
                if (fatal.value_or(fatal_on_error)) { WSACleanup(); }
                if (log.value_or(log_on_error)) { throw_err("Send", socket_fd, err_code); }
                closesocket(socket_fd);
                return err_code;
            }
            return value_buf;
        }
        int Sshutdown(int how, std::optional<bool> fatal = std::nullopt, std::optional<bool> log = std::nullopt){
            if (shutdown(socket_fd, how) == SOCKET_ERROR){
                int err_code = WSAGetLastError();
                if (fatal.value_or(fatal_on_error)) {WSACleanup();};
                if (log.value_or(log_on_error)) {throw_err("Shutdown", socket_fd, err_code);}
                closesocket(socket_fd);
                return err_code;
            }
            return 0;
        }
        SOCKET Saccept(sockaddr* clientAddr, int* clientAddrLen, std::optional<bool> fatal = std::nullopt, std::optional<bool> log = std::nullopt){
            SOCKET value_buf = accept(socket_fd, clientAddr, clientAddrLen);
            if (value_buf == INVALID_SOCKET){
                if (fatal.value_or(fatal_on_error)) {WSACleanup();};
                int err_code = WSAGetLastError();
                closesocket(socket_fd);
                if (log.value_or(log_on_error)) {throw_err("Accept", socket_fd, err_code);}
                return err_code;
            }
            return value_buf;
        }
        
};
#endif
