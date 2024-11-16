//
// Created by user on 8/31/24.
//remotedb.cpp
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <iostream>
#include <cstdlib> // For exit()
#include <cstdio>  // For popen() and pclose()
#include <cstring> // For strerror()
#include <stdexcept> // For runtime_error
#include "commands.cpp"
using namespace std;



/*void command_for_remote(const std::string db_____name,  std::string& grant_username,const  std::string acommand ) {
   

    if(acommand=="SHOW DATABASES;"){
        if (check_user_for_grant(grant_username)) {
            if (checkcompatiblegrant(grant_username, "SHOW")) {
                dbmanager a;
                a.Listdb();
                
    }
        }
    }
    // Ensure the command is an INSERT command
   else if (acommand.find("INSERT") != std::string::npos) {
        if (check_user_for_grant(grant_username)) {
            if (checkcompatiblegrant(grant_username, "INSERT")) {

                // Find "INTO" keyword and then extract everything after it until the first "("
                size_t into_pos = acommand.find("INTO");
                if (into_pos != std::string::npos) {
                    size_t table_name_start = into_pos + 4; // Position after "INTO"
                    size_t table_name_end = acommand.find("(", table_name_start);
                    std::string table_name = acommand.substr(table_name_start, table_name_end - table_name_start);

                    // Trim spaces from table_name
                    table_name.erase(std::remove_if(table_name.begin(), table_name.end(), ::isspace), table_name.end());

                    // Extract the column names (structure)
                    size_t strukturs_start = table_name_end + 1;
                    size_t strukturs_end = acommand.find(")", strukturs_start);
                    std::string strukturs_str = acommand.substr(strukturs_start, strukturs_end - strukturs_start);

                    std::vector<std::string> strukturs;
                    std::stringstream strukturs_stream(strukturs_str);
                    std::string column;
                    while (std::getline(strukturs_stream, column, ',')) {
                        column.erase(std::remove_if(column.begin(), column.end(), ::isspace), column.end()); // Remove any extra spaces
                        strukturs.push_back(column);
                    }

                    // Extract the values
                    size_t values_start = acommand.find("VALUES") + 7;
                    size_t values_end = acommand.find(")", values_start);
                    std::string values_str = acommand.substr(values_start, values_end - values_start);

                    std::vector<std::string> values;
                    std::stringstream values_stream(values_str);
                    std::string value;
                    while (std::getline(values_stream, value, ',')) {
                        value.erase(std::remove_if(value.begin(), value.end(), ::isspace), value.end()); // Remove any extra spaces
                        values.push_back(value);
                    }

                    // Check if the database is selected
                    if (validUSE()) {
                        if (strukturs.size() != values.size()) {
                            std::cout << "Syntax Error: Wrong number of values!\n";
                            return;
                        }

                        for (size_t i = 0; i < strukturs.size(); i++) {
                            if (!checkValueCompatibility(db_____name, table_name, strukturs[i], values[i])) {
                                std::cout << "Error: Value " << values[i] << " is not compatible with column " << strukturs[i] << "!\n";
                                return;
                            }
                        }

                        // If all checks pass, proceed to insert the values into the table
                        Table a(db_____name);
                        a.insertIntoTable(table_name, values);
                    } else {
                        std::cout << "Not selected Database!\n";
                    }
                }
            } else {
                std::cout << "User does not have the necessary INSERT grant.\n";
            }
        } else {
            std::cout << "User grant check failed.\n";
        }
    } else {
        std::cout << "Command is not an INSERT command.\n";
    }
}
*/
bool authentifcation_user(const std::string user,const std::string passwd) {
    string username=user, userpasswd=passwd;


    std::ifstream afile("/home/DinoDB/dinodb/DBS/users/users.dbs");
    string line;
    bool userfound = false;

    while (getline(afile, line)) {
        string currentuser = line.substr(line.find('.') + 1, line.find('\t') - line.find('.') - 1);
        if (currentuser == username) {
            userfound = true;
            string currentpasswd = line.substr(line.find('\t') + 1);
            if (currentpasswd == userpasswd) {

                afile.close();

            } else {
                cout << "User password is wrong!\n";
                afile.close();

            }
        }
    }

    afile.close();

    if (!userfound) {
        cout << "This user doesn't match in the DB's users!\n";
        return false;

    }else {
        return true;
    }


}
bool authentifcation_db(const std::string dbname) {
    bool have=false;
    std::ifstream dbfile("/home/DinoDB/dinodb/DBS/dbfiles/DBS.DBS");
    std::string line;
    while (getline(dbfile,line)) {
        std::string current_db=line.substr(line.find(".")+1);
        if(current_db==dbname) {
            have=true;
            break;
        }
    }
    return have;
}

int main(int argc, char* argv[])
{
    if (argc != 4) {
        cerr << "Usage: " << argv[0] << " <username> <password> <dbname>\n";
        return 1;
    }

    std::string username = argv[1];
    std::string password = argv[2];
    std::string databasename = argv[3];
    //std::string query = argv[4];
    

    if (!authentifcation_user(username, password)) {
        cerr << "Authentication failed for user " << username << ".\n";
        return -1;
    }else
    {
        if (!authentifcation_db(databasename)) {
            cerr << "Database " << databasename << " not found.\n";
            return -1;
        }else{  
                
                
                connectdb(username,password);
                //command_for_remote(databasename,username,query);
                connecta(databasename);

            
               
        }
    }
}


