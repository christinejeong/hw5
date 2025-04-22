#ifndef RECCHECK
#include <set>
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <map>
#include <algorithm>
// add or remove necessary headers as you please

#endif

#include "schedwork.h"

using namespace std;

// a constant that can be used to indicate an INVALID 
// worker ID if that is useful to your implementation.
// Feel free to not use or delete.
static const Worker_T INVALID_ID = (unsigned int)-1;


// Add prototypes for any helper functions here
// recursive helper function to fill the schedule using backtracking
bool backtrack(
  const AvailabilityMatrix& avail, 
  size_t dailyNeed,
  size_t maxShifts, 
  DailySchedule& sched,
  vector<int>& shiftCount,
  size_t day,
  size_t slot
);


// Add your implementation of schedule() and other helper functions here

bool schedule(
    const AvailabilityMatrix& avail,
    const size_t dailyNeed,
    const size_t maxShifts,
    DailySchedule& sched
)
{
    if(avail.size() == 0U){
        return false;
    }
    sched.clear();
    // Add your code below

    // initialize sched matrix to have n rows and dailyNeed cols & fill w/ INVALID_ID
    size_t numDays = avail.size(); // number of days to schedule
    size_t numWorkers = avail[0].size(); // number of workers
    sched.resize(numDays, vector<Worker_T>(dailyNeed, INVALID_ID));

    // initialize a shiftCount vector to track how many shifts each worker has been assigned
    vector<int> shiftCount(numWorkers, 0);

    // call backtrack() helper function to fill schedule
    return backtrack(avail, dailyNeed, maxShifts, sched, shiftCount, 0, 0);
}

bool backtrack(
  const AvailabilityMatrix& avail, 
  size_t dailyNeed,
  size_t maxShifts, 
  DailySchedule& sched,
  vector<int>& shiftCount,
  size_t day,
  size_t slot
)
{
  // base case: done if successfully scheduled all days
  if(day == sched.size()) {
    return true;
  }

  // total number of workers
  size_t numWorkers = avail[0].size();

  // assigning each worker to the current [day][slot]
  for(Worker_T worker = 0; worker < numWorkers; ++worker) {
    // skip if worker is not available on this day
    if(!avail[day][worker]) continue;
    // skip if worker has already reached max shift limit
    if(shiftCount[worker] >= static_cast<int>(maxShifts)) continue;
    // check if worker is already scheduled earlier same day
    bool alreadyScheduled = false;
    for(size_t i = 0; i < slot; ++i) {
      if(sched[day][i] == worker) {
        alreadyScheduled = true;
        break;
      }
    }
    if(alreadyScheduled) continue;

    // assign worker to current slot
    sched[day][slot] = worker;
    shiftCount[worker]++;

    // move to next slot (next day if current day is full)
    size_t nextDay = day;
    size_t nextSlot = slot + 1;
    if(nextSlot == dailyNeed) {
      nextDay++;
      nextSlot = 0;
    }

    // fill in rest of the schedule
    if(backtrack(avail, dailyNeed, maxShifts, sched, shiftCount, nextDay, nextSlot)) return true;

    // backtrack: undo assignment and try diff worker
    sched[day][slot] = INVALID_ID;
    shiftCount[worker]--;
  }
  return false;
}

