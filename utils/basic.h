/********************************************
 * Copyright (c) 2026 Shun/翔海 (@shun4midx) *
 * Project: Taipei-MRT-Scheduler            *
 * File Type: C++ Header file               *
 * File: basic.h                            *
 ****************************************** */

#pragma once
#include <unordered_map>
#include <string>
#include <vector>

// ======== DEFINITIONS ======== //
enum Language {zh, en, jp, kr};
enum Line {R, O, G, BL, BR, Y};

extern const std::unordered_map<std::string, Line> LINES;

// ======== STRUCTS ========= //
typedef struct station {
    Line line;
    int stn_num;
} Station;

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
bool validStation(const Station& station);
bool validStation(int line_int, int stn_num);

Station makeStation(int line_int, int stn_num); // Returns station given relevant detail

std::string getName(const Station& station, int lang_int = zh); // Default to Chinese
std::string getName(int line_int, int stn_num, int lang_int = zh);