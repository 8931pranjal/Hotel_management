#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_LENGTH 100

// Structure for client information
struct Client {
    char name[MAX_LENGTH];
    char email[MAX_LENGTH];
    char contactNo[MAX_LENGTH];
    char username[MAX_LENGTH];
    char password[MAX_LENGTH];
};

// Structure for reservation information
struct Reservation {
    char roomType[MAX_LENGTH];
    char referenceNumber[MAX_LENGTH];
    int month;
    int fromDate;
    int toDate;
    int confirmed;
    char loggedInUsername[MAX_LENGTH];
};

// Structure for a Node in the linked list
struct Node {
    struct Reservation reservation;
    struct Node* next;
};

// Class-like structure for Hotel Reservation System
struct HotelReservationSystem {
    struct Node* head;
};

void initHotelReservationSystem(struct HotelReservationSystem* sys) {
    sys->head = NULL;
}

// Function to add a reservation to the linked list
void addReservation(struct HotelReservationSystem* sys, struct Reservation res) {
    struct Node* newNode = (struct Node*)malloc(sizeof(struct Node));
    if (newNode == NULL) {
        printf("Memory allocation error!\n");
        exit(EXIT_FAILURE);
    }

    newNode->reservation = res;
    newNode->next = NULL;

    if (sys->head == NULL) {
        sys->head = newNode;
    } else {
        struct Node* current = sys->head;
        while (current->next != NULL) {
            current = current->next;
        }
        current->next = newNode;
    }
}

int main() {
    struct HotelReservationSystem sys;
    initHotelReservationSystem(&sys);

    // Sample usage
    struct Reservation res1 = {
        .roomType = "Single",
        .referenceNumber = "REF123",
        .month = 4,
        .fromDate = 15,
        .toDate = 20,
        .confirmed = 1,
        .loggedInUsername = "user123"
    };

    struct Reservation res2 = {
        .roomType = "Double",
        .referenceNumber = "REF456",
        .month = 5,
        .fromDate = 10,
        .toDate = 17,
        .confirmed = 0,
        .loggedInUsername = "user456"
    };

    addReservation(&sys, res1);
    addReservation(&sys, res2);

    // Printing sample reservations
    struct Node* current = sys.head;
    while (current != NULL) {
        printf("Room Type: %s\n", current->reservation.roomType);
        printf("Reference Number: %s\n", current->reservation.referenceNumber);
        printf("Month: %d\n", current->reservation.month);
        printf("From Date: %d\n", current->reservation.fromDate);
        printf("To Date: %d\n", current->reservation.toDate);
        printf("Confirmed: %s\n", current->reservation.confirmed ? "Yes" : "No");
        printf("Logged In Username: %s\n\n", current->reservation.loggedInUsername);

        current = current->next;
    }

    // Freeing memory (optional if no longer needed)
    current = sys.head;
    while (current != NULL) {
        struct Node* temp = current;
        current = current->next;
        free(temp);
    }

    return 0;
}
