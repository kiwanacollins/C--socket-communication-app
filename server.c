// Include necessary header files for networking and I/O operations
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>     // For close()
#include <sys/socket.h> // For socket operations
#include <netinet/in.h> // For sockaddr_in

int main() {
    // Declare variables for server and client socket file descriptors
    int server_fd, client_fd;
    struct sockaddr_in address;     // Structure for handling internet addresses
    socklen_t addr_len = sizeof(address);

    // Create TCP socket
    server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (server_fd < 0) {
        perror("socket error");
        return 1;
    }

    // Set socket option to reuse address (prevents "Address already in use" error)
    int optval = 1;
    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof(optval)) < 0) {
        perror("setsockopt error");
        return 1;
    }

    // Initialize server address structure
    memset(&address, 0, sizeof(address));           // Clear structure
    address.sin_family = AF_INET;                   // Use IPv4
    address.sin_addr.s_addr = INADDR_ANY;          // Accept connections from any IP
    address.sin_port = htons(3000);                // Set port number

    // Bind socket to specified IP and port
    printf("Server starting on port 3000...\n");
    if (bind(server_fd, (struct sockaddr*)&address, sizeof(address)) < 0) {
        perror("bind error");
        return 1;
    }

    // Start listening for connections
    printf("Bound to port. Listening...\n");
    listen(server_fd, 1);  // Allow only one connection in queue

    // Accept incoming connection
    printf("Waiting for client connection...\n");
    client_fd = accept(server_fd, (struct sockaddr*)&address, &addr_len);
    if (client_fd < 0) {
        perror("accept error");
        return 1;
    }

    // Receive data from client
    printf("Client connected. Waiting for data...\n");
    char buffer[1024];
    int bytes = recv(client_fd, buffer, sizeof(buffer) - 1, 0);
    if (bytes < 0) {
        perror("recv error");
        return 1;
    }
    printf("Received %d bytes: %s\n", bytes, buffer);
    
    // Echo received data back to client
    send(client_fd, buffer, bytes, 0);

    // Clean up and close sockets
    close(client_fd);
    close(server_fd);
    return 0;
}
