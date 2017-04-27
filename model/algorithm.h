#ifndef ALGORITHM_H
#define ALGORITHM_H

class Maze;
class Room;
class Door;

#include "mazesolver.h"
#include "mazegenerator.h"

class DFSMazeGenerator : public MazeGenerator {
public:
    DFSMazeGenerator(Maze* maze);

    bool isFinished() override;

    void step() override;
};

class KruskallMazeGenerator : public MazeGenerator {
public:
    KruskallMazeGenerator(Maze* maze);

    bool isFinished() override;

    void step() override;
};

class OpenAllMazeGenerator : public MazeGenerator {
    Maze::DoorIterator nextDoorIt_;
public:
    OpenAllMazeGenerator(Maze* maze);

    bool isFinished() override;

    void step() override;
};

class DFSProgram : public MazeSolver::Program {
public:
    Direction move(LockedRoom currentRoom, std::vector<AdjLockedRoom> adjRooms, const Position& targetPosition) override;
};

class CustomProgram : public MazeSolver::Program {
public:
    Direction move(LockedRoom currentRoom, std::vector<AdjLockedRoom> adjRooms, const Position& targetPosition) override;
};

class RandomProgram : public MazeSolver::Program {
public:
    Direction move(LockedRoom currentRoom, std::vector<AdjLockedRoom> adjRooms, const Position& targetPosition) override;
};

#endif // ALGORITHM_H
