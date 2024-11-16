#include <iostream>
#include <cstring>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <cstdlib>

#define PORT 10757
#define BUFFER_SIZE 1024

void run_terminal_process(int client_socket) {
    pid_t pid = fork();
    if (pid < 0) {
        perror("fork failed");
        exit(EXIT_FAILURE);
    }

    if (pid == 0) {  // Child process
        // Redirect stdin, stdout, and stderr of terminal to client socket
        if (dup2(client_socket, STDIN_FILENO) < 0 ||
            dup2(client_socket, STDOUT_FILENO) < 0 ||
            dup2(client_socket, STDERR_FILENO) < 0) {
            perror("dup2 failed");
            exit(EXIT_FAILURE);
        }

        // Execute terminal program
        execlp("./terminal", "terminal", (char*)NULL);
        perror("execlp failed");
        exit(EXIT_FAILURE);
    } else {  // Parent process
        // Close the client socket
        close(client_socket);

        // Wait for the child process to finish
        wait(NULL);
    }
}

int main() {
    int server_fd, new_socket;
    struct sockaddr_in address;
    int addrlen = sizeof(address);

    // Create socket file descriptor
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
        perror("socket failed");
        exit(EXIT_FAILURE);
    }

    // Define address and port
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);

    // Bind socket to the address and port
    if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0) {
        perror("bind failed");
        close(server_fd);
        exit(EXIT_FAILURE);
    }

    // Listen for incoming connections
    if (listen(server_fd, 3) < 0) {
        perror("listen failed");
        close(server_fd);
        exit(EXIT_FAILURE);
    }

    std::cout << "Server listening on port " << PORT << std::endl;

    while (true) {
        // Accept incoming connection
        if ((new_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t*)&addrlen)) < 0) {
            perror("accept failed");
            continue;
        }

        std::cout << "Client connected" << std::endl;

        // Send connection success message
        const char *success_msg = "Successfully connected to Dino DB!\n";
        if (send(new_socket, success_msg, strlen(success_msg), 0) < 0) {
            perror("send failed");
        }

        // Send prompt message
        const char *prompt_msg = "Please enter your command:\n";
        if (send(new_socket, prompt_msg, strlen(prompt_msg), 0) < 0) {
            perror("send failed");
        }

        // Run terminal process
        run_terminal_process(new_socket);
    }

    // Close the server socket
    close(server_fd);

    return 0;
}
