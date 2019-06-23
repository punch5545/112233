#pragma once
#include "Framework.h"

enum class EventType : uint
{
    Update,
    Render,
};

#define EVENT_HANDLER_STATIC(func)  []() { func(); }
#define EVENT_HANDLER(func)         [this]() { func(); }

class EventSystem final
{
public:
    typedef std::function<void()> Subscriber;

public:
    static EventSystem& Get()
    {
        static EventSystem instance;
        return instance;
    }

    void Subscibe(const EventType& type, Subscriber&& func)
    {
        subscribers[type].emplace_back(std::forward<Subscriber>(func));
    }

    void Fire(const EventType& type)
    {
        if (subscribers.find(type) == subscribers.end())
            return;

        for (auto subscriber : subscribers[type])
            subscriber();
    }

    void Clear()
    {
        subscribers.clear();
    }

private:
    EventSystem() = default;
    ~EventSystem() = default;

private:
    std::map<EventType, std::vector<Subscriber>> subscribers;
};