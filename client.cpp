
#include <winsock2.h>
#include <ws2tcpip.h>
#include <windows.h>
#include <iostream>
#include <thread>
#include <chrono>
#include <tchar.h>
using namespace std;

int main(int argc, char* argv[]){
    SOCKET clientSocket;
    int port = 55555;
    WSADATA wsaData;
    int wsaerr;
    WORD wVersionRequested = MAKEWORD(2,2);
    wsaerr = WSAStartup(wVersionRequested, &wsaData);
    if(wsaerr != 0){
        cout << "Winsock Initialization failed" << endl;
    }else{
        cout << "Winsock Initialization successful" << endl;
        cout << "status" << wsaData.szSystemStatus << endl;
    }
    clientSocket = INVALID_SOCKET;
    clientSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if(clientSocket == INVALID_SOCKET){
        cout << "Socket creation failed" << WSAGetLastError() << endl;
        WSACleanup();
    }else{
        cout << "Socket creation successful" << endl;
    }

    sockaddr_in clientService;
    clientService.sin_family = AF_INET;
    InetPton(AF_INET, _T("192.168.0.103"), &clientService.sin_addr.s_addr);
    clientService.sin_port = htons(port);
    if(connect(clientSocket, (SOCKADDR*)&clientService, sizeof(clientService)) == SOCKET_ERROR){
        cout << "Connect failed with error" << WSAGetLastError() << endl;
        WSACleanup();
        return 0;
    }else{
        cout << "Connect successful" << endl;
    }
    

    char buffer[200];
    cin.getline(buffer, 200);
    int byteCount = send(clientSocket, buffer, 200, 0);
    if(byteCount > 0){
        cout << "Message Sent: " << buffer << endl;
    }else{
        WSACleanup();
    }
    system("pause");
    WSACleanup();
    return 0;
    
}