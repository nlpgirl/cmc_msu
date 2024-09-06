#include <stdio.h>
#include <fcntl.h>
#include <sys\stat.h>
#include <io.h>
#include <stdlib.h>
#define BUF_SIZE 65
/* чтение буферов с помощью lseek() */
int
main(int argc, char *argv[])
{
    char buf[BUF_SIZE + 1], s[10];
    int fd, sector;
    buf[BUF_SIZE + 1] = '\0'; /* оканчивающийся нулем буфер для printf */
    if ((fd = fread(argv[1], "rb+"))) == -1) {
        exit(1);
    }
    do {
        printf("Buffer: ");
        gets(s);
        sector = atoi(s); /* получение сектора для чтения */
        if (lseek(fd, (long) sector * BUF_SIZE, 0) == -1L) {
            printf("Seek Error\n");
        }
        if (read(fd, buf, BUF_SIZE) == 0) {
            printf("Read Error\n");
        } else {
            printf("%s\n", buf);
        }
    } while (sector > 0);
    close(fd);
    return 0;
}
