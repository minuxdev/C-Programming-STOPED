#include <stdio.h>
#include <string.h>
#include <dirent.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <errno.h>

extern int errno;

#define MAX_LEN 255
#define MODE 0775


void copy_content(char src[], char new_path[]);
void make_dir(char dst[]);
void read_write(char file_name[], char new_file[]);
void get_src_name(char *src, char *src_n);
int check_source(char  *src);

int main(void)
{
    char src[MAX_LEN], dst[MAX_LEN];
    char src_n[100], new_path[MAX_LEN];
    int check;

    // Get paths: Sorce & Destiny
    printf("From: ");
    fgets(src, MAX_LEN, stdin);
    src[strlen(src) -1] = 0;

    printf("To: ");
    fgets(dst, MAX_LEN, stdin);
    dst[strlen(dst) -1] = 0;

    // Check if source is directory or file
    // 0 = file; 1 = directory
    check = check_source(src);

    get_src_name(src, src_n);
    sprintf(new_path, "%s/%s", dst, src_n);
    
    switch (check)
    {
    case 0:
        printf("Source is file! %d\n", check);
        read_write(src, new_path);
        break;
    case 1:
        printf("Source is directory %d\n", check);
        make_dir(new_path);

        copy_content(src, new_path);

        break;
    default:
        printf("Error opening source!");
        break;
    }

    printf("*** Process Completed *** \n");
    return 0;
}


// 1 - List Directory contents
void copy_content(char src[MAX_LEN], char new_path[MAX_LEN])
{
    DIR *sp, *dp;
    struct dirent *entity;
    char name[100], subdir[100];

    sp = opendir(src);
    if (sp == NULL){
        perror("Open Sorce: ");
        exit(1);
    } else {
        dp = opendir(new_path);
        if (dp == NULL){
            perror("Open Dest: ");
            exit(2);
        } else {
            while ( (entity = readdir(sp)) ){
                if ( strcmp(entity->d_name, ".") != 0 && strcmp(entity->d_name, "..") != 0){
                    
                    if (entity->d_type == DT_DIR){
                        
                        
                        sprintf(name, "%s/%s", new_path, entity->d_name);
                        make_dir(name);
                        printf("Name: %s/%s", new_path, entity->d_name);
                        sprintf(subdir, "%s/%s", src, entity->d_name);
                        
                        copy_content(name, subdir);

                    } else {
                        
                        sprintf(name, "%s/%s", new_path, entity->d_name);
                        sprintf(subdir, "%s/%s", src, entity->d_name);
                        
                        printf("Writing into... subdir: %s\nname: %s\n", subdir, name);
                        
                        read_write(subdir, name);
                    }
                }
            }
        }
    }
}

// 2 - Make directory
void make_dir(char dst[])
{   
    int stat = mkdir(dst, MODE);
    if (stat == 0){
    } else {
        printf("%s Already exist!\n", dst);
        perror("Make Dir: ");
    }
}

// 3 -Read and Write content into new file
void read_write(char file_name[], char new_file[])
{
    FILE *sp, *dp;
    char buffer[128];
    int byte = 1;

    memset(buffer, 0, sizeof(buffer));

    sp = fopen(file_name, "rb");
    if ( sp == NULL ){

        perror("Open File: ");
        exit(1);
    } else {
        dp = fopen(new_file, "wb+");
        if ( dp == NULL ) {
            perror("Write New File: ");
            exit(1);
        } else {
            while ( byte > 0 ){
                byte = fread(buffer, sizeof(buffer), 1, sp);
                fwrite(buffer, sizeof(buffer), 1, dp);
            }
            fclose(sp);
            fclose(dp);
        }
    }
}

// 4 - Get source name
void get_src_name(char *src, char *src_n)
{
    char *p1, *name;
    char local_src[MAX_LEN];

    strcpy(local_src, src);

    p1 = strtok(local_src, "/");
    while (p1 != NULL){
        name = p1;
        p1 = strtok(NULL, "/");
    }
    strcpy(src_n, name);
}

// 5 - Check if source is file or directory
int check_source(char  *src)
{
    DIR *dp;
    FILE *fp;
    struct dirent *entity;
    
    dp = opendir(src);
    fp = fopen(src, "r");

    if ( dp == NULL && fp == NULL ){
        printf("Source is not a valid address...");
        exit(9);
    } else {
        if (dp == NULL ){
            return 0;
        } else {
            return 1;
        }
    }
    closedir(dp);
    fclose(fp);
    return -1;
}

// /Users/minux/Documents/c-lab/source
// /Users/minux/Documents/c-lab/dest
