#pragma once
#include "ISubsystem.h"

class Task final
{
public:
    typedef std::function<void()> Process;

public:
    Task(Process&& process)
    {
        this->process = std::forward<Process>(process);
    }
    ~Task() = default;

    void Execute()
    {
        process();
    }

private:
    Process process;
};


class Thread final : public ISubsystem
{
public:
    Thread(class Context* context);
    ~Thread();

    const bool Initialize() override;

    void Invoke();

    template <typename Process>
    void AddTask(Process&& process);

private:
    std::vector<std::thread> threads;
    std::queue<std::shared_ptr<Task>> tasks;
    std::mutex task_mutex;
    std::condition_variable condition_var;
    uint thread_count;
    bool bStopping;
};

template<typename Process>
inline void Thread::AddTask(Process && process)
{
    if (threads.empty())
    {
        LOG_WARNING("No available threads, function will execute in the same thread");
        process();
        return;
    }

    std::unique_lock<std::mutex> lock(task_mutex);

    tasks.push(std::make_shared<Task>(std::bind(std::forward<Process>(process))));

    lock.unlock();

    condition_var.notify_one();
}
