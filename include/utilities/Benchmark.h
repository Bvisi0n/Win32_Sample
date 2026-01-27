#ifndef BENCHMARK_H
#define BENCHMARK_H

// ------ STL ---------------------------------------
#include <algorithm>   // std::sort
#include <chrono>      // High-resolution clock
#include <format>      // std::format (Required for your PrintResults)
#include <functional>  // std::function, std::bind, std::forward
#include <iostream>    // std::wcout
#include <numeric>     // std::accumulate
#include <string>      // std::wstring, std::wstring_view
#include <vector>      // Storage for results

template <typename F, typename... Args>
class Benchmark {
public:
    Benchmark(std::wstring_view name, F&& func, Args&&... args)
        : m_Name(name),
        m_Task(std::bind(std::forward<F>(func), std::forward<Args>(args)...)) {}

    void Run(size_t iterations = 100, float discardPercentage = 0.1f)
    {
    #ifdef _DEBUG
        m_Results.clear();
        m_Results.reserve(iterations);

        for (size_t i = 0; i < iterations; ++i) {
            auto start = std::chrono::high_resolution_clock::now();

            m_Task();

            auto end = std::chrono::high_resolution_clock::now();
            auto duration = std::chrono::duration_cast<std::chrono::nanoseconds>(end - start);
            m_Results.push_back(duration.count());
        }

        CalculateStats(discardPercentage);
    #endif
    }

    void PrintResults() const
    {
    #ifdef _DEBUG
        if (m_Results.empty())
        {
            std::wcout << L"----- Benchmark: " << m_Name << L" (No results) -----" << std::endl;
            return;
        }
        std::wcout << L"\n----- Benchmark: " << m_Name << L" -----" << std::endl;
        std::wcout << std::format(L"Iterations:  {}\n", m_Results.size());
        std::wcout << std::format(L"Avg (Trim):  {:.2f} microseconds\n", m_AverageNs / 1000.0);
        std::wcout << std::format(L"Min:         {:.2f} microseconds\n", m_MinNs / 1000.0);
        std::wcout << std::format(L"Max:         {:.2f} microseconds\n", m_MaxNs / 1000.0);
        std::wcout << L"-------------------------------------" << std::endl;
    #endif
    }

private:
    void CalculateStats(float discardPercentage)
    {
        if (m_Results.empty()) return;

        std::sort(m_Results.begin(), m_Results.end());

        m_MinNs = static_cast<double>(m_Results.front());
        m_MaxNs = static_cast<double>(m_Results.back());

        size_t cutOff = static_cast<size_t>(m_Results.size() * discardPercentage);

        auto startIt = m_Results.begin() + cutOff;
        auto endIt = m_Results.end() - cutOff;

        if (startIt >= endIt)
        {
            m_AverageNs = std::accumulate(m_Results.begin(), m_Results.end(), 0.0) / m_Results.size();
        }
        else
        {
            double sum = std::accumulate(startIt, endIt, 0.0);
            m_AverageNs = sum / std::distance(startIt, endIt);
        }
    }

    std::wstring m_Name;
    std::function<void()> m_Task;
    std::vector<long long> m_Results;

    double m_AverageNs = 0.0;
    double m_MinNs = 0.0;
    double m_MaxNs = 0.0;
};

#endif