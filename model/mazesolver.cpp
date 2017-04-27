#include "mazesolver.h"
#include "algorithm.h"

#include <iterator>

MazeSolver::MazeSolver(Room *startRoom, Room *targetRoom)
    : startRoom_(startRoom),
      targetRoom_(targetRoom),
      currentRoom_(startRoom),
      program_(std::make_unique<StopProgram>())
{
}

void MazeSolver::step()
{
    if (isFinished())
        return;

    std::vector<AdjLockedRoom> adjRooms;
    std::transform(currentRoom_->adjRooms().begin(),
                   currentRoom_->adjRooms().end(),
                   std::back_inserter(adjRooms),
                   [] (AdjRoom& room) { return AdjLockedRoom{room.room, room.direction}; });

    auto direction = program_->move(LockedRoom{currentRoom_}, std::move(adjRooms), targetRoom_->position());
    if (direction == Direction::Stop) {
        finished_ = true;
        return;
    }

    setCurrentRoom(currentRoom_->adjRoom(direction));
}

Position MazeSolver::position()
{
    return currentRoom_->position();
}

Room *MazeSolver::currentRoom()
{
    return currentRoom_;
}

void MazeSolver::setMovementOberserver(MazeSolver::Observer *observer)
{
    observer_ = observer;
}

void MazeSolver::setCurrentRoom(Room* room)
{
    currentRoom_ = room;
    notifyMoved();
}

void MazeSolver::notifyMoved()
{
    if (observer_ != nullptr)
        observer_->moved();
}

void MazeSolver::start(std::unique_ptr<MazeSolver::Program> program)
{
    finished_ = false;
    program_ = std::move(program);
    setCurrentRoom(startRoom_);
}

void MazeSolver::reset()
{
    finished_ = false;
    program_ = std::make_unique<StopProgram>();
    setCurrentRoom(startRoom_);
}

bool MazeSolver::isFinished()
{
    return finished_;
}

Direction StopProgram::move(LockedRoom, std::vector<AdjLockedRoom>, const Position&)
{
    return Direction::Stop;
}
