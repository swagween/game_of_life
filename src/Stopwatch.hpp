//
//  Stopwatch.hpp
//  Game of Life
//
//  Created by Alex Frasca on 01/05/2023.
//
#pragma once

#include <iostream>
#include <iomanip>
#include <chrono>

namespace conway {

using Clock = std::chrono::steady_clock;
using Time = std::chrono::duration<float>;

class Stopwatch {
public:
    Stopwatch(std::string_view label) : m_label(label) {};
    ~Stopwatch() { lap(); };
    
    void lap(std::string_view new_label = {}) {
        auto const now = Clock::now();
        auto const dt = Time{now - m_start};
        std::cout << std::fixed << std::setprecision(3) << dt.count() * 1000.0f << "ms: " << m_label << "\n";
        m_label = new_label;
        m_start = now;
    }
    
private:
    std::string_view m_label;
    Clock::time_point m_start{Clock::now()};
};

} // end conway

/* Stopwatch_hpp */
