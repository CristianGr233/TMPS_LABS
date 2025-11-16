
#ifndef LAB_2_SINGLETON_H
#define LAB_2_SINGLETON_H


class InventorySingleton {
private:
    string primaryWeapon;
    string secondaryWeapon;
    string grenade;
    InventorySingleton()
    {
        primaryWeapon = "None";
        secondaryWeapon = "None";
        grenade = "None";
    }
    static InventorySingleton *singleton;

public:

    InventorySingleton(InventorySingleton &clone) = delete;

    static InventorySingleton *getInstance(){
        if (singleton == nullptr) {
            singleton = new InventorySingleton();
        }
        return singleton;
    }

    void setLoadout(string primary, string secondary, string g)
    {
        primaryWeapon = primary;
        secondaryWeapon = secondary;
        grenade = g;
    }

    void setPrimaryWeapon(string weapon)
    {
        primaryWeapon = weapon;
    }

    void setSecondaryWeapon(string weapon)
    {
        secondaryWeapon = weapon;
    }

    void setGrenade(string grnd)
    {
        grenade = grnd;
    }

    void printInventory()
    {
        cout<<"Primary Weapon: "<<primaryWeapon<<endl;
        cout<<"Secondary Weapon: "<<secondaryWeapon<<endl;
        cout<<"Grenades: "<<grenade<<endl;

    }

};

InventorySingleton* InventorySingleton::singleton = nullptr;

void func(InventorySingleton b)
{
    b.printInventory();
}

void SingletonDemo()
{

    InventorySingleton *inventory = InventorySingleton::getInstance();



    inventory->setLoadout("M4A1", "M9", "Frag Grenade");
    InventorySingleton& inventory2 = *InventorySingleton::getInstance();
    inventory2.setPrimaryWeapon("SIG Sauer MCX");
    inventory->printInventory();

    //InventorySingleton inventory3 = *inventory; //<-- Not allowed
    //InventorySingleton inventory4 = *InventorySingleton::getInstance(); //<-- Not allowed
    //func(*inventory); //<-- Not allowed
}

#endif //LAB_2_SINGLETON_H
