

#ifndef LAB_4_OBSERVER_H
#define LAB_4_OBSERVER_H


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








#endif //LAB_4_OBSERVER_H
