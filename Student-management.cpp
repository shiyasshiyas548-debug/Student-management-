#include <iostream>
#include <fstream>
#include <string>
#include <iomanip>
#include <vector>
#include <algorithm>

using namespace std;

struct Student {
    int id;
    string name;
    int age;
    string course;
    float gpa;
};

const string FILE_NAME = "students.dat";

void clearScreen() {
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
}

void pauseScreen() {
    cout << "\nPress Enter to continue...";
    cin.ignore();
    cin.get();
}

void printHeader(const string& title) {
    cout << "\n";
    cout << "=========================================\n";
    cout << title << "\n";
    cout << "=========================================\n\n";
}

void printTableHeader() {
    cout << left
         << setw(6) << "ID"
         << setw(20) << "Name"
         << setw(6) << "Age"
         << setw(15) << "Course"
         << setw(6) << "GPA"
         << "\n";

    cout << string(53, '-') << "\n";
}

void printStudent(const Student& s) {
    cout << left
         << setw(6) << s.id
         << setw(20) << s.name
         << setw(6) << s.age
         << setw(15) << s.course
         << fixed << setprecision(2) << setw(6) << s.gpa
         << "\n";
}

vector<Student> loadAll() {
    vector<Student> students;

    ifstream fin(FILE_NAME, ios::binary);
    if (!fin)
        return students;

    Student s;
    while (fin.read(reinterpret_cast<char*>(&s), sizeof(s)))
        students.push_back(s);

    fin.close();
    return students;
}

void saveAll(const vector<Student>& students) {
    ofstream fout(FILE_NAME, ios::binary | ios::trunc);

    for (const auto& s : students)
        fout.write(reinterpret_cast<const char*>(&s), sizeof(s));

    fout.close();
}

bool idExists(const vector<Student>& students, int id) {
    for (const auto& s : students) {
        if (s.id == id)
            return true;
    }
    return false;
}

void addStudent() {
    clearScreen();
    printHeader("Add Student");

    vector<Student> students = loadAll();
    Student s;

    cout << "Enter Student ID: ";
    cin >> s.id;

    if (idExists(students, s.id)) {
        cout << "Student ID already exists.\n";
        pauseScreen();
        return;
    }

    cin.ignore();

    cout << "Enter Name: ";
    getline(cin, s.name);

    cout << "Enter Age: ";
    cin >> s.age;
    cin.ignore();

    cout << "Enter Course: ";
    getline(cin, s.course);

    cout << "Enter GPA: ";
    cin >> s.gpa;

    students.push_back(s);
    saveAll(students);

    cout << "\nStudent added successfully.\n";
    pauseScreen();
}

void displayAll() {
    clearScreen();
    printHeader("All Students");

    vector<Student> students = loadAll();

    if (students.empty()) {
        cout << "No records found.\n";
        pauseScreen();
        return;
    }

    printTableHeader();

    for (const auto& s : students)
        printStudent(s);

    cout << "\nTotal Records: " << students.size() << "\n";
    pauseScreen();
}

void searchStudent() {
    clearScreen();
    printHeader("Search Student");

    int id;

    cout << "Enter Student ID: ";
    cin >> id;

    vector<Student> students = loadAll();

    for (const auto& s : students) {
        if (s.id == id) {
            cout << "\nStudent Found\n";
            cout << "ID: " << s.id << "\n";
            cout << "Name: " << s.name << "\n";
            cout << "Age: " << s.age << "\n";
            cout << "Course: " << s.course << "\n";
            cout << "GPA: " << fixed << setprecision(2) << s.gpa << "\n";

            pauseScreen();
            return;
        }
    }

    cout << "Student not found.\n";
    pauseScreen();
}

void updateStudent() {
    clearScreen();
    printHeader("Update Student");

    int id;

    cout << "Enter Student ID: ";
    cin >> id;

    vector<Student> students = loadAll();

    for (auto& s : students) {
        if (s.id == id) {
            cin.ignore();

            cout << "Enter New Name: ";
            getline(cin, s.name);

            cout << "Enter New Age: ";
            cin >> s.age;
            cin.ignore();

            cout << "Enter New Course: ";
            getline(cin, s.course);

            cout << "Enter New GPA: ";
            cin >> s.gpa;

            saveAll(students);

            cout << "\nStudent record updated successfully.\n";
            pauseScreen();
            return;
        }
    }

    cout << "Student not found.\n";
    pauseScreen();
}

void deleteStudent() {
    clearScreen();
    printHeader("Delete Student");

    int id;

    cout << "Enter Student ID: ";
    cin >> id;

    vector<Student> students = loadAll();

    auto it = remove_if(
        students.begin(),
        students.end(),
        [id](const Student& s) {
            return s.id == id;
        });

    if (it == students.end()) {
        cout << "Student not found.\n";
    } else {
        char confirm;

        cout << "Are you sure you want to delete this record? (y/n): ";
        cin >> confirm;

        if (tolower(confirm) == 'y') {
            students.erase(it, students.end());
            saveAll(students);

            cout << "\nStudent deleted successfully.\n";
        } else {
            cout << "Deletion cancelled.\n";
        }
    }

    pauseScreen();
}

void showMenu() {
    clearScreen();

    cout << "\n===== STUDENT MANAGEMENT SYSTEM =====\n";
    cout << "1. Add Student\n";
    cout << "2. Display All Students\n";
    cout << "3. Search Student\n";
    cout << "4. Update Student\n";
    cout << "5. Delete Student\n";
    cout << "6. Exit\n";
    cout << "\nEnter your choice: ";
}

int main() {
    int choice;

    do {
        showMenu();
        cin >> choice;

        switch (choice) {
            case 1:
                addStudent();
                break;

            case 2:
                displayAll();
                break;

            case 3:
                searchStudent();
                break;

            case 4:
                updateStudent();
                break;

            case 5:
                deleteStudent();
                break;

            case 6:
                cout << "\nThank you for using the Student Management System.\n";
                break;

            default:
                cout << "Invalid choice. Please try again.\n";
                pauseScreen();
        }

    } while (choice != 6);

    return 0;
}