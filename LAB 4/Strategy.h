

#ifndef LAB_4_STRATEGY_H
#define LAB_4_STRATEGY_H



class Weapon
{
public:
  virtual void attack() = 0;
};

class MeleeWeapon : public Weapon
{
    void attack() override
    {
        cout<<"Attacking with Melee..."<<endl;
    }
};

class Gun : public Weapon {
private:
    int bullets;
public:
    Gun(int b) {
        bullets = b;
    }

    void attack() override
    {
        if(bullets == 0)
        {
            cout<<"Cannot fire, magazine empty"<<endl;
            return;
        }
        cout<<"Shooting gun..."<<endl;
        cout<<"Bullets left: "<<bullets<<endl;
    }



};

class Grenade : public Weapon
{
private:
    int range;

public:
    Grenade(int r)
    {
        range = r;
    }
    void attack() override
    {
        cout<<"Throwing grenade..."<<endl;
    }
};

class Context{
private:
    Weapon* weapon;
public:
    void equipWeapon(Weapon* w)
    {
        cout<<"Equipping new weapon..."<<endl;
        weapon = w;
    }

    void attackWeapon()
    {
        weapon->attack();
    }

};



void StrategyDemo()
{
    MeleeWeapon* Knife = new MeleeWeapon;
    Gun* M4 = new Gun(30);
    Grenade* F1 = new Grenade(70);
    Context context;

    context.equipWeapon(M4);
    context.attackWeapon();
    context.equipWeapon(Knife);
    context.attackWeapon();
    context.equipWeapon(F1);
    context.attackWeapon();

}





#endif //LAB_4_STRATEGY_H
