/*
* lab11.cpp - главный файл проекта.
* Copyright (C) 2022 Смирновский М.Е. <mistertonyragrel@gmail.com>
* Данная программа решает задачу конвертирования данных в формат CSV.
* Входные данные в файле с именем, задаваемым пользователем
* Выходные данные в файле с именем, задаваемым пользователем
* Программа имеет меню пользователя, формируемое с помощью функции
* Дата последней редакции 04.01.2022
* Программа доступна по лицензии GNU */

#include <stdio.h> //Подключение заголовочного файла библиотеки ввода/вывода
#include <stdio.h> //Подключение заголовочного файла библиотеки файлового ввода/вывода
#include <locale.h> //Подключение заголовочного файла библиотеки для работы с локалями
#include <windows.h> //Подключение заголовочного файла системной библиотеки

//Размеры полей
#define LASTNAME_SIZE 25
#define FIRSTNAME_SIZE 20
#define SURNAME_SIZE 30
#define DATE_SIZE 11
#define PROFESSION_SIZE 20
//Сообщения для меню
#define ITEM_NO_EXIST "Выбранный пункт меню не существует!\n"
#define MENU_TITLE "\n---Меню программы---\n"
#define ITEM_OUT "[%d] - %s\n"
#define SELECT_ITEM "Выберите пункт меню (от 0 до %d): "
#define BUFFER_SIZE 7
#define MAX_MENU_ITEM_SIZE 30
#define MAX_ITEM_ARRAY_SIZE 50
//Сообщения
#define EXIT_MSG "Работа программы завершена!\n"
#define MENU_ERR_MSG "Пункта меню с таким номером не существует!\n"
#define ABOUT_PROGRAM "Данная программа решает задачу конвертирования данных в формат CSV.\n"
#define AUTHOR "Copyright (C) 2022 Смирновский М.Е. <mistertonyragrel@gmail.com>.\n"
#define OPEN_FILE_ERR "\nНевозможно открыть файл %s!\n"
#define CREATE_FILE_ERR "\nНевозможно создать файл %s!\n"
//----- Структура для хранения набора полей -----

typedef struct{
     char lastname[LASTNAME_SIZE]; //Фамилия
     char firstname[FIRSTNAME_SIZE]; //Имя
     char surname[SURNAME_SIZE]; //Отчетство
     char dateOfBirth[DATE_SIZE]; //Дата рождения
     char gender; //Пол
     short childrens; //Количество детей
     char profession[PROFESSION_SIZE];//Профессия
     float experience; //Стаж работы
}Persons;

//----- Структура для хранения меню пользователя -----

typedef struct{
    char menuItems[MAX_ITEM_ARRAY_SIZE][MAX_MENU_ITEM_SIZE];
    int menuCount;
    const char *noExist;
    const char *title;
    const char *output;
    const char *select;
}Tmenu;

//----- Прототипы используемых функций -----

int readPerson(FILE *,Persons *);
int printPerson(FILE *,Persons);
int makeMenu(Tmenu *);
void menuInit(Tmenu *, int, char (*)[]);
void closeProgram();
void convert(char *, char *);
void showInputFormat();
void showOutputFormat();
void showFileContents();

//----- Начало программы ----

int main(){
    setlocale( LC_ALL,"Russian" ); //Установление кодировки

    int choose=0; //Переменная для хранения выбора пользователя
    FILE *inFile; //Дескриптор входного файла
    FILE *outFile; //Дескриптор выходного файла

    char inputfilename[128]="input.txt"; //Определим имя входного файла по умолчанию
    char outputfilename[128]="output.csv"; //Определим имя выходного файла по умолчанию
    Tmenu menu;
    //Массив с пунктами меню
    char buffer[BUFFER_SIZE][MAX_MENU_ITEM_SIZE] = {{"Выход"},
                                                    {"О программе"},
                                                    {"Об авторе"},
                                                    {"Конвертировать"},
                                                    {"Формат входных данных"},
                                                    {"Формат выходных данных"},
                                                    {"Просмотреть файл"}};

    menuInit(&menu, BUFFER_SIZE, buffer); // Инициализируем поля структуры Tmenu

    do{
        switch(choose = makeMenu(&menu)){
            case 0:
                printf(EXIT_MSG);
                break;
            case 1:
                printf(ABOUT_PROGRAM);
                break;
            case 2:
                printf(AUTHOR);
                break;
            case 3:
                convert(inputfilename, outputfilename); // Запускаем конвертацию
                break;
            case 4:
                showInputFormat();
                break;
            case 5:
                showOutputFormat();
                break;
            case 6:
                showFileContents();
                break;
            default:
                printf(MENU_ERR_MSG);
        }
    }while(choose!=0);

    return EXIT_SUCCESS;
}

/*
    Функция читает данные из файла и записывает в поля структуры persons
*/
int readPerson(FILE *filePtr, Persons *person){
    return fscanf(filePtr,"%s\n%s\n%s\n%s\n%c%d%s\n%f",person->lastname, person->firstname,
                  person->surname, person->dateOfBirth, &person->gender, &person->childrens,
                  person->profession, &person->experience);
}

/*
    Функция записывает преобразованные данные в файл
*/
int printPerson(FILE *filePtr, Persons person){
    return fprintf(filePtr,"\"%s\";\"%s\";\"%s\";\"%s\";\"%c\";\"%d\";\"%s\";\"%f\"\n",
                   person.lastname,person.firstname,person.surname,person.dateOfBirth,
                   person.gender,person.childrens,person.profession,person.experience);
}

/*
    Функция выводит меню в консоль
*/
int makeMenu(Tmenu *menu){
    printf(menu->title);
    for(int i = 0; i < menu->menuCount; i++)
        printf(menu->output, i, menu->menuItems[i]);

    int choose = 0;
    printf(menu->select, menu->menuCount-1);

    while(scanf("%d", &choose)==1 && (choose>= menu->menuCount || choose<0)){
        printf(menu->noExist);
        printf(menu->select, menu->menuCount-1);
    }

    return choose;
}

/*
    Функция инициализирует все поля структуры Tmenu
*/
void menuInit(Tmenu *menu, int menuCount, char buffer[BUFFER_SIZE][MAX_MENU_ITEM_SIZE]){
    menu->noExist = ITEM_NO_EXIST;
    menu->title = MENU_TITLE;
    menu->output = ITEM_OUT;
    menu->select = SELECT_ITEM;

    for(int i = 0; i<menuCount; i++) // Инициализируем массив с пунктами меню
        strcpy(menu->menuItems[i], buffer[i]);

    menu->menuCount = menuCount; // Инициализируем переменную, хранящую количество пунктов меню
}
/*
    Функция конвертирует данные из входного файла
    в формат CSV и записывает результат в выходной файл
*/
void convert(char *input, char *output){
    FILE *fr, *fw; //Дескрипторы входного и выходного файлов
    printf("\n---Старт работы конвертера---");
    printf("\nВведите имя входного файла: ");
    scanf("%s", input);  // Пользователь вводит имя входного файла
    getchar();

    printf("Введите имя выходного файла: ");
    scanf("%s", output); // Пользователь вводит имя выходного файла
    getchar();

    Persons person; //Структура данных

    fr = fopen(input, "r"); //Открываем файл в режиме чтения
    fw = fopen(output, "w"); // Открываем файл в режиме записи

    if(fr == NULL){
        printf(OPEN_FILE_ERR, input);
        fclose(fr);//Закрываем входной файл
        fclose(fw);//Закрываем выходной файл
        return;
    }
    if(fw == NULL){
        printf(CREATE_FILE_ERR, output);
        fclose(fr);//Закрываем входной файл
        fclose(fw);//Закрываем выходной файл
        return;
    }
    while(readPerson(fr, &person) != EOF){
        printf("\nОбработка записи пользователя: %s...",person.lastname);
        printPerson(fw,person);
    }
    rewind(fr);
    fclose(fr);//Закрываем входной файл
    fclose(fw);//Закрываем выходной файл
    printf("\n---Работа конвертера завершена---\n");

}

//----- Функция выводит формат входных данных -----

void showInputFormat(){
    char array[8][70] = {{"<Фамилия> - строка, 25 символов\n"},
                         {"<Имя> - строка, 20 символов\n"},
                         {"<Отчество> - строка, 30 символов\n"},
                         {"<Дата рождения> - строка ДД.ММ.ГГГГ, 11 символов\n"},
                         {"<Пол> - 1 символ\n"},
                         {"<Количество детей> - число short\n"},
                         {"<Профессия> - строка, 20 символов\n"},
                         {"<Стаж работы> - число float\n"}};
    printf("\n--- Формат входных данных ---");
    for(int i = 0; i<8; i++) printf("\n%s",array[i]);
    putchar('\n');
}
//----- Функция выводит формат выходных данных -----

void showOutputFormat(){
    const char *str = "<Фамилия>;<Имя>;<Отчество>;"
                      "<Дата рождения>;<Пол>;"
                      "<Количество детей>;"
                      "<Профессия>;<Стаж работы>";
    printf("\n--- Формат выходных данных ---");
    printf("\n%s\n", str);
}

//----- Функция выводит содержимое выбранного файла -----
void showFileContents(){
    char fileName[30];
    printf("\n--- Просмотр содержимого файла ---");
    printf("\nВведите имя файла: ");
    scanf("%s", fileName);
    getchar();

    FILE *fr = fopen(fileName, "r");
    if(fr==NULL){
        printf("\nНе удалось открыть файл %s\n", fileName);
        fclose(fr);
    }else{
        while(!feof(fr)){
            char str[300];
            fscanf(fr, "%s\n", str);
            printf("%s\n",str);
        }
        rewind(fr);
        fclose(fr);
    }
}


