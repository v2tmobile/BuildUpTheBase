#pragma once
#ifndef GAMELOGIC_H
#define GAMELOGIC_H

class Beatup;
class Buildup;

///Handles the game's logic, most of it will handle here and
/// be sent to the Layers to update visual stuff as needed
//
/// The goal is to have all the logic in one set of files MVC style and 
///  the display of it somewhere else

class GameLogic
{
    protected:
        static GameLogic* _instance;
    public:
        GameLogic();
        bool init();

        void update(float dt);

        static GameLogic* getInstance();

        Beatup* beatup;
        Buildup* buildup;
};

#endif
