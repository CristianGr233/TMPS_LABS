
#ifndef LAB_4_MEDIATOR_H
#define LAB_4_MEDIATOR_H


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

class IMediator {
public:
    virtual void notify(IComponent* sender, string event) = 0;

    virtual void fireGun() = 0;
    virtual void reloadGun() = 0;
    virtual void toggleAimGun() = 0;
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


void MediatorDemo()
{

    SFXComponent sfx;
    VFXComponent vfx;
    AnimationComponent anim;
    GunComponent gun;




    GunMediator mediator(&gun, &sfx, &vfx, &anim);
    sfx.setMediator(&mediator);
    vfx.setMediator(&mediator);
    anim.setMediator(&mediator);
    gun.setMediator(&mediator);
    gun.setAmmo(2);

    gun.toggleAim();
    cout<<endl;
    mediator.fireGun();
    cout<<endl;
    mediator.fireGun();
    cout<<endl;
    mediator.fireGun();
    cout<<endl;
    gun.reload();
    cout<<endl;
    gun.toggleAim();
}



#endif //LAB_4_MEDIATOR_H

