// Include necessary header files for networking and I/O operations
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>     // For close()
#include <sys/socket.h> // For socket operations
#include <netinet/in.h> // For sockaddr_in
#include <arpa/inet.h>  // For inet_pton

int main() {
    // Declare socket file descriptor and server address structure
    int sock_fd;
    struct sockaddr_in server_addr;
    char buffer[1024];          // Buffer to store received data
    char message[1024];         // Buffer for user input

    // Prompt user for input and store it
    printf("Enter your message to be sent: ");
    fgets(message, sizeof(message), stdin);
    message[strcspn(message, "\n")] = 0;  // Remove trailing newline character

    // Create TCP socket
    sock_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (sock_fd < 0) {
        perror("socket error");
        return 1;
    }

    // Initialize server address structure
    memset(&server_addr, 0, sizeof(server_addr));        // Clear structure
    server_addr.sin_family = AF_INET;                    // Use IPv4
    server_addr.sin_port = htons(3000);                  // Set port number
    inet_pton(AF_INET, "127.0.0.1", &server_addr.sin_addr); // Set IP address

    // Attempt to connect to server
    printf("Attempting to connect to server...\n");
    if (connect(sock_fd, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0) {
        perror("connect error");
        return 1;
    }

    // Send message to server
    printf("Connected. Sending message: %s\n", message);
    int sent = send(sock_fd, message, strlen(message), 0);
    if (sent < 0) {
        perror("send error");
        return 1;
    }
    printf("Sent %d bytes\n", sent);

    // Receive echo response from server
    int bytes = recv(sock_fd, buffer, sizeof(buffer) - 1, 0);
    buffer[bytes] = '\0';  // Null terminate received data
    printf("Client message to be sent: %s\n", buffer);

    // Clean up and close socket
    close(sock_fd);
    return 0;
}
