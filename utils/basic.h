/********************************************
 * Copyright (c) 2026 Shun/翔海 (@shun4midx) *
 * Project: Taipei-MRT-Scheduler            *
 * File Type: C++ Header file               *
 * File: basic.h                            *
 ****************************************** */

#pragma once
#include <unordered_map>
#include <string>

enum Language {zh, en, jp, kr};
enum Line {R, O, G, BL, BR, Y};

extern std::unordered_map<std::string, Line> LINES;