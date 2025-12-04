#include <iostream>
#include <vector>

using namespace std;

#include "Strategy.h"
#include "Observer.h"
#include "Mediator.h"


int main()
{
    cout<<"\n--------------Mediator Demo--------------:\n\n"<<endl;
    MediatorDemo();
    cout<<"\n\n--------------Observer Demo--------------:\n\n"<<endl;
    ObserverDemo();
    cout<<"\n\n--------------Strategy Demo--------------:\n\n"<<endl;
    StrategyDemo();


    return 0;
}