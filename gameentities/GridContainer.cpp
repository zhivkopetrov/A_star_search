/*
 * StaticImage.cpp
 *
 *  Created on: 8 Feb 2018
 *      Author: Zhivko Petrov
 */

//Corresponding header
#include "GridContainer.h"

//C system headers

//C++ system headers
#include <cstdlib>

//Other libraries headers
#include <SDL2/SDL_events.h>

//Own components headers
#include "GameProxyInterface.hpp"
#include "common/CommonStructs.hpp"

#define TILE_DIMENSION 55


GridContainer::GridContainer() : _gameInterface(nullptr),
                                 _pathNodeRsrcId(0),
				                 _wallNodeRsrcId(0),
				                 _startNodeRsrcId(0),
				                 _endNodeRsrcId(0)
{

}

int32_t GridContainer::init(GameProxyInterface * gameInterface,
                            const uint8_t vericalLineRsrcId,
	     	 	 	 	    const uint8_t horizontalLineRsrcId,
							const uint8_t startNodeRsrcId,
						    const uint8_t endNodeRsrcId,
					    	const uint8_t pathNodeRsrcId,
				     		const uint8_t wallNodeRsrcId)
{
    _gameInterface   = gameInterface;
	_pathNodeRsrcId  = pathNodeRsrcId;
	_wallNodeRsrcId  = wallNodeRsrcId;
	_startNodeRsrcId = startNodeRsrcId;
	_endNodeRsrcId   = endNodeRsrcId;

	for(int32_t i = 0; i < HORIZONTAL_LINE_COUNT; ++i)
	{
		gridLines[i].rsrcId = horizontalLineRsrcId;
		gridLines[i].frame = 0;
		gridLines[i].pos.x = 0;
		gridLines[i].pos.y = i * LINE_OFFSET;
	}

	for(int32_t i = 0; i < VERTICAL_LINE_COUNT; ++i)
	{
		gridLines[i + HORIZONTAL_LINE_COUNT].rsrcId = vericalLineRsrcId;
		gridLines[i + HORIZONTAL_LINE_COUNT].frame = 0;
		gridLines[i + HORIZONTAL_LINE_COUNT].pos.x = i * LINE_OFFSET;
		gridLines[i + HORIZONTAL_LINE_COUNT].pos.y = 0;
	}

	for(int32_t i = 0; i < GRID_HEIGHT; ++i)
	{
		for(int32_t j = 0; j < GRID_WIDTH; ++j)
		{
			pathNodes[i][j].frame = 0;
			pathNodes[i][j].pos.x = (j * LINE_OFFSET) + 5;
			pathNodes[i][j].pos.y = (i * LINE_OFFSET) + 5;
		}
	}

	clearGrid();

    return EXIT_SUCCESS;
}

void GridContainer::handleUserEvent(SDL_Event & e)
{
    if(e.type == SDL_KEYUP)
    {
        switch(e.key.keysym.sym)
        {
            case SDLK_s:
                onStartNodeEntered();
                break;

            case SDLK_w:
                onWallAdd();
                break;

            case SDLK_r:
                onWallRemove();
                break;

            case SDLK_e:
                onEndNodeEntered();
                break;

            default:
                break;
        }
    }
    else if(e.type == SDL_MOUSEBUTTONDOWN)
    {
        if(e.button.button == SDL_BUTTON_LEFT)
        {
            onWallAdd();
        }
        else if(e.button.button == SDL_BUTTON_RIGHT)
        {
            onWallRemove();
        }
    }
}

void GridContainer::clearGrid()
{
    for(int32_t i = 0; i < GRID_HEIGHT; ++i)
    {
        for(int32_t j = 0; j < GRID_WIDTH; ++j)
        {
            isPathNodeActive[i][j] = false;
        }
    }
}

void GridContainer::clearGridFromAStarPathNodes()
{
    for(int32_t i = 0; i < GRID_HEIGHT; ++i)
    {
        for(int32_t j = 0; j < GRID_WIDTH; ++j)
        {
            if(_pathNodeRsrcId == pathNodes[i][j].rsrcId)
            {
                isPathNodeActive[i][j] = false;
            }
        }
    }
}

void GridContainer::addAStarPathNode(const int32_t nodeX, const int32_t nodeY)
{
    pathNodes[nodeY][nodeX].rsrcId = _pathNodeRsrcId;
    isPathNodeActive[nodeY][nodeX] = true;
}

void GridContainer::addCollision(const int32_t nodeX, const int32_t nodeY)
{
    pathNodes[nodeY][nodeX].rsrcId = _wallNodeRsrcId;
    isPathNodeActive[nodeY][nodeX] = true;
}

void GridContainer::removeCollision(const int32_t nodeX, const int32_t nodeY)
{
    isPathNodeActive[nodeY][nodeX] = false;
}

void GridContainer::addStartNode(const int32_t nodeX, const int32_t nodeY)
{
    //first clear the old start node, if any
    for(int32_t i = 0; i < GRID_HEIGHT; ++i)
    {
        for(int32_t j = 0; j < GRID_WIDTH; ++j)
        {
            if(_startNodeRsrcId == pathNodes[i][j].rsrcId)
            {
                isPathNodeActive[i][j] = false;
            }
        }
    }

    pathNodes[nodeY][nodeX].rsrcId = _startNodeRsrcId;
    isPathNodeActive[nodeY][nodeX] = true;
}

void GridContainer::addEndNode(const int32_t nodeX, const int32_t nodeY)
{
    //first clear the old start node, if any
    for(int32_t i = 0; i < GRID_HEIGHT; ++i)
    {
        for(int32_t j = 0; j < GRID_WIDTH; ++j)
        {
            if(_endNodeRsrcId == pathNodes[i][j].rsrcId)
            {
                isPathNodeActive[i][j] = false;
            }
        }
    }

    pathNodes[nodeY][nodeX].rsrcId = _endNodeRsrcId;
    isPathNodeActive[nodeY][nodeX] = true;
}

void GridContainer::onWallAdd()
{
    int32_t nodeX = 0;
    int32_t nodeY = 0;

    if(getSelectedNode(&nodeX, &nodeY))
    {
        addCollision(nodeX, nodeY);
        _gameInterface->onNodeChanged(NodeType::WALL_ADD, nodeX, nodeY);
    }
}

void GridContainer::onWallRemove()
{
    int32_t nodeX = 0;
    int32_t nodeY = 0;

    if(getSelectedNode(&nodeX, &nodeY))
    {
        removeCollision(nodeX, nodeY);
        _gameInterface->onNodeChanged(NodeType::WALL_REMOVE, nodeX, nodeY);
    }
}

void GridContainer::onStartNodeEntered()
{
    int32_t nodeX = 0;
    int32_t nodeY = 0;

    if(getSelectedNode(&nodeX, &nodeY))
    {
        _gameInterface->onNodeChanged(NodeType::START_CHANGE, nodeX, nodeY);
    }
}

void GridContainer::onEndNodeEntered()
{
    int32_t nodeX = 0;
    int32_t nodeY = 0;

    if(getSelectedNode(&nodeX, &nodeY))
    {
        _gameInterface->onNodeChanged(NodeType::END_CHANGE, nodeX, nodeY);
    }
}

bool GridContainer::getSelectedNode(int32_t * nodeX, int32_t * nodeY)
{
    SDL_Point mousePos;
    /* capture mouse position on the screen */
    SDL_GetMouseState(&mousePos.x, &mousePos.y);

    SDL_Rect currBoundaryRect;
    currBoundaryRect.w = TILE_DIMENSION;
    currBoundaryRect.h = TILE_DIMENSION;

    for(int32_t i = 0; i < GRID_HEIGHT; ++i)
    {
        for(int32_t j = 0; j < GRID_WIDTH; ++j)
        {
            currBoundaryRect.x = pathNodes[i][j].pos.x;
            currBoundaryRect.y = pathNodes[i][j].pos.y;

            if(SDL_PointInRect(&mousePos, &currBoundaryRect))
            {
                *nodeX = j;
                *nodeY = i;

                return true;
            }
        }
    }

    return false;
}





