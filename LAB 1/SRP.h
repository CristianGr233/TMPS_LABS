
#ifndef LAB_1_SRP_H
#define LAB_1_SRP_H


//Before

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


void SRP_before()
{
    User user("admin", "secretpass");

    if (user.validate())
    {
        user.saveToDB();
        user.display();
    }
    else
        cout << "Invalid user data.\n";
}


//After


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

void SRP_after()
{
    UserEntity user("admin2", "secretpass2");

    UserRepository userRepository;

    UserService userService(userRepository);
    userService.registerUser(user);

    UserController userController;
    userController.display(user);
}

#endif //LAB_1_SRP_H
