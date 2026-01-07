#include <algorithm>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <string>
#include <vector>
#include <Windows.h>
#include "logic.h"

int main() {
    SetConsoleCP(1251);
    SetConsoleOutputCP(1251);

    std::vector<Task> tasks;
    const std::string main_file = "tasks.txt";
    LoadTasksFromFile(tasks, main_file);

    int choice;
    while (true) {
        std::cout << "\n--- CALENDAR-PLANNER 2026 ---\n"
            << "1. Monthly calendar    2. Show all tasks\n"
            << "3. Add a task          4. Search by date\n"
            << "5. Edit a task         6. Delete task\n"
            << "7. Import from file    0. Exit\n> ";

        if (!(std::cin >> choice)) break;
        if (choice == 0) break;

        switch (choice) {
        case 1: {
            int m, y;
            std::cout << "Enter the month (1-12) and year (e.g. 2026): ";
            std::cin >> m >> y;
            if (m >= 1 && m <= 12) PrintMonthCalendar(tasks, m, y);
            else std::cout << "Error: Invalid month.\n";
            break;
        }
        case 2:
            ListAllTasks(tasks);
            break;
        case 3: {
            std::string d, desc;
            std::cout << "Date (YYYY-MM-DD): "; std::cin >> d;
            std::cin.ignore();
            std::cout << "Description: "; std::getline(std::cin, desc);
            if (IsValidDate(d)) {
                tasks.push_back({ d, desc });
                std::sort(tasks.begin(), tasks.end());
                SaveTasksToFile(tasks, main_file);
            }
            else {
                std::cout << "Error: Invalid date format.\n";
            }
            break;
        }
        case 4: {
            std::string d;
            std::cout << "Enter a date to search (YYYY-MM-DD): "; std::cin >> d;
            bool found = false;
            for (size_t i = 0; i < tasks.size(); ++i) {
                if (tasks[i].date == d) {
                    std::cout << "[" << i << "] " << tasks[i].description << "\n";
                    found = true;
                }
            }
            if (!found) std::cout << "There are no tasks for this date.\n";
            break;
        }
        case 5: {
            ListAllTasks(tasks);
            size_t id;
            std::cout << "Enter ID to change: "; std::cin >> id;
            if (id < tasks.size()) {
                std::cout << "New date (YYYY-MM-DD): "; std::cin >> tasks[id].date;
                std::cin.ignore();
                std::cout << "New description: "; std::getline(std::cin, tasks[id].description);
                std::sort(tasks.begin(), tasks.end());
                SaveTasksToFile(tasks, main_file);
            }
            break;
        }
        case 6: {
            ListAllTasks(tasks);
            size_t id;
            std::cout << "Enter ID to delete: "; std::cin >> id;
            if (id < tasks.size()) {
                tasks.erase(tasks.begin() + id);
                SaveTasksToFile(tasks, main_file);
            }
            break;
        }
        case 7:
            ImportTasksFromExternalFile(tasks, main_file);
            break;
        default:
            std::cout << "Wrong choice.\n";
        }
    }

    return 0;
}

