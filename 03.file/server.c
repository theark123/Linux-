#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>

#define PORT 8000
#define MAX_BUFFER_SIZE 1024
#define folder_path "/home/dell/workspace/fang/Windows/file/A/"
void upload_file(int client_socket, const char* filename) {
    // 构造服务器上保存文件的路径
    char file_path[MAX_BUFFER_SIZE];
    sprintf(file_path, "%s%s", folder_path, filename);

    FILE* file = fopen(file_path, "wb");
    if (file == NULL) {
        printf("无法创建文件: %s\n", file_path);
        return;
    }
    char buffer[MAX_BUFFER_SIZE];
    int bytes_received;
    printf("文件内容是:\n");
    while ((bytes_received = read(client_socket, buffer, sizeof(buffer))) > 0) {
        fwrite(buffer, 1, bytes_received, file);
        printf("%s", buffer);
    }
    fclose(file);
    printf("\n写入完成\n ");
}


void download_file(int client_socket, const char* filename) {
    // 构造服务器上要发送的文件的路径
    char file_path[MAX_BUFFER_SIZE];
    sprintf(file_path, "%s%s", folder_path, filename);

    FILE* file = fopen(file_path, "rb");
    if (file == NULL) {
        printf("文件未找到: %s\n", file_path);
        const char* not_found = "文件未找到";
        write(client_socket, not_found, strlen(not_found));
        return;
    }

    char buffer[MAX_BUFFER_SIZE];
    int bytes_read;
    while ((bytes_read = fread(buffer, 1, sizeof(buffer), file)) > 0) {
        write(client_socket, buffer, bytes_read);
    }

    fclose(file);
}


int main() {
    int server_fd, new_socket;
    struct sockaddr_in address;
    int addrlen = sizeof(address);
    char buffer[MAX_BUFFER_SIZE] = {0};

    //创建套接字 这里的代码将返回的套接字描述符赋值给变量 server_fd，以便后续在服务器程序中使用该套接字进行绑定、监听、接受连接等操作。
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
        perror("socket failed");
        exit(EXIT_FAILURE);
    }


    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);

    // 将套接字绑定到服务器地址和端口
    if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0) {
        perror("bind failed");
        exit(EXIT_FAILURE);
    }

    //开始监听连接请求
    if (listen(server_fd, 3) < 0) {
        perror("listen failed");
        exit(EXIT_FAILURE);
    }

    printf("Server listening on port %d\n", PORT);

        memset(buffer, 0, MAX_BUFFER_SIZE);
        if ((new_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t *)&addrlen)) < 0) {
            perror("accept failed");
            exit(EXIT_FAILURE);
        }
        printf("连接成功    ");

            read(new_socket, buffer, MAX_BUFFER_SIZE);

            char filename[MAX_BUFFER_SIZE] = {0};
            sscanf(buffer, "%s", filename);
            printf("文件名是: %s    \n", filename);
            memset(buffer, 0, MAX_BUFFER_SIZE);


            read(new_socket, buffer, MAX_BUFFER_SIZE);
            // 检查请求
            char request_type[10];
            sscanf(buffer, "%s", request_type);
            memset(buffer, 0, MAX_BUFFER_SIZE);

            if (strcmp(request_type, "1") == 0) {

                printf("下载文件到客户端\n");
                download_file(new_socket, filename);
                printf("下载成功.\n");
            }
            else if (strcmp(request_type, "2") == 0) {

                printf("上传文件到服务器\n");

                // 接收文件内容并保存到服务器
                upload_file(new_socket, filename);
                printf("上传成功.\n");
            }
            else if(strcmp(request_type, "3") == 0){
                printf("Received close command. Closing the server...\n");

            }
    close(new_socket);
    close(server_fd);
    return 0;
}
