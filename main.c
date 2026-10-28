#include "tp3.h"
#include <stdio.h>

void printMenu() {
    printf("\n=== HOTEL MANAGEMENT SYSTEM ===\n");
    printf("1. Create a hotel chain\n");
    printf("2. Add a location to a hotel chain\n");
    printf("3. Add a booking at a location\n");
    printf("4. Display the locations in a hotel chain\n");
    printf("5. Display the customers at a location\n");
    printf("6. Delete a booking at a location\n");
    printf("7. Delete a location\n");
    printf("8. Search for a customer by last name\n");
    printf("9. Process booking file\n");
    printf("10. Quit\n");
    printf("Choose an option: ");
}

int main() {
    T_Hotel *hotel = NULL;
    int choice = 0;

    while (1) {
        printMenu();
        if (scanf("%d", &choice) != 1) {
            while (getchar() != '\n'); 
            continue;
        }

        if (choice == 10) {
            printf("Exiting program...\n");
            if (hotel) freeHotel(hotel);
            break;
        }

        if (choice != 1 && choice != 9 && hotel == NULL) {
            printf("Error: Please create or load a hotel chain first (Option 1 or 9).\n");
            continue;
        }

        switch (choice) {
            case 1: {
                if (hotel) {
                    printf("Hotel chain already exists. Recreating...\n");
                    freeHotel(hotel);
                }
                char name[100];
                printf("Enter hotel chain name: ");
                while (getchar() != '\n'); 

                if (fgets(name, sizeof(name), stdin) != NULL) {
                    name[strcspn(name, "\n")] = '\0';
                }
                hotel = createHotel(name);
                printf("Hotel chain '%s' created.\n", hotel->name);
                break;
            }
            case 2: {
                char city[100];
                printf("Enter city name: ");
                while (getchar() != '\n');
                if (fgets(city, sizeof(city), stdin) != NULL) {
                    city[strcspn(city, "\n")] = '\0';
                }
                T_Site *site = createSite(city);
                if (addSite(hotel, site)) {
                    printf("Location '%s' added successfully.\n", city);
                } else {
                    printf("Error: Could not add location (City already exists or invalid).\n");
                    freeSite(site);
                }
                break;
            }
            case 3: {
                char city[100], last[50], first[50];
                int age, room;
                printf("Enter city: ");
                while (getchar() != '\n');
                if (fgets(city, sizeof(city), stdin) != NULL) {
                    city[strcspn(city, "\n")] = '\0';
                }

                T_Site *curr = hotel->first;
                while (curr && strcasecmp(curr->city, city) != 0) {
                    curr = curr->next;
                }

                if (!curr) {
                    printf("Error: Location '%s' not found.\n", city);
                    break;
                }

                // while (getchar() != '\n');
                printf("Enter Last Name: ");
                if (fgets(last, sizeof(last), stdin) != NULL) {
                    last[strcspn(last, "\n")] = '\0';
                }
                printf("Enter First Name: ");
                if (fgets(first, sizeof(first), stdin) != NULL) {
                    first[strcspn(first, "\n")] = '\0';
                }
                printf("Enter Age: ");
                scanf("%d", &age);
                printf("Enter Room Number: ");
                scanf("%d", &room);

                T_Client *client = createClient(last, first, age, room);
                if (addClient(curr, client)) {
                    printf("Booking added successfully.\n");
                } else {
                    printf("Failed to add booking.\n");
                    freeClient(client);
                }
                break;
            }
            case 4:
                displayHotel(hotel);
                break;
            case 5: {
                char city[100];
                printf("Enter city: ");
                while (getchar() != '\n');
                if (fgets(city, sizeof(city), stdin) != NULL) {
                    city[strcspn(city, "\n")] = '\0';
                }

                T_Site *curr = hotel->first;
                while (curr && strcasecmp(curr->city, city) != 0) {
                    curr = curr->next;
                }

                if (curr) {
                    displaySite(curr);
                } else {
                    printf("Location '%s' not found.\n", city);
                }
                break;
            }
            case 6: {
                char city[100], last[50], first[50];
                printf("Enter city: ");
                while (getchar() != '\n');
                if (fgets(city, sizeof(city), stdin) != NULL) {
                    city[strcspn(city, "\n")] = '\0';
                }
                printf("Enter Last Name: ");
                if (fgets(last, sizeof(last), stdin) != NULL) {
                    last[strcspn(last, "\n")] = '\0';
                }
                printf("Enter First Name: ");
                if (fgets(first, sizeof(first), stdin) != NULL) {
                    first[strcspn(first, "\n")] = '\0';
                }

                T_Site *curr = hotel->first;
                while (curr && strcasecmp(curr->city, city) != 0) {
                    curr = curr->next;
                }

                if (curr && deleteClient(curr, last, first)) {
                    printf("Booking deleted successfully.\n");
                } else {
                    printf("Error: Client or location not found.\n");
                }
                break;
            }
            case 7: {
                char city[100];
                printf("Enter city to delete: ");
                while (getchar() != '\n');
                if (fgets(city, sizeof(city), stdin) != NULL) {
                    city[strcspn(city, "\n")] = '\0';
                }

                if (deleteSite(hotel, city)) {
                    printf("Location '%s' and all its bookings deleted.\n", city);
                } else {
                    printf("Location not found.\n");
                }
                break;
            }
            case 8: {
                char prefix[50];
                printf("Enter last name search criteria/prefix: ");
                while (getchar() != '\n');
                if (fgets(prefix, sizeof(prefix), stdin) != NULL) {
                    prefix[strcspn(prefix, "\n")] = '\0';
                }
                searchClients(hotel, prefix);
                break;
            }
            case 9:
                if (!hotel) {
                    hotel = createHotel("Default Chain");
                }
                processingfile(hotel);
                break;
            default:
                printf("Invalid choice. Try again.\n");
        }
    }

    return 0;
}