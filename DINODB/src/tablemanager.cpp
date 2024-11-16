#include <iostream>
#include <string>
#include "dbstruktur.cpp"
#include <iomanip>
#include <fstream>
#include "db.cpp"
using namespace std;


#include <vector>
class Table {
protected:
    string dbname;
    int tableid;

public:


    Table(const std::string& name) :dbname(name),tableid(1){
        //settableid:
        ifstream ids("/home/DinoDB/dinodb/ids/tableid/" + dbname + "_"+"tablesid.ids");
        if (ids.is_open()) {
            ids >> tableid;
            ids.close();
        }
        //cout << "\33[34mTable ID: " << tableid << endl;

    }



void insertTableData(const std::string& tablename, const std::vector<std::string>& columns, const std::vector<std::string>& values) {
    // Yapı dosyasını oku
    std::ifstream strk("/home/DinoDB/dinodb/DBS/databases/" + dbname + "/" + tablename + ".strk");
    if (!strk.is_open()) {
        std::cout << "\33[31mStructure file not found for table " << tablename << ".\n";
        return;
    }

    // Tablo dosyasını aç
    std::ofstream tbl("/home/DinoDB/dinodb/DBS/databases/" + dbname + "/" + tablename + ".tbl", std::ios::app);
    if (!tbl.is_open()) {
        std::cout << "\33[31mFailed to open table file " << tablename << ".\n";
        return;
    }

    std::string line;
    std::vector<std::string> tableColumns;
    while (std::getline(strk, line)) {
        size_t p_index = line.find(" ");
        if (p_index != std::string::npos) {
            tableColumns.push_back(line.substr(0, p_index));
        }
    }

    // Sütun sayısını ve değer sayısını karşılaştır
    if (columns.size() != values.size()) {
        std::cout << "\33[91mError! Number of columns and values must match.\n";
        return;
    }

    // Sütunların sırasını ve değerlerin sırasını kontrol et
    for (size_t i = 0; i < columns.size(); ++i) {
        if (std::find(tableColumns.begin(), tableColumns.end(), columns[i]) == tableColumns.end()) {
            std::cout << "\33[91mError! Column '" << columns[i] << "' does not exist in the table.\n";
            return;
        }
    }

    // En uzun sütun başlığı uzunluğunu bul
    size_t maxColWidth = 0;
    for (const auto& col : tableColumns) {
        maxColWidth = std::max(maxColWidth, col.length());
    }

    // Verileri dosyaya yaz
    std::vector<std::string> data(tableColumns.size(), "");
    for (size_t i = 0; i < columns.size(); ++i) {
        auto it = std::find(tableColumns.begin(), tableColumns.end(), columns[i]);
        if (it != tableColumns.end()) {
            size_t index = std::distance(tableColumns.begin(), it);
            data[index] = values[i];
        }
    }

    for (size_t i = 0; i < data.size(); ++i) {
        if(i==0){
             tbl <<data[i] << "\t";
        }else{
        tbl << std::setw(maxColWidth + 2) << data[i] << "\t";
    }
    }
    tbl << "\n";

    std::cout << "\33[32mValues successfully inserted into table " << tablename << ".\n";
}





    void creatTable(const std::string& tablename, std::vector<Columns>& columns) {
        // Cədvəlin artıq olub-olmadığını yoxlayırıq

        std::ifstream tbl( "/home/DinoDB/dinodb/DBS/databases/" +dbname +"/" + tablename + ".tbl");
        if (tbl.good()) {
            std::cout << "\33[31mTable " << tablename << "\33[31m already exists.\n";
            return;
        }

        // Cədvəl strukturu və cədvəl fayllarını yaradırıq

        std::ofstream strk("/home/DinoDB/dinodb/DBS/databases/" +dbname +"/" + tablename + ".strk");
        std::ofstream tbl_out("/home/DinoDB/dinodb/DBS/databases/" +dbname +"/" + tablename + ".tbl");
std::ofstream dbfile("/home/DinoDB/dinodb/DBS/dbfiles/" +dbname + ".db", std::ios::app);
        if (strk.is_open() && tbl_out.is_open() && dbfile.is_open()) {
            for (const auto& column : columns) {
                // Struktur faylına sütun məlumatlarını yazırıq
                strk << column.name << " " << column.type << "\n";
            }
            
            for (const auto& column : columns) {
                tbl_out << column.name <<"\t\t";
            }
            tbl_out << "\n";
            dbfile<<tableid<<"."<<tablename<<"\n";
            dbfile.close();
            strk.close();
            tbl_out.close();

            std::cout << "\33[32mTable " << tablename << " \33[32mcreated successfully in database " << dbname << ".\n";
        } else {
            std::cout << "\33[31mFailed to create table " << tablename << ".\n";
        }
    }
    void listTableData(const std::string& tablename) {
        std::ifstream tbl("/home/DinoDB/dinodb/DBS/databases/" + dbname + "/" + tablename + ".tbl");
        if (!tbl.is_open()) {
            std::cout << "\33[31mTable " << tablename << "\33[31mdoes not exist.\n";
            return;
        }

        std::string line;
        while (std::getline(tbl, line)) {
            std::cout << line << "\n";
        }
    }
    void Listtable() {
        std::ifstream dbfile("/home/DinoDB/dinodb/DBS/dbfiles/" +dbname + ".db");
        string line;
        int tmp=0;
        if (dbfile.is_open()) {
            while (getline(dbfile, line)) {
                std::string tablename = line.substr(line.find('.') + 1);
                cout << tablename << "\n";
                tmp++;
            }
            dbfile.close();
            if(tmp==0) {
                cout<<"\33[31mNo Table in "<<dbname<<"\33[31m  database!\n";
            }
        } else {
            cout << "\33[31mFailed to open database file " << dbname << ".\n";
        }
    }
   







void alterTableAddColumn(const std::string& tablename, const Columns& column) {
    // File paths
    std::string strk_path = "/home/DinoDB/dinodb/DBS/databases/" + dbname + "/" + tablename + ".strk";
    std::string tbl_path = "/home/DinoDB/dinodb/DBS/databases/" + dbname + "/" + tablename + ".tbl";
    std::string tbl_temp_path = "/home/DinoDB/dinodb/DBS/databases/" + dbname + "/" + tablename + ".temp";

    // Open files
    std::ofstream strk_out(strk_path, std::ios::app);
    std::ifstream tbl_in(tbl_path);
    std::ofstream tbl_out(tbl_temp_path);

    if (strk_out.is_open() && tbl_in.is_open() && tbl_out.is_open()) {
        // Update .strk file
        strk_out << column.name << " " << column.type << "\n";
        strk_out.close();

        // Add new column header to .tbl file
        std::string line;
        std::getline(tbl_in, line); // Read headers

        // Append new column header with two tabs
        tbl_out << line << "\t" << column.name << "\n";

        // Copy existing rows with new alignment
        while (std::getline(tbl_in, line)) {
            tbl_out << line << "\t\n"; // Ensure two-tab alignment for new column
        }

        tbl_in.close();
        tbl_out.close();

        // Replace old .tbl file with the updated one
        std::remove(tbl_path.c_str());
        std::rename(tbl_temp_path.c_str(), tbl_path.c_str());

        std::cout << "\33[32mColumn " << column.name << " added to table " << tablename << ".\n";
    } else {
        std::cout << "\33[31mFailed to add column to table " << tablename << ".\n";
    }
}



void alterTableDropColumn(const std::string& tablename, const std::string& column_name) {
    std::ifstream strk_in("/home/DinoDB/dinodb/DBS/databases/" + dbname + "/" + tablename + ".strk");
    std::ofstream strk_out("/home/DinoDB/dinodb/DBS/databases/" + dbname + "/" + tablename + ".tmp");
    std::ifstream tbl_in("/home/DinoDB/dinodb/DBS/databases/" + dbname + "/" + tablename + ".tbl");
    std::ofstream tbl_out("/home/DinoDB/dinodb/DBS/databases/" + dbname + "/" + tablename + ".tmp2");

    if (strk_in.is_open() && strk_out.is_open() && tbl_in.is_open() && tbl_out.is_open()) {
        std::string line;
        bool found = false;
        int rsay=0;
        int esay;
        // Update the structure file (.strk)
        while (getline(strk_in, line)) {
            rsay++;
            if (line.find(column_name) == std::string::npos) {
                strk_out << line << "\n";
                
            } else {
                esay=rsay;
                found = true;
            }
        }
        strk_in.close();
        strk_out.close();
        std::remove(("/home/DinoDB/dinodb/DBS/databases/" + dbname + "/" + tablename + ".strk").c_str());
        std::rename(("/home/DinoDB/dinodb/DBS/databases/" + dbname + "/" + tablename + ".tmp").c_str(), ("/home/DinoDB/dinodb/DBS/databases/" + dbname + "/" + tablename + ".strk").c_str());

        if (found) {
            // Update the data file (.tbl)
            std::vector<int> column_indexes;
            std::string header_line;
            getline(tbl_in, header_line);
            std::istringstream header_stream(header_line);
            std::string header;
            int index = 0;

            // Identify columns to keep
            while (std::getline(header_stream, header, '\t')) {
                if (header.find(column_name) == std::string::npos) {
                    column_indexes.push_back(index);
                }
                index++;
            }

            // Rewrite data file without the dropped column
            std::string word;
            bool first = true;
            while (getline(tbl_in, line)) {
                std::istringstream line_stream(line);
                int col_index = 0;
                bool first_in_row = true; // To handle tab separation properly

                while (line_stream >> word) {
                    if (std::find(column_indexes.begin(), column_indexes.end(), col_index) != column_indexes.end()) {
                        if (!first_in_row) {
                            tbl_out << "\t\t";
                        }
                        tbl_out << word;
                        first_in_row = false;
                    }
                    col_index++;
                }
                tbl_out << "\n";
            }
            tbl_in.close();
            tbl_out.close();

            std::remove(("/home/DinoDB/dinodb/DBS/databases/" + dbname + "/" + tablename + ".tbl").c_str());
            std::rename(("/home/DinoDB/dinodb/DBS/databases/" + dbname + "/" + tablename + ".tmp2").c_str(), ("/home/DinoDB/dinodb/DBS/databases/" + dbname + "/" + tablename + ".tbl").c_str());

            std::cout << "\33[32mColumn " << column_name << " removed from table " << tablename << ".\n";
        } else {
            tbl_in.close();
            tbl_out.close();
            std::cout << "\33[31mColumn " << column_name << " not found in table " << tablename << ".\n";
        }
    } else {
        std::cout << "\33[31mFailed to remove column from table " << tablename << ".\n";
    }
}





    void Deletetable(const string& tablename) {
        // Tabel faylını silirik
        if (remove(("/home/DinoDB/dinodb/DBS/databases/" +dbname +"/" + tablename + ".tbl").c_str()) == 0) {
           

            // Verilənlər bazasının faylından da həmin cədvəli silirik
            ifstream dbfile("/home/DinoDB/dinodb/DBS/dbfiles/" +dbname + ".db");
            ofstream temp("/home/DinoDB/dinodb/DBS/dbfiles/temp.db");
            string line;
            bool tablefound = false;

            if (dbfile.is_open() && temp.is_open()) {
                while (getline(dbfile, line)) {
                    std::string currenttable = line.substr(line.find('.') + 1);
                    if (currenttable != tablename) {
                        temp << line << "\n";
                    } else {
                        tablefound = true;
                    }
                }
                dbfile.close();
                temp.close();

                // Faylları dəyişdiririk
                remove(("/home/DinoDB/dinodb/DBS/dbfiles/" +dbname + ".db").c_str());
                remove(("/home/DinoDB/dinodb/DBS/databases/" +dbname +"/" + tablename + ".tbl").c_str());
                remove(("/home/DinoDB/dinodb/DBS/databases/" +dbname +"/" + tablename + ".strk").c_str());
                rename("/home/DinoDB/dinodb/DBS/dbfiles/temp.db", ("/home/DinoDB/dinodb/DBS/dbfiles/" +dbname + ".db").c_str());


                if (tablefound) {
                    cout << "\33[32mTable " << tablename << "\33[32m removed from database file successfully.\n";
                } else {
                    cout << "\33[31mTable not found in database file.\n";
                }
            } else {
                cout << "\33[31mFailed to delete table from database file " << dbname << ".\n";
            }

        } else {
            cout << "\33[31mFailed to delete table " << tablename << ".\n";
        }
    }
};

