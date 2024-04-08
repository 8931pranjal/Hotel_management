#ifndef RESERVATIONSYSTEM_H
#define RESERVATIONSYSTEM_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define NUM_MONTHS 12
#define NUM_DAYS_PER_WEEK 7
#define MAX_LENGTH 100

typedef struct {
    char roomType[MAX_LENGTH];
    char referenceNumber[MAX_LENGTH];
    int month;
    int fromDate;
    int toDate;
    int confirmed;
    char loggedInUsername[MAX_LENGTH];
} Reservation;

void confirmSelectedReservation();
void Roomreserve(int chosenMonth, int chosenFromDate, int chosenToDate, const char* loggedInUsername);
void confirmSelectedReservation();
void cancelReservation();
void sched(const char* loggedInUsername);
void displayRoomInformation();
void DisplayRoomAvailability(int chosenMonth, int chosenFromDate);
char* generateReferenceNumber();

typedef struct {
    char* monthNames[NUM_MONTHS];
    int startingDays[NUM_MONTHS];
} Calendar;

struct RoomAvailability {
    int count;
    int* availability;
};

typedef struct {
    char type[MAX_LENGTH];
    RoomAvailability* availabilityByMonth;
    int price;
} RoomType;

RoomType roomTypes[] = {
    {"Standard Room", NULL, 15, 5000},
    {"Deluxe Room", NULL, 10, 9000},
    {"Suite Room", NULL, 10, 12000},
    {"Executive Room", NULL, 7, 20000},
    {"Penthouse", NULL, 3, 35000}
};

Reservation* reservations;
Calendar calendar;

void Roomreserve(int chosenMonth, int chosenFromDate, int chosenToDate, const char* loggedInUsername) {
    char roomreserve_opt[MAX_LENGTH];
    int roomIndex = -1;

    while (1) {
        printf("\nPlease select your preferred room type:\n");
        for (int i = 0; i < sizeof(roomTypes) / sizeof(roomTypes[0]); i++) {
            printf("[%c] %s\n", 'A' + i, roomTypes[i].type);
        }
        printf("\n[%c] Back\n", 'A' + sizeof(roomTypes) / sizeof(roomTypes[0]));
        printf("[%c] Exit\n", 'A' + sizeof(roomTypes) / sizeof(roomTypes[0]) + 1);
        printf("Enter your preferred room: ");
        scanf(" %s", roomreserve_opt);

        // Convert the input to uppercase
        for (int i = 0; roomreserve_opt[i]; i++) {
            roomreserve_opt[i] = toupper(roomreserve_opt[i]);
        }

        switch (roomreserve_opt[0]) {
            case 'A':
            case 'B':
            case 'C':
            case 'D':
            case 'E':
                roomIndex = roomreserve_opt[0] - 'A'; // Assigning value to roomIndex
                break;
            case 'F':
                return;
            case 'G':
                printf("Thank you for using the Reservation System. Goodbye!\n");
                exit(0);
            default:
                printf("Invalid entry. Please try again.\n");
                break; // Loop to re-ask for valid input
        }

        if (roomIndex != -1)
            break; // Exit the loop if a valid room selection is made
    }

    if (roomTypes[roomIndex].availabilityByMonth[chosenMonth - 1].availability[chosenFromDate - 1] > 0) {
        char* referenceNumber = generateReferenceNumber();
        strcpy(reservations->roomType, roomTypes[roomIndex].type);
        strcpy(reservations->referenceNumber, referenceNumber);
        reservations->month = chosenMonth;
        reservations->fromDate = chosenFromDate;
        reservations->toDate = chosenToDate;
        reservations->confirmed = 0;
        strcpy(reservations->loggedInUsername, loggedInUsername);

        // Store the reservation details in reservations.txt
        FILE* outFile = fopen("reservations.txt", "a");
        if (outFile != NULL) {
            fprintf(outFile, "%s,%s,%d,%d,%d,%d,%s\n", reservations->roomType, reservations->referenceNumber,
                    reservations->month, reservations->fromDate, reservations->toDate, reservations->confirmed, reservations->loggedInUsername);
            fclose(outFile);
            printf("Reservation saved successfully.\n");
        } else {
            printf("Unable to open the file for writing.\n");
        }

        // Calculate the total amount based on the price, the duration of the stay, and the priceMultiplier
        int numDays = chosenToDate - chosenFromDate;
        if (numDays == 0) {
            numDays = 1;
        }
        int totalAmount = roomTypes[roomIndex].price * numDays;

        printf("\nRoom reserved successfully. Reference number: %s\n", referenceNumber);
        printf("Total amount: PHP %d.00\n", totalAmount);

        system("pause");

    } else {
        printf("Sorry, no rooms of this type are available.\n");
    }
}

void loadReservationsFromFile() {
    FILE* inFile = fopen("reservations.txt", "r");
    if (inFile != NULL) {
        reservations = malloc(sizeof(Reservation));
        int count = 0;
        char line[MAX_LENGTH];

        while (fgets(line, sizeof(line), inFile) != NULL) {
            char roomType[MAX_LENGTH];
            char referenceNumber[MAX_LENGTH];
            int month, fromDate, toDate, confirmed;
            char loggedInUsername[MAX_LENGTH];

            if (sscanf(line, "%[^,],%[^,],%d,%d,%d,%d,%s", roomType, referenceNumber, &month, &fromDate, &toDate, &confirmed, loggedInUsername) == 7) {
                strcpy(reservations[count].roomType, roomType);
                strcpy(reservations[count].referenceNumber, referenceNumber);
                reservations[count].month = month;
                reservations[count].fromDate = fromDate;
                reservations[count].toDate = toDate;
                reservations[count].confirmed = confirmed;
                strcpy(reservations[count].loggedInUsername, loggedInUsername);
                count++;
                reservations = realloc(reservations, (count + 1) * sizeof(Reservation));
            }
        }

        fclose(inFile);
    }
}

char* generateReferenceNumber() {
    char* referenceNumber = malloc(7 * sizeof(char));
    const char alphanumeric[] =
        "0123456789"
        "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
        "abcdefghijklmnopqrstuvwxyz";
    const int alphanumericLength = strlen(alphanumeric);

    srand((unsigned int)time(NULL));
    for (int i = 0; i < 6; i++) {
        referenceNumber[i] = alphanumeric[rand() % alphanumericLength];
    }
    referenceNumber[6] = '\0';

    return referenceNumber;
}

void confirmSelectedReservation() {
    char referenceNumber[MAX_LENGTH];
    printf("Enter the reference number of the reservation to confirm: ");
    scanf(" %s", referenceNumber);

    int reservationFound = 0;
    for (int i = 0; i < sizeof(reservations) / sizeof(reservations[0]); i++) {
        if (strcmp(reservations[i].referenceNumber, referenceNumber) == 0) {
            reservationFound = 1;

            // Check if the reservation is already confirmed
            if (reservations[i].confirmed) {
                printf("This reservation is already confirmed.\n");
                return;
            }

            // Find the room type index
            int roomIndex = -1;
            for (int j = 0; j < sizeof(roomTypes) / sizeof(roomTypes[0]); j++) {
                if (strcmp(roomTypes[j].type, reservations[i].roomType) == 0) {
                    roomIndex = j;
                    break;
                }
            }

            if (roomIndex == -1) {
                printf("Invalid room type for the reservation.\n");
                return;
            }

            // Call DisplayRoomAvailability to check the number of available rooms
            int chosenMonth = reservations[i].month;
            int chosenFromDate = reservations[i].fromDate;
            //DisplayRoomAvailability(chosenMonth, chosenFromDate);

            // Check if the rooms are available for the reservation dates
            int chosenToDate = reservations[i].toDate;
            for (int k = chosenFromDate; k <= chosenToDate; k++) {
                if (roomTypes[roomIndex].availabilityByMonth[chosenMonth - 1].availability[k - 1] <= 0) {
                    printf("Sorry, the rooms are not available for the reservation dates.\n");
                    return;
                }
            }

            // Mark the reservation as confirmed
            reservations[i].confirmed = 1;

            // Adjust room availability for the chosen date range of the selected room type
            for (int l = chosenFromDate; l <= chosenToDate; l++) {
                roomTypes[roomIndex].availabilityByMonth[chosenMonth - 1].availability[l - 1]--;
            }

            printf("Reservation confirmed successfully.\n");
            return;
        }
    }

    if (!reservationFound) {
        printf("Invalid reference number. Reservation not found.\n");
    }
}

void deleteReservation() {
    char referenceNumber[MAX_LENGTH];
    printf("Enter the reference number of the reservation to cancel: ");
    scanf(" %s", referenceNumber);

    int reservationFound = 0;
    for (int i = 0; i < sizeof(reservations) / sizeof(reservations[0]); i++) {
        if (strcmp(reservations[i].referenceNumber, referenceNumber) == 0) {
            reservationFound = 1;
            for (int j = i; j < sizeof(reservations) / sizeof(reservations[0]) - 1; j++) {
                reservations[j] = reservations[j + 1];
            }
            printf("Reservation successfully cancelled.\n");
            break;
        }
    }

    if (!reservationFound) {
        printf("Invalid reference number. Reservation not found.\n");
    }
}

void Display_reservation_admin() {
    char display_opt[MAX_LENGTH];
    do {
        system("cls");
        printf("=== Manage Reservation Schedule ===\n\n");

        if (sizeof(reservations) / sizeof(reservations[0]) == 0) {
            printf("No reservations found.\n");
        }
        for (int i = 0; i < sizeof(reservations) / sizeof(reservations[0]); i++) {
            printf("[%d] [Username: %s] [Room Type: %s] (Date booked: %d/%d-%d, Reference number: %s)",
                   i + 1, reservations[i].loggedInUsername, reservations[i].roomType,
                   reservations[i].month, reservations[i].fromDate, reservations[i].toDate,
                   reservations[i].referenceNumber);
            printf("  Status: %s\n", (reservations[i].confirmed ? "CONFIRMED" : "PENDING"));
        }

        printf("\n[a] Confirm reservation");
        printf("\n[b] Delete reservation\n");
        printf("[c] Show Available Rooms\n");
        printf("[d] Logout\n");
        printf("[e] Exit\n");
        printf("Enter your choice: ");
        scanf(" %s", display_opt);
        printf("\n");

        // Convert the input to uppercase
        display_opt[0] = toupper(display_opt[0]);

        switch (display_opt[0]) {
            case 'A':
                confirmSelectedReservation();
                system("pause");
                break;
            case 'B':
                deleteReservation();
                system("pause");
                break;
            case 'C':
                char choice;
                int chosenMonth, chosenFromDate;
                printf("Enter the month number (1-12): ");
                scanf("%d", &chosenMonth);
                printf("Enter the date (1-%d): ", calendar.getMonthDays(chosenMonth, 2024));
                scanf("%d", &chosenFromDate);
                DisplayRoomAvailability(chosenMonth, chosenFromDate);
                while (1) {
                    printf("\n[a]Back: ");
                    scanf(" %c", &choice);
                    if (choice == 'a') {
                        break;
                    } else {
                        printf("Invalid. Try again.\n");
                    }
                }
                system("pause");
                break;
            case 'D':
                printf("Logging out...\n");
                break;
            case 'E':
                printf("Thank you for using the Reservation System. Goodbye!\n");
                exit(0);
            default:
                printf("Invalid entry.\n");
        }
    } while (display_opt[0] != 'D');
}

void Display_reservation_client(const char* loggedInUsername) {
    system("cls");
    printf("\nMY RESERVATIONS:\n");
    int reservationCount = 0;

    for (int i = 0; i < sizeof(reservations) / sizeof(reservations[0]); i++) {
        if (strcmp(reservations[i].loggedInUsername, loggedInUsername) == 0) {
            reservationCount++;
            printf("[%d] %s (Date booked: %d/%d-%d, Reference number: %s)",
                   reservationCount, reservations[i].roomType,
                   reservations[i].month, reservations[i].fromDate, reservations[i].toDate,
                   reservations[i].referenceNumber);
            printf("  Status: %s\n", (reservations[i].confirmed ? "CONFIRMED" : "PENDING"));
        }
    }

    if (reservationCount == 0) {
        printf("You have no reservations.\n");
    }

    char display_opt[MAX_LENGTH];
    printf("\n[a] Cancel reservation\n");
    printf("[b] Back\n");
    printf("[c] Exit\n");
    printf("Enter your choice: ");
    scanf(" %s", display_opt);
    printf("\n");

    // Convert the input to uppercase
    display_opt[0] = toupper(display_opt[0]);

    switch (display_opt[0]) {
        case 'A':
            deleteReservation();
            system("pause");
            break;
        case 'B':
            return;
        case 'C':
            printf("Thank you for using the Reservation System. Goodbye!\n");
            exit(0);
        default:
            printf("Invalid entry.\n");
    }

    // Clear the input buffer
    while (getchar() != '\n');
}

void sched(const char* loggedInUsername) {
    int chosenMonth, chosenFromDate, chosenToDate;

    while (1) {
        printf("Enter the month number (1-12): ");
        scanf("%d", &chosenMonth);

        if (chosenMonth < 1 || chosenMonth > NUM_MONTHS) {
            printf("Invalid month number. Please try again.\n");
            while (getchar() != '\n');
        } else {
            break;
        }
    }

    int numDays = calendar.getMonthDays(chosenMonth, 2024);

    // Display the calendar for the chosen month
    calendar.displayMonth(chosenMonth, 2024, 1);

    while (1) {
        printf("Enter the starting date (1-%d): ", numDays);
        scanf("%d", &chosenFromDate);

        if (chosenFromDate < 1 || chosenFromDate > numDays) {
            printf("Invalid date.\n");
        } else {
            break;
        }
    }

    while (1) {
        printf("Enter the ending date (1-%d): ", numDays);
        scanf("%d", &chosenToDate);

        if (chosenToDate <= chosenFromDate || chosenToDate > numDays) {
            printf("Invalid date.\n");
        } else {
            break;
        }
    }

    system("cls");

    printf("\nROOM AVAILABILITY on %s %d:\n", calendar.monthNames[chosenMonth - 1], chosenFromDate);
    printf("Room Type\tAvailable Rooms\n");

    for (int i = 0; i < sizeof(roomTypes) / sizeof(roomTypes[0]); i++) {
        int availableRooms = roomTypes[i].availabilityByMonth[chosenMonth - 1].availability[chosenFromDate - 1];
        printf("%s\t\t%d\n", roomTypes[i].type, availableRooms);
    }

    Roomreserve(chosenMonth, chosenFromDate, chosenToDate, loggedInUsername);
}

void DisplayRoomAvailability(int chosenMonth, int chosenFromDate) {
    if (chosenMonth < 1 || chosenMonth > NUM_MONTHS) {
        printf("Invalid month number.\n");
        return;
    }

    int numDays = calendar.getMonthDays(chosenMonth, 2024);
    printf("\nRoom Type\tAvailable Rooms\n");

    for (int i = 0; i < sizeof(roomTypes) / sizeof(roomTypes[0]); i++) {
        int availableRooms = roomTypes[i].availabilityByMonth[chosenMonth - 1].availability[chosenFromDate - 1];
        printf("%s\t\t%d\n", roomTypes[i].type, availableRooms);
    }
}

RoomType* getAvailableRooms(int chosenMonth, int chosenFromDate) {
    RoomType* availableRooms = malloc(sizeof(roomTypes));

    // Adjust room availability based on reservations for the chosen date
    for (int i = 0; i < sizeof(reservations) / sizeof(reservations[0]); i++) {
        if (reservations[i].month == chosenMonth && reservations[i].fromDate <= chosenFromDate && reservations[i].toDate >= chosenFromDate) {
            for (int j = 0; j < sizeof(roomTypes) / sizeof(roomTypes[0]); j++) {
                if (strcmp(roomTypes[j].type, reservations[i].roomType) == 0) {
                    roomTypes[j].availabilityByMonth[chosenMonth - 1].availability[chosenFromDate - 1]--;
                }
            }
        }
    }

    return availableRooms;
}

#endif /* RESERVATIONSYSTEM_H */
