//dbmanager.cpp
// Created by Natiq on 8/22/2024.
//
// DB manager class
//dbmanager.cpp
#include <iostream>
#include <fstream>
#include <sys/stat.h>  // Linux üçün qovluq yaratmaq üçün
#include <sys/types.h>
#include <sys/wait.h>
using namespace std;


bool createDirectory(const std::string& dirName) {
    // Qovluq yaratmaq
    if (mkdir(dirName.c_str(), 0755) == 0) {  // 0755 icazələri ilə yaradılır
        return true;
    } else {
        perror("mkdir");  // Hata mesajını çap edir
        return false;
    }
}
 int dbid;

using namespace  std;
class dbmanager {
public:
    dbmanager(){
        dbid=1;
        setdbid() ;
    }
    void setdbid() {
        ifstream ids("/home/DinoDB/dinodb/ids/dbid/ddb.ids");
        if (ids.is_open()) {
            ids >> dbid;
            ids.close();
        }
        cout << "\33[94mDB ID: " << "\33[93m"<<dbid<<"\n";
        
    }

    void creatdb(const string& dbname){
        std::ifstream db_check("/home/DinoDB/dinodb/DBS/dbfiles/" + dbname + ".db");


        if (db_check.good()) {
            cout <<"\33[96m"<< dbname << " Database already exists.\n";
            db_check.close();
            return;
        }

        // Fayl yaradılır
        std::ofstream DBS("/home/DinoDB/dinodb/DBS/dbfiles/DBS.DBS", ios::app);
        std::ofstream db("/home/DinoDB/dinodb/DBS/dbfiles/" + dbname + ".db");
        if (db.is_open()) {
            db.close();
            if (DBS.is_open()) {
                DBS << dbid << "." << dbname << "\n";
                cout << dbname << " \33[92mDatabase created successfully!\n";
                dbid++;

                DBS.close();

                // DB ID-ni faylda yenilə
                ofstream ids("/home/DinoDB/dinodb/ids/dbid/ddb.ids", std::ofstream::trunc);
                ids << dbid;
                ids.close();
            } else {
                cout << "\33[31mFailed to create database,error:1 " << dbname << ".\n";
            }
        } else {
            cout << "\33[31mFailed to create database,error:2 " << dbname << ".\n";
        }
    }

    void Listdb(){
        std::string line;
        std::ifstream DBS("/home/DinoDB/dinodb/DBS/dbfiles/DBS.DBS");
        int tmp=0;
        while (getline(DBS, line)) {
            std::string currentdb = line.substr(line.find(".") + 1);
            std::cout << currentdb << "\n";
            tmp++;
        }
        if(tmp==0) {
            cout<<"\33[81mNo Database!\n";
        }
    }

    void Deletedb(const string& dbname) {
        // Silmək üçün fayl
        if (remove(("/home/DinoDB/dinodb/DBS/dbfiles/" + dbname + ".db").c_str()) == 0) {
            cout << dbname << "\33[32mdatabase deleted successfully!\n";
            std::string scriptPath = "/home/DinoDB/installation_files/remove_directory.sh";

            // Shell scriptini çalıştırmak için komut oluştur
            std::string commandlsa = scriptPath + " " + dbname;

            // Əmri icra edin
            int result = std::system(commandlsa.c_str());


            // DBS.DBS faylından da silirik
            ifstream afile("/home/DinoDB/dinodb/DBS/dbfiles/DBS.DBS");
            ofstream bfile("/home/DinoDB/dinodb/DBS/dbfiles/tmp.DBS");
            string line;
            while (getline(afile, line)) {
                string currentdb = line.substr(line.find(".") + 1);
                if (currentdb != dbname) {
                    bfile << line << "\n";
                }
            }
            afile.close();
            bfile.close();

            remove("/home/DinoDB/dinodb/DBS/dbfiles/DBS.DBS");
            rename("/home/DinoDB/dinodb/DBS/dbfiles/tmp.DBS", "/home/DinoDB/dinodb/DBS/dbfiles/DBS.DBS");


        } else {
            cout << "\33[31mFailed to delete database: " << dbname << ".\n";
        }
    }
};

