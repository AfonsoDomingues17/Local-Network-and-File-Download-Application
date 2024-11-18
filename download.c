#include <stdio.h>
#include <sys/socket.h>
#include <netdb.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <unistd.h>

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
    char * file;
    // char file[PARAM_SIZE];
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

    param->file = strrchr(param->path, '/') + 1; //extract the file

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

int readAnswer(int sockfd, char *answer){
    char code[4] = {0};
    read(sockfd, answer, 1024 - 1);
    strncpy(code, answer, 3);
    int ret = atoi(code);
    
    printf(answer);
    //while (read(sockfd, asw, 1024 - 1) > 0) printf(asw);

    return ret;
}

int main(int argc, char **argv) {

    struct parameters param;

    if (argc != 2) printf("Usage: %s ftp://[<user>:<password>@]<host>/<url-path>\n", argv[0]);

    if(parseURL(argv[1],&param) != 0){
        printf("How to call: ftp://[<user>:<password>@]<host>/<url-path>\n");
        exit(1);
    }
    printf("V1:%s\n",param.user);
    printf("V1:%s\n",param.password);
    printf("V1:%s\n",param.file);
    printf("V1:%s\n",param.host);
    printf("V1:%s\n",param.ip);
    printf("V1:%s\n",param.path);

    char answer[1024] = {0}; // answer == &answer[0]
    int socket_A = openSocket(param.ip,FTP_PORT);
    int code = readAnswer(socket_A, answer);
    if(code != WELCOME_AN){
        printf("Error: Wrong answer received, supposed to be welcome!\n");
        exit(1);
    }
    
    char cmd[5 + PARAM_SIZE];
    strcpy(cmd, "user ");
    strcat(cmd, param.user);
    strcat(cmd, "\n");
   
    write(socket_A,cmd,strlen(cmd));
    memset(answer, 0, 1024);
    code = readAnswer(socket_A,answer);
    if(code != PASSWORD_ANS){
        printf("Error: Wrong answer received, supposed to request password!\n");
        exit(1);
    }
    
    strcpy(cmd, "pass ");
    strcat(cmd, param.password);
    strcat(cmd, "\n");

    write(socket_A, cmd, strlen(cmd));
    memset(answer, 0, 1024);
    code = readAnswer(socket_A,answer);
    if(code != LOGIN_ANS){
        printf("Error: Wrong answer received, supposed to be login!\n");
        exit(1);
    }

    write(socket_A,"pasv\n",5);
    memset(answer, 0, 1024);
    code = readAnswer(socket_A,answer);
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
    strcat(cmd, "\n");

    write(socket_A, cmd, strlen(cmd));
    memset(answer, 0, 1024);
    code = readAnswer(socket_A,answer);
    if(code != OPEN_BINARY_ANS){
        printf("Error: Wrong answer received, supposed to be login!\n");
        exit(1);
    }
    
    FILE *file = fopen(param.file,"w");
    if(file == NULL){
        printf("Error, Could not open the file\n");
        exit(1);
    }
    int n_bytes;
    char temp[1024];

    while(1){
        n_bytes = read(socket_B,temp,1024 - 1);
        fwrite(temp,1, n_bytes, file);
        if(n_bytes <= 0) break;
    }
    fclose(file);
    memset(answer, 0, 1024);
    code = readAnswer(socket_A,answer);
    if(code != TRANSFER_COMP_ANS){
        printf("Error: Wrong answer received, supposed to be transfer completed!\n");
        exit(1);
    }
    write(socket_A,"quit\n",5);
    memset(answer, 0, 1024);
    code = readAnswer(socket_A,answer);
    if(code != QUIT_ANS){
        printf("Error: Wrong answer received, supposed to be transfer completed!\n");
        exit(1);
    }
    if (close(socket_A) != 0){
        printf("Error: Could not close socket A\n");
        exit(1);
    }

    if(close(socket_B) != 0) {
        printf("Error: Could not close socket B");
        exit(1);
    }
    return 0;
}