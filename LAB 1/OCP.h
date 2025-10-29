
#ifndef LAB_1_OCP_H
#define LAB_1_OCP_H


//Before

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

void OCP_before(){

    Burger hamburger(HAMBURGER);
    Burger cheeseburger(CHEESEBURGER);
    Burger crispychickenburger(CRISPYCHICKENBURGER);

    vector<Burger> burgers = {hamburger, cheeseburger, crispychickenburger};

    for(auto burger : burgers)
    {
        cout<<endl;
        burger.makeBurger();
    }
}

//After

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


void OCP_after(){


    Hamburger hamburger;
    Cheeseburger cheeseburger;
    Crispychickenburger crispychickenburger;

    vector<IBurger*> burgers = {&hamburger, &cheeseburger, &crispychickenburger};

    for(auto burger : burgers)
    {
        cout<<endl;
        burger->makeBurger();
    }

}

#endif //LAB_1_OCP_H
