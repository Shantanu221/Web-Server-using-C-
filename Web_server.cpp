#define _WINSOCK_DEPRECATED_NO_WARNINGS
// #pragma comment(lib,"ws2_32.lib")

#include<bits/stdc++.h>
// #include<iostream>
#include<winsock2.h>
#include<string>
#pragma comment(lib,"ws2_32.lib")

using namespace std;

int main(){
    cout<<"Attempting to create a server\n";
    SOCKET wsocket;
    SOCKET new_wsocket;
    WSADATA wsaData;
    struct sockaddr_in server;
    int server_len;
    int BUFFER_SIZE = 30720;

    //initialize
    if(WSAStartup(MAKEWORD(2,2),&wsaData)!=0){
        cout<<"Could not initialize\n";
    }
    //creating a socket
    wsocket = socket(AF_INET,SOCK_STREAM,IPPROTO_TCP);
    if(wsocket==INVALID_SOCKET){
        cout<<"Could not create socket\n";
    }
    //bind socket to address
    server.sin_family = AF_INET;
    server.sin_addr.s_addr = inet_addr("127.0.0.1");
    server.sin_port =htons(8080);
    server_len = sizeof(server);

    if(bind(wsocket,(SOCKADDR*)&server,server_len)!=0){
        cout<<"Could not bind socket\n";
    }

    //listen to address
    if(listen(wsocket,20)!=0){
        cout<<"Could not start listening\n";
    }

    cout<<"Listening on 127.0.0.1.8080 \n";
    int bytes=0;
    while(true){
        //accept client request
        new_wsocket = accept(wsocket,(SOCKADDR*)&server,&server_len);
        if(new_wsocket==INVALID_SOCKET){
            cout<<"Could not accept\n";
        }

        //read request
        char buff[30720]={0};
        bytes = recv(new_wsocket,buff,BUFFER_SIZE,0);
        if(bytes<0){
            cout<<"Could not read client request\n";
        }

        string serverMessage = "HTTP/1.1 200 OK\nContent-Type: text/html\n Content_Length: ";
        string response = "<html><body><font size=\"6\">Text in cpp !</font></body></html>";
        serverMessage.append(to_string(response.size()));
        serverMessage.append("\n\n");
        serverMessage.append(response);

        int bytesSent = 0;
        int TotalBytesSent = 0;

        while(TotalBytesSent<serverMessage.size()){
            bytesSent = send(new_wsocket,serverMessage.c_str(),serverMessage.size(),0);
            if(bytesSent<0){
                cout<<"Could not send respond\n";
            }
            TotalBytesSent+=bytesSent;
        }
        cout<<"Sent response to client\n";

        closesocket(new_wsocket);
    }
    closesocket(wsocket);
    WSACleanup();


    return 0;
}