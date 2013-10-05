#include <stdio.h>
#include <netinet/in.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#include <iostream>
#include <string>
#include <list>
#include <stdlib.h>
#include <fstream>
#include <functional>

#define A 54059 /* a prime */
#define B 76963 /* another prime */
#define C 86969 /* yet another prime */

using namespace std;

typedef struct sockaddr SA;

string hash_str(const char* s)
{
   char buffer[50];
   unsigned h = 31 /* also prime */;
   while (*s) {
     h = (h * A) ^ (s[0] * B);
     s++;
   }
   sprintf (buffer, "%u", h);
   std::string a  = buffer;
   cout << a << endl;
   return a; // or return h % C;
}

int main(int argc, char const *argv[])
{
    
    fd_set master;
    fd_set read_fds;
    int ret;
    FD_ZERO(&read_fds);
    FD_ZERO(&master);
    char buf[100000];
    int nread;
    struct sockaddr_in servaddr;
    int sockfd;
    /* Create socket at the client side */
    sockfd = socket(AF_INET,SOCK_STREAM,0);

    if(sockfd==-1)
    {
        printf("socket creation failed...\n");
        exit(0);
    }

    else
    {
        printf("Socket successfully created..\n");
    }
    
    bzero(&servaddr,sizeof(servaddr));
    servaddr.sin_family=AF_INET;
    servaddr.sin_addr.s_addr=inet_addr(argv[1]);
    servaddr.sin_port=htons(atoi(argv[2]));

    /* Connect to the server by calling the function connect */
    if(connect(sockfd,(SA *)&servaddr,sizeof(servaddr))!=0)
    {
        printf("Connection with the server failed...\n");
        exit(1);
    }
    else
    {
        printf("Connected to the server..\n");

        /* Send JOIN message to the server */ 
        //sendJoin(sockfd, arg);
        /* Add client socket and standard input to the set */
        //FD_SET(sockfd, &master);
        //strcpy(buf,argv[5]);
        //strcat(buf,argv[4]);
        //strcat(buf,argv[5]);
        //cout << buf << endl;
        //string fileName;
        //fileName = hash_str(buf);
        strcpy(buf,"GET /html/default.asp HTTP/1.0\n\n");
        //strcpy(buf, "\n");
        //strcpy(buf, "Host: www.qq.com");
        //strcpy(buf, "\n\n");
        //sleep(2);
        cout << buf << endl;
        send(sockfd,buf,sizeof buf,0);
        bzero(buf,100000);
        cout << "Server Alive" << endl;
        nread = recv(sockfd,buf,sizeof buf,0);
        cout << buf << endl;
        cout << strlen(buf) << endl;
        //FILE *ostream = fopen(fileName.c_str(),"w");
        int i =0;
        int fputsVal;
        int p = 0;

        /*

        while((nread = read(sockfd,buf,512)) > 0){
            cout << buf << endl;
            cout << strlen(buf) << endl;
            //404
            if((strlen(buf) == 18) && (strcmp(buf,"404 Page Not Found") == 0)){
                cout << "404 Page Not Found" << endl;
                p = 1;
                break;
            }
            if((strlen(buf) == 0) || ((strlen(buf) == 4) && (strcmp(buf," END") == 0))){
                break;
            }
            fputsVal = fputs(buf,ostream);
            
            bzero(buf,sizeof buf);
            //write(sockfd,(void *) &ack,2);
            i++;
        }
        if(p == 1){
            remove(fileName.c_str());
        }else{
            cout << "File written" << endl;
        }
        fclose(ostream);
        //FD_SET(STDIN_FILENO, &master);
    */
    }
    close(sockfd);
}
