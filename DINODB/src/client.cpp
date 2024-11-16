#include <iostream>
#include <string>
#include <cstring>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>

#define BUFFER_SIZE 1024
#define DEFAULT_PORT 10757 // Defolt port nömrəsi

int main(int argc, char *argv[]) {
    if (argc < 2) {
        std::cerr << "Usage: " << argv[0] << " <server_ip> [port]" << std::endl;
        return -1;
    }

    int sock = 0;
    struct sockaddr_in serv_addr;
    char buffer[BUFFER_SIZE] = {0};

    // Portu argumentdən oxu, yoxsa defolt port istifadə et
    int port = (argc > 2) ? std::stoi(argv[2]) : DEFAULT_PORT;

    // Socket yaradılması
    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        std::cerr << "Socket creation error" << std::endl;
        return -1;
    }

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(port);

    // IPv4 adresini mətn formatından binar formata çevir
    if (inet_pton(AF_INET, argv[1], &serv_addr.sin_addr) <= 0) {
        std::cerr << "Invalid address/ Address not supported" << std::endl;
        return -1;
    }

    // Serverə qoşulma
    if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) {
        std::cerr << "Connection failed" << std::endl;
        return -1;
    }

    std::cout << "Connected to server on port " << port << ". You can start typing commands." << std::endl;

    // Serverdən ilkin cavabı oxu
    int valread = read(sock, buffer, BUFFER_SIZE);
    if (valread > 0) {
        std::cout << buffer;
    } else {
        std::cerr << "Failed to read initial connection message from server" << std::endl;
        close(sock);
        return -1;
    }

    // Prompt mesajını oxu
    memset(buffer, 0, BUFFER_SIZE);
    valread = read(sock, buffer, BUFFER_SIZE);
    if (valread > 0) {
        std::cout << buffer;
    } else {
        std::cerr << "Failed to read prompt message from server" << std::endl;
        close(sock);
        return -1;
    }

    // İstifadəçidən komanda oxu və serverə göndər
    while (true) {
        std::cout << "> ";
        std::string command;
        std::getline(std::cin, command);

        if (command == "exit") {
            break;
        }

        // Komandanı serverə göndər
        std::string command_with_newline = command + "\n";
        if (send(sock, command_with_newline.c_str(), command_with_newline.size(), 0) < 0) {
            std::cerr << "Failed to send command to server" << std::endl;
            continue;
        }

        // Serverdən cavabı oxu
        memset(buffer, 0, BUFFER_SIZE); // Bufferyi təmizlə
        int valread = read(sock, buffer, BUFFER_SIZE);
        if (valread > 0) {
            std::cout << buffer;
        } else {
            std::cerr << "Failed to read response from server" << std::endl;
            break;
        }
    }

    // Socketi bağla
    close(sock);

    return 0;
}
