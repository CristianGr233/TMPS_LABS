# Creational Design Patterns

### Course: Software Design Techniques and Mechanisms

Author: Cristian Grubîi
----


## Abstract Factory

The Abstract Factory Design Pattern is used for grouping object factories that have a common theme. 
In other words, it is particularly useful for enforcing a constraint on which product objects can be used together.


As an example, consider Kits in a shooter video game. There are different kits, and they allow different kinds of weapons, 
a Primary and a  Secondary. The following snippet shows the declaration of the interfaces for both Primary(AbstractProductA) and Secondary(AbstractProductB) weapons, together
with their concrete implementations (MachineGun - ProductA1, SubMachineGun - ProductA2, Pistol - Product B1, ATLauncher - Product B2). 


```
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

```

Next, KitFactory(AbstractFactory), together with MGKitFactory(ConcreteFactory1), and ATKitFactory(ConcreteFactory2) 
are declared. The role of the factory is to combine
predetermined types of weapons in a kit.


```
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
```

The client uses the interfaces KitFactory, PrimaryWeapon, and SecondaryWeapon, ensuring that each kit
has the correct combination of weapons.

```
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
```




## FactoryMethod

The FactoryMethod Design Principle allows us to create objects without specifying the exact class to create.
It is useful when we want the client to depend on abstractions instead of concrete classes (e.g. using overridden virtual functions
instead of constructors for building objects), especially when the exact class of the object is not known beforehand.
When using this pattern, it is the subclass that decides which class is instantiated.

In the following snippet, we have the Weapon (Product) interface, together with Shotgun (ConcreteProduct1) and
Rifle (ConcreteProduct2) classes. Each concrete product will be instantiated by its corresponding concrete factory.

```
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
```

After declaring the products, we can now declare the WeaponFactory (Creator), and the ShotgunFactory(ConcreteCreator1),
RifleFactory(ConcreteCreator2) respectively. Each product has a name, and all weapons of the same type 
are stored in the corresponding concrete factory. When calling the createWeapon() method, 
the factory selects only one weapon (at random) from its list of weapons, and returns it.

```
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
```

The client code instantiates a product using overridden virtual functions from the factory.
In this snippet, a factory is selected at random (say we wanted equal odds for each type of weapon), 
and a weapon returned by the factory is equipped. 
```
void FactoryMethodDemo(){


    vector<WeaponFactory*> factories = {new ShotgunFactory, new RifleFactory};

    Weapon* weapon = factories[rand() % factories.size()]->createWeapon();
    weapon->equipWeapon();
}
```




## Builder + Prototype

The Builder Design Pattern construct complex objects by separating construction and 
representation. It is useful when classes have many attributes, which often leads to large constructors, making
it difficult for the client to correctly instantiate the class. Another use case is when a class also has
optional attributes, which may also lead to many smaller constructors. 

The Prototype Design Pattern creates objects by cloning existing ones. A way in which it can be useful
is for copying complex objects, whose copies may have mostly similar data.

By combining these patterns, we can build complex objects, clone them, and make
changes to the clones in a readable manner. In the case of C++, using cloning instead of
simple assignments is almost necessary due to the pointers within builder classes 
(e.g. to avoid working with same blocks of memory).

The following is the structure of the Product, together with concrete implementations.
```
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


```

Next are the builders (which are also cloneable), responsible for building weapons one
step at a time. It is worth noting that not all parts that are defined in the interface have
a "real implementation" (i.e. produce a result), due to the fact that, for instance, handguns do not
have handguards, bipods, and stocks. However, it is still convenient to use the Builder pattern, since
the construction code is the same for all weapon types, and the object can still be constructed successfully.

```
class GunBuilder{
public:

    virtual GunBuilder& attachReceiver(string s) = 0;
    virtual GunBuilder& attachBarrel(string s) = 0;
    virtual GunBuilder& attachHandguard(string s) = 0;
    virtual GunBuilder& attachBipod(string s) = 0;
    virtual GunBuilder& attachStock(string s) = 0;
    virtual GunBuilder& attachGrip(string s) = 0;
    virtual GunBuilder& attachMagazine(string s) = 0;
    virtual CraftableWeapon* getResult() = 0;
    virtual GunBuilder* clone() = 0;

};

class ARBuilder : public GunBuilder{
private:


    AssaultRifle* rifle = new AssaultRifle;
    void Reset()
    {
        rifle = new AssaultRifle;
    }


    ARBuilder(ARBuilder &other)
    {
        rifle = new AssaultRifle(*other.rifle);
    }
public:
    ARBuilder(){
        Reset();
    }
    GunBuilder& attachBarrel(string s) override
    {
        rifle->barrel = s;
        return *this;
    }
    GunBuilder& attachBipod(string s) override
    {
        rifle->bipod = s;
        return *this;
    }
    GunBuilder& attachGrip(string s) override
    {
        rifle->grip = s;
        return *this;
    }
    GunBuilder& attachHandguard(string s) override
    {
        rifle->handguard = s;
        return *this;
    }
    GunBuilder& attachMagazine(string s) override
    {
        rifle->magazine = s;
        return *this;
    }
    GunBuilder& attachReceiver(string s) override
    {
        rifle->receiver = s;
        return *this;
    }
    GunBuilder& attachStock(string s) override
    {
        rifle->stock = s;
        return *this;
    }

    CraftableWeapon* getResult()
    {
        return rifle;
    }

    GunBuilder* clone() override
    {
        return new ARBuilder(*this);
    }
};

class HandgunBuilder: public GunBuilder{
private:
    Handgun* handgun;

    void Reset()
    {
        handgun = new Handgun;
    }
    HandgunBuilder(HandgunBuilder &other)
    {
        handgun = new Handgun(*other.handgun);
    }
public:

    HandgunBuilder(){
        Reset();
    }
    GunBuilder& attachBarrel(string s) override
    {
        handgun->barrel = s;
        return *this;
    }
    GunBuilder& attachBipod(string s) override
    {
        return *this;
    }
    GunBuilder& attachGrip(string s) override
    {
        handgun->grip = s;
        return *this;
    }
    GunBuilder& attachHandguard(string s) override
    {
        return *this;
    }
    GunBuilder& attachMagazine(string s) override
    {
        handgun->magazine = s;
        return *this;
    }
    GunBuilder& attachReceiver(string s) override
    {
        handgun->receiver = s;
        return *this;
    }
    GunBuilder& attachStock(string s) override
    {
        return *this;
    }

    CraftableWeapon* getResult(){
        return handgun;
    }

    GunBuilder* clone() override
    {
        return new HandgunBuilder(*this);
    }

};
```

Lastly, the Director defines the order in which the construction steps are called.
It is useful for the case when there is a "default", or "common" object
which may or may not be modified by the client.
```
class Director{
public:
    Director() = default;

    CraftableWeapon* constructWeapon(GunBuilder* builder)
    {
        builder->attachReceiver("Stock Receiver")
        .attachBarrel("Stock Barrel")
        .attachHandguard("Stock Handguard")
        .attachBipod("No Bipod")
        .attachGrip("Stock Pistol Grip")
        .attachStock("Basic Stock")
        .attachMagazine("Stock Magazine");
        return builder->getResult();

    }
};
```

For the client, we create a Handgun Builder and an AR Builder, and pass them to the director, which builds
the weapons. In addition, cloning is used to create new builder instances and customize the
weapons.
```
void BuilderPrototypeDemo()
{
    HandgunBuilder Hbuilder;
    ARBuilder ARbuilder;
    Director director;
    director.constructWeapon(&ARbuilder);
    director.constructWeapon(&Hbuilder);
    GunBuilder* ARbuilder2 = ARbuilder.clone();

    ARbuilder2->attachMagazine("40-round Magazine")
    .attachHandguard("Rail Handguard")
    .attachBarrel("Long Barrel")
    .attachBipod("Bipod");

    cout<<"Customized Rifle:"<<endl;
    ARbuilder2->getResult()->show();
    cout<<endl;
    cout<<"Stock Rifle:"<<endl;
    ARbuilder.getResult()->show();
    cout<<endl;


    GunBuilder* Hbuilder2 = Hbuilder.clone();

    Hbuilder2->attachMagazine("21-round Magazine")
    .attachGrip("Ergonomic Pistol Grip");

    cout<<"Customized Handgun:"<<endl;
    Hbuilder2->getResult()->show();
    cout<<endl;
    cout<<"Stock Handgun:"<<endl;
    Hbuilder.getResult()->show();

}
```



## Singleton

Singletons are used for restricting object creation for a class to only one instance.
There are multiple steps to ensure this is the case: private constructor, deleting the
ability for objects to clone the instance, or for a copy to be used in a function.
Consider a single player game. There should be only one Inventory instance. Implementing
a Singleton for representing the Inventory is a good way to ensure no bugs such as 
"Multiple, conflicting, inventories" exist. Another advantage is that this is a way to
ensure global accessibility, so the inventory may be accessed
from anywhere without the need to pass it as a parameter.

```
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
```


The client code first sets some loadout for an instance, then tries to modify another variable, however
it leads to the same initial instance. There are also examples of which operations are not allowed.
```
void func(InventorySingleton b)
{
    b.printInventory();
}

void SingletonDemo()
{

    InventorySingleton *inventory = InventorySingleton::getInstance();



    inventory->setLoadout("M4A1", "M9", "Frag Grenade");
    InventorySingleton& inventory2 = *InventorySingleton::getInstance();
    inventory2.setPrimaryWeapon("AK-12");
    inventory->printInventory();

    //InventorySingleton inventory3 = *inventory; //<-- Not allowed
    //InventorySingleton inventory4 = *InventorySingleton::getInstance(); //<-- Not allowed
    //func(*inventory); //<-- Not allowed
}
```