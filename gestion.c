#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#define MAX 256
#define MAX_c 8000

typedef struct Client {
    char id[6] ; 
    char prenom[30] ; 
    char nom[30] ; 
    char age[30] ; 
    char nbPersonnes[30] ;
    char nbJours[30] ; 
    char pays[50] ;
    char departement[50] ;
} Client;

int initialisation(char *csvName, Client clients[MAX_c]) { //Mets à jour les donner du tableau avant/pendant/après modification du csv.
    int k = 1;
    FILE *fic = fopen(csvName, "r") ; 
        char text[255];  // Initialisation d'une chaine de caractère qui va stocker le contenu d'une ligne du csv.
        int row = 0, count = 0; // Initialisation de la ligne de départ à 0 et d'un compteur pour permettre de se repérer s'il s'agit du nom, du prénom etc...
        char row_c[10] = "id";
        char delim[] = ","; // La délimitation entre les valeurs est ','.
        while(fgets(text,254, fic) != NULL) {
            char *ptr = strtok(text,delim) ; 
            while (ptr != NULL) {
                // 0: Nom, 1: Prénom, 2: Age, 3: Nombre de personnes, 4: Nombre de jours, 5: Le pays, 6: Le département.
                if (row != 0) {
                    sprintf(row_c, "%d", row) ; 
                }
                strcpy(clients[row].id, row_c) ;
                if (count == 0) strcpy(clients[row].prenom, ptr);
                else if (count == 1) strcpy(clients[row].nom, ptr);
                else if (count == 2) strcpy(clients[row].age, ptr);
                else if (count == 3) strcpy(clients[row].nbPersonnes, ptr);
                else if (count == 4) strcpy(clients[row].nbJours, ptr);
                else if (count == 5) strcpy(clients[row].pays, ptr);
                else if (count == 6) strcpy(clients[row].departement, ptr);
                ptr = strtok(NULL, delim) ; 
                count++ ;
            }
            row += 1, count = 0;
        } 
    fclose(fic) ;
    return row ; 
}

int deleteUser(char *csvName, int idToDelete, Client clients[]) {
    char ligne[MAX], ligneCpy[MAX];
    int valid = 0 ;
    FILE *oldCsv = fopen(csvName, "r");
    FILE *newCsv = fopen("temp.csv", "w");
    int count = 0;
    if (!(oldCsv || newCsv)) exit(1);
    while(fgets(ligne, MAX-1, oldCsv) != NULL) {
        if (count != idToDelete) {
            fputs(ligne, newCsv);
        } else {
            valid = -1; 
        }
        count++;
    }
    fclose(oldCsv);
    fclose(newCsv);
    remove(csvName);
    rename("temp.csv", csvName);
    initialisation(csvName, clients) ; 
    return valid ; 
}

void addUser(char *csvName, int taille, Client clients[MAX_c]) {
    FILE *csvFile = fopen(csvName, "a");
    printf("\033[0m");
    //Moyen de réduire ça ?
    sprintf(clients[taille].id, "%d", taille) ; 
    printf("\nQuel est le prénom : ");
    scanf("%s",clients[taille].prenom);
    printf("\nQuel est le nom : ");
    scanf("%s",clients[taille].nom);
    printf("\nQuel est l'âge : ");
    scanf("%s",clients[taille].age);
    printf("\nCombien de personnes : ");
    scanf("%s",clients[taille].nbPersonnes);
    printf("\nCombien de jours de vacances : ");
    scanf("%s",clients[taille].nbJours);
    printf("\nDe quel pays provient la personne : ");
    scanf("%s",clients[taille].pays);

    if (strcmp(clients[taille].pays, "France") == 0) {
        printf("\nDépartement : ");
        scanf("%s",clients[taille].departement);
    } else {
        strcpy(clients[taille].departement, "0\0"); 
    }

    fprintf(csvFile,"\n%s,%s,%s,%s,%s,%s,%s", clients[taille].prenom, clients[taille].nom, clients[taille].age, 
    clients[taille].nbPersonnes, clients[taille].nbJours, clients[taille].pays, 
    clients[taille].departement);
    fclose(csvFile);
    initialisation(csvName, clients) ; 
}

void generateTable(Client clients[MAX_c], int taille, char *lastNameToFind, char *firstNameToFind) { //Affiche tout le CSV dans la console 
int k = -1 ; 
    for (int i = 0; i < taille; i++) {
        if((strcmp(lastNameToFind, clients[i].nom) == 0 && strcmp(firstNameToFind,clients[i].prenom) == 0) || strcmp(lastNameToFind, "0") == 0 || strcmp(firstNameToFind, "0") == 0|| i == 0) {
            printf("\n====== =========== =========== ==== ============ ======== ================ ================");
            printf("\n%s", clients[i].id);
            for(int k = strlen(clients[i].id); k <= 5; k++) printf(" ");  

            printf(" %s", clients[i].prenom);
            for(int k = strlen(clients[i].prenom); k < 11; k++) printf(" ");

            printf(" %s", clients[i].nom); 
            for(int k = strlen(clients[i].nom); k < 11; k++) printf(" ");

            printf(" %s", clients[i].age);
            for(int k = strlen(clients[i].age); k < 4; k++) printf(" ");

            printf(" %s", clients[i].nbPersonnes);
            for(int k = strlen(clients[i].nbPersonnes); k < 12; k++) printf(" ");

            printf(" %s", clients[i].nbJours);
            for(int k = strlen(clients[i].nbJours); k < 8; k++) printf(" ");

            printf(" %s", clients[i].pays);
            for(int k = strlen(clients[i].pays); k < 16; k++) printf(" ");

            printf(" %s", clients[i].departement);   
            k++ ;
        }   
    }

    printf("\033[0;32m"); // Police verte
    printf("\n\n\nIl y a %d résultats.\n\n", k) ;
    printf("\033[0m");

}

void triClients(Client clients[], int taille, int choice) {
    Client clientTemporaire; 
    if (choice == 1) {
        for (int i = 1; i < taille; i++) {
            for (int j = 1; j < taille; j++) {
            if (strcmp(clients[i].nom, clients[j].nom) < 0) {
                clientTemporaire = clients[i];
                clients[i] = clients[j];
                clients[j] = clientTemporaire;
                }
            }
        }
    } else {
        for (int i = 1; i < taille; i++) {
            for (int j = 1; j < taille; j++) {
            if (strcmp(clients[i].pays, clients[j].pays) < 0) {
                clientTemporaire = clients[i];
                clients[i] = clients[j];
                clients[j] = clientTemporaire;
                }
            }
        }        
    }
    generateTable(clients, taille, "0", "0") ;
}

void fusion(Client clients[], int begin, int end, int taille, int choice) {
    if (choice == 1) {
        if (end - begin != 0) {
            Client insert_val ; 
            int center = (begin+end)/2 ;
            fusion(clients, begin, center, taille, choice) ;
            fusion(clients, center+1, end, taille, choice) ;
            int left = begin ; 
            int right = center+1 ; 
            while ((right<=end) && (left < right)) {
                // if (clients[right].nom > clients[left].nom) {
                if (strcmp(clients[left].nom,clients[right].nom) > 0) {
                    left++ ;
                }  
                else {
                    insert_val = clients[right] ; 
                    for(int i=right; i>left; i--) {
                        clients[i] = clients[i-1] ;
                    }
                    clients[left] = insert_val ;
                    left++ ; right++ ;
                }
            }
        }
    } else {
        if (end - begin != 0) {
            Client insert_val ; 
            int center = (begin+end)/2 ;
            fusion(clients, begin, center, taille, choice) ;
            fusion(clients, center+1, end, taille, choice) ;
            int left = begin ; 
            int right = center+1 ; 
            while ((right<=end) && (left < right)) {
                // if (clients[right].nom > clients[left].nom) {
                if (strcmp(clients[left].pays,clients[right].pays) > 0) {
                    left++ ;
                }  
                else {
                    insert_val = clients[right] ; 
                    for(int i=right; i>left; i--) {
                        clients[i] = clients[i-1] ;
                    }
                    clients[left] = insert_val ;
                    left++ ; right++ ;
                }
            }
        }      
    }
}

int main() {
    int choice = 0; // C'est la condition qui détermine ce que nous voulons faire.
    int run ; // C'est la condition qui détermine si nous voulons continuer à utiliser le programme ou non.
    char csvName[MAX];
    int taille ; 
    int valid = 0 ;
    printf("\n\n\n\n\n\n\n\n\n\n\n\n") ;
    printf("Bonjour ! Bienvenue dans le gestionnaire de statistiques.\n\n");
    printf("Il y a plusieurs règles à respecter : \n");
    printf("1) Ne pas mettre d'accent.\n") ;
    printf("2) Et toutes les informations doivent être présentes.\n\n") ;
    do {
        printf("Quel est le nom de votre fichier ?\n") ;
        scanf("%s", csvName) ; 
        strcat(csvName, ".csv") ;
        FILE *fic = fopen(csvName, "r");
        if (fic != 0) {
            valid = 1 ;
        } else {
            printf("\033[0;31m");
            printf("\n\n\n\nCe nom de fichier est invalide.\n") ;
            printf("\033[0m");
            printf("1) Il doit être de l'extension .csv\n") ;
            printf("2) Ne mettez pas .csv à la fin.\n") ;
            printf("3) Le fichier doit exister.\n\n") ;
        }
        fclose(fic) ; 
    } while(valid==0) ;

    Client clients[MAX_c] ; 
    taille = initialisation(csvName, clients) ;

    do {
        // Affichage du menu principal
        printf("----------------------------------------------\n\n") ;
        printf("Que voulez-vous faire ?\n");
        printf("- Quitter le logiciel (0)\n");
        printf("- Lire tout le dossier client (1)\n");
        printf("- Ajouter ou supprimer un client (2)\n");
        printf("- Faire une recherche (3)\n");
        printf("- Trier le fichier (4)\n");
        printf("Rentrer votre choix : ");
        scanf("%d", &choice);

        if ((choice > 0) && (choice < 5)) {
            if (choice == 1) { // Lecture totale
                generateTable(clients, taille, "0", "0") ; 
            } else if (choice == 2) { // Ajout/Suppression
                int bin = 0;
                printf("\nVous voulez ajouter ou supprimer un client ? ") ;

                do { //Contrôle de saisie
                    printf("1. Ajout \n2. Suppresion\n") ;
                    scanf("%d", &bin) ;
                } while ((bin != 1) && (bin != 2)) ;

                if (bin == 1) { //Ajout
                    taille++ ;
                    addUser(csvName, taille, clients) ; 
                } else if(bin == 2) { //Supression
                    char name[30];
                    int delID;
                    printf("\n\n\n") ;
                    printf("\nQuel nom voulez-vous chercher ?  ") ;
                    scanf("%s", name) ;
                    generateTable(clients, taille, name, "0") ; 
                    printf("\nQuel ID voulez-vous supprimer ?  ") ;
                    scanf("%d", &delID) ; 
                    taille += deleteUser(csvName, delID, clients) ; 
                    initialisation(csvName, clients) ; 
                }

            } else if (choice == 3) { // Recherche.
                char lastName[30], firstName[30] ; 
                char *delID ;
                printf("\n\n\n") ;
                printf("\nQuel nom voulez-vous chercher ?  ") ;
                scanf("%s", lastName) ;
                printf("\nQuele prénom voulez-vous chercher ? ");
                scanf("%s", firstName) ; 
                generateTable(clients, taille, lastName, firstName) ; 
            } else if (choice == 4) {
                int oneOrTwo ;
                printf("\n\n\nVoulez-vous trier par nom ou par pays ?\n") ; 
                printf("1. Nom \n2. Pays\n") ;
                scanf("%d",&oneOrTwo) ; 
                if (oneOrTwo == 1) {
                    fusion(clients, 1, taille-1, taille, oneOrTwo) ; 
                } else if (oneOrTwo == 2) {
                    fusion(clients, 1, taille-1, taille, oneOrTwo) ; 
                }
                generateTable(clients, taille, "0", "0") ; 

            }

            printf("\n----------------------------------------------");
            printf("\nVoulez-vous faire autre chose ? (0 pour non, 1 pour oui)\n"); // Condition de sortie
            scanf("%d", &run) ;
            printf("\n\n") ;
        } else {
            if (choice == 0) {
                run = 0 ;
            }
            if (choice > 3) {
                printf("\033[0;31m"); // Police rouge
                printf("\n\nVous n'avez pas rentrer un choix existant ! Veuillez saisir de nouveau votre choix.\n\n") ;
                printf("\033[0m"); // Police blanche
            }
        }

    } while (run == 1) ;

    printf("\033[0;32m") ; //Police verte
    printf("\nMerci et au revoir !\n\n") ;
    return 0 ;
}