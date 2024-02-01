#include <iostream>
#include <fstream>
#include <vector>
#include <ctime>
#include <iomanip>
#include <json/json.h>
#include <bcrypt/BCrypt.hpp>

class Task {
public:
    Task(const std::string& title, const std::string& description, int priority, const std::string& dueDate)
        : title(title), description(description), priority(priority), dueDate(dueDate), status("pending") {}

    const std::string& getTitle() const { return title; }
    const std::string& getDescription() const { return description; }
    int getPriority() const { return priority; }
    const std::string& getDueDate() const { return dueDate; }
    const std::string& getStatus() const { return status; }

    void setPriority(int newPriority) { priority = newPriority; }
    void setDueDate(const std::string& newDueDate) { dueDate = newDueDate; }
    void setStatus(const std::string& newStatus) { status = newStatus; }

private:
    std::string title;
    std::string description;
    int priority;
    std::string dueDate;
    std::string status;
};

class User {
public:
    User(const std::string& username, const std::string& password) : username(username), password(hashPassword(password)) {}

    const std::string& getUsername() const { return username; }
    const std::string& getPassword() const { return password; }

    bool authenticate(const std::string& inputPassword) const {
        return BCrypt::validatePassword(inputPassword, password);
    }

private:
    std::string username;
    std::string password;

    std::string hashPassword(const std::string& plainPassword) const {
        return BCrypt::generateHash(plainPassword);
    }
};

class TaskManager {
public:
    void addTask(const Task& task) {
        tasks.push_back(task);
    }

    void editTask(const std::string& title, const std::string& newDescription, int newPriority, const std::string& newDueDate) {
        for (auto& task : tasks) {
            if (task.getTitle() == title) {
                task = Task(title, newDescription, newPriority, newDueDate);
                std::cout << "Task edited successfully." << std::endl;
                return;
            }
        }
        std::cout << "Task not found." << std::endl;
    }

    void deleteTask(const std::string& title) {
        auto it = std::remove_if(tasks.begin(), tasks.end(), [title](const Task& task) {
            return task.getTitle() == title;
        });

        if (it != tasks.end()) {
            tasks.erase(it, tasks.end());
            std::cout << "Task deleted successfully." << std::endl;
        } else {
            std::cout << "Task not found." << std::endl;
        }
    }

    void displayTasks() {
        if (tasks.empty()) {
            std::cout << "No tasks available." << std::endl;
        } else {
            for (const auto& task : tasks) {
                std::cout << "Title: " << task.getTitle() << "\nDescription: " << task.getDescription()
                          << "\nPriority: " << task.getPriority() << "\nDue Date: " << task.getDueDate()
                          << "\nStatus: " << task.getStatus() << "\n\n";
            }
        }
    }

    void setTaskStatus(const std::string& title, const std::string& newStatus) {
        for (auto& task : tasks) {
            if (task.getTitle() == title) {
                task.setStatus(newStatus);
                std::cout << "Task status updated successfully." << std::endl;
                return;
            }
        }
        std::cout << "Task not found." << std::endl;
    }

    void setTaskPriority(const std::string& title, int newPriority) {
        for (auto& task : tasks) {
            if (task.getTitle() == title) {
                task.setPriority(newPriority);
                std::cout << "Task priority updated successfully." << std::endl;
                return;
            }
        }
        std::cout << "Task not found." << std::endl;
    }

    void setTaskDueDate(const std::string& title, const std::string& newDueDate) {
        for (auto& task : tasks) {
            if (task.getTitle() == title) {
                task.setDueDate(newDueDate);
                std::cout << "Task due date updated successfully." << std::endl;
                return;
            }
        }
        std::cout << "Task not found." << std::endl;
    }

    const std::vector<Task>& getTasks() const { return tasks; }

    void addTasks(const std::vector<Task>& newTasks) {
        tasks.insert(tasks.end(), newTasks.begin(), newTasks.end());
    }

private:
    std::vector<Task> tasks;
};

class FileManager {
public:
    void saveTasksToFile(const std::string& filename, const std::vector<Task>& tasks) {
        Json::Value root;
        for (const auto& task : tasks) {
            Json::Value taskJson;
            taskJson["title"] = task.getTitle();
            taskJson["description"] = task.getDescription();
            taskJson["priority"] = task.getPriority();
            taskJson["dueDate"] = task.getDueDate();
            taskJson["status"] = task.getStatus();
            root.append(taskJson);
        }

        std::ofstream file(filename);
        file << std::setw(4) << root;
        file.close();
    }

    std::vector<Task> readTasksFromFile(const std::string& filename) {
        std::vector<Task> tasks;

        Json::Value root;
        std::ifstream file(filename);
        file >> root;

        for (const auto& taskJson : root) {
            Task task(
                taskJson["title"].asString(),
                taskJson["description"].asString(),
                taskJson["priority"].asInt(),
                taskJson["dueDate"].asString()
            );
            task.setStatus(taskJson["status"].asString());
            tasks.push_back(task);
        }

        file.close();
        return tasks;
    }

    void saveUsersToFile(const std::string& filename, const std::vector<User>& users) {
        Json::Value root;
        for (const auto& user : users) {
            Json::Value userJson;
            userJson["username"] = user.getUsername();
            userJson["password"] = user.getPassword();
            root.append(userJson);
        }

        std::ofstream file(filename);
        file << std::setw(4) << root;
        file.close();
    }

    std::vector<User> readUsersFromFile(const std::string& filename) {
        std::vector<User> users;

        Json::Value root;
        std::ifstream file(filename);
        file >> root;

        for (const auto& userJson : root) {
            User user(
                userJson["username"].asString(),
                userJson["password"].asString()
            );
            users.push_back(user);
        }

        file.close();
        return users;
    }
};

class UI {
public:
    void run(TaskManager& taskManager, FileManager& fileManager) {
        while (true) {
            int choice;
            std::cout << "1. Add Task\n2. Edit Task\n3. Delete Task\n4. Display Tasks\n";
            std::cout << "5. Set Task Status\n6. Set Task Priority\n7. Set Task Due Date\n8. Save and Exit\n";
            std::cout << "Enter your choice: ";
            std::cin >> choice;

            switch (choice) {
                case 1:
                    addTask(taskManager);
                    break;
                case 2:
                    editTask(taskManager);
                    break;
                case 3:
                    deleteTask(taskManager);
                    break;
                case 4:
                    taskManager.displayTasks();
                    break;
                case 5:
                    setTaskStatus(taskManager);
                    break;
                case 6:
                    setTaskPriority(taskManager);
                    break;
                case 7:
                    setTaskDueDate(taskManager);
                    break;
                case 8:
                    fileManager.saveTasksToFile("tasks.json", taskManager.getTasks());
                    std::cout << "Exiting...\n";
                    return;
                default:
                    std::cout << "Invalid choice. Please try again.\n";
            }
        }
    }

private:
    void addTask(TaskManager& taskManager) {
        std::string title, description, dueDate;
        int priority;

        std::cout << "Enter task details:\n";
        std::cout << "Title: ";
        std::cin.ignore();
        std::getline(std::cin, title);

        std::cout << "Description: ";
        std::getline(std::cin, description);

        std::cout << "Priority: ";
        std::cin >> priority;

        std::cout << "Due Date: ";
        std::cin >> dueDate;

        Task task(title, description, priority, dueDate);
        taskManager.addTask(task);
        std::cout << "Task added successfully.\n";
    }

    void editTask(TaskManager& taskManager) {
        std::string title, newDescription, newDueDate;
        int newPriority;

        std::cout << "Enter the title of the task to edit: ";
        std::cin.ignore();
        std::getline(std::cin, title);

        std::cout << "Enter new task details:\n";
        std::cout << "New Description: ";
        std::getline(std::cin, newDescription);

        std::cout << "New Priority: ";
        std::cin >> newPriority;

        std::cout << "New Due Date: ";
        std::cin >> newDueDate;

        taskManager.editTask(title, newDescription, newPriority, newDueDate);
    }

    void deleteTask(TaskManager& taskManager) {
        std::string title;
        std::cout << "Enter the title of the task to delete: ";
        std::cin.ignore();
        std::getline(std::cin, title);

        taskManager.deleteTask(title);
    }

    void setTaskStatus(TaskManager& taskManager) {
        std::string title, newStatus;
        std::cout << "Enter the title of the task: ";
        std::cin.ignore();
        std::getline(std::cin, title);

        std::cout << "Enter the new status (e.g., pending, completed): ";
        std::cin >> newStatus;

        taskManager.setTaskStatus(title, newStatus);
    }

    void setTaskPriority(TaskManager& taskManager) {
        std::string title;
        int newPriority;

        std::cout << "Enter the title of the task: ";
        std::cin.ignore();
        std::getline(std::cin, title);

        std::cout << "Enter the new priority: ";
        std::cin >> newPriority;

        taskManager.setTaskPriority(title, newPriority);
    }

    void setTaskDueDate(TaskManager& taskManager) {
        std::string title, newDueDate;

        std::cout << "Enter the title of the task: ";
        std::cin.ignore();
        std::getline(std::cin, title);

        std::cout << "Enter the new due date: ";
        std::cin >> newDueDate;

        taskManager.setTaskDueDate(title, newDueDate);
    }
};

int main() {
    FileManager fileManager;

    std::vector<Task> tasks = fileManager.readTasksFromFile("tasks.json");
    TaskManager taskManager;
    taskManager.addTasks(tasks);

    std::vector<User> users = fileManager.readUsersFromFile("users.json");

    UI taskManagerUI;

    while (true) {
        int choice;
        std::cout << "1. Sign Up\n2. Log In\n3. Exit\n";
        std::cout << "Enter your choice: ";
        std::cin >> choice;

        switch (choice) {
            case 1:
                signUp(users, fileManager);
                break;
            case 2:
                if (logIn(users)) {
                    taskManagerUI.run(taskManager, fileManager);
                } else {
                    std::cout << "Login failed. Incorrect username or password.\n";
                }
                break;
            case 3:
                fileManager.saveTasksToFile("tasks.json", taskManager.getTasks());
                fileManager.saveUsersToFile("users.json", users);
                std::cout << "Exiting...\n";
                return 0;
            default:
                std::cout << "Invalid choice. Please try again.\n";
        }
    }
}
