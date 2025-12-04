

#ifndef LAB_3_ADAPTER_H
#define LAB_3_ADAPTER_H



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




void AdapterDemo()
{

    InputManager inputManager;
    GamepadAdapter gamepadAdapter(inputManager);

    inputManager.takeInput("MouseL");
    gamepadAdapter.takeInput("TriggerR");
}





#endif //LAB_3_ADAPTER_H
