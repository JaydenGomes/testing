#include <iostream>
#include <string>
#include <cstring>      // for memset
#include <unistd.h>     // for close()
#include <arpa/inet.h>  // for inet_addr(), htons(), sockaddr_in
#include <sys/socket.h> // for socket(), connect()

using namespace std;

void client_program() {
    // Create socket
    int client_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (client_socket == -1) {
        cout << "Client socket creation failed" << endl;
        return;
    }

    // Define server address
    sockaddr_in server_address;
    memset(&server_address, 0, sizeof(server_address));
    server_address.sin_family = AF_INET;
    server_address.sin_port = htons(5000);
    server_address.sin_addr.s_addr = inet_addr("127.0.0.1");

    // Connect to the server
    if (connect(client_socket, (sockaddr*)&server_address, sizeof(server_address)) < 0) {
        cout << "Connection failed" << endl;
        close(client_socket);
        return;
    }

    cout << "Connected to server" << endl;

    char message[1024];
    while (true) {
        // Get message from user
        cout << "Enter message: ";
        string input;
        getline(cin, input);
        send(client_socket, input.c_str(), input.length(), 0);

        if (input == "Over") {
            break;
        }

        // Receive response from the server
        int bytes_received = recv(client_socket, message, sizeof(message) - 1, 0);
        if (bytes_received < 0) {
            cout << "Receive failed" << endl;
            break;
        }

        message[bytes_received] = '\0';  // Null-terminate the received string
        cout << "Server received: " << message << endl;
    }

    // Clean up
    close(client_socket);
}

int main() {
    client_program();
    return 0;
}