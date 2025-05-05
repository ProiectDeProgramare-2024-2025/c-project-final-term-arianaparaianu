#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*--------------------------------------------------*/
/* Detectăm platforma și configurăm ecranul & culori */
/*--------------------------------------------------*/
#ifdef _WIN32
    #ifndef WIN32_LEAN_AND_MEAN
    #define WIN32_LEAN_AND_MEAN
    #endif
    #include <windows.h>
    #ifndef ENABLE_VIRTUAL_TERMINAL_PROCESSING
    #define ENABLE_VIRTUAL_TERMINAL_PROCESSING 0x0004
    #endif
    #define CLEAR_SCREEN "cls"
#else
    #define CLEAR_SCREEN "clear"
#endif

/*----------------------------*/
/*        ANSI CULORI         */
/*----------------------------*/
#define COLOR_RESET      "\033[0m"
#define COLOR_GREEN      "\033[0;32m"
#define COLOR_RED        "\033[0;31m"
#define COLOR_LIGHT_BLUE "\033[0;94m"

#define FILE_NAME "flota_auto.txt"

typedef struct {
    char marca[50];
    char tip[50];
    char numar_inmatriculare[20];
} Autovehicul;

/*--------------------------------------------------*/
/*          Protocoale funcţii (same as original)   */
/*--------------------------------------------------*/
static void afiseaza_autovehicule(void);
static void adauga_autovehicul(void);
static void cauta_autovehicul(void);
static void sterge_autovehicul(void);

/*----------------------------*/
/*       Utilitare afişare    */
/*----------------------------*/
static void printTitle(const char *title)
{
    printf(COLOR_LIGHT_BLUE "\n===== %s =====\n" COLOR_RESET, title);
}

static void pauseAndReturn(void)
{
    puts("\nApasati Enter pentru a reveni...");
    getchar();
}

static void afiseaza_meniu(void)
{
    printTitle("Meniu Principal");
    puts("1. Afiseaza autovehicule");
    puts("2. Adauga autovehicul");
    puts("3. Cauta autovehicul");
    puts("4. Sterge autovehicul");
    puts("5. Iesire");
    printf("Alegeti o optiune: ");
}

/*----------------------------*/
/*             MAIN           */
/*----------------------------*/
int main(void)
{
#ifdef _WIN32
    /* Activăm secvenţele ANSI pe Windows 10+ */
    HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
    if (hOut != INVALID_HANDLE_VALUE) {
        DWORD dwMode = 0;
        if (GetConsoleMode(hOut, &dwMode)) {
            SetConsoleMode(hOut, dwMode | ENABLE_VIRTUAL_TERMINAL_PROCESSING);
        }
    }
#endif

    int optiune;
    do {
        afiseaza_meniu();
        if (scanf("%d", &optiune) != 1) {
            int c; while ((c = getchar()) != '\n' && c != EOF) {}
            optiune = 0;
        }
        getchar(); /* curăţă newline‑ul după scanf */

        switch(optiune) {
            case 1: afiseaza_autovehicule(); break;
            case 2: adauga_autovehicul();    break;
            case 3: cauta_autovehicul();     break;
            case 4: sterge_autovehicul();    break;
            case 5: puts("Iesire...");       break;
            default: puts("Optiune invalida!"); pauseAndReturn();
        }
    } while(optiune != 5);

    return 0;
}

/*----------------------------*/
/*      Implementări CRUD     */
/*----------------------------*/
static void afiseaza_autovehicule(void)
{
    system(CLEAR_SCREEN);
    printTitle("Lista Autovehicule");

    FILE *f = fopen(FILE_NAME, "r");
    if (!f) {
        puts(COLOR_RED "Eroare la deschiderea fisierului sau fisierul nu exista!" COLOR_RESET);
        pauseAndReturn();
        return;
    }

    Autovehicul a;
    int gasit = 0;
    while (fscanf(f, "%49[^,],%49[^,],%19[^\n]\n", a.marca, a.tip, a.numar_inmatriculare) == 3) {
        printf(COLOR_GREEN "Marca: %s, Tip: %s, Nr. Inmatriculare: %s" COLOR_RESET "\n",
               a.marca, a.tip, a.numar_inmatriculare);
        gasit = 1;
    }
    fclose(f);
    if (!gasit) puts("Nu exista inca autovehicule in flota.");

    pauseAndReturn();
}

static void adauga_autovehicul(void)
{
    system(CLEAR_SCREEN);
    printTitle("Adauga Autovehicul");

    FILE *f = fopen(FILE_NAME, "a");
    if (!f) {
        puts(COLOR_RED "Eroare la deschiderea fisierului!" COLOR_RESET);
        pauseAndReturn();
        return;
    }

    Autovehicul a;
    printf("Introduceti marca: ");
    fgets(a.marca, sizeof a.marca, stdin);
    strtok(a.marca, "\n");

    printf("Introduceti tipul vehiculului: ");
    fgets(a.tip, sizeof a.tip, stdin);
    strtok(a.tip, "\n");

    printf("Introduceti numarul de inmatriculare: ");
    fgets(a.numar_inmatriculare, sizeof a.numar_inmatriculare, stdin);
    strtok(a.numar_inmatriculare, "\n");

    fprintf(f, "%s,%s,%s\n", a.marca, a.tip, a.numar_inmatriculare);
    fclose(f);

    puts(COLOR_GREEN "Autovehicul adaugat cu succes!" COLOR_RESET);
    pauseAndReturn();
}

static void cauta_autovehicul(void)
{
    system(CLEAR_SCREEN);
    printTitle("Cauta Autovehicul");

    char numar[20];
    printf("Introduceti numarul de inmatriculare: ");
    fgets(numar, sizeof numar, stdin);
    strtok(numar, "\n");

    FILE *f = fopen(FILE_NAME, "r");
    if (!f) {
        puts(COLOR_RED "Fisierul nu exista!" COLOR_RESET);
        pauseAndReturn();
        return;
    }

    Autovehicul a;
    int gasit = 0;
    while (fscanf(f, "%49[^,],%49[^,],%19[^\n]\n", a.marca, a.tip, a.numar_inmatriculare) == 3) {
        if (strcmp(a.numar_inmatriculare, numar) == 0) {
            printf(COLOR_GREEN "Autovehicul gasit: Marca: %s, Tip: %s, Nr. Inmatriculare: %s" COLOR_RESET "\n",
                   a.marca, a.tip, a.numar_inmatriculare);
            gasit = 1;
            break;
        }
    }
    fclose(f);
    if (!gasit) puts(COLOR_RED "Autovehiculul nu a fost gasit!" COLOR_RESET);

    pauseAndReturn();
}

static void sterge_autovehicul(void)
{
    system(CLEAR_SCREEN);
    printTitle("Sterge Autovehicul");

    char numar[20];
    printf("Introduceti numarul de inmatriculare al masinii de sters: ");
    fgets(numar, sizeof numar, stdin);
    strtok(numar, "\n");

    FILE *f = fopen(FILE_NAME, "r");
    if (!f) {
        puts(COLOR_RED "Fisierul nu exista!" COLOR_RESET);
        pauseAndReturn();
        return;
    }

    FILE *temp = fopen("temp.txt", "w");
    if (!temp) {
        puts(COLOR_RED "Eroare la crearea fisierului temporar!" COLOR_RESET);
        fclose(f);
        pauseAndReturn();
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
        puts(COLOR_GREEN "Autovehicul sters cu succes!" COLOR_RESET);
    else
        puts(COLOR_RED "Autovehiculul nu a fost gasit!" COLOR_RESET);

    pauseAndReturn();
}
