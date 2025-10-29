
#ifndef LAB_1_ISP_H
#define LAB_1_ISP_H


//Before

class IEmployee{
    virtual void codeBackend() = 0;
    virtual void codeFrontend() = 0;
    virtual void deployApp() = 0;
    virtual void developNetwork() = 0;
};

class Frontender:IEmployee
{
public:
    void codeBackend() override
    {
        cout<<"I don't know how to code backend"<<endl;
    }

    void codeFrontend() override
    {
        cout<<"Coding Frontend..."<<endl;
    }

    void deployApp() override
    {
        cout<<"I don't know how to deploy an app"<<endl;
    }

    void developNetwork() override
    {
        cout<<"I don't know how to develop a Network Infrastructure"<<endl;
    }

};

void ISP_before()
{
    Frontender frontender;
    frontender.codeBackend();
    frontender.codeFrontend();
    frontender.deployApp();
    frontender.developNetwork();
}

//After

class IBackend{
    virtual void codeBackend() = 0;
};


class IFrontend{
    virtual void codeFrontend() = 0;
};


class IDevOps{
    virtual void deployApp() = 0;
};


class INetwork{
    virtual void developNetwork() = 0;
};


class FullStackDev: IBackend, IFrontend
{
public:
    void codeBackend() override
    {
        cout<<"Coding Backend..."<<endl;
    }

    void codeFrontend() override
    {
        cout<<"Coding Frontend..."<<endl;
    }
};


class DevOpsEngineer : IDevOps{
public:
    void deployApp()
    {
        cout<<"Deploying App..."<<endl;
    }
};

class NetworkArchitect : INetwork{
public:
    void developNetwork() override
    {
        cout<<"Developing network..."<<endl;
    }
};


void ISP_after()
{
    FullStackDev fullStackDev;
    fullStackDev.codeFrontend();
    fullStackDev.codeBackend();

    DevOpsEngineer devOpsEngineer;
    devOpsEngineer.deployApp();

    NetworkArchitect networkArchitect;
    networkArchitect.developNetwork();

}

#endif //LAB_1_ISP_H
