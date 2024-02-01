# Personal Task Manager

A simple console-based Personal Task Manager with features like task management, user signup, and login.

## Features

- Task Management:
  - Add, edit, and delete tasks with title, description, priority, due date, and status.
  - Display tasks with details.
  - Set task status, priority, and due date.

- User Management:
  - Signup with a unique username and securely hashed password.
  - Login with username and password authentication.

- File Operations:
  - Save and read tasks to/from a JSON file preserving object-oriented structure.
  - Save and read user data to/from a JSON file.

## Dependencies

- JSONcpp library: For handling JSON data.
- bcrypt library: For password hashing and validation.

## Usage

1. Compile the code using a C++ compiler.

```bash
g++ -o PersonalTaskManager PersonalTaskManager.cpp -ljsoncpp -lbcrypt
```

2. Run the executable.
```bash
./PersonalTaskManager
```
3. Follow the on-screen menu for task and user management.

## Note
Make sure to install the required libraries (jsoncpp, bcrypt) before compiling.
The user data is stored in "users.json," and task data is stored in "tasks.json."
The code is a console-based application, and interaction is through the command line.
Feel free to customize and extend the code based on your requirements!