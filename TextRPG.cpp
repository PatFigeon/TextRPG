#include <iostream>
#include <random>

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

class hitLogic
{
    private:
        mt19937 gen;
        int randomNumGen()
        {
            uniform_int_distribution<int> distrib(0, 100);
            int randomNum = distrib(gen);
            return randomNum;
        }
        double attackVariance()
        {
            uniform_int_distribution<int> distrib(80, 120);
            float generatedValue = distrib(gen);
            float attackVariance = generatedValue/100;
            return attackVariance;
        }
    public:
        hitLogic()
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
};

class actionableEntity
{
    public:
        int level;
        int hitpoints;
        int mana;
        bool isPoisoned = false;
        bool isBlocking = false;
        bool isDead = false;
};

class heroMoveList
{
    private:
        hitLogic logic;
    public:
        enum class moveset {ATTACK, HEAL, BLOCK};

        moveset selectAction()
        {
            char action;
            cin >> action;
            action = tolower(action);
            switch(action)
            {
                case 'a':
                    return moveset::ATTACK;
                default:
                    cout << "I'm still working on that move buster" << endl;
            }
        }
        
        void actionLogic(moveset action, int heroLevel)
        {
            switch(action)
            {
                case moveset::ATTACK:
                    int moveParameters[3] = {30, 95, 2}; // {power, accuracy, criticalRate}
                    if (logic.hasAttackHit(moveParameters[1]) == true)
                    {
                        bool isCritical = logic.isAttackCritical(moveParameters[2]);
                        int damage = logic.damageFormula(moveParameters[0], isCritical, heroLevel);
                        if (isCritical == true)
                        {
                            cout << "A critical hit!" << endl;
                        }
                        cout << "The attack hit for " << damage << endl;
                    }
                    else
                    {
                        cout << "Attack has missed" << endl;
                    }
            }
        }       
};

class hero: public actionableEntity
{
    private:
        checkForDeath heroDeath;
        heroMoveList hMove;
        actionableEntity heroEntity;
    public:
        int heroLevel = 50;
        int hitpoints = 500;
        int mana = 500;

        bool checkPlayerDeath()
        {
            if (heroDeath.deathCheck(hitpoints, heroEntity.isDead) != true)
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
            cout << "Choose from the following:\na to attack\nb to block\nh to heal" << endl;
            heroMoveList::moveset action = hMove.selectAction();
            hMove.actionLogic(action, heroLevel); 
        }
};

class boss: public actionableEntity
{
    private:
        checkForDeath bossDeath;
    public:
        int bossLevel = 55;
        int hitpoints = 3000;
        int mana = 999;

        bool checkBossDeath()
        {
            if (bossDeath.deathCheck(hitpoints, isDead) != true)
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
    private:
        hero hero;
        boss boss;
        checkForDeath bossDeath;
        checkForDeath heroDeath;
    public:
        void turnOrder()
        {
            while (hero.checkPlayerDeath() != true && boss.checkBossDeath() != true)
            {
                cout << "Player death check passed" << endl;
                hero.playerRegularAction();
                cout << "Boss death check passed" << endl;
                boss.bossRegularAction();            
            }
        }
};

int main()
{
    turnController turn;
    turn.turnOrder();
    return 0;
}