#include <iostream>
#include <string>
#include <list>
#include <stdlib.h>
#include <fstream>
#include <functional>
#include <stdio.h>
#include <netinet/in.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#define MAX_SIZE 2



#define A 54059 /* a prime */
#define B 76963 /* another prime */
#define C 86969 /* yet another prime */

using namespace std;


int pClientCount =0;
//string fileNames[10] = {"One","Two","Three","Four","Five","Six","Seven","Eight","Nine","Ten"};

struct webPage {

    string webPageName;
    string hash;
    string webPageType;
    string last_modified;

};

typedef struct webPage wP;
typedef struct sockaddr SA; 

std::list<wP> wP_List;


void cacheClear(string hash){
    //std::string f = fileNames[ind]; 
    remove(hash.c_str());
    cout << "File Deleted " << hash << endl; 
}

string checkHead(string hash, string fileName){
    const int MAX_BUFFER = 2048;
    bool found = false;
    std:string lMD;
    string cmd="HEAD HTTP/1.0 "; //http://www.ece.tamu.edu/~reddy/ee602_08.html";
    cmd.append(fileName);
    char buffer[MAX_BUFFER];
    hash.append("_1");
    FILE *stream = popen(cmd.c_str(), "r");
    FILE *ostream = fopen(hash.c_str(),"ab+");
    if (stream){
       while (!feof(stream))
       {
            if (fgets(buffer, MAX_BUFFER, stream) != NULL)
            {
               fprintf(ostream, buffer);
            }
       }
       rewind(ostream);
       char *line;
       size_t len = 0;
       int read;
       while(!feof(ostream)){
            if((read = getline(&line, &len, ostream)) != -1){
                string tempLine = line;
                std::size_t pos = tempLine.find("Last-Modified:");
                if(pos != std::string::npos){
                    lMD = tempLine.substr(14);
                    found = true;
                    break;
                }
            }
       }
    }
    if(found){
        cout << "LMD " << lMD << endl;
    }else{
        lMD = "";
    }
    pclose(stream);
    cacheClear(hash);
    fclose(ostream);
    return lMD;
}

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

void printList(){

    for(std::list<wP>::iterator list_iter = wP_List.begin(); 
    list_iter != wP_List.end(); list_iter++)
    {
        wP wPTemp = *list_iter;
        std::cout<<wPTemp.webPageName<<endl;
        //std::cout<<wPTemp.webPageType<<endl;
    }
}
int fileWrite(string hash,string fileName){    
    /*
        ofstream f;
        f.open(fileName.c_str());
        f << system("GET HTTP/1.0 http://www.ece.tamu.edu/~reddy/ee602_08.html");
        f.close();
        return 1;
    */
    const int MAX_BUFFER = 2048;
    string cmd="GET HTTP/1.0 "; //http://www.ece.tamu.edu/~reddy/ee602_08.html";
    cmd.append(fileName);
    char buffer[MAX_BUFFER];
    FILE *stream = popen(cmd.c_str(), "r");
    FILE *ostream = fopen(hash.c_str(),"w");
    if (stream){
       while (!feof(stream))
       {
            if (fgets(buffer, MAX_BUFFER, stream) != NULL)
            {
               fprintf(ostream, buffer);
            }
       }
       fprintf(ostream,"\n END");
       pclose(stream);
       fclose(ostream);
    }
}
int lRU(string text){
    int i=0;
    int ret = 0;
    bool found = false;
    wP wPObj;
    string hash;
    hash = hash_str(text.c_str());
    if(wP_List.empty()){
        wPObj.webPageName = text;
        wPObj.hash = hash;
        fileWrite(hash,text);
        cout << "File got using GET from server -- Send file to client" << endl;
        wPObj.last_modified = checkHead(hash,text);
        wP_List.push_front(wPObj);
        ret  = 1;
    }else{
        std::list<wP>::iterator list_iter = wP_List.begin();
        for(;list_iter != wP_List.end(); list_iter++)
        {
            wPObj = *list_iter;
            if (wPObj.webPageName.compare(text) == 0){
                found = true;
                break;
            }
            i++;
        }
        if(found){
            cout << "File already in cache -- Send file to client" << endl;
            if(wPObj.last_modified.compare("") == 0){
                fileWrite(hash,text);
                cout << "Last modified date can't be determined from HEAD" << endl;
                cout << "File got using GET from server -- Send file to client" << endl;
            }else{
                std::string lmLocal = checkHead(hash,text);
                if(wPObj.last_modified.compare(lmLocal) != 0){
                    fileWrite(hash,text);
                    cout << "Last modified date has changed" << endl;
                    cout << "File got using GET from server -- Send file to client" << endl;
                }else{
                    cout << "Last modified date has not changed - Send old file to client" << endl;
                }
            }
            if(i != 0){
                // Check Timestamp and update (HEAD)
                wP_List.erase(list_iter);
                wP_List.push_front(wPObj);
                ret = 0;
            }
        }else{
            // GET
            if(wP_List.size() == MAX_SIZE){
                std::list<wP>::iterator list_iter_l = wP_List.begin();
                for(int k=0;k < wP_List.size() - 1;list_iter_l++,k++);
                wPObj = *list_iter_l;
                cacheClear(wPObj.hash);
                wP_List.erase(list_iter_l);
            }
            wP wPObjNew;
            wPObjNew.webPageName = text;
            wPObjNew.hash = hash;
            wPObjNew.last_modified = checkHead(hash,text);
            wP_List.push_front(wPObjNew);
            fileWrite(hash,text);
            cout << "File got using GET from server -- Send file to client" << endl;
            ret = 1;
        }       
    }
    return ret;
}

void sendFile(string file, int connfd){

    FILE *ostream = fopen(file.c_str(),"r");
    char buf[512];
    char ack[2];
    int n;

    while(!feof(ostream)){
        fgets(buf,512,ostream);
        write(connfd,(void *) &buf,512);
        bzero(buf,512);
    }
    cout << "File Sent to Client" << endl;
    fclose(ostream);
}

int main(int argc, char const *argv[])
{
    
    /*
    int i;
    int count = 0, ret;
    string links;

    cin >> i;
    for(int k = 0;k<i; k++){
        cout << "Enter links :";
        cin >>  links;
        lRU(links);
    }
    */
    int serverSockFd, connfd, len;
    //int ret;
    int clientStatus = 0;
    struct sockaddr_in servAddr,cli[10];
    
    fd_set master;
    fd_set read_fds;
    int fdmax;
    int temp;
    int i=0;
    int s=0;
    char tempc[20];
    int readbytes;
    char buf[500];


    serverSockFd = socket(AF_INET,SOCK_STREAM,0);
    if(serverSockFd == -1)
    {
        printf("Server Socket creation failed . \n");
        //ret = -1;
    }
    else{
        printf("Server Socket successfully created . \n");
    }
    

    bzero(&servAddr,sizeof(servAddr));
    bzero(buf,500);

    servAddr.sin_family = AF_INET;
    servAddr.sin_addr.s_addr = htonl(INADDR_ANY);
    servAddr.sin_port = htons(atoi(argv[1]));

    if((bind(serverSockFd, (SA*)&servAddr, sizeof(servAddr)))!=0)
    {
        printf("Server Socket bind failed . \n");
        exit(1);
        //ret = -1;
    }
    else
    {
        printf("Server Socket successfully bound . \n");
    }
    
    if((listen(serverSockFd, 10))!=0)
    {
        printf("Server Listen failed...\n");
        exit(1);
        //ret = -1;
    }
    else
    {
        printf("Server listening . \n");
    }

    FD_SET(serverSockFd, &master);
    fdmax = serverSockFd;

    for(;;){
        read_fds = master;
        if(select(fdmax+1, &read_fds, NULL, NULL, NULL) == -1){
            perror("select");
            exit(4);
        }

        for(i=0 ; i<=fdmax ; i++){
            if(FD_ISSET(i, &read_fds)){
                if(i == serverSockFd){
                    //new Connections
                    cout << "New Connection" << endl;
                    len = sizeof(cli[pClientCount]);
                    connfd = accept(serverSockFd,(SA *)&cli[pClientCount],(socklen_t * __restrict__)&len);
                    if(connfd < 0)
                    {
                        printf("Server acccept failed . \n");
                        //ret = -1;
                    }
                    else
                    {
                        cout << "Server accepted client" << endl;
                        FD_SET(connfd, &master);
                        if(connfd > fdmax){
                            fdmax = connfd;
                        }
                        if ((readbytes = read(connfd,buf,sizeof buf)) > 0) {
                            
                            cout << "Inside read " << readbytes << endl;
                            //buf[readbytes] = '\0';
                            cout << buf << endl;
                            //string tempBuf = buf;
                            //string webPage = tempBuf.substr(11);
                            //cout << webPage << endl;
                            lRU(buf);
                            string fileHash = hash_str(buf);
                            sendFile(fileHash,connfd);
                            pClientCount++;
                            
                        }
                          
                    }
                }/*else{
                    // handle data from a client
                    if ((readbytes = read(i,(SM *) &clientMessage,sizeof(clientMessage))) <= 0) {
                        // got error or connection closed by client
                        if (readbytes == 0) {
                            // connection closed
                            printf("selectserver: socket %d hung up\n", i);
                        } else {
                            perror("recv");
                        }
                        clientMessage = constructServerPacket(i,clientMessage,6);
                        clearClient(i);
                        close(i);
                        FD_CLR(i, &master); // remove from master set
                        broadcast(fdmax,master,serverSockFd,i,clientMessage);
                    }
                    else{
                        //send data to clients
                        clientAttribute[1] = clientMessage.attribute[0];
                        clientHeader.vrsn = 3;
                        clientHeader.type = '3';
                        clientHeader.length = 2;
                        clientAttribute[0].type = 2;
                        clientAttribute[1].type = 4;

                        for(s=0; s<pclientCount; s++){
                            
                            if(clients[s].active == 1){

                                if(clients[s].fd == i){

                                    strcpy(tempc, clients[s].username);
                                    strcpy(clientAttribute[0].payload, tempc);
                                    clientAttribute[0].length = strlen(clientAttribute[0].payload);
                                    clientAttribute[1].length = strlen(clientAttribute[1].payload);
                                    clientMessage.attribute[0] = clientAttribute[0];
                                    clientMessage.attribute[1] = clientAttribute[1];
                                    clientMessage.header = clientHeader;
                                }   
                            }
                            
                        } 
                        broadcast(fdmax,master,serverSockFd,i,clientMessage);   
                    }
                }*/
        }
    }
}
    close(serverSockFd);
    return 0;
}

