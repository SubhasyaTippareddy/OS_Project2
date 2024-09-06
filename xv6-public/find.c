#include "types.h"
#include "stat.h"
#include "user.h"
#include "fs.h"

#define DIR_SIZE 20
char *
format_name(char *string)
{
    static char store[DIR_SIZE + 1];
    char *i;
    int len;
    for (i = string + strlen(string); i >= string && *i != '/'; i--)
        ;
    i++;
    len = strlen(i);
    if (len >= DIR_SIZE)
        return i;
    memmove(store, i, len);
    memset(store + len, '\0', DIR_SIZE - len);
    return store;
}

void type_dir(char *file_path, char *dirname, struct stat metadata, char *buffer, int file_des)
{
    struct dirent entry;
    struct stat new_metadata;
    char *point;
    int len;

    strcpy(buffer, file_path);
    point = buffer + strlen(buffer);
    *point++ = '/';

    len = sizeof(entry);
    while (read(file_des, &entry, len) == len)
    {
        if (entry.inum != 0)
        {
            stat(buffer, &metadata);
            memmove(point, entry.name, DIR_SIZE);
            point[DIR_SIZE] = 0;

            if (metadata.type == 1 && strcmp(format_name(buffer), dirname) == 0)
            {
                printf(1, "%s\n", buffer);
            }
            else if (metadata.type == 1)
            {
                if (strcmp(format_name(buffer), ".") != 0 && strcmp(format_name(buffer), "..") != 0)
                {
                    int new_file_des = open(buffer, 0);
                    fstat(file_des, &new_metadata);
                    type_dir(buffer, dirname, new_metadata, buffer, new_file_des);
                    close(new_file_des);
                }
            }
        }
    }
}

void inum(char *file_path, char *filename, struct stat metadata, char *buffer, int file_des, int value, int sign)
{
    struct dirent entry;
    struct stat new_metadata;
    char *point;
    int len;

    strcpy(buffer, file_path);
    point = buffer + strlen(buffer);
    *point++ = '/';

    len = sizeof(entry);
    while (read(file_des, &entry, len) == len)
    {
        if (entry.inum != 0)
        {
            memmove(point, entry.name, DIR_SIZE);
            point[DIR_SIZE] = 0;
            stat(buffer, &metadata);

            if (metadata.type == 2 && strcmp(format_name(buffer), filename) == 0)
            {
                if ((metadata.ino == value && sign == 0) || (metadata.ino > value && sign == 1) || (metadata.ino < value && sign == -1))
                    printf(1, "%s\n", buffer);
            }
            else if (metadata.type == 1)
            {
                if (strcmp(format_name(buffer), ".") != 0 && strcmp(format_name(buffer), "..") != 0)
                {
                    fstat(file_des, &new_metadata);
                    int new_file_des = open(buffer, 0);
                    inum(buffer, filename, new_metadata, buffer, new_file_des, value, sign);
                    close(new_file_des);
                }
            }
        }
    }
}

void find_dir(char *file_path, char *dirname, struct stat metadata, char *buffer, int file_des)
{
    struct dirent entry;
    struct stat new_metadata;
    char *point;
    int len;

    strcpy(buffer, file_path);
    point = buffer + strlen(buffer);
    *point++ = '/';

    len = sizeof(entry);
    while (read(file_des, &entry, len) == len)
    {
        if (entry.inum != 0)
        {
            memmove(point, entry.name, DIR_SIZE);
            stat(buffer, &metadata);
            point[DIR_SIZE] = 0;
            if (metadata.type == 1)
            {
                if (strcmp(format_name(buffer), ".") != 0 && strcmp(format_name(buffer), "..") != 0)
                {
                    fstat(file_des, &new_metadata);
                    int new_file_des = open(buffer, 0);

                    if (strcmp(format_name(buffer), dirname) == 0)
                        printf(1, "%s\n", buffer);
                    find_dir(buffer, dirname, new_metadata, buffer, new_file_des);
                    close(new_file_des);
                }
            }
        }
    }
}

void find_file(char *file_path, char *filename, struct stat metadata, char *buffer, int file_des)
{
    struct dirent entry;
    struct stat new_metadata;
    char *point;
    int len;

    strcpy(buffer, file_path);
    point = buffer + strlen(buffer);
    *point++ = '/';

    len = sizeof(entry);
    while (read(file_des, &entry, len) == len)
    {
        if (entry.inum != 0)
        {
            memmove(point, entry.name, DIR_SIZE);
            stat(buffer, &metadata);
            point[DIR_SIZE] = 0;
            if (metadata.type == 2 && strcmp(format_name(buffer), filename) == 0)
            {
                printf(1, "%s\n", buffer);
            }
            else if (metadata.type == 1)
            {
                if (strcmp(format_name(buffer), ".") != 0 && strcmp(format_name(buffer), "..") != 0)
                {
                    fstat(file_des, &new_metadata);
                    int new_file_des = open(buffer, 0);
                    find_file(buffer, filename, new_metadata, buffer, new_file_des);
                    close(new_file_des);
                }
            }
        }
    }
}

void find(char *file_path, char *filename, struct stat metadata, char *buffer, int file_des, int flag)
{
    struct dirent entry;
    struct stat new_metadata;
    char *point;
    int len;

    strcpy(buffer, file_path);
    point = buffer + strlen(buffer);
    *point++ = '/';

    len = sizeof(entry);
    while (read(file_des, &entry, len) == len)
    {
        if (entry.inum != 0)
        {
            memmove(point, entry.name, DIR_SIZE);
            stat(buffer, &metadata);
            point[DIR_SIZE] = 0;
            if (metadata.type == 2 && strcmp(format_name(buffer), filename) == 0)
            {
                if (flag == 0)
                    printf(1, "%d %s\n", metadata.ino, buffer); // to cover -printi
                else
                    printf(1, "%s\n", buffer);
            }
            else if (metadata.type == 1)
            {
                if (strcmp(format_name(buffer), ".") != 0 && strcmp(format_name(buffer), "..") != 0)
                {
                    fstat(file_des, &new_metadata);
                    int new_file_des = open(buffer, 0);

                    if (strcmp(format_name(buffer), filename) == 0 && flag != 0)
                        printf(1, "%s\n", buffer);
                    find(buffer, filename, new_metadata, buffer, new_file_des, flag);
                    close(new_file_des);
                }
            }
        }
    }
}

// argv[0] - find
// argv[1] - target directory
// argv[2] - -name
// argv[3] - filename
// argv[4] - -type, -inum, -printi
// argv[5] - (+/-) inode, f,d

int main(int argc, char *argv[])
{
    int file_descriptor;
    char buffer[512];
    struct stat file_info;

    char *flag;   // for argv[4]
    char *choice; // for argv[5]

    if (argc < 3) // find .
    {
        printf(1, "Error:  Incomplete command.\n");
    }
    else if (argc < 4) // find . -name
    {
        printf(1, "Error:  Target Directory/File name is either not provided or doesn't exist.\n");
    }
    else
    {
        flag = argv[4];   // parameter
        choice = argv[5]; // extra_args

        file_descriptor = open(argv[1], 0);
        fstat(file_descriptor, &file_info);
        if (file_info.type == 1) // if its a directory
        {
            if (strcmp(argv[2], "-name") == 0)
            {
                if (argc == 4) // default find function
                {
                    find(argv[1], argv[3], file_info, buffer, file_descriptor, 1);
                }
                else if (strcmp(flag, "-type") == 0) // type function
                {
                    if (strcmp(choice, "d") == 0)
                    {
                        find_dir(argv[1], argv[3], file_info, buffer, file_descriptor); // for type d - directories
                    }
                    else if (strcmp(choice, "f") == 0)
                    {
                        find_file(argv[1], argv[3], file_info, buffer, file_descriptor); // for type f - files
                    }
                    else
                    {
                        printf(1, "Error:  Incorrect flag provided. Available choices for type command: -f -d.\n");
                    }
                }
                else if (strcmp(flag, "-printi") == 0) //list all files that match the filename with inode number
                {
                    find(argv[1], argv[3], file_info, buffer, file_descriptor, 0);
                }
                else if (strcmp(flag, "-inum") == 0) //inode number comparision
                {
                    if (strchr(choice, '+'))
                    {
                        choice = choice + 1;
                        inum(argv[1], argv[3], file_info, buffer, file_descriptor, atoi(choice), 1); // for +inum
                    }
                    else if (strchr(choice, '-'))
                    {
                        choice = choice + 1;
                        inum(argv[1], argv[3], file_info, buffer, file_descriptor, atoi(choice), -1); // for -inum
                    }
                    else
                    {
                        inum(argv[1], argv[3], file_info, buffer, file_descriptor, atoi(choice), 0); // for inum
                    }
                }
                else{
                    printf(1,"Error:  Incorrect option given.\nAvailable Options find . -name <name> [-type (d) (f)] [-inum (+/-inode)] [-printi]\n");
                }
            }
            else
            {
                printf(1, "Error : Incorrect command, use '-name'.\n");
            }
        }
        else
        {
            printf(1, "Error : Location not present.\n");
        }
        close(file_descriptor);
    }
    exit();
}