#include <algorithm>
#include <fstream>
#include <iomanip>
#include <iostream>
#include "logic.h"

//proveryaet correctnost formata dati YYYY-MM-DD
bool IsValidDate(const std::string& date) {
    if (date.length() != 10) return false;
    if (date[4] != '-' || date[7] != '-') return false;
    for (int i = 0; i < 10; ++i) {
        if (i == 4 || i == 7) continue;
        if (!isdigit(date[i])) return false;
    }
    return true;
}

//Save tekushiy spisok zadach in general file
void SaveTasksToFile(const std::vector<Task>& tasks, const std::string& filename) {
    std::ofstream file(filename);
    if (!file.is_open()) return;
    for (int i = 0; i < static_cast<int>(tasks.size()); ++i) {
        file << tasks[i].date << "|" << tasks[i].description << "\n";
    }
}

//Zagruzhaet zadachi from general file pri starte programmi
void LoadTasksFromFile(std::vector<Task>& tasks, const std::string& filename) {
    tasks.clear();
    std::ifstream file(filename);
    if (!file.is_open()) return;

    std::string line;
    while (std::getline(file, line)) {
        size_t pos = line.find('|');
        if (pos != std::string::npos) {
            tasks.push_back({ line.substr(0, pos), line.substr(pos + 1) });
        }
    }
    std::sort(tasks.begin(), tasks.end());
}

//Import zadachi from any out file avec proverkoi 
void ImportTasksFromExternalFile(std::vector<Task>& tasks, const std::string& main_file) {
    std::string ext_filename;
    std::cout << "Enter the path to the external file to import: ";
    std::cin >> ext_filename;

    std::ifstream file(ext_filename);
    if (!file.is_open()) {
        std::cout << "Error: File not found.\n";
        return;
    }

    std::string line;
    int imported_count = 0;
    while (std::getline(file, line)) {
        size_t pos = line.find('|');
        if (pos != std::string::npos) {
            std::string date = line.substr(0, pos);
            std::string desc = line.substr(pos + 1);

            if (IsValidDate(date) && !desc.empty()) {
                tasks.push_back({ date, desc });
                imported_count++;
            }
        }
    }

    if (imported_count > 0) {
        std::sort(tasks.begin(), tasks.end());
        SaveTasksToFile(tasks, main_file);
        std::cout << "Tasks successfully imported: " << imported_count << "\n";
    }
    else {
        std::cout << "No matching data found in file (YYYY-MM-DD|text).\n";
    }
}

//Proverka zadach na concretnoe chislo
bool HasTasksOnDate(const std::vector<Task>& tasks, int day, int month, int year) {
    char buf[11];
    snprintf(buf, sizeof(buf), "%04d-%02d-%02d", year, month, day);
    std::string target_date(buf);

    for (int i = 0; i < static_cast<int>(tasks.size()); ++i) {
        if (tasks[i].date == target_date) return true;
    }
    return false;
}

//algorithm zellera (0 - Ñá, 1 - Âñ, ..., 6 - Ïò)
int CalculateDayOfWeek(int day, int month, int year) {
    if (month < 3) {
        month += 12;
        year--;
    }
    int k = year % 100;
    int j = year / 100;
    return (day + 13 * (month + 1) / 5 + k + k / 4 + j / 4 + 5 * j) % 7;
}

//vivod calendarya
void PrintMonthCalendar(const std::vector<Task>& tasks, int month, int year) {
    const char* months[] = { "", "January", "February", "March", "April", "May", "June",
                            "July", "August", "September", "October", "November", "December" };
    int days_in_month[] = { 0, 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 };
    if (month == 2 && ((year % 4 == 0 && year % 100 != 0) || (year % 400 == 0))) {
        days_in_month[2] = 29;
    }
    std::cout << "\n      " << months[month] << " " << year << "\n";
    std::cout << "Mon Tue Wed Thu Fri Sat Sun\n";

    // Perevod systemi zellera pn-vs (0-6)
    int first_day = (CalculateDayOfWeek(1, month, year) + 5) % 7;
    for (int i = 0; i < first_day; ++i) std::cout << "    ";
    for (int day = 1; day <= days_in_month[month]; ++day) {
        if (HasTasksOnDate(tasks, day, month, year)) {
            std::cout << "\033[38;5;208m" << std::setw(2) << day << "\033[0m";
        }
        else {
            std::cout << std::setw(2) << day;
        }

        if ((day + first_day) % 7 == 0) std::cout << "\n";
        else std::cout << "  ";
    }
    std::cout << "\n";
}

//vivod all zadach
void ListAllTasks(const std::vector<Task>& tasks) {
    if (tasks.empty()) {
        std::cout << "The task list is empty.\n";
        return;
    }
    std::cout << "\n--- List of all tasks ---\n";
    for (size_t i = 0; i < tasks.size(); ++i) {
        std::cout << "[" << i << "] " << tasks[i].date << " | " << tasks[i].description << "\n";
    }
}