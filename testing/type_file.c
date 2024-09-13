#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <unistd.h>
#include <errno.h>

// Verifica si el archivo es un directorio
int is_directory(mode_t mode) {
    return (mode & S_IFMT) == S_IFDIR;
}

// Verifica si el archivo es un archivo regular
int is_regular_file(mode_t mode) {
    return (mode & S_IFMT) == S_IFREG;
}

// Verifica si el archivo es un enlace simbólico
int is_symlink(mode_t mode) {
    return (mode & S_IFMT) == S_IFLNK;
}

// Verifica si el archivo es ejecutable
int is_executable(mode_t mode) {
    return (mode & S_IXUSR) || (mode & S_IXGRP) || (mode & S_IXOTH);
}

void check_file_type(const char *pathname) {
    struct stat path_stat;

    if (access(pathname, F_OK) != 0) {
        perror("access");
        return;
    }

    if (stat(pathname, &path_stat) != 0) {
        perror("stat");
        return;
    }

    if (is_regular_file(path_stat.st_mode)) {
        printf("'%s' es un archivo regular.\n", pathname);
    } else if (is_directory(path_stat.st_mode)) {
        printf("'%s' es un directorio.\n", pathname);
    } else if (is_symlink(path_stat.st_mode)) {
        printf("'%s' es un enlace simbólico.\n", pathname);
    } else {
        printf("'%s' es de un tipo desconocido.\n", pathname);
    }

    if (is_executable(path_stat.st_mode)) {
        printf("'%s' es ejecutable.\n", pathname);
    } else {
        printf("'%s' no es ejecutable.\n", pathname);
    }
}

int main() {
    const char *path1 = "a.out";  // Cambia esto por una ruta de archivo
    const char *path2 = "/usr/bin/ls";   // Cambia esto por una ruta a un comando
    const char *path3 = "/home/user";    // Cambia esto por una ruta a un directorio

    check_file_type(path1);
    check_file_type(path2);
    check_file_type(path3);

    return 0;
}
