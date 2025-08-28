#include <iostream>
#include <valarray>
#include "lib/TaskScheduler.h"

int main() {
    float a = 1;
    float b = -2;
    float c = 0;

    TTaskScheduler scheduler;

    auto id0 = scheduler.add([](float a, float c) -> float { return -4 * a * c; }, a, c);
    auto id1 = scheduler.add([](float b, float* v) -> float { return b * b + *v; }, b,
                             &(scheduler.GetFutureResult<float>(id0)));
    auto id2 = scheduler.add([](float b, float* d) -> float { return -b + std::sqrt(*d); }, b,
                             &(scheduler.GetFutureResult<float>(id1)));
    auto id3 = scheduler.add([](float b, float* d) -> float { return -b - std::sqrt(*d); }, b,
                             &(scheduler.GetFutureResult<float>(id1)));
    auto id4 = scheduler.add([](float a, float* v) -> float { return *v / (2 * a); }, a,
                             &(scheduler.GetFutureResult<float>(id2)));
    auto id5 = scheduler.add([](float a, float* v) -> float { return *v / (2 * a); }, a,
                             &(scheduler.GetFutureResult<float>(id3)));


//    int a = 1;
//    int b = -2;
//    int c = 0;

//    auto id0 = scheduler.add([]() -> int { return 100; });
//    auto id1 = scheduler.add([](int * v)-> int { return 50 + *v; }, &(scheduler.GetFutureResult<int>(id0)));
//    auto id2 = scheduler.add([](int b, int * d) -> int { return b * (*d); }, b, &(scheduler.GetFutureResult<int>(id1)));


//    auto id3 = scheduler.add([](float b, float* d) -> float { return -b - std::sqrt(*d); }, b, &(scheduler.GetFutureResult<float>(id1)));
//    auto id4 = scheduler.add([](float a, float* v) -> float { return *v / (2 * a); }, a, &(scheduler.GetFutureResult<float>(id2)));
//    auto id5 = scheduler.add([](float a, float* v) -> float { return *v / (2 * a); }, a, &(scheduler.GetFutureResult<float>(id3)));


    scheduler.ExecuteAll();

    std::cout << "id0 = " << scheduler.GetResult<float>(id0) << std::endl;
    std::cout << "id1 = " << scheduler.GetResult<float>(id1) << std::endl;
    std::cout << "id2 = " << scheduler.GetResult<float>(id2) << std::endl;
    std::cout << "id3 = " << scheduler.GetResult<float>(id3) << std::endl;
    std::cout << "id4, x1 = " << scheduler.GetResult<float>(id4) << std::endl;
    std::cout << "id5, x2 = " << scheduler.GetResult<float>(id5) << std::endl;

    return 0;
}