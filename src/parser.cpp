#include<iostream>
#include"../include/parser.h"
#include<sstream>
#include<algorithm>
using namespace std;

// Convert string to uppercase
string Parser::to_upper(string s) {
    transform(s.begin(), s.end(), s.begin(), ::toupper);
    return s;
}

// Split SQL string into tokens
vector<string> Parser::tokenize(string sql) {
    vector<string> tokens;
    stringstream ss(sql);
    string word;
    while(ss >> word) {
        tokens.push_back(word);
    }
    return tokens;
}

Query Parser::parse(string sql) {
    Query q;
    q.is_valid = true;       //initialize query q assuming valid query initial

    // Tokenize
    vector<string> tokens=tokenize(sql);

    // Empty input
    if(tokens.empty()) {
        q.is_valid = false;
        q.error_message = "Empty query";
        return q;
    }

    string cmd=to_upper(tokens[0]);

    // ─────────────────────────────────────
    // SELECT * FROM students
    // SELECT * FROM students WHERE age = 20
    // ─────────────────────────────────────

    if(cmd=="SELECT") {
        if(tokens.size() < 4) {
            q.is_valid = false;
            q.error_message = "Invalid SELECT. Use: SELECT * FROM table";
            return q;
        }

         q.table_name=tokens[3];

        if(tokens.size()==4){
            q.type=SELECT_ALL;    
        }
        // SELECT * FROM students WHERE age = 20
       
        else if(tokens.size() >= 8 &&
                to_upper(tokens[4]) == "WHERE") {
            q.type           = SELECT_WHERE;
            q.where.column   = tokens[5];
            q.where.op       = tokens[6];
            q.where.value    = tokens[7];
        }
        else {
             q.is_valid = false;
             q.error_message = "Invalid SELECT syntax";
        }

        }

    // ─────────────────────────────────────
    // INSERT INTO students VALUES (1, Rahul, 20, 8.5)
    // ─────────────────────────────────────


    else if(cmd=="INSERT") {
        if(tokens.size()<4) {
            q.is_valid=false;
            q.error_message = "Invalid INSERT. Use: INSERT INTO table VALUES (...)";
            return q;
        }

        q.type = INSERT;
        q.table_name = tokens[2];

    // Extract values between ( and )
        string values_str = "";
        bool inside = false;
        for(char c : sql) {
            if(c == '(') { inside = true;  continue; }
            if(c == ')') { inside = false; continue; }
            if(inside) values_str += c;
        }

        if(values_str.empty()) {
            q.is_valid=false;
            q.error_message = "No values found between ( and )";
            return q;
        }

                // Split values by comma
        stringstream vs(values_str);
        string val;
        while(getline(vs, val, ',')) {
            // Trim spaces
            val.erase(0, val.find_first_not_of(" "));
            val.erase(val.find_last_not_of(" ") + 1);
            q.insert_values.push_back(val);
        }
    }
// ─────────────────────────────────────
// DELETE FROM students WHERE id = 1
// ─────────────────────────────────────

    else if(cmd=="DELETE") {
        if(tokens.size()<7) {
            q.is_valid=false;
            q.error_message= "Invalid DELETE. Use: DELETE FROM table WHERE col = val";
            return q;
        }

        q.type=DELETE_WHERE;
        q.table_name=tokens[2];
        q.where.column=tokens[4];
        q.where.op=tokens[5];
        q.where.value=tokens[6];
    }

    // ─────────────────────────────────────
    // UPDATE students SET cgpa = 9.5 WHERE id = 3
    // ─────────────────────────────────────
    else if(cmd=="UPDATE") {
        if(tokens.size() < 10) {
            q.is_valid = false;
            q.error_message = "Invalid UPDATE. Use: UPDATE table SET col = val WHERE id = n";
            return q;
        }

        q.type           = UPDATE_ROW;
        q.table_name     = tokens[1];
        q.update_column  = tokens[3];
        q.update_value   = tokens[5];
        q.where.column   = tokens[7];
        q.where.op       = tokens[8];
        q.where.value    = tokens[9];

    }

    // ─────────────────────────────────────
    // Unknown command
    // ─────────────────────────────────────
    else {
        q.is_valid = false;
        q.error_message = "Unknown command '" + tokens[0] +
                          "'. Use SELECT, INSERT, DELETE, UPDATE";
    }
    return q;
        
}



