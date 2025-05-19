#include <iostream>
#include <string>
#include <cstring>      // For memset
#include <unistd.h>     // For close()
#include <arpa/inet.h>  // For htons(), inet_ntoa(), etc.
#include <sys/socket.h> // For socket functions
#include <netinet/in.h> // For sockaddr_in

using namespace std;

void server_program() {
    // Create socket
    int server_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (server_socket == -1) {
        cout << "Server socket creation failed" << endl;
        return;
    }

    // Define server address
    sockaddr_in server_address;
    memset(&server_address, 0, sizeof(server_address));
    server_address.sin_family = AF_INET;
    server_address.sin_addr.s_addr = INADDR_ANY;
    server_address.sin_port = htons(5000);

    // Bind socket to the address
    if (bind(server_socket, (sockaddr*)&server_address, sizeof(server_address)) < 0) {
        cout << "Bind failed" << endl;
        close(server_socket);
        return;
    }

    // Listen for incoming connections
    if (listen(server_socket, 1) < 0) {
        cout << "Listen failed" << endl;
        close(server_socket);
        return;
    }

    cout << "Server waiting for client..." << endl;

    // Accept incoming client connection
    sockaddr_in client_address;
    socklen_t client_address_len = sizeof(client_address);
    int client_socket = accept(server_socket, (sockaddr*)&client_address, &client_address_len);
    if (client_socket < 0) {
        cout << "Client connection failed" << endl;
        close(server_socket);
        return;
    }

    cout << "Client connected" << endl;

    char message[1024];
    while (true) {
        // Receive message from the client
        int bytes_received = recv(client_socket, message, sizeof(message) - 1, 0);
        if (bytes_received < 0) {
            cout << "Receive failed" << endl;
            break;
        }
        message[bytes_received] = '\0';  // Null-terminate the received string
        cout << "Client sent: " << message << endl;

        if (string(message) == "Over") {
            break;
        }

        // Send response to the client
        cout << "Enter response: ";
        string response;
        getline(cin, response);
        send(client_socket, response.c_str(), response.length(), 0);
    }

    // Clean up
    close(client_socket);
    close(server_socket);
}

int main() {
    server_program();
    return 0;
}