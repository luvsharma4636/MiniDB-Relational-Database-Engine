#pragma once
#include <string>
using namespace std;

// One row = one student record
struct Row {
    int id;
    char name[50];
    int age;
    float cgpa;
};

// How many rows fit in one page
const int PAGE_SIZE = 4096;
const int ROWS_PER_PAGE = PAGE_SIZE / sizeof(Row);
const int MAX_PAGES = 100;

class StorageEngine {
public:
    void insert_row(string table_name, Row& row);
    void read_all_rows(string table_name);
    void delete_row(string table_name, int id);
    int  count_rows(string table_name);
};