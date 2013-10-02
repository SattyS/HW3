#include <iostream>
#include <string>
#include <list>
#define MAX_SIZE 5
using namespace std;


struct webPage {

    string webPageName;
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
void lRU(string text){
    int i=0;
    bool found = false;
    wP wPObj;
    if(wP_List.empty()){
        wPObj.webPageName = text;
        wP_List.push_front(wPObj);
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
                wP_List.erase(list_iter);
                wP_List.push_front(wPObj);
            }
        }else{
            if(wP_List.size() == MAX_SIZE){
                std::list<wP>::iterator list_iter_l = wP_List.begin();
                for(int k=0;k < wP_List.size() - 1;list_iter_l++,k++);
                wP_List.erase(list_iter_l);
            }
            wP wPObjNew;
            wPObjNew.webPageName = text;
            wP_List.push_front(wPObjNew);
        }       
    }
    
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

    lRU("www.google.com");
    lRU("www.yahoo.com");
    lRU("www.google.com");
    lRU("www.utd.com");
    lRU("www.utd.com");
    lRU("www.yahoo.com");
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
    printList();

    return 0;
}

