

#ifndef LAB_3_FACADE_H
#define LAB_3_FACADE_H

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


#endif //LAB_3_FACADE_H
