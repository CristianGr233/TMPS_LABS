#include <iostream>
#include <string>
#include <vector>

using namespace std;

#include "SRP.h"
#include "OCP.h"
#include "ISP.h"

int main(){

cout<<endl<<"SRP - BEFORE:"<<endl;
SRP_before();
cout<<endl<<"SRP - AFTER:"<<endl;
SRP_after();

cout<<endl<<"OCP - BEFORE:"<<endl;
OCP_before();
cout<<endl<<"OCP - AFTER:"<<endl;
OCP_after();

cout<<endl<<"ISP - BEFORE:"<<endl;
ISP_before();
cout<<endl<<"ISP - AFTER:"<<endl;
ISP_after();

    return 0;
}