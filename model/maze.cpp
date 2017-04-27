#include "maze.h"

Position Room::position() const
{
    return pos_;
}

size_t Room::row() const
{
    return pos_.row;
}

size_t Room::col() const
{
    return pos_.col;
}

void Room::addAdjRoom(Direction direction, Room* room)
{
    adjRooms_.emplace_back(AdjRoom{direction, room});
}

void Room::removeAdjRoom(Room* room)
{
    adjRooms_.erase(std::remove_if(adjRooms_.begin(), adjRooms_.end(),
                              [room](const auto& adjRoom) { return adjRoom.room == room; }),
               adjRooms_.end());
}

void Room::removeAdjRoom(Direction direction)
{
    adjRooms_.erase(std::remove_if(adjRooms_.begin(), adjRooms_.end(),
                              [direction](const auto& adjRoom) { return adjRoom.direction == direction; }),
                    adjRooms_.end());
}

void Room::addAdjDoor(Door* door)
{
    adjDoors_.push_back(door);
}

range<Room::AdjRoomIterator> Room::adjRooms()
{
    return make_range(adjRooms_.begin(), adjRooms_.end());
}

Room *Room::adjRoom(Direction direction)
{
    auto it = std::find_if(adjRooms_.begin(), adjRooms_.end(),
                              [direction](const auto& adjRoom) { return adjRoom.direction == direction; });
    if (it == adjRooms_.end())
        throw std::logic_error{"Adjacent room not found with this direction"};

    return it->room;
}

range<Room::AdjDoorIterator> Room::adjDoors()
{
    return make_range(adjDoors_.begin(), adjDoors_.end());
}

void Room::mark()
{
    marked_ = true;
    notifyMarked();
}

void Room::unMark()
{
    marked_ = false;
    notifyUnMarked();
}

bool Room::isMarked() const
{
    return marked_;
}

void Room::setModificationListener(Room::ModificationListener *listener)
{
    listener_ = listener;
}

void Room::notifyMarked()
{
    if (listener_ != nullptr)
        listener_->marked();
}

void Room::notifyUnMarked()
{
    if (listener_ != nullptr)
        listener_->unmarked();
}

Room *Door::oppositeRoom(const Room* other)
{
    return other == firstRoom_ ? secondRoom_ : firstRoom_;
}

Direction Door::oppositeDirection(const Room* room) const
{
    if (room == firstRoom_)
        return direction_;
    else
        return opposite(direction_);
}

std::pair<Room *, Room *> Door::rooms()
{
    return std::make_pair(firstRoom_, secondRoom_);
}

void Door::close()
{
    firstRoom_->removeAdjRoom(secondRoom_);
    secondRoom_->removeAdjRoom(firstRoom_);
    closed_ = true;
}

void Door::open()
{
    firstRoom_->addAdjRoom(direction_, secondRoom_);
    secondRoom_->addAdjRoom(opposite(direction_), firstRoom_);
    closed_ = false;
    notifyOpened();
}

bool Door::isClosed()
{
    return closed_;
}

Direction Door::direction()
{
    return direction_;
}

void Door::setModificationListener(Door::ModificationListener *listener)
{
    listener_ = listener;
}

void Door::notifyOpened()
{
    if (listener_ != nullptr)
        listener_->opened();
}

Maze::Maze(size_t nrow, size_t ncol)
    : nrow_{nrow}, ncol_{ncol}
{
    std::vector<std::vector<Room*>> M(2);

    M[0] = makeRow(0, ncol);

    setStart(M[0][0]);

    for (size_t i = 1; i < nrow; ++i) {
        auto r1 = (i-1) % 2;
        auto r2 = i % 2;
        M[r2] = makeRow(i, ncol);
        for (size_t j = 0; j < ncol; ++j) {
            auto door = std::make_unique<Door>(M[r1][j], M[r2][j], Direction::South);
            addDoor(std::move(door));
        }
    }

    setEnd(M[(nrow-1) % 2][ncol-1]);
}

std::vector<Room*> Maze::makeRow(size_t row, size_t ncol)
{
    std::vector<Room*> r(ncol);

    r[0] = new Room(row, 0);
    addRoom(std::unique_ptr<Room>(r[0]));
    for (size_t i = 1; i < ncol; ++i) {
        r[i] = new Room(row, i);
        addRoom(std::unique_ptr<Room>(r[i]));
        auto door = std::make_unique<Door>(r[i-1], r[i], Direction::East);
        addDoor(std::move(door));
    }

    return r;
}

void Maze::addDoor(std::unique_ptr<Door> door)
{
    doors_.emplace_back(std::move(door));
}

void Maze::addRoom(std::unique_ptr<Room> room)
{
    rooms_.emplace_back(std::move(room));
}

void Maze::setStart(Room* start)
{
    start_ = start;
}

Room *Maze::getStart()
{
    return start_;
}

void Maze::setEnd(Room* end)
{
    end_ = end;
}

Room *Maze::getEnd()
{
    return end_;
}

size_t Maze::nrow()
{
    return nrow_;
}

size_t Maze::ncol()
{
    return ncol_;
}

range<Maze::RoomIterator> Maze::rooms()
{
    return make_range(rooms_.begin(), rooms_.end());
}

range<Maze::DoorIterator> Maze::doors()
{
    return make_range(doors_.begin(), doors_.end());
}

void Maze::setModificationListener(Maze::ModificationListener *listener)
{
    listener_ = listener;
}

Direction opposite(Direction direction)
{
    switch (direction) {
    case Direction::East:
        return Direction::West;
    case Direction::West:
        return Direction::East;
    case Direction::North:
        return Direction::South;
    case Direction::South:
        return Direction::North;
    case Direction::Stop:
        return Direction::Stop;
    }

    throw std::logic_error{"Opposite direction unknown"};
}

bool operator==(const Position& x, const Position& y)
{
    return (x.col == y.col) && (x.row == y.row);
}

bool operator!=(const Position& x, const Position& y)
{
    return !(x == y);
}

void closeDoors(Maze *maze)
{
    for (auto& door: maze->doors())
        door->close();
}

void unmarkRooms(Maze *maze)
{
    for (auto& room: maze->rooms())
        room->unMark();
}

void resetDoorsRooms(Maze *maze)
{
    closeDoors(maze);
    unmarkRooms(maze);
}
