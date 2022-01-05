/*
* lab11.cpp - ������� ���� �������.
* Copyright (C) 2022 ����������� �.�. <mistertonyragrel@gmail.com>
* ������ ��������� ������ ������ ��������������� ������ � ������ CSV.
* ������� ������ � ����� � ������, ���������� �������������
* �������� ������ � ����� � ������, ���������� �������������
* ��������� ����� ���� ������������, ����������� � ������� �������
* ���� ��������� �������� 04.01.2022
* ��������� �������� �� �������� GNU */

#include <stdio.h> //����������� ������������� ����� ���������� �����/������
#include <stdio.h> //����������� ������������� ����� ���������� ��������� �����/������
#include <locale.h> //����������� ������������� ����� ���������� ��� ������ � ��������
#include <windows.h> //����������� ������������� ����� ��������� ����������

//������� �����
#define LASTNAME_SIZE 25
#define FIRSTNAME_SIZE 20
#define SURNAME_SIZE 30
#define DATE_SIZE 11
#define PROFESSION_SIZE 20
//��������� ��� ����
#define ITEM_NO_EXIST "��������� ����� ���� �� ����������!\n"
#define MENU_TITLE "\n---���� ���������---\n"
#define ITEM_OUT "[%d] - %s\n"
#define SELECT_ITEM "�������� ����� ���� (�� 0 �� %d): "
#define BUFFER_SIZE 7
#define MAX_MENU_ITEM_SIZE 30
#define MAX_ITEM_ARRAY_SIZE 50
//���������
#define EXIT_MSG "������ ��������� ���������!\n"
#define MENU_ERR_MSG "������ ���� � ����� ������� �� ����������!\n"
#define ABOUT_PROGRAM "������ ��������� ������ ������ ��������������� ������ � ������ CSV.\n"
#define AUTHOR "Copyright (C) 2022 ����������� �.�. <mistertonyragrel@gmail.com>.\n"
#define OPEN_FILE_ERR "\n���������� ������� ���� %s!\n"
#define CREATE_FILE_ERR "\n���������� ������� ���� %s!\n"
//----- ��������� ��� �������� ������ ����� -----

typedef struct{
     char lastname[LASTNAME_SIZE]; //�������
     char firstname[FIRSTNAME_SIZE]; //���
     char surname[SURNAME_SIZE]; //���������
     char dateOfBirth[DATE_SIZE]; //���� ��������
     char gender; //���
     short childrens; //���������� �����
     char profession[PROFESSION_SIZE];//���������
     float experience; //���� ������
}Persons;

//----- ��������� ��� �������� ���� ������������ -----

typedef struct{
    char menuItems[MAX_ITEM_ARRAY_SIZE][MAX_MENU_ITEM_SIZE];
    int menuCount;
    const char *noExist;
    const char *title;
    const char *output;
    const char *select;
}Tmenu;

//----- ��������� ������������ ������� -----

int readPerson(FILE *,Persons *);
int printPerson(FILE *,Persons);
int makeMenu(Tmenu *);
void menuInit(Tmenu *, int, char (*)[]);
void closeProgram();
void convert(char *, char *);
void showInputFormat();
void showOutputFormat();
void showFileContents();

//----- ������ ��������� ----

int main(){
    setlocale( LC_ALL,"Russian" ); //������������ ���������

    int choose=0; //���������� ��� �������� ������ ������������
    FILE *inFile; //���������� �������� �����
    FILE *outFile; //���������� ��������� �����

    char inputfilename[128]="input.txt"; //��������� ��� �������� ����� �� ���������
    char outputfilename[128]="output.csv"; //��������� ��� ��������� ����� �� ���������
    Tmenu menu;
    //������ � �������� ����
    char buffer[BUFFER_SIZE][MAX_MENU_ITEM_SIZE] = {{"�����"},
                                                    {"� ���������"},
                                                    {"�� ������"},
                                                    {"��������������"},
                                                    {"������ ������� ������"},
                                                    {"������ �������� ������"},
                                                    {"����������� ����"}};

    menuInit(&menu, BUFFER_SIZE, buffer); // �������������� ���� ��������� Tmenu

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
                convert(inputfilename, outputfilename); // ��������� �����������
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
    ������� ������ ������ �� ����� � ���������� � ���� ��������� persons
*/
int readPerson(FILE *filePtr, Persons *person){
    return fscanf(filePtr,"%s\n%s\n%s\n%s\n%c%d%s\n%f",person->lastname, person->firstname,
                  person->surname, person->dateOfBirth, &person->gender, &person->childrens,
                  person->profession, &person->experience);
}

/*
    ������� ���������� ��������������� ������ � ����
*/
int printPerson(FILE *filePtr, Persons person){
    return fprintf(filePtr,"\"%s\";\"%s\";\"%s\";\"%s\";\"%c\";\"%d\";\"%s\";\"%f\"\n",
                   person.lastname,person.firstname,person.surname,person.dateOfBirth,
                   person.gender,person.childrens,person.profession,person.experience);
}

/*
    ������� ������� ���� � �������
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
    ������� �������������� ��� ���� ��������� Tmenu
*/
void menuInit(Tmenu *menu, int menuCount, char buffer[BUFFER_SIZE][MAX_MENU_ITEM_SIZE]){
    menu->noExist = ITEM_NO_EXIST;
    menu->title = MENU_TITLE;
    menu->output = ITEM_OUT;
    menu->select = SELECT_ITEM;

    for(int i = 0; i<menuCount; i++) // �������������� ������ � �������� ����
        strcpy(menu->menuItems[i], buffer[i]);

    menu->menuCount = menuCount; // �������������� ����������, �������� ���������� ������� ����
}
/*
    ������� ������������ ������ �� �������� �����
    � ������ CSV � ���������� ��������� � �������� ����
*/
void convert(char *input, char *output){
    FILE *fr, *fw; //����������� �������� � ��������� ������
    printf("\n---����� ������ ����������---");
    printf("\n������� ��� �������� �����: ");
    scanf("%s", input);  // ������������ ������ ��� �������� �����
    getchar();

    printf("������� ��� ��������� �����: ");
    scanf("%s", output); // ������������ ������ ��� ��������� �����
    getchar();

    Persons person; //��������� ������

    fr = fopen(input, "r"); //��������� ���� � ������ ������
    fw = fopen(output, "w"); // ��������� ���� � ������ ������

    if(fr == NULL){
        printf(OPEN_FILE_ERR, input);
        fclose(fr);//��������� ������� ����
        fclose(fw);//��������� �������� ����
        return;
    }
    if(fw == NULL){
        printf(CREATE_FILE_ERR, output);
        fclose(fr);//��������� ������� ����
        fclose(fw);//��������� �������� ����
        return;
    }
    while(readPerson(fr, &person) != EOF){
        printf("\n��������� ������ ������������: %s...",person.lastname);
        printPerson(fw,person);
    }
    rewind(fr);
    fclose(fr);//��������� ������� ����
    fclose(fw);//��������� �������� ����
    printf("\n---������ ���������� ���������---\n");

}

//----- ������� ������� ������ ������� ������ -----

void showInputFormat(){
    char array[8][70] = {{"<�������> - ������, 25 ��������\n"},
                         {"<���> - ������, 20 ��������\n"},
                         {"<��������> - ������, 30 ��������\n"},
                         {"<���� ��������> - ������ ��.��.����, 11 ��������\n"},
                         {"<���> - 1 ������\n"},
                         {"<���������� �����> - ����� short\n"},
                         {"<���������> - ������, 20 ��������\n"},
                         {"<���� ������> - ����� float\n"}};
    printf("\n--- ������ ������� ������ ---");
    for(int i = 0; i<8; i++) printf("\n%s",array[i]);
    putchar('\n');
}
//----- ������� ������� ������ �������� ������ -----

void showOutputFormat(){
    const char *str = "<�������>;<���>;<��������>;"
                      "<���� ��������>;<���>;"
                      "<���������� �����>;"
                      "<���������>;<���� ������>";
    printf("\n--- ������ �������� ������ ---");
    printf("\n%s\n", str);
}

//----- ������� ������� ���������� ���������� ����� -----
void showFileContents(){
    char fileName[30];
    printf("\n--- �������� ����������� ����� ---");
    printf("\n������� ��� �����: ");
    scanf("%s", fileName);
    getchar();

    FILE *fr = fopen(fileName, "r");
    if(fr==NULL){
        printf("\n�� ������� ������� ���� %s\n", fileName);
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


