#include "../include/storage.h"
#include <fstream>
#include <iostream>
#include <vector>
using namespace std;

void StorageEngine::insert_row(string table_name, Row& row) {
    string filename = "../data/" + table_name + ".db";
    ofstream file(filename, ios::binary | ios::app);
    
    if(!file) {
        cout << "Error: Cannot open file\n";
        return;
    }
    
    file.write(reinterpret_cast<char*>(&row), sizeof(Row));
    file.close();
    cout << "1 row inserted.\n";
}

void StorageEngine::read_all_rows(string table_name) {
    string filename = "../data/" + table_name + ".db";
    ifstream file(filename, ios::binary);
    
    if(!file) {
        cout << "Error: Table '" << table_name << "' not found\n";
        return;
    }
    
    Row row;
    int count = 0;
    
    // Print table header
    cout << "+-----+--------------------+-----+------+\n";
    cout << "| id  | name               | age | cgpa |\n";
    cout << "+-----+--------------------+-----+------+\n";
    
    // Read and print each row
    while(file.read(reinterpret_cast<char*>(&row), sizeof(Row))) {
        if(row.id == -1) continue; // skip deleted rows
        printf("| %-3d | %-18s | %-3d | %-4.1f |\n",
               row.id, row.name, row.age, row.cgpa);
        count++;
    }
    
    cout << "+-----+--------------------+-----+------+\n";
    cout << count << " row(s) found.\n";
    file.close();
}

void StorageEngine::delete_row(string table_name, int id) {
    string filename = "../data/" + table_name + ".db";
    fstream file(filename, ios::binary | ios::in | ios::out);
    
    if(!file) {
        cout << "Error: Table not found\n";
        return;
    }
    
    Row row;
    while(file.read(reinterpret_cast<char*>(&row), sizeof(Row))) {
        if(row.id == id) {
            // Mark as deleted by setting id to -1
            row.id = -1;
           file.seekp(-(long)sizeof(Row), ios::cur);
            file.write(reinterpret_cast<char*>(&row), sizeof(Row));
            cout << "1 row deleted.\n";
            return;
        }
    }
    cout << "Row with id=" << id << " not found.\n";
}

int StorageEngine::count_rows(string table_name) {
    string filename = "../data/" + table_name + ".db";
    ifstream file(filename, ios::binary);
    Row row;
    int count = 0;
    while(file.read(reinterpret_cast<char*>(&row), sizeof(Row))) {
        if(row.id != -1) count++;
    }
    return count;
}