#include <iostream>
#include <chrono>
#include <random>
#include <thread>
#include <cmath>
#include <stdexcept>
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
        }
        bool isAttackCritical(const int& criticalRate)
        {
            if (randomNumGen() <= criticalRate)
            {
                return true;
            }
        }
};

class actionableEntity
{
    public:
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
        
        void actionLogic(moveset action)
        {
            switch(action)
            {
                case moveset::ATTACK:
                    int moveParameters[3] = {30, 95, 2}; // {power, accuracy, criticalRate}
                    if (logic.hasAttackHit(moveParameters[1]) == true)
                    {
                        cout << "Attack has hit" << endl;
                        if (logic.isAttackCritical(moveParameters[2]) == true)
                        {
                            cout << "Attack is critical" << endl;
                        }
                        else
                        {
                            cout << "No crit" << endl;
                        }
                    }
                    else
                    {
                        cout << "Attack has missed, skipping crit check" << endl;
                    }
            }
        }       
};

class hero: public actionableEntity
{
    private:
        checkForDeath heroDeath;
        heroMoveList hMove;
    public:
        int hitpoints = 500;
        int mana = 500;

        void playerRegularAction()
        {
            if (heroDeath.deathCheck(hitpoints, isDead) != true)
            {
                cout << "Choose from the following:\na to attack\nb to block\nh to heal" << endl;
                heroMoveList::moveset action = hMove.selectAction();
                hMove.actionLogic(action);    
            }
        }
};

class boss: public actionableEntity
{
    private:
        //checkForDeath bossDeath;
    public:
        int hitpoints = 3000;
        int mana = 999;

        bool bossRegularAction()
        {
            //if (bossDeath.deathCheck(hitpoints, isDead) != true);
            {

            }

        }
};

class turnController
{

};

int main()
{
    hero hero;
    hero.playerRegularAction();
    return 0;
}