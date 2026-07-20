#include <iostream>
#include <random>
#include <vector>
#include <string>

using namespace std;

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

        // LOOTBOX SYSTEM
        string rollLootBox(int& currentHitpoints, const int& maxHitpoints)
        {
            int roll = randomNumGen();
            if (roll <= 40)
            {
                return "COMMON DROP: A piece of lint. (Nothing happened)";
            }
            else if (roll <= 70)
            {
                currentHitpoints = updateHealthpoints(40, maxHitpoints, currentHitpoints);
                return "UNCOMMON DROP: Minor Health Potion! Restored 40 HP.";
            }
            else if (roll <= 95)
            {
                return "RARE DROP: Shiny Armor! (Looks cool, does nothing)";
            }
            else
            {
                currentHitpoints = maxHitpoints;
                return "LEGENDARY DROP: Divine Blessing! Fully restored HP!";
            }
        }
};

class heroMoveList: public moveLogic
{
    public:
        enum class moveset {ATTACK, HEAL, BLOCK, GIVEUP, LOOTBOX};

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
                    case 'l':
                        power = 0;
                        accuracy = 100;
                        criticalRate = 0;
                        manaCost = 10;
                        moveParameters = {power, accuracy, criticalRate, manaCost};
                        return moveset::LOOTBOX;
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
                case moveset::LOOTBOX:
                    cout << "Opening a Surprise Mechanics Loot Box..." << endl;
                    cout << rollLootBox(currentHitpoints, maxHitpoints) << endl;
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
        int currentHitpoints = 300; //lowered for heal command test
        int healingDone;
        int mana = 500;
        bool isDead = false;

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
            cout << "Choose from the following:\na to attack\nb to block\nh to heal\nl to open loot box\ng to give up (quit)" << endl;
            heroMoveList::moveset action = heroMove.selectAction();
            int manaCost = heroMove.actionLogic(action, heroLevel, maxHitpoints, currentHitpoints);
            mana = heroMove.updateMana(mana, manaCost);
            cout << "Hero mana is now " << mana << endl;
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
            while (hero.checkPlayerDeath() != true && boss.checkBossDeath() != true)
            {
                //cout << "Player death check passed" << endl;
                cout << hero.heroName << "'s turn" << endl;
                hero.playerRegularAction();
                //cout << "Boss death check passed" << endl;
                if (hero.checkPlayerDeath() != true)
                {
                    cout << "Enemy turn" << endl;
                    boss.bossRegularAction();
                }        
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