#include <stdio.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <dirent.h>
#include <stdlib.h>
#include <time.h>
#include <stdbool.h>

int configs(int argc, char *argv[]);
int run_init();
int run_add(int argc, char *argv[]);
int add_to_staging(char *filepath);
int copy_file(char file_name[], char dest_path[]);
int last_modification_time();
int check_if_there_is_a_sap_repo();
char *where_is_sap();
int is_path_existed(char path[]);

int main(int argc, char *argv[]) {
    if (argc == 1) {
        printf("Welcome to sap :)\n");
        return 0;
    }
    if (strcmp(argv[1], "config") == 0) {
        configs(argc, argv);
        return 0;
    }
    if (argc == 2 && strcmp(argv[1], "init") == 0) {
        run_init();
        return 0;
    }
    if (strcmp(argv[1], "add") == 0 && argc >= 3) {
        run_add(argc, argv);
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

int run_init() {
    DIR *dir;
    struct dirent *entry;
    bool exist = false;
    char cwd[260];
    char tmp_dir[260];
    /////
    if (getcwd(cwd, sizeof(cwd)) == NULL) {
        perror("error");
        return 1;
    }
    do {
        dir = opendir(".");
        if (dir == NULL) {
            perror("error");
            return 1;
        }
        while ((entry = readdir(dir)) != NULL) {
            if (strcmp(entry->d_name, ".sap") == 0) {
                exist = true;
                char *s = getcwd(s, 260);
                printf("Sap had been initialized before in %s\n", s);
                return 0;
            }
        }
        if (getcwd(tmp_dir, sizeof(tmp_dir)) == NULL) {
            perror("error");
            return 1;
        }
        if (strcmp(tmp_dir, "/") != 0)
            if (chdir("..") != 0) return 1;
    } while (strcmp(tmp_dir, "/") != 0);
    if (!exist) {
        chdir(cwd);
        if (mkdir(".sap", 0777) == 0) {
            FILE *fptr = fopen(".sap/staging.txt", "w");
            if (fptr != NULL) {
                printf("Initialized empty Sap repository in %s\n", cwd);
                return 0;
            } else {
                perror("sap initialization was not successful");
                return 1;
            }
        } else {
            perror("sap initialization failed");
            return 1;
        }
    }
}

int run_add(int argc, char *argv[]) {
    int is_inited = check_if_there_is_a_sap_repo();
    if (is_inited == -1) {
        printf("error: No repo has been initialized yet\n");
        return 0;
    }
    for (int i = 2; i < argc; ++i) {
        char *filepath = argv[i];
        if (is_path_existed(filepath) == -1) {
            add_to_staging(filepath);
            char s[260];
            strcpy(s, argv[i]);
            copy_file(s, ".sap/");
        }
        printf("Done!\n");
    }
    return 0;
}

int add_to_staging(char *filepath) {
    FILE *fptr;
    fptr = fopen(".sap/staging.txt", "r");
    if (fptr == NULL)
        perror("error");
    char line[256];
    while (fgets(line, sizeof(line), fptr) != NULL) {
        int length = strlen(line);
        if (length > 0 && line[length - 1] == '\n')
            line[length - 1] = '\0';
        if (strcmp(line, filepath) == 0) {
            printf("This file is also added before.\n");
            fclose(fptr);
            return 0;
        }
    }
    fclose(fptr);
    fptr = fopen(".sap/staging.txt", "a");
    fprintf(fptr, "%s\n", filepath);
    fclose(fptr);
    return 0;
}

int copy_file(char file_name[], char dest_path[]){
    FILE *sourceFile, *targetFile;
    char dest_file_path[260];
    strcpy(dest_file_path, dest_path);
    strcat(dest_file_path, file_name);
    char *file_path;
    getcwd(file_path, 100);
    char ch;
    sourceFile = fopen(file_name, "rb");
    if (sourceFile == NULL) {
    perror("Unable to open source file");
    return 1;
    }
    targetFile = fopen(dest_file_path, "wb");
    if (targetFile == NULL) {
    perror("Unable to open dest file");
    return 1;
    }
    while ((ch = fgetc(sourceFile)) != EOF)
        fputc(ch, targetFile);
    fclose(sourceFile);
    fclose(targetFile);
    return 0;
}

int check_if_there_is_a_sap_repo() {
    DIR *dir;
    struct dirent *entry;
    bool exist = false;
    char cwd[260];
    char tmp_dir[260];
    /////
    if (getcwd(cwd, sizeof(cwd)) == NULL) {
        perror("error");
        return 1;
    }
    do {
        dir = opendir(".");
        if (dir == NULL) {
            perror("error");
            return 1;
        }
        while ((entry = readdir(dir)) != NULL) {
            if (strcmp(entry->d_name, ".sap") == 0) {
                exist = true;
                return 0;
            }
        }
        if (getcwd(tmp_dir, sizeof(tmp_dir)) == NULL) {
            perror("error");
            return 1;
        }
        if (strcmp(tmp_dir, "/") != 0)
            if (chdir("..") != 0) return 1;
    } while (strcmp(tmp_dir, "/") != 0);
    if (!exist)
        return -1;
}

char *where_is_sap() {
    DIR *dir;
    char *sap_path;
    struct dirent *entry;
    char cwd[260];
    char tmp_dir[260];
    /////
    if (getcwd(cwd, sizeof(cwd)) == NULL) {
        perror("error");
    }
    do {
        dir = opendir(".");
        if (dir == NULL)
            perror("error");
        while ((entry = readdir(dir)) != NULL) {
            if (strcmp(entry->d_name, ".sap") == 0) {
                sap_path = (char *) malloc(260);
                getcwd(sap_path, 260);
                return sap_path;
            }
        }
        if (getcwd(tmp_dir, sizeof(tmp_dir)) == NULL)
            perror("error");
        if (strcmp(tmp_dir, "/") != 0)
            chdir("..");
    } while (strcmp(tmp_dir, "/") != 0);
}

int is_path_existed(char path[]) {
    struct stat ex;
    /////
    char *definite_path = where_is_sap();
    char *q = strcat(definite_path, "/");
    char *p = strcat(definite_path, path);
    int exist = stat(definite_path,&ex);
    if(exist != 0) {
        printf("Directory does not exist.\n");
        return 0;
    }
    return -1;
}
