#pragma once
#include<iostream>
#include<vector>
using namespace std;

enum QueryType{
    SELECT_ALL,
    SELECT_WHERE,
    INSERT,
    DELETE_WHERE,
    UPDATE_ROW,
    UNKNOWN,
};

struct Condition {
    string column;   // e.g. "id"
    string op;       // e.g. "="
    string value;    // e.g. "5"
};

struct Query {
    QueryType      type;
    string         table_name;
    Condition      where;
    vector<string> insert_values;  // for INSERT
    string         update_column;  // for UPDATE
    string         update_value;   // for UPDATE
    bool           is_valid;
    string         error_message;
};

class Parser {
    vector<string> tokenize(string sql);
    string         to_upper(string s);
public:
    Query parse(string sql);
};


