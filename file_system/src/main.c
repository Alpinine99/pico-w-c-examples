#include <stdio.h>
#include <FreeRTOS.h>
#include <task.h>
#include <pico/stdlib.h>
#include <pico_hal.h>


void peep();

int main(void) {
  stdio_init_all();

  xTaskCreate(
      peep, "peep", 256, NULL, 1, NULL
  );

  vTaskStartScheduler();

  while (true);
}

void peep() {

  while (!stdio_usb_connected());

  printf("File system analysis\n");
  if (pico_mount(false) != LFS_ERR_OK) {
    fprintf(stderr, "Mount failed\n");
  } else {
    printf("fs mounted\n");

    // show fs size
    struct pico_fsstat_t stat;
    pico_fsstat(&stat);
    printf("FS: block %d size %d used %d\n",
        (int) stat.block_count, (int) stat.block_size, (int) stat.blocks_used);

    // open root
    int dir = pico_dir_open("/");
    if (dir < 0) {
      fprintf(stderr, "could not open /\n");
    } else {
      if (pico_remove("/boot") < 0) 
        printf("failed to remove directory /boot\n");

      if (pico_mkdir("/var") < 0)
        printf("failed to create directory /var\n");

      printf("files and directories list\n");
      struct lfs_info info;
      while (pico_dir_read(dir, &info)) {
        char* type = info.type == LFS_TYPE_DIR ? "directory" : "regular file";
        printf("[name] %-15s [type] %-15s [size] %-10zu\n", info.name, type, info.size);
      }
      pico_dir_close(dir);

      if ((dir = pico_dir_open("/var")) < 0) {
        fprintf(stderr, "failed to open /var\n");
      } else {
        // create e file and open for write
        int file = (int) pico_open("write-test.txt", LFS_O_WRONLY | LFS_O_CREAT);
        if (file >= 0) {
          char buf[] = "Hello mate how are you man?";
          if (pico_write(file, buf, sizeof(buf)+1) == (strlen(buf)+1))
            printf("written data to file\n");
          else
            fprintf(stderr, "write failed\n");
          pico_close(file);
        }
        file = (int) pico_open("write-test.txt", LFS_O_RDONLY);
        if (file >= 0) {
          char buf[64];
          size_t n = pico_read(file, buf, sizeof(buf));
          if (n <= 0) {
            fprintf(stderr, "read failed\n");
          } else {
            buf[n] = '\0';
            printf("data: %s\n", buf);
          }
          if (pico_remove("write-test.txt") < 0)
            fprintf(stderr, "delete failed\n");
          pico_close(file);
        }
        pico_dir_close(dir);
      }
    }
    pico_unmount();
  }

  while (true);
}

