#include <iostream>
#include <ctime>

using namespace std;

class Stopwatch {
    int seconds;
    bool paused;
    mutable int queryCount;

    static int stopwatchCount;
    static int activeStopwatches;

    time_t lastUpdate;

public:
    explicit Stopwatch(int sec) : seconds(sec), paused(false), queryCount(0) {
        lastUpdate = time(nullptr);
        stopwatchCount++;
        activeStopwatches++;
    }

    explicit Stopwatch(double min) : seconds(static_cast<int>(min * 60)), paused(false), queryCount(0) {
        lastUpdate = time(nullptr);
        stopwatchCount++;
        activeStopwatches++;
    }

    ~Stopwatch() {
        stopwatchCount--;
        if (!paused) {
            activeStopwatches--;
        }
    }

    void Tick() {
        if (!paused) {
            time_t now = time(nullptr);
            seconds += static_cast<int>(now - lastUpdate);
            lastUpdate = now;
        }
    }

    void pause(bool pauseState = true) {
        if (paused != pauseState) {
            paused = pauseState;

            if (paused) {
                activeStopwatches--;
            }
            else {
                lastUpdate = time(nullptr);
                activeStopwatches++;
            }
        }
    }

    int GetSeconds() const {
        queryCount++;
        return seconds;
    }

    double GetMinutes() const {
        queryCount++;
        return seconds / 60.0;
    }

    bool IsPaused() const {
        return paused;
    }

    int GetQueryCount() const {
        return queryCount;
    }

    static int GetStopwatchCount() {
        return stopwatchCount;
    }

    static int GetActiveStopwatches() {
        return activeStopwatches;
    }
};

int Stopwatch::stopwatchCount = 0;
int Stopwatch::activeStopwatches = 0;

int main() {
    Stopwatch sw1(180);    
    Stopwatch sw2(2.5);  

    cout << "Seconds sw1: " << sw1.GetSeconds() << endl;
    cout << "Minutes sw2: " << sw2.GetMinutes() << endl;

    sw1.Tick();
    sw2.Tick();

    cout << "\nAfter Tick:" << endl;
    cout << "sw1 seconds: " << sw1.GetSeconds() << endl;
    cout << "sw2 seconds: " << sw2.GetSeconds() << endl;

    sw1.pause();
    cout << "\nsw1 paused: " << sw1.IsPaused() << endl;

    cout << "\nTotal stopwatches: " << Stopwatch::GetStopwatchCount() << endl;
    cout << "Active stopwatches: " << Stopwatch::GetActiveStopwatches() << endl;

    return 0;
}