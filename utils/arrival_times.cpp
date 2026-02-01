/********************************************
 * Copyright (c) 2026 Shun/翔海 (@shun4midx) *
 * Project: Taipei-MRT-Scheduler            *
 * File Type: C++ file                      *
 * File: arrival_times.cpp                  *
 ****************************************** */

#include "arrival_times.h"
#include "basic.h"

#include <stdexcept>
#include <fstream>
#include <filesystem>
#include <iostream>
#include <algorithm>

// ======== DEFINITIONS ======== //
const Time INVALID_TIME = Time{-1, -1};

// ======== LOADING ======== //
std::string dayGroup(const Line& line, int day_type) {
    if (day_type <= 0 || day_type > 7) {
        throw std::invalid_argument("Invalid day_type: " + std::to_string(day_type));
    } else {
        if (day_type <= 5) {
            return "12345";
        } else if (line == R) { // Only R has 6 and 7 separately
            return std::to_string(day_type);
        } else {
            return "67";
        }
    }
}

std::vector<std::string> split(const std::string& s) {
    std::vector<std::string> out;
    std::string cur;

    for (char c : s) {
        if (c == ',') {
            out.push_back(cur);
            cur.clear();
        } else {
            cur.push_back(c);
        }
    }
    out.push_back(cur);
    return out;
}

// Rmb to have exception for BR line
std::vector<Train> loadStationSchedule(const Station& stn, int day_type) {
    // Detect wrong inputs
    if (day_type <= 0 || day_type > 7) {
        throw std::invalid_argument("Invalid day type: " + std::to_string(day_type));
    }

    if (!validStation(stn)) {
        throw std::invalid_argument("Invalid station");
    }

    // Find correct file
    // Reads arrival_times/generated/{LINE}/{STATION}_{DAYS}.csv
    std::string file_name = std::filesystem::path(__FILE__).parent_path()/"arrival_times"/"generated"/LINE_TO_STR.at(stn.line)/(stationToCode(stn) + "_" + dayGroup(stn.line, day_type) + ".csv");

    std::ifstream fin(file_name);
    std::string read_line;

    // Parse info
    std::vector<Train> train_schedule;

    while (std::getline(fin, read_line)) {
        std::vector<std::string> train_info = split(read_line); // line,destination,direction,time (where line is like e.g. R-1)

        // typedef struct train {Station arrive; int time; int direction; Station train_dest;} Train;
        // Swallow invalid stations
        try {
            Train new_train;
            new_train.arrive = stn;
            new_train.time = std::stoi(train_info[3]);
            new_train.direction = std::stoi(train_info[2]);
            new_train.train_dest = codeToStation(train_info[1]);

            train_schedule.push_back(new_train);
        } catch (std::exception& e) {
            // Don't do anything
        }
    }

    return train_schedule;
}

void printTrainSchedule(const std::vector<Train>& train_schedule) {
    for (Train t : train_schedule) {
        std::cout << stationToCode(t.arrive) << " " << t.time << " " << t.direction << " " << stationToCode(t.train_dest) << std::endl;
    }
}

// ======== QUERY ======== //
bool sameStation(const Station& stn1, const Station& stn2) {
    return stn1.line == stn2.line && stn1.stn_num == stn2.stn_num;
}

bool sameTime(const Time& time1, const Time& time2) {
    return time1.hr == time2.hr && time1.min == time2.min;
}

int timeToMins(const Time& time) {
    return time.hr * 60 + time.min;
}

Time minsToTime(int mins) {
    if (mins < 0 || mins > 26 * 60) { // No train arrives at 2am of the next day I think
        throw std::invalid_argument("Invalid argument: " + std::to_string(mins));
    }

    return Time{mins / 60, mins % 60};
}

std::string timeToStr(const Time& time) {
    if (sameTime(time, INVALID_TIME)) {
        return "INVALID_TIME";
    }
    
    return (time.hr < 10 ? "0" : "") + std::to_string(time.hr) + ":" + (time.min < 10 ? "0" : "") + std::to_string(time.min);
}

bool oneTrainReachDest(const Station& stn, const Station& dest, Train train) {
    // Detect exceptions
    if (!validStation(stn)) {
        throw std::invalid_argument("Invalid station stn");
    }

    if (!validStation(dest)) {
        throw std::invalid_argument("Invalid station dest");
    }

    if (sameStation(stn, dest)) {
        throw std::invalid_argument("stn and dest are the same station");
    }

    // Code
    if (stn.line != dest.line) { // Need interchange
        return false;
    }

    if (stn.line != O) { // Orange line is diff
        if (stn.stn_num < dest.stn_num && stn.stn_num < train.train_dest.stn_num) { // Increasing
            return dest.stn_num <= train.train_dest.stn_num;
        } else if (stn.stn_num > dest.stn_num && stn.stn_num > train.train_dest.stn_num) { // Decreasing
            return dest.stn_num >= train.train_dest.stn_num;
        } else { // Wrong dir
            return false;
        }
    } else {
        // Rule out wrong direction stuff
        if ((stn.stn_num > dest.stn_num && stn.stn_num <= train.train_dest.stn_num) || (stn.stn_num < dest.stn_num && stn.stn_num >= train.train_dest.stn_num)) {
            return false;
        }

        if (stn.stn_num > dest.stn_num) { // Decreasing branch (definitely fine since the train is a valid train)
            return dest.stn_num >= train.train_dest.stn_num;
        } else { // Increasing branch
            // O12 is the last station that overlaps both branches
            if (dest.stn_num <= 12) {
                return dest.stn_num <= train.train_dest.stn_num;
            } else if (dest.stn_num <= train.train_dest.stn_num) { // Check the correct branch given that its number is plausibly reachable
                if (dest.stn_num < 50) { // dest in O01 branch
                    return train.train_dest.stn_num < 50;
                } else { // dest in O50 branch
                    return train.train_dest.stn_num >= 50;
                }
            } else { // Impossible
                return false;
            }
        }
    }
}

Time nextTrainTime(const Station& stn, int day_type, const Time& curr_time, const Station& dest) {
    return nextTrainTime(stn, day_type, timeToMins(curr_time), dest);
}

Time nextTrainTime(const Station& stn, int day_type, int now_mins, const Station& dest) {
    if (!validStation(stn)) {
        throw std::invalid_argument("Invalid station stn");
    }

    if (!validStation(dest)) {
        throw std::invalid_argument("Invalid station dest");
    }

    std::vector<Train> train_schedule = loadStationSchedule(stn, day_type);

    // Find the closest entry that has time >= now_mins
    auto it = std::lower_bound(
        train_schedule.begin(),
        train_schedule.end(),
        now_mins,
        [](const Train& t, int value) {
            return t.time < value;
        }
    );
    
    if (it != train_schedule.end()) {
        // *it is the first Train with t.time >= target_time
        while (!oneTrainReachDest(stn, dest, *it)) {
            if (++it == train_schedule.end()) {
                return INVALID_TIME;
            }
        }

        // Otherwise it's a valid time
        return minsToTime(it->time);
    } else { // No entry with such a time
        return INVALID_TIME;
    }
}