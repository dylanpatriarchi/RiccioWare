#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <string.h>
#include <openssl/aes.h>

#define MAX_LINE_LENGTH 8192
#define MAX_LINES 100

void decrypt_file(const char *file_path, const unsigned char *aes_key) {
    FILE *fp = fopen(file_path, "r+");
    if (fp == NULL) {
        printf("Impossibile aprire il file %s per la lettura/scrittura\n", file_path);
        return;
    }

    fseek(fp, 0L, SEEK_END);
    long file_size = ftell(fp);
    rewind(fp);

    unsigned char *ciphertext = (unsigned char *)malloc(file_size);
    if (ciphertext == NULL) {
        printf("Errore durante l'allocazione di memoria\n");
        fclose(fp);
        return;
    }

    unsigned char *plaintext = (unsigned char *)malloc(file_size);
    if (plaintext == NULL) {
        printf("Errore durante l'allocazione di memoria\n");
        fclose(fp);
        free(ciphertext);
        return;
    }

    fread(ciphertext, 1, file_size, fp);

    AES_KEY aes;
    AES_set_decrypt_key(aes_key, 128, &aes);

    for (int i = 0; i < file_size; i += AES_BLOCK_SIZE) {
        AES_decrypt(ciphertext + i, plaintext + i, &aes);
    }

    fseek(fp, 0L, SEEK_SET);
    fwrite(plaintext, 1, file_size, fp);

    fclose(fp);
    free(ciphertext);
    free(plaintext);
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

                            printf("Decriptando il contenuto di %s:\n", file_path);
                            
                            unsigned char aes_key[] = "fiolisostancobuonanotte";
                            
                            decrypt_file(file_path, aes_key);
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