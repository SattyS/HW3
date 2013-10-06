#include <iostream>
#include <string>
#include <stdlib.h>
#include <ctime>
#include <sstream>
using namespace std;
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
int main(){
	string a = "Mon, 06 Oct 2013 17:37:49 GMT";
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
	string cdate = currentDateTime();
	cout << a << endl;
    cout << cdate << endl;
    cout << isBig(a,cdate) << endl;
	return 0;
}
