#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_LENGTH 500

// Structure for Room information
struct Room {
    char type[MAX_LENGTH];
    char details[MAX_LENGTH];
    int price;
};

// Function to compare rooms by price for sorting
int compareByPrice(const void* a, const void* b) {
    const struct Room* room1 = (const struct Room*)a;
    const struct Room* room2 = (const struct Room*)b;
    return (room2->price - room1->price);
}

// Function to display room information
void displayRoomInformation() {
    struct Room rooms[] = {
        {"PENTHOUSE",
         "  Room Capacity: 10 or more\n"
         "  All amenities included in a Suite Room\n\n"
         "  *Included: (only for Penthouse guests)\n"
         "     - Expansive space and luxurious design\n"
         "     - Private balcony\n"
         "     - Personalized gourmet dining experience with a \n"
         "      private chef and customized menu\n"
         "     - Private access to a jacuzzi\n"
         "     - Unlimited access to hotel's sport facility and \n"
         "      hotel's recreational facility\n"
         "     - Private butler service for the duration of the stay",
         35000},

         {"EXECUTIVE ROOM",
         "  Room Capacity: 6\n"
         "  All amenities included in a Deluxe Room\n"
         "  Additional Single bedsize (1)\n\n"
         "  *Included: (only for Executive Room guests)\n"
         "     - Complimentary daily breakfast and exclusive access \n"
         "      to an executive lounge with snacks and beverages\n"
         "     - Access to the hotel's business center and meeting rooms\n"
         "     - Priority access to the hotel's recreational facilities \n"
         "      limited to spa and game room",
         20000},

         {"SUITE ROOM",
         "  Room Capacity: 4\n"
         "  All amenities included in a Deluxe Room\n"
         "  Double bedsize (2)\n"
         "  Separate living room\n"
         "  Kitchenette / fully-equipped kitchen\n\n"
         "  *Included: (only for Suite Room guests)\n"
         "     - Personalized in-room dining experience with a private chef\n"
         "     - Welcome fruit basket and a bottle of wine upon arrival\n"
         "     - Private yoga session\n"
         "     - Access to the hotel's sports facility limited to bowling \n"
         "      and billiard",
         12000},

         {"DELUXE ROOM",
         "  Room Capacity: 4\n"
         "  All amenities included in a Standard Room\n"
         "  Mini-refrigerator\n"
         "  California King bedsize (1)\n"
         "  Single bedsize (1)\n"
         "  Upgraded furnishings and decor\n\n"
         "  *Included: (only for Deluxe Room guests)\n"
         "     - Option for a gourmet dining experience at an on-site \n"
         "      restaurant\n"
         "     - Complimentary access to the hotel's fitness center\n"
         "     - Cooking class with a professional chef",
         9000},

         {"STANDARD ROOM",
         "  Room Capacity: 1 or 2\n"
         "  Single bedsize (2)\n"
         "  Private bathroom with shower\n"
         "  Television\n"
         "  Wi-Fi\n"
         "  Air condition\n\n"
         "  *Included: (only for Standard Room guests)\n"
         "     - Option for in-room dining with a variety of meal \n"
         "      choices available",
         5000}
    };

    int numRooms = sizeof(rooms) / sizeof(rooms[0]);

    // Sort the rooms by price in descending order
    qsort(rooms, numRooms, sizeof(struct Room), compareByPrice);

    printf("Room Information (Highest to Lowest Price):\n");
    for (int i = 0; i < numRooms; i++) {
        printf("\n======================================================================\n");
        printf("Price: %d Php/night\n", rooms[i].price);
        printf("Type: %s\n\n", rooms[i].type);
        printf("%s\n\n", rooms[i].details);
        printf("======================================================================\n\n");
    }
}

int main() {
    displayRoomInformation();
    return 0;
}
