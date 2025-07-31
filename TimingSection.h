//
// Created by traw-GG on 31.07.2025.
//
#pragma once

#include "main.h"

struct TimingSection {
    std::string name;
    std::chrono::high_resolution_clock::time_point start;
    long long min_duration_ms;

    TimingSection(const char* name, long long min_duration);
    ~TimingSection();
};

inline thread_local std::vector<TimingSection> timingStack;

TimingSection::TimingSection(const char* name, long long min_duration)
        : name(name),
          start(std::chrono::high_resolution_clock::now()),
          min_duration_ms(min_duration) {
    timingStack.push_back(*this);
}

TimingSection::~TimingSection() {
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(
            std::chrono::high_resolution_clock::now() - start).count();

    if (duration >= min_duration_ms) {
        std::string indent(timingStack.size() - 1, ' ');
        Log("%s[TIMING] %s: %lld ms (threshold: %lld ms)",
            indent.c_str(),
            name.c_str(),
            duration,
            min_duration_ms);
    }
    timingStack.pop_back();
}

#define TIMED_SECTION(name, min_ms) TimingSection _timing_guard_##__LINE__(name, min_ms)