#include <stdio.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <dirent.h>
#include <stdlib.h>
#include <time.h>

int configs(int argc, char *argv[]);

int main(int argc, char *argv[]) {
    if (argc == 1) {
        printf("Welcome to sap :)\n");
        return 0;
    }
    if (strcmp(argv[1], "config") == 0) {
        configs(argc, argv);
        return 0;
    }
}

int configs(int argc, char *argv[]) {
    FILE *ptr;
    DIR *dir;
    char user_name[100];
    char user_email[100];
    time_t t;
    /////
    if (argc == 4 && strcmp(argv[2], "user.name") == 0) {
        dir = opendir("./.configs");
        if (!dir)
            mkdir("./.configs", 0777);
        ptr = fopen("./.configs/username.txt", "w");
        if (ptr == NULL) {
            perror("Failed to open configs file");
            return 1; }
        sprintf(user_name, "user name: %s", argv[3]);
        fprintf(ptr, "%s", user_name);
        t = time(NULL);
        fprintf(ptr, "\n%ld", t);
        fclose(ptr);
        printf("user name updated successfully!\n");
        return 0;
    }
    /////
    if (argc == 4 && strcmp(argv[2], "user.email") == 0) {
        dir = opendir("./.configs");
        if (!dir)
            mkdir("./.configs", 0777);
        ptr = fopen("./.configs/user_email.txt", "w");
        if (ptr == NULL) {
            perror("Failed to open configs file");
            return 1;
        }
        sprintf(user_email, "user email: %s", argv[3]);
        fprintf(ptr, "%s", user_email);
        t = time(NULL);
        fprintf(ptr, "\n%ld", t);
        fclose(ptr);
        printf("user email updated successfully!\n");
        return 0;
    }
    /////
    if (argc == 5 && strcmp(argv[2], "-global") == 0 && strcmp(argv[3], "user.name") == 0) {
        dir = opendir("/mnt/c/.configs");
        if (!dir)
            mkdir("/mnt/c/.configs", 0777);
        ptr = fopen("/mnt/c/.configs/user_name.txt", "w");
        if (ptr == NULL) {
            perror("Failed to open configs file");
            return 1;
        }
        sprintf(user_name, "global user name: %s", argv[4]);
        fprintf(ptr, "%s", user_name);
        t = time(NULL);
        fprintf(ptr, "\n%ld", t);
        fclose(ptr);
        printf("global user name updated successfully!\n");
        return 0;
    }
    if (argc == 5 && strcmp(argv[2], "-global") == 0 && strcmp(argv[3], "user.email") == 0) {
        dir = opendir("/mnt/c/.configs");
        if (!dir)
            mkdir("/mnt/c/.configs", 0777);
        ptr = fopen("/mnt/c/.configs/user_email.txt", "w");
        if (ptr == NULL) {
            perror("Failed to open configs file");
            return 1;
        }
        sprintf(user_email, "global user email: %s", argv[4]);
        fprintf(ptr, "%s", user_email);
        t = time(NULL);
        fprintf(ptr, "\n%ld", t);
        fclose(ptr);
        printf("global user email updated successfully!\n");
        return 0;
    }
    if (argc == 4 && strstr(argv[2], "alias.") != NULL) {

    }
    if (argc == 5 && strcmp(argv[2], "-global") == 0 && strstr(argv[3], "alias.") != NULL) {

    }
        printf("Wrong config command!\n");
}