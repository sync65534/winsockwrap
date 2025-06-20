#ifndef WINSOCKWRAP
#define WINSOCKWRAP

#include <winsock2.h>
#include <ws2tcpip.h>
#include <stdio.h>
#include <stdlib.h>

class sckman {
    public:
        int sck_fd;
        sockaddr* addr;
        int addrlen;


        sckman(){
            addrlen = sizeof(addr);
        }
        int Ssocket(int af, int type, int protocol){
            sck_fd = socket(af, type, protocol);
            if (sck_fd == INVALID_SOCKET){
                WSACleanup();
                return WSAGetLastError();
            }
            return 0;
        }

        int Sbind(){    // Fills addr and addrlen from the objects addr
            if (bind(sck_fd, addr, addrlen) == SOCKET_ERROR){
                WSACleanup();
                closesocket(sck_fd);
                return WSAGetLastError();
            }
            return 0;
        }
        int Slisten(int backlog){
            if (listen(sck_fd, backlog) == SOCKET_ERROR){
                WSACleanup();
                closesocket(sck_fd);
                return WSAGetLastError();
            }
            return 0;
        }
        SOCKET Saccept(sockaddr* clientAddr, int* clientAddrLen){
            SOCKET value_buf = accept(sck_fd, clientAddr, clientAddrLen);
            if (value_buf == INVALID_SOCKET){
                WSACleanup();
                closesocket(sck_fd);
                return WSAGetLastError();
            }
            return value_buf;
        }
        int Srecv(char* buf, int len, int flags){
            int value_buf = recv(sck_fd, buf, len, flags);
            if (value_buf == SOCKET_ERROR){
                WSACleanup();
                closesocket(sck_fd);
                return WSAGetLastError();
            }
            return value_buf;
        }
        int Ssend(char* buf, int len, int flags){
            int value_buf = send(sck_fd, buf, len, flags); 
            if (value_buf == SOCKET_ERROR){
                WSACleanup();
                closesocket(sck_fd);
                return WSAGetLastError();
            }
            return value_buf;
        }
        int Sshutdown(int how){
            if (shutdown(sck_fd, how) == SOCKET_ERROR){
                WSACleanup();
                closesocket(sck_fd);
            }
            return 0;
        }

};
#endif
