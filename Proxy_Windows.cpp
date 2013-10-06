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
#include <ctime>
#include <sstream>
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
    string expireTime;

};

typedef struct webPage wP;
typedef struct sockaddr SA; 

std::list<wP> wP_List;
string dateformat(string a){
	a = a.substr(5,a.length());
	a = a.substr(0,a.length() - 4);
	string day = a.substr(0,2);
	string month = a.substr(3,3);
	string year = a.substr(7,4);
	int im;
	string m[] = {"Jan", "Feb", "Mar", "Apr", "May", "Jun", "Jul", "Aug", "Sep", "Oct", "Nov", "Dec"};
	for(int i=0; i <12; i++){
		if(month.compare(m[i]) == 0){
			im = i + 1;
			break;
		}
	}
	month = inttostring(im);
	a.replace(3,3,month);
	return a;
}
string inttostring (int a){
	std::stringstream ss;
	ss << a;
	std::string s(ss.str());
	return s;
}
const std::string currentDateTime() {
    time_t     now = time(0);
    struct tm  tstruct;
    char       buf[80];
    tstruct = *localtime(&now);
    strftime(buf, sizeof(buf), "%d %m %Y %X", &tstruct);
    string sBuf = buf;
	string hour = sBuf.substr(11,2);
	int h = atoi(hour.c_str());
	h = (h + 5) % 24;
	hour = inttostring(h);
	sBuf.replace(11,2,hour);
    return sBuf;
}
int isBig(string a, string b){
	
	int fday = atoi(a.substr(0,2).c_str());
	int fmonth = atoi(a.substr(3,2).c_str());
	int fyear = atoi(a.substr(6,4).c_str());
	string ftime = a.substr(11,a.length() - 1);
	
	int sday = atoi(b.substr(0,2).c_str());
	int smonth = atoi(b.substr(3,2).c_str());
	int syear = atoi(b.substr(6,4).c_str());
	string stime = b.substr(11,a.length() - 1);
	
	string dum = "";
	ftime.replace(2,1,dum);
	ftime.replace(4,1,dum);
	stime.replace(2,1,dum);
	stime.replace(4,1,dum);
	
	int fint = atoi(ftime.c_str());
	int sint = atoi(stime.c_str());
	
	cout << ftime << endl;
	cout << stime << endl;
	
	if(fyear == syear){
		if(fmonth == smonth){
			if(fday == sday){
				if(fint == sint){
					return 0;
				}else if(fint < sint){
					return -1;
				}else{
					return 1;
				}
			}else if(fday < sday){
				return -1;
			}else{
				return 1;
			}
		}else if(fmonth < smonth){
			return -1;
		}else{
			return 1;
		}		
	}else if(fyear < syear){
		return 1;
	}else{
		return -1;
	}
}

void cacheClear(string hash, string wp){
    //std::string f = fileNames[ind]; 
    remove(hash.c_str());
    cout << "File Deleted " << hash << " " << wp << endl; 
}

string getAddress(string clientMessage){
	std::size_t httpPos = clientMessage.find("http://");
    string fullHostAddr = clientMessage.substr(httpPos, clientMessage.length());
    cout << "FULL Host " << fullHostAddr << endl;
    return fullHostAddr;
}
string getIP(string clientMessage){
	std::size_t httpPos = clientMessage.find("http://");
    string fullHostAddr = clientMessage.substr(httpPos + 7, clientMessage.length());
    cout << "FULL Host " << fullHostAddr << endl;
    std::size_t slash = fullHostAddr.find("/");
    string ip = fullHostAddr.substr(0,slash);
    cout << ip << endl;
    return ip;
}
string getMessage(string clientMessage){
	std::size_t http = clientMessage.find("HTTP/1.0");
    string getMessage = clientMessage.substr(0,http + 8);
    cout << "GET message " << getMessage << endl;
	return getMessage;
}
string* getLastModifiedAndExpire(string filename){
	
	string *ret = new string[2];
	string message = getMessage(filename);
	string woGet = message.substr(3);
	char buffer[200];
	strcpy(buffer,"HEAD");
	strcat(buffer,woGet.c_str());
	strcat(buffer,"\n\n");
	
	struct sockaddr_in servaddr;
    int sockfd;
    /* Create socket at the client side */
    sockfd = socket(AF_INET,SOCK_STREAM,0);

    if(sockfd==-1)
    {
        printf("WServer Socket creation failed...\n");
        exit(0);
    }

    else
    {
        printf("WServer Socket successfully created..\n");
    }
    
    bzero(&servaddr,sizeof(servaddr));
    servaddr.sin_family=AF_INET;
    //cout << "IP " << ip << endl;
    servaddr.sin_addr.s_addr=inet_addr(getIP(filename).c_str());
    servaddr.sin_port=htons(80);

    /* Connect to the server by calling the function connect */
    if(connect(sockfd,(SA *)&servaddr,sizeof(servaddr))!=0)
    {
        printf("WServer Connection with the server failed...\n");
        exit(1);
    }
    else
    {
        printf("Connected to the Wserver..\n");
        const int MAX_BUFFER_RECV = 10000;
        //const int MAX_BUFFER_SEND = 10000;
        char Rbuf[MAX_BUFFER_RECV];
        //char Sbuf[MAX_BUFFER_SEND];
        //strcpy(Sbuf,getMessage(fileName).c_str());
        //strcat(Sbuf,"\n\n");
        //strcat(Sbuf, fileName.c_str());
        //strcpy(buf, "Host: www.qq.com");
        //strcpy(buf, "\n\n");
        //sleep(2);
        cout << buffer << endl;
        send(sockfd,buffer,sizeof buffer,0);
        bzero(Rbuf,10000);
        //cout << "Server Alive" << endl;
        recv(sockfd,Rbuf,sizeof Rbuf,0);
        close(sockfd);
        cout << "Buff Length " << strlen(Rbuf) << endl;
        cout << Rbuf << endl;
        string serverMessage = Rbuf;
        char lmDate[50];
        char exDate[50];
        int k=0;
	cout << endl << "Server Message " << endl << serverMessage;
        std::size_t expirePos = serverMessage.find("Expires:");
        std::size_t lmPos = serverMessage.find("Last-Modified:");
        for (unsigned i=lmPos + 15; (i<serverMessage.length()) && (serverMessage.at(i) != '\n'); ++i)
 		{
    		lmDate[k] = serverMessage.at(i);
		//cout << lmDate[k] << endl;
    		k++;
  		}
		lmDate[k] = '\0';
  		k = 0;
  		for (unsigned i=expirePos + 9; (i<serverMessage.length()) && (serverMessage.at(i) != '\n'); ++i)
  		{
    		exDate[k] = serverMessage.at(i);
		//cout << exDate[k] << endl;
    		k++;
  		}
		exDate[k] = '\0';
		//cout << lmDate << endl;
		//cout << exDate << endl;
  		ret[0] = lmDate;
  		ret[1] = exDate;
  		//cout << ret[0] << ret[1] << endl;
		cout << ret[0] << endl;
		cout << ret[1] << endl;
  		return ret;
        //string eTime = serverMessage.substr(expirePos,)
    }

}
/*
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
                std::size_t vPos = tempLine.find("200 OK");
                std::size_t pos = tempLine.find("Last-Modified:");
                if(vPos != std::string::npos){
                    lMD = "OK";
                    found = true;
                }
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
    cacheClear(hash, "Temp");
    fclose(ostream); 
    return lMD;
}
*/
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
    struct sockaddr_in servaddr;
    int sockfd;
    /* Create socket at the client side */
    sockfd = socket(AF_INET,SOCK_STREAM,0);

    if(sockfd==-1)
    {
        printf("WServer Socket creation failed...\n");
        exit(0);
    }

    else
    {
        printf("WServer Socket successfully created..\n");
    }
    
    bzero(&servaddr,sizeof(servaddr));
    servaddr.sin_family=AF_INET;
    //cout << "IP " << ip << endl;
    servaddr.sin_addr.s_addr=inet_addr(getIP(fileName).c_str());
    servaddr.sin_port=htons(80);

    /* Connect to the server by calling the function connect */
    if(connect(sockfd,(SA *)&servaddr,sizeof(servaddr))!=0)
    {
        printf("WServer Connection with the server failed...\n");
        exit(1);
    }
    else
    {
        printf("Connected to the Wserver..\n");
        const int MAX_BUFFER_RECV = 100000;
        const int MAX_BUFFER_SEND = 100000;
        char Rbuf[MAX_BUFFER_RECV];
        char Sbuf[MAX_BUFFER_SEND];
        strcpy(Sbuf,getMessage(fileName).c_str());
        strcat(Sbuf,"\n\n");
        //strcat(Sbuf, fileName.c_str());
        //strcpy(buf, "Host: www.qq.com");
        //strcpy(buf, "\n\n");
        //sleep(2);
        cout << Sbuf << endl;
        send(sockfd,Sbuf,sizeof Sbuf,0);
        bzero(Rbuf,100000);
        //cout << "Server Alive" << endl;
        recv(sockfd,Rbuf,sizeof Rbuf,0);
        cout << "Buff Length " << strlen(Rbuf) << endl;
        cout << Rbuf << endl;

        FILE *ostream = fopen(hash.c_str(),"w");
        fputs(Rbuf, ostream);
        fputs("\n END", ostream);
        fclose(ostream);
    }
    close(sockfd);
}
int lRU(string text, int connfd){
    int i=0;
    int ret = 0;
    bool found = false;
    wP wPObj;
    string hash;
    hash = hash_str(getAddress(text).c_str());    
    if(wP_List.empty()){
        string *retS = getLastModifiedAndExpire(text);
        wPObj.last_modified = retS[0];
        wPObj.expireTime = retS[1];
        wPObj.webPageName = text;
        wPObj.hash = hash;
        fileWrite(hash,text);
        cout << "File got using GET from server -- Send file to client" << endl;
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
            string expTime = wPObj.expireTime;
            string curTime = currentDateTime();
            cout << "Current Time" << curTime;
            expTime = dateFormat(expTime);
            cout << "Expire time" << expTime;
            int result;
            result = isBig(curTime,expTime);
            if(result == -1){
            	cout << "Expire Time > Current Time - Send old file to client" << endl;
            }else{
            	cout << "Current Time > Expire Time" << endl;
            	string *retS = getLastModifiedAndExpire(text);
            	wPObj.expireTime = retS[1];
            	string lmDate = retS[0];
            	int result = isBig(dateFormat(lmDate), dateFormat(wPObj.last_modified));
            	if(result == 0){
            		cout << "Last modified date is unchanged - Send old file to client" << endl;
            	}else{
            		cout << "Last modified date has changed" << endl;
                    cout << "File got using GET from server -- Send file to client" << endl;
            		wPObj.last_modified = retS[0];
            		fileWrite(hash,text);
            	}
            }
            if(i != 0){
                wP_List.erase(list_iter);
                wP_List.push_front(wPObj);
                ret = 0;
            }
        }else{
            string *retS = getLastModifiedAndExpire(text);
            if(wP_List.size() == MAX_SIZE){
                std::list<wP>::iterator list_iter_l = wP_List.begin();
                for(int k=0;k < wP_List.size() - 1;list_iter_l++,k++);
                wPObj = *list_iter_l;
                cacheClear(wPObj.hash,wPObj.webPageName);
                wP_List.erase(list_iter_l);
            }
            wP wPObjNew;
            wPObjNew.webPageName = text;
            wPObjNew.hash = hash;
            wPObjNew.last_modified = retS[0];
            wPObjNew.expire_Time = retS[1];
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
        //sleep(1);
        write(connfd,(void *) &buf,512);
        bzero(buf,512);
    }
    cout << "File Sent to Client" << endl;
    fclose(ostream);
}

bool checkPacket(string packet){
    bool ok = false;
    std::size_t gpos = packet.find("GET ");
    std::size_t hpos = packet.find("HTTP/1.0 ");
    if((gpos != std::string::npos) && (hpos != std::string::npos)){
        ok = true;
    }
    return ok;
}
int main(int argc, char const *argv[])
{
    
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
                            if(!checkPacket(buf)){
                                char pBuf [] = "Invalid request";
                                send(connfd,pBuf,sizeof pBuf,0);
                            }else{
                                int lRURet;
                                string sBuf = buf;
                                //string webPageFull = sBuf.substr(15);
                                //cout << webPageFull << endl;
                                lRURet = lRU(sBuf,connfd);
                                if(lRURet == 2){
                                    char pBuf [] = "404 Page Not Found";
                                    write(connfd,(void *) &pBuf,512);
                                }else{
                                    string fileHash = hash_str(buf);
                                    sendFile(fileHash,connfd);
                                }
                                pClientCount++;
                            }
                            //string tempBuf = buf;
                            //string webPage = tempBuf.substr(11);
                            //cout << webPage << endl;
                            
                            
                        }
                          
                    }
                }
        }
    }
}
    close(serverSockFd);
    return 0;
}

