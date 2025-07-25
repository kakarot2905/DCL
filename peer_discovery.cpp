//COMPILE :  g++ peer_discovery.cpp -o peer_discovery.exe -std=c++17 -lws2_32 -pthread
//COMPLETE COMPILE : g++ peer_discovery.cpp -o peer_discovery.exe -static -static-libgcc -static-libstdc++ -std=c++17 -pthread -lws2_32







#define _WINSOCK_DEPRECATED_NO_WARNINGS

#include <winsock2.h>
#include <ws2tcpip.h>
#include <windows.h>
#include <iostream>
#include <thread>
#include <chrono>

#pragma comment(lib, "ws2_32.lib")


#define BROADCAST_PORT 54000
#define BROADCAST_IP "192.168.1.255"
#define BUFFER_SIZE 1024

#if !defined(__cpp_lib_thread) && (!defined(_GLIBCXX_HAS_GTHREADS) || !defined(_GLIBCXX_USE_C99_STDINT_TR1))
#error "Your compiler does not support std::thread. Please use a newer version of MinGW-w64 or MSVC."
#endif

void sendBroadcast(SOCKET& sock, sockaddr_in& broadcastAddr, const std::string& message) {
    std::cout << "[SENDING] To " << inet_ntoa(broadcastAddr.sin_addr) << ": " << message << std::endl;
    sendto(sock, message.c_str(), message.length(), 0, (sockaddr*)&broadcastAddr, sizeof(broadcastAddr));
}

void listenForPeers(SOCKET& sock) {
    char buffer[BUFFER_SIZE];
    sockaddr_in senderAddr;
    int senderLen = sizeof(senderAddr);

    // Get local IP address
    char hostName[256];
    gethostname(hostName, sizeof(hostName));
    hostent* localHost = gethostbyname(hostName);
    char* localIP = inet_ntoa(*(struct in_addr*)localHost->h_addr_list[0]);

    while (true) {
        int bytesReceived = recvfrom(sock, buffer, BUFFER_SIZE, 0, (sockaddr*)&senderAddr, &senderLen);
        if (bytesReceived > 0) {
            buffer[bytesReceived] = '\0';
            
            char* senderIP = inet_ntoa(senderAddr.sin_addr);

            // Ignore messages from self
            if (strcmp(senderIP, localIP) == 0)
                continue;

            std::cout << "[RECEIVED] From " << senderIP << ": " << buffer << std::endl;
        }
    }
}


int main() {
    WSADATA wsaData;
    WSAStartup(MAKEWORD(2, 2), &wsaData);

    SOCKET udpSocket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);

    BOOL broadcastEnable = TRUE;
    setsockopt(udpSocket, SOL_SOCKET, SO_BROADCAST, (char*)&broadcastEnable, sizeof(broadcastEnable));

    sockaddr_in broadcastAddr;
    broadcastAddr.sin_family = AF_INET;
    broadcastAddr.sin_port = htons(BROADCAST_PORT);
    broadcastAddr.sin_addr.s_addr = inet_addr(BROADCAST_IP);

    sockaddr_in recvAddr;
    recvAddr.sin_family = AF_INET;
    recvAddr.sin_port = htons(BROADCAST_PORT);
    recvAddr.sin_addr.s_addr = INADDR_ANY;
    bind(udpSocket, (sockaddr*)&recvAddr, sizeof(recvAddr));
    std::thread listener(listenForPeers, std::ref(udpSocket));

    while (true) {
        sendBroadcast(udpSocket, broadcastAddr, "Hello from peer!");
        std::this_thread::sleep_for(std::chrono::seconds(1));
    }

    closesocket(udpSocket);
    WSACleanup();
    return 0;
}
