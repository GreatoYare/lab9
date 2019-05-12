#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

enum MenuItem
{
    ADD,
    SEARCHNUMBER,
    SEARCHAPARTMENT,
    SEARCHSALARY,
    EXIT
};

enum EnumCit
{
    NUMBER,
    STREET,
    GENDER,
    APARTMENT,
    NAME,
    SALARY
};

struct citizen
{
    int number;
    char *street;
    char *gender;
    int apartment;
    char *name;
    double salary;
} address;

FILE *inputFile;

/*For error response with Message*/
int YareErrorResponse(const char *error, int errorcode)
{
    printf("[ERROR] %s", error);
    return errorcode;
}

/*Initialization Array Of Citizens*/
struct citizen *YareInitArrayOfCits()
{
    return (struct citizen *) malloc(sizeof(struct citizen));
}

/*Get String Length*/
int YareGetStringSize(char *string)
{
    int i = 0;
    while (string[i] != '\0')
    {
        i++;
    }
    return i + 1;
}

/*Initialization String Field*/
char *YareInitStringField(char *string)
{
    int len = YareGetStringSize(string);
    char *str = (char *) malloc(len * sizeof(char));

    strcpy(str, string);
    return str;
}

/*Resize Array Of Citizens*/
struct citizen *YareResizeArrayOfCits(struct citizen *cit, int size)
{
    return (struct citizen *) realloc(cit, (size + 1) * sizeof(struct citizen));
}

/*Increment Enum Of Cit*/
void YareIncEnumOfCit(enum EnumCit *enumCit)
{
    *enumCit = (enum EnumCit) (*enumCit + 1);
    if ((*enumCit) == SALARY + 1)
    {
        (*enumCit) = NUMBER;
    }
}

/*Read Field*/
void YareReadFieldFromString(struct citizen *cit, enum EnumCit enumCit, char *string)
{
    switch (enumCit)
    {
    case NUMBER:
    {
        (*cit).number = atoi(string);
        break;
    }
    case STREET:
    {
        (*cit).street = YareInitStringField(string);
        break;
    }
    case GENDER:
    {
        (*cit).gender = YareInitStringField(string);
        break;
    }
    case APARTMENT:
    {
        (*cit).apartment = atoi(string);
        break;
    }
    case NAME:
    {
        (*cit).name = YareInitStringField(string);
        break;
    }
    case SALARY:
    {
        (*cit).salary = atof(string);
        break;
    }
    }
}

/*Read from CVS format file*/
struct citizen *YareReadFromFile(FILE *file, int *size)
{
    struct citizen *cit = YareInitArrayOfCits();
    enum EnumCit enumCit = NUMBER;

    if (file != NULL)
    {
        char scanner[1024];
        while (fgets(scanner, 1024, file))
        {
            char *words = strtok(scanner, ";,");
            while (words != NULL)
            {
                YareReadFieldFromString(&(cit[(*size)]), enumCit, words);
                YareIncEnumOfCit(&enumCit);
                words = strtok(NULL, ";,");
            }
            ++(*size);
            cit = YareResizeArrayOfCits(cit, (*size));
        }
        return cit;
    }
    else
    {
        return NULL;
    }
}

void YareFreeMemory(const struct citizen *arrayOfCits, int size);

/*For New Cit Add*/
void YareReadNewCit(struct citizen *arrayOfCits, int size)
{
    char string[1024];
    printf("Write number of house: ");
    scanf("%d", &arrayOfCits[size].number);
    printf("Write street: ");
    scanf("%s", &string);
    arrayOfCits[size].street = YareInitStringField(string);
    printf("Write citizen gender: ");
    scanf("%s", &string);
    arrayOfCits[size].gender = YareInitStringField(string);
    printf("Write number of apartment: ");
    scanf("%d", &arrayOfCits[size].apartment);
    printf("Write name of citizen: ");
    scanf("%s", &string);
    arrayOfCits[size].name = YareInitStringField(string);
    printf("Write citizen salary: ");
    scanf("%lf", &arrayOfCits[size].salary);
    fprintf(inputFile, "%d;%s;%s;%d;%s;%lf", arrayOfCits[size].number, arrayOfCits[size].street, arrayOfCits[size].gender, arrayOfCits[size].apartment, arrayOfCits[size].name, arrayOfCits[size].salary);
}


/*Sort salary*/
int compareS(const struct citizen *a, const struct citizen *b)
{
    return ((*b).salary - (*a).salary);
}

int compareN(const struct citizen *a, const struct citizen *b)
{
    return ((*b).number - (*a).number);
}

int compareA(const struct citizen *a, const struct citizen *b)
{
    return ((*b).apartment - (*a).apartment);
}

/*Print list*/
void YarePrintArrayOfCit(const struct citizen *arrayOfCits, int size)
{
    printf("NUMBER\tSTREET\t\t\tGENDER\tAPARTMENT\tNAME\t\t\tSALARY\n");
    for (int i = 0; i < size; ++i)
    {
        printf("%d\t%-20s\t%s\t%d\t%-31s\t%lf\t\n", arrayOfCits[i].number, arrayOfCits[i].street, arrayOfCits[i].gender, arrayOfCits[i].apartment, arrayOfCits[i].name, arrayOfCits[i].salary);
    }
}

/*Print menu*/
void YareShowMenuWithList(const struct citizen *arrayOfCits, int size)
{
    system("cls");
    printf("Your citizens:\n");
    YarePrintArrayOfCit(arrayOfCits, size);
    printf("\nMenu:\n");
    printf("1. Add new citizen\n");
    printf("2. Search by field and sort by NUMBER\n");
    printf("3. Search by field and sort by APARTMENT\n");
    printf("4. Search by filed and sort by SALARY\n");
    printf("5. Exit\n");
    printf("\nCommand: ");
}

/*For free Memory*/
void YareFreeMemory(const struct citizen *arrayOfCits, int size)
{
    for (int i = 0; i < size; ++i)
    {
        free(arrayOfCits[i].name);
        free((void *) &arrayOfCits[size]);
    }
    free((void *) arrayOfCits);
}

/*Check field*/
int YareCheckField(char *string)
{
    char N[] = "number";
    char A[] = "apartment";
    char S[] = "salary";
    if(strcmp(N, string) == 0)
        return 1;
    else if(strcmp(A, string) == 0)
        return 2;
    else if(strcmp(S, string) == 0)
        return 3;
    else
        return 0;
}

/*Sort by number*/
void YareSearchByNumber(struct citizen *cit, int size, char *MIN, char *MAX, char *string)
{
    printf("\nSEARCH:\n");
    int count = 0;
    int check;
    check = YareCheckField(string);
    struct citizen *cits;
    memcpy(&cits, &cit, sizeof(cit));
    qsort(cits, size, sizeof(struct citizen), (int (*)(const void *, const void *)) compareN);
    if (check == 1)
    {
        int max,min;
        max = atoi(MAX);
        min = atoi(MIN);
        for (int i = 0; i < size; ++i)
        {
            if(cit[i].number < max && cit[i].number > min)
            {
                YarePrintArrayOfCit(&(cit[i]), 1);
                count++;
            }
        }
    }
    else if (check == 2)
    {
        int max,min;
        max = atoi(MAX);
        min = atoi(MIN);
        for (int i = 0; i < size; ++i)
        {
            if(cit[i].apartment < max && cit[i].apartment > min)
            {
                YarePrintArrayOfCit(&(cit[i]), 1);
                count++;
            }
        }
    }
    else if (check == 3)
    {
        double max,min;
        max = atof(MAX);
        min = atof(MIN);
        for (int i = 0; i < size; ++i)
        {
            if(cit[i].salary < max && cit[i].salary > min)
            {
                YarePrintArrayOfCit(&(cit[i]), 1);
                count++;
            }
        }
    }
    else if (check == 0)
        printf("You made a mistake in a name of field");
    if(count == 0)
        printf("Not Found\n");
    printf("Press 0 to continue ");
    scanf("%d", &count);
}

/*Sort by apartment*/
void YareSearchByApartment(struct citizen *cit, int size, char *MIN, char *MAX, char *string)
{
    printf("\nSEARCH:\n");
    int count = 0;
    int check;
    check = YareCheckField(string);
    struct citizen *cits;
    memcpy(&cits, &cit, sizeof(cit));
    qsort(cits, size, sizeof(struct citizen), (int (*)(const void *, const void *)) compareA);
    if (check == 1)
    {
        int max,min;
        max = atoi(MAX);
        min = atoi(MIN);
        for (int i = 0; i < size; ++i)
        {
            if(cit[i].number < max && cit[i].number > min)
            {
                YarePrintArrayOfCit(&(cit[i]), 1);
                count++;
            }
        }
    }
    else if (check == 2)
    {
        int max,min;
        max = atoi(MAX);
        min = atoi(MIN);
        for (int i = 0; i < size; ++i)
        {
            if(cit[i].apartment < max && cit[i].apartment > min)
            {
                YarePrintArrayOfCit(&(cit[i]), 1);
                count++;
            }
        }
    }
    else if (check == 3)
    {
        double max,min;
        max = atof(MAX);
        min = atof(MIN);
        for (int i = 0; i < size; ++i)
        {
            if(cit[i].salary < max && cit[i].salary > min)
            {
                YarePrintArrayOfCit(&(cit[i]), 1);
                count++;
            }
        }
    }
    else if (check == 0)
        printf("You made a mistake in a name of field");
    if(count == 0)
        printf("Not Found\n");
    printf("Press 0 to continue ");
    scanf("%d", &count);
}

/*Sort by salary*/
void YareSearchBySalary(struct citizen *cit, int size, char *MIN, char *MAX, char *string)
{
    printf("\nSEARCH:\n");
    int count = 0;
    int check;
    check = YareCheckField(string);
    struct citizen *cits;
    memcpy(&cits, &cit, sizeof(cit));
    qsort(cits, size, sizeof(struct citizen), (int (*)(const void *, const void *)) compareS);
    if (check == 1)
    {
        int max,min;
        max = atoi(MAX);
        min = atoi(MIN);
        for (int i = 0; i < size; ++i)
        {
            if(cit[i].number < max && cit[i].number > min)
            {
                YarePrintArrayOfCit(&(cit[i]), 1);
                count++;
            }
        }
    }
    else if (check == 2)
    {
        int max,min;
        max = atoi(MAX);
        min = atoi(MIN);
        for (int i = 0; i < size; ++i)
        {
            if(cit[i].apartment < max && cit[i].apartment > min)
            {
                YarePrintArrayOfCit(&(cit[i]), 1);
                count++;
            }
        }
    }
    else if (check == 3)
    {
        double max,min;
        max = atof(MAX);
        min = atof(MIN);
        for (int i = 0; i < size; ++i)
        {
            if(cit[i].salary < max && cit[i].salary > min)
            {
                YarePrintArrayOfCit(&(cit[i]), 1);
                count++;
            }
        }
    }
    else if (check == 0)
        printf("You made a mistake in a name of field");
    if(count == 0)
        printf("Not Found\n");
    printf("Press 0 to continue ");
    scanf("%d", &count);
}

int main()
{
    inputFile = fopen("inputt.csv", "r+");
    struct citizen *arrayOfCits;
    int size = 0;

    if ((arrayOfCits = YareReadFromFile(inputFile, &size)) == NULL)
        return YareErrorResponse("Memory Error", 501);

    enum MenuItem menu = EXIT;
    do
    {
        YareShowMenuWithList(arrayOfCits, size);
        scanf("%d", &menu);

        switch (menu)
        {
        case ADD + 1:
        {
            YareReadNewCit(arrayOfCits, size);
            arrayOfCits = YareResizeArrayOfCits(arrayOfCits, ++size);
            break;
        }
        case SEARCHNUMBER + 1:
        {
            printf("Choose field:");
            char string[80];
            scanf("%s", &string);
            int ix = 0;
            char character;
            while(string[ix])
            {
                character = string[ix];
                string[ix] = (char)tolower(character);
                ix++;
            }
            char MAX[80];
            char MIN[80];
            printf("Enter MAX point of field:");
            scanf("%s", &MAX);
            printf("Enter MIN point of field:");
            scanf("%s", &MIN);
            YareSearchByNumber(arrayOfCits, size, MIN, MAX, string);
            break;
        }
        case SEARCHAPARTMENT + 1:
        {
            printf("Choose field:");
            char string[80];
            scanf("%s", &string);
            int ix = 0;
            char character;
            while(string[ix])
            {
                character = string[ix];
                string[ix] = (char)tolower(character);
                ix++;
            }
            char MAX[80];
            char MIN[80];
            printf("Enter MAX point of field:");
            scanf("%s", &MAX);
            printf("Enter MIN point of field:");
            scanf("%s", &MIN);
            YareSearchByApartment(arrayOfCits, size, MIN, MAX, string);
            break;
        }
        case SEARCHSALARY + 1:
        {
            printf("Choose field:");
            char string[80];
            scanf("%s", &string);
            int ix = 0;
            char character;
            while(string[ix])
            {
                character = string[ix];
                string[ix] = (char)tolower(character);
                ix++;
            }
            char MAX[80];
            char MIN[80];
            printf("Enter MAX point of field:");
            scanf("%s", &MAX);
            printf("Enter MIN point of field:");
            scanf("%s", &MIN);
            YareSearchBySalary(arrayOfCits, size, MIN, MAX,string);
            break;
        }
        }
    }
    while (menu != EXIT + 1);

    YareFreeMemory(arrayOfCits, size);
    fclose(inputFile);
    return 0;
}
