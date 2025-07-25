
#include <winsock2.h>
#include <ws2tcpip.h>
#include <windows.h>
#include <iostream>
#include <thread>
#include <chrono>
#include <tchar.h>
using namespace std;

int main(int argc, char* argv[]) {
    SOCKET serverSocket, acceptSocket;
    int port= 55555;
    WSADATA wsaData;;
    int wsaerr;
    WORD wVersionRequested = MAKEWORD(2,2);
    wsaerr = WSAStartup(wVersionRequested, &wsaData);
    if(wsaerr != 0){
        cout << "Winsock Initialization failed" << endl;
        return 0;
    }else{
        cout << "Winsock Initialization successful" << endl;
        cout << "status" << wsaData.szSystemStatus << endl;
    }
    
    serverSocket = INVALID_SOCKET;
    serverSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if(serverSocket == INVALID_SOCKET){
        cout << "Socket creation failed" << WSAGetLastError() << endl;
        WSACleanup();
        return 0;
    }else{
        cout << "Socket creation successful" << endl;
    }
    sockaddr_in service;
    service.sin_family = AF_INET;
    InetPton(AF_INET, _T("192.168.0.103"), &service.sin_addr.s_addr);
    service.sin_port = htons(port);
    if(bind(serverSocket, (SOCKADDR*)&service, sizeof(service)) == SOCKET_ERROR){
        cout << "Bind failed with error" << WSAGetLastError() << endl;
        closesocket(serverSocket);
        WSACleanup();
        return 0;
    }else{
        cout << "Bind successful" << endl;
    }

    if(listen(serverSocket, 1) == SOCKET_ERROR){
        cout << "Listen failed with error" << WSAGetLastError() << endl;
    }else{
        cout << "Listen successful" << endl;
    }

    acceptSocket = accept(serverSocket, NULL, NULL);
    if (acceptSocket == INVALID_SOCKET){
        cout << "Accept failed with error" << WSAGetLastError() << endl;
    }
    cout << "Accept successful" << endl;
    
    char buffer[200];
    int byteCount = recv(acceptSocket, buffer, 200, 0);
    if(byteCount > 0){
        cout << "Message Received: " << buffer << endl;
    }else{
        WSACleanup();
    }
    system("pause");
    WSACleanup();
}