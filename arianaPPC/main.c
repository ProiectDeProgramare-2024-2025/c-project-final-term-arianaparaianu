#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#if !defined(_WIN32)
#include <strings.h>   /* pentru strcasecmp pe POSIX */
#else
#define strcasecmp _stricmp /* compatibilitate Windows */
#endif

#define FILE_NAME "flota_auto.txt"

/* ====== CULORI ANSI ====== */
#define COLOR_RESET   "\033[0m"
#define COLOR_MENU    "\033[1;36m"
#define COLOR_ITEM    "\033[0;36m"
#define COLOR_PROMPT  "\033[0;33m"
#define COLOR_OK      "\033[0;32m"
#define COLOR_ERR     "\033[0;31m"
#define COLOR_INFO    "\033[1;34m"

#define COLOR_BRAND  "\033[1;36m"  /* cyan deschis */
#define COLOR_TYPE   "\033[0;33m"  /* galben  */
#define COLOR_PLATE  "\033[0;32m"  /* verde   */

/* ====== STRUCTURA DATELOR ====== */
typedef struct {
    char marca[50];
    char tip[50];
    char numar_inmatriculare[20];
} Autovehicul;

/* ====== PROTOTIPURI ====== */
void afiseaza_meniu(void);
void afiseaza_autovehicule(void);
void adauga_autovehicul(void);
void cauta_autovehicul(void);
void actualizeaza_autovehicul(void);
void sterge_autovehicul(void);
int  e_back(const char *s);
void clear_line(char *s);

/* ============================================================= */
int main(void) {
#ifdef _WIN32
    system("");
#endif
    int optiune = 0;

    while (1) {
        afiseaza_meniu();
        if (scanf("%d", &optiune) != 1) {
            fprintf(stderr, COLOR_ERR "Eroare la citirea optiunii!\n" COLOR_RESET);
            return 1;
        }
        getchar();

        switch (optiune) {
            case 1: afiseaza_autovehicule();    break;
            case 2: adauga_autovehicul();       break;
            case 3: cauta_autovehicul();        break;
            case 4: actualizeaza_autovehicul(); break;
            case 5: sterge_autovehicul();       break;
            case 6: puts(COLOR_INFO "Iesire..." COLOR_RESET); return 0;
            default: puts(COLOR_ERR "Optiune invalida!" COLOR_RESET); break;
        }
    }
}

/* ============================================================= */
/* HELPER FUNCTIONS                                              */
/* ============================================================= */


void clear_line(char *s) {
    size_t len = strlen(s);
    if (len && s[len - 1] == '\n') s[len - 1] = '\0';
}


int e_back(const char *s) {
    if (!s) return 0;
    while (*s && isspace((unsigned char)*s)) ++s; /* ignora spatii initiale */
    return strcasecmp(s, "back") == 0 || strcasecmp(s, "b") == 0;
}

/* ============================================================= */
/* FUNCTII MENIU & OPERATII                                      */
/* ============================================================= */

void afiseaza_meniu(void) {
    printf(COLOR_MENU "\n==================== MENIU PRINCIPAL ====================\n" COLOR_RESET);
    printf(COLOR_ITEM "1. Afiseaza autovehicule\n" COLOR_RESET);
    printf(COLOR_ITEM "2. Adauga autovehicul\n" COLOR_RESET);
    printf(COLOR_ITEM "3. Cauta autovehicul\n" COLOR_RESET);
    printf(COLOR_ITEM "4. Actualizeaza numar de inmatriculare\n" COLOR_RESET);
    printf(COLOR_ITEM "5. Sterge autovehicul\n" COLOR_RESET);
    printf(COLOR_ITEM "6. Iesire\n" COLOR_RESET);
    printf(COLOR_PROMPT "Alege o optiune: " COLOR_RESET);
}

void afiseaza_autovehicule(void) {
    FILE *f = fopen(FILE_NAME, "r");
    if (!f) {
        puts(COLOR_ERR "Eroare la deschiderea fisierului sau fisierul nu exista!" COLOR_RESET);
        return;
    }

    Autovehicul a;
    puts(COLOR_INFO "\nLista autovehicule:" COLOR_RESET);

    while (fscanf(f, "%49[^,],%49[^,],%19[^\n]\n",
                  a.marca, a.tip, a.numar_inmatriculare) == 3) {
        printf(" • Marca: "  COLOR_BRAND "%-12s" COLOR_RESET
               " | Tip: "    COLOR_TYPE  "%-10s" COLOR_RESET
               " | Nr.: "    COLOR_PLATE "%s"    COLOR_RESET
               "\n",
               a.marca, a.tip, a.numar_inmatriculare);
    }
    fclose(f);

    puts(COLOR_PROMPT "\n(Enter pentru meniu principal)" COLOR_RESET);
    getchar();
}


void adauga_autovehicul(void) {
    Autovehicul a;

    printf(COLOR_PROMPT "\nIntrodu marca (sau 'back'): " COLOR_RESET);
    if (!fgets(a.marca, sizeof a.marca, stdin)) return; clear_line(a.marca);
    if (e_back(a.marca)) return;

    printf(COLOR_PROMPT "Introdu tipul (sau 'back'): " COLOR_RESET);
    if (!fgets(a.tip, sizeof a.tip, stdin)) return; clear_line(a.tip);
    if (e_back(a.tip)) return;

    printf(COLOR_PROMPT "Introdu numarul de inmatriculare (sau 'back'): " COLOR_RESET);
    if (!fgets(a.numar_inmatriculare, sizeof a.numar_inmatriculare, stdin)) return; clear_line(a.numar_inmatriculare);
    if (e_back(a.numar_inmatriculare)) return;

    FILE *f = fopen(FILE_NAME, "a");
    if (!f) {
        puts(COLOR_ERR "Eroare la deschiderea fisierului!" COLOR_RESET);
        return;
    }
    fprintf(f, "%s,%s,%s\n", a.marca, a.tip, a.numar_inmatriculare);
    fclose(f);

    puts(COLOR_OK "Autovehicul adaugat cu succes!" COLOR_RESET);
}

void cauta_autovehicul(void) {
    char numar[20];
    printf(COLOR_PROMPT "\nIntrodu numarul de inmatriculare (sau 'back'): " COLOR_RESET);
    if (!fgets(numar, sizeof numar, stdin)) return; clear_line(numar);
    if (e_back(numar)) return;

    FILE *f = fopen(FILE_NAME, "r");
    if (!f) { puts(COLOR_ERR "Fisierul nu exista!" COLOR_RESET); return; }

    Autovehicul a; int gasit = 0;
    while (fscanf(f, "%49[^,],%49[^,],%19[^\n]\n", a.marca, a.tip, a.numar_inmatriculare) == 3) {
        if (strcmp(a.numar_inmatriculare, numar) == 0) {
            printf(COLOR_OK "Gasit: Marca=%s, Tip=%s, Nr=%s\n" COLOR_RESET, a.marca, a.tip, a.numar_inmatriculare);
            gasit = 1; break;
        }
    }
    fclose(f);
    if (!gasit) puts(COLOR_ERR "Autovehiculul nu a fost gasit!" COLOR_RESET);
}

void actualizeaza_autovehicul(void) {
    char numar_vechi[20];

    /* ── 1. Citește numărul de înmatriculare ce trebuie modificat ── */
    printf(COLOR_PROMPT "\nNumarul de inmatriculare de modificat (sau 'back'): " COLOR_RESET);
    if (!fgets(numar_vechi, sizeof numar_vechi, stdin)) return;
    clear_line(numar_vechi);
    if (e_back(numar_vechi)) return;

    /* ── 2. Verifică dacă numărul există în fișier ── */
    FILE *f = fopen(FILE_NAME, "r");
    if (!f) { puts(COLOR_ERR "Fisierul nu exista!" COLOR_RESET); return; }

    Autovehicul a;
    int gasit = 0;
    while (fscanf(f, "%49[^,],%49[^,],%19[^\n]\n",
                  a.marca, a.tip, a.numar_inmatriculare) == 3) {
        if (strcmp(a.numar_inmatriculare, numar_vechi) == 0) {
            gasit = 1;
            break;
        }
    }
    fclose(f);

    if (!gasit) {
        puts(COLOR_ERR "Autovehiculul nu a fost gasit!" COLOR_RESET);
        return;
    }

    /* ── 3. Dacă numărul există, cere numărul nou ── */
    char numar_nou[20];
    printf(COLOR_PROMPT "Introdu noul numar (sau 'back'): " COLOR_RESET);
    if (!fgets(numar_nou, sizeof numar_nou, stdin)) return;
    clear_line(numar_nou);
    if (e_back(numar_nou)) return;

    /* ── 4. Rescrie fișierul cu numărul actualizat ── */
    f = fopen(FILE_NAME, "r");
    if (!f) { puts(COLOR_ERR "Fisierul nu exista!" COLOR_RESET); return; }

    FILE *temp = fopen("temp.txt", "w");
    if (!temp) {
        puts(COLOR_ERR "Eroare la crearea fisierului temporar!" COLOR_RESET);
        fclose(f);
        return;
    }

    while (fscanf(f, "%49[^,],%49[^,],%19[^\n]\n",
                  a.marca, a.tip, a.numar_inmatriculare) == 3) {
        if (strcmp(a.numar_inmatriculare, numar_vechi) == 0) {
            strncpy(a.numar_inmatriculare, numar_nou,
                    sizeof a.numar_inmatriculare - 1);
            a.numar_inmatriculare[sizeof a.numar_inmatriculare - 1] = '\0';
        }
        fprintf(temp, "%s,%s,%s\n", a.marca, a.tip, a.numar_inmatriculare);
    }

    fclose(f);
    fclose(temp);
    remove(FILE_NAME);
    rename("temp.txt", FILE_NAME);

    puts(COLOR_OK "Numarul de inmatriculare a fost actualizat cu succes!" COLOR_RESET);
}


void sterge_autovehicul(void) {
    char numar[20];
    printf(COLOR_PROMPT "\nNumarul de inmatriculare de sters (sau 'back'): " COLOR_RESET);
    if (!fgets(numar, sizeof numar, stdin)) return; clear_line(numar);
    if (e_back(numar)) return;

    FILE *f = fopen(FILE_NAME, "r");
    if (!f) { puts(COLOR_ERR "Fisierul nu exista!" COLOR_RESET); return; }

    FILE *temp = fopen("temp.txt", "w");
    if (!temp) {
        puts(COLOR_ERR "Eroare la crearea fisierului temporar!" COLOR_RESET);
        fclose(f); return;
    }

    Autovehicul a; int gasit = 0;
    while (fscanf(f, "%49[^,],%49[^,],%19[^\n]\n", a.marca, a.tip, a.numar_inmatriculare) == 3) {
        if (strcmp(a.numar_inmatriculare, numar) != 0) {
            fprintf(temp, "%s,%s,%s\n", a.marca, a.tip, a.numar_inmatriculare);
        } else gasit = 1;
    }
    fclose(f); fclose(temp);

    remove(FILE_NAME);
    rename("temp.txt", FILE_NAME);

    if (gasit) puts(COLOR_OK "Autovehicul sters cu succes!" COLOR_RESET);
    else       puts(COLOR_ERR "Autovehiculul nu a fost gasit!" COLOR_RESET);
}
