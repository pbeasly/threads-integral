#include <iostream>
#include <thread>
#include <vector>
#include <cmath>
#include <cstdlib>
#include <mutex>
#include <chrono>

//Perform an integration using threads.  
// User inputs number of points and number of threads
// compile with g++ -std=c++14 -o xint_threads integrate-threads.cpp

// establish the analytic result
const double L_accurate = 35.0/8.0 + std::log(6);

// Create function for integrand
double L(double x)
{
    
    return std::sqrt(1.0+(1.0/x-x/4.0)*(1.0/x-x/4.0));

}


// Function to integrate over a range
double integrate_range(double a, double b, int num_points) {
    double sum = 0.0;
    double h = (b - a) / num_points;
    for (double x = a; x < b; x += h) {
        sum += L(x) * h;
    }
    return sum;
}

// Mutex to protect shared variable
std::mutex result_mutex;


int main(int argc, char* argv[]) {

    // start time for
    auto start = std::chrono::high_resolution_clock::now();

    if (argc != 3) {
        std::cerr << "Usage: " << argv[0] << " <num points> <num threads>\n";
        return 1;
    }
    int num_points = std::atoi(argv[1]); //convert number of points to type int
    int num_threads = std::atoi(argv[2]); //convert number of threads to type int
    int pts_in_thread = num_points/num_threads; // points in thread interval
    double a = 1.0;
    double b = 6.0;
    double h = (b - a) / num_threads; // interval size for each thread
    std::vector<std::thread> threads;
    double result = 0.0; // Shared variable

    // Create threads and split up the integral problem
    for (int i = 0; i < num_threads; i++) {
        double start = a + i * h;
        double end = start + h;
        threads.push_back(std::thread([start, end, pts_in_thread, &result]() {
            double thread_result = integrate_range(start, end, pts_in_thread);
            std::lock_guard<std::mutex> lock(result_mutex); // Lock the mutex
            result += thread_result; // Add the thread's result to the shared variable
            std::cout << "Thread " << std::this_thread::get_id() << ": "
                      << "integral from " << start << " to " << end
                      << " = " << thread_result << std::endl;
        }));
    }

    // Wait for threads to finish
    for (auto& thread : threads) {
        thread.join();
    }

    std::cout << "Total integral: " << result << std::endl;
    std::cout << "Analytic result:  "<< L_accurate <<std::endl;
    std::cout << "error: "<< L_accurate-result <<std::endl;

    // stop time and compute duration
    auto stop = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::nanoseconds> (stop-start);
    long double run_time = duration.count()*1e-9; //convert to seconds

    std::cout << "Number of Threads: " << num_threads << std::endl;
    std::cout << "Run time: " << run_time << std::endl;

    return 0;
}
