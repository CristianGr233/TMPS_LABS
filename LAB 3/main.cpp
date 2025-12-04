#include <iostream>
#include <vector>

using namespace std;

#include "Adapter.h"
#include "Composite.h"
#include "Facade.h"

int main()
{
    cout<<"\n--------------Adapter Demo--------------:\n\n"<<endl;
    AdapterDemo();
    cout<<"\n\n--------------Composite Demo--------------:\n\n"<<endl;
    CompositeDemo();
    cout<<"\n\n--------------Facade Demo--------------:\n\n"<<endl;
    FacadeDemo();


    return 0;
}