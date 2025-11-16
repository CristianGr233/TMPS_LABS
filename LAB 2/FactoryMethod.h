
#ifndef LAB_2_FACTORYMETHOD_H
#define LAB_2_FACTORYMETHOD_H


class Weapon{
public:
    virtual void equipWeapon() = 0;
};

class Shotgun : public Weapon{
private:
    string name;
public:
    Shotgun(string n)
    {name = n;}
    void equipWeapon() override
    {
        cout<<"Equipping Shotgun "<<name<<endl;
    }
};

class Rifle : public Weapon{
private:
    string name;
public:
    Rifle(string n)
    {name = n;}
    void equipWeapon() override
    {
        cout<<"Equipping Rifle "<<name <<endl;
    }
};

class WeaponFactory{
public:

    virtual Weapon* createWeapon() = 0;
    ~WeaponFactory() = default;
};


class ShotgunFactory : public WeaponFactory{
private:
    vector<Shotgun*> shotguns;
public:
    void Reset()
    {
        shotguns = {new Shotgun("Benelli M4 Super 90"),
                    new Shotgun("Remington 870"),
                    new Shotgun("Mossberg 500")
        };

    }
    ShotgunFactory()
    {
        Reset();
    }
    Weapon* createWeapon() override
    {
        cout<<"Selecting Shotgun, adding shotgun ammo"<<endl;
        Shotgun *shotgun = shotguns[rand() % shotguns.size()];
        Reset();
        return shotgun;
    }
};

class RifleFactory : public WeaponFactory{
private:
    vector<Rifle*> rifles;

public:

    void Reset()
    {
        rifles= {new Rifle("M4A1"),
                 new Rifle("MK17"),
                 new Rifle("MK18")
        };
    }

    RifleFactory()
    {
        Reset();
    }

    Weapon* createWeapon() override
    {
        cout<<"Selecting Rifle, adding rifle ammo"<<endl;
        Rifle *rifle = rifles[rand() % rifles.size()];
        Reset();
        return rifle;
    }
};

void FactoryMethodDemo(){


    vector<WeaponFactory*> factories = { new ShotgunFactory, new RifleFactory };

    Weapon* weapon = factories[rand() % factories.size()]->createWeapon();
    weapon->equipWeapon();

}

#endif //LAB_2_FACTORYMETHOD_H
