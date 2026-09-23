#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

void primes(int rpipe) {

  int prime;
  if (rpipe >= 0) {
    if (read(rpipe, &prime, sizeof(prime)) <= 0) exit(0);
    printf("prime %d\n", prime);
  }

  int cpipe[2];
  pipe(cpipe);

  int pid = fork();

  if (pid == 0) {
    close(cpipe[1]);
    if (rpipe >= 0) close(rpipe);
    primes(cpipe[0]);

  } else {
    close(cpipe[0]);

    if (rpipe == -1) {
      for (int i = 2; i <= 35; i++) {
        write(cpipe[1], &i, sizeof(int));
      }

    } else {
      int num;
      while (read(rpipe, &num, sizeof(num)) > 0) {
        if (num % prime != 0) {
          write(cpipe[1], &num, sizeof(num));
        }
      }
      close(rpipe);
    }

    close(cpipe[1]);
    wait(0);
  }
}

int main(int argc, char *argv[]) {
  if (argc != 1) {
    printf("Arguments not supported.\n");
    exit(1);
  }

  primes(-1);

  exit(0);
}
