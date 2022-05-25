#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

struct Interval {
    uint from;
    uint to;
    uint64_t passengerCount;
};

struct Cell {
    uint64_t passengerCount = 0;
    size_t controllerIdx = 0;
    size_t stationIdx = 0;
};


uint64_t calculateInterval(const vector<Interval> &intervals, size_t& last, size_t station, size_t count) {
    uint64_t res = 0;
    while(last < intervals.size() && station == intervals[last].from) {
        res += intervals[last].passengerCount;
        last++;
    }
    return res;
}

void calculateInsertions(vector<vector<uint64_t>>& insertions, const vector<Interval>& intervals, size_t stationCount) {
    for (auto interval : intervals)  {
        for (size_t i = 0; i < stationCount - 1; i++) {
            for (size_t j = i; j < stationCount - 1; j++) {
                if (interval.from != i && interval.from <= j && interval.from >=i  && interval.to > j) {
                    insertions[i][j] += interval.passengerCount;
                }
            }
        }
    }
}
void findBest(vector<vector<Cell>>& arr, const vector<Interval>& intervals, size_t controllerCount, size_t stationCount, size_t recordCount) {
    uint64_t curr = 0;
    Cell max{};
    vector<vector<uint64_t>> insertions(stationCount - 1, vector<uint64_t> (stationCount - 1, 0));
    calculateInsertions(insertions, intervals, stationCount);
    for (size_t i = 1; i < controllerCount; i++) {
        for (size_t station = i; station < stationCount-(controllerCount-i) ; station++) { //i-1
            //interval = calculateInterval(intervals, last, station, recordCount);
            for (size_t k = i-1; k < station; k++) {//k =station -2; station -1
                curr = arr[i-1][k].passengerCount + insertions[k][station];
                if (curr > max.passengerCount) {
                    max.passengerCount = curr;
                    max.stationIdx = k;
                    max.controllerIdx = i - 1;
                }
            }
            arr[i][station] = max;
            max = Cell();
        }
    }
}



void PrintAns(const vector<vector<Cell>>& arr, size_t stationsCount, size_t controllerCount) {
    Cell max{};
    size_t idx = 0;
    for (size_t i = 0; i < arr[controllerCount - 1].size(); i++) {
        if (arr[controllerCount -1][i].passengerCount > max.passengerCount) {
            max = arr[controllerCount - 1][i];
            idx = i;
        }
    }
    cout << max.passengerCount << endl;
    cout << "[" << idx << ";" << idx+1 << "]";
    if (idx == 0)
        return;
    for (size_t i = 0; i < controllerCount - 1; i++){
        cout << ",[" << max.stationIdx << ";" << max.stationIdx + 1 << "]";
        max = arr[max.controllerIdx][max.stationIdx];
    }
}


int main() {
    size_t controllerCount, recordCount, stationsCount;
    cin >> controllerCount >> stationsCount >> recordCount; //if controller cnt > station count -> print all
    vector<Interval> intervals;
    if (controllerCount >= stationsCount) {
        controllerCount = stationsCount - 1;
    }
    vector<vector<Cell>> arr(controllerCount + 1, vector<Cell> (stationsCount - 1, Cell()));

    for (size_t i = 0; i < recordCount; i++) {
        Interval interval{};
        cin >> interval.from >> interval.to >> interval.passengerCount;
        intervals.push_back(interval);
        for (uint j = interval.from; j < interval.to; j++) {
            arr[0][j].passengerCount += interval.passengerCount;
        }
    }
    sort(intervals.begin(), intervals.end(), [] (const Interval& a, const Interval& b) {return a.from < b.from;});
    findBest(arr, intervals, controllerCount, stationsCount, recordCount);
    PrintAns(arr, stationsCount, controllerCount);


    return 0;
}



/*
uint64_t max = 0;
size_t argmax = 0;
for (size_t i = 0; i < pathOfIntervals.size(); i++) {
if (pathOfIntervals[i] > max) {
max = pathOfIntervals[i];
argmax = i;
}
}
cout << max << endl;
cout << "[" << argmax << ";" << argmax + 1 << "]" << endl;
*/