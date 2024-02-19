#include <sys/stat.h>
#include <sys/types.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>
#include <windows.h>
#include <unistd.h>
#include <shlwapi.h>

typedef struct
{
    char argc;
    char *name;
    char *syntax;
    char *description;
    void (*function)(char **);
} action_t;

#define ACTIONS 8
#define KDBEX "kdbex"
#define SERVICE_NAME "KdBex"
#define APPDATA "C:\\WINDOWS\\sysnative\\config\\systemprofile\\AppData\\Roaming\\Kdbex\\"

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

void check_folder()
{
    char folder[MAX_PATH];
    strcpy(folder, APPDATA);
    struct stat st = {0};
    if (stat(folder, &st) == -1)
    {
        mkdir(folder);
    }
}

FILE *open_config(char *mode)
{
    char folder[MAX_PATH];
    strcpy(folder, APPDATA);
    strcat(folder, "config.json");
    return fopen(folder, mode);
}

FILE *open_log()
{
    char folder[MAX_PATH];
    strcpy(folder, APPDATA);
    strcat(folder, "server.log");
    return fopen(folder, "r");
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
    fclose(file);
    char command[1000];
    char cd[MAX_PATH];
    GetModuleFileName(NULL, cd, MAX_PATH);
    PathRemoveFileSpec(cd);
    snprintf(command, 1000, "nssm.exe install %s \"%s\\server.bat\"", SERVICE_NAME, cd);
    system(command);
    snprintf(command, 1000, "nssm.exe start %s", SERVICE_NAME);
    system(command);
}

void keygen(char **argv)
{
    char key[100];
    for (int i = 0; i < 100; i++)
    {
        int r = rand() % 16;
        if (r < 10)
        {
            key[i] = (char)(r + 48);
        }
        else
        {
            key[i] = (char)(r + 87);
        }
    }
    printf("Random key: %s\n", key);
}

void uninstall(char **argv)
{
    char command[100];
    snprintf(command, 100, "nssm.exe remove %s confirm", SERVICE_NAME);
    system(command);
}

void stop(char **argv)
{
    char command[100];
    snprintf(command, 100, "nssm.exe stop %s", SERVICE_NAME);
    system(command);
}

void view_logs(char **argv)
{
    FILE *log = open_log();
    char *line = NULL;
    size_t len = 0;
    int count = atoi(argv[2]);
    char *lines[count];
    for (int i = 0; i < count; i++)
    {
        lines[i] = NULL;
    }
    int current = 0;
    int max = 0;
    while (getline(&line, &len, log) != -1)
    {
        if (lines[current] == NULL)
        {
            lines[current] = malloc(200);
        }
        strcpy(lines[current], line);
        current = (current + 1) % count;
        if (max < count)
        {
            max++;
        }
    }
    for (int i = 0; i < count; i++)
    {
        if (lines[(current + i) % count] != NULL)
        {
            printf("%s", lines[(current + i) % count]);
        }
    }
    for (int i = 0; i < count; i++)
    {
        free(lines[i]);
    }
}

int main(int argc, char **argv)
{
    actions[0] = (action_t){0, "--help", "", "Shows the list of commands", &help};
    actions[1] = (action_t){0, "status", "", "Shows the status of the service", &status};
    actions[2] = (action_t){0, "config", "", "Shows the current config", &view};
    actions[3] = (action_t){3, "install", "<filePath> <port> <token>", "Installs the server with this config", &install};
    actions[4] = (action_t){0, "remove", "<filePath> <port> <token>", "Uninstalls", &uninstall};
    actions[5] = (action_t){0, "keygen", "", "Generates a random key", &keygen};
    actions[6] = (action_t){0, "stop", "", "Stops the nssm service, used for a reinstall", &stop};
    actions[7] = (action_t){1, "log", "<count>", "View the last logs", &view_logs};
    check_folder();
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