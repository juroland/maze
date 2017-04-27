#include "random.h"

#include "algorithm.h"
#include "maze.h"

DFSMazeGenerator::DFSMazeGenerator(Maze* maze)
    : MazeGenerator(maze)
{

}

bool DFSMazeGenerator::isFinished()
{
    return true;
}

void DFSMazeGenerator::step()
{
}

KruskallMazeGenerator::KruskallMazeGenerator(Maze* maze)
    : MazeGenerator(maze)
{
}

bool KruskallMazeGenerator::isFinished()
{
    return true;
}

void KruskallMazeGenerator::step()
{
}

Direction DFSProgram::move(LockedRoom currentRoom, std::vector<AdjLockedRoom> adjRooms, const Position& target)
{
    return Direction::Stop;
}

Direction CustomProgram::move(LockedRoom currentRoom, std::vector<AdjLockedRoom> adjRooms, const Position& target)
{
    return Direction::Stop;
}

Direction RandomProgram::move(LockedRoom currentRoom, std::vector<AdjLockedRoom> adjRooms, const Position& target)
{
    if (target == currentRoom.position() || adjRooms.size() == 0)
        return Direction::Stop;

    currentRoom.mark();
    auto direction = adjRooms[pick_an_integer<size_t>(0, adjRooms.size()-1)].direction();
    return direction;
}

OpenAllMazeGenerator::OpenAllMazeGenerator(Maze *maze)
    : MazeGenerator(maze), nextDoorIt_(maze->doors().begin())
{ }

bool OpenAllMazeGenerator::isFinished()
{
    return nextDoorIt_ == maze_->doors().end();
}

void OpenAllMazeGenerator::step()
{
    (*nextDoorIt_)->open();
    ++nextDoorIt_;
}
