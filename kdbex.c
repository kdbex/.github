#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>

typedef struct
{
    char argc;
    char *name;
    char *syntax;
    char *description;
    void (*function)(char **);
} action_t;

#define ACTIONS 5
#define KDBEX "kdbex"
#define SERVICE_NAME "KdBex"
#define FOLDER_NAME "/Kdbex/"

action_t actions[ACTIONS];

void help(char **argv)
{
    printf("Usage: %s <command> [args]\n", KDBEX);
    printf("Commands:\n");
    for (unsigned char i = 0; i < ACTIONS; i++)
    {
        action_t action = actions[i];
        printf("%s %s %s : %s\n", KDBEX, action.name, action.syntax, action.description);
    }
}

void status(char **argv)
{
    char command[100];
    strcpy(command, "nssm.exe status ");
    strcat(command, SERVICE_NAME);
    system(command);
}

FILE *open_config(char *mode)
{
    char *appdata = getenv("APPDATA");
    strcat(appdata, FOLDER_NAME);
    strcat(appdata, "config.json");
    return fopen(appdata, mode);
}

void view(char **argv)
{
    FILE *config = open_config("r");
    if (config == NULL)
    {
        printf("Error opening file. Maybe try to create the service first.\n");
        return;
    }
    char ch;
    do
    {
        ch = fgetc(config);
        printf("%c", ch);
    } while (ch != EOF);

    fclose(config);
}

void install(char **argv)
{
    char *path = argv[2];
    char *port = argv[3];
    char *cryptKey = argv[4];
    char config[1000];
    snprintf(config, 1000, "{\"filePath\":\"%s\",\"port\":%s,\"cryptKey\":\"%s\"}", path, port, cryptKey);
    FILE *file = open_config("w");
    fputs(config, file);
    char command[200];
    snprintf(command, 200, "nssm.exe install %s \"D:\\Workspace\\Kdbex\\server\\node.exe\" \"D:\\Workspace\\Kdbex\\server\\dist\\server\\app.js\"", SERVICE_NAME);
    system(command);
}

void uninstall(char **argv)
{
    char command[100];
    snprintf(command, 100, "nssm.exe remove %s confirm", SERVICE_NAME);
    system(command);
}

int main(int argc, char **argv)
{
    actions[0] = (action_t){0, "--help", "", "Shows the list of commands", &help};
    actions[1] = (action_t){0, "status", "", "Shows the status of the service", &status};
    actions[2] = (action_t){0, "config", "", "Shows the current config", &view};
    actions[3] = (action_t){3, "install", "<filePath> <port> <token>", "Installs the server with this config", &install};
    actions[4] = (action_t){0, "remove", "<filePath> <port> <token>", "Uninstalls", &uninstall};

    if (argc == 1)
    {
        printf("Use --help to get help \n");
    }
    for (unsigned char i = 0; i < ACTIONS; i++)
    {
        action_t action = actions[i];
        if (strcmp(argv[1], action.name) == 0)
        {
            if (argc - 2 == action.argc)
            {
                action.function(argv);
            }
            else
            {
                printf("Invalid number of arguments, correct syntax: %s %s %s\n", KDBEX, action.name, action.syntax);
            }
            return 0;
        }
    }
    printf("Unknown command. Use --help to get help \n");
    return 0;
}