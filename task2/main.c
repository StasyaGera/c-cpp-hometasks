#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>

typedef struct
{
    int id;
    char name[128];
    char number[128];
} Person;

typedef struct
{
    int size;
    Person *persons;
} Collection;

FILE *source;
Collection book;
int curID;
const char *fileName;

bool checkInput(char *input)
{
    if (!strcmp(input, ""))
    {
        printf("Error: missing input.\n");
        return false;
    }

    int i = 0;
    if (isalpha(input[0]))
    {
        while (input[i] != '\0')
        {
            if (!isalpha(input[i++]))
            {
                printf("Error: only A-Z and a-z symbols are allowed in contact's name.\n");
                return false;
            }
        }
    }
    else
    {
        while (input[i] != '\0')
        {
            if (isdigit(input[i++]))
            {
                return true;
            }
        }
        printf("Error: contact's number must contain at least one digit.\n");
        return false;
    }
    return true;
}

int findByID(int id)
{
    int i;
    for (i = 0; i < book.size; i++)
    {
        if (book.persons[i].id == id)
        {
            return i;
        }
    }
    return -1;
}

char *makeNeutral(char *input)
{
    int i = 0;
    bool isName = (isalpha(input[0]) ? true : false);
    char *answer = malloc(100*sizeof(char));

    i = 0;
    int j = 0;
    if (!isName)
    {
        while(input[i] != '\0')
        {
            if (isdigit(input[i]))
            {
                answer[j++] = input[i];
            }
            i++;
        }
        answer[j] = '\0';
    }
    else
    {
        while(input[i] != '\0')
        {
            answer[i] = tolower(input[i++]);
        }
        answer[i] = '\0';
    }
    return answer;
}

void fileUpdate()
{
    freopen(fileName, "w", source);
    int i;
    for (i = 0; i < book.size; i++)
    {
        if (book.persons[i].id)
        {
            fprintf(source, "%d %s %s\n", book.persons[i].id, book.persons[i].name, book.persons[i].number);
        }
    }
    freopen(fileName, "a+", source);
    return;
}

void find(char *input)
{
    int i;
    bool found = false;
    input = makeNeutral(input);
    if (isdigit(input[0]))
    {
        for (i = 0; i < book.size; i++)
        {
            if ((book.persons[i].id) && (!strcmp(makeNeutral(book.persons[i].number), input)))
            {
                printf("%d %s %s\n", book.persons[i].id, book.persons[i].name, book.persons[i].number);
                found = true;
            }
        }
    }
    else
    {
        for (i = 0; i < book.size; i++)
        {
            if ((book.persons[i].id) && (strstr(makeNeutral(book.persons[i].name), input)))
            {
                printf("%d %s %s\n", book.persons[i].id, book.persons[i].name, book.persons[i].number);
                found = true;
            }
        }
    }
    if (!found)
    {
        printf("Error: no such contact.\n");
    }
    return;
}

void add(char *name, char *number)
{
    book.size++;
    book.persons = realloc(book.persons, book.size*sizeof(Person));
    book.persons[book.size - 1].id = curID;
    strcpy(book.persons[book.size - 1].name, name);
    strcpy(book.persons[book.size - 1].number, number);
    fprintf(source, "%d %s %s\n", curID++, name, number);
    return;
}

void delete(int id)
{
    int i = findByID(id);
    if (i >= 0)
    {
        book.persons[i].id = 0;
        fileUpdate();
    }
    return;
}

void change(int id, char *command, char *newValue)
{
    int i = findByID(id);
    if (i >= 0)
    {
        if (!strcmp(command, "number"))
        {
            strcpy(book.persons[i].number, newValue);
        }
        else if (!strcmp(command, "name"))
        {
            strcpy(book.persons[i].name, newValue);
        }
        else
        {
            printf ("Error: don't understand command \"%s\".\n", command);
            return;
        }
        fileUpdate();
    }
    return;
}

int main(int argc, const char *argv[])
{
    fileName = argv[1];
    source = fopen(fileName, "a+");
    if (source == NULL)
    {
        perror("Error");
    }

    book.size = 0;
    book.persons = malloc(book.size*sizeof(Person));

    int id;
    char *name = malloc(128*sizeof(char)), *number = malloc(128*sizeof(char));

    rewind(source);
    while (fscanf(source, "%d%s%s", &id, name, number) == 3)
    {
        book.size++;
        book.persons = realloc(book.persons, book.size*sizeof(Person));
        book.persons[book.size - 1].id = id;
        strcpy(book.persons[book.size - 1].name, name);
        strcpy(book.persons[book.size - 1].number, number);
    }
    curID = (book.size ? (book.persons[book.size - 1].id + 1) : 1);

    char *command = malloc(128*sizeof(char)), *input = malloc(128*sizeof(char));
    while (1)
    {
        scanf("%s", command);
        if (!strcmp(command, "find"))
        {
            scanf("%s", input);
            if (checkInput(input))
            {
                find(input);
            }
        }
        else if (!strcmp(command, "create"))
        {
            scanf("%s%s", name, number);
            if (checkInput(name) && checkInput(number))
            {
                add(name, number);
            }
        }
        else if (!strcmp(command, "delete"))
        {
            scanf("%d", &id);
            delete(id);
        }
        else if (!strcmp(command, "change"))
        {
            scanf("%d%s%s", &id, command, input);
            if (checkInput(input))
            {
                change(id, command, input);
            }
        }
        else if (!strcmp(command, "exit"))
        {
            fclose(source);

            free(name);
            free(number);
            free(command);
            free(input);
            free(book.persons);

            return 0;
        }
        else
        {
            printf ("Error: don't understand command \"%s\".\n", command);
        }
        fflush(source);
    }
}