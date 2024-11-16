//
// Created by user on 8/30/24.
//
// commands.cpp
#include "tablemanager.cpp"
#include <unistd.h>
bool isValidType(const std::string& type) {
    for (const auto& validType : structurenames) {
        if (type == validType) return true;
    }
    return false;
}

std::string current_db="none";
std::string temp_dbs;

void showPrompt() {
    cout << "\33[93mDinoDB [" << current_db << "] > ";  // Dynamically update prompt with current database
}

bool validUSE()
{
    if (current_db != "none")
    {
        return true;
    } else
    {
        return false;
    }
}

void subdbcommands(const std::string& db="") {
    std::ifstream dbs("/home/DinoDB/dinodb/DBS/dbfiles/DBS.DBS");
    if(!dbs.is_open())
    {
        cout<<"\33[31mService connected error!,pls wait recconnect!\n";
        subdbcommands();
    }

    string line;
    bool dbhave=false;
    while(getline(dbs,line)) {
        std::string tmpdb=line.substr(line.find(".")+1);
        if(tmpdb==temp_dbs) {
            dbhave=true;
            break;
        }
    }
    if(db!="")
    {
        if(dbhave)
        {
            current_db=temp_dbs;

            cout<<"\33[92m"<<current_db<<" database selected!\n";
            showPrompt();

            string command, command1, command2;

            std::cin >> command;

            std::transform(command.begin(), command.end(), command.begin(),
                          [](unsigned char c) { return std::toupper(c); });
            if(command == "USE"){
                if(check_user_for_grant(grant_username)){
                    vector<std::string> aaas;
                    aaas.push_back("SHOW");
                    if(!uncompatiblegrant(grant_username,aaas)){
                        std::string dbname;
                        cin>>dbname;
                        if (dbname.back() != ';') {
                            cout << "\33[91mSyntax Error!\n";
                            subdbcommands();
                        } else {
                            string temp = dbname.substr(0, dbname.length() - 1);
                            temp_dbs = temp;
                            subdbcommands(temp);

                        }



                    }else
                    {
                        cout<<"\33[208mPermission denied!\n";
                        subdbcommands();
                    }
                }else{
                    cout<<"\33[95mthis user none priviligies!\n";
                    subdbcommands();
                }
            }





else if (command == "INSERT") {
    if (check_user_for_grant(grant_username)) {
        if (checkcompatiblegrant(grant_username, "INSERT")) {

            // "INTO" anahtar kelimesini oku
            std::cin >> command1;
            command1 = trim(command1);
            std::transform(command1.begin(), command1.end(), command1.begin(),
                   [](unsigned char c) { return std::toupper(c); });

            if (command1 == "INTO") {
                std::string tablename;
                std::cin >> tablename;
                tablename = trim(tablename);

                // Sütunlar bölümünü oku
                std::string columns_section;
                std::getline(std::cin, columns_section, ')'); // Kapanış parantezine kadar oku
                columns_section = trim(columns_section) + ")"; // Kapanış parantezini geri ekle

                if (columns_section.front() != '(' || columns_section.back() != ')') {
                    std::cout << "\33[91mSyntax Error! Expected columns in '(...)'\n";
                    subdbcommands();
                    return;
                }

                // Sütunları çıkar ve işle
                std::string columns = columns_section.substr(1, columns_section.length() - 2); // Çevreleyen parantezleri kaldır
                std::vector<std::string> colNames;
                std::stringstream colStream(columns);
                std::string col;
                while (std::getline(colStream, col, ',')) {
                    colNames.push_back(trim(col));
                }

                // "VALUES" anahtar kelimesini oku
                std::string values_keyword;
                std::cin >> values_keyword;
                values_keyword = trim(values_keyword);
                std::transform(values_keyword.begin(), values_keyword.end(), values_keyword.begin(),
                    [](unsigned char c) { return std::toupper(c); });

                if (values_keyword != "VALUES") {
                    std::cout << "\33[91mSyntax Error! Expected 'VALUES' keyword\n";
                    subdbcommands();
                    return;
                }

                // Değerler bölümünü oku
                std::string values_section;
                std::getline(std::cin, values_section, ')'); // Kapanış parantezine kadar oku
                values_section = trim(values_section) + ")"; // Kapanış parantezini geri ekle

                if (values_section.front() != '(' || values_section.back() != ')') {
                    std::cout << "\33[91mSyntax Error! Expected values in '(...)'\n";
                    subdbcommands();
                    return;
                }

                // Değerleri çıkar ve işle
                std::string values = values_section.substr(1, values_section.length() - 2); // Çevreleyen parantezleri kaldır
                std::vector<std::string> valNames;
                std::stringstream valStream(values);
                std::string val;
                while (std::getline(valStream, val, ',')) {
                    valNames.push_back(trim(val));
                }

                // Sütun ve değer sayısını karşılaştır
                if (colNames.size() != valNames.size()) {
                    std::cout << "\33[91mError! Number of columns and values must match\n";
                    subdbcommands();
                    return;
                }

                // Table sınıfından bir örnek oluştur ve insertTableData fonksiyonunu çağır
                Table a(current_db);
                a.insertTableData(tablename, colNames, valNames);

            } else {
                std::cout << "\33[91mSyntax Error! Expected 'INTO' keyword\n";
                subdbcommands();
            }
        } else {
            std::cout << "\33[95mThis user does not have insert privileges!\n";
            subdbcommands();
        }
    } else {
        std::cout << "\33[95mUser privileges not checked!\n";
        subdbcommands();
    }
}




           
else if (command == "ALTER") {
    if (check_user_for_grant(grant_username)) {
        if (checkcompatiblegrant(grant_username, "ALTER")) {
            std::cin >> command1 >> command2;
            std::transform(command1.begin(), command1.end(), command1.begin(), ::toupper);
            command1 = trim(command1);

            if (command1 == "TABLE") {
                std::string tablename = trim(command2);
                std::string alter_cmd;
                std::cin >> alter_cmd;
                std::transform(alter_cmd.begin(), alter_cmd.end(), alter_cmd.begin(), ::toupper);
                alter_cmd = trim(alter_cmd);

                if (alter_cmd == "ADD") {
                    if (validUSE()) {
                        std::string colname, coltype;
                        std::cin >> colname >> coltype;
                        colname = trim(colname);
                        coltype = trim(coltype);

                        if (coltype.back() != ';') {
                            std::cout << "\33[91mSyntax Error! Expected ');'\n";
                        } else {
                            coltype = coltype.substr(0, coltype.length() - 1);
                            if (isValidType(coltype)) {
                                Table a(current_db);
                                a.alterTableAddColumn(tablename, Columns(colname, coltype));
                                subdbcommands();
                            } else {
                                std::cout << "\33[31mThis type: " << coltype << " is unrecognized!!\n";
                                subdbcommands();
                            }
                        }
                    } else {
                        std::cout << "\33[31mNot selected Database!\n";
                        subdbcommands();
                    }
                } else if (alter_cmd == "DROP") {
                    if (validUSE()) {
                        Table a(current_db);
                        std::string colname;
                        std::cin >> colname;
                        colname = trim(colname);

                        if (colname.back() != ';') {
                            std::cout << "\33[91mSyntax Error! Expected ');'\n";
                        } else {
                            colname = colname.substr(0, colname.length() - 1);
                            a.alterTableDropColumn(tablename, colname);
                            subdbcommands();
                        }
                    } else {
                        std::cout << "\33[31mNot selected Database!\n";
                        subdbcommands();
                    }
                } else {
                    std::cout << "\33[31mInvalid command!\n";
                }
            }
        } else {
            std::cout << "\33[95mThis user has no privileges!\n";
            subdbcommands();
        }
    }
}




        else if(command == "SHOW")
        {
            if(check_user_for_grant(grant_username))
            {
                if(checkcompatiblegrant(grant_username,"SHOW")){
                    cin>>command1;
                    std::transform(command1.begin(), command1.end(), command1.begin(),
                   [](unsigned char c) { return std::toupper(c); });
                    if (command1 == "DATABASES;") {
                        dbmanager a;
                        a.Listdb();
                        subdbcommands();
                    }
                    else if (command1 == "TABLES;")
                    {
                        if(validUSE()){
                            Table a(current_db);
                            a.Listtable();
                            subdbcommands();
                        }else
                        {
                            cout<<"\33[31mNot selected Database!\n";
                            subdbcommands();
                        }
                    }else if (command1 == "GRANTS") {
                        cin>>command2;
                        std::transform(command2.begin(), command2.end(), command2.begin(),
                               [](unsigned char c) { return std::toupper(c); });
                        command2=trim(command2);
                        if(command2=="FOR"){
                            std::string username;
                            cin>>username;
                            if (username.back() != ';') {
                                cout << "\33[91mSyntax Error!\n";
                                subdbcommands();
                            } else {
                                username = username.substr(0, username.length() - 1);
                                ListGrandUser(username);
                                subdbcommands();
                            }
                        }
                    }

                    else {
                        cout<<"\33[31mInvalid command!\n";
                    }
                }
            }else{
                cout<<"\33[95mthis user none priviligies!\n";
                subdbcommands();
            }
        }
        else if (command == "SELECT")
        {
            if(check_user_for_grant(grant_username))
            {
                if(checkcompatiblegrant(grant_username,"SELECT")){
                    cin >> command1 >> command2;
                    command1=trim(command1);
                    command2=trim(command2);
                    std::transform(command1.begin(), command1.end(), command1.begin(),
                  [](unsigned char c) { return std::toupper(c); });

                    
                    if (command1 == "*FROM")
                    {
                        Table a(current_db);
                        if(command2.back()!=';') {
                            cout << "\33[91mSyntax Error! Expected ';'.\n";
                        }else {
                            command2 = command2.substr(0, command2.length() - 1);
                            if(validUSE()){
                                a.listTableData(command2);
                                subdbcommands();
                            }else
                            {
                                cout << "\33[31mNot selected Database!\n";
                            }

                        }
                    }else if(command1=="*"){
                        std::transform(command2.begin(), command2.end(), command2.begin(),
                  [](unsigned char c) { return std::toupper(c); });
                        if(command2=="FROM"){
                            std::string ctablename;
                            std::cin>>ctablename;
                            ctablename=trim(ctablename);
                            Table a(current_db);
                        if(ctablename.back()!=';') {
                            cout << "\33[91mSyntax Error! Expected ';'.\n";
                        }else {
                            ctablename = ctablename.substr(0, ctablename.length() - 1);
                            if(validUSE()){
                                a.listTableData(ctablename);
                                subdbcommands();
                            }else
                            {
                                cout << "\33[31mNot selected Database!\n";
                            }

                        }
                        }
                    }
                }
            }else{
                cout<<"\33[95mthis user none priviligies!\n";
                subdbcommands();
            }
        }





        else if (command == "CREATE")
        {
            if(check_user_for_grant(grant_username))
            {
                if(checkcompatiblegrant(grant_username,"CREATE")){
                    cin >> command1;
                    std::transform(command1.begin(), command1.end(), command1.begin(),
                   [](unsigned char c) { return std::toupper(c); });

                    command1=trim(command1);
                    if (command1 == "DATABASE") {
                        cin >> command2;
                        command2=trim(command2);
                        if (command2.back() != ';') {
                            cout << "\33[91mSyntax Error!\n";
                            subdbcommands();

                        } else {
                            string temp = command2.substr(0, command2.length() - 1);
                            dbmanager a;
                           std::string scriptPath = "/home/DinoDB/installation_files/create_directory.sh";

                            // Shell scriptini çalıştırmak için komut oluştur
                            std::string commandlsa = scriptPath + " " + temp;

                            // Əmri icra edin
                            std::system(commandlsa.c_str());

                            // Çalıştırma sonucunu kontrol et

                            a.creatdb(temp);
                            subdbcommands();
                        }
                    }else if(command1=="USER"){
                        string username,passwd;
                        cin>>username>>passwd;
                        username=trim(username);
                        passwd=trim(passwd);
                        connecttodb a;
                        if (passwd.back() != ';') {
                            cout << "\33[91mSyntax Error!\n";
                            subdbcommands();
                        } else {
                            string tmp = passwd.substr(0, passwd.length() - 1);
                            a.userAdd(username, tmp);
                            subdbcommands();
                        }
                    }
else if (command1 == "TABLE")
{
    string tablename;
    cin >> tablename;
    char ch;
    cin >> ch;

    if (ch != '(') {
        cout << "\33[91mSyntax Error! Expected '('.\n";
        subdbcommands();
        return;
    }

    vector<Columns> columns;
    string colname, coltype;
    string line;

    // Bütün sütun təriflərini ')' işarəsinə qədər oxuyuruq
    getline(cin, line, ')'); // ')' simvoluna qədər oxuyur

    // Sonrakı simvolun ';' olduğunu yoxlayırıq
    char semicolon;
    cin >> semicolon;
    if (semicolon != ';') {
        cout << "\33[91mSyntax Error! Expected ';' after ')'.\n";
        subdbcommands();
        return;
    }

    std::istringstream iss(line);
    string token;

    // Vergüllərlə ayrılmış sütun təriflərini emal edirik
    while (getline(iss, token, ',')) {
        token = trim(token);

        // Sütun adını və tipini ayırırıq
        size_t spacePos = token.find(' ');
        if (spacePos == string::npos) {
            cout << "\33[91mSyntax Error! Expected column type for '" << token << "'.\n";
            subdbcommands();
            return;
        }

        colname = trim(token.substr(0, spacePos));
        coltype = trim(token.substr(spacePos + 1));
        std::transform(coltype.begin(), coltype.end(), coltype.begin(),
                       [](unsigned char c) { return std::toupper(c); });

        // Sütun tipinin düzgünlüyünü yoxlayırıq
        if (isValidType(coltype)) {
            columns.push_back(Columns(colname, coltype));
        } else {
            std::cout << "\33[31mThis type: " << coltype << " is unrecognized!!\n";
            subdbcommands();
            return;
        }
    }

    // Verilənlər bazası seçilibsə, cədvəli yaradırıq
    if (validUSE()) {
        Table a(current_db);
        a.creatTable(tablename, columns);
        subdbcommands();
    } else {
        cout << "\33[31mNo database selected!\n";
        subdbcommands();
    }
}



                }
            }
        }


        else if(command == "SET") {

            if(check_user_for_grant(grant_username)){
                if(checkcompatiblegrant(grant_username,"UPDATE")){
                    cin >> command1;
                    std::transform(command1.begin(), command1.end(), command1.begin(),
                             [](unsigned char c) { return std::toupper(c); });
                        command1=trim(command1);
                    if(command1 == "PASSWORD") {
                        string username, password;
                        cin >> username >> password;
                        password=trim(password);
                        username=trim(username);
                        connecttodb a;
                        if (password.back() != ';') {
                            cout << "\33[91mSyntax Error!\n";
                            subdbcommands();
                        } else {
                            string tmp = password.substr(0, password.length() - 1);
                            a.changeUserPassword(username, tmp);
                            subdbcommands();
                        }
                    } else{
                        cout<<"\33[31mInvalid command!\n";
                        subdbcommands();
                    }
                }
            }else{
                cout<<"\33[95mthis user none priviligies!\n";
                subdbcommands();
            }
        }


        else if(command == "DROP") {

            if(check_user_for_grant(grant_username)){
                if(checkcompatiblegrant(grant_username,"DROP")){
                    cin >> command1;
                    command1=trim(command1);
                    std::transform(command1.begin(), command1.end(), command1.begin(),
                             [](unsigned char c) { return std::toupper(c); });

                    if(command1 == "DATABASE") {
                        string dbsname;
                        cin >> dbsname;
                        dbsname = trim(dbsname);
                        if (dbsname.back() != ';') {
                            cout << "\33[91mSyntax Error!\n";
                            subdbcommands();
                        } else {
                            string tmp = dbsname.substr(0, dbsname.length() - 1);

                            dbmanager a;
                            a.Deletedb(tmp);
                            subdbcommands();
                        }
                    }
                    else if(command1=="TABLE")
                    {
                        std::string tablename;
                        std::cin >> tablename;
                        tablename=trim(tablename);
                        if(validUSE())
                        {
                            if (tablename.back() != ';') {
                            cout << "\33[91mSyntax Error!\n";
                            subdbcommands();
                        } else {
                            tablename = tablename.substr(0, tablename.length() - 1);

                            Table a(current_db);
                            a.Deletetable(tablename);
                            subdbcommands();
                        }
                        }else
                        {
                            cout << "\33[31mNot selected Database!\n";
                            subdbcommands();
                        }
                    }
                    else if(command1 == "USER") {
                        string username;
                        cin >> username;
                        username=trim(username);
                        if (username.back() != ';') {
                            cout << "\33[91mSyntax Error!\n";
                            subdbcommands();
                        } else {
                            string tmp = username.substr(0, username.length() - 1);
                            connecttodb a;

                            a.deleteUser(tmp);
                            subdbcommands();

                        }
                    }else{
                        cout<<"\33[31mInvalid command!\n";
                        subdbcommands();
                    }
                }

            }else{
                cout<<"\33[95mthis user none priviligies!\n";
                subdbcommands();
            }
        }


        else if(command == "EXIT;"  ||  command == "QUIT;"  ||  command == "QUIT" ) {
            exit(1);
        }
            else if(command == "EXIT") {
            cout << "\33[91mSyntax Error!\n";
            subdbcommands();
        }



            else if(command == "RESTART" || command == "REBOOT" || command == "RESTART;" || command == "REBOOT;") {
        if(check_user_for_grant(grant_username)) {
        if(checkcompatiblegrant(grant_username, "UPDATE")) {
            // Stop the service
            system("systemctl stop DinoDB");
            cout << "\33[32mDinoDB service is stopping!";
            cout<<"\33[33m"<<"\n";
            // Wait for 10 seconds
            sleep(10);

            // Start the service again
            int result = system("systemctl start DinoDB");
            if(result == 0) {
                cout << "\33[32mDinoDB service restarted successfully!";
                cout<<"\33[33m"<<"\n";
            } else {
                cout << "\33[31mThere was a problem restarting the DinoDB service!\n";
            }
        }
    } else {
        cout << "\33[35mThis user has no privileges!\n";
        subdbcommands();
    }
}


    else if(command=="GRANT"){
         if(check_user_for_grant(grant_username)){
            if(checkcompatiblegrant(grant_username,"GRANT")){
        cin>>command1;
        bool valuen=false;
        command1=trim(command1);
         std::transform(command1.begin(), command1.end(), command1.begin(),
                   [](unsigned char c) { return std::toupper(c); });
        if(command1.back()==','){
            cout << "\33[91mSyntax Error!\n";
        }else{
            if(checkgrands(command1)){
                cin>>command2;
                 command2=trim(command2);
                std::transform(command2.begin(), command2.end(), command2.begin(),
                   [](unsigned char c) { return std::toupper(c); });
                if(command2=="ON"){
                    std::string command3,command4;
                    cin>>command3;
                 command3=trim(command3);
                std::transform(command3.begin(), command3.end(), command3.begin(),
                   [](unsigned char c) { return std::toupper(c); });
                if(command3=="*.*"){

                     cin>>command4;
                 command4=trim(command4);
                std::transform(command4.begin(), command4.end(), command4.begin(),
                   [](unsigned char c) { return std::toupper(c); });
                if(command4=="TO"){
                    std::string testuser;
                    cin>>testuser;
                    if (testuser.back() != ';') {
                cout << "\33[91mSyntax Error!\n";
                subdbcommands();
            } else {
                 testuser = testuser.substr(0, testuser.length() - 1);

                connecttodb s;

                if(s.checkUser(testuser)){
                    addgrantpermission(testuser,command1);
                subdbcommands();
                }else{
                    cout<<testuser<<"\33[31m -user not found\n";
                subdbcommands();
                }


            }
                }else{
            cout<<"\33[31mInvalid command!\n";
            subdbcommands();
        }
                }else{
            cout<<"\33[31mInvalid command!\n";
            subdbcommands();
        }
                }else{
                if(checkgrands(command2)){
                    std::string testuser;
                    cin>>testuser;
                    if (testuser.back() != ';') {
                cout << "\33[91mSyntax Error!\n";
                subdbcommands();
            } else {
                string testuser = testuser.substr(0, testuser.length() - 1);
                connecttodb dsa;

                if(dsa.checkUser(testuser)){
                    addgrantpermission(testuser,command1);
                subdbcommands();
                }else{
                    cout<<testuser<<"\33[31m -user not found\n";
                subdbcommands();
                }
                ChangeGrand(testuser,command2);
                subdbcommands();

            }
                }else{

            cout<<"\33[31mInvalid command!\n";
            subdbcommands();
                }
        }

            }else{
                cout<<"\33[31mWrong permission!\n";
            }
        }
    }
}else {
        cout << "\33[35mThis user has no privileges!\n";
        subdbcommands();
    }
    }







        else{
            cout<<"\33[31mInvalid command!\n";
            subdbcommands();
        }





    }




        else {

        cout<<temp_dbs<<" \33[94mdatabase doesnt exist on Dino Db!\n";
        subdbcommands();
    }
}
    else
    {


            showPrompt();

            string command, command1, command2;

            std::cin >> command;

            std::transform(command.begin(), command.end(), command.begin(),
                          [](unsigned char c) { return std::toupper(c); });
            if(command == "USE"){
                if(check_user_for_grant(grant_username)){
                    vector<std::string> aaas;
                    aaas.push_back("SHOW");
                    if(!uncompatiblegrant(grant_username,aaas)){
                        std::string dbname;
                        cin>>dbname;
                        if (dbname.back() != ';') {
                            cout << "\33[91mSyntax Error!\n";
                            subdbcommands();
                        } else {
                            string temp = dbname.substr(0, dbname.length() - 1);
                            temp_dbs = temp;
                            subdbcommands(temp);

                        }



                    }else
                    {
                        cout<<"\33[208mPermission denied!\n";
                        subdbcommands();
                    }
                }else{
                    cout<<"\33[95mthis user none priviligies!\n";
                    subdbcommands();
                }
            }

            
            
          
else if (command == "INSERT") {
    if (check_user_for_grant(grant_username)) {
        if (checkcompatiblegrant(grant_username, "INSERT")) {

            // "INTO" anahtar kelimesini oku
            std::cin >> command1;
            command1 = trim(command1);
            std::transform(command1.begin(), command1.end(), command1.begin(),
                   [](unsigned char c) { return std::toupper(c); });

            if (command1 == "INTO") {
                std::string tablename;
                std::cin >> tablename;
                tablename = trim(tablename);

                // Sütunlar bölümünü oku
                std::string columns_section;
                std::getline(std::cin, columns_section, ')'); // Kapanış parantezine kadar oku
                columns_section = trim(columns_section) + ")"; // Kapanış parantezini geri ekle

                if (columns_section.front() != '(' || columns_section.back() != ')') {
                    std::cout << "\33[91mSyntax Error! Expected columns in '(...)'\n";
                    subdbcommands();
                    return;
                }

                // Sütunları çıkar ve işle
                std::string columns = columns_section.substr(1, columns_section.length() - 2); // Çevreleyen parantezleri kaldır
                std::vector<std::string> colNames;
                std::stringstream colStream(columns);
                std::string col;
                while (std::getline(colStream, col, ',')) {
                    colNames.push_back(trim(col));
                }

                // "VALUES" anahtar kelimesini oku
                std::string values_keyword;
                std::cin >> values_keyword;
                values_keyword = trim(values_keyword);
                std::transform(values_keyword.begin(), values_keyword.end(), values_keyword.begin(),
                    [](unsigned char c) { return std::toupper(c); });

                if (values_keyword != "VALUES") {
                    std::cout << "\33[91mSyntax Error! Expected 'VALUES' keyword\n";
                    subdbcommands();
                    return;
                }

                // Değerler bölümünü oku
                std::string values_section;
                std::getline(std::cin, values_section, ')'); // Kapanış parantezine kadar oku
                values_section = trim(values_section) + ")"; // Kapanış parantezini geri ekle

                if (values_section.front() != '(' || values_section.back() != ')') {
                    std::cout << "\33[91mSyntax Error! Expected values in '(...)'\n";
                    subdbcommands();
                    return;
                }

                // Değerleri çıkar ve işle
                std::string values = values_section.substr(1, values_section.length() - 2); // Çevreleyen parantezleri kaldır
                std::vector<std::string> valNames;
                std::stringstream valStream(values);
                std::string val;
                while (std::getline(valStream, val, ',')) {
                    valNames.push_back(trim(val));
                }

                // Sütun ve değer sayısını karşılaştır
                if (colNames.size() != valNames.size()) {
                    std::cout << "\33[91mError! Number of columns and values must match\n";
                    subdbcommands();
                    return;
                }

                // Table sınıfından bir örnek oluştur ve insertTableData fonksiyonunu çağır
                Table a(current_db);
                a.insertTableData(tablename, colNames, valNames);

            } else {
                std::cout << "\33[91mSyntax Error! Expected 'INTO' keyword\n";
                subdbcommands();
            }
        } else {
            std::cout << "\33[95mThis user does not have insert privileges!\n";
            subdbcommands();
        }
    } else {
        std::cout << "\33[95mUser privileges not checked!\n";
        subdbcommands();
    }
}





        else if (command == "ALTER") {
    if (check_user_for_grant(grant_username)) {
        if (checkcompatiblegrant(grant_username, "ALTER")) {
            std::cin >> command1 >> command2;
            std::transform(command1.begin(), command1.end(), command1.begin(), ::toupper);
            command1 = trim(command1);

            if (command1 == "TABLE") {
                std::string tablename = trim(command2);
                std::string alter_cmd;
                std::cin >> alter_cmd;
                std::transform(alter_cmd.begin(), alter_cmd.end(), alter_cmd.begin(), ::toupper);
                alter_cmd = trim(alter_cmd);

                if (alter_cmd == "ADD") {
                    if (validUSE()) {
                        std::string colname, coltype;
                        std::cin >> colname >> coltype;
                        colname = trim(colname);
                        coltype = trim(coltype);

                        if (coltype.back() != ';') {
                            std::cout << "\33[91mSyntax Error! Expected ');'\n";
                        } else {
                            coltype = coltype.substr(0, coltype.length() - 1);
                            if (isValidType(coltype)) {
                                Table a(current_db);
                                a.alterTableAddColumn(tablename, Columns(colname, coltype));
                                subdbcommands();
                            } else {
                                std::cout << "\33[31mThis type: " << coltype << " is unrecognized!!\n";
                                subdbcommands();
                            }
                        }
                    } else {
                        std::cout << "\33[31mNot selected Database!\n";
                        subdbcommands();
                    }
                } else if (alter_cmd == "DROP") {
                    if (validUSE()) {
                        Table a(current_db);
                        std::string colname;
                        std::cin >> colname;
                        colname = trim(colname);

                        if (colname.back() != ';') {
                            std::cout << "\33[91mSyntax Error! Expected ');'\n";
                        } else {
                            colname = colname.substr(0, colname.length() - 1);
                            a.alterTableDropColumn(tablename, colname);
                            subdbcommands();
                        }
                    } else {
                        std::cout << "\33[31mNot selected Database!\n";
                        subdbcommands();
                    }
                } else {
                    std::cout << "\33[31mInvalid command!\n";
                }
            }
        } else {
            std::cout << "\33[95mThis user has no privileges!\n";
            subdbcommands();
        }
    }
}

 



        else if(command == "SHOW")
        {
            if(check_user_for_grant(grant_username))
            {
                if(checkcompatiblegrant(grant_username,"SHOW")){
                    cin>>command1;
                    std::transform(command1.begin(), command1.end(), command1.begin(),
                   [](unsigned char c) { return std::toupper(c); });
                    if (command1 == "DATABASES;") {
                        dbmanager a;
                        a.Listdb();
                        subdbcommands();
                    }
                    else if (command1 == "TABLES;")
                    {
                        if(validUSE()){
                            Table a(current_db);
                            a.Listtable();
                            subdbcommands();
                        }else
                        {
                            cout<<"\33[31mNot selected Database!\n";
                            subdbcommands();
                        }
                    }else if (command1 == "GRANTS") {
                        cin>>command2;
                        std::transform(command2.begin(), command2.end(), command2.begin(),
                               [](unsigned char c) { return std::toupper(c); });
                        command2=trim(command2);
                        if(command2=="FOR"){
                            std::string username;
                            cin>>username;
                            if (username.back() != ';') {
                                cout << "\33[91mSyntax Error!\n";
                                subdbcommands();
                            } else {
                                username = username.substr(0, username.length() - 1);
                                ListGrandUser(username);
                                subdbcommands();
                            }
                        }
                    }

                    else {
                        cout<<"\33[31mInvalid command!\n";
                    }
                }
            }else{
                cout<<"\33[95mthis user none priviligies!\n";
                subdbcommands();
            }
        }
         else if (command == "SELECT")
        {
            if(check_user_for_grant(grant_username))
            {
                if(checkcompatiblegrant(grant_username,"SELECT")){
                    cin >> command1 >> command2;
                    command1=trim(command1);
                    command2=trim(command2);
                    std::transform(command1.begin(), command1.end(), command1.begin(),
                  [](unsigned char c) { return std::toupper(c); });

                    
                    if (command1 == "*FROM")
                    {
                        Table a(current_db);
                        if(command2.back()!=';') {
                            cout << "\33[91mSyntax Error! Expected ';'.\n";
                        }else {
                            command2 = command2.substr(0, command2.length() - 1);
                            if(validUSE()){
                                a.listTableData(command2);
                                subdbcommands();
                            }else
                            {
                                cout << "\33[31mNot selected Database!\n";
                            }

                        }
                    }else if(command1=="*"){
                        std::transform(command2.begin(), command2.end(), command2.begin(),
                  [](unsigned char c) { return std::toupper(c); });
                        if(command2=="FROM"){
                            std::string ctablename;
                            std::cin>>ctablename;
                            ctablename=trim(ctablename);
                            Table a(current_db);
                        if(ctablename.back()!=';') {
                            cout << "\33[91mSyntax Error! Expected ';'.\n";
                        }else {
                            ctablename = ctablename.substr(0, ctablename.length() - 1);
                            if(validUSE()){
                                a.listTableData(ctablename);
                                subdbcommands();
                            }else
                            {
                                cout << "\33[31mNot selected Database!\n";
                            }

                        }
                        }
                    }
                }
            }else{
                cout<<"\33[95mthis user none priviligies!\n";
                subdbcommands();
            }
        }


        else if (command == "CREATE")
        {
            if(check_user_for_grant(grant_username))
            {
                if(checkcompatiblegrant(grant_username,"CREATE")){
                    cin >> command1;
                    std::transform(command1.begin(), command1.end(), command1.begin(),
                   [](unsigned char c) { return std::toupper(c); });

                    command1=trim(command1);
                    if (command1 == "DATABASE") {
                        cin >> command2;
                        command2=trim(command2);
                        
                        if (command2.back() != ';') {
                            cout << "\33[91mSyntax Error!\n";
                            subdbcommands();

                        } else {
                            string temp = command2.substr(0, command2.length() - 1);
                            dbmanager a;
                            std::string scriptPath = "/home/DinoDB/installation_files/create_directory.sh";

                            // Shell scriptini çalıştırmak için komut oluştur
                            std::string commandlsa = scriptPath + " " + temp;

                            // Əmri icra edin
                            int result = std::system(commandlsa.c_str());

                            // Çalıştırma sonucunu kontrol et

                            a.creatdb(temp);
                            subdbcommands();
                        }
                    }else if(command1=="USER"){
                        string username,passwd;
                        cin>>username>>passwd;
                        username=trim(username);
                        passwd=trim(passwd);
                        connecttodb a;
                        if (passwd.back() != ';') {
                            cout << "\33[91mSyntax Error!\n";
                            subdbcommands();
                        } else {
                            string tmp = passwd.substr(0, passwd.length() - 1);
                            a.userAdd(username, tmp);
                            subdbcommands();
                        }
                    }



else if (command1 == "TABLE")
{
    string tablename;
    cin >> tablename;
    char ch;
    cin >> ch;

    if (ch != '(') {
        cout << "\33[91mSyntax Error! Expected '('.\n";
        subdbcommands();
        return;
    }

    vector<Columns> columns;
    string colname, coltype;
    string line;

    // Bütün sütun təriflərini ')' işarəsinə qədər oxuyuruq
    getline(cin, line, ')'); // ')' simvoluna qədər oxuyur

    // Sonrakı simvolun ';' olduğunu yoxlayırıq
    char semicolon;
    cin >> semicolon;
    if (semicolon != ';') {
        cout << "\33[91mSyntax Error! Expected ';' after ')'.\n";
        subdbcommands();
        return;
    }

    std::istringstream iss(line);
    string token;

    // Vergüllərlə ayrılmış sütun təriflərini emal edirik
    while (getline(iss, token, ',')) {
        token = trim(token);

        // Sütun adını və tipini ayırırıq
        size_t spacePos = token.find(' ');
        if (spacePos == string::npos) {
            cout << "\33[91mSyntax Error! Expected column type for '" << token << "'.\n";
            subdbcommands();
            return;
        }

        colname = trim(token.substr(0, spacePos));
        coltype = trim(token.substr(spacePos + 1));
        std::transform(coltype.begin(), coltype.end(), coltype.begin(),
                       [](unsigned char c) { return std::toupper(c); });

        // Sütun tipinin düzgünlüyünü yoxlayırıq
        if (isValidType(coltype)) {
            columns.push_back(Columns(colname, coltype));
        } else {
            std::cout << "\33[31mThis type: " << coltype << " is unrecognized!!\n";
            subdbcommands();
            return;
        }
    }

    // Verilənlər bazası seçilibsə, cədvəli yaradırıq
    if (validUSE()) {
        Table a(current_db);
        a.creatTable(tablename, columns);
        subdbcommands();
    } else {
        cout << "\33[31mNo database selected!\n";
        subdbcommands();
    }
}

                  
                    
                
                
                
                
                }
            }
        }


        else if(command == "SET") {

            if(check_user_for_grant(grant_username)){
                if(checkcompatiblegrant(grant_username,"UPDATE")){
                    cin >> command1;
                    std::transform(command1.begin(), command1.end(), command1.begin(),
                             [](unsigned char c) { return std::toupper(c); });
                        command1=trim(command1);
                    if(command1 == "PASSWORD") {
                        string username, password;
                        cin >> username >> password;
                        password=trim(password);
                        username=trim(username);
                        connecttodb a;
                        if (password.back() != ';') {
                            cout << "\33[91mSyntax Error!\n";
                            subdbcommands();
                        } else {
                            string tmp = password.substr(0, password.length() - 1);
                            a.changeUserPassword(username, tmp);
                            subdbcommands();
                        }
                    } else{
                        cout<<"\33[31mInvalid command!\n";
                        subdbcommands();
                    }
                }
            }else{
                cout<<"\33[95mthis user none priviligies!\n";
                subdbcommands();
            }
        }


        else if(command == "DROP") {

            if(check_user_for_grant(grant_username)){
                if(checkcompatiblegrant(grant_username,"DROP")){
                    cin >> command1;
                    command1=trim(command1);
                    std::transform(command1.begin(), command1.end(), command1.begin(),
                             [](unsigned char c) { return std::toupper(c); });

                    if(command1 == "DATABASE") {
                        string dbsname;
                        cin >> dbsname;
                        dbsname = trim(dbsname);
                        if (dbsname.back() != ';') {
                            cout << "\33[91mSyntax Error!\n";
                            subdbcommands();
                        } else {
                            string tmp = dbsname.substr(0, dbsname.length() - 1);

                            dbmanager a;
                            a.Deletedb(tmp);
                            subdbcommands();
                        }
                    }
                    else if(command1=="TABLE")
                    {
                        std::string tablename;
                        std::cin >> tablename;
                        tablename=trim(tablename);
                        if(validUSE())
                        {

                            if (tablename.back() != ';') {
                            cout << "\33[91mSyntax Error!\n";
                            subdbcommands();
                        } else {
                            tablename = tablename.substr(0, tablename.length() - 1);

                            
                            Table a(current_db);
                            a.Deletetable(tablename);
                            subdbcommands();
                        }
                        }else
                        {
                            cout << "\33[31m selected Database!\n";
                            subdbcommands();
                        }
                    }
                    else if(command1 == "USER") {
                        string username;
                        cin >> username;
                        username=trim(username);
                        if (username.back() != ';') {
                            cout << "\33[91mSyntax Error!\n";
                            subdbcommands();
                        } else {
                            string tmp = username.substr(0, username.length() - 1);
                            connecttodb a;

                            a.deleteUser(tmp);
                            subdbcommands();

                        }
                    }else{
                        cout<<"\33[31mInvalid command!\n";
                        subdbcommands();
                    }
                }

            }else{
                cout<<"\33[95mthis user none priviligies!\n";
                subdbcommands();
            }
        }


        else if(command == "EXIT;"  ||  command == "QUIT;"  ||  command == "QUIT" ) {
            exit(1);
        }
            else if(command == "EXIT") {
            cout << "\33[91mSyntax Error!\n";
            subdbcommands();
        }



            else if(command == "RESTART" || command == "REBOOT" || command == "RESTART;" || command == "REBOOT;") {
        if(check_user_for_grant(grant_username)) {
        if(checkcompatiblegrant(grant_username, "UPDATE")) {
            // Stop the service
            system("systemctl stop DinoDB");
            cout << "\33[32mDinoDB service is stopping!";
            cout<<"\33[33m"<<"\n";

            // Wait for 10 seconds
            sleep(10);

            // Start the service again
            int result = system("systemctl start DinoDB");
            if(result == 0) {
                cout << "\33[32mDinoDB service restarted successfully!";
                cout<<"\33[33m"<<"\n";
            } else {
                cout << "\33[31mThere was a problem restarting the DinoDB service!\n";
            }
        }
    } else {
        cout << "\33[35mThis user has no privileges!\n";
        subdbcommands();
    }
}


    else if(command=="GRANT"){
         if(check_user_for_grant(grant_username)){
            if(checkcompatiblegrant(grant_username,"GRANT")){
        cin>>command1;
        bool valuen=false;
        command1=trim(command1);
         std::transform(command1.begin(), command1.end(), command1.begin(),
                   [](unsigned char c) { return std::toupper(c); });
        if(command1.back()==','){
            cout << "\33[91mSyntax Error!\n";
        }else{
            if(checkgrands(command1)){
                cin>>command2;
                 command2=trim(command2);
                std::transform(command2.begin(), command2.end(), command2.begin(),
                   [](unsigned char c) { return std::toupper(c); });
                if(command2=="ON"){
                    std::string command3,command4;
                    cin>>command3;
                 command3=trim(command3);
                std::transform(command3.begin(), command3.end(), command3.begin(),
                   [](unsigned char c) { return std::toupper(c); });
                if(command3=="*.*"){

                     cin>>command4;
                 command4=trim(command4);
                std::transform(command4.begin(), command4.end(), command4.begin(),
                   [](unsigned char c) { return std::toupper(c); });
                if(command4=="TO"){
                    std::string testuser;
                    cin>>testuser;
                    if (testuser.back() != ';') {
                cout << "\33[91mSyntax Error!\n";
                subdbcommands();
            } else {
                 testuser = testuser.substr(0, testuser.length() - 1);

                connecttodb s;

                if(s.checkUser(testuser)){
                    addgrantpermission(testuser,command1);
                subdbcommands();
                }else{
                    cout<<testuser<<"\33[31m -user not found\n";
                subdbcommands();
                }


            }
                }else{
            cout<<"\33[31mInvalid command!\n";
            subdbcommands();
        }
                }else{
            cout<<"\33[31mInvalid command!\n";
            subdbcommands();
        }
                }else{
                if(checkgrands(command2)){
                    std::string testuser;
                    cin>>testuser;
                    if (testuser.back() != ';') {
                cout << "\33[91mSyntax Error!\n";
                subdbcommands();
            } else {
                string testuser = testuser.substr(0, testuser.length() - 1);
                connecttodb dsa;

                if(dsa.checkUser(testuser)){
                    ChangeGrand(testuser,command2);
                subdbcommands();
                }else{
                    cout<<testuser<<"\33[31m -user not found\n";
                subdbcommands();
                }
                
                subdbcommands();

            }
                }else{

            cout<<"\33[31mInvalid command!\n";
            subdbcommands();
                }
        }

            }else{
                cout<<"\33[31mWrong permission!\n";
            }
        }
    }
}else {
        cout << "\33[35mThis user has no privileges!\n";
        subdbcommands();
    }
    }







        else{
            cout<<"\33[31mInvalid command!\n";
            subdbcommands();
        }





    }


}






void Terminal() {

    note();
   
    rootuser();
    connectdb();
    while (true) {
        subdbcommands();
    }


}
void connecta(const std::string database____name){
	
		subdbcommands(database____name);
	

}

