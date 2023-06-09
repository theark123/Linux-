#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#define PORT 8080
#define BUFFER_SIZE 1024

int main() {
    int server_fd, new_socket, valread;
    struct sockaddr_in address;
    int addrlen = sizeof(address);
    char buffer[BUFFER_SIZE] = {0};

    // 创建套接字 这里的代码将返回的套接字描述符赋值给变量 server_fd，以便后续在服务器程序中使用该套接字进行绑定、监听、接受连接等操作。
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
        perror("Socket creation failed");
        exit(EXIT_FAILURE);
    }

    // 设置服务器地址和端口
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = inet_addr("127.0.0.1");
    address.sin_port = htons(PORT);

    // 将套接字绑定到服务器地址和端口
    if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0) {
        perror("Bind failed");
        exit(EXIT_FAILURE);
    }

    // 开始监听连接请求
    if (listen(server_fd, 2) < 0) {
        perror("Listen failed");
        exit(EXIT_FAILURE);
    }
    printf("Server listening on port %d\n", PORT);
    // 等待客户端连接
    if ((new_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t *)&addrlen)) < 0) {
        perror("Accept failed");
        exit(EXIT_FAILURE);
    }

    // 接收和发送消息
    while (1) {
        // 接收客户端消息
        memset(buffer, 0, BUFFER_SIZE);
        valread = read(new_socket, buffer, BUFFER_SIZE);
        if (valread <= 0) {
            break;
        }

        printf("Received message from client: %s\n", buffer);

        // 判断是否收到关闭命令
        if (strncmp(buffer, "010", 3) == 0) {
            printf("Received close command. Closing the server...\n");
            break;
        }
        // 发送客户端消息给客户端
        send(new_socket, buffer, strlen(buffer), 0);
    }

    // 关闭套接字
    close(new_socket);
    close(server_fd);
    return 0;
}