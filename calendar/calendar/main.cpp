#include <algorithm>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <string>
#include <vector>
#include <Windows.h>
#include <logic.h>

int main() {
    SetConsoleCP(1251);
    SetConsoleOutputCP(1251);

    std::vector<Task> tasks;
    const std::string main_file = "tasks.txt";
    LoadTasksFromFile(tasks, main_file);

    int choice;
    while (true) {
        std::cout << "\n--- КАЛЕНДАРЬ-ПЛАНИРОВЩИК 2026 ---\n"
            << "1. Календарь на месяц     2. Показать все задачи\n"
            << "3. Добавить задачу        4. Поиск по дате\n"
            << "5. Редактировать задачу   6. Удалить задачу\n"
            << "7. Импорт из файла        0. Выход\n> ";

        if (!(std::cin >> choice)) break;
        if (choice == 0) break;

        switch (choice) {
        case 1: {
            int m, y;
            std::cout << "Введите месяц (1-12) и год (например, 2026): ";
            std::cin >> m >> y;
            if (m >= 1 && m <= 12) PrintMonthCalendar(tasks, m, y);
            else std::cout << "Ошибка: неверный месяц.\n";
            break;
        }
        case 2:
            ListAllTasks(tasks);
            break;
        case 3: {
            std::string d, desc;
            std::cout << "Дата (YYYY-MM-DD): "; std::cin >> d;
            std::cin.ignore();
            std::cout << "Описание: "; std::getline(std::cin, desc);
            if (IsValidDate(d)) {
                tasks.push_back({ d, desc });
                std::sort(tasks.begin(), tasks.end());
                SaveTasksToFile(tasks, main_file);
            }
            else {
                std::cout << "Ошибка: неверный формат даты.\n";
            }
            break;
        }
        case 4: {
            std::string d;
            std::cout << "Введите дату для поиска (YYYY-MM-DD): "; std::cin >> d;
            bool found = false;
            for (size_t i = 0; i < tasks.size(); ++i) {
                if (tasks[i].date == d) {
                    std::cout << "[" << i << "] " << tasks[i].description << "\n";
                    found = true;
                }
            }
            if (!found) std::cout << "На эту дату задач нет.\n";
            break;
        }
        case 5: {
            ListAllTasks(tasks);
            size_t id;
            std::cout << "Введите ID для изменения: "; std::cin >> id;
            if (id < tasks.size()) {
                std::cout << "Новая дата (YYYY-MM-DD): "; std::cin >> tasks[id].date;
                std::cin.ignore();
                std::cout << "Новое описание: "; std::getline(std::cin, tasks[id].description);
                std::sort(tasks.begin(), tasks.end());
                SaveTasksToFile(tasks, main_file);
            }
            break;
        }
        case 6: {
            ListAllTasks(tasks);
            size_t id;
            std::cout << "Введите ID для удаления: "; std::cin >> id;
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
            std::cout << "Неверный выбор.\n";
        }
    }

    return 0;
}
