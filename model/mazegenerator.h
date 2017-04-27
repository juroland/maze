#ifndef MAZEGENERATOR_H
#define MAZEGENERATOR_H

#include <memory>

class Maze;

class MazeGenerator {
public:
    MazeGenerator(Maze* maze);
    virtual bool isFinished() = 0;
    virtual void step() = 0;
    virtual ~MazeGenerator() = default;

protected:
    Maze* maze_;
};

struct MazeGeneratorFactory {
    virtual std::unique_ptr<MazeGenerator> makeGenerator(Maze* maze) const = 0;
    virtual ~MazeGeneratorFactory() = default;
};

template <typename GeneratorT>
struct ConcreteMazeGeneratorFactory : MazeGeneratorFactory {
    std::unique_ptr<MazeGenerator> makeGenerator(Maze* maze) const override{
        return std::make_unique<GeneratorT>(maze);
    }
};

#endif // MAZEGENERATOR_H
