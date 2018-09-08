/*
 * Game.cpp
 *
 *  Created on: 7 Feb 2018
 *      Author: Zhivko Petrov
 */

//Corresponding header
#include "Game.h"

//C system headers
#include <unistd.h>

//C++ system headers
#include <cstdlib>
#include <cstdio>

//Other libraries headers
#include <SDL2/SDL_events.h>

//Own components headers
#include "common/CommonDefines.h"
#include "common/Time.h"
#include "common/Random.hpp"

#define MONITOR_WIDTH 1920
#define MONITOR_HEIGHT 1080


Game::Game()
{

}

int32_t Game::init(const bool isDiagonalMovementAllowed)
{
    int32_t err = EXIT_SUCCESS;

    //initialise monitor to FullHD
    if(EXIT_SUCCESS != _renderer.init(0, 0, MONITOR_WIDTH, MONITOR_HEIGHT))
    {
        fprintf(stderr, "Error in _renderer.init() \n");

        err = EXIT_FAILURE;
    }

    if(EXIT_SUCCESS == err)
    {
        if(EXIT_SUCCESS != _gridContainer.init(this,
                                               Textures::VERTICAL_LINE,
        									   Textures::HORIZONTAL_LINE,
											   Textures::START_NODE,
											   Textures::END_NODE,
											   Textures::A_STAR_PATH,
											   Textures::WALL))
        {
            fprintf(stderr, "Error in _gridLineContainer.init()\n");

            err = EXIT_FAILURE;
        }
    }

    if(EXIT_SUCCESS == err)
    {
        if(EXIT_SUCCESS !=  _debugConsole.init(_renderer.getTextureContainer()))
        {
            fprintf(stderr, "Error in _debugConsole.init()\n");

            err = EXIT_FAILURE;
        }
    }

    if(EXIT_SUCCESS == err)
    {
        const std::string text = "No possible path found";

        if(EXIT_SUCCESS != _noPathText.init(_renderer.getTextureContainer(),
                                            Textures::NO_AVAILABLE_PATH_TEXT,
                                            SDL_Point { 410, 460 },
                                            text.c_str(),
                                            FontSize::BIG))
        {
            fprintf(stderr, "Error in _winTimeoutText.init()\n");

            err = EXIT_FAILURE;
        }
    }

    if(EXIT_SUCCESS == err)
    {
        HeuristicFunction heuristic = Heuristic::manhattan;

        //euclidean heuristic is better for diagonal movements
        if(isDiagonalMovementAllowed)
        {
            heuristic = Heuristic::diagonal;
        }

        if(EXIT_SUCCESS != _generator.init(GRID_WIDTH,
        								   GRID_HEIGHT,
    									   isDiagonalMovementAllowed,
    									   heuristic))
        {
        	fprintf(stderr, "Error, _generator.init() failed\n");

        	err = EXIT_FAILURE;
        }
    }

    return err;
}

void Game::deinit()
{
    _renderer.deinit();
}

void Game::mainLoop()
{
    SDL_Event e;
    memset(&e, 0, sizeof(e));

    Time time;

    while(true)
    {
        if(handleUserEvent(e))
        {
        	break;
        }

        updateWorldState(time.getElapsed().toMicroseconds());

        drawWorld();
    }
}

void Game::updateWorldState(const int32_t elapsedTime)
{
    if(_debugConsole.isActive())
    {
        _debugConsole.update(elapsedTime);
    }
}

void Game::drawWorld()
{
    _renderer.clearScreen();

    _renderer.drawTextureArr(_gridContainer.gridLines, TOTAL_LINES_COUNT);

	for(int32_t i = 0; i < GRID_HEIGHT; ++i)
	{
		for(int32_t j = 0; j < GRID_WIDTH; ++j)
		{
			if(_gridContainer.isPathNodeActive[i][j])
			{
			    _renderer.drawTexture(&_gridContainer.pathNodes[i][j]);
			}
		}
	}

	if(_noPathText.isActive)
	{
	    _renderer.drawTexture(&_noPathText.drawParams);
	}

    if(_debugConsole.isActive())
    {
        _renderer.drawTexture(_debugConsole.getDrawParams());
    }

    _renderer.finishFrame();
}

bool Game::handleUserEvent(SDL_Event & e)
{
    if(0 != SDL_PollEvent(&e))
    {
        if((SDLK_ESCAPE == e.key.keysym.sym && e.type == SDL_KEYUP) ||
           (SDL_QUIT == e.type))
        {
            return true;
        }

        _debugConsole.handleEvent(e);
        _gridContainer.handleUserEvent(e);
    }

    return false;
}

void Game::onNodeChanged(const NodeType nodeType,
                         const int32_t  nodeX,
                         const int32_t  nodeY)
{
    switch(nodeType)
    {
        case NodeType::WALL_ADD:
            _generator.addCollision({nodeX, nodeY});

            _gridContainer.clearGridFromAStarPathNodes();
            _gridContainer.addCollision(nodeX, nodeY);
            break;

        case NodeType::WALL_REMOVE:
            _generator.removeCollision({nodeX, nodeY});

            _gridContainer.clearGridFromAStarPathNodes();
            _gridContainer.removeCollision(nodeX, nodeY);
            break;

        case NodeType::START_CHANGE:
            _generator.setStartNodePos({nodeX, nodeY});

            _gridContainer.clearGridFromAStarPathNodes();
            _gridContainer.addStartNode(nodeX, nodeY);
            break;

        case NodeType::END_CHANGE:
            _generator.setEndNodePos({nodeX, nodeY});

            _gridContainer.clearGridFromAStarPathNodes();
            _gridContainer.addEndNode(nodeX, nodeY);
            break;

        default:
            break;
    }

    evaluateAStar();
}

void Game::evaluateAStar()
{
    // This method returns vector of coordinates from target to source.
    const std::vector<Point> path = _generator.findPath();

    if(path.empty())
    {
        return;
    }

    if((path.front() != _generator.getEndNodePos()))
    {
        _noPathText.isActive = true;
    }
    else
    {
        _noPathText.isActive = false;

        const size_t SIZE = path.size();

        //do not print the first and the last node as part of the path, because
        //they mark the start and finish
        for(size_t i = 1; i < SIZE - 1; ++i)
        {
            _gridContainer.addAStarPathNode(path[i].x, path[i].y);
        }
    }
}





