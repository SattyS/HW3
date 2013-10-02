#include <iostream>
#include <string>
#include <list>
#include <stdlib.h>
#include <fstream>
#include <functional>
#define MAX_SIZE 2


#define A 54059 /* a prime */
#define B 76963 /* another prime */
#define C 86969 /* yet another prime */

using namespace std;



//string fileNames[10] = {"One","Two","Three","Four","Five","Six","Seven","Eight","Nine","Ten"};

struct webPage {

    string webPageName;
    string hash;
    string webPageType;
    string last_modified;

};

typedef struct webPage wP;

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

int main(int argc, char const *argv[])
{
    /* code */
    /*
    if(argc != 2){
        return 1;
    }
    std::string webPageName(argv[1]);
    cout<<webPageName<<endl;
    */
    /*
    lRU("www.google.com");
    lRU("www.yahoo.com");
    lRU("www.google.com");
    lRU("www.utd.com");
    lRU("www.utd.com");
    lRU("www.yahoo.com");
    */
    /*
    wP wPObj[10];
    wPObj[0].webPageName = "www.google.com";
    wPObj[0].webPageType = "text/html";
    wPObj[1].webPageName = "www.google1.com";
    wPObj[1].webPageType = "text1/html";
    wPObj[2].webPageName = "www.google2.com";
    wPObj[2].webPageType = "text2/html";

    wP_List.push_front(wPObj[0]);
    wP_List.push_front(wPObj[1]);
    wP_List.push_front(wPObj[2]);
    */
    //printList();    

    //system("GET HTTP/1.0 http://www.ece.tamu.edu/~reddy/ee602_08.html");
    int i;
    int count = 0, ret;
    string links;

    cin >> i;
    for(int k = 0;k<i; k++){
        cout << "Enter links :";
        cin >>  links;
        lRU(links);
    }


    //checkHead("2723130594","www.w3.org");

    return 0;
}

