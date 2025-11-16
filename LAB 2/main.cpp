#include <iostream>
#include <string>
#include <vector>
#include <random>
using namespace std;

#include "FactoryMethod.h"
#include "AbstractFactory.h"
#include "Builder+Prototype.h"
#include "Singleton.h"

int main()
{
    srand(time(0));

    cout<<"Abstract Factory"<<endl;
    AbstractFactoryDemo();
    cout<<endl<<endl;
    cout<<"Factory Method"<<endl;
    FactoryMethodDemo();
    cout<<endl<<endl;
    cout<<"Builder + Prototype"<<endl;
    BuilderPrototypeDemo();
    cout<<endl<<endl;
    cout<<"Singleton"<<endl;
    SingletonDemo();

    return 0;
}