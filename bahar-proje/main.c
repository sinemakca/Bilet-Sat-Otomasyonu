#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
typedef enum
{
    menuGuncelKoltuk,
    biletSatisGuncelKoltuk
} guncelKoltuk;
typedef enum
{
    ogrenci = 1,
    tam
} biletTipi;
 
typedef enum
{
    bos,
    dolu
} koltukDurumu;

typedef struct
{
    int koltuklar[50];
    int doluKoltuk;
} matineler;

typedef struct
{
    matineler matine[5];
} salonlar;

salonlar red, green, blue;

FILE *dosyaAcma(char[], char[]);
void kullaniciGiris();
void kullaniciKontrol();
void sifreleme(char *);
void sifreCozme(char *);
void anaMenu();
void biletSatisi();
void salonIslemleri(char[], salonlar *, int);
void logDosyasiOlusturmaOkuma(char[], salonlar *);
int koltukKontrol(matineler);
void hasilatIslem(int);
int hasilatDosyaAcma(int *);
void hasilatDosyaYazdirma();
void hasilatDosyaOkuma();
int koltukKayit(char[], salonlar *, int);
void koltukLogGuncelleme(int);
void koltukLOG(char[], salonlar *, FILE *, int);

FILE *dosyaAcma(char dosyaAD[], char DosyaMOD[])
{
    FILE *dosya;
    dosya = fopen(dosyaAD, DosyaMOD);

    return dosya;
}

int main()
{
    kullaniciGiris();
    anaMenu();

    return 0;
}
void kullaniciGiris()
{
    char kullaniciAD[20];
    char kullaniciSIFRE[20];

    printf("kullanici adi giriniz ");
    scanf("%s", kullaniciAD);
    printf("sifre giriniz ");
    scanf("%s", kullaniciSIFRE);

    kullaniciKontrol(kullaniciAD, kullaniciSIFRE);
}
void kullaniciKontrol(char kullaniciAD[], char kullaniciSIFRE[])
{
    char ad[20], sifre[20];

    FILE *dosyaSifre;
    dosyaSifre = dosyaAcma("sifre.txt", "r");

    if (dosyaSifre == NULL)
    {
        dosyaSifre = dosyaAcma("sifre.txt", "w+");
        sifreleme(kullaniciAD);
        sifreleme(kullaniciSIFRE);

        fprintf(dosyaSifre, "kullanici adi: %s\nsifre: %s", kullaniciAD, kullaniciSIFRE);
    }
    else
    {
        fscanf(dosyaSifre, "kullanici adi: %s\nsifre: %s", ad, sifre);
        sifreCozme(ad);
        sifreCozme(sifre);

        if (strcmp(ad, kullaniciAD) != 0 || strcmp(sifre, kullaniciSIFRE) != 0)
        {
            printf("\ngiris basarisiz\n\n");
            kullaniciGiris();
        }
    }

    fclose(dosyaSifre);
}
void sifreleme(char *sifrele)
{
    for (int i = 0; sifrele[i] != '\0'; i++)
    {
        sifrele[i] = sifrele[i] - 1;
    }
}
void sifreCozme(char *sifrele)
{
    for (int i = 0; sifrele[i] != '\0'; i++)
    {
        sifrele[i] = sifrele[i] + 1;
    }
}

void anaMenu()
{
    int menuSecim;
    do
    {
        printf("\n1.Bilet alım\n2.Hasilat bilgisi\n3.koltuk bilgisi\n4.cikis\n");
        scanf("%d", &menuSecim);
        switch (menuSecim)
        {
        case 1:
            biletSatisi();
            break;
        case 2:
            hasilatDosyaOkuma();
            break;
        case 3:
            logDosyasiOlusturmaOkuma("red.txt", &red);
            logDosyasiOlusturmaOkuma("green.txt", &green);
            logDosyasiOlusturmaOkuma("blue.txt", &blue);
            koltukLogGuncelleme(menuGuncelKoltuk);
            break;
        case 4:
            exit(0);
            break;
        default:
            printf("hatali secim\n");
            break;
        }
    } while (1);
}

void biletSatisi()
{
    int salonSecim;

    printf("\nRed salonununda-->fizik vize sınavı[korku,gerilim]\nGreen salonunda--> Gora\nBlue salonununda-->The Matrix\n");
    printf("\n1.Red\n2.Green\n3.Blue\nSalon seciniz ");
    scanf("%d", &salonSecim);
    switch (salonSecim)
    {
    case 1:
        salonIslemleri("red.txt", &red, salonSecim);
        break;
    case 2:
        salonIslemleri("green.txt", &green, salonSecim);
        break;
    case 3:
        salonIslemleri("blue.txt", &blue, salonSecim);
        break;
    default:
        printf("yanlis secim\n\n");
        break;
    }
}
void salonIslemleri(char DosyaAD[], salonlar *salon, int salonSecim)
{
    int matineSecim, biletTipSecim, koltukNO;

    printf("matine seciniz[1-5] ");
    scanf("%d", &matineSecim);

    logDosyasiOlusturmaOkuma(DosyaAD, salon);

    int doluKoltuk = koltukKontrol(salon->matine[matineSecim - 1]);

    if (doluKoltuk >= 50)
    {
        printf("%d Salonun %d matinesinde bos koltuk kalmamistir\n\n", salonSecim, matineSecim + 1);
        biletSatisi();
    }

    printf("\nbilet tipi seciniz\n1.ogrenci\n2.tam\n");
    scanf("%d", &biletTipSecim);
    hasilatIslem(biletTipSecim);

    koltukNO = koltukKayit(DosyaAD, salon, (matineSecim - 1));
    printf("koltuk no: %d\n", koltukNO);
    printf("Kayit tamamlandi\n");

    koltukLogGuncelleme(biletSatisGuncelKoltuk);
}

void logDosyasiOlusturmaOkuma(char dosyaAD[], salonlar *salon)
{
    FILE *logDosya;
    logDosya = dosyaAcma(dosyaAD, "r");
    if (logDosya == NULL)
    {
        logDosya = dosyaAcma(dosyaAD, "w+");
        for (int i = 0; i < 5; i++)
        {
            for (int j = 0; j < 50; j++)
            {
                salon->matine[i].koltuklar[j] = 0;
                fprintf(logDosya, "%d ", salon->matine[i].koltuklar[j]);
            }
            fprintf(logDosya, "\n");
        }
    }
    else
    {
        for (int i = 0; i < 5; i++)
        {
            for (int j = 0; j < 50; j++)
            {
                fscanf(logDosya, "%d", &salon->matine[i].koltuklar[j]);
            }
        }
    }
    fclose(logDosya);
}

int koltukKontrol(matineler matine)
{
    int *p, doluKoltukSayac = 0;
    p = matine.koltuklar;

    for (int i = 0; i < 50; i++)
    {
        if (*(p + i) == dolu)
        {
            doluKoltukSayac++;
        }
    }

    return doluKoltukSayac;
}

void hasilatIslem(int biletTip)
{
    int hasilat = hasilatDosyaAcma(&hasilat);

    if (biletTip == ogrenci)
    {
        hasilat = hasilat + 20;
    }
    else if (biletTip == tam)
    {
        hasilat = hasilat + 30;
    }
    hasilatDosyaYazdirma(hasilat);
}

int hasilatDosyaAcma(int *hasilat)
{
    FILE *dosya;
    dosya = dosyaAcma("hasilatLOG.txt", "r");
    if (dosya == NULL)
    {
        
        dosya = dosyaAcma("hasilatLOG.txt", "w+");
        *hasilat = 0;
        fprintf(dosya, "hasilat: %d", *hasilat);
    }
    else
    {
        fscanf(dosya, "hasilat: %d", hasilat);
    }
    fclose(dosya);

    return *hasilat;
}

void hasilatDosyaYazdirma(int hasilat)
{
    FILE *dosya;
    dosya = dosyaAcma("hasilatLOG.txt", "w");
    fprintf(dosya, "hasilat: %d", hasilat);
    fclose(dosya);
}
void hasilatDosyaOkuma()
{
    int DosyaHasilat;

    FILE *dosya;
    dosya = dosyaAcma("hasilatLOG.txt", "r");
    fscanf(dosya, "hasilat: %d", &DosyaHasilat);
    fclose(dosya);
    printf("toplam hasilat: %d\n\n", DosyaHasilat);
}

int koltukKayit(char dosyaAD[], salonlar *salon, int matineIndex)
{
    int randomSayi = 0;
    FILE *dosya;
    dosya = dosyaAcma(dosyaAD, "w+");
    srand(time(0));
    while (1)
    {
        randomSayi = rand() % 49;

        if (salon->matine[matineIndex].koltuklar[randomSayi] == bos)
        {
            salon->matine[matineIndex].koltuklar[randomSayi] = dolu;
            break;
        }
    }

    for (int i = 0; i < 5; i++)
    {

        for (int j = 0; j < 50; j++)
        {
            fprintf(dosya, "%d ", salon->matine[i].koltuklar[j]);
        }
        fprintf(dosya, "\n");
    }

    fclose(dosya);
    return randomSayi;
}

void koltukLogGuncelleme(int guncelKoltuk)
{
    FILE *dosya;
    dosya = dosyaAcma("koltukLOG.txt", "w+");
    koltukLOG("red", &red, dosya, guncelKoltuk);
    koltukLOG("green", &green, dosya, guncelKoltuk);
    koltukLOG("blue", &blue, dosya, guncelKoltuk);
    fclose(dosya);
}
void koltukLOG(char salonAD[], salonlar *salon, FILE *dosya, int guncelKoltuk)
{

    fprintf(dosya, "salon %s\n", salonAD);
    if (guncelKoltuk == menuGuncelKoltuk)
        printf("salon %s\n\n", salonAD);

    for (int i = 0; i < 5; i++)
    {
        salon->matine[i].doluKoltuk = 0;
        for (int j = 0; j < 50; j++)
        {
            if (salon->matine[i].koltuklar[j] == dolu)
            {
                salon->matine[i].doluKoltuk += 1;
            }
        }

        fprintf(dosya, "%d.matine %d koltuk dolu\n", (i + 1), salon->matine[i].doluKoltuk);

        if (guncelKoltuk == menuGuncelKoltuk)
            printf("%d.matine %d koltuk dolu\n", (i + 1), salon->matine[i].doluKoltuk);
    }

    fprintf(dosya, "\n");
}
