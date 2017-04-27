#ifndef MAZE_H
#define MAZE_H

#include <memory>
#include <vector>
#include <unordered_map>
#include <algorithm>

#include "utility.h"

enum class Direction : size_t { North, South, East, West, Stop };

Direction opposite(Direction direction);

class Door;
class Room;

class RoomIterator;

class Maze {
public:
    Maze(size_t nrow, size_t ncol);

    Room* getStart();
    Room* getEnd();

    size_t nrow();
    size_t ncol();

    using RoomContainer = std::vector<std::unique_ptr<Room>>;
    using RoomIterator = RoomContainer::iterator;

    range<RoomIterator> rooms();

    using DoorContainer = std::vector<std::unique_ptr<Door>>;
    using DoorIterator = DoorContainer::iterator;

    range<DoorIterator> doors();

    class ModificationListener;
    void setModificationListener(ModificationListener* listener);

private:
    std::vector<Room*> makeRow(size_t row, size_t ncol);
    void addDoor(std::unique_ptr<Door>);
    void addRoom(std::unique_ptr<Room>);

    void setStart(Room*);
    void setEnd(Room*);

    RoomContainer rooms_;
    DoorContainer doors_;

    Room* start_;
    Room* end_;

    ModificationListener* listener_ = nullptr;

    size_t nrow_;
    size_t ncol_;
};

void closeDoors(Maze* maze);
void unmarkRooms(Maze* maze);
void resetDoorsRooms(Maze* maze);

struct Position {
    size_t row;
    size_t col;
};

bool operator==(const Position&, const Position&);
bool operator!=(const Position&, const Position&);

struct AdjRoom {
    Direction direction;
    Room* room;
};

class Room {
public:
    Room(size_t row, size_t col)
        : pos_{row, col}
    { }

    Position position() const;
    size_t row() const;
    size_t col() const;

    void mark();
    void unMark();
    bool isMarked() const;

    void addAdjRoom(Direction, Room*);
    void removeAdjRoom(Room*);
    void removeAdjRoom(Direction);

    void addAdjDoor(Door*);

    using AdjRoomContainer = std::vector<AdjRoom>;
    using AdjRoomIterator = AdjRoomContainer::iterator;

    range<AdjRoomIterator> adjRooms();
    Room* adjRoom(Direction);

    using AdjDoorContainer = std::vector<Door*>;
    using AdjDoorIterator = AdjDoorContainer::iterator;

    range<AdjDoorIterator> adjDoors();

    class ModificationListener;
    void setModificationListener(ModificationListener* listener);

private:
    void notifyMarked();
    void notifyUnMarked();

    AdjRoomContainer adjRooms_;
    AdjDoorContainer adjDoors_;

    Position pos_;

    bool marked_ = false;

    ModificationListener* listener_ = nullptr;
};

class LockedRoom {
public:
    LockedRoom(Room* room): room_(room) {}
    Position position() const { return room_->position(); }
    void mark() { room_->mark(); }
    void unMark() { room_->unMark(); }
    bool isMarked() const { return room_->isMarked(); }
private:
    Room* room_;
};

class AdjLockedRoom {
public:
    AdjLockedRoom(const Room* room, Direction direction): room_(room), direction_(direction) {}
    Position position() const { return room_->position(); }
    Direction direction() const { return direction_; }
    bool isMarked() const { return room_->isMarked(); }
private:
    const Room* room_;
    Direction direction_;
};

class Door {
public:
    Door(Room* first, Room* second, Direction direction)
        : firstRoom_(first),
          secondRoom_(second),
          direction_(direction)
    {
        first->addAdjDoor(this);
        second->addAdjDoor(this);
    }

    Room* oppositeRoom(const Room*);
    Direction oppositeDirection(const Room*) const;

    std::pair<Room*, Room*> rooms();

    void close();
    void open();
    bool isClosed();

    Direction direction();

    class ModificationListener;
    void setModificationListener(ModificationListener* listener);

private:
    void notifyOpened();

    Room* firstRoom_;
    Room* secondRoom_;
    Direction direction_;
    bool closed_ = true;

    ModificationListener* listener_ = nullptr;
};

class SquareMazeFactory {
public:
    virtual Maze* makeMaze(size_t nrow, size_t ncol);
    virtual Door* makeDoor(Room* first, Room* second, Direction direction) = 0;
    virtual Room* makeRoom(size_t row, size_t col) = 0;

    virtual ~SquareMazeFactory() = default;

private:
    std::vector<Room*> makeRow(Maze* maze, size_t row, size_t ncol);
};

class Maze::ModificationListener {
public:
    virtual void roomAdded(Room* room) = 0;
    virtual void doorAdded(Door* door) = 0;
    virtual ~ModificationListener() = default;
};

class Room::ModificationListener {
public:
    virtual void marked() = 0;
    virtual void unmarked() = 0;
    virtual ~ModificationListener() = default;
};

class Door::ModificationListener {
public:
    virtual void opened() = 0;
    virtual void closed() = 0;
    virtual ~ModificationListener() = default;
};

#endif // MAZE_H
