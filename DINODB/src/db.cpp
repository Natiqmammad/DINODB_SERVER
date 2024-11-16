//db.cpp
#include <iostream>
#include <fstream>
#include <cerrno>
#include <string>
#include "dbmanager.cpp"
#include "granduser.cpp"
#include <algorithm> // For std::transform
#include <cctype>  
using namespace std;

void connecta(const std::string);
 int userid;

std::string grant_username;

class connecttodb {
    int userid;

public:
    connecttodb ()  {
        userid=1;
        setuserid();
    }
    void setuserid() {
        ifstream ids("/home/DinoDB/dinodb/ids/userid/userids.ids");
        if (ids.is_open()) {
            ids >> userid;
            ids.close();
        }
        cout << "\33[220mUser ID: " <<"\33[220m"<<userid<<"\n" ;
        

    }

    void userAdd(const std::string& username, const std::string& passwd) {
        if (userid < 100) {
            ofstream users_passwd("/home/DinoDB/dinodb/DBS/users/users.dbs", ios::app);
            users_passwd << userid << "." << username << "\t" << passwd << "\n";
            users_passwd.close();

            userid++;
            ofstream ids("/home/DinoDB/dinodb/ids/userid/userids.ids", std::ofstream::trunc);
            ids << userid;
            ids.close();

            cout << "\33[32mUser " << "\33[32m"<<username << " created successfully.";
            cout<<"\33[33m"<<"\n";
        } else {
            cout << "\33[31mUser limit reached! Cannot add more users.\n";
            cout<<"\33[33m"<<"\n";
        }
    }
    void Listusers(){
        ifstream afile("/home/DinoDB/dinodb/DBS/users/users.dbs");
        string line;
        while (getline(afile, line)) {
            string currentuser = line.substr(line.find('.') + 1, line.find('\t') - line.find('.') - 1);
            cout<<currentuser<<"\n";
        }
    }

     bool checkUser(const std::string& username) {
        ifstream afile("/home/DinoDB/dinodb/DBS/users/users.dbs");
        if (!afile.is_open()) {
            cout << "\33[31mError opening users database file!\n";
            return false;
        }

        string line;
        bool userfound = false;
        while (getline(afile, line)) {
            string currentuser = line.substr(line.find('.') + 1, line.find('\t') - line.find('.') - 1);
            if (currentuser == username) {
                userfound = true;
                break;
            }
        }
        afile.close();

        if (userfound) {
            return true;
        } else {
            return false;
        }
    }
    void changeUserPassword(const std::string& username, const std::string& newpasswd) {
        ifstream afile("/home/DinoDB/dinodb/DBS/users/users.dbs");
        ofstream bfile("/home/DinoDB/dinodb/DBS/users/temp.dbs");
        string line;
        bool userfound = false;

        while (getline(afile, line)) {
            string currentuser = line.substr(line.find('.') + 1, line.find('\t') - line.find('.') - 1);
            if (currentuser == username) {
                string tmpid = line.substr(0, line.find('.'));
                bfile << tmpid << "." << username << "\t" << newpasswd << "\n";
                userfound = true;
            } else {
                bfile << line << "\n";
            }
        }

        afile.close();
        bfile.close();

        if (userfound) {
            remove("/home/DinoDB/dinodb/DBS/users/users.dbs");
            rename("/home/DinoDB/dinodb/DBS/users/temp.dbs", "/home/DinoDB/dinodb/DBS/users/users.dbs");
            cout <<"\33[32m" <<username << " password changed successfully!";
            cout<<"\33[33m"<<"\n";

        } else {
            remove("/home/DinoDB/dinodb/DBS/users/temp.dbs");
            cout << "\33[31mUsername not found in the database!\n";
        }
    }
    void deleteUser(const std::string& username) {
        if (username == "root") {
            cout << "\33[31mRoot user cannot be deleted!\n";
            // Assume commands() is a function that shows the command menu or similar

            return;
        }

        // Step 1: Remove the user from the users database
        ifstream afile("/home/DinoDB/dinodb/DBS/users/users.dbs");
        ofstream bfile("/home/DinoDB/dinodb/DBS/users/temp.dbs");

        if (!afile.is_open() || !bfile.is_open()) {
            cout << "\33[34mError pls reconnect\n";
            return;
        }

        string line;
        bool userfound = false;
        while (getline(afile, line)) {
            std::string currentuser = line.substr(line.find('.') + 1, line.find('\t') - line.find('.') - 1);
            if (currentuser == username) {
                userfound = true;
                // Skip writing this user to the temp file
            } else {
                bfile << line << "\n";
            }
        }
        afile.close();
        bfile.close();

        // Step 2: Remove the user's permissions from the permissions file
        ifstream cfile(permissions_file);
        ofstream dfile(permissions_temp);

        if (!cfile.is_open() || !dfile.is_open()) {
            cout << "\33[34mError pls recconect!\n";
            return;
        }

        string line2;
        bool userfound2 = false;
        while (getline(cfile, line2)) {
            std::string currentuser = line2.substr(0, line2.find('\t'));
            if (currentuser == username) {
                userfound2 = true;
                // Skip writing this user's permissions to the temp file
            } else {
                dfile << line2 << "\n";
            }
        }
        cfile.close();
        dfile.close();

        // Step 3: Replace original files with updated files
        if (userfound) {
            // Remove the old files and rename temp files
            std::remove("/home/DinoDB/dinodb/DBS/users/users.dbs");
            std::remove(permissions_file.c_str());
            std::rename("/home/DinoDB/dinodb/DBS/users/temp.dbs", "/home/DinoDB/dinodb/DBS/users/users.dbs");
            std::rename(permissions_temp.c_str(), permissions_file.c_str());
            cout <<"\33[32m"<< username << " successfully deleted!";
            cout<<"\33[33m"<<"\n";
        } else {
            std::remove("/home/DinoDB/dinodb/DBS/users/temp.dbs");
            std::remove(permissions_temp.c_str());
            cout << "\33[31mUsername not found in the database!\n";
        }
    }
};
    int connectdb( std::string user___name="", std::string user___password="") {
        if(user___name=="" && user___password==""){
        cout << "Please enter username and password!\n";
        string username, userpasswd;
        cout<<"Username:";
        cin >> username;
        if(username=="exit;" || username=="EXIT;" || username=="quit;" || username=="QUIT;" ){
            exit(1);
        }
        cout<<"\n";
        cout<<"Password:";
        cin >> userpasswd;

        ifstream afile("/home/DinoDB/dinodb/DBS/users/users.dbs");
        string line;
        bool userfound = false;

        while (getline(afile, line)) {
            string currentuser = line.substr(line.find('.') + 1, line.find('\t') - line.find('.') - 1);
            if (currentuser == username) {
                userfound = true;
                string currentpasswd = line.substr(line.find('\t') + 1);
                if (currentpasswd == userpasswd) {
                    cout << "\33[32mSuccessful login!";
                    cout<<"\33[33m"<<"\n";
                    grant_username=username;
                    afile.close();



                    return 0;
                } else {
                    cout << "\33[31mPassword wrong!\n";
                    afile.close();
                    connectdb();
                }
            }
        }

        afile.close();

        if (!userfound) {
            cout << "\33[31mThis user doesn't match in the DB!\n";
            connectdb();

        }

        return 0;
    }

    else{
        ifstream afile("/home/DinoDB/dinodb/DBS/users/users.dbs");
        string line;
        bool userfound = false;

        while (getline(afile, line)) {
            string currentuser = line.substr(line.find('.') + 1, line.find('\t') - line.find('.') - 1);
            if (currentuser == user___name) {
                userfound = true;
                string currentpasswd = line.substr(line.find('\t') + 1);
                if (currentpasswd == user___password) {
                    cout << "\33[32mSuccessful login!";
                    cout<<"\33[33m"<<"\n";
                    grant_username=user___name;
                   
                    afile.close();



                    return 0;
                } else {
                    cout << "\33[31mPassword wrong!\n";
                    afile.close();
                    connectdb();
                    
                }
            }
        }

        afile.close();

        if (!userfound) {
            cout << "\33[31mThis user doesn't match in the DB!\n";
            connectdb();

        }

        return 0;
    }
    }
    void rootuser() {
        std::ifstream testroot("/home/DinoDB/dinodb/DBS/users/users.dbs");

        // Fayl mövcud deyilsə, avtomatik olaraq yaradılır
        if (!testroot.is_open()) {
            std::ofstream createfile("/home/DinoDB/dinodb/DBS/users/users.dbs");
            if (!createfile.is_open()) {
                cout << "\33[31mFailed to create file for users \n";
                rootuser();
            }
            createfile.close();
        }

        // Fayl oxunur
        if (!testroot.is_open()) {
            cout << "\33[31mFailed starting,pls wait recconect!\n";
            rootuser();
            return ;
        }

        std::string line;
        bool root_have = false;


        while (getline(testroot, line)) {
            std::string currentuser = line.substr(line.find('.') + 1, line.find('\t') - line.find('.') - 1);
            if (currentuser == "root") {
                root_have = true;
                break;
            }
        }

        testroot.close();



        testroot.close();
        if(!root_have){
            connecttodb a;
            a.userAdd("root","root");
            addgrantpermission("root","ALL");
        }
    }

void note() {
    cout << "\33[93m**********************************************\n";
    cout << "\33[93m*                                            *\n";
    cout << "\33[93m*         Welcome to Dino DB!                *\n";
    cout << "\33[93m*     Your Ultimate Database Solution        *\n";
    cout << "\33[93m*                                            *\n";
    cout << "\33[92m*         Created by: Natiq Memmedov         *\n";
    cout << "\33[96m*    All rights reserved to Dino Community   *\n";
    cout << "\33[96m*    For support, visit: www.dinodb.com      *\n";
    cout << "\33[95m*           Community © 2024                 *\n";
    cout << "\33[93m**********************************************\n";
}
