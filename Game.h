/*
 * Game.h
 *
 *  Created on: 7 Feb 2018
 *      Author: Zhivko Petrov
 */

#ifndef GAME_H_
#define GAME_H_

//C system headers

//C++ system headers
#include <cstdint>

//Other libraries headers

//Own components headers
#include "GameProxyInterface.hpp"
#include "DebugConsole.h"
#include "gameentities/GridContainer.h"
#include "pathfinding/MazeGenerator.h"

#include "sdl/Renderer.h"

//Forward declarations
union SDL_Event;


class Game : GameProxyInterface
{
    public:
        Game();
        virtual ~Game() = default;

        int32_t init(const bool isDiagonalMovementAllowed);

        void deinit();

        inline void start()
        {
            mainLoop();
        }

    private:
        void mainLoop();

        void updateWorldState(const int32_t elapsedTime);

        void drawWorld();

        bool handleUserEvent(SDL_Event & e);

        void evaluateAStar();

        virtual void onNodeChanged(const NodeType nodeType,
                                   const int32_t  nodeX,
                                   const int32_t  nodeY) override final;

        Renderer                  _renderer;

        MazeGenerator 			  _generator;

        DebugConsole              _debugConsole;

        GridContainer             _gridContainer;

        Text					  _noPathText;
};

#endif /* GAME_H_ */

