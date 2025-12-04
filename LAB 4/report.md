# BehaviouralDesign Patterns

### Course: Software Design Techniques and Mechanisms

Author: Cristian Grubîi
----

## Strategy

The Strategy Design Pattern allows the client to interchange classes that have
implemented a family of algorithms. 

For the implementation was considered the case when, in a shooter video game, 
a player may want to attack using different types of weapons - either a 
melee weapon, a gun, or a grenade(each having its own kind of logic). The player
presses the same button, but expects a different
result based on the weapon equipped. This is an example where
the Strategy pattern may help do this in a clean manner.

Firstly, the weapon interface is declared , with the method attack(), followed
by all its concrete implementations : MeleeWeapon, Gun, and Grenade.


```
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
```


Then is defined the Context class, which is responsible
for equipping a weapon, then executing its logic.

```
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
```

In the client code, different kinds of weapons are
equipped, followed by their attack logic.
```
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
```

And the results show how different objects can be
interchangeable.
```
Equipping new weapon...
Shooting gun...
Bullets left: 30
Equipping new weapon...
Attacking with Melee...
Equipping new weapon...
Throwing grenade...
```



## Observer

The Observer Design Pattern is useful when multiple objects
need to know about events that happen to another object.

The Facade code from the last lab was restructured in a way to fit this
scenario: 


The handlers are reused:
```
class SFXHandler {
public:
    void playSFX(string sound)
    {
        cout<<"Playing Sound: "<<sound<<endl;
    }
};

class VFXHandler{
public:
    void showVFX(string effect)
    {
        cout<<"Showing Effect: " << effect<<endl;
    }

};


class AnimationHandler{
private:
    vector<pair<string,float>> animations {
            {"ANIM_Gun_Equip", 3.6},
            {"ANIM_Gun_Load_Mag", 2.7},
            {"ANIM_Gun_Unload_Mag", 1.8},
            {"ANIM_Gun_Load_Chamber", 1},
            {"ANIM_Weapon_Recoil", 0.2},
            {"ANIM_Gun_Aim", 2.4},
            {"ANIM_Gun_Lower", 2}
    };
public:
    void playAnimation(string animation_name)
    {
        cout<<"Playing Animation: "<<animation_name<<endl;

    }
    float getAnimationTime(string animation_name)
    {
        for(auto animation : animations)
        {
            if(animation.first == animation_name)
                return animation.second;
        }
        cout<<"Animation not found: "<<animation_name<<endl;
        return 0;
    }

};
```

A new interface - GunObserver, and its concrete implementations
(for each type of handler) are also defined:

```
class GunObserver{
public:
    virtual void onFire() {}
    virtual void onReload() {}
    virtual void onAim(bool aiming) {}
};


class SFXObserver : public GunObserver{
private:
    SFXHandler sfx;
public:
    SFXObserver(SFXHandler s)
    {
        sfx = s;
    }

    void onFire() override{
        sfx.playSFX("SFX_Gunshot");
        sfx.playSFX("SFX_Casing_Drop");
    }

    void onReload() override {
        sfx.playSFX("SFX_Gun_Reload");
    }

};


class VFXObserver : public GunObserver{
private:
    VFXHandler vfx;
public:
    VFXObserver(VFXHandler v)
    {
        vfx = v;
    }

    void onFire() override {
        vfx.showVFX("VFX_Muzzle_Flash");
    }
};

class AnimationObserver : public GunObserver{
private:
    AnimationHandler anim;
public:

    AnimationObserver(AnimationHandler a)
    {
        anim = a;
    }

    void onFire() override {
        anim.playAnimation("ANIM_Gun_Recoil");
    }

    void onReload() override {
        anim.playAnimation("ANIM_Gun_Reload");
    }

    void onAim(bool aiming) override {
        anim.playAnimation(aiming ? "ANIM_Gun_Aim" : "ANIM_Gun_Lower");
    }
};
```

Last is the GunSubject, which is the "observed" class.

```
class GunSubject{
private:
    vector<GunObserver*> observers;
public:
    void addObserver(GunObserver* obs)
    {
        observers.push_back(obs);
    }

    void notifyFire()
    {
        for(auto observer : observers)
            observer->onFire();
    }

    void notifyReload()
    {
        for(auto observer : observers)
            observer->onReload();
    }

    void notifyAim(bool aiming)
    {
        for(auto observer : observers)
            observer->onAim(aiming);
    }

};
```

So this is different behaviour from the Facade implementation, due to the fact 
that the logic of each handler for each kind of event is encapsulated
within the observer classes, so there is no central class calling each subsystem
whenever needed.



The client code consists of intializing the observers and 
adding them to the subject, followed by an input:
```
void ObserverDemo()
{
    SFXHandler sfxHandler;
    SFXObserver* sfxObserver = new SFXObserver(sfxHandler);
    VFXHandler vfxHandler;
    VFXObserver* vfxObserver = new VFXObserver(vfxHandler);
    AnimationHandler animationHandler;
    AnimationObserver* animationObserver = new AnimationObserver(animationHandler);

    GunSubject gunSubject;
    gunSubject.addObserver(sfxObserver);
    gunSubject.addObserver(vfxObserver);
    gunSubject.addObserver(animationObserver);

    string input = "MouseL";
    bool aiming = false;
    if(input == "MouseL")
        gunSubject.notifyFire();
    else if(input == "MouseR")
    {
        gunSubject.notifyAim(aiming);
        aiming = !aiming;
    }
    else if (input == "R")
    {
        gunSubject.notifyReload();
    }
    
}
```

The result is shown for only one type of input:
```
Playing Sound: SFX_Gunshot
Playing Sound: SFX_Casing_Drop
Showing Effect: VFX_Muzzle_Flash
Playing Animation: ANIM_Gun_Recoil
```


## Mediator

The Mediator Design Pattern 


The Facade code from the last lab (+ the Observer one from this lab) was restructured in a way to fit this
design pattern, such that it's not a class calling every subsystem, nor subsystems 
reacting to events of a class, but rather subsystems "communicating" with each other through a central class.
In both of previous cases - the events within a subsystem are irrelevant to the others. However, it will be 
seen in this case that subsystems notify the mediator of certain internal events, then the mediator
may call other subsystems based on this internal event. 
Still, in all 3 cases - the subsystem are unaware(directly) of eachother(i.e. no dependencies).
The main idea of the implementation is the following: Mediator defines "triggers" - actions
that should take place when a particular condition is met.


First are declared the components(subsystems): first an interface,
then its concrete implementations - which have internal logic (called by mediator), then
notify the mediator when they are done. All subsystems have a mediator instance declared:
```
class IMediator;

class IComponent {
protected:
    IMediator* mediator = nullptr;

public:
    void setMediator(IMediator* m)
    {
        mediator = m;
    }
};

class SFXComponent : public IComponent {
public:
    void play(string sfx) {
        cout << "Playing Sound: " << sfx << endl;

        mediator->notify(this, "SFX_Played_" + sfx.substr(4));
    }
};

class VFXComponent : public IComponent {
public:
    void show(string vfx) {
        cout << "Showing Effect: " << vfx << endl;

        mediator->notify(this, "VFX_Played_" + vfx.substr(4));
    }
};

class AnimationComponent : public IComponent {
public:
    void play(string anim) {
        cout << "Playing Animation: " << anim << endl;

        mediator->notify(this, "Animation_Finished_" + anim.substr(5));
    }
};

class GunComponent : public IComponent {
private:
    bool aiming = false;
    int ammo;

public:
    void fire() {
        if(ammo == 0)
        {
            mediator->notify(this, "Gun_Empty_Fire");
            return;
        }

        ammo--;
        mediator->notify(this, "Gun_Fire");

        bool jam = rand()%2;
        if(jam)
            mediator->notify(this, "Gun_Jammed");


    }

    void reload() {
        mediator->notify(this, "Gun_Reload");
    }

    void toggleAim() {
        aiming = !aiming;
        if(aiming)
            mediator->notify(this, "Gun_Aim");
        else
            mediator->notify(this, "Gun_Lower");
    }
    void setAmmo(int a)
    {
        ammo = a;
    }
};

```

Then follows the Mediator itself. It has a "ReactOnX()" method (where
X is a subsystem) for each subsystem. So depending on who is notifying an event
(as seen in the notify() method), it calls the appropriate ReactOnX() method. This is 
different from the Facade because now its not one-way communication(Facade -> Subsystem), but a two-way
communication(Mediator<-> Subsystem) for each subsystem. In addition, the Facade is more of a 
God object than the Mediator:

```
class GunMediator : public IMediator {
private:
    GunComponent* gun;
    SFXComponent* sfx;
    VFXComponent* vfx;
    AnimationComponent* anim;

    void ReactOnGun(string event)
    {
        if(event == "Gun_Fire")
        {
            sfx->play("SFX_Gunshot");
            sfx->play("SFX_Casing_Drop");

            vfx->show("VFX_Muzzle_Flash");

            anim->play("ANIM_Gun_Recoil");
        }
        else if(event == "Gun_Empty_Fire")
        {
            sfx->play("SFX_Empty_Click");
        }
        else if(event == "Gun_Reload")
        {
            anim->play("ANIM_Gun_Reload");
            sfx->play("SFX_Gun_Reload");
        }
        else if(event == "Gun_Aim")
        {
            anim->play("ANIM_Gun_Aim");
        }
        else if(event == "Gun_Lower")
        {
            anim->play("ANIM_Gun_Lower");
        }
        else if(event == "Gun_Jammed")
        {
            sfx->play("SFX_Jammed_Gun");
            anim->play("ANIM_Gun_Unjam");
        }
    }

    void ReactOnAnim(string event)
    {
        if(event == "Animation_Finished_Gun_Reload")
        {
            vfx->show("VFX_Mag_Drop");
        }
        else if(event == "Animation_Finished_Gun_Aim")
        {
            cout << "Camera sway reduced." << endl;
        }
    }
    void ReactOnSFX(string event)
    {
        if(event == "SFX_Played_Casing_Drop")
        {
            vfx->show("VFX_Casing_Hit_Ground");
        }
        else if(event == "SFX_Played_Gun_Reload")
        {
            vfx->show("VFX_Small_Shake");
        }
    }

    void ReactOnVFX(string event)
    {
        if(event == "VFX_Played_Muzzle_Flash")
        {
            vfx->show("VFX_Camera_Bloom");
        }
    }

public:
    GunMediator(GunComponent* g, SFXComponent* s, VFXComponent* v, AnimationComponent* a)
    {
        gun = g;
        sfx = s;
        vfx = v;
        anim = a;
        gun->setMediator(this);
        sfx->setMediator(this);
        vfx->setMediator(this);
        anim->setMediator(this);
    }

    void notify(IComponent* sender, string event) override
    {
        if (sender == gun)
            ReactOnGun(event);
        else if (sender == sfx)
            ReactOnSFX(event);
        else if (sender == vfx)
            ReactOnVFX(event);
        else if (sender == anim)
            ReactOnAnim(event);
    }

    void fireGun() override
    {
        gun->fire();
    }
    void reloadGun() override
    {
        gun->reload();
    }

    void toggleAimGun() override
    {
        gun->toggleAim();
    }

};
```


The client code also communicates only with the mediator, not
the subsystems directly:
```
void MediatorDemo()
{

    SFXComponent sfx;
    VFXComponent vfx;
    AnimationComponent anim;
    GunComponent gun;




    GunMediator mediator(&gun, &sfx, &vfx, &anim);
    gun.setAmmo(2);

    mediator.toggleAimGun();
    cout<<endl;
    mediator.fireGun();
    cout<<endl;
    mediator.fireGun();
    cout<<endl;
    mediator.fireGun();
    cout<<endl;
    mediator.reloadGun();
    cout<<endl;
    mediator.toggleAimGun();
}
```

And this is the result:
```
Playing Animation: ANIM_Gun_Aim
Camera sway reduced.

Playing Sound: SFX_Gunshot
Playing Sound: SFX_Casing_Drop
Showing Effect: VFX_Casing_Hit_Ground
Showing Effect: VFX_Muzzle_Flash
Showing Effect: VFX_Camera_Bloom
Playing Animation: ANIM_Gun_Recoil
Playing Sound: SFX_Jammed_Gun
Playing Animation: ANIM_Gun_Unjam

Playing Sound: SFX_Gunshot
Playing Sound: SFX_Casing_Drop
Showing Effect: VFX_Casing_Hit_Ground
Showing Effect: VFX_Muzzle_Flash
Showing Effect: VFX_Camera_Bloom
Playing Animation: ANIM_Gun_Recoil
Playing Sound: SFX_Jammed_Gun
Playing Animation: ANIM_Gun_Unjam

Playing Sound: SFX_Empty_Click

Playing Animation: ANIM_Gun_Reload
Showing Effect: VFX_Mag_Drop
Playing Sound: SFX_Gun_Reload
Showing Effect: VFX_Small_Shake

Playing Animation: ANIM_Gun_Lower
```