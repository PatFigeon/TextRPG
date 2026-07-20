#include <iostream>
#include <random>
#include <vector>
#include <string>

using namespace std;

// ==========================================
// LOOT SYSTEM (From Flowcharts)
// ==========================================

// 1. Define Item & Rarity (plain data, no logic)
enum class Rarity { COMMON, UNCOMMON, RARE, LEGENDARY };

struct Item 
{
    string name;
    Rarity rarity;
};

// 2. Build LootTable (weighted roll logic)
class LootTable 
{
    private:
        mt19937 gen;
        vector<pair<Item, int>> tableItems; // Pairs an Item with its drop weight
    public:
        LootTable() 
        {
            random_device rd;
            gen = mt19937(rd());
            
            // Populating the table with items and their weighted chances
            tableItems.push_back({{"A piece of lint", Rarity::COMMON}, 50});
            tableItems.push_back({{"Minor Health Potion", Rarity::UNCOMMON}, 30});
            tableItems.push_back({{"Shiny Armor", Rarity::RARE}, 15});
            tableItems.push_back({{"Divine Blessing", Rarity::LEGENDARY}, 5});
        }

        Item rollItem() 
        {
            int totalWeight = 0;
            for (size_t i = 0; i < tableItems.size(); i++) 
            {
                totalWeight += tableItems.at(i).second;
            }

            uniform_int_distribution<int> distrib(1, totalWeight);
            int randomNum = distrib(gen);

            int currentWeight = 0;
            for (size_t i = 0; i < tableItems.size(); i++) 
            {
                currentWeight += tableItems.at(i).second;
                if (randomNum <= currentWeight) 
                {
                    return tableItems.at(i).first;
                }
            }
            return tableItems.at(0).first; // Fallback
        }
};

// 3. Build LootBox + Factory (tiers & roll counts)
class LootBox 
{
    public:
        string tierName;
        int rollCount;
};

class LootBoxFactory 
{
    public:
        LootBox buildsBox(const int& bossLevel) 
        {
            LootBox box;
            // Picks tier by boss level
            if (bossLevel < 20) 
            {
                box.tierName = "Bronze";
                box.rollCount = 1;
            } 
            else if (bossLevel < 50) 
            {
                box.tierName = "Silver";
                box.rollCount = 2;
            } 
            else 
            {
                box.tierName = "Gold";
                box.rollCount = 3;
            }
            return box;
        }
};

// ==========================================
// EXISTING COMBAT LOOP CLASSES
// ==========================================

class checkForDeath
{
    public:
        bool deathCheck(const int& hitpoints, bool& isDead)
        {
            if (hitpoints <= 0)
                {
                    isDead = true;
                }
            return isDead;
        } 
};

class moveLogic
{
    private:
        mt19937 gen;
        int randomNumGen()
        {
            uniform_int_distribution<int> distrib(0, 100);
            int randomNum = distrib(gen);
            return randomNum;
        }
        float attackVariance()
        {
            uniform_real_distribution<float> distrib(0.9f, 1.1f);
            float attackVariance = distrib(gen);
            return attackVariance;
        }
    public:
        moveLogic()
        {
            random_device rd;
            gen = mt19937(rd());
        }
        bool hasAttackHit(const int& accuracy)
        {
            if (randomNumGen() <= accuracy)
            {
                return true;
            }
            else
            {
                return false;
            }
        }
        bool isAttackCritical(const int& criticalRate)
        {
            if (randomNumGen() <= criticalRate)
            {
                return true;
            }
            else
            {
                return false;
            }          
        }
        int damageFormula(const int& power, const bool& isCritical, const int& level)
        {
            int damageMult;
            int damage;

            if (isCritical == true)
            {
                damageMult = 2;
            }
            else
            {
                damageMult = 1;
            }

            damage = (power * level/5) * attackVariance() * damageMult;
            return damage;
        }

        int healingFormula(const int& power, const int& level)
        {
            int healing = (power * level/10) * attackVariance();
            return healing;
        }

        int updateMana(int& currentMana, const int& manaCost)
        {
            int newMana = currentMana - manaCost;
            return newMana;
        }

        int updateHealthpoints(const int& healing, const int& maxHitpoints, int& currentHitpoints)
        {
            int newHitpoints;
            if (currentHitpoints + healing > maxHitpoints)
            {
                return maxHitpoints;
            }
            else
            {
                newHitpoints = currentHitpoints + healing;
                return newHitpoints;
            }
        }
};

class heroMoveList: public moveLogic
{
    public:
        enum class moveset {ATTACK, HEAL, BLOCK, GIVEUP};

        int power;
        int accuracy;
        int criticalRate;
        int manaCost;
        vector<int> moveParameters;

        moveset selectAction()
        {
            char action;
            while (true)
            {
                cin >> action;
                action = tolower(action);
                switch(action)
                {
                    case 'a':
                        power = 30;
                        accuracy = 95;
                        criticalRate = 2;
                        manaCost = 3;
                        moveParameters = {power, accuracy, criticalRate, manaCost};
                        return moveset::ATTACK;
                    case 'h':
                        power = 20;
                        accuracy = 100;
                        criticalRate = 0;
                        manaCost = 50;
                        moveParameters = {power, accuracy, criticalRate, manaCost};
                        return moveset::HEAL;
                    case 'g':
                        return moveset::GIVEUP;
                    default:
                        cout << "I'm still working on that move buster" << endl;
                }
            }         
        }

        int damage;
        int healing;
        
        int actionLogic(const moveset& action, const int& heroLevel, const int& maxHitpoints, int& currentHitpoints)
        {
            switch(action)
            {
                case moveset::ATTACK:
                    if (hasAttackHit(moveParameters.at(1)) == true)
                    {
                        bool isCritical = isAttackCritical(moveParameters.at(2));
                        damage = damageFormula(moveParameters.at(0), isCritical, heroLevel);
                        if (isCritical == true)
                        {
                            cout << "A critical hit!" << endl;
                        }
                        cout << "The attack hit for " << damage << " damage" << endl;
                    }
                    else
                    {
                        cout << "Attack has missed" << endl;
                    }
                    return moveParameters.at(3);
                case moveset::HEAL:
                    healing = healingFormula(moveParameters.at(0), heroLevel);
                    currentHitpoints = updateHealthpoints(healing, maxHitpoints, currentHitpoints);
                    cout << "Healed for " << healing << " health" << endl;
                    cout << "Hero health is now " << currentHitpoints << " health" << endl;
                    return moveParameters.at(3);
                case moveset::GIVEUP:
                    currentHitpoints = 0;
                    return 0;
                case moveset::BLOCK:
                    return 0;
            }
        }
};

class hero
{
    private:
        checkForDeath heroDeath;
        heroMoveList heroMove;
    public:
        string heroName;
        int heroLevel = 50;
        int maxHitpoints = 500;
        int damageTaken;
        int currentHitpoints = 300; 
        int healingDone;
        int mana = 500;
        bool isDead = false;
        
        vector<Item> inventory; // Added for the new loot system

        void setHeroName(string newName)
        {
            heroName = newName;
        }

        bool checkPlayerDeath()
        {
            if (heroDeath.deathCheck(currentHitpoints, isDead) != true)
            {
                return false;
            }
            else
            {
                return true;
            }
        }
        
        void playerRegularAction()
        {
            cout << "Choose from the following:\na to attack\nb to block\nh to heal\ng to give up (quit)" << endl;
            heroMoveList::moveset action = heroMove.selectAction();
            int manaCost = heroMove.actionLogic(action, heroLevel, maxHitpoints, currentHitpoints);
            mana = heroMove.updateMana(mana, manaCost);
        }

        // 4. Hero receives items and adds to inventory
        void receiveItems(const vector<Item>& items)
        {
            for (const Item& item : items) 
            {
                inventory.push_back(item);
                cout << heroName << " received: " << item.name << "!" << endl;
            }
        }
};

class boss
{
    private:
        checkForDeath bossDeath;
    public:
        int bossLevel = 55;
        int maxHitpoints = 3000;
        int damageTaken;
        int currentHitpoints = maxHitpoints;
        int mana = 999;
        bool isDead = false;

        bool checkBossDeath()
        {
            if (bossDeath.deathCheck(currentHitpoints, isDead) != true)
            {
                return false;
            }
            else
            {
                return true;
            }
        }

        void bossRegularAction()
        {

        }
};

class turnController
{
    public:
        void turnOrder(hero& hero, boss& boss)
        {
            // Combat loop (Existing classes)
            while (hero.checkPlayerDeath() != true && boss.checkBossDeath() != true)
            {
                cout << hero.heroName << "'s turn" << endl;
                hero.playerRegularAction();
                if (hero.checkPlayerDeath() != true)
                {
                    cout << "Enemy turn" << endl;
                    boss.bossRegularAction();
                }        
            }

            // 4. Wire into boss defeat
            // Boss defeated -> checkBossDeath() returns true
            if (boss.checkBossDeath() == true) 
            {
                cout << "\nBoss Defeated!" << endl;
                
                LootBoxFactory boxFactory;
                LootBox droppedBox = boxFactory.buildsBox(boss.bossLevel);
                
                cout << "You found a " << droppedBox.tierName << " loot box!" << endl;
                
                LootTable lootTable;
                vector<Item> droppedItems;
                
                // Roll items based on the box's rollCount
                for (int i = 0; i < droppedBox.rollCount; i++) 
                {
                    droppedItems.push_back(lootTable.rollItem());
                }

                // Hero receives items
                hero.receiveItems(droppedItems);
            }
        }
};

class gameSetup
{
    private:
        turnController turn;
        boss boss;
        hero hero;
    public:
        void setupGame()
        {
            cout << "Welcome to the grand adventure of TextRPG!" << endl;
            cout << "Please enter your hero's name: ";
            string inputName;
            cin >> inputName;
            hero.setHeroName(inputName);
            turn.turnOrder(hero, boss);
        }
};

int main()
{
    gameSetup setup;
    setup.setupGame();
    return 0;
}