
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>

#define FILENAME "vols.dat"

typedef struct {
    char code[10];
    char destination[50];
    char date[15];
    int seats_available;
} Flight;

// Fonction pour ajouter un vol
void add_flight() {
    Flight flight;
    int fd = open(FILENAME, O_WRONLY | O_CREAT | O_APPEND, 0644);
    if (fd == -1) {
        perror("Erreur d'ouverture du fichier");
        return;
    }

    printf("Entrez le code du vol : ");
    scanf("%s", flight.code);
    printf("Entrez la destination : ");
    scanf("%s", flight.destination);
    printf("Entrez la date (format YYYY-MM-DD) : ");
    scanf("%s", flight.date);
    printf("Entrez le nombre de si�ges disponibles : ");
    scanf("%d", &flight.seats_available);

    if (write(fd, &flight, sizeof(Flight)) == -1) {
        perror("Erreur d'�criture dans le fichier");
    } else {
        printf("Vol ajout� avec succ�s !\n");
    }

    close(fd);
}

// Fonction pour afficher les informations des vols
void display_flights() {
    Flight flight;
    int fd = open(FILENAME, O_RDONLY);
    if (fd == -1) {
        perror("Erreur d'ouverture du fichier");
        return;
    }

    printf("Liste des vols :\n");
    while (read(fd, &flight, sizeof(Flight)) > 0) {
        printf("Code : %s, Destination : %s, Date : %s, Si�ges disponibles : %d\n",
               flight.code, flight.destination, flight.date, flight.seats_available);
    }

    close(fd);
}

// Fonction pour r�server un si�ge
void reserve_seat() {
    Flight flight;
    char code[10];
    int fd = open(FILENAME, O_RDWR);
    if (fd == -1) {
        perror("Erreur d'ouverture du fichier");
        return;
    }

    printf("Entrez le code du vol : ");
    scanf("%s", code);

    int found = 0;
    while (read(fd, &flight, sizeof(Flight)) > 0) {
        if (strcmp(flight.code, code) == 0) {
            found = 1;
            if (flight.seats_available > 0) {
                flight.seats_available--;
                lseek(fd, -sizeof(Flight), SEEK_CUR);
                if (write(fd, &flight, sizeof(Flight)) == -1) {
                    perror("Erreur d'�criture dans le fichier");
                } else {
                    printf("R�servation r�ussie !\n");
                }
            } else {
                printf("Aucun si�ge disponible pour ce vol.\n");
            }
            break;
        }
    }

    if (!found) {
        printf("Vol non trouv�.\n");
    }

    close(fd);
}

// Fonction pour annuler une r�servation
void cancel_reservation() {
    Flight flight;
    char code[10];
    int fd = open(FILENAME, O_RDWR);
    if (fd == -1) {
        perror("Erreur d'ouverture du fichier");
        return;
    }

    printf("Entrez le code du vol : ");
    scanf("%s", code);

    int found = 0;
    while (read(fd, &flight, sizeof(Flight)) > 0) {
        if (strcmp(flight.code, code) == 0) {
            found = 1;
            flight.seats_available++;
            lseek(fd, -sizeof(Flight), SEEK_CUR);
            if (write(fd, &flight, sizeof(Flight)) == -1) {
                perror("Erreur d'�criture dans le fichier");
            } else {
                printf("Annulation r�ussie !\n");
            }
            break;
        }
    }

    if (!found) {
        printf("Vol non trouv�.\n");
    }

    close(fd);
}

// Menu principal
void menu() {
    int choice;
    do {
        printf("\n=== Syst�me de r�servation de vols ===\n");
        printf("1. Ajouter un vol\n");
        printf("2. R�server un si�ge\n");
        printf("3. Annuler une r�servation\n");
        printf("4. Afficher les vols\n");
        printf("5. Quitter\n");
        printf("Votre choix : ");
        scanf("%d", &choice);

        switch (choice) {
            case 1:
                add_flight();
                break;
            case 2:
                reserve_seat();
                break;
            case 3:
                cancel_reservation();
                break;
            case 4:
                display_flights();
                break;
            case 5:
                printf("Au revoir !\n");
                break;
            default:
                printf("Choix invalide, veuillez r�essayer.\n");
        }
    } while (choice != 5);
}

int main() {
    menu();
    return 0;
}
