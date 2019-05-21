#include <sched.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "libkdump.h" 


int main(int argc, char *argv[]){
  
  // Initialize configuration 
  libkdump_config_t config;
  config = libkdump_get_autoconfig();
  libkdump_init(config);

  const char *secret = "This is a secret! Don't tell anyone!";
  int len = strlen(secret);

  printf("Secret: %s\n", secret);

  // Retrieves the physical address of a virtual address
  size_t paddr = libkdump_virt_to_phys((size_t)secret);
  
  /*if (!paddr) {
    printf("\x1b[31;1m[!]\x1b[0m Program requires root privileges (or read access to /proc/<pid>/pagemap)!\n");
    libkdump_cleanup();
    exit(1);
  }*/

  printf("Physical address of secret: 0x%zx\n", paddr);
  printf("Click CTRL+C to exit\n");
  
  while (1) {
    // keep string cached for better results
    volatile size_t dummy = 0, i;
    for (i = 0; i < len; i++) {
      dummy += secret[i];
    }
    sched_yield();
  }

  libkdump_cleanup();

  return 0;
}
