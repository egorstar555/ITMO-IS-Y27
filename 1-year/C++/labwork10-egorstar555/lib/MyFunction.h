#pragma once

template<typename T>
class MyFunction;

template<typename ReturnType, typename... Args>
class MyFunction<ReturnType(Args...)> {
public:
    MyFunction() : func(nullptr) {}

    template<typename Func>
    MyFunction(Func f) : func(new FunctionHolder<Func>(f)) {}

    ReturnType operator()(Args... args) {
        if (func) {
            return func->call(args...);
        }
    }

private:
    class FunctionBase {
    public:
        virtual ReturnType call(Args... args) = 0;

        virtual ~FunctionBase() {}
    };

    template<typename Func>
    class FunctionHolder : public FunctionBase {
    public:
        FunctionHolder(Func f) : func(f) {}

        ReturnType call(Args... args) override {
            return func(args...);
        }

    private:
        Func func;
    };

    FunctionBase* func;
};