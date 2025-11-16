

#ifndef LAB_2_BUILDER_PROTOTYPE_H
#define LAB_2_BUILDER_PROTOTYPE_H

#include "CraftableWeapon.h"

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





#endif //LAB_2_BUILDER_PROTOTYPE_H
