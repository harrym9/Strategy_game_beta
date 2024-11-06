#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <graphics.h>
#include <conio.h>

#define MAX_LINE_LENGTH 50
#define MAX_HEROES 10
#define MAX_MONSTERS 10
#define MAX_NAME_LENGTH 50
#define CELL_SIZE 40
#define ROWS 20
#define COLS 20

typedef struct {
    int piyadeler;
    int suvariler;
    int okcular;
    int kusatma_makineleri;
} Birimler;

typedef struct {
    char saldiri_gelistirmesi;
    char savunma_ustaligi;
    char elit_egitim;
    char kusatma_ustaligi;
} ArastirmaSeviyesi;

typedef struct {
    Birimler birimler;
    char kahramanlar[MAX_HEROES][MAX_NAME_LENGTH];
    int kahraman_sayisi;
    char canavarlar[MAX_MONSTERS][MAX_NAME_LENGTH];
    int canavar_sayisi;
    ArastirmaSeviyesi arastirma_seviyesi;
} InsanImparatorlugu;

typedef struct {
    int ork_dovusculeri;
    int mizrakcilar;
    int varg_binicileri;
    int troller;
} OrkBirimler;

typedef struct {
    OrkBirimler birimler;
    char kahramanlar[MAX_HEROES][MAX_NAME_LENGTH];
    int kahraman_sayisi;
    char canavarlar[MAX_MONSTERS][MAX_NAME_LENGTH];
    int canavar_sayisi;
    ArastirmaSeviyesi arastirma_seviyesi;
} OrkLegi;

typedef struct {
    InsanImparatorlugu insan_imparatorlugu;
    OrkLegi ork_legi;
} Data;

typedef struct {
    char arastirma_ismi[256];
    char seviye[256];
    char bonus_degeri[256];
    char aciklama[256];
} ResearchData;

ResearchData researchDizi[100];
int researchSay = 0;

// Canavarlar
typedef struct {
    char canavar_ismi[MAX_LINE_LENGTH];
   	char bonus_turu[MAX_LINE_LENGTH];
    char bonus_degeri[MAX_LINE_LENGTH];
    char aciklama[MAX_LINE_LENGTH];
} Canavar;

typedef struct {
    Canavar canavarlar[MAX_HEROES];
    int canavar_sayisi;
} OrkLegiCanavar;

typedef struct {
    Canavar canavarlar[MAX_HEROES];
    int canavar_sayisi;
} InsanImparatorluguCanavar;

typedef struct {
    OrkLegiCanavar orkLegi;
    InsanImparatorluguCanavar insanImparatorlugu;
} CanavarData;

// Kahramanlar
typedef struct {
    char kahraman_ismi[MAX_LINE_LENGTH];
    char bonus_turu[MAX_LINE_LENGTH];
    char bonus_degeri[MAX_LINE_LENGTH];
    char aciklama[MAX_LINE_LENGTH];
} Kahraman;

typedef struct {
    Kahraman kahramanlar[MAX_HEROES];
    int kahraman_sayisi;
} OrkLegiKahraman;

typedef struct {
    Kahraman kahramanlar[MAX_HEROES];
    int kahraman_sayisi;
} InsanImparatorluguKahraman;

typedef struct {
    OrkLegiKahraman orkLegi;
    InsanImparatorluguKahraman insanImparatorlugu;
} kahramanData;

// Global bonus degikenleri
float insan_imparatorlugu_bonus = 1.0;
float ork_legi_bonus = 1.0;

float insan_imparatorlugu_defence_bonus = 1.0;
float ork_legi_defence_bonus = 1.0;

float piyade_bonus = 1.0;
float suvari_bonus = 1.0;
float okcu_bonus = 1.0;
float kusatma_makinesi_bonus = 1.0;

float ork_dovusculeri_bonus = 1.0;
float mizrakcilar_bonus = 1.0;
float varg_binicileri_bonus = 1.0;
float troll_bonus = 1.0;

int piyade_saldiri_gucu = 30;
int suvari_saldiri_gucu = 50;
int okcu_saldiri_gucu = 40;
int kusatma_makinesi_saldiri_gucu = 100;

int ork_dovusculeri_saldiri_gucu = 25;
int mizrakcilar_saldiri_gucu = 30;
int varg_binicileri_saldiri_gucu = 40;
int troll_saldiri_gucu = 70;

int piyade_savunma = 50;
int suvari_savunma = 30;
int okcu_savunma = 20;
int kusatma_makinesi_savunma = 50;

int ork_dovusculeri_savunma = 20;
int mizrakcilar_savunma = 25;
int varg_binicileri_savunma = 35;
int troll_savunma = 40;


int piyade_saglik = 100;
int suvari_saglik = 120;
int okcu_saglik = 80;
int kusatma_makinesi_saglik = 150;

int ork_dovusculeri_saglik = 100;
int mizrakcilar_saglik = 90;
int varg_binicileri_saglik = 130;
int troll_saglik = 200;

int piyade_kritik_sans = 10;
int suvari_kritik_sans = 15;
int okcu_kritik_sans = 20;
int kusatma_makinesi_kritik_sans = 5;

int ork_dovusculeri_kritik_sans = 12;
int mizrakcilar_kritik_sans = 18;
int varg_binicileri_kritik_sans = 10;
int troll_kritik_sans = 25;


void clear(char *str) {
    // Son bosluklari icin
    size_t len = strlen(str);
    while (len > 0 && (isspace((unsigned char)str[len - 1]) || str[len - 1] == ',' || str[len - 1] == ']')) {
        str[--len] = '\0';
    }
}

void read_json(Data* data, const char* filename) {
    FILE* file = fopen(filename, "r");
    if (file == NULL) {
        printf("Dosya acilamadi!\n");
        return;
    }

    char line[MAX_LINE_LENGTH];

    int isOrkLegi = 0, isInsanImparatorlugu = 0;

    while (fgets(line, sizeof(line), file)) {
        // OrkLegi kismi
        if (strstr(line, "\"ork_legi\"")) {
            isOrkLegi = 1;
            isInsanImparatorlugu = 0;
            continue;
        }

        // InsanImparatorlugu kismi
        if (strstr(line, "\"insan_imparatorlugu\"")) {
            isOrkLegi = 0;
            isInsanImparatorlugu = 1;
            continue;
        }

        // Veri
        if (isOrkLegi) {
            // Saldiri Gelistirmesi
            if (strstr(line, "saldiri_gelistirmesi")) {
                sscanf(line, " \"saldiri_gelistirmesi\": %d,", &data->ork_legi.arastirma_seviyesi.saldiri_gelistirmesi);

            }
            if (strstr(line, "savunma_ustaligi")) {
                sscanf(line, " \"savunma_ustaligi\": %d,", &data->ork_legi.arastirma_seviyesi.savunma_ustaligi);
            }
            if (strstr(line, "elit_egitim")) {
                sscanf(line, " \"elit_egitim\": %d,", &data->ork_legi.arastirma_seviyesi.elit_egitim);
            }
            // Birimler
            if (strstr(line, "ork_dovusculeri")) {
                sscanf(line, " \"ork_dovusculeri\": %d,", &data->ork_legi.birimler.ork_dovusculeri);
            }
            if (strstr(line, "mizrakcilar")) {
                sscanf(line, " \"mizrakcilar\": %d,", &data->ork_legi.birimler.mizrakcilar);
            }
            if (strstr(line, "varg_binicileri")) {
                sscanf(line, " \"varg_binicileri\": %d,", &data->ork_legi.birimler.varg_binicileri);
            }
            if (strstr(line, "troller")) {
                sscanf(line, " \"troller\": %d,", &data->ork_legi.birimler.troller);
            }
            if (strstr(line, "\"kahramanlar\": [") != NULL) {
                char *start = strstr(line, "[") + 1; // Ilk acilis parantezinin sonrasini al
                char *end = strstr(start, "]"); // Kapanis parantezini bul
                if (end != NULL) {
                    *end = '\0'; // Kapanis parantezini null ile bitir
                    char *token = strtok(start, "\","); // Duzgun delimiterleri kullaniliyor
                    while (token != NULL) {
                        clear(token); // Kahraman ismini temizle
                        strcpy(data->ork_legi.kahramanlar[data->ork_legi.kahraman_sayisi], token);
                        data->ork_legi.kahraman_sayisi++;
                        token = strtok(NULL, "\",");
                    }
                } else {
                    printf("Kapanis parantezi bulunamadi!\n");
                }
            }
            if (strstr(line, "\"canavarlar\": [") != NULL) {
                char *start = strstr(line, "[") + 1;
                char *end = strstr(start, "]");
                if (end != NULL) {
                    *end = '\0';
                    char *token = strtok(start, "\",");
                    while (token != NULL) {
                        clear(token);
                        strcpy(data->ork_legi.canavarlar[data->ork_legi.canavar_sayisi], token);
                        data->ork_legi.canavar_sayisi++;
                        token = strtok(NULL, "\",");
                    }
                } else {
                    printf("Kapanis parantezi bulunamadi!\n");
                }
            }

        }


        if (isInsanImparatorlugu) {
            // Savunma Ustaligi
            if (strstr(line, "saldiri_gelistirmesi")) {
                sscanf(line, " \"saldiri_gelistirmesi\": %d,", &data->insan_imparatorlugu.arastirma_seviyesi.saldiri_gelistirmesi);
            }
            if (strstr(line, "savunma_ustaligi")) {
                sscanf(line, " \"savunma_ustaligi\": %d,", &data->insan_imparatorlugu.arastirma_seviyesi.savunma_ustaligi);
            }
            if (strstr(line, "elit_egitim")) {
                sscanf(line, " \"elit_egitim\": %d,", &data->insan_imparatorlugu.arastirma_seviyesi.elit_egitim);
            }
            if (strstr(line, "kusatma_ustaligi")) {
                sscanf(line, " \"kusatma_ustaligi\": %d,", &data->insan_imparatorlugu.arastirma_seviyesi.kusatma_ustaligi);
            }
            // Birimler
            if (strstr(line, "piyadeler")) {
                sscanf(line, " \"piyadeler\": %d,", &data->insan_imparatorlugu.birimler.piyadeler);
            }
            if (strstr(line, "okcular")) {
                sscanf(line, " \"okcular\": %d,", &data->insan_imparatorlugu.birimler.okcular);
            }
            if (strstr(line, "suvariler")) {
                sscanf(line, " \"suvariler\": %d,", &data->insan_imparatorlugu.birimler.suvariler);
            }
            if (strstr(line, "kusatma_makineleri")) {
                sscanf(line, " \"kusatma_makineleri\": %d,", &data->insan_imparatorlugu.birimler.kusatma_makineleri);
            }
            if (strstr(line, "\"kahramanlar\": [") != NULL) {
                char *start = strstr(line, "[") + 1;
                char *end = strstr(start, "]");
                if (end != NULL) {
                    *end = '\0';
                    char *token = strtok(start, "\",");
                    while (token != NULL) {
                        clear(token);
                        strcpy(data->insan_imparatorlugu.kahramanlar[data->insan_imparatorlugu.kahraman_sayisi], token);
                        data->insan_imparatorlugu.kahraman_sayisi++;
                        token = strtok(NULL, "\",");
                    }
                } else {
                    printf("Kapanis parantezi bulunamadi!\n");
                }
            }
            if (strstr(line, "\"canavarlar\": [") != NULL) {
                char *start = strstr(line, "[") + 1;
                char *end = strstr(start, "]");
                if (end != NULL) {
                    *end = '\0';
                    char *token = strtok(start, "\",");
                    while (token != NULL) {
                        clear(token);
                        strcpy(data->insan_imparatorlugu.canavarlar[data->insan_imparatorlugu.canavar_sayisi], token);
                        data->insan_imparatorlugu.canavar_sayisi++;
                        token = strtok(NULL, "\",");
                    }
                } else {
                    printf("Kapanis parantezi bulunamadi!\n");
                }
            }
        }
    }

    fclose(file);
}

void print_data(const Data *data) {
	printf("\n---------------------- JSON Dosyasi Okunan Veriler --------------------------------\n");
    printf("Insan Imparatorlugu:\n");
    printf("  Piyadeler: %d\n", data->insan_imparatorlugu.birimler.piyadeler);
    printf("  Suvariler: %d\n", data->insan_imparatorlugu.birimler.suvariler);
    printf("  Okcular: %d\n", data->insan_imparatorlugu.birimler.okcular);
    printf("  Kusatma Makineleri: %d\n", data->insan_imparatorlugu.birimler.kusatma_makineleri);
    printf("  Kahramanlar: ");
    for (int i = 0; i < data->insan_imparatorlugu.kahraman_sayisi; i++) {
        printf("%s ", data->insan_imparatorlugu.kahramanlar[i]);
    }
    printf("\n  Canavarlar: ");
    for (int i = 0; i < data->insan_imparatorlugu.canavar_sayisi; i++) {
        printf("%s ", data->insan_imparatorlugu.canavarlar[i]);
    }
    printf("\n  Arastirma Seviyesi - Savunma Ustaligi: %d\n", data->insan_imparatorlugu.arastirma_seviyesi.savunma_ustaligi);
    printf("  Arastirma Seviyesi - Saldiri Ustaligi: %d\n", data->insan_imparatorlugu.arastirma_seviyesi.saldiri_gelistirmesi);
    printf("  Arastirma Seviyesi - Elit Egitim: %d\n", data->insan_imparatorlugu.arastirma_seviyesi.elit_egitim);
    printf("  Arastirma Seviyesi - Kusatma Ustaligi: %d\n", data->insan_imparatorlugu.arastirma_seviyesi.kusatma_ustaligi);

    printf("\nOrk Legi:\n");
    printf("  Ork Dovuscileri: %d\n", data->ork_legi.birimler.ork_dovusculeri);
    printf("  Mizrakcilar: %d\n", data->ork_legi.birimler.mizrakcilar);
    printf("  Varg Binicileri: %d\n", data->ork_legi.birimler.varg_binicileri);
	printf("  Troller: %d\n", data->ork_legi.birimler.troller);
    printf("  Kahramanlar: ");
    for (int i = 0; i < data->ork_legi.kahraman_sayisi; i++) {
        printf("%s ", data->ork_legi.kahramanlar[i]);
    }
    printf("\n  Canavarlar: ");
    for (int i = 0; i < data->ork_legi.canavar_sayisi; i++) {
        printf("%s ", data->ork_legi.canavarlar[i]);
    }
    printf("\n  Arastirma Seviyesi - Savunma Ustaligi: %d\n", data->ork_legi.arastirma_seviyesi.savunma_ustaligi);
    printf("  Arastirma Seviyesi - Saldiri Gelistirmesi: %d\n", data->ork_legi.arastirma_seviyesi.saldiri_gelistirmesi);
    printf("  Arastirma Seviyesi - Elit Egitim: %d\n", data->ork_legi.arastirma_seviyesi.elit_egitim);

}

void draw_table(Data* data) {
	cleardevice(); // Ekrani temizler

    int x, y;

    // Degiskenleri guncelledik
    int total_units_piyade_insan_imp = data->insan_imparatorlugu.birimler.piyadeler;
    int total_units_okcular_insan_imp = data->insan_imparatorlugu.birimler.okcular;
    int total_units_suvari_insan_imp = data->insan_imparatorlugu.birimler.suvariler;
    int total_units_kusatma_makineleri_insan_imp = data->insan_imparatorlugu.birimler.kusatma_makineleri;

    int filled_cells_piyade_insan_imp = total_units_piyade_insan_imp / 100;
    int filled_cells_okcular_insan_imp = total_units_okcular_insan_imp / 100;
    int filled_cells_suvari_insan_imp = total_units_suvari_insan_imp / 100;
    int filled_cells_kusatma_makineleri_insan_imp = total_units_kusatma_makineleri_insan_imp / 100;


    int total_units_ork_dovuscileri = data->ork_legi.birimler.ork_dovusculeri;
    int total_units_mizrakcilar = data->ork_legi.birimler.mizrakcilar;
    int total_units_varg_binicileri = data->ork_legi.birimler.varg_binicileri;
    int total_units_troller = data->ork_legi.birimler.troller;

    int filled_cells_ork_dovuscileri = total_units_ork_dovuscileri / 100;
    int filled_cells_mizrakcilar = total_units_mizrakcilar / 100;
    int filled_cells_varg_binicileri = total_units_varg_binicileri / 100;
    int filled_cells_troller = total_units_troller / 100;

    settextstyle(DEFAULT_FONT, HORIZ_DIR, 1); //yazi boyutunu ayarlamak icin


    for (int row = 0; row < ROWS; row++) {
        for (int col = 0; col < COLS; col++) {
            x = col * CELL_SIZE; // X koordinati
            y = row * CELL_SIZE; // Y koordinati
            rectangle(x, y, x + CELL_SIZE, y + CELL_SIZE);
        }
    }

    // Piyade birimlerini  cizme
    for (int col = 0; col <= filled_cells_piyade_insan_imp; col++) {
        x = col * CELL_SIZE;
        y = 0; // Ilk satir

        if (col < filled_cells_piyade_insan_imp) {
        	setfillstyle(SOLID_FILL, GREEN);
            bar(x, y, x + CELL_SIZE-2, y + CELL_SIZE-2);
            char text[10];
            sprintf(text, "%d", 100);
            outtextxy(x + CELL_SIZE / 4, y + CELL_SIZE / 4, text);
        }
        else if (col == filled_cells_piyade_insan_imp && total_units_piyade_insan_imp % 100 > 0) {
            setfillstyle(SOLID_FILL, GREEN);
            bar(x, y, x + CELL_SIZE-2, y + CELL_SIZE-2);
            char text[10];
            sprintf(text, "%d", total_units_piyade_insan_imp % 100);
            outtextxy(x + CELL_SIZE / 4, y + CELL_SIZE / 4, text);
        }
    }
    // Okcular birimlerini  cizme
    for (int col = 0; col <= filled_cells_okcular_insan_imp; col++) {
        x = col * CELL_SIZE;
        y = CELL_SIZE;
        if (col < filled_cells_okcular_insan_imp) {
        	setfillstyle(SOLID_FILL, GREEN);
            bar(x, y, x + CELL_SIZE-2, y + CELL_SIZE-2);
            char text[10];
            sprintf(text, "%d", 100);
            outtextxy(x + CELL_SIZE / 4, y + CELL_SIZE / 4, text);
        }
        else if (col == filled_cells_okcular_insan_imp && total_units_okcular_insan_imp % 100 > 0) {
        	setfillstyle(SOLID_FILL, GREEN);
            bar(x, y, x + CELL_SIZE-2, y + CELL_SIZE-2);
            char text[10];
            sprintf(text, "%d", total_units_okcular_insan_imp % 100);
            outtextxy(x + CELL_SIZE / 4, y + CELL_SIZE / 4, text);
        }
    }

	// Suvari birimlerini  cizme
    for (int col = 0; col <= filled_cells_suvari_insan_imp; col++) {
        x = col * CELL_SIZE;
        y = 2 * CELL_SIZE;
        if (col < filled_cells_suvari_insan_imp) {
        	setfillstyle(SOLID_FILL, GREEN);
            bar(x, y, x + CELL_SIZE-2, y + CELL_SIZE-2);
            char text[10];
            sprintf(text, "%d", 100);
            outtextxy(x + CELL_SIZE / 4, y + CELL_SIZE / 4, text);
        }
        else if (col == filled_cells_suvari_insan_imp && total_units_suvari_insan_imp % 100 > 0) {
        	setfillstyle(SOLID_FILL, GREEN);
            bar(x, y, x + CELL_SIZE-2, y + CELL_SIZE-2);
            char text[10];
            sprintf(text, "%d", total_units_suvari_insan_imp % 100);
            outtextxy(x + CELL_SIZE / 4, y + CELL_SIZE / 4, text);
        }
    }
    // Kusatma makinelerini  cizme
    for (int col = 0; col <= filled_cells_kusatma_makineleri_insan_imp; col++) {
        x = col * CELL_SIZE;
        y = 3 * CELL_SIZE;
        if (col < filled_cells_kusatma_makineleri_insan_imp) {
        	setfillstyle(SOLID_FILL, GREEN);
            bar(x, y, x + CELL_SIZE-2, y + CELL_SIZE-2);
            char text[10];
            sprintf(text, "%d", 100);
            outtextxy(x + CELL_SIZE / 4, y + CELL_SIZE / 4, text);
        }
        else if (col == filled_cells_kusatma_makineleri_insan_imp && total_units_kusatma_makineleri_insan_imp % 100 > 0) {
        	setfillstyle(SOLID_FILL, GREEN);
            bar(x, y, x + CELL_SIZE-2, y + CELL_SIZE-2);
            char text[10];
            sprintf(text, "%d", total_units_kusatma_makineleri_insan_imp % 100);
            outtextxy(x + CELL_SIZE / 4, y + CELL_SIZE / 4, text);
        }
    }
    // Ork Dovusuculeri birimlerini  cizme
    for (int col = 0; col <= filled_cells_ork_dovuscileri; col++) {
        x = col * CELL_SIZE;
        y = 19 * CELL_SIZE;
        if (col < filled_cells_ork_dovuscileri) {
        	setfillstyle(SOLID_FILL, GREEN);
            bar(x, y, x + CELL_SIZE-2, y + CELL_SIZE-2);
            char text[10];
            sprintf(text, "%d", 100);
            outtextxy(x + CELL_SIZE / 4, y + CELL_SIZE / 4, text);
        }
        else if (col == filled_cells_ork_dovuscileri && total_units_ork_dovuscileri % 100 > 0) {
        	setfillstyle(SOLID_FILL, GREEN);
            bar(x, y, x + CELL_SIZE-2, y + CELL_SIZE-2);
            char text[10];
            sprintf(text, "%d", total_units_ork_dovuscileri % 100);
            outtextxy(x + CELL_SIZE / 4, y + CELL_SIZE / 4, text);
        }
    }
    // Mizrakcilar birimlerini  cizme
    for (int col = 0; col <= filled_cells_mizrakcilar; col++) {
        x = col * CELL_SIZE;
        y = 18 * CELL_SIZE;
        if (col < filled_cells_mizrakcilar) {
        	setfillstyle(SOLID_FILL, GREEN);
            bar(x, y, x + CELL_SIZE-2, y + CELL_SIZE-2);
            char text[10];
            sprintf(text, "%d", 100);
            outtextxy(x + CELL_SIZE / 4, y + CELL_SIZE / 4, text);
        }
        else if (col == filled_cells_mizrakcilar && total_units_mizrakcilar % 100 > 0) {
        	setfillstyle(SOLID_FILL, GREEN);
            bar(x, y, x + CELL_SIZE-2, y + CELL_SIZE-2);
            char text[10];
            sprintf(text, "%d", total_units_mizrakcilar % 100);
            outtextxy(x + CELL_SIZE / 4, y + CELL_SIZE / 4, text);
        }
    }
    // Varg Binicileri birimlerini  cizme
    for (int col = 0; col <= filled_cells_varg_binicileri; col++) {
        x = col * CELL_SIZE;
        y = 17 * CELL_SIZE;

        if (col < filled_cells_varg_binicileri) {
        	setfillstyle(SOLID_FILL, GREEN);
            bar(x, y, x + CELL_SIZE-2, y + CELL_SIZE-2);
            char text[10];
            sprintf(text, "%d", 100);
            outtextxy(x + CELL_SIZE / 4, y + CELL_SIZE / 4, text);
        }
        else if (col == filled_cells_varg_binicileri && total_units_varg_binicileri % 100 > 0) {
        	setfillstyle(SOLID_FILL, GREEN);
            bar(x, y, x + CELL_SIZE-2, y + CELL_SIZE-2);
            char text[10];
            sprintf(text, "%d", total_units_varg_binicileri % 100);
            outtextxy(x + CELL_SIZE / 4, y + CELL_SIZE / 4, text);
        }
    }
    //Troller birimlerini  cizme
    for (int col = 0; col <= filled_cells_troller; col++) {
        x = col * CELL_SIZE;
        y = 16 * CELL_SIZE;
        if (col < filled_cells_troller) {
        	setfillstyle(SOLID_FILL, GREEN);
            bar(x, y, x + CELL_SIZE-2, y + CELL_SIZE-2);
            char text[10];
            sprintf(text, "%d", 100);
            outtextxy(x + CELL_SIZE / 4, y + CELL_SIZE / 4, text);
        }
        else if (col == filled_cells_troller && total_units_troller % 100 > 0) {
        	setfillstyle(SOLID_FILL, GREEN);
            bar(x, y, x + CELL_SIZE-2, y + CELL_SIZE-2);
            char text[10];
            sprintf(text, "%d", total_units_troller % 100);
            outtextxy(x + CELL_SIZE / 4, y + CELL_SIZE / 4, text);
        }
    }
    settextstyle(BOLD_FONT, HORIZ_DIR, 2); // Yazi stilini ayari
    // Birim isimlerini ekleme
    outtextxy((CELL_SIZE * 20) + 10, 10, "Piyadeler");
    outtextxy((CELL_SIZE * 20) + 10, CELL_SIZE + 10, "Okcular");
    outtextxy((CELL_SIZE * 20) + 10, (2 * CELL_SIZE) + 10, "Suvariler");
    outtextxy((CELL_SIZE * 20) + 10, (3 * CELL_SIZE) + 10, "Kusatma Makineleri");
    outtextxy((CELL_SIZE * 20) + 10, (4 * CELL_SIZE) + 10, "Kahramanlar:");

    int x1 = CELL_SIZE * 20 + 175; // X koordinati
    int y1 = (4 * CELL_SIZE) + 10;  // Y koordinati
    for (int i = 0; i < data->insan_imparatorlugu.kahraman_sayisi; i++) {
        outtextxy(x1, y1, data->insan_imparatorlugu.kahramanlar[i]);
        x1 += 250;
    }


    outtextxy(CELL_SIZE * 20 + 10, (5 * CELL_SIZE) + 10, "Canavarlar:");
    x1 = CELL_SIZE * 20 + 175;
    for (int i = 0; i < data->insan_imparatorlugu.canavar_sayisi; i++) {
        outtextxy(x1, (5 * CELL_SIZE) + 10, data->insan_imparatorlugu.canavarlar[i]);
        x1 += 250;
    }
    outtextxy(CELL_SIZE * 20 + 10, (14 * CELL_SIZE) + 10, "Kahramanlar:");
    int x2 = CELL_SIZE * 20 + 175;
    for (int i = 0; i < data->ork_legi.kahraman_sayisi; i++) {
        outtextxy(x2, (14 * CELL_SIZE) + 10, data->ork_legi.kahramanlar[i]); // Kahramanin ismini yaz
        x2 += 250;
    }

    outtextxy(CELL_SIZE * 20 + 10, (15 * CELL_SIZE) + 10, "Canavarlar:");
    x2 = CELL_SIZE * 20 + 175;
    for (int i = 0; i < data->ork_legi.canavar_sayisi; i++) {
        outtextxy(x2, (15 * CELL_SIZE) + 10, data->ork_legi.canavarlar[i]); // Canavarin ismini yaz
        x2 += 250;
    }


    outtextxy(CELL_SIZE * 20 + 10, (19 * CELL_SIZE) + 10, "Ork Dovusculeri");
    outtextxy(CELL_SIZE * 20 + 10, (18 * CELL_SIZE) + 10, "Mizrakcilar");
    outtextxy(CELL_SIZE * 20 + 10, (17 * CELL_SIZE) + 10, "Varg Binicileri");
    outtextxy(CELL_SIZE * 20 + 10, (16 * CELL_SIZE) + 10, "Troller");
}

int toplam_saldiri_gucu(Data* data, const char* type,int tur) {

    if (strcmp(type, "insan_imparatorlugu") == 0) {

		float piyade_kritik = 1.0;
		float suvari_kritik = 1.0;
		float okcu_kritik = 1.0;
		float kusatma_makinesi_kritik = 1.0;

        if (piyade_kritik_sans > 0 && tur % (100 / piyade_kritik_sans) == 0) {
            piyade_kritik = 1.5;
            printf("\n\nPiyade kritik eylemi gerceklesti!\n\n");
        }

        if (suvari_kritik_sans > 0 && tur % (100 / suvari_kritik_sans) == 0) {
            suvari_kritik = 1.5;
            printf("\n\nSuvari kritik eylemi gerceklesti!\n\n");
        }

        if (okcu_kritik_sans > 0 && tur % (100 / okcu_kritik_sans) == 0) {
            okcu_kritik = 1.5;
            printf("\n\nOkcular kritik eylemi gerceklesti!\n\n");
        }

        if (kusatma_makinesi_kritik_sans > 0 && tur % (100 / kusatma_makinesi_kritik_sans) == 0) {
            kusatma_makinesi_kritik = 1.5;
            printf("\n\nKusatma makineleri kritik eylemi gerceklesti!\n\n");
        }


		int saldiri_gucu = insan_imparatorlugu_bonus *
		    ((data->insan_imparatorlugu.birimler.piyadeler * piyade_saldiri_gucu * piyade_kritik) +
		     (data->insan_imparatorlugu.birimler.suvariler * suvari_saldiri_gucu * suvari_kritik) +
		     (data->insan_imparatorlugu.birimler.okcular * okcu_saldiri_gucu * okcu_kritik) +
		     (data->insan_imparatorlugu.birimler.kusatma_makineleri * kusatma_makinesi_saldiri_gucu * kusatma_makinesi_kritik));


        return saldiri_gucu;

    }else if (strcmp(type, "ork_legi") == 0) {

		float ork_dovusculeri_kritik = 1.0;
		float mizrakcilar_kritik = 1.0;
		float varg_binicileri_kritik = 1.0;
		float troll_kritik = 1.0;

        // kritik sans kontrol
        if (ork_dovusculeri_kritik_sans > 0 && tur % (100 / ork_dovusculeri_kritik_sans) == 0) {
            ork_dovusculeri_kritik = 1.5;
            printf("\n\nOrk dovusculeri kritik eylemi gerceklesti!\n\n");
        }

        if (mizrakcilar_kritik_sans > 0 && tur % (100 / mizrakcilar_kritik_sans) == 0) {
            mizrakcilar_kritik = 1.5;
            printf("\n\nMizrakcilar kritik eylemi gerceklesti!\n\n");
        }

        if (varg_binicileri_kritik_sans > 0 && tur % (100 / varg_binicileri_kritik_sans) == 0) {
            varg_binicileri_kritik = 1.5;
            printf("\n\nVarg binicileri kritik eylemi gerceklesti!\n\n");
        }

        if (troll_kritik_sans > 0 && tur % (100 / troll_kritik_sans) == 0) {
            troll_kritik = 1.5;
            printf("\n\nTroll kritik eylemi gerceklesti!\n\n");
        }

		int saldiri_gucu = ork_legi_bonus *
		    ((data->ork_legi.birimler.ork_dovusculeri * ork_dovusculeri_saldiri_gucu * ork_dovusculeri_kritik) +
		     (data->ork_legi.birimler.mizrakcilar * mizrakcilar_saldiri_gucu * mizrakcilar_kritik) +
		     (data->ork_legi.birimler.varg_binicileri * varg_binicileri_saldiri_gucu * varg_binicileri_kritik) +
		     (data->ork_legi.birimler.troller * troll_saldiri_gucu * troll_kritik));

        return saldiri_gucu;
    }
    return 0;

}

int toplam_savunma_gucu(Data* data,  const char* type) {
	int savunma_gucu = 0;
    if(type == "insan_imparatorlugu") {
        savunma_gucu = insan_imparatorlugu_defence_bonus*((data->insan_imparatorlugu.birimler.piyadeler * piyade_savunma) +
                       (data->insan_imparatorlugu.birimler.suvariler * suvari_savunma) +
                       (data->insan_imparatorlugu.birimler.okcular * okcu_savunma) +
                       (data->insan_imparatorlugu.birimler.kusatma_makineleri * kusatma_makinesi_savunma));
    } else if(type == "ork_legi") {
        savunma_gucu = ork_legi_defence_bonus*((data->ork_legi.birimler.ork_dovusculeri * ork_dovusculeri_savunma) +
                       (data->ork_legi.birimler.mizrakcilar * mizrakcilar_savunma) +
                       (data->ork_legi.birimler.varg_binicileri * varg_binicileri_savunma) +
                       (data->ork_legi.birimler.troller * troll_savunma ));
    }

    return savunma_gucu;
}

int single_unit_savunma_gucu(const char* type, const char* birim_turu, Data* data){
int savunma_gucu=0;

    if(strcmp(type, "insan_imparatorlugu") == 0) {
        if(strcmp(birim_turu, "piyade") == 0) {
            savunma_gucu = data->insan_imparatorlugu.birimler.piyadeler * piyade_savunma  * insan_imparatorlugu_defence_bonus;
        } else if(strcmp(birim_turu, "suvari") == 0) {
            savunma_gucu = data->insan_imparatorlugu.birimler.suvariler * suvari_savunma * insan_imparatorlugu_defence_bonus;
        } else if(strcmp(birim_turu, "okcu") == 0) {
            savunma_gucu = data->insan_imparatorlugu.birimler.okcular * okcu_savunma * insan_imparatorlugu_defence_bonus;
        } else if(strcmp(birim_turu, "kusatma_makinesi") == 0) {
            savunma_gucu = data->insan_imparatorlugu.birimler.kusatma_makineleri * kusatma_makinesi_savunma * insan_imparatorlugu_defence_bonus;
        }
        savunma_gucu *= insan_imparatorlugu_defence_bonus;
    } else if(strcmp(type, "ork_legi") == 0) {

        if(strcmp(birim_turu, "ork_dovusculeri") == 0) {
            savunma_gucu = data->ork_legi.birimler.ork_dovusculeri * ork_dovusculeri_savunma * ork_legi_defence_bonus;
        } else if(strcmp(birim_turu, "mizrakcilar") == 0) {
            savunma_gucu = data->ork_legi.birimler.mizrakcilar * mizrakcilar_savunma * ork_legi_defence_bonus;
        } else if(strcmp(birim_turu, "varg_binicileri") == 0) {
            savunma_gucu = data->ork_legi.birimler.varg_binicileri * varg_binicileri_savunma * ork_legi_defence_bonus;
        } else if(strcmp(birim_turu, "troller") == 0) {
            savunma_gucu = data->ork_legi.birimler.troller * troll_savunma  * ork_legi_defence_bonus;
        }
        savunma_gucu *= ork_legi_defence_bonus;
    }
    return savunma_gucu;
}

int kalan_piyade_hasar = 0;
int kalan_suvari_hasar = 0;
int kalan_okcu_hasar = 0;
int kalan_kusatma_makinesi_hasar = 0;

int kalan_ork_dovuscu_hasar = 0;
int kalan_mizrakci_hasar = 0;
int kalan_varg_binicisi_hasar = 0;
int kalan_troll_hasar = 0;


void hasar_dagilimi_fun(Data* data, const char* turn,int tur) {

	FILE *dosya = fopen("savas_sim.txt", "a");
    if (dosya == NULL) {
        printf("Dosya a ma hatasi!\n");
        return;
    }

    int hasar_dagilimi = 0;

	int insan_toplam_saldiri = toplam_saldiri_gucu(data, "insan_imparatorlugu",tur);
	int insan_toplam_savunma = toplam_savunma_gucu(data, "insan_imparatorlugu");

	int ork_toplam_saldiri = toplam_saldiri_gucu(data, "ork_legi",tur);
	int ork_toplam_savunma = toplam_savunma_gucu(data, "ork_legi");

	if (tur >= 5) {
	    int yorgunluk = (tur / 5);
	    float azaltma_orani = 1.0f;

	    for (int i = 0; i < yorgunluk; i++) {
	        azaltma_orani *= 0.9f;
	    }

	    if (azaltma_orani < 0) {
	        azaltma_orani = 0;
	    }

	    printf("\n%d. Turda Yorgunluk Uygulandi. Azalma Orani: %.2f\n\n", tur, azaltma_orani);
	    fprintf(dosya, "\n%d. Turda Yorgunluk Uygulandi. Azalma Orani: %.2f\n\n", tur, azaltma_orani);

	    // Toplam saldiri  gucun  azalt
	    float yeni_insan_toplam_saldiri = insan_toplam_saldiri * azaltma_orani;
	    float yeni_ork_toplam_saldiri = ork_toplam_saldiri * azaltma_orani;


	    insan_toplam_saldiri = yeni_insan_toplam_saldiri > 0 ? yeni_insan_toplam_saldiri : 0;
	    ork_toplam_saldiri = yeni_ork_toplam_saldiri > 0 ? yeni_ork_toplam_saldiri : 0;
	}

    printf("Insan toplam saldiri: %d, Insan toplam savunma: %d\n", insan_toplam_saldiri, insan_toplam_savunma);
    printf("Ork toplam saldiri: %d, Ork toplam savunma: %d\n", ork_toplam_saldiri, ork_toplam_savunma);

	fprintf(dosya, "Insan toplam saldiri: %d, Insan toplam savunma: %d\n", insan_toplam_saldiri, insan_toplam_savunma);
    fprintf(dosya, "Ork toplam saldiri: %d, Ork toplam savunma: %d\n", ork_toplam_saldiri, ork_toplam_savunma);



    if (strcmp(turn, "insan_imparatorlugu") == 0) {
        // Ork ordusu icin hasar dagilim
        int ork_dovusculeri_savunma_gucu = single_unit_savunma_gucu("ork_legi", "ork_dovusculeri", data);
        int mizrakcilar_savunma_gucu = single_unit_savunma_gucu("ork_legi", "mizrakcilar", data);
        int varg_binicileri_gucu = single_unit_savunma_gucu("ork_legi", "varg_binicileri", data);
        int troller_gucu = single_unit_savunma_gucu("ork_legi", "troller", data);

         fprintf(dosya, "Ork Dovusculeri Savunma Gucu: %d\n", ork_dovusculeri_savunma_gucu);
        printf("Ork Dovusculeri Savunma Gucu: %d\n", ork_dovusculeri_savunma_gucu);

        fprintf(dosya, "Mizrakcilar Savunma Gucu: %d\n", mizrakcilar_savunma_gucu);
        printf("Mizrakcilar Savunma Gucu: %d\n", mizrakcilar_savunma_gucu);

        fprintf(dosya, "Varg Binicileri Savunma Gucu: %d\n", varg_binicileri_gucu);
        printf("Varg Binicileri Savunma Gucu: %d\n", varg_binicileri_gucu);

        fprintf(dosya, "Troller Savunma Gucu: %d\n", troller_gucu);
        printf("Troller Savunma Gucu: %d\n", troller_gucu);

 		if (data->ork_legi.birimler.ork_dovusculeri > 0) {
            float hasar_dagilimi_ork_dovusculeri = (float)(insan_toplam_saldiri - ork_toplam_savunma) * ((float)ork_dovusculeri_savunma_gucu / ork_toplam_savunma);

            hasar_dagilimi_ork_dovusculeri += kalan_ork_dovuscu_hasar;

            if (hasar_dagilimi_ork_dovusculeri > 0) {
                fprintf(dosya, "Ork Dovusculerine Dagitilan Hasar: %.2f\n", hasar_dagilimi_ork_dovusculeri);
                printf("Ork Dovusculerine Dagitilan Hasar: %.2f\n", hasar_dagilimi_ork_dovusculeri);

                int birim_kaybi = (int)(hasar_dagilimi_ork_dovusculeri / ork_dovusculeri_saglik);

				fprintf(dosya, "Kaybedilen Ork Dovusculeri: %d\n", birim_kaybi);
                printf("Kaybedilen Ork Dovusculeri: %d\n", birim_kaybi);

                data->ork_legi.birimler.ork_dovusculeri -= birim_kaybi;

            	kalan_ork_dovuscu_hasar = hasar_dagilimi_ork_dovusculeri-(birim_kaybi * ork_dovusculeri_saglik);

                if (data->ork_legi.birimler.ork_dovusculeri < 0)
				 data->ork_legi.birimler.ork_dovusculeri = 0;


                fprintf(dosya, "Kalan Ork Dovusculeri: %d\n", data->ork_legi.birimler.ork_dovusculeri);
                printf("Kalan Ork Dovusculeri: %d\n", data->ork_legi.birimler.ork_dovusculeri);
            } else {
                fprintf(dosya, "Ork Dovusculerine Dagitilan Hasar Yok\n");
                printf("Ork Dovusculerine Dagitilan Hasar Yok\n");
            }
        }
     if (data->ork_legi.birimler.varg_binicileri > 0) {
            float hasar_dagilimi_varg_binicileri = (float)(insan_toplam_saldiri - ork_toplam_savunma) * ((float)varg_binicileri_gucu / ork_toplam_savunma);

            hasar_dagilimi_varg_binicileri += kalan_varg_binicisi_hasar;

            if (hasar_dagilimi_varg_binicileri > 0) {
                fprintf(dosya, "Varg Binicilerine Dagitilan Hasar: %.2f\n", hasar_dagilimi_varg_binicileri);
                printf("Varg Binicilerine Dagitilan Hasar: %.2f\n", hasar_dagilimi_varg_binicileri);
                int varg_binici_kaybi = (int)(hasar_dagilimi_varg_binicileri / varg_binicileri_saglik);
                data->ork_legi.birimler.varg_binicileri -= varg_binici_kaybi;

                kalan_varg_binicisi_hasar = hasar_dagilimi_varg_binicileri-(varg_binici_kaybi * varg_binicileri_saglik);

                if (data->ork_legi.birimler.varg_binicileri < 0) data->ork_legi.birimler.varg_binicileri = 0;
                fprintf(dosya, "Kalan Varg Binicileri: %d\n", data->ork_legi.birimler.varg_binicileri);
                printf("Kalan Varg Binicileri: %d\n", data->ork_legi.birimler.varg_binicileri);
            } else {
                fprintf(dosya, "Varg Binicilerine Dagitilan Hasar Yok\n");
                printf("Varg Binicilerine Dagitilan Hasar Yok\n");
            }
        }

        if (data->ork_legi.birimler.troller > 0) {
            float hasar_dagilimi_troller = (float)(insan_toplam_saldiri - ork_toplam_savunma) * ((float)troller_gucu / ork_toplam_savunma);

            hasar_dagilimi_troller += kalan_troll_hasar;

            if (hasar_dagilimi_troller > 0) {
                fprintf(dosya, "Troller'e Dagitilan Hasar: %.2f\n", hasar_dagilimi_troller);
                printf("Troller'e Dagitilan Hasar: %.2f\n", hasar_dagilimi_troller);
                int troll_kaybi = (int)(hasar_dagilimi_troller / troll_saglik);
                data->ork_legi.birimler.troller -= troll_kaybi;

				kalan_troll_hasar = hasar_dagilimi_troller-(troll_kaybi * troll_saglik);

                if (data->ork_legi.birimler.troller < 0) data->ork_legi.birimler.troller = 0;
                fprintf(dosya, "Kalan Troller: %d\n", data->ork_legi.birimler.troller);
                printf("Kalan Troller: %d\n", data->ork_legi.birimler.troller);
            } else {
                fprintf(dosya, "Troller'e Dagitilan Hasar Yok\n");
                printf("Troller'e Dagitilan Hasar Yok\n");
            }
        }
        if (data->ork_legi.birimler.mizrakcilar > 0) {

			float hasar_dagilimi_mizrakcilar = (float)(insan_toplam_saldiri - ork_toplam_savunma) * ((float)mizrakcilar_savunma_gucu / ork_toplam_savunma);

            hasar_dagilimi_mizrakcilar += kalan_mizrakci_hasar;

            if (hasar_dagilimi_mizrakcilar > 0) {
                fprintf(dosya, "Mizrakcilar'a Dagitilan Hasar: %.2f\n", hasar_dagilimi_mizrakcilar);
                printf("Mizrakcilar'a Dagitilan Hasar: %.2f\n", hasar_dagilimi_mizrakcilar);

        		int mizrakci_kaybi = (int)(hasar_dagilimi_mizrakcilar / mizrakcilar_saglik);
                data->ork_legi.birimler.mizrakcilar -= mizrakci_kaybi;
                if (data->ork_legi.birimler.mizrakcilar < 0) data->ork_legi.birimler.mizrakcilar = 0;

                kalan_mizrakci_hasar = hasar_dagilimi_mizrakcilar-(mizrakci_kaybi * mizrakcilar_saglik);

                fprintf(dosya, "Kalan Mizrakcilar: %d\n", data->ork_legi.birimler.mizrakcilar);
                printf("Kalan Mizrakcilar: %d\n", data->ork_legi.birimler.mizrakcilar);
            } else {
                fprintf(dosya, "Troller'e Dagitilan Hasar Yok\n");
                printf("Troller'e Dagitilan Hasar Yok\n");
            }
        }

    } else {
        // Insan ordusu icin hasar dagilimi
        int piyade_savunma_gucu = single_unit_savunma_gucu("insan_imparatorlugu", "piyade", data);
        int suvari_savunma_gucu = single_unit_savunma_gucu("insan_imparatorlugu", "suvari", data);
        int kusatma_makinesi_gucu = single_unit_savunma_gucu("insan_imparatorlugu", "kusatma_makinesi", data);
        int okcu_gucu = single_unit_savunma_gucu("insan_imparatorlugu", "okcu", data);

        printf("Piyade Savunma Gucu: %d\n", piyade_savunma_gucu);
        printf("Suvari Savunma Gucu: %d\n", suvari_savunma_gucu);
        printf("Kusatma Makinesi Savunma Gucu: %d\n", kusatma_makinesi_gucu);
        printf("Okcu Savunma Gucu: %d\n", okcu_gucu);

        fprintf(dosya,"Piyade Savunma Gucu: %d\n", piyade_savunma_gucu);
        fprintf(dosya,"Suvari Savunma Gucu: %d\n", suvari_savunma_gucu);
        fprintf(dosya,"Kusatma Makinesi Savunma Gucu: %d\n", kusatma_makinesi_gucu);
        fprintf(dosya,"Okcu Savunma Gucu: %d\n", okcu_gucu);

	if (data->insan_imparatorlugu.birimler.piyadeler > 0) {
	    float hasar_dagilimi_piyade = (float)(ork_toplam_saldiri - insan_toplam_savunma) * ((float)piyade_savunma_gucu / insan_toplam_savunma);

		hasar_dagilimi_piyade += kalan_piyade_hasar;

	    if (hasar_dagilimi_piyade > 0) {
	        fprintf(dosya, "Piyadelere Dagitilan Hasar: %.2f\n", hasar_dagilimi_piyade);
	        printf("Piyadelere Dagitilan Hasar: %.2f\n", hasar_dagilimi_piyade);
	        int piyade_kaybi = (int)(hasar_dagilimi_piyade / piyade_saglik);
	        data->insan_imparatorlugu.birimler.piyadeler -= piyade_kaybi;
	        if (data->insan_imparatorlugu.birimler.piyadeler < 0) data->insan_imparatorlugu.birimler.piyadeler = 0;

	        kalan_piyade_hasar = hasar_dagilimi_piyade-(piyade_kaybi * piyade_saglik);

	        fprintf(dosya, "Kalan Piyade: %d\n", data->insan_imparatorlugu.birimler.piyadeler);
	        printf("Kalan Piyade: %d\n", data->insan_imparatorlugu.birimler.piyadeler);
	    } else {
	        fprintf(dosya, "Piyadelere Dagitilan Hasar Yok\n");
	        printf("Piyadelere Dagitilan Hasar Yok\n");
	    }
	}

	if (data->insan_imparatorlugu.birimler.suvariler > 0) {
	    float hasar_dagilimi_suvari = (float)(ork_toplam_saldiri - insan_toplam_savunma) * ((float)suvari_savunma_gucu / insan_toplam_savunma);

		hasar_dagilimi_suvari += kalan_suvari_hasar;

	    if (hasar_dagilimi_suvari > 0) {
	        fprintf(dosya, "Suvarilere Dagitilan Hasar: %.2f\n", hasar_dagilimi_suvari);
	        printf("Suvarilere Dagitilan Hasar: %.2f\n", hasar_dagilimi_suvari);
	        int suvari_kaybi = (int)(hasar_dagilimi_suvari / suvari_saglik);
	        data->insan_imparatorlugu.birimler.suvariler -= suvari_kaybi;

	        kalan_suvari_hasar = hasar_dagilimi_suvari-(suvari_kaybi * suvari_saglik);

	        if (data->insan_imparatorlugu.birimler.suvariler < 0) data->insan_imparatorlugu.birimler.suvariler = 0;
	        fprintf(dosya, "Kalan Suvariler: %d\n", data->insan_imparatorlugu.birimler.suvariler);
	        printf("Kalan Suvariler: %d\n", data->insan_imparatorlugu.birimler.suvariler);
	    } else {
	        fprintf(dosya, "Suvarilere Dagitilan Hasar Yok\n");
	        printf("Suvarilere Dagitilan Hasar Yok\n");
	    }
	}

	if (data->insan_imparatorlugu.birimler.kusatma_makineleri > 0) {
	    float hasar_dagilimi_kusatma_makinesi = (float)(ork_toplam_saldiri - insan_toplam_savunma) * ((float)kusatma_makinesi_gucu / insan_toplam_savunma);

		hasar_dagilimi_kusatma_makinesi += kalan_kusatma_makinesi_hasar;

	    if (hasar_dagilimi_kusatma_makinesi > 0) {
	        fprintf(dosya, "Kusatma Makinesine Dagitilan Hasar: %.2f\n", hasar_dagilimi_kusatma_makinesi);
	        printf("Kusatma Makinesine Dagitilan Hasar: %.2f\n", hasar_dagilimi_kusatma_makinesi);
	        int kusatma_makinesi_kaybi = (int)(hasar_dagilimi_kusatma_makinesi / kusatma_makinesi_saglik);
	        data->insan_imparatorlugu.birimler.kusatma_makineleri -= kusatma_makinesi_kaybi;

	        kalan_kusatma_makinesi_hasar = hasar_dagilimi_kusatma_makinesi-(kusatma_makinesi_kaybi * kusatma_makinesi_saglik);

	        if (data->insan_imparatorlugu.birimler.kusatma_makineleri < 0) data->insan_imparatorlugu.birimler.kusatma_makineleri = 0;
	        fprintf(dosya, "Kalan Kusatma Makinesi: %d\n", data->insan_imparatorlugu.birimler.kusatma_makineleri);
	        printf("Kalan Kusatma Makinesi: %d\n", data->insan_imparatorlugu.birimler.kusatma_makineleri);
	    } else {
	        fprintf(dosya, "Kusatma Makinesine Dagitilan Hasar Yok\n");
	        printf("Kusatma Makinesine Dagitilan Hasar Yok\n");
	    }
	}

	if (data->insan_imparatorlugu.birimler.okcular > 0) {
	    float hasar_dagilimi_okcu = (float)(ork_toplam_saldiri - insan_toplam_savunma) * ((float)okcu_gucu / insan_toplam_savunma);

		hasar_dagilimi_okcu += kalan_okcu_hasar;

	    if (hasar_dagilimi_okcu > 0) {
	        fprintf(dosya, "Okcilara Dagitilan Hasar: %.2f\n", hasar_dagilimi_okcu);
	        printf("Okcilara Dagitilan Hasar: %.2f\n", hasar_dagilimi_okcu);
	        int okcu_kaybi = (int)(hasar_dagilimi_okcu / okcu_saglik);
	        data->insan_imparatorlugu.birimler.okcular -= okcu_kaybi;

	        kalan_okcu_hasar = hasar_dagilimi_okcu-(okcu_kaybi * okcu_saglik);

	        if (data->insan_imparatorlugu.birimler.okcular < 0) data->insan_imparatorlugu.birimler.okcular = 0;
	        fprintf(dosya, "Kalan Okcilar: %d\n", data->insan_imparatorlugu.birimler.okcular);
	        printf("Kalan Okcilar: %d\n", data->insan_imparatorlugu.birimler.okcular);
	    } else {
	        fprintf(dosya, "Okcilara Dagitilan Hasar Yok\n");
	        printf("Okcilara Dagitilan Hasar Yok\n");
	    }
	}

    }
    printf("\n");
}

int tum_birimler_tukendi(Data* data, const char* taraf) {
    if (strcmp(taraf, "insan_imparatorlugu") == 0) {
        return data->insan_imparatorlugu.birimler.piyadeler <= 0 && data->insan_imparatorlugu.birimler.suvariler <= 0 && data->insan_imparatorlugu.birimler.okcular <= 0 && data->insan_imparatorlugu.birimler.kusatma_makineleri <= 0;
    } else {
        return data->ork_legi.birimler.ork_dovusculeri <= 0 && data->ork_legi.birimler.mizrakcilar <= 0 && data->ork_legi.birimler.varg_binicileri <= 0 && data->ork_legi.birimler.troller <= 0;
    }
}

void print_birim(const char* birim_adi, int saldiri, int savunma, int saglik, int kritik_sans) {
    printf("%s:\n", birim_adi);
    printf("  Saldiri: %d\n", saldiri);
    printf("  Savunma: %d\n", savunma);
    printf("  Saglik: %d\n", saglik);
    printf("  Kritik  ans: %d%%\n\n", kritik_sans);
}

void unit_type_json(const char* filename) {
	FILE* file = fopen(filename, "r");
	if (file == NULL) {
		printf("Dosya acilamadi!\n");
		return;
	}

	char line[MAX_LINE_LENGTH];
	int isOrkLegi = 0, isInsanImparatorlugu = 0;

	while (fgets(line, sizeof(line), file)) {
		if (strstr(line, "\"ork_legi\"")) {
			isOrkLegi = 1;
			isInsanImparatorlugu = 0;
			continue;
		}

		if (strstr(line, "\"insan_imparatorlugu\"")) {
			isOrkLegi = 0;
			isInsanImparatorlugu = 1;
			continue;
		}

		if (isInsanImparatorlugu) {
			if (strstr(line, "\"piyadeler\"")) {
				while (fgets(line, sizeof(line), file) && strstr(line, "}") == NULL) {
					if (strstr(line, "saldiri")) {
						sscanf(line, " \"saldiri\": %d,", &piyade_saldiri_gucu);
					} else if (strstr(line, "savunma")) {
						sscanf(line, " \"savunma\": %d,", &piyade_savunma);
					} else if (strstr(line, "saglik")) {
						sscanf(line, " \"saglik\": %d,", &piyade_saglik);
					} else if (strstr(line, "kritik_sans")) {
						sscanf(line, " \"kritik_sans\": %d,", &piyade_kritik_sans);
					}
				}
			} else if (strstr(line, "\"okcular\"")) {
				while (fgets(line, sizeof(line), file) && strstr(line, "}") == NULL) {
					if (strstr(line, "saldiri")) {
						sscanf(line, " \"saldiri\": %d,", &okcu_saldiri_gucu);
					} else if (strstr(line, "savunma")) {
						sscanf(line, " \"savunma\": %d,", &okcu_savunma);
					} else if (strstr(line, "saglik")) {
						sscanf(line, " \"saglik\": %d,", &okcu_saglik);
					} else if (strstr(line, "kritik_sans")) {
						sscanf(line, " \"kritik_sans\": %d,", &okcu_kritik_sans);
					}
				}
			} else if (strstr(line, "\"suvariler\"")) {
				while (fgets(line, sizeof(line), file) && strstr(line, "}") == NULL) {
					if (strstr(line, "saldiri")) {
						sscanf(line, " \"saldiri\": %d,", &suvari_saldiri_gucu);
					} else if (strstr(line, "savunma")) {
						sscanf(line, " \"savunma\": %d,", &suvari_savunma);
					} else if (strstr(line, "saglik")) {
						sscanf(line, " \"saglik\": %d,", &suvari_saglik);
					} else if (strstr(line, "kritik_sans")) {
						sscanf(line, " \"kritik_sans\": %d,", &suvari_kritik_sans);
					}
				}
			} else if (strstr(line, "\"kusatma_makineleri\"")) {
				while (fgets(line, sizeof(line), file) && strstr(line, "}") == NULL) {
					if (strstr(line, "saldiri")) {
						sscanf(line, " \"saldiri\": %d,", &kusatma_makinesi_saldiri_gucu);
					} else if (strstr(line, "savunma")) {
						sscanf(line, " \"savunma\": %d,", &kusatma_makinesi_savunma);
					} else if (strstr(line, "saglik")) {
						sscanf(line, " \"saglik\": %d,", &kusatma_makinesi_saglik);
					} else if (strstr(line, "kritik_sans")) {
						sscanf(line, " \"kritik_sans\": %d,", &kusatma_makinesi_kritik_sans);
					}
				}
			}
		}

		if (isOrkLegi) {
			if (strstr(line, "\"ork_dovusculeri\"")) {
				while (fgets(line, sizeof(line), file) && strstr(line, "}") == NULL) {
					if (strstr(line, "saldiri")) {
						sscanf(line, " \"saldiri\": %d,", &ork_dovusculeri_saldiri_gucu);
					} else if (strstr(line, "savunma")) {
						sscanf(line, " \"savunma\": %d,", &ork_dovusculeri_savunma);
					} else if (strstr(line, "saglik")) {
						sscanf(line, " \"saglik\": %d,", &ork_dovusculeri_saglik);
					} else if (strstr(line, "kritik_sans")) {
						sscanf(line, " \"kritik_sans\": %d,", &ork_dovusculeri_kritik_sans);
					}
				}
			} else if (strstr(line, "\"mizrakcilar\"")) {
				while (fgets(line, sizeof(line), file) && strstr(line, "}") == NULL) {
					if (strstr(line, "saldiri")) {
						sscanf(line, " \"saldiri\": %d,", &mizrakcilar_saldiri_gucu);
					} else if (strstr(line, "savunma")) {
						sscanf(line, " \"savunma\": %d,", &mizrakcilar_savunma);
					} else if (strstr(line, "saglik")) {
						sscanf(line, " \"saglik\": %d,", &mizrakcilar_saglik);
					} else if (strstr(line, "kritik_sans")) {
						sscanf(line, " \"kritik_sans\": %d,", &mizrakcilar_kritik_sans);
					}
				}
			} else if (strstr(line, "\"varg_binicileri\"")) {
				while (fgets(line, sizeof(line), file) && strstr(line, "}") == NULL) {
					if (strstr(line, "saldiri")) {
						sscanf(line, " \"saldiri\": %d,", &varg_binicileri_saldiri_gucu);
					} else if (strstr(line, "savunma")) {
						sscanf(line, " \"savunma\": %d,", &varg_binicileri_savunma);
					} else if (strstr(line, "saglik")) {
						sscanf(line, " \"saglik\": %d,", &varg_binicileri_saglik);
					} else if (strstr(line, "kritik_sans")) {
						sscanf(line, " \"kritik_sans\": %d,", &varg_binicileri_kritik_sans);
					}
				}
			} else if (strstr(line, "\"troller\"")) {
				while (fgets(line, sizeof(line), file) && strstr(line, "}") == NULL) {
					if (strstr(line, "saldiri")) {
						sscanf(line, " \"saldiri\": %d,", &troll_saldiri_gucu);
					} else if (strstr(line, "savunma")) {
						sscanf(line, " \"savunma\": %d,", &troll_savunma);
					} else if (strstr(line, "saglik")) {
						sscanf(line, " \"saglik\": %d,", &troll_saglik);
					}else if (strstr(line, "kritik_sans")) {
						sscanf(line, " \"kritik_sans\": %d,", &troll_kritik_sans);
					}
				}
			}
		}
	}

	fclose(file);
}

void yazdir_unit_types() {
    printf("Insan Imparatorlugu Birimleri:\n");
    print_birim("Piyadeler", piyade_saldiri_gucu, piyade_savunma, piyade_saglik, piyade_kritik_sans);
    print_birim("Okcular", okcu_saldiri_gucu, okcu_savunma, okcu_saglik, okcu_kritik_sans);
    print_birim("Suvariler", suvari_saldiri_gucu, suvari_savunma, suvari_saglik, suvari_kritik_sans);
    print_birim("Kusatma Makineleri", kusatma_makinesi_saldiri_gucu, kusatma_makinesi_savunma, kusatma_makinesi_saglik, kusatma_makinesi_kritik_sans);

    printf("\nOrk Lejyonu Birimleri:\n");
    print_birim("Ork Dovusculeri", ork_dovusculeri_saldiri_gucu, ork_dovusculeri_savunma, ork_dovusculeri_saglik, ork_dovusculeri_kritik_sans);
    print_birim("Varg Binicileri", varg_binicileri_saldiri_gucu, varg_binicileri_savunma, varg_binicileri_saglik, varg_binicileri_kritik_sans);
    print_birim("Mizrakcilar", mizrakcilar_saldiri_gucu, mizrakcilar_savunma, mizrakcilar_saglik, mizrakcilar_kritik_sans);
    print_birim("Troller", troll_saldiri_gucu, troll_savunma, troll_saglik, troll_kritik_sans);
}

void heroes_json(kahramanData* data, const char* filename) {
	FILE* file = fopen(filename, "r");
	if (file == NULL) {
		printf("Dosya acilamadi!\n");
	}

	char line[256];
	char kahraman_ismi[MAX_LINE_LENGTH];
	char bonus_turu[MAX_LINE_LENGTH];
	char bonus_degeri[MAX_LINE_LENGTH];
	char aciklama[256];

	int isOrkLegi = 0, isInsanImparatorlugu = 0;

	while (fgets(line, sizeof(line), file)) {
		if (strstr(line, "\"ork_legi\"")) {
			isOrkLegi = 1;
			isInsanImparatorlugu = 0;
			continue;
		}
		if (strstr(line, "\"insan_imparatorlugu\"")) {
			isOrkLegi = 0;
			isInsanImparatorlugu = 1;
			continue;
		}

		if (isOrkLegi || isInsanImparatorlugu) {
			char* ptr = line;
			if (sscanf(ptr, " \"%[^\"]\" : {", kahraman_ismi) == 1) {
				fgets(line, sizeof(line), file);
				sscanf(line, " \"bonus_turu\" : \"%[^\"]\"", bonus_turu);
				fgets(line, sizeof(line), file);
				sscanf(line, " \"bonus_degeri\" : \"%[^\"]\"", bonus_degeri);
				fgets(line, sizeof(line), file);
				sscanf(line, " \"aciklama\" : \"%[^\"]\"", aciklama);

				if (isOrkLegi) {
					int idx = data->orkLegi.kahraman_sayisi;
					strcpy(data->orkLegi.kahramanlar[idx].kahraman_ismi, kahraman_ismi);
					strcpy(data->orkLegi.kahramanlar[idx].bonus_turu, bonus_turu);
					strcpy(data->orkLegi.kahramanlar[idx].bonus_degeri, bonus_degeri);
					strcpy(data->orkLegi.kahramanlar[idx].aciklama, aciklama);
					data->orkLegi.kahraman_sayisi++;
				}

				if (isInsanImparatorlugu) {
					int idx = data->insanImparatorlugu.kahraman_sayisi;
					strcpy(data->insanImparatorlugu.kahramanlar[idx].kahraman_ismi, kahraman_ismi);
					strcpy(data->insanImparatorlugu.kahramanlar[idx].bonus_turu, bonus_turu);
					strcpy(data->insanImparatorlugu.kahramanlar[idx].bonus_degeri, bonus_degeri);
					strcpy(data->insanImparatorlugu.kahramanlar[idx].aciklama, aciklama);
					data->insanImparatorlugu.kahraman_sayisi++;
				}
			}
		}
	}
	fclose(file);
}

void yazdir_heroes(kahramanData* kahraman_data){
	printf("\nOrk Lejisi Kahramanlari:");
	for (int i = 0; i < kahraman_data->orkLegi.kahraman_sayisi; i++) {
		Kahraman* kahraman = &kahraman_data->orkLegi.kahramanlar[i];
		printf("Kahraman Ismi: %s\n", kahraman->kahraman_ismi);
		printf("Bonus Turu: %s\n", kahraman->bonus_turu);
		printf("Bonus Degeri: %s\n", kahraman->bonus_degeri);
		printf("Aciklama: %s\n\n", kahraman->aciklama);
	}

	printf("\nInsan Imparatorlugu Kahramanlari:\n");
	for (int i = 0; i < kahraman_data->insanImparatorlugu.kahraman_sayisi; i++) {
		Kahraman* kahraman = &kahraman_data->insanImparatorlugu.kahramanlar[i];
		printf("Kahraman Ismi: %s\n", kahraman->kahraman_ismi);
		printf("Bonus Turu: %s\n", kahraman->bonus_turu);
		printf("Bonus Degeri: %s\n", kahraman->bonus_degeri);
		printf("Aciklama: %s\n\n", kahraman->aciklama);
	}

}

void heroes_hesapla(kahramanData* kahraman_data,Data* data){

	for (int i = 0; i < data->insan_imparatorlugu.kahraman_sayisi; i++) {
		for (int j = 0; j < kahraman_data->insanImparatorlugu.kahraman_sayisi; j++) {
			Kahraman* kahraman = &kahraman_data->insanImparatorlugu.kahramanlar[j];
			if (strstr(data->insan_imparatorlugu.kahramanlar[i],kahraman_data->insanImparatorlugu.kahramanlar[j].kahraman_ismi)){

				printf("Insan Imparatorlugu:\nBuff'i Eklenen Kahraman:%s\n",kahraman_data->insanImparatorlugu.kahramanlar[j].kahraman_ismi);
				if (strstr(kahraman->aciklama, "piyade")) {
					if(strstr(kahraman->bonus_turu, "saldiri")){
						int int_value = atoi(kahraman->bonus_degeri);
						float float_value = int_value / 100.0;
						piyade_saldiri_gucu *= (1.0 + float_value);
					}
					if(strstr(kahraman->bonus_turu, "savunma")){
						int int_value = atoi(kahraman->bonus_degeri);
						float float_value = int_value / 100.0;
						piyade_savunma *= (1.0 + float_value);
					}
					if(strstr(kahraman->bonus_turu, "kritik_sans")){
						int int_value = atoi(kahraman->bonus_degeri);
						float float_value = int_value;
						piyade_kritik_sans += float_value;
					}
				}
				if (strstr(kahraman->aciklama, "okcu")) {
					if(strstr(kahraman->bonus_turu, "saldiri")){
						int int_value = atoi(kahraman->bonus_degeri);
						float float_value = int_value / 100.0;
						okcu_saldiri_gucu *= (1.0 + float_value);
					}
					if(strstr(kahraman->bonus_turu, "savunma")){
						int int_value = atoi(kahraman->bonus_degeri);
						float float_value = int_value / 100.0;
						okcu_savunma *= (1.0 + float_value);
					}
					if(strstr(kahraman->bonus_turu, "kritik_sans")){
						int int_value = atoi(kahraman->bonus_degeri);
						float float_value = int_value;
						okcu_kritik_sans += float_value;
					}
				}
				if (strstr(kahraman->aciklama, "suvari")) {
					if(strstr(kahraman->bonus_turu, "saldiri")){
						int int_value = atoi(kahraman->bonus_degeri);
						float float_value = int_value / 100.0;
						suvari_saldiri_gucu *= (1.0 + float_value);
					}
					if(strstr(kahraman->bonus_turu, "savunma")){
						int int_value = atoi(kahraman->bonus_degeri);
						float float_value = int_value / 100.0;
						suvari_savunma *= (1.0 + float_value);
					}
					if(strstr(kahraman->bonus_turu, "kritik_sans")){
						int int_value = atoi(kahraman->bonus_degeri);
						float float_value = int_value;
						suvari_kritik_sans += float_value;
					}
				}
				if (strstr(kahraman->aciklama, "kusatma_makine")) {
					if(strstr(kahraman->bonus_turu, "saldiri")){
						int int_value = atoi(kahraman->bonus_degeri);
						float float_value = int_value / 100.0;
						kusatma_makinesi_saldiri_gucu *= (1.0 + float_value);
					}
					if(strstr(kahraman->bonus_turu, "savunma")){
						int int_value = atoi(kahraman->bonus_degeri);
						float float_value = int_value / 100.0;
						kusatma_makinesi_savunma *= (1.0 + float_value);
					}
					if(strstr(kahraman->bonus_turu, "kritik_sans")){
						int int_value = atoi(kahraman->bonus_degeri);
						float float_value = int_value;
						kusatma_makinesi_kritik_sans += float_value;
					}
				}
				if (strstr(kahraman->aciklama, "tum_birim")) {
					if(strstr(kahraman->bonus_turu, "saldiri")){
						int int_value = atoi(kahraman->bonus_degeri);
						float float_value = int_value / 100.0;
						insan_imparatorlugu_bonus += float_value;
					}
					if(strstr(kahraman->bonus_turu, "savunma")){
						int int_value = atoi(kahraman->bonus_degeri);
						float float_value = int_value / 100.0;
						insan_imparatorlugu_defence_bonus += float_value;
					}
				}
			}
		}
	}
	for (int i = 0; i < data->ork_legi.kahraman_sayisi; i++) {
		for (int j = 0; j < kahraman_data->orkLegi.kahraman_sayisi; j++) {

			Kahraman* kahraman = &kahraman_data->orkLegi.kahramanlar[j];

			if (strstr(data->ork_legi.kahramanlar[i],kahraman_data->orkLegi.kahramanlar[j].kahraman_ismi)){
				printf("Ork Legionu:\nBuff'i Eklenen Kahraman:%s\n",kahraman_data->orkLegi.kahramanlar[j].kahraman_ismi);

				if (strstr(kahraman->aciklama, "ork_dovuscu")) {
					if(strstr(kahraman->bonus_turu, "saldiri")){
						int int_value = atoi(kahraman->bonus_degeri);
						float float_value = int_value / 100.0;
						ork_dovusculeri_saldiri_gucu *= (1.0 + float_value);
					}
					if(strstr(kahraman->bonus_turu, "savunma")){
						int int_value = atoi(kahraman->bonus_degeri);
						float float_value = int_value / 100.0;
						ork_dovusculeri_savunma *= (1.0 + float_value);
					}
					if(strstr(kahraman->bonus_turu, "kritik_sans")){
						int int_value = atoi(kahraman->bonus_degeri);
						float float_value = int_value;
						ork_dovusculeri_kritik_sans += float_value;
					}
				}
				if (strstr(kahraman->aciklama, "varg_binici")) {
					if(strstr(kahraman->bonus_turu, "saldiri")){
						int int_value = atoi(kahraman->bonus_degeri);
						float float_value = int_value / 100.0;
						varg_binicileri_saldiri_gucu *= (1.0 + float_value);
					}
					if(strstr(kahraman->bonus_turu, "savunma")){
						int int_value = atoi(kahraman->bonus_degeri);
						float float_value = int_value / 100.0;
						varg_binicileri_savunma *= (1.0 + float_value);
					}
					if(strstr(kahraman->bonus_turu, "kritik_sans")){
						int int_value = atoi(kahraman->bonus_degeri);
						float float_value = int_value;
						varg_binicileri_kritik_sans += float_value;
					}
				}
				if (strstr(kahraman->aciklama, "mizrakci")) {
					if(strstr(kahraman->bonus_turu, "saldiri")){
						int int_value = atoi(kahraman->bonus_degeri);
						float float_value = int_value / 100.0;
						mizrakcilar_saldiri_gucu *= (1.0 + float_value);
					}
					if(strstr(kahraman->bonus_turu, "savunma")){
						int int_value = atoi(kahraman->bonus_degeri);
						float float_value = int_value / 100.0;
						mizrakcilar_savunma *= (1.0 + float_value);
					}
					if(strstr(kahraman->bonus_turu, "kritik_sans")){
						int int_value = atoi(kahraman->bonus_degeri);
						float float_value = int_value;
						mizrakcilar_kritik_sans += float_value;
					}
				}
				if (strstr(kahraman->aciklama, "troll")) {
					if(strstr(kahraman->bonus_turu, "saldiri")){
						int int_value = atoi(kahraman->bonus_degeri);
						float float_value = int_value / 100.0;
						troll_saldiri_gucu *= (1.0 + float_value);
					}
					if(strstr(kahraman->bonus_turu, "savunma")){
						int int_value = atoi(kahraman->bonus_degeri);
						float float_value = int_value / 100.0;
						troll_savunma *= (1.0 + float_value);
					}
					if(strstr(kahraman->bonus_turu, "kritik_sans")){
						int int_value = atoi(kahraman->bonus_degeri);
						float float_value = int_value;
						troll_kritik_sans += float_value;
					}
				}
				if (strstr(kahraman->aciklama, "tum_biriml")) {
					if(strstr(kahraman->bonus_turu, "saldiri")){
						int int_value = atoi(kahraman->bonus_degeri);
						float float_value = int_value / 100.0;
						ork_legi_bonus += float_value;
					}
					if(strstr(kahraman->bonus_turu, "savunma")){
						int int_value = atoi(kahraman->bonus_degeri);
						float float_value = int_value / 100.0;
						ork_legi_defence_bonus += float_value;
					}
				}
			}
		}
	}
}

void creatures_json(CanavarData* data, const char* filename) {
	FILE* file = fopen(filename, "r");
	if (file == NULL) {
		printf("Dosya acilamadi!\n");
	}

	char line[MAX_LINE_LENGTH];
	char canavar_ismi[MAX_LINE_LENGTH];
	char bonus_degeri[MAX_LINE_LENGTH];
	char bonus_turu[MAX_LINE_LENGTH];
	char aciklama[MAX_LINE_LENGTH];

	int isOrkLegi = 0, isInsanImparatorlugu = 0;

	while (fgets(line, sizeof(line), file)) {

		if (strstr(line, "\"ork_legi\"")) {
			isOrkLegi = 1;
			isInsanImparatorlugu = 0;
			continue;
		}

		if (strstr(line, "\"insan_imparatorlugu\"")) {
			isOrkLegi = 0;
			isInsanImparatorlugu = 1;
			continue;
		}

		if (isOrkLegi || isInsanImparatorlugu) {
			char* ptr = line;

			if (sscanf(ptr, " \"%[^\"]\" : {", canavar_ismi) == 1) {

				fgets(line, sizeof(line), file);
				sscanf(line, " \"etki_degeri\" : \"%[^\"]\"", bonus_degeri);

				fgets(line, sizeof(line), file);
				sscanf(line, " \"etki_turu\" : \"%[^\"]\"", bonus_turu);

				fgets(line, sizeof(line), file);
				sscanf(line, " \"aciklama\" : \"%[^\"]\"", aciklama);

				if (isOrkLegi) {
					int idx = data->orkLegi.canavar_sayisi;
					strcpy(data->orkLegi.canavarlar[idx].canavar_ismi, canavar_ismi);
					strcpy(data->orkLegi.canavarlar[idx].bonus_degeri, bonus_degeri);
					strcpy(data->orkLegi.canavarlar[idx].bonus_turu, bonus_turu);
					strcpy(data->orkLegi.canavarlar[idx].aciklama, aciklama);
					data->orkLegi.canavar_sayisi++;
				}

				if (isInsanImparatorlugu) {
					int idx = data->insanImparatorlugu.canavar_sayisi;
					strcpy(data->insanImparatorlugu.canavarlar[idx].canavar_ismi, canavar_ismi);
					strcpy(data->insanImparatorlugu.canavarlar[idx].bonus_degeri, bonus_degeri);
					strcpy(data->insanImparatorlugu.canavarlar[idx].bonus_turu, bonus_turu);
					strcpy(data->insanImparatorlugu.canavarlar[idx].aciklama, aciklama);
					data->insanImparatorlugu.canavar_sayisi++;
				}
			}
		}
	}

	fclose(file);
}

void yazdir_creatures(CanavarData* canavar_data){
	printf("Ork Lejisi Canavarlari:\n");
	for (int i = 0; i < canavar_data->orkLegi.canavar_sayisi; i++) {
		Canavar* canavar = &canavar_data->orkLegi.canavarlar[i];
		printf("Canavar Ismi: %s\n", canavar->canavar_ismi);
		printf("Bonus Turu: %s\n", canavar->bonus_turu);
		printf("Bonus Degeri: %s\n", canavar->bonus_degeri);
		printf("Aciklama: %s\n\n", canavar->aciklama);
	}

	printf("Insan Imparatorlugu Canavarlari:\n");
	for (int i = 0; i < canavar_data->insanImparatorlugu.canavar_sayisi; i++) {
		Canavar* canavar = &canavar_data->insanImparatorlugu.canavarlar[i];
		printf("Canavar Ismi: %s\n", canavar->canavar_ismi);
		printf("Bonus Turu: %s\n", canavar->bonus_turu);
		printf("Bonus Degeri: %s\n", canavar->bonus_degeri);
		printf("Aciklama: %s\n\n", canavar->aciklama);
	}

}

void creatures_hesapla(CanavarData* canavar_data,Data* data){

	for (int i = 0; i < data->ork_legi.canavar_sayisi; i++) {
		for (int j = 0; j < canavar_data->orkLegi.canavar_sayisi; j++) {

			Canavar* canavar = &canavar_data->orkLegi.canavarlar[j];

			if (strstr(data->ork_legi.canavarlar[i],canavar_data->orkLegi.canavarlar[j].canavar_ismi)){
				printf("Ork Leg:\nBuff'i Eklenen Canavar:%s\n",canavar_data->orkLegi.canavarlar[j].canavar_ismi);

				if (strstr(canavar->aciklama, "ork_dovuscu")) {
					if(strstr(canavar->bonus_turu, "saldiri")){
						int int_value = atoi(canavar->bonus_degeri);
						float float_value = int_value / 100.0;
						ork_dovusculeri_saldiri_gucu *= (1.0 + float_value);
					}
					if(strstr(canavar->bonus_turu, "savunma")){
						int int_value = atoi(canavar->bonus_degeri);
						float float_value = int_value / 100.0;
						ork_dovusculeri_savunma *= (1.0 + float_value);
					}
					if(strstr(canavar->bonus_turu, "kritik_sans")){
						int int_value = atoi(canavar->bonus_degeri);
						float float_value = int_value;
						ork_dovusculeri_kritik_sans += float_value;
					}
				}
				if (strstr(canavar->aciklama, "varg_binici")) {
					if(strstr(canavar->bonus_turu, "saldiri")){
						int int_value = atoi(canavar->bonus_degeri);
						float float_value = int_value / 100.0;
						varg_binicileri_saldiri_gucu *= (1.0 + float_value);
					}
					if(strstr(canavar->bonus_turu, "savunma")){
						int int_value = atoi(canavar->bonus_degeri);
						float float_value = int_value / 100.0;
						varg_binicileri_savunma *= (1.0 + float_value);
					}
					if(strstr(canavar->bonus_turu, "kritik_sans")){
						int int_value = atoi(canavar->bonus_degeri);
						float float_value = int_value;
						varg_binicileri_kritik_sans += float_value;
					}
				}
				if (strstr(canavar->aciklama, "mizrakci")) {
					if(strstr(canavar->bonus_turu, "saldiri")){
						int int_value = atoi(canavar->bonus_degeri);
						float float_value = int_value / 100.0;
						mizrakcilar_saldiri_gucu *= (1.0 + float_value);
					}
					if(strstr(canavar->bonus_turu, "savunma")){
						int int_value = atoi(canavar->bonus_degeri);
						float float_value = int_value / 100.0;
						mizrakcilar_savunma *= (1.0 + float_value);
					}
					if(strstr(canavar->bonus_turu, "kritik_sans")){
						int int_value = atoi(canavar->bonus_degeri);
						float float_value = int_value;
						mizrakcilar_kritik_sans += float_value;
					}
				}
				if (strstr(canavar->aciklama, "troll")) {
					if(strstr(canavar->bonus_turu, "saldiri")){
						int int_value = atoi(canavar->bonus_degeri);
						float float_value = int_value / 100.0;
						troll_saldiri_gucu *= (1.0 + float_value);
					}
					if(strstr(canavar->bonus_turu, "savunma")){
						int int_value = atoi(canavar->bonus_degeri);
						float float_value = int_value / 100.0;
						troll_savunma *= (1.0 + float_value);
					}
					if(strstr(canavar->bonus_turu, "kritik_sans")){
						int int_value = atoi(canavar->bonus_degeri);
						float float_value = int_value;
						troll_kritik_sans += float_value;
					}
				}
				if (strstr(canavar->aciklama, "tum_birim")) {
					if(strstr(canavar->bonus_turu, "saldiri")){
						int int_value = atoi(canavar->bonus_degeri);
						float float_value = int_value / 100.0;
						ork_legi_bonus += float_value;
					}
					if(strstr(canavar->bonus_turu, "savunma")){
						int int_value = atoi(canavar->bonus_degeri);
						float float_value = int_value / 100.0;
						ork_legi_defence_bonus += float_value;
					}
				}

			}
		}
	}
	for (int i = 0; i < data->insan_imparatorlugu.canavar_sayisi; i++) {
		for (int j = 0; j < canavar_data->insanImparatorlugu.canavar_sayisi; j++) {

			Canavar* canavar = &canavar_data->insanImparatorlugu.canavarlar[j];

			if (strstr(data->insan_imparatorlugu.canavarlar[i],canavar_data->insanImparatorlugu.canavarlar[j].canavar_ismi)){
				printf("Insan Imp:\nBuff'i Eklenen Canavar:%s\n",canavar_data->insanImparatorlugu.canavarlar[j].canavar_ismi);

				if (strstr(canavar->aciklama, "piyade")) {
					if(strstr(canavar->bonus_turu, "saldiri")){
						int int_value = atoi(canavar->bonus_degeri);
						float float_value = int_value / 100.0;
						piyade_saldiri_gucu *= (1.0 + float_value);

					}
					if(strstr(canavar->bonus_turu, "savunma")){
						int int_value = atoi(canavar->bonus_degeri);
						float float_value = int_value / 100.0;
						piyade_savunma *= (1.0 + float_value);
					}
					if(strstr(canavar->bonus_turu, "kritik_sans")){
						int int_value = atoi(canavar->bonus_degeri);
						float float_value = int_value;
						piyade_kritik_sans += float_value;
					}
				}
				if (strstr(canavar->aciklama, "okcu")) {
					if(strstr(canavar->bonus_turu, "saldiri")){
						int int_value = atoi(canavar->bonus_degeri);
						float float_value = int_value / 100.0;
						okcu_saldiri_gucu *= (1.0 + float_value);
					}
					if(strstr(canavar->bonus_turu, "savunma")){
						int int_value = atoi(canavar->bonus_degeri);
						float float_value = int_value / 100.0;
						okcu_savunma *= (1.0 + float_value);
					}
					if(strstr(canavar->bonus_turu, "kritik_sans")){
						int int_value = atoi(canavar->bonus_degeri);
						float float_value = int_value;
						okcu_kritik_sans += float_value;
					}
				}
				if (strstr(canavar->aciklama, "suvari")) {
					if(strstr(canavar->bonus_turu, "saldiri")){
						int int_value = atoi(canavar->bonus_degeri);
						float float_value = int_value / 100.0;
						suvari_saldiri_gucu *= (1.0 + float_value);
					}
					if(strstr(canavar->bonus_turu, "savunma")){
						int int_value = atoi(canavar->bonus_degeri);
						float float_value = int_value / 100.0;
						suvari_savunma *= (1.0 + float_value);
					}
					if(strstr(canavar->bonus_turu, "kritik_sans")){
						int int_value = atoi(canavar->bonus_degeri);
						float float_value = int_value;
						suvari_kritik_sans += float_value;
					}
				}
				if (strstr(canavar->aciklama, "kusatma_makine")) {
					if(strstr(canavar->bonus_turu, "saldiri")){
						int int_value = atoi(canavar->bonus_degeri);
						float float_value = int_value / 100.0;
						kusatma_makinesi_saldiri_gucu *= (1.0 + float_value);
					}
					if(strstr(canavar->bonus_turu, "savunma")){
						int int_value = atoi(canavar->bonus_degeri);
						float float_value = int_value / 100.0;
						kusatma_makinesi_savunma *= (1.0 + (1.0 + float_value));
					}
					if(strstr(canavar->bonus_turu, "kritik_sans")){
						int int_value = atoi(canavar->bonus_degeri);
						float float_value = int_value;
						kusatma_makinesi_kritik_sans += float_value;
					}
				}
				if (strstr(canavar->aciklama, "tum_birim")) {
					if(strstr(canavar->bonus_turu, "saldiri")){
						int int_value = atoi(canavar->bonus_degeri);
						float float_value = int_value / 100.0;
						insan_imparatorlugu_bonus += float_value;
					}
					if(strstr(canavar->bonus_turu, "savunma")){
						int int_value = atoi(canavar->bonus_degeri);
						float float_value = int_value / 100.0;
						insan_imparatorlugu_defence_bonus += float_value;
					}
				}

			}
		}
	}


}

char* research_json(const char* filename) {
	FILE* file = fopen(filename, "r");
	if (file == NULL) {
		printf("Dosya acilamadi!\n");
		return NULL;
	}

	char line[256];


	int savunma_ustaligi = 0, saldiri_gelistirmesi = 0, elit_egitim = 0, kusatma_ustaligi = 0;
	char temp[256] ="";
	while (fgets(line, sizeof(line), file)) {
		if (strstr(line, "\"savunma_ustaligi\"")) {
			strcpy(researchDizi[researchSay].arastirma_ismi, "savunma_ustaligi");
			savunma_ustaligi = 1;
			saldiri_gelistirmesi = 0;
			elit_egitim = 0;
			kusatma_ustaligi = 0;
			strcpy(temp, "savunma_ustaligi");
			continue;
		}

		else if (strstr(line, "\"saldiri_gelistirmesi\"")) {
			strcpy(researchDizi[researchSay].arastirma_ismi, "saldiri_gelistirmesi");
			savunma_ustaligi = 0;
			saldiri_gelistirmesi = 1;
			elit_egitim = 0;
			kusatma_ustaligi = 0;
			strcpy(temp, "saldiri_gelistirmesi");

			continue;
		}

		else if (strstr(line, "\"elit_egitim\"")) {
			strcpy(researchDizi[researchSay].arastirma_ismi, "elit_egitim");
			savunma_ustaligi = 0;
			saldiri_gelistirmesi = 0;
			elit_egitim = 1;
			kusatma_ustaligi = 0;
			strcpy(temp, "elit_egitim");

			continue;
		}

		else if (strstr(line, "\"kusatma_ustaligi\"")) {
			strcpy(researchDizi[researchSay].arastirma_ismi, "kusatma_ustaligi");
			savunma_ustaligi = 0;
			saldiri_gelistirmesi = 0;
			elit_egitim = 0;
			kusatma_ustaligi = 1;
			strcpy(temp, "kusatma_ustaligi");

			continue;
		}
		else{
			strcpy(researchDizi[researchSay].arastirma_ismi, temp);
		}

		if (savunma_ustaligi || saldiri_gelistirmesi || elit_egitim || kusatma_ustaligi) {
			// Seviye bilgilerini okuma
			if (sscanf(line, " \"seviye_%[^\"]\" :", researchDizi[researchSay].seviye) == 1) {
				fgets(line, sizeof(line), file);
				sscanf(line, " \"deger\" : \"%[^\"]\"", researchDizi[researchSay].bonus_degeri);

				fgets(line, sizeof(line), file);
				sscanf(line, " \"aciklama\" : \"%[^\"]\"", researchDizi[researchSay].aciklama);

				researchSay++;
			}
		}
	}

	fclose(file);
	return NULL;
}

void research_hesapla(Data* data){

	char insan_imp_saldiri_gelistirmesi[20];
	sprintf(insan_imp_saldiri_gelistirmesi, "%d", data->insan_imparatorlugu.arastirma_seviyesi.saldiri_gelistirmesi);

	char insan_imp_savunma_ustaligi[20];
	sprintf(insan_imp_savunma_ustaligi, "%d", data->insan_imparatorlugu.arastirma_seviyesi.savunma_ustaligi);

	char insan_imp_saldiri_elit_egitim[20];
	sprintf(insan_imp_saldiri_elit_egitim, "%d", data->insan_imparatorlugu.arastirma_seviyesi.elit_egitim);

	char insan_imp_kusatma_ustaligi[20];
	sprintf(insan_imp_kusatma_ustaligi, "%d", data->insan_imparatorlugu.arastirma_seviyesi.kusatma_ustaligi);


	char ork_legi_saldiri_gelistirmesi[20];
	sprintf(ork_legi_saldiri_gelistirmesi, "%d", data->ork_legi.arastirma_seviyesi.saldiri_gelistirmesi);

	char ork_legi_savunma_ustaligii[20];
	sprintf(ork_legi_savunma_ustaligii, "%d", data->ork_legi.arastirma_seviyesi.savunma_ustaligi);

	char ork_legi_saldiri_elit_egitim[20];
	sprintf(ork_legi_saldiri_elit_egitim, "%d", data->ork_legi.arastirma_seviyesi.elit_egitim);


	for (int i = 0; i < researchSay; i++){


		if(strcmp(researchDizi[i].seviye, insan_imp_saldiri_gelistirmesi) == 0 && strcmp(researchDizi[i].arastirma_ismi, "saldiri_gelistirmesi")== 0 ){

			printf("\nInsan Imparatorlugu Arastirma Etkileri Uygulaniyor\n");
			printf("Arastirma ismi: %s\n", researchDizi[i].arastirma_ismi);
			printf("Seviye: %s\n", researchDizi[i].seviye);
			printf("Bonus degeri: %s\n", researchDizi[i].bonus_degeri);
			printf("Aciklama: %s\n\n", researchDizi[i].aciklama);

			int int_value = atoi(researchDizi[i].bonus_degeri);
			float float_value = int_value / 100.0;
			piyade_saldiri_gucu *=  (1.0 + float_value);
			okcu_saldiri_gucu *=  (1.0 + float_value);
			suvari_saldiri_gucu *=  (1.0 + float_value);
			kusatma_makinesi_saldiri_gucu *=  (1.0 + float_value);

		}
		if(strcmp(researchDizi[i].seviye, insan_imp_savunma_ustaligi) == 0 && strcmp(researchDizi[i].arastirma_ismi, "savunma_ustaligi")== 0) {

			printf("\nInsan Imparatorlugu Arastirma Etkileri Uygulaniyor\n");
			printf("Arastirma ismi: %s\n", researchDizi[i].arastirma_ismi);
			printf("Seviye: %s\n", researchDizi[i].seviye);
			printf("Bonus degeri: %s\n", researchDizi[i].bonus_degeri);
			printf("Aciklama: %s\n\n", researchDizi[i].aciklama);

			int int_value = atoi(researchDizi[i].bonus_degeri);
			float float_value = int_value / 100.0;
			piyade_savunma *=  (1.0 + float_value);
			suvari_savunma *=  (1.0 + float_value);
			okcu_savunma *=  (1.0 + float_value);
			kusatma_makinesi_savunma *=  (1.0 + float_value);


		}
		if(strcmp(researchDizi[i].seviye, insan_imp_saldiri_elit_egitim) == 0 && strcmp(researchDizi[i].arastirma_ismi, "elit_egitim")== 0 ){

			printf("\nInsan Imparatorlugu Arastirma Etkileri Uygulaniyor\n");
			printf("Arastirma ismi: %s\n", researchDizi[i].arastirma_ismi);
			printf("Seviye: %s\n", researchDizi[i].seviye);
			printf("Bonus degeri: %s\n", researchDizi[i].bonus_degeri);
			printf("Aciklama: %s\n\n", researchDizi[i].aciklama);

			int int_value = atoi(researchDizi[i].bonus_degeri);
			float float_value = int_value / 100.0;
			piyade_kritik_sans += float_value;
			suvari_kritik_sans += float_value;
			okcu_kritik_sans += float_value;
			kusatma_makinesi_kritik_sans += float_value;

		}
		if(strcmp(researchDizi[i].seviye, insan_imp_kusatma_ustaligi) == 0 && strcmp(researchDizi[i].arastirma_ismi, "kusatma_ustaligi")== 0 ){

			printf("\nInsan Imparatorlugu Arastirma Etkileri Uygulaniyor\n");
			printf("Arastirma ismi: %s\n", researchDizi[i].arastirma_ismi);
			printf("Seviye: %s\n", researchDizi[i].seviye);
			printf("Bonus degeri: %s\n", researchDizi[i].bonus_degeri);
			printf("Aciklama: %s\n\n", researchDizi[i].aciklama);

			int int_value = atoi(researchDizi[i].bonus_degeri);
			float float_value = int_value / 100.0;
			kusatma_makinesi_saldiri_gucu *=  (1.0 + float_value);

		}
		if(strcmp(researchDizi[i].seviye, ork_legi_saldiri_gelistirmesi) == 0 && strcmp(researchDizi[i].arastirma_ismi, "saldiri_gelistirmesi")== 0 ){

			printf("\nOrk Legi Arastirma Etkileri Uygulaniyor\n");
			printf("Arastirma ismi: %s\n", researchDizi[i].arastirma_ismi);
			printf("Seviye: %s\n", researchDizi[i].seviye);
			printf("Bonus degeri: %s\n", researchDizi[i].bonus_degeri);
			printf("Aciklama: %s\n\n", researchDizi[i].aciklama);

			int int_value = atoi(researchDizi[i].bonus_degeri);
			float float_value = int_value / 100.0;
			ork_dovusculeri_saldiri_gucu *=  (1.0 + float_value);
			mizrakcilar_saldiri_gucu *=  (1.0 + float_value);
			varg_binicileri_saldiri_gucu *=  (1.0 + float_value);
			troll_saldiri_gucu *=  (1.0 + float_value);

		}
		if(strcmp(researchDizi[i].seviye, ork_legi_savunma_ustaligii) == 0&& strcmp(researchDizi[i].arastirma_ismi, "savunma_ustaligi")== 0 ){

			printf("\nOrk Legi Arastirma Etkileri Uygulaniyor\n");
			printf("Arastirma ismi: %s\n", researchDizi[i].arastirma_ismi);
			printf("Seviye: %s\n", researchDizi[i].seviye);
			printf("Bonus degeri: %s\n", researchDizi[i].bonus_degeri);
			printf("Aciklama: %s\n\n", researchDizi[i].aciklama);

			int int_value = atoi(researchDizi[i].bonus_degeri);
			float float_value = int_value / 100.0;
			ork_dovusculeri_savunma *=  (1.0 + float_value);
			mizrakcilar_savunma *=  (1.0 + float_value);
			varg_binicileri_savunma *=  (1.0 + float_value);
			troll_savunma *=  (1.0 + float_value);

		}
		if(strcmp(researchDizi[i].seviye, ork_legi_saldiri_elit_egitim) == 0 && strcmp(researchDizi[i].arastirma_ismi, "elit_egitim")== 0 ){

			printf("\nOrk Legi Arastirma Etkileri Uygulaniyor\n");
			printf("Arastirma ismi: %s\n", researchDizi[i].arastirma_ismi);
			printf("Seviye: %s\n", researchDizi[i].seviye);
			printf("Bonus degeri: %s\n", researchDizi[i].bonus_degeri);
			printf("Aciklama: %s\n\n", researchDizi[i].aciklama);

			int int_value = atoi(researchDizi[i].bonus_degeri);
			float float_value = int_value / 100.0;
			ork_dovusculeri_kritik_sans += float_value;
			mizrakcilar_kritik_sans += float_value;
			varg_binicileri_kritik_sans += float_value;
			troll_kritik_sans += float_value;

		}
	}

}

void tur_sistemi(Data* data) {
    int tur = 1;
    yazdir_unit_types();

    // maksimum 50 tur
    draw_table(data);
    getch();

    while (tur < 50) {
        printf("\n--- Tur %d ---\n", tur);

        printf("Insan Imparatorlugu saldiriyor...\n");
        hasar_dagilimi_fun(data, "insan_imparatorlugu", tur);
        draw_table(data);

        if (tum_birimler_tukendi(data, "ork_legi")) {
            printf("Insan Imparatorlugu kazandi!\n");
            break;
        }

        printf("Ork Lejyonu saldiriyor...\n");
        hasar_dagilimi_fun(data, "ork_legi", tur);
        draw_table(data);

        if (tum_birimler_tukendi(data, "insan_imparatorlugu")) {
            printf("Ork Lejyonu kazandi!\n");
            break;
        }

        printf("Sonraki tura gecmek icin bir tusa basin...\n");
        getch();
        tur++;
    }

    draw_table(data);
    getch();
}

void savas_sim_txt(const char *file_name) {
    FILE *file = fopen(file_name, "r");
    if (file != NULL) {
        fclose(file);
        remove(file_name);
    }
}

void senaryo_sec(Data *data) {
    int secim;
    printf("Senaryo girin(1-10): ");

    scanf("%d",&secim);

    char linkler[10][50]={
    "https://yapbenzet.org.tr/1.json",
    "https://yapbenzet.org.tr/2.json",
    "https://yapbenzet.org.tr/3.json",
    "https://yapbenzet.org.tr/4.json",
    "https://yapbenzet.org.tr/5.json",
    "https://yapbenzet.org.tr/6.json",
    "https://yapbenzet.org.tr/7.json",
    "https://yapbenzet.org.tr/8.json",
    "https://yapbenzet.org.tr/9.json",
    "https://yapbenzet.org.tr/10.json",
    };
    char comut[100]={0};
    sprintf(comut,"curl %s -o data.json",linkler[secim-1]);
    system(comut);

    const char* dosya_yolu = "C:\\Users\\gayra\\Desktop\\strateji_oyunu_v3\\data.json";

    read_json(data,dosya_yolu);
}

void ana_senaryo_yazdir() {
	for (int i = 0; i < researchSay; i++) {

		printf("Arastirma ismi: %s\n", researchDizi[i].arastirma_ismi);
		printf("Seviye: %s\n", researchDizi[i].seviye);
		printf("Bonus degeri: %s\n", researchDizi[i].bonus_degeri);
		printf("Aciklama: %s\n\n", researchDizi[i].aciklama);

	}
}

int main() {
    Data data;
    kahramanData kahraman_data;
    CanavarData canavar_data;

    memset(&data, 0, sizeof(data)); // Veri yapisini  sifirla
    memset(&kahraman_data, 0, sizeof(kahraman_data));
    memset(&canavar_data, 0, sizeof(canavar_data));

	const char *file_name = "savas_sim.txt";

    savas_sim_txt(file_name);

//    printf("\n---------------------------- Birim Ozellikleri ------------------------------------\n");

	unit_type_json("unit_types.json");

//	yazdir_unit_types();


//   printf("\n---------------------------- Kahraman Ozellikleri ---------------------------------\n");

	heroes_json(&kahraman_data,"heroes.json");

//	yazdir_heroes(&kahraman_data);

//	 printf("\n---------------------------- Canavar Ozellikleri ----------------------------------\n");

	creatures_json(&canavar_data,"creatures.json");

//	yazdir_creatures(&canavar_data);

//	printf("\n---------------------------- Arastirme Ozellikleri --------------------------------\n");

//	research_json( "research.json");

//	ana_senaryo_yazdir();

   	senaryo_sec(&data);

    print_data(&data);

    printf("\n----------------------------- Bonuslar Ekleniyor ----------------------------------\n");

    heroes_hesapla(&kahraman_data,&data);

  	creatures_hesapla(&canavar_data,&data);

	research_hesapla(&data);

	int gd = DETECT, gm;

	initgraph(&gd, &gm, "");

    tur_sistemi(&data);

    getch();

    closegraph();

    return 0;
}
