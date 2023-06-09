#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>

#define SERVER_IP "127.0.0.1"
#define PORT 8080
#define BUFFER_SIZE 1024

int main() {
    int sock = 0, valread;
    struct sockaddr_in serv_addr;
    char buffer[BUFFER_SIZE] = {0};

    // 创建套接字
    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        perror("Socket creation failed");
        exit(EXIT_FAILURE);
    }

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(PORT);

    // 将 IP 地址从字符串转换为网络字节序
    if (inet_pton(AF_INET, SERVER_IP, &serv_addr.sin_addr) <= 0) {
        perror("Invalid address/ Address not supported");
        exit(EXIT_FAILURE);
    }

    // 连接到服务器
    if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) {
        perror("Connection failed");
        exit(EXIT_FAILURE);
    }

    // 与服务器交互
    while (1) {
        // 输入要发送的消息
        printf("Enter message (type '010' to exit): ");
        fgets(buffer, BUFFER_SIZE, stdin);

        // 发送消息给服务器
        send(sock, buffer, strlen(buffer), 0);

        // 判断是否发送关闭命令
        if (strncmp(buffer, "010", 3)== 0) {
            printf("Sent close command. Closing the client...\n");
            break;
        }

        // 接收服务器的回复
        memset(buffer, 0, BUFFER_SIZE);
        valread = read(sock, buffer, BUFFER_SIZE);
        if (valread <= 0) {
            break;
        }

        printf("Received message from server: %s\n", buffer);
    }

    // 关闭套接字
    close(sock);
    return 0;
}
