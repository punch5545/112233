#include "Framework.h"
#include "Thread.h"

Thread::Thread(Context * context)
    : ISubsystem(context)
    , bStopping(false)
{
    thread_count = std::thread::hardware_concurrency() - 1;
}

Thread::~Thread()
{
    std::unique_lock<std::mutex> lock(task_mutex);

    bStopping = true;

    lock.unlock();

    condition_var.notify_all();

    for (auto& thread : threads)
        thread.join();

    threads.clear();
    threads.shrink_to_fit();
}

const bool Thread::Initialize()
{
    for (uint i = 0; i < thread_count; i++)
        threads.emplace_back(std::thread(&Thread::Invoke, this));

    return true;
}

void Thread::Invoke()
{
    std::shared_ptr<Task> task;

    while (true)
    {
        std::unique_lock<std::mutex> lock(task_mutex);

        condition_var.wait(lock, [this]() { return !tasks.empty() || bStopping; });

        if (bStopping && tasks.empty())
            return;

        task = tasks.front();

        tasks.pop();

        lock.unlock();

        task->Execute();
    }
}