#pragma once

#include <iostream>
#include <map>
#include "MyFunction.h"
#include "mystdany.h"


template<typename Func, typename... Args>
auto my_bind(Func&& func, Args&& ... args) {
    return [func, args...]() { return func(args...); };
}

class TTaskScheduler {
public:
    using TaskId = int;

    using StepResult = mystdany::any;

    struct TaskResult {
        StepResult step_result;
        bool is_result = false;
    };

    template<typename F, typename... Args>
    TaskId add(F&& func, Args&& ... args) {
        using ReturnType = decltype(func(args...));
        auto new_task = my_bind(func, std::forward<Args>(args)...);

        tasks_[id_] = new_task;
        results_[id_] = {ReturnType{}, false};

        return id_++;
    }

    template<typename T>
    T& GetFutureResult(TaskId task_id) {
        if (!results_.contains(task_id)) {
//            std::cerr << "Invalid Id";
            throw std::runtime_error("Invalid Id");
        }
        return mystdany::any_cast<T&>(results_[task_id].step_result);
    }

    template<typename T>
    T GetResult(TaskId task_id) {
        if (!results_.contains(task_id)) {
//            std::cerr << "Invalid Id";
            throw std::runtime_error("Invalid Id");
        }
        for (TaskId i = 0; i <= task_id; ++i) {
            if (!results_[i].is_result) {
                try {
                    tasks_[i]();
                } catch (std::exception& e) {
                    throw std::runtime_error("Invalid Function");
                }
                results_[i].step_result = tasks_[i]();
                results_[i].is_result = true;
            }
        }

        return mystdany::any_cast<T>(results_[task_id].step_result);
    }

    void ExecuteAll() {
        for (TaskId i = 0; i < tasks_.size(); ++i) {
            if (!results_[i].is_result) {
                results_[i].step_result = tasks_[i]();
                results_[i].is_result = true;
            }
        }
    }

private:
    std::map<TaskId, MyFunction<StepResult()>> tasks_;
    std::map<TaskId, TaskResult> results_;
    TaskId id_ = 0;
};
