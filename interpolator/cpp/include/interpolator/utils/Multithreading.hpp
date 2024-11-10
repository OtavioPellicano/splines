#ifndef MULTITHREADING_H
#define MULTITHREADING_H

#include <future>
#include <vector>

namespace splines::utils
{

/**
 * @brief The Multithreading struct
 *
 * This struct is used to run multithreading algorithms.
 * The class is header only to avoid Template Explicit Instantiation
 *
 */
struct Multithreading
{
    template <typename Output, typename InputIt, typename Task>
    static std::vector<Output> run(InputIt first, InputIt last, unsigned num_threads_user, Task task)
    {
        std::vector<Output> output;
        std::size_t range_length = std::distance(first, last);
        if (!range_length || !num_threads_user)
        {
            return output;
        }

        unsigned hardware_threads = std::thread::hardware_concurrency();
        unsigned num_threads = std::min(hardware_threads ? hardware_threads : 1, num_threads_user);
        std::size_t block_size = range_length / num_threads;

        std::vector<std::future<std::vector<Output>>> results(num_threads);

        InputIt block_first = first;
        for (unsigned i = 0; i < num_threads - 1; ++i)
        {
            InputIt block_last = block_first;
            std::advance(block_last, block_size);
            results[i] = std::async(&Multithreading::run_block<Output, InputIt, Task>, block_first, block_last, task);
            block_first = block_last;
        }

        output.reserve(range_length);

        // running leftover
        *results.rbegin() = std::async(&Multithreading::run_block<Output, InputIt, Task>, block_first, last, task);

        for (auto &fut : results)
        {
            for (auto res : fut.get())
            {
                output.insert(output.begin() + output.size(), std::move(res));
            }
        }

        return output;
    }

  private:
    template <typename Output, typename InputIt, typename Task>
    static std::vector<Output> run_block(InputIt first, InputIt last, Task task)
    {
        std::vector<Output> thread_result;
        thread_result.reserve(std::distance(first, last));
        for (; first != last; ++first)
        {
            thread_result.push_back(task(*first));
        }
        return thread_result;
    }
};

} // namespace splines::utils

#endif // MULTITHREADING_H
