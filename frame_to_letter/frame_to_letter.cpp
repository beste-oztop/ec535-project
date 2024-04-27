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
    while (file >> character.character >> character.position.x >> character.position.y) {
        characters.push_back(character);
        if (characters.size() >= MAX_CHARS) {
            std::cerr << "Error: Reached maximum number of characters (" << MAX_CHARS << ")" << std::endl;
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
    char closestCharacter = ' ';

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

    while (true) {
        // Get the coordinate from frame every 1 second
        std::this_thread::sleep_for(std::chrono::seconds(1));
        Coordinate input = findRedObjectCoordinates("frame.png");

        // Get the closest character to the input coordinates
        char result = getCharacterFromCoordinates(input, characters, numCharacters);

        cout << "The closest character is: " << result << endl;
        printf("x coord: %d, y coord: %d ",input.x, input.y);
    }

   return 0;
}