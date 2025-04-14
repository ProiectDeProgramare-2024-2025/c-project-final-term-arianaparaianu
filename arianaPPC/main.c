#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define FILE_NAME "flota_auto.txt"

typedef struct {
    char marca[50];
    char tip[50];
    char numar_inmatriculare[20];
} Autovehicul;

void afiseaza_autovehicule();
void adauga_autovehicul();
void cauta_autovehicul();
void sterge_autovehicul();

void afiseaza_meniu() {
    printf("\nMeniu:\n");
    printf("1. Afiseaza autovehicule\n");
    printf("2. Adauga autovehicul\n");
    printf("3. Cauta autovehicul\n");
    printf("4. Sterge autovehicul\n");
    printf("5. Iesire\n");
    printf("Alegeti o optiune: ");
}

int main() {
    int optiune;

    do {
        afiseaza_meniu();
        scanf("%d", &optiune);
        getchar(); // curata bufferul

        switch(optiune) {
            case 1: afiseaza_autovehicule(); break;
            case 2: adauga_autovehicul(); break;
            case 3: cauta_autovehicul(); break;
            case 4: sterge_autovehicul(); break;
            case 5: printf("Iesire...\n"); break;
            default: printf("Optiune invalida!\n");
        }
    } while(optiune != 5);

    return 0;
}

void afiseaza_autovehicule() {
    FILE *f = fopen(FILE_NAME, "r");
    if (!f) {
        printf("Eroare la deschiderea fisierului sau fisierul nu exista!\n");
        return;
    }

    Autovehicul a;
    printf("\nLista autovehicule:\n");
    while (fscanf(f, "%49[^,],%49[^,],%19[^\n]\n", a.marca, a.tip, a.numar_inmatriculare) == 3) {
        printf("Marca: %s, Tip: %s, Nr. Inmatriculare: %s\n", a.marca, a.tip, a.numar_inmatriculare);
    }
    fclose(f);
}

void adauga_autovehicul() {
    FILE *f = fopen(FILE_NAME, "a");
    if (!f) {
        printf("Eroare la deschiderea fisierului!\n");
        return;
    }

    Autovehicul a;
    printf("Introduceti marca: ");
    fgets(a.marca, 50, stdin);
    strtok(a.marca, "\n");

    printf("Introduceti tipul vehiculului: ");
    fgets(a.tip, 50, stdin);
    strtok(a.tip, "\n");

    printf("Introduceti numarul de inmatriculare: ");
    fgets(a.numar_inmatriculare, 20, stdin);
    strtok(a.numar_inmatriculare, "\n");

    fprintf(f, "%s,%s,%s\n", a.marca, a.tip, a.numar_inmatriculare);
    fclose(f);

    printf("Autovehicul adaugat cu succes!\n");
}

void cauta_autovehicul() {
    char numar[20];
    printf("Introduceti numarul de inmatriculare: ");
    fgets(numar, 20, stdin);
    strtok(numar, "\n");

    FILE *f = fopen(FILE_NAME, "r");
    if (!f) {
        printf("Fisierul nu exista!\n");
        return;
    }

    Autovehicul a;
    int gasit = 0;
    while (fscanf(f, "%49[^,],%49[^,],%19[^\n]\n", a.marca, a.tip, a.numar_inmatriculare) == 3) {
        if (strcmp(a.numar_inmatriculare, numar) == 0) {
            printf("Autovehicul gasit: Marca: %s, Tip: %s, Nr. Inmatriculare: %s\n", a.marca, a.tip, a.numar_inmatriculare);
            gasit = 1;
            break;
        }
    }
    fclose(f);
    if (!gasit) printf("Autovehiculul nu a fost gasit!\n");
}

void sterge_autovehicul() {
    char numar[20];
    printf("Introduceti numarul de inmatriculare al masinii de sters: ");
    fgets(numar, 20, stdin);
    strtok(numar, "\n");

    FILE *f = fopen(FILE_NAME, "r");
    if (!f) {
        printf("Fisierul nu exista!\n");
        return;
    }

    FILE *temp = fopen("temp.txt", "w");
    if (!temp) {
        printf("Eroare la crearea fisierului temporar!\n");
        fclose(f);
        return;
    }

    Autovehicul a;
    int gasit = 0;
    while (fscanf(f, "%49[^,],%49[^,],%19[^\n]\n", a.marca, a.tip, a.numar_inmatriculare) == 3) {
        if (strcmp(a.numar_inmatriculare, numar) != 0) {
            fprintf(temp, "%s,%s,%s\n", a.marca, a.tip, a.numar_inmatriculare);
        } else {
            gasit = 1;
        }
    }
    fclose(f);
    fclose(temp);

    remove(FILE_NAME);
    rename("temp.txt", FILE_NAME);

    if (gasit)
        printf("Autovehicul sters cu succes!\n");
    else
        printf("Autovehiculul nu a fost gasit!\n");
}
