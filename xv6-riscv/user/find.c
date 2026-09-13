#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/fs.h"

char* obtener_nombre(char *ruta) {
  char *aux;

  for(aux = ruta + strlen(ruta); aux >= ruta && *aux != '/'; aux--) {}
  aux++;
  return aux;
}

void find(char *ruta, char *archivo) {
  char buf[512], *p;
  int fd;
  struct stat st;
  struct dirent de;

  if ((fd = open(ruta, 0)) < 0) {
    printf("No se pudo abrir %s\n", ruta);
    return;
  }

  if (fstat(fd, &st) < 0) {
    printf("No se pudo obtener stat de %s\n", ruta);
    close(fd);
    return;
  }

  switch (st.type) {
    case T_FILE:
      char *nombre_arch = obtener_nombre(ruta);

      if (strcmp(nombre_arch, archivo) == 0) {
        printf("%s\n", ruta);
      }
      break;

    case T_DIR:
      if (strlen(ruta) + 1 + DIRSIZ + 1 > sizeof(buf)) {
        printf("Ruta demasiado larga");
        break;
      }

      strcpy(buf, ruta);
      p = buf + strlen(buf);
      *p++ = '/';

      while (read(fd, &de, sizeof(de)) == sizeof(de)) {
        if (de.inum == 0) {
          continue;
        }

        if (strcmp(de.name, ".") == 0 || strcmp(de.name, "..") == 0) {
          continue;
        }

        memmove(p, de.name, DIRSIZ);
        p[DIRSIZ] = '\0';

        find(buf, archivo);
      }

      break;
  }

  close(fd);
}

int main(int argc, char *argv[]) {
  if (argc != 3) {
    printf("Error: Argumentos inválidos\n");
    exit(1);
  }

  find(argv[1], argv[2]);
  exit(0);
}
