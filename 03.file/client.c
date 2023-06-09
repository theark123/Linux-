#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>

#define SERVER_IP "127.0.0.1"
#define PORT 8000
#define MAX_BUFFER_SIZE 1024
#define folder_path "/home/dell/workspace/fang/Windows/file/B/"
void download_file(int sock, const char* filename) {
    // 构造客户端上保存文件的路径
    char file_path[MAX_BUFFER_SIZE];
    sprintf(file_path, "%s%s", folder_path,filename);
    FILE* file = fopen(file_path, "wb");
    if (file == NULL) {
        printf("无法创建文件: %s\n", file_path);
        return;
    }

    char buffer[MAX_BUFFER_SIZE];
    int bytes_received;
    printf("文件内容是:\n");
    while ((bytes_received = read(sock, buffer, sizeof(buffer))) > 0) {
        fwrite(buffer, 1, bytes_received, file);
        printf("%s", buffer);
    }

    fclose(file);
    printf("\n写入完成\n");
}

void upload_file(int sock, const char* filename) {
    // 构造客户端上要上传的文件的路径
    char file_path[MAX_BUFFER_SIZE];
    sprintf(file_path, "%s%s", folder_path, filename);

    FILE* file = fopen(file_path, "rb");
    if (file == NULL) {
        printf("文件未找到: %s\n", file_path);
        return;
    }

    char buffer[MAX_BUFFER_SIZE];
    int bytes_read;
    while ((bytes_read = fread(buffer, 1, sizeof(buffer), file)) > 0) {
        if (write(sock, buffer, bytes_read) != bytes_read) {
            printf("文件上传失败: %s\n", file_path);
            return;
        }
    }
    fclose(file);
}




int main() {
    int sock = 0;
    struct sockaddr_in serv_addr;
    char buffer[MAX_BUFFER_SIZE] = {0};

    // 创建套接字
    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        printf("Socket creation error\n");
        return -1;
    }

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(PORT);

    //将 IP 地址从字符串转换为网络字节序
    if (inet_pton(AF_INET, SERVER_IP, &serv_addr.sin_addr) <= 0) {
        printf("Invalid address/ Address not supported\n");
        return -1;
    }

    // 连接到服务器
    if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) {
        printf("Connection Failed\n");
        return -1;
    }
        printf("输入文件名:\n");
        char filename[MAX_BUFFER_SIZE] = {0};
        scanf("%s", &buffer);
        sscanf(buffer, "%s", filename);
        write(sock, buffer, MAX_BUFFER_SIZE);
        memset(buffer, 0, MAX_BUFFER_SIZE);
        printf("文件名是:%s\n",filename);

        printf("选择功能:\n");
        printf("1. 下载文件\n");
        printf("2. 上传文件\n");
        printf("3. 退出\n");

        scanf("%s", &buffer);
        if (strcmp(buffer, "1") == 0) {

            write(sock, buffer, MAX_BUFFER_SIZE);
            memset(buffer, 0, MAX_BUFFER_SIZE);
            printf("下载中\n");
            download_file(sock, filename);
            printf("下载成功.\n");
        } else if (strcmp(buffer, "2") == 0) {
            write(sock, buffer, MAX_BUFFER_SIZE);
            memset(buffer, 0, MAX_BUFFER_SIZE);
            printf("上传中\n");
            upload_file(sock, filename);
            printf("上传成功.\n");
        } else if (strcmp(buffer, "3")== 0)
                printf("Sent close command. Closing the client...\n");
        close(sock);
    return 0;
}
