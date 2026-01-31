/********************************************
 * Copyright (c) 2026 Shun/翔海 (@shun4midx) *
 * Project: Taipei-MRT-Scheduler            *
 * File Type: C++ file                      *
 * File: basic.cpp                          *
 ****************************************** */

#include "basic.h"

std::unordered_map<std::string, Line> LINES = {
    {"R", R}, {"O", O}, {"G", G}, {"BL", BL}, {"BR", BR}, {"Y", Y}
};