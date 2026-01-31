/********************************************
 * Copyright (c) 2026 Shun/翔海 (@shun4midx) *
 * Project: Taipei-MRT-Scheduler            *
 * File Type: C++ Header file               *
 * File: name_converter.h                   *
 ****************************************** */

#pragma once
#include <string>
#include <vector>
#include <unordered_map>
#include "consts.h"

// ======== STORING INFORMATION ======== //
extern const std::string INVALID;

extern std::vector<std::unordered_map<Language, std::string>> R_NAMES;
extern std::vector<std::unordered_map<Language, std::string>> G_NAMES;
extern std::vector<std::unordered_map<Language, std::string>> BL_NAMES;
extern std::vector<std::unordered_map<Language, std::string>> BR_NAMES;
extern std::vector<std::unordered_map<Language, std::string>> Y_NAMES;
extern std::vector<std::unordered_map<Language, std::string>> O01_NAMES;
extern std::vector<std::unordered_map<Language, std::string>> O50_NAMES;

// ======== RETRIEVAL FUNCTIONS ======== //
std::string getName(int line_int, int stn_num, int lang_int = zh); // Default to Chinese