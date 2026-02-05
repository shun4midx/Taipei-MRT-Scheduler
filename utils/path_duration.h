/********************************************
 * Copyright (c) 2026 Shun/翔海 (@shun4midx) *
 * Project: Taipei-MRT-Scheduler            *
 * File Type: C++ Header file               *
 * File: path_duration.h                    *
 ****************************************** */

#pragma once

#include "basic.h"
#include <unordered_map>
#include <vector>

// ======== DEFINTIONS ======== //
typedef std::vector<Station> Path;
typedef std::vector<Time> PathTimes;

// ======== DATA ======== //
extern const int INVALID_DURATION;

extern const std::unordered_map<Line, std::vector<int>> LINE_DURATION; // Duration of each station from first avail station

// ======== FUNCTIONS ======== //
int getLineDuration(const Station& stn1, const Station& stn2); // One line without interchange

int perfectPathDuration(const Path& stn_path); // Path including interchanges, but assuming 0 wait time at all steps along the way (Same station but on different lines count as different points on the path for simplicity)

PathTimes pathETA(const Path& stn_path, Time curr_time, int day_type); // Actual path ETA including train waiting time, returns {} if impossible. Every element should be when the user would arrive at that station corr to stn_path, not when the upcoming train arrives.

std::string pathTimesToStr(const PathTimes& pt);
std::string pathToStr(const Path& p);