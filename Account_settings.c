#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "HotelReservationSystem.h"
#include "Account.h"

void Account_setting(HotelReservationSystem* sys, const char* loggedInUsername) {
    Account settings;
    system("cls");
    printf("=== Account Settings ===\n");
    char choice;
    printf("[a] User Info\n");
    printf("[b] Change Password\n");
    printf("[c] Update Email\n");
    printf("[d] Update Contact Number\n");
    printf("[e] Back\n");
    printf("[f] Exit\n");
    printf("Enter your choice: ");
    scanf(" %c", &choice);

    switch (choice) {
        case 'a': {
            char uname[MAX_LENGTH];
            strcpy(uname, loggedInUsername);
            system("cls");

            if (strlen(loggedInUsername) > 0) {
                // User logged in successfully
                settings.printUserInfo(uname);
            }
            printf("\n");
            printf("[a]Back     [b]Exit: ");
            scanf(" %c", &choice);
            if (choice == 'a') {
                break;
            } else if (choice == 'f') {
                exit(0);
            }
            break;
        }

        case 'b': {
            char currentPassword[MAX_LENGTH];
            char newPassword[MAX_LENGTH];

            printf("Enter current password: ");
            scanf(" %s", currentPassword);

            if (!settings.isCurrentPasswordValid(loggedInUsername, currentPassword)) {
                printf("Incorrect current password.\n");
                break;
            }

            printf("Enter new password: ");
            scanf(" %s", newPassword);

            settings.changePassword(loggedInUsername, currentPassword, newPassword);
            break;
        }

        case 'c': {
            char currentPassword[MAX_LENGTH];
            char newEmail[MAX_LENGTH];

            printf("Enter current password: ");
            scanf(" %s", currentPassword);

            if (!settings.isCurrentPasswordValid(loggedInUsername, currentPassword)) {
                printf("Incorrect current password.\n");
                break;
            }

            printf("Enter new email: ");
            scanf(" %s", newEmail);

            settings.updateEmail(loggedInUsername, currentPassword, newEmail);
            break;
        }

        case 'd': {
            char currentPassword[MAX_LENGTH];
            char newContactNumber[MAX_LENGTH];

            printf("Enter current password: ");
            scanf(" %s", currentPassword);

            if (!settings.isCurrentPasswordValid(loggedInUsername, currentPassword)) {
                printf("Incorrect current password.\n");
                system("pause");
                break;
            }

            printf("Enter new contact number: ");
            scanf(" %s", newContactNumber);

            settings.updateContactNumber(loggedInUsername, currentPassword, newContactNumber);
            break;
        }

        case 'e':
            return;

        case 'f':
            exit(0);

        default:
            printf("\nInvalid Input\n");
            break;
    }
}
