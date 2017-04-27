#ifndef MAZESOLVER_H
#define MAZESOLVER_H

#include "maze.h"

#include <stack>
#include <memory>
#include <vector>

class MazeSolver
{
public:
    class Program;
    struct ProgramFactory;

    MazeSolver(Room* startRoom, Room* targetRoom);

    void step();
    void start(std::unique_ptr<Program>);
    void reset();

    bool isFinished();

    Position position();
    Room* currentRoom();

    class Observer;
    void setMovementOberserver(Observer* observer);

private:
    void setCurrentRoom(Room*);

    void notifyMoved();

    Room* const startRoom_;
    Room* const targetRoom_;
    Room* currentRoom_;

    std::unique_ptr<Program> program_;

    Observer* observer_ = nullptr;

    bool finished_ = true;
};

class MazeSolver::Observer {
public:
    virtual void moved() = 0;
    virtual ~Observer() = default;
};


class MazeSolver::Program {
public:
    virtual Direction move(LockedRoom currentRoom, std::vector<AdjLockedRoom> adjRooms, const Position& targetPosition) = 0;
    virtual ~Program() = default;
};

class StopProgram : public MazeSolver::Program {
public:
    Direction move(LockedRoom currentRoom, std::vector<AdjLockedRoom> adjRooms, const Position& targetPosition) override;
};

struct MazeSolver::ProgramFactory {
    virtual std::unique_ptr<MazeSolver::Program> makeProgram() const = 0;
    virtual ~ProgramFactory() = default;
};

template <typename ProgramT>
struct ConcreteProgramFactory: MazeSolver::ProgramFactory {
    std::unique_ptr<MazeSolver::Program> makeProgram() const override {
        return std::make_unique<ProgramT>();
    }
};

#endif // MAZESOLVER_H
