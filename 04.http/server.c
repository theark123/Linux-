#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>

#define PORT 8000
#define MAX_BUFFER_SIZE 1024
#define filepath "/home/dell/workspace/fang/Windows/http/index.html"
int main() {
    int serverSocket, newSocket;
    struct sockaddr_in serverAddress, clientAddress;
    socklen_t clientLength;
    char buffer[MAX_BUFFER_SIZE] = {0};
    char httpResponse[MAX_BUFFER_SIZE] = {0};

    // 创建服务器套接字
    if ((serverSocket = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
        perror("Socket creation failed");
        exit(EXIT_FAILURE);
    }

    // 设置服务器地址
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_addr.s_addr = INADDR_ANY;
    serverAddress.sin_port = htons(PORT);

    // 将套接字绑定到指定的IP地址和端口
    if (bind(serverSocket, (struct sockaddr *)&serverAddress, sizeof(serverAddress)) < 0) {
        perror("Binding failed");
        exit(EXIT_FAILURE);
    }

    // 监听连接请求
    if (listen(serverSocket, 3) < 0) {
        perror("Listening failed");
        exit(EXIT_FAILURE);
    }

    printf("Server listening on port %d...\n", PORT);

    while (1) {
        // 接受新的连接
        if ((newSocket = accept(serverSocket, (struct sockaddr *)&clientAddress, &clientLength)) < 0) {
            perror("Accepting connection failed");
            exit(EXIT_FAILURE);
        }

        // 接收客户端请求
        memset(buffer, 0, sizeof(buffer));
        read(newSocket, buffer, MAX_BUFFER_SIZE - 1);
        printf("Received request:\n%s\n", buffer);

        // 获取文件路径
        FILE* file = fopen(filepath, "rb");
        if (file == NULL) {
            strcpy(httpResponse, "HTTP/1.1 404 Not Found\r\n\r\n");
        } else {
            strcpy(httpResponse, "HTTP/1.1 200 OK\r\nContent-Type: text/html; charset=utf-8\r\n\r\n");

            char line[MAX_BUFFER_SIZE];
            while (fgets(line, sizeof(line), file) != NULL) {
                strcat(httpResponse, line);
            }
            fclose(file);
        }

        // 发送HTTP响应
        write(newSocket, httpResponse, strlen(httpResponse));
        close(newSocket);
    }
    close(serverSocket);
    return 0;
}
