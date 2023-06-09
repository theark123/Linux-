#include <stdio.h>
#include <stdlib.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <string.h>

int main() {
    while(1){
        char name[100];
        printf("输入域名(按0退出）: ");
        scanf("%s", name);
        if(strcmp(name,"0")==0)
            break;
        struct hostent *host = gethostbyname(name);
        if (host == NULL) {
            printf("错误地址\n");
            exit(1);
        }

        struct in_addr **addr_list = (struct in_addr **)host->h_addr_list;
        //in_addr是32位的ipv4地址

        for (int i = 0; addr_list[i] != NULL; i++) {
            printf("域名 %s 的IP地址是: %s\n", name, inet_ntoa(*addr_list[i]));
        }
    }
    return 0;
}
