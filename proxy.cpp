#include <iostream>
#include <string>
#include <list>
#include <stdlib.h>
#include <fstream>
#define MAX_SIZE 5
using namespace std;

string fileNames[10] = {"One","Two","Three","Four","Five","Six","Seven","Eight","Nine","Ten"};

struct webPage {

    string webPageName;
    int index;
    string webPageType;

};

typedef struct webPage wP;

std::list<wP> wP_List;

void printList(){

    for(std::list<wP>::iterator list_iter = wP_List.begin(); 
    list_iter != wP_List.end(); list_iter++)
    {
        wP wPTemp = *list_iter;
        std::cout<<wPTemp.webPageName<<endl;
        //std::cout<<wPTemp.webPageType<<endl;
    }
}
void cacheClear(int ind){
    std::string f = fileNames[ind]; 
    remove(f.c_str());
    cout << "File Deleted " << fileNames[ind] << endl; 
}
int fileWrite(int k,string fileName){    
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
    FILE *ostream = fopen(fileNames[k].c_str(),"w");
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
int lRU(string text, int ind){
    int i=0;
    int ret = 0;
    bool found = false;
    wP wPObj;
    if(wP_List.empty()){
        wPObj.webPageName = text;
        wPObj.index = ind;
        wP_List.push_front(wPObj);
        fileWrite(ind,text);
        cout << "File got using GET from server -- Send file to client" << endl;
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
            if(i != 0){
                // Check Timestamp and update (HEAD)
                cout << "File already in cache -- Send file to client" << endl;
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
                cacheClear(wPObj.index);
                wP_List.erase(list_iter_l);
            }
            wP wPObjNew;
            wPObjNew.webPageName = text;
            wPObjNew.index = ind;
            wP_List.push_front(wPObjNew);
            fileWrite(ind,text);
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
        ret = lRU(links,count);
        if(ret != 0){
            cout << "Webpage html saved as " << fileNames[count] << endl;
            count ++;
        }
    }
    //fileWrite("One.html");

    return 0;
}

