
#ifndef LAB_3_COMPOSITE_H
#define LAB_3_COMPOSITE_H


class Item{
public:
    virtual void printContents() = 0;
    virtual float getWeight() = 0;
};

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












#endif //LAB_3_COMPOSITE_H
