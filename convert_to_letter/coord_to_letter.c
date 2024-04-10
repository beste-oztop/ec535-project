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

#include <stdio.h>
#include <math.h>

/*
Struct to represent the coordinate, read from the camera
*/
typedef struct {
    int x;
    int y;
} Coordinate;

/*
Struct to represent the characters and their corresponding coordinates
*/
typedef struct {
    char character;
    Coordinate position;
} CharacterData;

#define MAX_CHARS 100 // We have no more than 100 chars


/*
A helper function to determine the characters read from the coordinates
*/
char getCharacterFromCoordinates(Coordinate input, CharacterData characters[], int numCharacters) {
    double minDistance = INFINITY;
    char closestCharacter = ' ';

    for (int i = 0; i < numCharacters; i++) {
        // Calculate the distance between the input coordinates and each character's position
        double distance = sqrt(pow(input.x - characters[i].position.x, 2) + pow(input.y - characters[i].position.y, 2));
        
        // If the distance is smaller than the current minimum distance, update the closest character
        if (distance < minDistance) {
            minDistance = distance;
            closestCharacter = characters[i].character;
        }
    }

    return closestCharacter;
}

int main() {

    FILE *fptr; 
    char* fileName = "simple_coords.txt";

    fptr = fopen("simple_coords.txt", "r");

    // Check if the pointer works correctly
    if (fptr == NULL) {
    perror("fopen");
    return 1;
    }

    CharacterData characters[MAX_CHARS];
    int num_chars = 0;

    // Store each letter with its corresponding coordinate in the characters struct list
    while (fscanf(fptr, "%c %d %d\n", &characters[num_chars].character,
                    &characters[num_chars].position.x,
                    &characters[num_chars].position.y) == 3) {
    num_chars++;
    if (num_chars >= MAX_CHARS) {
        fprintf(stderr, "Error: Reached maximum number of characters (%d)\n", MAX_CHARS);
        break;
    }
    }

    fclose(fptr);

    // Checkpoint: print the characters and coordinates read from the text file
    for (int i = 0; i < num_chars; i++) {
    printf("Character: %c, Coordinates: (%d, %d)\n",
            characters[i].character, characters[i].position.x, characters[i].position.y);
    }

    // Example
    Coordinate input = {35, 45};

    // Get the closest character to the input coordinates
    char result = getCharacterFromCoordinates(input, characters, sizeof(characters) / sizeof(characters[0]));

    // Output : We expect to see the letter B for simple_coords.txt
    printf("The closest character is: %c\n", result);

    return 0;
}


