
#ifndef LAB_2_CRAFTABLEWEAPON_H
#define LAB_2_CRAFTABLEWEAPON_H

class CraftableWeapon{
public:
    virtual void show() = 0;
};


class AssaultRifle : public CraftableWeapon{
private:
    string receiver;
    string barrel;
    string handguard;
    string bipod;
    string stock;
    string grip;
    string magazine;
public:
    void show()
    {
        cout<<receiver<<endl;
        cout<<barrel<<endl;
        cout<<handguard<<endl;
        cout<<bipod<<endl;
        cout<<stock<<endl;
        cout<<grip<<endl;
        cout<<magazine<<endl;
    }
    friend class ARBuilder;
};


class Handgun : public CraftableWeapon{
private:
    string receiver;
    string barrel;
    string grip;
    string magazine;

public:
    void show()
    {
        cout<<receiver<<endl;
        cout<<barrel<<endl;
        cout<<grip<<endl;
        cout<<magazine<<endl;
    }
    friend class HandgunBuilder;
};






#endif //LAB_2_CRAFTABLEWEAPON_H
