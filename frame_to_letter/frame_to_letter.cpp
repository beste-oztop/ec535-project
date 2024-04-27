/*
***********
BU EC535 SPRING'24 
Term Project
AUTHORS: 
AMIN KHODAVERDIAN
BESTE OZTOP
SINA KARIMI
***********
*/

#include <iostream>
#include <fstream>
#include <cmath>
#include <vector>
#include <chrono>
#include <thread>
#include <arpa/inet.h> // inet_addr()
#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h> // bzero()
#include <sys/socket.h>
#include <unistd.h> // read(), write(), close()

#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"

using namespace cv;
using namespace std;

/*
Struct to represent the coordinate, read from the camera
*/
struct Coordinate {
    int x;
    int y;
};

/*
Struct to represent the characters and their corresponding coordinates
*/
struct CharacterData {
    char character;
    Coordinate position;
};


#define MAX_CHARS 100 // We have no more than 100 chars
#define PORT 8080
#define SA struct sockaddr

/*
Function to open the file, read its contents into a struct, and return the struct
*/
vector<CharacterData> readCharactersFromFile(const std::string& filename) {
    std::ifstream file(filename);
    std::vector<CharacterData> characters;

    // Check if the file is opened successfully
    if (!file) {
        std::cerr << "Error: Unable to open the file." << std::endl;
        return characters; // Empty vector
    }

    CharacterData character;
    string charStr;
    while (file >> charStr >> character.position.x >> character.position.y) {

        // Handling special characters
        if (charStr == "space") {
            character.character = ' ';
        } else if (charStr == "tab") {
            character.character = '\t';
        } else if (charStr == "enter") {
            character.character = '\n';
        } else if (charStr.size() == 1) {
            character.character = charStr[0];
        } else {
            cerr << "Invalid character: " << charStr << endl;
            continue;
        }

        characters.push_back(character);
        if (characters.size() >= MAX_CHARS) {
            cerr << "Error: Reached maximum number of characters (" << MAX_CHARS << ")" << endl;
            break;
        }
    }

    file.close();
    return characters;
}



/*
A helper function to determine the characters read from the coordinates
*/
char getCharacterFromCoordinates(Coordinate input, vector<CharacterData> characters, int numCharacters) {
    double minDistance = INFINITY;
    char closestCharacter;

    for (int i = 0; i < numCharacters; i++) {
        // Calculate the distance between the input coordinates and each character's position
        double distance = std::sqrt(std::pow(input.x - characters[i].position.x, 2) + std::pow(input.y - characters[i].position.y, 2));
        
        // If the distance is smaller than the current minimum distance, update the closest character
        if (distance < minDistance) {
            minDistance = distance;
            closestCharacter = characters[i].character;
        }
    }

    return closestCharacter;
}



Coordinate findRedObjectCoordinates(const std::string& filename) {
    cv::Mat image = cv::imread(filename);

    // The following hue, saturation and value are for "red" color.
    int hue_low = 170;
    int hue_high = 179;

    int sat_low = 150; 
    int sat_high = 255;

    int val_low = 60;
    int val_high = 255;

    // Keeping track of the coordinate - our main job!
    int x_pos = -1; 
    int y_pos = -1;

    // Convert the image from BGR to HSV
    Mat imgHSV; 
    cvtColor(image, imgHSV, COLOR_BGR2HSV); 

    // Threshold the image to isolate red objects
    Mat imgThresholded;
    inRange(imgHSV, Scalar(hue_low, sat_low, val_low), Scalar(hue_high, sat_high, val_high), imgThresholded); 

    // Perform morphological operations to clean up the thresholded image
    morphologyEx(imgThresholded, imgThresholded, MORPH_OPEN, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)));
    morphologyEx(imgThresholded, imgThresholded, MORPH_CLOSE, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)));

    // Calculate the moments of the thresholded image
    Moments oMoments = moments(imgThresholded);
    // Store the black and white image
    imwrite("output_red.png", imgThresholded);


    double dM01 = oMoments.m01;
    double dM10 = oMoments.m10;
    double dArea = oMoments.m00;

    // Calculate the coordinates
    if (dArea > 0) {
        x_pos = dM10 / dArea;
        y_pos = dM01 / dArea;
    }

    Coordinate coordinate = {x_pos, y_pos};

    return coordinate;
}


/*
Function to open the socket and connect to the server
*/

int openSocketAndConnect() {
    int sockfd;
    struct sockaddr_in servaddr;

    // socket create and verification
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd == -1) {
        cerr << "socket creation failed..." << endl;
        return -1;
    }
    else {
        cout << "Socket successfully created.." << endl;
    }

    bzero(&servaddr, sizeof(servaddr));

    // assign IP, PORT
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = inet_addr("10.239.121.109");
    servaddr.sin_port = htons(PORT);

    // connect the client socket to server socket
    if (connect(sockfd, (SA*)&servaddr, sizeof(servaddr)) != 0) {
        cerr << "connection with the server failed..." << endl;
        close(sockfd);
        return -1;
    }
    else {
        cout << "connected to the server.." << endl;
    }

    return sockfd;
}

int main( int argc, char** argv )
 {   
    // Read characters from file
    std::vector<CharacterData> characters = readCharactersFromFile("simple_coordinates.txt");
    int numCharacters = characters.size();

    /*
    // Checkpoint: print the characters and coordinates read from the text file
    for (const auto& character : characters) {
        std::cout << "Character: " << character.character << ", Coordinates: (" 
                  << character.position.x << ", " << character.position.y << ")" << std::endl;
    }
    */

    // Open socket and connect
    int sockfd = openSocketAndConnect();
    if (sockfd == -1) {
        cerr << "Failed to open socket and connect." << endl;
        return 1;
    }


    while (true) {
        // Get the coordinate from frame every 1 second
        std::this_thread::sleep_for(std::chrono::seconds(1));
        Coordinate input = findRedObjectCoordinates("frame.png");
        int x_pos = input.x;
        int y_pos = input.y;

        // Get the closest character to the input coordinates
        char result = getCharacterFromCoordinates(input, characters, numCharacters);

        cout << "The closest character is: " << result << endl;
        printf("x coord: %d, y coord: %d ",x_pos, y_pos);

        char message[100];
        sprintf(message, "%c", result);

        // Send the message to server
        if (send(sockfd, message, strlen(message), 0) < 0) {
            perror("Send failed");
            close(sockfd);
            return 1;
        }

    }

    // close the socket
    close(sockfd);

   return 0;
}