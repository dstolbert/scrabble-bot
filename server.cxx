
// Socket ish
#include <stdio.h>
#include <sys/socket.h>
#include <unistd.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <string.h>

#include <iostream>
#include <string>
#include <map>

#include "Tile.hxx"

using namespace std;

// Map request -> raw, header, body
map<string, string> parseBuffer(char* buffer) {

    // Parse buffer to string
    string parsed(buffer);

    // Extract JSON data
    int firstIdx = -1;
    int lastIdx = parsed.length();
    for (auto i=0; i<parsed.length(); i++) {
        const auto val = parsed[i];

        // Find first opening bracket
        if (firstIdx == -1 && val == '{') {
            firstIdx = i;
        }

        // Find last closing bracket
        else if (firstIdx >= 0 && val == '}') {
            lastIdx = i;
        };
    };

    map<string, string> request;
    request["raw"] = parsed;
    request["header"] = parsed.substr(0, firstIdx + 1);
    request["body"] = parsed.substr(firstIdx, lastIdx - firstIdx + 1);

    return request;
};

// Creates HTTP response from JSON formatted string body
string createResponse(string body) {

    // Construct raw header
    string response = "HTTP/1.1 200 OK \n"
                    "Server: Alpha-Master-Scrabble-Bot \n"
                    "Content-Type: application/json \n";

    // Add content length and spacer between header/body
    string contentLength = "Content-Length: " + to_string(body.length()) + "\n\n";

    // Add everything together
    response.append(contentLength);
    response.append(body);
    
    return response;
};


#define PORT 8080
int main(int argc, char const *argv[])
{
    int server_fd, new_socket; long valread;
    struct sockaddr_in address;
    int addrlen = sizeof(address);

    auto test = Tile("", "test");
    
    string hello = "Hello from server";
    
    // Creating socket file descriptor, reuse if already bound
    int option = 1;
    server_fd = socket(AF_INET, SOCK_STREAM, 0);
    setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &option, sizeof(option));    

    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons( PORT );
    
    memset(address.sin_zero, '\0', sizeof address.sin_zero);
    
    // Bind socket to port
    if (bind(server_fd, (struct sockaddr *)&address, sizeof(address))<0)
    {
        perror("In bind");
        exit(EXIT_FAILURE);
    }
    if (listen(server_fd, 10) < 0)
    {
        perror("In listen");
        exit(EXIT_FAILURE);
    }

    // Listen for incoming requests
    while(1)
    {
        cout << "\n+++++++ Waiting for new connection ++++++++\n\n";
        if ((new_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t*)&addrlen))<0)
        {
            cout << "In accept" << endl;
            exit(EXIT_FAILURE);
        }
        
        char buffer[30000] = {0};
        valread = read( new_socket , buffer, 30000);
        auto req = parseBuffer(buffer);

        cout << "header -> " << req.at("header") << endl;
        cout << "body -> " << req.at("body") << endl;

        string response = createResponse("{tile:" + test.letter + "}");

        write(new_socket, &response[0], response.length());
        close(new_socket);
    }

    return 0;
}