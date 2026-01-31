/********************************************
 * Copyright (c) 2026 Shun/翔海 (@shun4midx) *
 * Project: Taipei-MRT-Scheduler            *
 * File Type: C++ file                      *
 * File: name_converter.cpp                 *
 ****************************************** */

#include "name_converter.h"
#include <iostream>
#include <stdexcept>

// ======== RETRIEVAL FUNCTIONS ========= //
std::string getName(int line_int, int stn_num, int lang_int) {
    // Detect invalid inputs
    if (line_int < Line::R || line_int > Line::Y) {
        throw std::invalid_argument("getName: Invalid Line");
    }

    if (lang_int < Language::zh || lang_int > Language::kr) {
        throw std::invalid_argument("getName: Invalid Language");
    }

    // Deal with inputs
    std::unordered_map<Line, std::vector<std::unordered_map<Language, std::string>>> LINE_NAMES = {
        {R, R_NAMES}, {G, G_NAMES}, {BL, BL_NAMES}, {BR, BR_NAMES}, {Y, Y_NAMES}
    }; // except for O because that's an exception

    Line line = static_cast<Line>(line_int);
    Language lang = static_cast<Language>(lang_int);

    if (line != O) {
        if (stn_num <= 0 || stn_num >= LINE_NAMES[line].size() || LINE_NAMES[line][stn_num][lang] == INVALID) {
            throw std::invalid_argument("getName: No such station");
        } else {
            return LINE_NAMES[line][stn_num][lang];
        }
    } else {
        // Basic invalid
        if (stn_num <= 0 || stn_num >= 50 + O50_NAMES.size()) {
            throw std::invalid_argument("getName: No such station");
        }

        // O01 branch
        else if (stn_num < 50 && stn_num < O01_NAMES.size()) {
            return O01_NAMES[stn_num][lang];
        }

        // O50 branch
        else if (stn_num >= 50) {
            return O50_NAMES[stn_num - 50][lang];
        }

        else {
            throw std::invalid_argument("getName: No such station");
        }
    }
}