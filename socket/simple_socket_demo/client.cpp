#include <iostream>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <netdb.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <string.h>
using namespace std;

#define BUFFER_SIZE 1024 


int main(int argc, char* argv[]) {
    if (argc != 3) {
        cout << "Please use './client ip port' to execute the client" << endl;
        cout << "Example: ./client 127.0.0.1 8088" << endl;
    }

    // 第一步，创建客户端socket
    int client_socket;
    if ((client_socket = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
        cout << "Creating socket failed!" << endl;
        return -1;
    }

    // 第二步，向服务器发起连接
    hostent* host_addr;
    if ((host_addr = gethostbyname(argv[1])) == 0) {
        cout << "Gethostbyname failed!" << endl;
        close(client_socket);
        return -1;
    }

    sockaddr_in server_addr;
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(atoi(argv[2]));
    memcpy(&server_addr.sin_addr, host_addr->h_addr, host_addr->h_length);

    if (connect(client_socket, (sockaddr*) &server_addr, sizeof(server_addr)) != 0) {
        cout << "Connecting server failed!" << endl;
        close(client_socket);
        return -1;
    }

    // 第三步，与服务器通信
    char send_buffer[BUFFER_SIZE];
    char recv_buffer[BUFFER_SIZE];
    while (1) {
        memset(send_buffer, 0, sizeof(send_buffer));
	    cout << "Client: ";
        string message;
        cin >> message;
        strcpy(send_buffer, message.c_str());
        int send_ret;
        if ((send_ret = send(client_socket, send_buffer, strlen(send_buffer), 0)) <= 0) {
            cout << "Send failed!" << endl;
            break;
        }

        memset(recv_buffer, 0, sizeof(recv_buffer));
        int recv_ret;
        if ((recv_ret = recv(client_socket, recv_buffer, sizeof(recv_buffer), 0)) <= 0) {
            cout << "Recv failed!" << endl;
            break;
        }
        cout << "Server: " << recv_buffer << endl;
    }

    close(client_socket);
    return 0;
}
