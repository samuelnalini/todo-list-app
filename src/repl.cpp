#include "headers/repl.hpp"

#include <fstream>
#include <iostream>
#include <stdexcept>
#include <string>
#include <sstream>
#include <iomanip>

TaskState parseState(const std::string& stateStr) {
    if (stateStr == "complete") return TaskState::COMPLETE;
    if (stateStr == "incomplete") return TaskState::INCOMPLETE;
    throw std::runtime_error("Unknown state: " + stateStr);
}


std::string getInput(const char* prompt) {
    std::string input;
    std::cout << '\n' << prompt;
    std::getline(std::cin, input);

    return input;
}


bool REPL::LoadFromFile(const char* fileName) {
    std::ifstream inputFile(fileName);
    int items{ 0 };

    if (!inputFile.is_open()) {
        std::cerr << "Failed to load from file " << fileName << '\n';
        return false;
    }

    std::string line;
    while (std::getline(inputFile, line)) {
        std::istringstream iss(line);
        std::string name, desc, stateStr;

        if (!(iss >> std::quoted(name) >> std::quoted(desc) >> stateStr)) {
            std::cerr << "Failed to parse line: " << line << '\n';
            continue;
        }

        TaskState state;
        try {
            state = parseState(stateStr);
        } catch (const std::exception& e) {
            std::cerr << e.what() << '\n';
            continue;
        }

        Task newTask(name, desc, state);
        m_tasklist[name] = newTask;
        items++;
    }

    if (items > 0)
        std::cout << "\nLoaded " << items << " item" << (items > 1 ? "s." : ".") << '\n';
    else
        std::cout << "\nNo items to load.\n";

    return true;
}

bool REPL::SaveToFile(const char* fileName) {
    std::cout << "\nSaving to " << fileName << '\n';

    std::ofstream outputFile(fileName);
    int items{ 0 };

    if (!outputFile.is_open()) {
        std::cerr << "Failed to save to file " << fileName << '\n';
        return false;
    }

    std::ostringstream oss;
    for (const auto& task : m_tasklist) {
        std::string stateStr{ (task.second.state == TaskState::COMPLETE ? "complete" : "incomplete") };
        oss << "\"" << task.first << "\" \"" << task.second.desc << "\" " << stateStr << '\n';
        items++;
    }

    outputFile << oss.str();

    if (outputFile.fail()) {
        std::cerr << "Error occurred while writing to file " << fileName << '\n';
        return false;
    }

    std::cout << "Successfully saved " << items << " item" << (items > 1 ? "s " : " ") << " to " << fileName << '\n';
    outputFile.close();
    saved = true;
    return true;
}

void REPL::PrintMenu() {
    std::cout << "\nSelect an option:\n";
    std::cout << "1. Add - add a task\n";
    std::cout << "2. Rm - remove a task\n";
    std::cout << "3. Set - set a task's status\n";
    std::cout << "4. List - list the current tasks\n";
    std::cout << "5. Save - save tasks to file\n";
    std::cout << "6. Exit - exit the program\n";
}

void REPL::AddTask() {
    std::cout << "\nAdd task:\n";
    std::string taskName{ getInput("Task name > ") };

    if (m_tasklist.find(taskName) != m_tasklist.end()) {
        std::cerr << "Task '" << taskName << "' already exists!\n";
        return;
    }

    std::string taskDesc{ getInput("Desc > ") };
    std::string stateStr{ getInput("Status (0 - incomplete, 1 - complete) > ") };

    TaskState taskState;

    switch(std::stoi(stateStr)) {
        case 1:
            taskState = TaskState::COMPLETE;
            break;
        default:
            taskState = TaskState::INCOMPLETE;
            break;
    }

    Task newTask(taskName, taskDesc, taskState);
    m_tasklist[taskName] = newTask;
    std::cout << "Successfully added task '" << taskName << "'!\n";
    saved = false;
}

void REPL::RmTask() {
    std::cout << "\nRemove task:\n";
    std::string taskName{ getInput("Task name > ") };

    auto task{ m_tasklist.find(taskName) };
    if (task != m_tasklist.end()) {
        std::cout << "Removed task '" << task->first << "'!\n";
        m_tasklist.erase(task->first);
        saved = false;
        return;
    }

    std::cout << "Task '" << taskName << "' does not exist\n";
}

void REPL::SetTaskState() {
    std::cout << "\nSet task status:\n";
    std::string taskName{ getInput("Task name > ") };
    
    auto task{ m_tasklist.find(taskName) };
    if (task == m_tasklist.end()) {
        std::cerr << taskName << " does not exist!\n";
        return;
    }

    std::string stateStr{ getInput("New status (0 - incomplete, 1 - complete) > ") };
    TaskState taskState;
    
    switch(std::stoi(stateStr)) {
        case 1:
            taskState = TaskState::COMPLETE;
            break;
        default:
            taskState = TaskState::INCOMPLETE;
            break;
    }

    task->second.state = taskState;
    std::cout << "Marked " << taskName << " as " << (taskState == TaskState::COMPLETE ? "complete" : "incomplete") << '\n';
    saved = false;
}

void REPL::ListTasks() {
    int idx{ 1 };
    std::cout << "\nTask list:\n";
    for (auto& task : m_tasklist) {
        std::cout << task.first << " | " << task.second.desc << " | " << (task.second.state == TaskState::COMPLETE ? "complete" : "incomplete") << '\n';
        idx++;
    }
    std::cout << '\n';
}

void REPL::MainLoop() {
    std::cout << "\nWelcome\n";
    while (true) {
        PrintMenu();
        std::string input{ getInput("> ") };

        try {
            switch(std::stoi(input)) {
                case 1:
                    AddTask();
                    break;
                case 2:
                    RmTask();
                    break;
                case 3:
                    SetTaskState();
                    break;
                case 4:
                    ListTasks();
                    break;
                case 5:
                    SaveToFile("tasks");
                    break;
                case 6:
                    if (saved)
                        exit(0);
                    else {
                        std::cout << "\nYou have unsaved changes!\n";
                        std::string exitInp{ getInput("Exit anyway? (y/N) > ") };

                        if (exitInp == "y" || exitInp == "Y")
                            exit(0);
                        else
                            continue;
                    }
                    break;
                default:
                    continue;
            }
        } catch (const std::invalid_argument& e) {
            continue;
        }
    }
}

void REPL::Run() {
    LoadFromFile("tasks");

    MainLoop();
}
