#include <iostream>
#include <cstdlib> // For exit()
#include <cstdio>  // For popen() and pclose()
#include <cstring> // For strerror()
#include <stdexcept> // For runtime_error
#include "commands.cpp"

using namespace std;

bool isServerRunning(int port = 10757) {
    char command[256];
    snprintf(command, sizeof(command), "ss -an | grep %d", port);

    FILE* pipe = popen(command, "r");
    if (!pipe) {
        cerr << "Error: Could not run ss command" << endl;
        return false;
    }

    char buffer[128];
    bool portInUse = false;
    while (fgets(buffer, sizeof(buffer), pipe) != nullptr) {
        if (strstr(buffer, "LISTEN") != nullptr) {
            portInUse = true;
            break;
        }
    }

    pclose(pipe);
    return portInUse;
}

extern int run_main() {
    const int port = 10757; // Port number for the DinoDB server

    if (isServerRunning(port)) {
        system("clear"); // Clear the console to mimic starting fresh (Linux only)
        Terminal();    // Start the terminal session
    } else {
        cerr << "Error: The DinoDB server is not running on port " << port << ". Please start the server and try again." << endl;
        return 1; // Exit with an error code
    }

    return 0;
}

int main() {
    //return run_main();
    Terminal();
}
