#include "gamemanager.h"

gameManager::gameManager()
{

}

bool gameManager::isLevelCompleted()
{
    return (user.getX() == std::get<0>(level.getEnd()) &&
            user.getY() == std::get<1>(level.getEnd()));
}
