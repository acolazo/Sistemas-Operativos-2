#include <sys/socket.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <string.h>

int main() {
    char buff[20];
    char content[200];
    int sd, connfd, len, bytes_read;

    struct sockaddr_in servaddr, cliaddr;

    sd = socket(AF_INET, SOCK_DGRAM, 0);

    if (sd == -1) {
        puts("socket not created in server");
        return 1;
    } else {
        puts("socket created in server");
    }

    bzero(&servaddr, sizeof(servaddr));

    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = INADDR_ANY;
    servaddr.sin_port = htons(7802);

    if (bind(sd, (struct sockaddr *)&servaddr, sizeof(servaddr)) != 0) {
        puts("Not binded");
        return 1;
    } else {
        puts("Binded");
    }

    len = sizeof(cliaddr);

    recvfrom(sd, buff, 1024, 0, (struct sockaddr *)&cliaddr, &len);

    printf("%s\n", buff);

    FILE *fp = fopen(buff, "r");
    if (fp == NULL) {
        printf("File does not exist \n");
        return 1;
    }

    while (1) {
        bytes_read = read(fp, content, sizeof(content));
        if (bytes_read == 0)
            break;
        sendto(sd, content, sizeof(content), 0, (struct sockaddr*)&cliaddr, len);
        bzero(content, 200);
    }
    strcpy(content, "end");
    sendto(sd, content, sizeof(content), 0, (struct sockaddr*)&cliaddr, len);

    return 0;
}