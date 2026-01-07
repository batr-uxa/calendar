#ifndef LOGIC_H_
#define LOGIC_H_

#include <vector>
#include <string>

//struct zadachi
struct Task {
	std::string date; // format YYYY-MM-DD
	std::string description;
	bool operator<(const Task& other) const { return date < other.date; }
};

bool IsValidDate(const std::string& date);
void SaveTasksToFile(const std::vector<Task>& tasks, const std::string& filename);
void LoadTasksFromFile(std::vector<Task>& tasks, const std::string& filename);
void ImportTasksFromExternalFile(std::vector<Task>& tasks, const std::string& main_file);
bool HasTasksOnDate(const std::vector<Task>& tasks, int day, int month, int year);
int CalculateDayOfWeek(int day, int month, int year);
void PrintMonthCalendar(const std::vector<Task>& tasks, int month, int year);
void ListAllTasks(const std::vector<Task>& tasks);

#endif  