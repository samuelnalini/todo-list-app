#pragma once

#include <unordered_map>
#include <string>

enum class TaskState;
class Task;
class REPL;

enum class TaskState {
    INCOMPLETE,
    COMPLETE,
};

typedef struct Task {
    Task() {};
    Task(std::string name, std::string desc, TaskState state)
    : name(name)
    , desc(desc)
    , state(state)
    {};

    std::string name;
    std::string desc;
    TaskState state;

} Task;

typedef class REPL {
public:
    REPL() {};

    void Run();

private:
    bool LoadFromFile(const char* fileName);
    bool SaveToFile(const char* fileName);
    void MainLoop();
    void PrintMenu();
    void AddTask();
    void RmTask();
    void SetTaskState();
    void ListTasks();
    

private:
    std::unordered_map<std::string, Task> m_tasklist;
    bool saved{ true };
} REPL;
