#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

void find(char *ruta, char *archivo) {

}

int main(int argc, char *argv[]) {
  if (argc != 3) {
    printf("Error: Argumentos inválidos\n");
    exit(1);
  }

  find(argv[1], argv[2]);
  exit(0);
}
