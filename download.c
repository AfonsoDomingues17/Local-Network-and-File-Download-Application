#include <stdio.h>
#include <sys/socket.h>
#include <netdb.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>

#include <string.h>

#define SERVER_PORT 6000
#define SERVER_ADDR "192.168.28.96"

//Different Answers
#define FTP_PORT 21
#define WELCOME_AN 220
#define PASSWORD_ANS 331
#define LOGIN_ANS 230
#define PASV_ANS 227
#define OPEN_BINARY_ANS 150
#define OPEN_BINARY_ANS2 125
#define TRANSFER_COMP_ANS 226
#define QUIT_ANS 221

#define PARAM_SIZE 512

//TODO::struct with all the parametters of the URL
struct parameters {
    char user[PARAM_SIZE];
    char password[PARAM_SIZE];
    char ip[40]; // ipv4: 16; ipv6: 40.
    char host[PARAM_SIZE];
    char path[PARAM_SIZE];
    // char file[PARAM_SIZE];
    // char * file;
    unsigned file;
};
//TODO::defines da porta e das respostas

int parseURL(char *input, struct parameters *param){

    if (strncmp(input, "ftp://", 6) != 0) {
        printf("Error: URL must start with 'ftp://'\n");
        return -1;
    }
    input += 6;
    memset(param,0,sizeof(struct parameters)); //initialize struct values to empty string
    
    char *at_sign = strchr(input, '@'); //look for @ in the url
    if (at_sign){
        char *delimiter = strchr(input,':');
        strncpy(param->user,input,delimiter-input); //extract the user
        param->user[delimiter-input] = '\0';

        strncpy(param->password,delimiter+1,at_sign-delimiter-1); //extract the password
        param->password[at_sign-delimiter-1] = '\0';

        input = at_sign + 1;
    }
    else{
        strncpy(param->user,"anonymous",PARAM_SIZE);
        strncpy(param->password,"anonymous",PARAM_SIZE);
    }
    struct hostent *h;

    char *slash = strchr(input,'/');
    strncpy(param->host,input,slash-input); //extract the host

    strncpy(param->path,slash+1,PARAM_SIZE); //extract the path

    char * p = strrchr(param->path, '/');
    param->file = p - param->path + 1;
    if (p == NULL) param->file = 0;
/*     char *last_slash = strrchr(input, '/'); //extract the file
    strncpy(param->file,last_slash+1,PARAM_SIZE); */

    if ((h = gethostbyname(param->host)) == NULL) {
        printf("Invalid hostname '%s'\n", param->host);
        exit(-1);
    }
    strcpy(param->ip, inet_ntoa(*((struct in_addr *) h->h_addr))); //trasnform the host into an ip address

    return 0;
}

int openSocket(char *IP,int port){

    int sockfd;
    struct sockaddr_in server_addr;

    /*server address handling*/
    bzero((char *) &server_addr, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = inet_addr(IP);    /*32 bit Internet address network byte ordered*/
    server_addr.sin_port = htons(port);  

    /*open a TCP socket*/
    if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        perror("socket()");
        exit(-1);
    }


    /*connect to the server*/
    if (connect(sockfd, (struct sockaddr *) &server_addr,sizeof(server_addr)) < 0) {
        perror("connect()");
        exit(-1);
    }

    return sockfd;
}

/**
 * @brief Reads answer of the socket.
 * 
 * @param sockfd Socket file descriptor.
 * @param answer Buffer to store answer.
 * @param size Answer buffer size.
 * @return int - Returns the code of the answer if successful and -1 otherwise.
 */
int readAnswer(int sockfd, char *answer, unsigned size){
    int ret = -1;
    char byte;
    int i = 0;
    while (read(sockfd, &byte, 1) > 0) {
        if (i < size - 1) {
            answer[i] = byte;
            i++;
        }
        if (byte == '\n') {
            answer[i] = '\0';
            printf("A: %s", answer);
            if ( // If last line, break
                (answer[0] >= '0' && answer[0] <= '9') &&
                (answer[1] >= '0' && answer[1] <= '9') &&
                (answer[2] >= '0' && answer[2] <= '9') &&
                answer[3] == ' '
            ) {break;}
            i = 0;
        }
    }    
    sscanf(answer, "%d", &ret);

    return ret;
}

int main(int argc, char **argv) {

    struct parameters param;

    if (argc != 2) printf("Usage: %s ftp://[<user>:<password>@]<host>/<url-path>\n", argv[0]);

    if(parseURL(argv[1],&param) != 0){
        printf("How to call: ftp://[<user>:<password>@]<host>/<url-path>\n");
        exit(1);
    }
    printf("user:   %s\n",param.user);
    printf("pw:     %s\n",param.password);
    printf("file i: %d\n",param.file);
    printf("file:   %s\n",param.path + param.file);
    printf("host:   %s\n",param.host);
    printf("ip:     %s\n",param.ip);
    printf("path:   %s\n",param.path);

    char answer[1024] = {0};
    int socket_A = openSocket(param.ip,FTP_PORT);
    int code = readAnswer(socket_A, answer, 1024);

    if(code != WELCOME_AN){
        printf("code=%d\n", code);
        printf("Error: Wrong answer received, supposed to be welcome!\n");
        exit(1);
    }
    
    char cmd[6 + PARAM_SIZE] = {0};
    strcpy(cmd, "user ");
    strcat(cmd, param.user);
    strcat(cmd, "\r\n");
   
    write(socket_A,cmd,strlen(cmd));
    printf("cmd: %s", cmd);
    memset(answer, 0, 1024);
    code = readAnswer(socket_A,answer, 1024);
    if(code != PASSWORD_ANS){
        printf("code:%d\n",code);
        printf("Error: Wrong answer received, supposed to request password!\n");
        exit(1);
    }
    
    strcpy(cmd, "pass ");
    strcat(cmd, param.password);
    strcat(cmd, "\r\n");

    write(socket_A, cmd, strlen(cmd));
    printf("cmd: %s", cmd);
    memset(answer, 0, 1024);

    code = readAnswer(socket_A,answer, 1024);
    if(code != LOGIN_ANS){
        printf("Error: Wrong answer received, supposed to be login!\n");
        exit(1);
    }
    strcpy(cmd, "pasv\r\n");

    write(socket_A,cmd,6);
    printf("cmd: %s", cmd);
    memset(answer, 0, 1024);

    code = readAnswer(socket_A,answer, 1024);

    if(code != PASV_ANS){
        printf("Error: Wrong answer received, supposed to be pasv info!\n");
        exit(1);
    }

    unsigned char _i[4] = {0};
    unsigned char _p[2] = {0};
    char * beg = strchr(answer, '(');
    sscanf(beg, "(%hhd,%hhd,%hhd,%hhd,%hhd,%hhd)", _i, _i+1, _i+2, _i+3, _p, _p+1);
    
    int port = _p[0] * 256 + _p[1];
    char ip[40] = {0};
    sprintf(ip, "%d.%d.%d.%d", _i[0], _i[1], _i[2], _i[3]);
    int socket_B = openSocket(ip, port);
    
    strcpy(cmd, "retr ");
    strcat(cmd, param.path);
    strcat(cmd, "\r\n");

    write(socket_A, cmd, strlen(cmd));
    printf("cmd: %s", cmd);
    memset(answer, 0, 1024);
    code = readAnswer(socket_A,answer, 1024);
    printf("code:%d\n",code);
    if(code != OPEN_BINARY_ANS && code != OPEN_BINARY_ANS2){
        printf("Error: Wrong answer received, supposed to be login!\n");
        exit(1);
    }
    
    FILE *file = fopen(param.path + param.file,"w");
    if(file == NULL){
        printf("Error, Could not open the file\n");
        exit(1);
    }
    int n_bytes;
    char temp[1024];

    while(1){
        n_bytes = read(socket_B,temp,1024);
        fwrite(temp,1, n_bytes, file);
        if(n_bytes <= 0) break;
    }
    fclose(file);
    if(close(socket_B) != 0) {
        printf("Error: Could not close socket B");
        exit(1);
    }

    memset(answer, 0, 1024);

    code = readAnswer(socket_A,answer, 1024);

    if(code != TRANSFER_COMP_ANS){
        printf("Error: Wrong answer received, supposed to be transfer completed!\n");
        exit(1);
    }
    strcpy(cmd, "quit\r\n");
    write(socket_A,cmd,6);

    printf("cmd: %s", cmd);

    memset(answer, 0, 1024);
    code = readAnswer(socket_A,answer, 1024);
    printf("code:%d\n",code);
    if(code != QUIT_ANS){
        printf("Error: Wrong answer received, supposed to be transfer completed!\n");
        exit(1);
    }
    if (close(socket_A) != 0){
        printf("Error: Could not close socket A\n");
        exit(1);
    }

   
    return 0;
}