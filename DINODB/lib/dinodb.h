#ifndef DINODB_H
#define DINODB_H

#include <iostream>
#include <string>
#include <cstring>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <vector>

#define DEFAULT_PORT 10757
#define BUFFER_SIZE 1024

class Dinodb {
private:
    struct Connectionas {
        std::string dbname;
        std::string username;
        std::string password;
        std::string dbhost;
        std::string query;
        int dbport;
    };

    Connectionas* conn;
    int sock;

public:
    Dinodb() : conn(new Connectionas()), sock(-1) {}

    void setConnection(const std::string& host, const std::string& user, const std::string& pass, 
                       const std::string& dbname, const std::string& query, int port = DEFAULT_PORT) {
        conn->dbhost = host;
        conn->username = user;
        conn->password = pass;
        conn->dbname = dbname;
        conn->query = query;
        conn->dbport = port;
    }

    void connect_to_Server() {
        struct sockaddr_in serv_addr = {0};
        char buffer[BUFFER_SIZE] = {0};

        // Socket yaradılması
        if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
            std::cerr << "Socket creation error" << std::endl;
            exit(EXIT_FAILURE);
        }

        serv_addr.sin_family = AF_INET;
        serv_addr.sin_port = htons(conn->dbport);

        // IP adresini yoxlama
        if (inet_pton(AF_INET, conn->dbhost.c_str(), &serv_addr.sin_addr) <= 0) {
            std::cerr << "Invalid address or address not supported" << std::endl;
            exit(EXIT_FAILURE);
        }

        // Serverə qoşulma
        if (connect(sock, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) < 0) {
            std::cerr << "Connection failed" << std::endl;
            exit(EXIT_FAILURE);
        }

        std::cout << "Connected to server." << std::endl;

        sendCommand("remotedb " + conn->username + " " + conn->password + " " + conn->dbname);
        executeQuery();
    }

    void sendCommand(const std::string& command) {
        if (send(sock, command.c_str(), command.size(), 0) < 0) {
            std::cerr << "Failed to send command: " << command << std::endl;
        } else {
            std::cout << "Command sent: " << command << std::endl;
        }
    }

    void executeQuery() {
        std::vector<std::string> commands = {
            "USE " + conn->dbname + ";",
            conn->query
        };

        char buffer[BUFFER_SIZE] = {0};
        for (const auto& cmd : commands) {
            sendCommand(cmd);

            int valread = read(sock, buffer, BUFFER_SIZE);
            if (valread > 0) {
                buffer[valread] = '\0';
                std::cout << "Server response: " << buffer << std::endl;
            } else {
                std::cerr << "Failed to read response from server" << std::endl;
                break;
            }
        }
    }

    ~Dinodb() {
        if (sock >= 0) {
            close(sock);
        }
        delete conn;
    }
};

#endif // DINODB_H
