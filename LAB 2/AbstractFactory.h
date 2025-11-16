
#ifndef LAB_2_ABSTRACTFACTORY_H
#define LAB_2_ABSTRACTFACTORY_H


class PrimaryWeapon{
public:
    virtual void equipPrimary() = 0;
    virtual ~PrimaryWeapon() = default;
};


class SecondaryWeapon{
public:
    virtual void equipSecondary() = 0;
    virtual ~SecondaryWeapon() = default;
};

class MachineGun : public PrimaryWeapon{
    void equipPrimary() override
    {
        cout<<"Equipping Primary : Machine Gun";
    }
};

class Pistol : public SecondaryWeapon{
    void equipSecondary() override
    {
        cout<<"Equipping Secondary : Pistol";
    }
};

class SubMachineGun : public PrimaryWeapon
{
    void equipPrimary() override
    {
        cout<<"Equipping Primary : SubMachine Gun";
    }
};

class ATLauncher : public SecondaryWeapon
{
    void equipSecondary() override{
        cout<<"Equipping Secondary : AT Launcher";
    }
};


class KitFactory{
public:
    virtual PrimaryWeapon* makePrimary() = 0;
    virtual SecondaryWeapon* makeSecondary() = 0;
    virtual ~KitFactory() = default;
};


class MGKitFactory : public KitFactory{
    PrimaryWeapon* makePrimary() override
    {
        return new MachineGun;
    }

    SecondaryWeapon* makeSecondary() override
    {
        return new Pistol;
    }
};


class ATKitFactory : public KitFactory{

    PrimaryWeapon* makePrimary() override
    {
        return new SubMachineGun;
    }

    SecondaryWeapon* makeSecondary() override
    {
        return new ATLauncher;
    }

};


void AbstractFactoryDemo()
{
    vector<KitFactory*> kits = { new MGKitFactory(), new ATKitFactory() };

    for (auto kit : kits)
    {
        PrimaryWeapon* primary = kit->makePrimary();
        SecondaryWeapon* secondary = kit->makeSecondary();
        primary->equipPrimary();
        cout<<", ";
        secondary->equipSecondary();
        cout << endl;
    }
}





#endif //LAB_2_ABSTRACTFACTORY_H
