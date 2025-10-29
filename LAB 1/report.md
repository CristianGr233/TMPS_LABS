# SOLID Principles

### Course: Software Design Techniques and Mechanisms

Author: Cristian Grubîi
----


## SRP
Single Responsibility Principles states that 
a class should have only one reason to change. 
In other words, each class should be responsible for
only a single "role" in the system.


### SRP - before

In the following example, an attempt is made to combine
multiple functionalities into the same User class.
The main issue is the fact that trying to integrate
a "repository" within this class, it does not really make sense,
as another object would have to be made, also containing 
an username and a password. 

```
class User{

private:
    string username;
    string password;
    //vector<User> users; <--- Not happening
public:
    User(string user, string pass)
    {
        username = user;
        password = pass;
    }

    bool validate(){
        return !username.empty() && password.length() >= 6;
    };

    void saveToDB(){
        cout<<"Saving user "<<username<<" to database.\n";
    }

    void display()
    {
        cout<<"Username: "<<username << "\n";
    }

};
```


### SRP - after


With some modifications, the User class has been split into
- UserEntity, which contains the fields;
- UserRepository, used to store users;
- UserService, used for logic (e.g. validating info);
- UserController, used for displaying data.

In this manner, each class is responsible for one thing only.

```
class UserService;
class UserRepository;
class UserController;

class UserEntity {
private:
    string username;
    string password;

public:

    UserEntity(string user, string pass)
    {
        username = user;
        password = pass;
    }

friend class UserService;
friend class UserRepository;
friend class UserController;
};

class UserRepository{
private:
    vector<UserEntity> users;
public:
    void saveToDB(UserEntity user)
    {
        cout << "Saving user '" << user.username << "' to database.\n";
        users.push_back(user);
    }
};


class UserService{
private:
    UserRepository repo;
public:
    UserService(UserRepository r)
    {
        repo = r;
    }

    bool validate(UserEntity user)
    {
        return !user.username.empty() && user.password.length() >= 6;
    }

    void registerUser(UserEntity user)
    {
        if (validate(user))
        {
            repo.saveToDB(user);
            cout << "User registration successful.\n";
        }
        else
            cout << "User validation failed.\n";

    }

};

class UserController{
public:
    void display(UserEntity user)
    {
        cout<<"Username: "<<user.username << "\n";
    }
};
```




## OCP

Open-Closed principle consists in the idea that classes should be open for
extension, but closed for modification (i.e. any new behaviour is added
without modifying existing code).


### OCP - before

At first glance, there seems to be nothing wrong with
this example. However, if there were to appear more burgers
on the menu, it would mean changing the enum, then the makeBurger() method.
In this way, the behaviour of the class is changed by
modifying existing code, thus breaking OCP.

```
enum BurgerType{
    HAMBURGER,
    CHEESEBURGER,
    CRISPYCHICKENBURGER
};


class Burger{

public:
    BurgerType Type;

    Burger() = default;

    Burger(BurgerType BT){
        Type = BT;
    };

    void makeBurger()
    {
        switch (Type)
        {
            case BurgerType::HAMBURGER:
                cout<<"Making Hamburger"<<endl;
                cout<<"Adding buns"<<endl;
                cout<<"Adding beef patty"<<endl;
                cout<<"Adding ketchup"<<endl;
                break;
            case BurgerType::CHEESEBURGER:
                cout<<"Making Cheeseburger"<<endl;
                cout<<"Adding buns"<<endl;
                cout<<"Adding beef patty"<<endl;
                cout<<"Adding cheese"<<endl;
                cout<<"Adding ketchup"<<endl;
                break;
            case BurgerType::CRISPYCHICKENBURGER:
                cout<<"Making CrispyChickenBurger"<<endl;
                cout<<"Adding buns"<<endl;
                cout<<"Adding crispy chicken patty"<<endl;
                cout<<"Adding lettuce"<<endl;
                cout<<"Adding tomato"<<endl;
                cout<<"Adding garlic mayo"<<endl;
                break;
        }
    }

};
```

### OCP - after


If instead Burger was an interface, with the method
makeBurger(), and each new type of burger were to inherit
from it, this would mean that no changes are done to existing code,
and new burgers would be added by extending the code instead.

```
class IBurger{
public:
    virtual void makeBurger() = 0;
};

class Hamburger: public IBurger
{
public:
    void makeBurger() override
    {
        cout<<"Making Hamburger"<<endl;
        cout<<"Adding buns"<<endl;
        cout<<"Adding beef patty"<<endl;
        cout<<"Adding ketchup"<<endl;
    }
};

class Cheeseburger: public IBurger
{
public:
    void makeBurger() override
    {
        cout<<"Making Cheeseburger"<<endl;
        cout<<"Adding buns"<<endl;
        cout<<"Adding beef patty"<<endl;
        cout<<"Adding cheese"<<endl;
        cout<<"Adding ketchup"<<endl;
    }

};


class Crispychickenburger : public IBurger
{
public:
    void makeBurger() override
    {
        cout<<"Making CrispyChickenBurger"<<endl;
        cout<<"Adding buns"<<endl;
        cout<<"Adding crispy chicken patty"<<endl;
        cout<<"Adding lettuce"<<endl;
        cout<<"Adding tomato"<<endl;
        cout<<"Adding garlic mayo"<<endl;
    }
};
```


## ISP


Interface Segregation Principle states that clients should
not be forced to depend on methods they do not use. This implies that
"large" interfaces are split into smaller, more specific interfaces.


### ISP - before

Having a general interface, with the methods of all
the tasks an employee may do, it will be seen that 
concrete implementations of said interface will lead to
classes which are forced to implement roles that are of no
interest to them (i.e. that they lack the competence of
doing).

```
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
```




### ISP - after

As such, the interface can be split into multiple interfaces,
each of them having defined a single task. If a role has
multiple responsibilities, then it may inherit from multiple interfaces.


```
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
```