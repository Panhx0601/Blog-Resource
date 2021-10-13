#include <iostream>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <netdb.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
using namespace std;


int main(int argc, char* argv[]) {
    if (argc != 2) {
        cout << "Please use './server port' to execute the server" << endl;
        cout << "Example: ./server 8088" << endl;
    }

    // 第一步，创建服务器的socket
    int server_socket;
    if ((server_socket = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
        cout << "Creating socket failed!" << endl;
        return -1;
    }

    //第二步，服务器通信地址和端口绑定到服务器socket
    sockaddr_in server_addr;
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = htonl(INADDR_ANY); // 指定任意IP地址为服务器IP地址
    // server_addr.sin_addr.s_addr = inet_addr("172.0.0.1");
    server_addr.sin_port = htons(atoi(argv[1]));

    if (bind(server_socket, (sockaddr*) &server_addr, sizeof(server_addr))) {
        cout << "Bind socket failed!" << endl;
        close(server_socket);
        return -1;
    }

    // 第三步，设置socket为监听状态
    if (listen(server_socket, 5) != 0) {
        cout << "Listen client failed!" << endl;
        close(server_socket);
        return -1;
    }

    // 第四步，接收客户端的连接
    int client_socket;
    int addr_len = sizeof(sockaddr_in);
    sockaddr_in client_addr;
    client_socket = accept(server_socket, (sockaddr*) &client_addr, (socklen_t*) &addr_len);
    cout << "客户端：" << inet_ntoa(client_addr.sin_addr) << " 已连接" << endl;

    // 第五步，与客户端通信
    const int BUFFER_SIZE = 1024;
    char recv_buffer[BUFFER_SIZE];
    char send_buffer[BUFFER_SIZE];
    while (1) {
        memset(recv_buffer, 0, sizeof(recv_buffer));
        int recv_ret;
        if ((recv_ret = recv(client_socket, recv_buffer, sizeof(recv_buffer), 0) <= 0)) {
            cout << "Receive failed! recv_ret = " + recv_ret << endl; 
            break;
        }
        cout << "Client: " << recv_buffer << endl;

        int send_ret;
        strcpy(send_buffer, "OK");
        if ((send_ret = send(client_socket, send_buffer, strlen(send_buffer), 0) <= 0)) {
            cout << "Send failed! send_ret = " + send_ret << endl;
            break;
	}
        cout << "Server: " << send_buffer << endl;
    }

    close(server_socket);
    close(client_socket);
    return 0;
}
