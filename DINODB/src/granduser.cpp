#include "../include/granduser.h"


#include <sstream>
void addgrantpermission(const std::string& username, const std::string& permission) {
        std::ifstream afile("/home/DinoDB/dinodb/DBS/users/users.dbs");
        if(!afile.is_open()){
            cout<<"\33[31mError: user database not found. Please restart the server.\n";
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
         if (userfound) {
            std::ifstream perms_file_in(permissions_file);
            if(perms_file_in.good()){
        string line2;
        bool userfound2 = false;
        
     while (std::getline(perms_file_in, line2)) {
        // Satırda tab karakterine göre ayırma
        std::size_t tab_pos = line2.find('\t');
        if (tab_pos != std::string::npos) {
            std::string currentuser = line2.substr(0, tab_pos);

            // Kullanıcı adı ile karşılaştırma
            if (currentuser == username) {
                userfound2 = true;
                break; // Kullanıcı bulunduğunda döngüyü kır
            }
        }
    }
    if(userfound2){
        cout<<"\33[31mThis user have a permisions!\n";
    }else{
        std::ofstream perms_file(permissions_file,ios::app);
        std::string line;
        perms_file<< username << "\t" << permission << "\n";
        perms_file.close();
    }
            }else{
        std::ofstream perms_file(permissions_file,ios::app);
        std::string line;
        perms_file<< username << "\t" << permission << "\n";
        perms_file.close();
            }

        
         }else{
            cout << "\33[31mUsername not found in the database!\n";
         }

    }


void ListGrands(){
std::ifstream afile(permissions_file);
if (!afile.is_open()) {
        std::cerr << "\33[31mError  permissions   reading.\n";
        return;
    }
        std::string line;
        while (getline(afile, line)) {
            std::cout<<line<<"\n";
        }
}


void ListGrandUser(const std::string& username) {
    ifstream afile(permissions_file);
    std::string line;
    bool userfound = false;

    while (getline(afile, line)) {
        std::size_t tab_pos = line.find('\t');
        if (tab_pos != std::string::npos) {
            std::string currentuser = line.substr(0, tab_pos);
            if (currentuser == username) {
                cout << line << "\n";
                userfound = true;
                break;
            }
        }
    }
    if (!userfound) {
        cout << "\33[31mNo permissions found for user: " << username << "\n";
    }
    afile.close(); 
}




bool checkcompatiblegrant(const std::string& username, const std::string& action) {
    std::ifstream afile(permissions_file);
    if (!afile.is_open()) {
        std::cerr << "\33[31mError permissions reading.\n";
        return false;
    }

    std::string line;
    while (getline(afile, line)) {
        std::size_t tab_pos = line.find('\t');
        if (tab_pos != std::string::npos) {
            std::string currentuser = line.substr(0, tab_pos);
            if (currentuser == username) {
                std::string current_perm = line.substr(tab_pos + 1);

                if (current_perm == "ALL" || current_perm == "All") {
                    ALL n_perm;
                    for (const auto& x : n_perm.permissions) {
                        if (x == action) {
                            cout << "\33[94mPermission granted.";
                            cout<< "\33[93m"<<"\n";
                            return true;
                        }
                    }
                }
                else if (current_perm == "WRITE") {
                    WRITE n_perm;
                    for (const auto& x : n_perm.permissions) {
                        if (x == action) {
                            cout << "\33[94mPermission granted.";
                            cout<< "\33[93m"<<"\n";
                            return true;
                        }
                    }
                }
                else if (current_perm == "MODIFY") {
                    MODIFY n_perm;
                    for (const auto& x : n_perm.permissions) {
                        if (x == action) {
                            cout << "\33[94mPermission granted.";
                            cout<< "\33[93m"<<"\n";
                            return true;
                        }
                    }
                }
                else if (current_perm == "CREATE") {
                    CREAT n_perm;
                     for (const auto& x : n_perm.permissions) {
                        if (x == action) {
                        cout << "\33[94mPermission granted.";
                            cout<< "\33[93m"<<"\n";
                        return true;
                    }
                }
                }
                else if (current_perm == "SELECT") {
                    SELECT n_perm;
                     for (const auto& x : n_perm.permissions) {
                        if (x == action) {
                        cout << "\33[94mPermission granted.";
                            cout<< "\33[93m"<<"\n";
                        return true;
                    }
                }
                }
                else if (current_perm == "UPDATE") {
                    UPDATE n_perm;
                     for (const auto& x : n_perm.permissions) {
                        if (x == action) {
                        cout << "\33[94mPermission granted.";
                            cout<< "\33[93m"<<"\n";
                        return true;
                    }
                }
                }
                else if (current_perm == "INSERT") {
                    INSERT n_perm;
                     for (const auto& x : n_perm.permissions) {
                        if (x == action) {
                        cout << "\33[94mPermission granted.";
                            cout<< "\33[93m"<<"\n";
                        return true;
                    }
                }
                }
                else if (current_perm == "DELETE") {
                    DELETE n_perm;
                     for (const auto& x : n_perm.permissions) {
                        if (x == action) {
                        cout << "\33[94mPermission granted.";
                            cout<< "\33[93m"<<"\n";
                        return true;
                    }
                }
                }
                else if (current_perm == "DROP") {
                    DROP n_perm;
                   for (const auto& x : n_perm.permissions) {
                        if (x == action) {
                        cout << "\33[94mPermission granted.";
                            cout<< "\33[93m"<<"\n";
                        return true;
                    }
                }
                }
                else if (current_perm == "ALTER") {
                    ALTER n_perm;
                    for (const auto& x : n_perm.permissions) {
                        if (x == action) {
                        cout << "\33[94mPermission granted.";
                            cout<< "\33[93m"<<"\n";
                        return true;
                    }
                }
                }
                else if (current_perm == "SHOW") {
                    SHOW n_perm;
                    for (const auto& x : n_perm.permissions) {
                        if (x == action) {
                        cout << "\33[94mPermission granted.";
                            cout<< "\33[93m"<<"\n";
                        return true;
                    }
                }
                }
                else if (current_perm == "GRANT") {
                    GRANT n_perm;
                     for (const auto& x : n_perm.permissions) {
                        if (x == action) {
                        cout << "\33[94mPermission granted.";
                            cout<< "\33[93m"<<"\n";
                        return true;
                    }
                }
                }
                else if (current_perm == "READ") {
                    READ n_perm;
                    for (const auto& x : n_perm.permissions) {
                        if (x == action) {
                            cout << "\33[94mPermission granted.";
                            cout<< "\33[93m"<<"\n";
                            return true;
                        }
                    }
                }
                else if (current_perm == "SERVICE") {
                    SERVICE n_perm;
                    for (const auto& x : n_perm.permissions) {
                        if (x == action) {
                            cout << "\33[94mPermission granted.";
                            cout<< "\33[93m"<<"\n";
                            return true;
                        }
                    }
                }
            }
        }
    } 
   cout << "\33[94mPermission denied.\n";
    return false;
}



bool uncompatiblegrant(const std::string& username, const std::vector<std::string>& requested_permissions) {
    std::ifstream afile(permissions_file);
    if (!afile.is_open()) {
        std::cerr << "\33[31mError opening permissions file for reading.\n";
        return false;
    }

    std::string line;
    std::string user_permissions;
    bool userfound = false;

    // Kullanıcıyı bul ve izinlerini oku
    while (getline(afile, line)) {
        std::size_t tab_pos = line.find('\t');
        if (tab_pos != std::string::npos) {
            std::string currentuser = line.substr(0, tab_pos);
            if (currentuser == username) {
                user_permissions = line.substr(tab_pos + 1);
                userfound = true;
                break;
            }
        }
    }
    afile.close();

    if (!userfound) {
        std::cerr << "\33[31mUsername not found in the database with permissions.\n";
        
        return false;

    }

    std::istringstream perms_stream(user_permissions);
    std::string perm;
    std::vector<std::string> user_perm_list;
    while (perms_stream >> perm) {
        user_perm_list.push_back(perm);
    }

    // Kullanıcının sadece verilen izinlere sahip olup olmadığını kontrol et
    for (const auto& user_perm : user_perm_list) {
        if (std::find(requested_permissions.begin(), requested_permissions.end(), user_perm) == requested_permissions.end()) {
            // Kullanıcının bir izin listesinde olmayan başka bir izni varsa, false döner
            return false;
        }
    }

    // Kullanıcının izinlerinin hepsi istenen izinler arasında varsa, true döner
    return true;
}


bool checkgrands(const std::string& grands){
    bool found=false;
    for(auto& x:grandssa){
        if(x==grands){
            found=true;
            break;
        }
    }
    if(!found){
        return false;
    }
    else{
        return true;
    }
}

void ChangeGrand(const std::string& username, const std::string& new_permission) {
    // Open the original permissions file for reading
    ifstream perms_file_in(permissions_file);
    if (!perms_file_in) {
        cout << "\33[31mError opening permissions  for reading!\n";
        return;
    }

    
    ofstream perms_temp_out(permissions_temp);
    if (!perms_temp_out) {
        cout << "\33[31mError :temporary  not opening!\n";
        return;
    }

    string line;
    bool userfound = false;

    // Read each line from the original file
    while (getline(perms_file_in, line)) {
        std::size_t tab_pos = line.find('\t');
        if (tab_pos != std::string::npos) {
            std::string currentuser = line.substr(0, tab_pos);
            if (currentuser == username) {
                // Write the updated permission line to the temporary file
                perms_temp_out << username << '\t' << new_permission << "\n";
                userfound = true;
            } else {
                // Write the unchanged line to the temporary file
                perms_temp_out << line << "\n";
            }
        } else {
            perms_temp_out << line << "\n";
        }
    }
    perms_file_in.close();
    perms_temp_out.close();

    // If the user was not found, add them to the temporary file
    if (!userfound) {
        ofstream perms_temp_append(permissions_temp, ios::app);
        if (!perms_temp_append) {
            cout << "\33[31mError opening temporary permissions file for appending!\n";
            return;
        }
        perms_temp_append << username << '\t' << new_permission << "\n";
        perms_temp_append.close();
    }

    // Replace the original permissions file with the temporary file
    if (std::remove(permissions_file.c_str()) != 0) {
        cout << "\33[31mError deleting the original permissions file!\n";
        return;
    }
    if (std::rename(permissions_temp.c_str(), permissions_file.c_str()) != 0) {
        cout << "\33[31mError renaming temporary file to original file name!\n";
        return;
    }
}

bool check_user_for_grant(std::string& username) {
    std::ifstream ff(permissions_file);
    if (!ff.is_open()) {
        std::cerr << "\33[31mError opening permissions file.\n";
        return false;
    }

    std::string line;
    while (getline(ff, line)) {
        size_t tab_pos = line.find("\t");
        if (tab_pos == std::string::npos) {
            continue; 
        }

        std::string current__username = line.substr(0, tab_pos);
        if (current__username == username) {
            ff.close();
            return true;  }
    }

    ff.close();
    return false; 
}