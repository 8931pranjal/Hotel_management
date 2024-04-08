#include <stdio.h>
#include <string.h>
#include <conio.h>
#include <stdlib.h>
#include "Account.h"
#include "HRS.h"
#include "Account_setting.h"
#include "Sorted_roomprice.h"
#include "ReservationSystem.h"

// Function prototypes
void displayClientMenu();
void RoomReserve();
void Reservation(HotelReservationSystem* sys, const char* loggedInUsername);
void AdminMenu(HotelReservationSystem* sys);
void ClientMenu(HotelReservationSystem* sys, const char* loggedInUsername);
void showLoginPage(HotelReservationSystem* sys);

// Function to display the client menu
void displayClientMenu() {
    printf("=== Client Menu ===\n");
    printf("[a] Room Reservation\n");
    printf("[b] Display reservation/s\n");
    printf("[c] Account settings\n");
    printf("[d] Log Out\n");
    printf("[e] Exit\n");
    printf("Enter your choice: ");
}

void RoomReserve() {
    printf("=== Room Reservation ===\n");
    printf("[a] Schedule Room Reservation\n");
    printf("[b] View room types and amenities\n");
    printf("[c] Show available rooms\n");
    printf("[d] Back\n");
    printf("[e] Exit\n");
    printf("Enter your choice: ");
}

void Reservation(HotelReservationSystem* sys, const char* loggedInUsername) {
    char choice;
    do {
        system("cls");
        RoomReserve();
        scanf(" %c", &choice);
        switch (choice) {
            case 'a':
                sched(loggedInUsername); // Pass the loggedInUsername to sched
                break;
            case 'b':
                char choice;
                system("cls");
                displayRoomInformation();
                while (1) {
                    printf("[a]Back: ");
                    scanf(" %c", &choice);
                    if (choice == 'a') {
                        break;
                    } else {
                        printf("Invalid. Try again.\n");
                    }
                }
                break;
            case 'c':
                int chosenMonth, chosenFromDate;
                printf("Enter the month number (1-12): ");
                scanf("%d", &chosenMonth);
                printf("Enter the date (1-%d): ", calendar_getMonthDays(chosenMonth, 2024));
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
            case 'd':
                return;
            case 'e':
                exit(0);
            default:
                printf("Invalid choice. Please try again.\n");
        }
    } while (choice != 'd');
}

// Function to handle the main admin menu
void AdminMenu(HotelReservationSystem* sys) {
    Display_reservation_admin();
}

void ClientMenu(HotelReservationSystem* sys, const char* loggedInUsername) {
    char choice;
    do {
        system("cls");
        displayClientMenu();
        scanf(" %c", &choice);

        switch (choice) {
            case 'a':
                Reservation(sys, loggedInUsername); // Pass loggedInUsername to Reservation
                break;
            case 'b':
                Display_reservation_client(loggedInUsername);
                break;
            case 'c':
                Account_setting(sys, loggedInUsername);
                break;
            case 'd':
                printf("Logging out...\n");
                break;
            case 'e':
                printf("Exiting...\n");
                exit(0);
            default:
                printf("Invalid choice. Please try again.\n");
        }

        // Clear the input buffer to avoid invalid inputs causing issues in the next loop iteration
        fflush(stdin);

    } while (choice != 'd');
}

// Function to login
void showLoginPage(HotelReservationSystem* sys) {
    Account account;
    system("cls");
    printf("Login System\n");
    char choice;
    printf("[a] Register\n");
    printf("[b] Login\n");
    printf("[c] Exit\n");
    printf("Enter your choice: ");

    scanf(" %c", &choice);
    while ((choice != 'a') && (choice != 'b') && (choice != 'c')) {
        printf("Invalid choice. Please try again: ");
        scanf(" %c", &choice);
    }

    switch (choice) {
        case 'a':
            account_registerUser();
            break;
        case 'b': {
            char loggedInUsername[MAX_LENGTH];
            account_loginUser(loggedInUsername);
            if (strlen(loggedInUsername) > 0) {
                if (strcmp(loggedInUsername, "admin") == 0) {
                    printf("Admin is logged in.\n");
                    AdminMenu(sys);
                } else {
                    printf("Logged in successfully.\n");
                    ClientMenu(sys, loggedInUsername); // Pass loggedInUsername to ClientMenu
                }
            }
            break;
        }
        case 'c':
            printf("Exiting...\n");
            exit(0);
        default:
            printf("\nInvalid Input\n");
            break;
    }

    // Clear the input buffer
    fflush(stdin);
    system("pause");
}

int main() {
    HotelReservationSystem sys;
    loadReservationsFromFile();
    while (1) {
        showLoginPage(&sys);
    }
    return 0;
}
