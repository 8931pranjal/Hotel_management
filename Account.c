
#include<stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_LENGTH 100

char credentials[MAX_LENGTH] = "credentials";

char* encryptPassword(const char* password) {
    char* encryptedPassword = strdup(password);
    // Perform a simple hash by reversing the password
    int length = strlen(encryptedPassword);
    for (int i = 0; i < length / 2; i++) {
        char temp = encryptedPassword[i];
        encryptedPassword[i] = encryptedPassword[length - i - 1];
        encryptedPassword[length - i - 1] = temp;
    }
    return encryptedPassword;
}

int isUserExists(const char* username) {
    FILE* file = fopen(credentials, "r");
    if (!file) {
        return 0;
    }
    char line[MAX_LENGTH];
    while (fgets(line, sizeof(line), file)) {
        if (strncmp(line, username, strlen(username)) == 0) {
            fclose(file);
            return 1;
        }
    }
    fclose(file);
    return 0;
}

int isLoginValid(const char* username, const char* password) {
    FILE* file = fopen(credentials, "r");
    if (!file) {
        return 0;
    }
    char line[MAX_LENGTH];
    while (fgets(line, sizeof(line), file)) {
        char storedUsername[MAX_LENGTH];
        char storedPassword[MAX_LENGTH];
        sscanf(line, "%[^,],%[^,]", storedUsername, storedPassword);
        if (strcmp(storedUsername, username) == 0 && strcmp(storedPassword, encryptPassword(password)) == 0) {
            fclose(file);
            return 1;
        }
    }
    fclose(file);
    return 0;
}

void createUser(const char* username, const char* password, const char* name, const char* email, const char* contactNumber) {
    FILE* file = fopen(credentials, "a");
    if (!file) {
        printf("Error opening file!\n");
        return;
    }
    fprintf(file, "%s,%s,%s,%s,%s\n", username, encryptPassword(password), name, email, contactNumber);
    fclose(file);
    printf("User created successfully.\n");
}

int isValidEmail(const char* email) {
    // Check for the presence of '@' symbol
    char* atPos = strchr(email, '@');
    if (!atPos) {
        return 0;
    }
    // Check for the presence of '.' after the '@' symbol
    char* dotPos = strchr(atPos, '.');
    if (!dotPos) {
        return 0;
    }
    // Check if there are at least two characters after the last dot
    if (strlen(dotPos) <= 2) {
        return 0;
    }
    return 1;
}

void registerUser() {
    char name[MAX_LENGTH];
    char email[MAX_LENGTH];
    char contactNumber[MAX_LENGTH];
    char username[MAX_LENGTH];
    char password[MAX_LENGTH];

    printf("=== Register ===\n");
    printf("Enter name: ");
    fgets(name, sizeof(name), stdin);
    name[strcspn(name, "\n")] = 0;

    int validEmail = 0;
    while (!validEmail) {
        printf("Enter email: ");
        fgets(email, sizeof(email), stdin);
        email[strcspn(email, "\n")] = 0;
        if (!isValidEmail(email)) {
            printf("Invalid email format. Please enter a valid email.\n");
        } else {
            validEmail = 1;
        }
    }

    int validContactNumber = 0;
    while (!validContactNumber) {
        printf("Enter contact number (11 digits): ");
        fgets(contactNumber, sizeof(contactNumber), stdin);
        contactNumber[strcspn(contactNumber, "\n")] = 0;
        if (strlen(contactNumber) == 11) {
            int allDigits = 1;
            for (int i = 0; i < strlen(contactNumber); i++) {
                if (!isdigit(contactNumber[i])) {
                    allDigits = 0;
                    break;
                }
            }
            if (allDigits) {
                validContactNumber = 1;
            }
        }
        if (!validContactNumber) {
            printf("Invalid contact number. Please enter a valid 11-digit number.\n");
        }
    }

    int usernameExists = 0;
    while (!usernameExists) {
        printf("Enter username: ");
        fgets(username, sizeof(username), stdin);
        username[strcspn(username, "\n")] = 0;

        if (isUserExists(username)) {
            printf("Username already exists. Please try again.\n");
        } else if (strcmp(username, "admin") == 0) {
            printf("'admin' is restricted as a username. Please try again.\n");
        } else {
            printf("Enter password: ");
            fgets(password, sizeof(password), stdin);
            password[strcspn(password, "\n")] = 0;
            createUser(username, password, name, email, contactNumber);
            usernameExists = 1;
        }
    }
}

char* getMaskedInput() {
    const char maskChar = '*';
    char* input = malloc(MAX_LENGTH * sizeof(char));
    if (!input) {
        printf("Memory allocation error!\n");
        exit(EXIT_FAILURE);
    }

    int index = 0;
    char ch;
    while ((ch = getchar()) != '\n' && ch != EOF) {
        if (ch == '\b' && index > 0) {
            input[--index] = '\0';
            printf("\b \b");
        } else if (isprint(ch)) {
            input[index++] = ch;
            putchar(maskChar);
        }
    }
    input[index] = '\0';
    printf("\n");
    return input;
}

char* loginUser() {
    char username[MAX_LENGTH];
    char* password;

    printf("=== Log In ===\n");
    printf("Enter username: ");
    fgets(username, sizeof(username), stdin);
    username[strcspn(username, "\n")] = 0;

    if (strcmp(username, "admin") == 0) {
        printf("Enter password: ");
        password = getMaskedInput();
        if (strcmp(password, "hrsadmin") == 0) {
            free(password);
            return "admin";
        } else {
            printf("Invalid password.\n");
            free(password);
            return "";
        }
    }

    if (!isUserExists(username)) {
        printf("User does not exist.\n");
        return "";
    } else {
        printf("Enter password: ");
        password = getMaskedInput();
        if (isLoginValid(username, password)) {
            free(password);
            return username;
        } else {
            printf("Invalid username or password.\n");
            free(password);
            return "";
        }
    }
}

int isCurrentPasswordValid(const char* username, const char* currentPassword) {
    FILE* file = fopen(credentials, "r");
    if (!file) {
        return 0;
    }
    char line[MAX_LENGTH];
    while (fgets(line, sizeof(line), file)) {
        char storedUsername[MAX_LENGTH];
        char storedPassword[MAX_LENGTH];
        sscanf(line, "%[^,],%[^,]", storedUsername, storedPassword);
        if (strcmp(storedUsername, username) == 0 && strcmp(storedPassword, encryptPassword(currentPassword)) == 0) {
            fclose(file);
            return 1;
        }
    }
    fclose(file);
    return 0;
}

void changePassword(const char* username, const char* currentPassword, const char* newPassword) {
    if (!isCurrentPasswordValid(username, currentPassword)) {
        printf("Incorrect current password.\n");
        return;
    }

    FILE* file = fopen(credentials, "r");
    if (!file) {
        printf("Error opening file!\n");
        return;
    }
    FILE* outFile = fopen("temp_credentials", "w");
    if (!outFile) {
        printf("Error creating temporary file!\n");
        fclose(file);
        return;
    }

    char line[MAX_LENGTH];
    while (fgets(line, sizeof(line), file)) {
        char storedUsername[MAX_LENGTH];
        char storedPassword[MAX_LENGTH];
        sscanf(line, "%[^,],%[^,]", storedUsername, storedPassword);
        if (strcmp(storedUsername, username) == 0 && strcmp(storedPassword, encryptPassword(currentPassword)) == 0) {
            fprintf(outFile, "%s,%s\n", username, encryptPassword(newPassword));
            printf("Password changed successfully.\n");
        } else {
            fprintf(outFile, "%s", line);
        }
    }

    fclose(file);
    fclose(outFile);
    remove(credentials);
    rename("temp_credentials", credentials);
}

void printUserInfo(const char* username) {
    FILE* file = fopen(credentials, "r");
    if (!file) {
        printf("Error opening file!\n");
        return;
    }

    char line[MAX_LENGTH];
    while (fgets(line, sizeof(line), file)) {
        char storedUsername[MAX_LENGTH];
        char storedPassword[MAX_LENGTH];
        char storedName[MAX_LENGTH];
        char storedEmail[MAX_LENGTH];
        char storedContactNumber[MAX_LENGTH];
        sscanf(line, "%[^,],%[^,],%[^,],%[^,],%[^,]", storedUsername, storedPassword, storedName, storedEmail, storedContactNumber);
        if (strcmp(storedUsername, username) == 0) {
            printf("=== User Info ===\n\n");
            printf("Username: %s\n", username);
            printf("Name: %s\n", storedName);
            printf("Email: %s\n", storedEmail);
            printf("Contact Number: %s\n", storedContactNumber);
            fclose(file);
            return;
        }
    }

    fclose(file);
    printf("User information not found.\n");
}

int main() {
    int choice;
    char* loggedInUser = NULL;

    while (1) {
        printf("\n=== Menu ===\n");
        printf("1. Register\n");
        printf("2. Log In\n");
        printf("3. Change Password\n");
        printf("4. View User Info\n");
        printf("5. Exit\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);
        getchar(); // Clear the newline character from the buffer

        switch (choice) {
            case 1:
                registerUser();
                break;
            case 2:
                if (loggedInUser != NULL) {
                    printf("Already logged in as %s. Please log out to log in as a different user.\n", loggedInUser);
                } else {
                    loggedInUser = loginUser();
                }
                break;
            case 3:
                if (loggedInUser != NULL) {
                    printf("=== Change Password ===\n");
                    char currentPassword[MAX_LENGTH];
                    char newPassword[MAX_LENGTH];

                    printf("Enter current password: ");
                    fgets(currentPassword, sizeof(currentPassword), stdin);
                    currentPassword[strcspn(currentPassword, "\n")] = 0;

                    printf("Enter new password: ");
                    fgets(newPassword, sizeof(newPassword), stdin);
                    newPassword[strcspn(newPassword, "\n")] = 0;

                    changePassword(loggedInUser, currentPassword, newPassword);
                } else {
                    printf("Please log in to change password.\n");
                }
                break;
            case 4:
                if (loggedInUser != NULL) {
                    printUserInfo(loggedInUser);
                } else {
                    printf("Please log in to view user info.\n");
                }
                break;
            case 5:
                printf("Exiting...\n");
                if (loggedInUser != NULL) {
                    free(loggedInUser);
                }
                exit(EXIT_SUCCESS);
            default:
                printf("Invalid choice. Please try again.\n");
        }
    }

    return 0;
}
