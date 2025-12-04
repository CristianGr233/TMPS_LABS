# Structural Design Patterns

### Course: Software Design Techniques and Mechanisms

Author: Cristian Grubîi
----

## Adapter

The Adapter Design Pattern is used for converting the interface
of a class into an interface a client expects, letting classes interact
that couldn't otherwise due to incompatibility.

For implementation was considered the case when there is a game
that only support keyboard/mouse:

```
class InputManager {
vector<pair<string, void (InputManager::*)()>> inputs;

public:
void fireGun() {
cout << "Firing Gun..." << endl;
}

    void aimGun()
    {
        cout<<"Aiming Gun..."<<endl;
    }

    void reloadGun()
    {
        cout<<"Reloading Gun..."<<endl;
    }


    void takeInput(string input_key)
    {
        cout<<"\n\nReceived Input (Keyboard/Mouse): "<<input_key<<endl;
        for(auto input : inputs)
        {
            if(input.first == input_key)
                (this->*input.second)();
        }
    }

    InputManager() {
        inputs = {{"MouseL", &InputManager::fireGun},
                  {"MouseR", &InputManager::aimGun},
                  {"R",      &InputManager::reloadGun}};
    }
};
```

But then a new controller becomes
supported (e.g. a Gamepad). Its inputs are not compatible
with the InputManager interface. Instead of modifying
the InputManager interface, which, firstly - breaks OCP, and 
secondly - in practice may not work because, say there was a restriction
to only one input-to-method mapping in the class, or errors would appear
due to the program not recognizing the gamepad inputs properly. 
It would also be tedious to rewrite all the logic 
that has to happen in a new "Gamepad" class. This means that
adding an adapter which works properly with the controller
inputs, and assigns methods(from InputManager) to them would
lead to the client getting the expected result.


```
class GamepadAdapter{
    InputManager inputManager;

    vector<pair<string, void (InputManager::*)()>> inputs;
public:
    GamepadAdapter(InputManager& inp) {

        inputManager = inp;

        inputs = {{"TriggerR", &InputManager::fireGun},
                  {"TriggerL", &InputManager::aimGun},
                  {"X",      &InputManager::reloadGun}};
    }

    void takeInput(string input_key)
    {
        cout<<"Received Input (Controller): "<<input_key<<endl;
        for(auto input : inputs)
        {
            if(input.first == input_key)
                (inputManager.*input.second)();
        }
    }
};

```

Obviously, the implementation of the adapter is more
based on the concept rather than something practical.
Nevertheless, the client can reach the expected result
using either Keyboard/Mouse or the Gamepad.


The Client code consists of just "forwarding input"
to the inputManager and gamepadAdapter objects.

```
void AdapterDemo()
{

    InputManager inputManager;
    GamepadAdapter gamepadAdapter(inputManager);

    inputManager.takeInput("MouseL");
    gamepadAdapter.takeInput("TriggerR");
}
```

From the result, it can be seen that the input types were
different, yet the expected result was reached.

```
Received Input (Keyboard/Mouse): MouseL
Firing Gun...
Received Input (Controller): TriggerR
Firing Gun...
```




## Composite

The Composite Design Pattern is useful for the cases
when the objects can (and should) be organized in a tree structure, reaching
part-whole hierarchies. Both the objects and their compositions 
can be treated uniformly by the client, which means that if the types differ, 
the differences can be ignored.

For implementation, I considered an analogy to an inventory
system. There are "primitive" objects
(i.e. other objects cannot be nested within them) such as medkit, magazines, etc.
Then there are "container objects" - uniforms, vests, backpacks 
(and something more abstract - the inventory itself).
For a feature such as calculating total weight of a particular container, there would 
be a need for checking the type of the object, then if there was more subnesting, it would lead
to a lot of recursive calls, affecting performance.


For the code, first the Item interface is declared, with the
printContents() and getWeight() virtual methods:

```
class Item{
public:
    virtual void printContents() = 0;
    virtual float getWeight() = 0;
};
```

The "primitive" items will be referred to as "RegularItem".
printContents() simply returns the name of the object, and getWeight() - 
directly returns its weight.
```
class RegularItem : public Item
{
private:
    string name;
    float weight;
public:
    RegularItem(string n, float w){
        name = n;
        weight = w;
    }
    void printContents() override{
        cout<<name<<endl;
    }
    float getWeight()
    {
        return weight;
    }
};
```

Then, there is also the class Container, which also has
nodes of type Item*. The printContents() method prints the name of 
the object and those of the subnested objects(all the way to the primitives).
The same is done for weight, however, in this case, the weight of the 
containers themselves are neglected.

```
class Container : public Item
{
private:
    string name;
    vector<Item*> nodes;

public:
    Container(string n){
        name = n;
    }
    void printContents() override
    {
        cout<<name<<" Contents :"<<endl;
        for(auto node : nodes)
            node->printContents();
    }

    void add(Item* item){
        nodes.push_back(item);
    }


    float getWeight()
    {
        float weight = 0;
        for(auto item : nodes)
        {
            weight+= item->getWeight();
        }
        return weight;
    }

};
```

The demo declares 3 Containers : backpack, vest, and uniform.
And Items like rifle, medkit, and magazines.

```
void CompositeDemo()
{

    Container* backpack = new Container("Backpack");
    Container* vest = new Container("Vest");
    Container* uniform = new Container("Uniform");
    RegularItem* rifle = new RegularItem("M16", 4);
    RegularItem* medkit = new RegularItem("Medkit", 1);
    RegularItem* mag_1 = new RegularItem("M16_mag", 0.6);
    RegularItem* mag_2 = new RegularItem("M16_mag", 0.6);
    RegularItem* mag_3 = new RegularItem("M16_mag", 0.6);

    backpack->add(medkit);
    vest->add(mag_1);
    vest->add(mag_2);
    vest->add(mag_3);

    Container inventory("Player_inventory");
    inventory.add(rifle);
    inventory.add(uniform);
    inventory.add(vest);
    inventory.add(backpack);

    inventory.printContents();
    cout<<"Total Weight: "<<inventory.getWeight()<<" kg";


}
```


The results can be likened to those of a Breadth-First Search - it goes level
by level, and when it reaches a level - it prints the names of all 
primitives(i.e. leaf nodes). If a container is found, it goes down a level.

```
Player_inventory Contents :
M16
Uniform Contents :
Vest Contents :
M16_mag
M16_mag
M16_mag
Backpack Contents :
Medkit
Total Weight: 6.8 kg
```



## Facade

The Facade Design Pattern is used for hiding complex subsystem
interactions from the client.

As an analogy was considered the different types of handlers
in the average video game, such as SFX, VFX, Animations, etc.
The client should not have to call the methods of the objects of these
classes, it would be better if only the input was passed to a class (e.g. the Facade), then
this class orchestrated everything. Furthermore, the subsystems
may not be able to know about eachother, thus another arguments for 
the need of an orchestrator, which calls each subsystem itself. 


For the code, first are considered some gun stats, the Gun, and Player(which can have a state)
classes.

```
struct GunStats{
    int recoil;
    int magazine;
};

class Gun{
private:
    string model;
    GunStats gunStats;
    int bullets;
public:
    Gun() = default;

    Gun(string mdl, GunStats gstats) {
        model = mdl;
        gunStats = gstats;
        bullets = gstats.magazine;
    }

    bool canFire()
    {
        return bullets > 0;
    }
    void consumeBullet()
    {
        bullets--;
    }

    void reload()
    {
        bullets = gunStats.magazine;
    }

    int getBullets()
    {
        return bullets;
    }

    string getModel()
    {
        return model;
    }

};

class Player{
public:
    enum class State { Idle, Aiming, Shooting, Reloading };
private:
    State state = State::Idle;

public:
    State getState()
    {
        return state;
    }
    void setState(State s)
    {
        state = s;
    }
};
```


Then are the handlers: SFXHandler - which plays sounds, VFXHandler - which displays particles
and other visual data, AnimationHandler - which is responsible for animations, and
EntityHandler - responsible for managing entities(e.g. creating them). These
handlers are not aware of one another, they just perform their duties independently.
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

class EntityHandler{
private:
    vector<pair<string, GunStats>> guns = {
            {"M4A1", {46, 30}},
            {"AK-12", {70, 30}}
    };
public:
    Gun createGun(string gun_model)
    {
        GunStats gunStats = {0, 0};
        for(auto g : guns)
        {
            if(gun_model == g.first)
                gunStats = g.second;

        }
        Gun gun(gun_model, gunStats);
        cout<<"Creating Gun"<<endl;
        cout<<"Generating mesh..."<<endl;
        cout<<"Generating textures..."<<endl;

        return gun;
    }


};
```


The GunFacade class is responsible for taking an input and performing
an operation(i.e. call one of its methods) based on it. In addition, 
there is the equipGun() method that creates the Gun itself(e.g. loads textures, mesh, etc.), and plays
some animations and sounds. As for the other methods, it can be seen how
the facade makes the cooperation of subsystems easier, without unnecessary dependencies.


```
class GunFacade{
private:
    SFXHandler sfxHandler;
    VFXHandler vfxHandler;
    AnimationHandler animationHandler;
    EntityHandler entityHandler;
    vector<pair<string, void(GunFacade::*)(Gun&, Player&)>> inputs;



    void fireGun(Gun& gun, Player& player)
    {
        if (!gun.canFire()) {
           cout<<"Cannot fire, reload gun!"<<endl;
           return;
        }
        if (player.getState() == Player::State::Reloading) {
            cout << "Cannot fire while reloading!" << endl;
            return;
        }

        Player::State prevState = player.getState();
        player.setState(Player::State::Shooting);
        gun.consumeBullet();

       vfxHandler.showVFX("VFX_Muzzle_Flash");
       sfxHandler.playSFX("SFX_Gunshot");
       animationHandler.playAnimation("ANIM_Gun_Recoil");

       player.setState(prevState);

       sfxHandler.playSFX("SFX_Casing_Drop");
    }

    void aimGun(Gun& gun, Player& player)
    {
        if (player.getState() == Player::State::Aiming) {
            player.setState(Player::State::Idle);
            animationHandler.playAnimation("ANIM_Gun_Lower");
        }
        else if (player.getState() == Player::State::Idle) {
            player.setState(Player::State::Aiming);
            animationHandler.playAnimation("ANIM_Gun_Aim");
        }


    }


    void reloadGun(Gun& gun, Player& player)
    {
        if (player.getState() == Player::State::Shooting) {
            cout << "Can't reload while shooting!" << endl;
            return;
        }

        Player::State prevState = player.getState();
        player.setState(Player::State::Reloading);
        cout<<"Reloading Gun..."<<endl;

        animationHandler.playAnimation("ANIM_Gun_Unload_Mag");
        sfxHandler.playSFX("SFX_Gun_Unload_Mag");
        animationHandler.playAnimation("ANIM_Gun_Load_Mag");
        sfxHandler.playSFX("SFX_Gun_Load_Mag");
        animationHandler.playAnimation("ANIM_Gun_Load_Chamber");
        sfxHandler.playSFX("SFX_Gun_Load_Chamber");

        gun.reload();

        player.setState(prevState);
    }

public:
    GunFacade(VFXHandler& vfx, SFXHandler& sfx, AnimationHandler& anim, EntityHandler& entity)
    {
        vfxHandler = vfx;
        sfxHandler = sfx;
        animationHandler = anim;
        entityHandler = entity;
        inputs = {
                {"MouseL", &GunFacade::fireGun},
                {"MouseR", &GunFacade::aimGun},
                {"R", &GunFacade::reloadGun}
        };
    }

    Gun equipGun(string gun_model)
    {
        Gun gun = entityHandler.createGun(gun_model);

        cout<<"\nEquipping gun..."<<endl;

        animationHandler.playAnimation("ANIM_Gun_Equip");
        animationHandler.playAnimation("ANIM_Gun_Load_Mag");
        sfxHandler.playSFX("SFX_Gun_Load_Mag");
        animationHandler.playAnimation("ANIM_Gun_Load_Chamber");
        sfxHandler.playSFX("Gun_Load_Chamber");

        cout<<"Equipped gun"<<endl;
        return gun;
    }

    void processInput(string input_key, Gun& gun, Player& player)
    {
        cout<<"\n\nReceived Input : "<<input_key<<"\n\n";
        for(auto input : inputs)
        {
            if(input.first == input_key)
                (this->*input.second)(gun, player);
        }
    }


};
```

The client code for the most part consists of inputs of keyboard
and mouse being passed to the facade.

```
void FacadeDemo()
{
SFXHandler sfxHandler;
VFXHandler vfxHandler;
AnimationHandler animationHandler;
EntityHandler entityHandler;

    GunFacade gunFacade(vfxHandler, sfxHandler, animationHandler, entityHandler);
    Player player1;
    Gun gun = gunFacade.equipGun("M4A1");
    gunFacade.processInput("MouseL", gun, player1);
    gunFacade.processInput("MouseR", gun, player1);
    gunFacade.processInput("MouseL", gun, player1);
    gunFacade.processInput("R", gun, player1);
    gunFacade.processInput("MouseL", gun, player1);
    gunFacade.processInput("MouseR", gun, player1);

}
```

And the result of the client code is the following:

```
Creating Gun
Generating mesh...
Generating textures...

Equipping gun...
Playing Animation: ANIM_Gun_Equip
Playing Animation: ANIM_Gun_Load_Mag
Playing Sound: SFX_Gun_Load_Mag
Playing Animation: ANIM_Gun_Load_Chamber
Playing Sound: Gun_Load_Chamber
Equipped gun


Received Input : MouseL

Showing Effect: VFX_Muzzle_Flash
Playing Sound: SFX_Gunshot
Playing Animation: ANIM_Gun_Recoil
Playing Sound: SFX_Casing_Drop


Received Input : MouseR

Playing Animation: ANIM_Gun_Aim


Received Input : MouseL

Showing Effect: VFX_Muzzle_Flash
Playing Sound: SFX_Gunshot
Playing Animation: ANIM_Gun_Recoil
Playing Sound: SFX_Casing_Drop


Received Input : R

Reloading Gun...
Playing Animation: ANIM_Gun_Unload_Mag
Playing Sound: SFX_Gun_Unload_Mag
Playing Animation: ANIM_Gun_Load_Mag
Playing Sound: SFX_Gun_Load_Mag
Playing Animation: ANIM_Gun_Load_Chamber
Playing Sound: SFX_Gun_Load_Chamber


Received Input : MouseL

Showing Effect: VFX_Muzzle_Flash
Playing Sound: SFX_Gunshot
Playing Animation: ANIM_Gun_Recoil
Playing Sound: SFX_Casing_Drop


Received Input : MouseR

Playing Animation: ANIM_Gun_Lower
```

