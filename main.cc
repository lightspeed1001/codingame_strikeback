#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <cmath>

using namespace std;


struct Vector2D
{
    public:
    Vector2D() : x(0), y(0)
    {}

    Vector2D(float x, float y) : x(x), y(y)
    {}

    float x, y;

    const float Dot(const Vector2D other) const
    {
        return x * other.x + y * other.y;
    }

    float Size() const
    {
        return sqrt(Dot(*this));
    }

    Vector2D Normalized() const
    {
        float s = Size();
        
        return *this / s;
    }

    Vector2D operator +(const Vector2D& other) const
	{ 
        return Vector2D(x + other.x, y + other.y); 
    }

    Vector2D operator -(const Vector2D& other) const
	{ 
        return Vector2D(x - other.x, y - other.y); 
    }

    bool operator ==(const Vector2D& other) const
    {
        return x == other.x && y == other.y;
    }

    bool operator !=(const Vector2D& other) const
    {
        return !(*this == other);
    }

    Vector2D operator *(const float s) const
    {
        return Vector2D(x * s, y * s); 
    }

    Vector2D operator /(const float s) const 
    {
    	float r = 1.0f / s;
	    return *this * r;
    }
};

class BoostManager
{
    public:
    BoostManager() : _boostsAvailable(1), _foundAllCheckpoints(false), _maxDistance(0)
    {

    }

    bool ShouldBoost(float dist)
    {
        // Need to have a boost available and have found everything
        if(_boostsAvailable <= 0 || !_foundAllCheckpoints) return false;
        
        if(dist + DISTANCE_ERROR_MARGIN > _maxDistance)
        {
            _boostsAvailable--;
            return true;
        }

        return false;
    }
    
    void AddCheckpoint(Vector2D c, int dist)
    {
        // Already found everything, abort
        if(_foundAllCheckpoints) return;
        
        // First checkpoint, just add it
        if(checkpoints.empty())
        {
            checkpoints.push_back(c);
        }
        else if(checkpoints.back() != c)
        {
            // New checkpoint, add it to the list
            checkpoints.push_back(c);

            // We've looped back, found everything
            if(c == checkpoints.front())
                _foundAllCheckpoints = true;
        }

        if(dist > _maxDistance) _maxDistance = dist;
    }

    bool HasBoost()
    {
        return _boostsAvailable > 0;
    }

    private:
    vector<Vector2D> checkpoints;
    bool _foundAllCheckpoints = false;
    float _maxDistance = 0;
    int _boostsAvailable = 1;

    const float DISTANCE_ERROR_MARGIN = 2000;
};



/**
 * Auto-generated code below aims at helping you parse
 * the standard input according to the problem statement.
 **/

int main()
{
    int STOP_ANGLE = 90;
    int STEERING_ANGLE = 1;
    int MAX_THRUST = 100;
    int MIN_THRUST = 0;
    int CHECKPOINT_RADIUS = 600;
    int CHECKPOINT_SLOW_RADIUS = CHECKPOINT_RADIUS * 3;

    int boostsAvailable = 1;

    bool initialPosition = true;
    Vector2D oldPosition(0,0);

    BoostManager bm;
    // game loop
    while (1) {
        int x;
        int y;
        int nextCheckpointX; // x position of the next check point
        int nextCheckpointY; // y position of the next check point
        int nextCheckpointDist; // distance to the next checkpoint
        int nextCheckpointAngle; // angle between your pod orientation and the direction of the next checkpoint
        cin >> x >> y >> nextCheckpointX >> nextCheckpointY >> nextCheckpointDist >> nextCheckpointAngle; cin.ignore();
        int opponentX;
        int opponentY;
        bool shouldBoost = false;
        cin >> opponentX >> opponentY; cin.ignore();
        
        Vector2D currentPosition(x, y);
        if(initialPosition)
        {
            oldPosition = Vector2D(x, y);
            initialPosition = false;
        }

        Vector2D nextPoint(nextCheckpointX, nextCheckpointY);
        bm.AddCheckpoint(nextPoint, nextCheckpointDist);

        // Write an action using cout. DON'T FORGET THE "<< endl"
        // To debug: cerr << "Debug messages..." << endl;
        // You have to output the target position
        // followed by the power (0 <= thrust <= 100)
        // i.e.: "x y thrust"

        int absAngle = abs(nextCheckpointAngle);
        int thrust = MAX_THRUST;
        
        if(absAngle >= STOP_ANGLE)
        {
            // Stop to turn around
            thrust = 0;
        }
        else if(absAngle >= STEERING_ANGLE)
        {
            // Apply some steering/overcorrections to combat drifting
            // https://gamedevelopment.tutsplus.com/tutorials/understanding-steering-behaviors-seek--gamedev-849
            Vector2D desiredDirection(nextCheckpointX - x, nextCheckpointY - y);
            desiredDirection = desiredDirection.Normalized();

            Vector2D currentDirection = currentPosition - oldPosition;
            currentDirection = currentDirection.Normalized();

            Vector2D steeringDirection = desiredDirection - currentDirection;
            steeringDirection = steeringDirection.Normalized() * MAX_THRUST;

            nextCheckpointX += steeringDirection.x;
            nextCheckpointY += steeringDirection.y;

            // Move slower when trying to turn inside checkpoint
            if(nextCheckpointDist < CHECKPOINT_SLOW_RADIUS)
                thrust *= (STOP_ANGLE - absAngle) / float(STOP_ANGLE);
        }
        else if(bm.ShouldBoost(nextCheckpointDist))
        {
            // Boost on the optimal time
            shouldBoost = true;
        }
        else if(nextCheckpointDist <= CHECKPOINT_SLOW_RADIUS)
        {
            // Slow down when approaching checkpoints
            thrust *= clamp((float)nextCheckpointDist/(float)CHECKPOINT_SLOW_RADIUS, 0.f, 1.f);
        }

        cout << nextCheckpointX << " " << nextCheckpointY << " " ;
        if(shouldBoost)
        {
            cout << "BOOST" << endl;
        }
        else
        {
            cout << thrust << endl;
        }

        oldPosition = currentPosition;
    }
}
