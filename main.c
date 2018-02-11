//#include "stdafx.h"
#include <stdio.h>
#include <locale.h>
#include <conio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

struct afrik{
    char name[100];
    float e65, e66;
}a;

const char shapka1[] = "---------------------------------------------------------------------------------";
const char shapka[] = "-----------------------------------------------------------------";

void task(FILE *in, char name[], int eof){
    FILE *out;
    int i, j, k1=0, k2=0, eof2;
    struct afrik b;
    float min=-1, r1;
    char mname[100], r2[100];
    strcpy(mname, "");
    in = fopen(name, "rb+");
    out = fopen("fout", "wb+");
    if(in==NULL || out==NULL){
        printf("Не удалось открыть файл.\n");
        exit(-1);
    }
    rewind(in);
    i=0;
    printf("\nСтраны, чей экспорт алмазов в 1966-м превысил 1000 тыс.:\n%s\n|\tСтрана\t\t\t|\t"
                   "Объем экспорта в 1966-м\t|\n%s\n", shapka, shapka);
    while(i<=eof){
        fseek(in, i, SEEK_SET);
        fread(a.name, sizeof(a.name)+1, 1, in);
        fread(&a.e65, sizeof(a.e65), 1, in);
        if(a.e65<min || min==-1){
            min = a.e65;
            strcpy(mname, a.name);
        }
        fread(&a.e66, sizeof(a.e66), 1, in);
        if(a.e66>1000){
            printf("|\t%-20s\t|\t    %8.2f   \t\t|\n%s\n", a.name, a.e66, shapka);
            k1=1;
        }
        if(a.e66<2000){
            fwrite(a.name, sizeof(a.name)+1, 1, out);
            fwrite(&a.e66, sizeof(a.e66), 1, out);
            k2=1;
        }
        i += sizeof(a.name) + 1 + sizeof(a.e65) + sizeof(a.e66);
    }
    fclose(in);
    fclose(out);
    if(k1==0)
        printf("\t\tНет таких стран\t\t\t\n");
    if(k2!=0){
        out = fopen("fout", "rb+");
        if(out == NULL){
            printf("Не удалось открыть файл.\n");
            exit(-1);
        }
        rewind(out);
        fseek(out, 0, SEEK_END);
        eof2 = ftell(out);
        i=0;
        printf("\nСтраны, чей экспорт алмазов в 1966-м не превысил 2000 тыс.:\n%s\n|\tСтрана\t\t\t|\tОбъем экспорта в "
                       "1966-м\t|\n%s\n", shapka, shapka);
        while(i<=eof2){
            fseek(out, i, SEEK_SET);
            fread(a.name, sizeof(a.name)+1, 1, out);
            fread(&a.e66, sizeof(a.e66), 1, out);
            if((i + sizeof(a.name) + 1 + sizeof(a.e66))<eof2){
                j= i + sizeof(a.name) + 1 + sizeof(a.e66);
                fseek(out, j, SEEK_SET);
                fread(b.name, sizeof(a.name)+1, 1, out);
                fread(&b.e66, sizeof(a.e66), 1, out);
                while(j<eof2){
                    if(strcmp(a.name, b.name)>0){
                        strcpy(r2, a.name);
                        r1 = a.e66;
                        strcpy(a.name, b.name);
                        a.e66 = b.e66;
                        strcpy(b.name, r2);
                        b.e66 = r1;
                        fseek(out, j, SEEK_SET);
                        fwrite(b.name, sizeof(a.name)+1, 1, out);
                        fwrite(&b.e66, sizeof(a.e66), 1, out);
                    }
                    j += sizeof(a.name) + 1 + sizeof(a.e66);
                }
                printf("|\t%-20s\t|\t%8.2f\t\t|\n%s\n", a.name, a.e66, shapka);
            }
            i += sizeof(a.name) + 1 + sizeof(a.e66);
        }
        fclose(out);
    }
    else
        printf("\nНет стран, прирост которых в 1966-м году не превысил 2000 тыс. карат.\n");
    printf("\nСтрана с минимальным экспортом в 1965-м: %s (экспорт = %8.2f тыс. карат).\n", mname, min);
}

void display(FILE *in, char name[]){
    int i, eof;
    in = fopen(name, "rb+");
    if(in==NULL){
        printf("Не удалось открыть файл.\n");
        exit(-1);
    }
    rewind(in);
    printf("Ваша таблица:\n%s\n|\tСтрана\t\t\t|\t   1965   \t|\t   1966   \t|\n%s\n", shapka1, shapka1);
    fseek(in, 0, SEEK_END);
    eof=ftell(in);
    i=0;
    while(i<eof){
        fseek(in, i, SEEK_SET);
        fread(a.name, sizeof(a.name)+1, 1, in);
        fread(&a.e65, sizeof(a.e65), 1, in);
        fread(&a.e66, sizeof(a.e66), 1, in);
        printf("|\t%-20s\t|\t%8.2f\t|\t%8.2f\t|\n%s\n", a.name, a.e65, a.e66, shapka1);
        i+=sizeof(a.name)+1 + sizeof(a.e65) + sizeof(a.e66);
    }
    fclose(in);
    printf("\t========== ВЫПОЛНЕНИЕ ЗАДАНИЯ ==========\n");
    system("pause");
    task(in, name, eof);
}

void main() {
    //setlocale(LC_ALL, "rus");
    system("chcp 65001");
    int error;
    char c, filename[13];
    FILE *fin;
    do {
        printf("Введите имя файла: ");
        while (scanf("%s", filename) != 1) {
            printf("Произошла ошибка. Пожалуйста, повторите ввод: ");
            _flushall();
        }
        fflush(stdin);
        fin = fopen(filename, "wb+");
        if(fin==NULL)
            printf("Не удалось создать файл %s\n", filename);
    }
    while(fin==NULL);
    printf("Введите данные вашей таблицы:\n");
    do{
        do{
            error=0;
            printf("Название страны: ");
            if (scanf("%s", a.name)!=1){
                printf("Произошла ошибка.\n");
                error=1;
            }
            else
            if(strcmp(a.name, "")!=0)
                fwrite(a.name, sizeof(a.name)+1, 1, fin);
            else{
                printf("Вы ввели пустую строку.\n");
                error=1;
            }
            fflush(stdin);
        }while(error);
        do{
            error=0;
            printf("Экспорт за 1965-й год: ");
            if (scanf("%f", &a.e65) != 1) {
                printf("Произошла ошибка.\n");
                error=1;
            }
            else
            if(a.e65>=0)
                fwrite(&a.e65, sizeof(a.e65), 1, fin);
            else{
                printf("Экспорт не может быть отрицательным.\n");
                error=1;
            }
            fflush(stdin);
        }while(error);
        do{
            error=0;
            printf("Экспорт за 1966-й год: ");
            if (scanf("%f", &a.e66) != 1) {
                printf("Произошла ошибка.\n");
                error=1;
            }
            else
            if(a.e66>=0)
                fwrite(&a.e66, sizeof(a.e66), 1, fin);
            else{
                printf("Экспорт не может быть отрицательным.\n");
                error=1;
            }
            fflush(stdin);
        }while(error);
        printf("\tПродолжить ввод?\nДа - 'Y';\tНет - любой другой символ.\n");
        c = getch();
        c = toupper(c);
    }while(c=='Y');
    fclose(fin);
    display(fin, filename);
}