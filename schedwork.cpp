#ifndef RECCHECK
#include <set>
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <map>
#include <algorithm>
#endif

// add or remove necessary headers as you please
#include "schedwork.h"

using namespace std;

// a constant that can be used to indicate an INVALID 
// worker ID if that is useful to your implementation.
// Feel free to not use or delete.
static const Worker_T INVALID_ID = (unsigned int)-1;


// Add prototypes for any helper functions here
void scheduler(const AvailabilityMatrix& avail,
    const size_t dailyNeed,
    const size_t maxShifts,
		const size_t w_cnt,
    size_t n_day,
    size_t d_needed,
    std::vector<size_t> shiftCnt,
		std::vector<size_t> shiftToday,
    DailySchedule& sched,
		bool& found);


// Add your implementation of schedule() and other helper functions here

bool schedule(
    const AvailabilityMatrix& avail,
    const size_t dailyNeed,
    const size_t maxShifts,
    DailySchedule& sched
)
{
    if(avail.size() == 0){
        return false;
    }
    sched.clear();
    // Add your code below

		// # of workers
		const int w_cnt = avail[0].size();

		// keeps track of # of shifts per worker, starts a 0 
		std::vector<size_t> shiftCnt(w_cnt, 0);
		std::vector<size_t> shiftToday(w_cnt, 0);

		// setting up schedule
		// note: avail.size() is the # of days
		for (size_t n = 0; n < avail.size(); n++){
			std::vector<Worker_T> temp;
			for (size_t d = 0; d < dailyNeed; d++){
				temp.push_back(INVALID_ID);
			}
			sched.push_back(temp);
		}

		bool found = false;

		scheduler(avail, dailyNeed, maxShifts, w_cnt, 0, 0, shiftCnt, shiftToday, sched, found);
		return found;

}


void scheduler(const AvailabilityMatrix& avail,
    const size_t dailyNeed,
    const size_t maxShifts,
		const size_t w_cnt,
    size_t n_day,
    size_t d_needed,
    std::vector<size_t> shiftCnt,
		std::vector<size_t> shiftToday,
    DailySchedule& sched,
		bool& found)
{
    // iterations should deal with scheduling a day at a time
		if (found){
			return;
		}
    // if the day is unscheduled and the # of needed workers (d) not reached
		else {
	    // for each worker that exists
      // (avail[0] gives the first column, each columns is a different worker)
			// k = worker
      for (unsigned int k = 0; k < w_cnt; k++){
				if (found || sched[avail.size() - 1][dailyNeed - 1] != INVALID_ID) {
					return;
				}
        if (avail[n_day][k] == 1 && shiftCnt[k] < maxShifts && shiftToday[k] == 0){
					// insert worker id into that time slot
					sched[n_day][d_needed] = k;
					// increase their shift tracker by 1
					shiftCnt[k] += 1;
					// track that they are already scheduled for today as well
					shiftToday[k] += 1;

					// EARLY RETURN CASE:
					if (sched[avail.size() - 1][dailyNeed - 1] != INVALID_ID) {
						found = true;
						return;
					}

					// when at last col, recurse to next row in array
					if (d_needed == dailyNeed - 1){ // next row, reset shiftToday
						std::vector<size_t> shiftToday(w_cnt, 0);
						scheduler(avail, dailyNeed, maxShifts, w_cnt, n_day + 1, 0, shiftCnt, shiftToday, sched, found);
					}
					else{ // next col
						scheduler(avail, dailyNeed, maxShifts, w_cnt, n_day, d_needed + 1, shiftCnt, shiftToday, sched, found);
					}

					shiftCnt[k] -= 1;
					shiftToday[k] -= 1;
				} 
      }
		}
}

