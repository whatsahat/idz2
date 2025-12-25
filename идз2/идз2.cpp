#include <iostream>
#include <cstdlib>
#include <cmath>
#include <ctime>
#include <limits>
#include <fstream>
#include <string>

using namespace std;

typedef int** Table;


int processTable(Table table, int rows, int cols, int& result) {
    if (rows <= 0 || cols <= 0 || table == nullptr) return 1;

    int maxZeros = 0, minSum = INT_MAX;
    bool hasZero = false;

    for (int i = 0; i < rows; i++) {
        int zeros = 0, sum = 0;
        for (int j = 0; j < cols; j++) {
            if (table[i][j] == 0)
            {
                zeros++;
            }
            sum += abs(table[i][j]);
        }

        if (zeros > 0)
        {
            hasZero = true;
        }

        if (zeros > maxZeros) 
        {
            maxZeros = zeros;
            minSum = sum;
        }
        else if (zeros == maxZeros && sum < minSum)
        {
            minSum = sum;
        }
    }

    result = hasZero ? minSum : 0;
    return 0;
}

Table createDynamicTable(int rows, int cols) {
    Table table = new int* [rows];
    for (int i = 0; i < rows; i++) {
        table[i] = new int[cols];
    }
    return table;
}

void deleteTable(Table& table, int rows) {
    if (table != nullptr) {
        for (int i = 0; i < rows; i++) {
            delete[] table[i];
        }
        delete[] table;
        table = nullptr;
    }
}

void clearInputBuffer() {
    cin.clear();
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
}

int safeInputInt(const string& prompt) {
    int value;
    while (true) {
        cout << prompt;
        cin >> value;
        if (cin.fail()) {
            cout << "Input error! Please enter an integer.\n";
            clearInputBuffer();
        }
        else {
            clearInputBuffer();
            break;
        }
    }
    return value;
}

void createTable(Table& table, int& rows, int& cols) {
    if (table != nullptr) {
        deleteTable(table, rows);
    }

     cout << "Enter number of rows: ";
    rows = safeInputInt("");
    while (rows <= 0) {
         cout << "Invalid number of rows! Please enter again: ";
        rows = safeInputInt("");
    }

     cout << "Enter number of columns: ";
    cols = safeInputInt("");
    while (cols <= 0) {
         cout << "Invalid number of columns! Please enter again: ";
        cols = safeInputInt("");
    }
        
    table = createDynamicTable(rows, cols); // создание таблицы

     cout << "\nChoose filling method:\n";
     cout << "1 - Manual input from keyboard\n";
     cout << "2 - Random generation\n";

    int choice = safeInputInt("Your choice: ");
    while (choice != 1 && choice != 2) {
         cout << "Wrong choice! Enter 1 or 2: ";
        choice = safeInputInt("");
    }

    if (choice == 1) {
         cout << "Enter table elements:\n";
        for (int i = 0; i < rows; i++) {
            for (int j = 0; j < cols; j++) {
                 cout << "Element [" << i << "][" << j << "]: ";
                table[i][j] = safeInputInt("");
            }
        }
    }
    else {
        int minVal, maxVal;
         cout << "Enter minimum value: ";
        minVal = safeInputInt("");
         cout << "Enter maximum value: ";
        maxVal = safeInputInt("");

        for (int i = 0; i < rows; i++) {
            for (int j = 0; j < cols; j++) {
                table[i][j] = minVal + rand() % (maxVal - minVal + 1);
            }
        }
    }

     cout << "Table created successfully!\n";
}

int loadTableFromFile(Table& table, int& rows, int& cols) {
     string filename;
     cout << "Enter filename: ";
     cin >> filename;
    clearInputBuffer();

     ifstream file(filename);
    if (!file.is_open()) {
        return 1;   //файл не читается
    }

    file >> rows >> cols;
    if (file.fail() || rows <= 0 || cols <= 0) {
        file.close(); //навереные размеры таблицы
        return 2; 
    }

    if (table != nullptr) {
        deleteTable(table, rows);
    }


    table = createDynamicTable(rows, cols);
    if (table == nullptr) {
        file.close();
        return 3;
    }

    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            file >> table[i][j];
            if (file.fail()) {
                deleteTable(table, rows);
                file.close();
                return 4; // неверные данные
            }
        }
    }

    file.close();
    return 0;
}

void showTable(Table table, int rows, int cols) {
    if (table == nullptr || rows == 0 || cols == 0) {
         cout << "Table is not created!\n";
        return;
    }

     cout << "\nTable " << rows << "x" << cols << ":\n";
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
             cout << table[i][j] << "\t";
        }
         cout << "\n";
    }
}

int main() {
    Table table = nullptr; 
    int rows = 0, cols = 0;
    int result = 0;
    int choice;

    srand(static_cast<unsigned int>(time(nullptr)));

     cout << "Table Processing Program\n";
     cout << "Task: In rows with maximum number of zeros find minimum sum of absolute values\n\n";

    do {
         cout << "\n=== MENU ===\n";
         cout << "1 - Create table\n";
         cout << "2 - Load table from file\n";
         cout << "3 - Show table\n";
         cout << "4 - Process table\n";
         cout << "5 - Show result\n";
         cout << "6 - Exit\n";

        choice = safeInputInt("Choose menu item: ");

        switch (choice) {
        case 1:
            createTable(table, rows, cols);
            break;

        case 2: {
            int fileResult = loadTableFromFile(table, rows, cols);
            if (fileResult == 0) {
                 cout << "Table loaded successfully from file!\n";
            }
            else if (fileResult == 1) {
                 cout << "Error: Cannot open file!\n";
            }
            else if (fileResult == 2) {
                 cout << "Error: Invalid table dimensions in file!\n";
            }
            else if (fileResult == 3) {
                 cout << "Error: Memory allocation failed!\n";
            }
            else if (fileResult == 4) {
                 cout << "Error: Invalid data in file!\n";
            }
            break;
        }

        case 3:
            showTable(table, rows, cols);
            break;

        case 4:
            if (table == nullptr || rows == 0 || cols == 0) {
                 cout << "Please create a table first!\n";
            }
            else {
                int code = processTable(table, rows, cols, result);
                if (code == 0) {
                     cout << "Processing completed successfully!\n";
                }
                else {
                     cout << "Error: Invalid table parameters!\n";
                }
            }
            break;

        case 5:
             cout << "Result: " << result << "\n";
            break;

        case 6:
             cout << "Exiting program...\n";
            break;

        default:
             cout << "Wrong choice! Please try again.\n";
        }
    } while (choice != 6);

    deleteTable(table, rows);

    return 0;
}