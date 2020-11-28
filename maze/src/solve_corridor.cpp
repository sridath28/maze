#include <a_star.h>
#include <maze.h>

using namespace std;
using namespace ecn;

// a node is a x-y position, we move from 1 each time
class Position : public Point
{
    typedef std::unique_ptr<Position> PositionPtr;
    enum direction {up, down, left, right};

private:
    int m_distance;

public:
    // constructor from coordinates
    Position(int _x, int _y, int _distance = 1) : Point(_x, _y), m_distance(_distance) {}

    // constructor from base ecn::Point
    Position(ecn::Point p) : Point(p.x, p.y) {}

    int distToParent()
    {
        // in line-based motion, distance is the number of cell we could travel without hitting a wall
        return m_distance;
    }

    bool isLine(direction dir, int x, int y) {

        switch(dir) {
            case(up):
                x--;
                break;
            case(down): 
                x++;
                break;
            case(right):
                y++;
                break;
            case(left):
                y--;
                break;
            }
        if (maze.isFree(x, y)) return true;
        else return false;
    }

    std::vector<PositionPtr> children()
    {
        // this method should return  all positions reachable from this one
        std::vector<PositionPtr> generated;

        // Create new variables for Position, to prevent modifying the current node
        int new_x;
        int new_y;
        int distance;
        
        // Loop through four DIRECTIONS and loop until a wall is met in each direction
        for (int i = 0 ; i < 4 ; i++) {
            switch(i) {
                case(0):
                    distance = 1;
                    new_x = x + distance;
                    new_y = y;
                    if(!maze.isFree(new_x, new_y)) break;
                    while (maze.isFree(new_x, new_y)) {
                        new_x = new_x + 1;
                        distance++;
                    }
                    generated.push_back(std::make_unique<Position>(x + 1, y, distance - 1));
                    break;
                case(1):
                    distance = 1;
                    new_x = x - distance;
                    new_y = y;
                    if(!maze.isFree(new_x, new_y)) break;
                    while (maze.isFree(new_x, new_y)) {
                        new_x = new_x - 1;
                        distance++;
                    }
                    generated.push_back(std::make_unique<Position>(x - 1, y, distance - 1));
                    break;
                case(2):
                    distance = 1;
                    new_x = x;
                    new_y = y + distance;
                    if(!maze.isFree(new_x, new_y)) break;
                    while (maze.isFree(new_x, new_y)) {
                        new_y = new_y + 1;
                        distance++;
                    }
                    generated.push_back(std::make_unique<Position>(x, y + 1, distance - 1));
                    break;
                case(3):
                    distance = 1;
                    new_x = x;
                    new_y = y - distance;
                    if(!maze.isFree(new_x, new_y)) break;
                    while (maze.isFree(new_x, new_y)) {
                        new_y = new_y - 1;
                        distance++;
                    }
                    generated.push_back(std::make_unique<Position>(x, y - 1, distance - 1));
                    break;
            }
        }
        return generated;
    }
};


int main( int argc, char **argv )
{
    // load file
    std::string filename = "maze.png";
    if(argc == 2)
        filename = std::string(argv[1]);

    // let Point know about this maze
    Position::maze.load(filename);

    // initial and goal positions as Position's
    Position start = Position::maze.start(),
             goal = Position::maze.end();

    // call A* algorithm
    ecn::Astar(start, goal);

    // save final image
    Position::maze.saveSolution("corridor");
    cv::waitKey(0);

}
