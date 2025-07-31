#pragma once

#include <filesystem>
#include <unordered_map>
#include <string>

enum class TaskState;
class Config;
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

typedef class Config {
public:
    Config() = default;

    std::filesystem::path* getSavePath() {
        return &m_savepath;
    }

    void setSavePath(std::filesystem::path path) {
        m_savepath = path;
    }

private:
    std::filesystem::path m_savepath{ "tasks" };
} Config;

typedef class REPL {
public:
    REPL() = default;

    void Run(std::filesystem::path configPath);

private:
    bool LoadConfig(std::filesystem::path configPath);
    bool LoadFromFile();
    bool SaveToFile();
    void MainLoop();
    void PrintMenu();
    void AddTask();
    void RmTask();
    void SetTaskState();
    void ListTasks();
    
private:
    std::unordered_map<std::string, Task> m_tasklist;
    Config m_config;
    bool m_saved{ true };
} REPL;
