#include <stdio.h>
#include <fcntl.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <stdio.h>
#include <sys/stat.h>
#include <stdint.h>
#include "test_data.h"
#include <err.h>
#include <errno.h>
#include <unistd.h>
#include <string.h>
#include <sys/resource.h>

#define DAT_SIZE		TEST_SIZE
#define PERMS_SIZE		PERM_LEN
#define SEED_SIZE		SEED_LEN

#define LD_PERMS		0x0F
#define LD_T_PERMS		0x81
#define LD_SEED			0xF0
#define ENCRYPT			0x3C
#define DECRYPT			0x11
#define DONE			0xCC

#define CTRL_REG        0x00
#define SRC_REG         0x10
#define DST_REG         0x18
#define SIZE_REG        0x20
#define CONTROL_REG     0X28

#define SRC_ADDR        0x30000000
#define DST_ADDR        0x34000000
#define SEED_ADDR       0x38000000
#define PERMS_ADDR       0x3C000000

#define QPP_MAP_SIZE    0x10000
#define SRC_MAP_SIZE    0x04000000
#define DST_MAP_SIZE    0x04000000
#define SEED_MAP_SIZE   0x04000000
#define PERM_MAP_SIZE   0x04000000

#define SRC_MAP_OFFSET  0x0
#define DST_MAP_OFFSET  SRC_MAP_SIZE
#define SEED_MAP_OFFSET DST_MAP_OFFSET+DST_MAP_SIZE
#define PERM_MAP_OFFSET SEED_MAP_OFFSET+SEED_MAP_SIZE



volatile unsigned result[TEST_SIZE];



extern int errno ;
int errnum;

#define handle_error(msg) \
    do { perror(msg); exit(-1); } while (0)

void write_reg(volatile unsigned *addr, unsigned offset, unsigned value);
uint32_t read_reg(volatile unsigned *addr, unsigned offset);
int qpp_ready(volatile unsigned *qpp_addr);
int qpp_done(volatile unsigned *qpp_addr);
void qpp_set_size(volatile unsigned *qpp_addr, unsigned value);
void qpp_set_dst(volatile unsigned *qpp_addr, unsigned value);
void qpp_set_src(volatile unsigned *qpp_addr, unsigned value);
void qpp_start(volatile unsigned *qpp_addr);
void qpp_set_control(volatile unsigned *qpp_addr, unsigned value);


void write_reg(volatile unsigned *addr, unsigned offset, unsigned value) {
    addr[offset >> 2] = value;
}

uint32_t read_reg(volatile unsigned *addr, unsigned offset) {
    return addr[offset >> 2];
}

int qpp_ready(volatile unsigned *qpp_addr) {
    unsigned Data = read_reg(qpp_addr, CTRL_REG);
    return !(Data & 0x1);
}

int qpp_done(volatile unsigned *qpp_addr) {
    unsigned Data = read_reg(qpp_addr, CTRL_REG);
    return (Data >> 1) & 0x1;
}

void qpp_set_size(volatile unsigned *qpp_addr, unsigned value) {
    write_reg(qpp_addr, SIZE_REG, value);
}

void qpp_set_dst(volatile unsigned *qpp_addr, unsigned value) {
    write_reg(qpp_addr, DST_REG, value);
}

void qpp_set_src(volatile unsigned *qpp_addr, unsigned value) {
    write_reg(qpp_addr, SRC_REG, value);
}

void qpp_start(volatile unsigned *qpp_addr) {
    unsigned Data = read_reg(qpp_addr, CTRL_REG) & 0x80;
    write_reg(qpp_addr, CTRL_REG, Data | 0x01);
}

void qpp_set_control(volatile unsigned *qpp_addr, unsigned value) {
    write_reg(qpp_addr, CONTROL_REG, value);
}



int main() {
    struct rlimit limit;
    if (getrlimit(RLIMIT_AS, &limit) == -1) {
        perror("getrlimit");
        return 1;
    }
    printf("Maximum process size: %llu bytes\n", (unsigned long long)limit.rlim_cur);
    printf("%ld\n", sysconf(_SC_PAGE_SIZE));
    volatile unsigned *qpp_addr;
    volatile unsigned *dst_addr;
    volatile unsigned *src_addr;
    volatile unsigned *seed_addr;
    volatile unsigned *perm_addr;

    
    struct stat sb0;
    struct stat sb1;

    
    int qpp_fd = open("/dev/uio0", O_RDWR | O_SYNC);
	if (qpp_fd < 1) {
        handle_error("Could not open /dev/uio0");
	}

    /* mmap the qpp UIO device */
    qpp_addr = (volatile unsigned *)mmap(NULL, QPP_MAP_SIZE, PROT_READ|PROT_WRITE, MAP_SHARED,
                    qpp_fd, 0);
    if (!qpp_addr) {
        handle_error("Could not mmap /dev/uio0");
    }

    int src_fd = open("/dev/uio1", O_RDWR | O_SYNC);
	if (src_fd < 1) {
        handle_error("Could not open /dev/uio1");
	}

    int dst_fd = open("/dev/uio2", O_RDWR | O_SYNC);
	if (dst_fd < 1) {
        handle_error("Could not open /dev/uio2");
	}

    int seed_fd = open("/dev/uio3", O_RDWR | O_SYNC);
	if (seed_fd < 1) {
        handle_error("Could not open /dev/uio3");
	}

    int perm_fd = open("/dev/uio4", O_RDWR | O_SYNC);
	if (perm_fd < 1) {
        handle_error("Could not open /dev/uio4");
	}

    /* mmap the reserved mem UIO device */
    src_addr = (volatile unsigned *)mmap(NULL, SRC_MAP_SIZE, PROT_READ|PROT_WRITE, MAP_SHARED,
                    src_fd, SRC_MAP_OFFSET);
    if (src_addr == (void*)-1) {
        errnum = errno;
        fprintf(stderr, "Value of errno: %d\n", errno);
        fprintf(stderr, "Error opening file: %s\n", strerror( errnum ));
        handle_error("Could not mmap /dev/uio1 src");
    }
    // int size_reg = 0x20;
    // qpp_addr[size_reg >> 2] = 100;

    dst_addr = (volatile unsigned *)mmap(NULL, DST_MAP_SIZE, PROT_READ|PROT_WRITE, MAP_SHARED,
                    dst_fd, 0);
    if (dst_addr == (void*)-1) {
        errnum = errno;
        fprintf(stderr, "Value of errno: %d\n", errno);
        fprintf(stderr, "Error opening file: %s\n", strerror( errnum ));
        handle_error("Could not mmap /dev/uio2 dst");
    }

    seed_addr = (volatile unsigned *)mmap(NULL, SEED_MAP_SIZE, PROT_READ|PROT_WRITE, MAP_SHARED,
                    seed_fd, 0);
    if (seed_addr == (void*)-1) {
        errnum = errno;
        fprintf(stderr, "Value of errno: %d\n", errno);
        fprintf(stderr, "Error opening file: %s\n", strerror( errnum ));
        handle_error("Could not mmap /dev/uio3 dst");
    }

    perm_addr = (volatile unsigned *)mmap(NULL, PERM_MAP_SIZE, PROT_READ|PROT_WRITE, MAP_SHARED,
                    perm_fd, 0);
    if (perm_addr == (void*)-1) {
        errnum = errno;
        fprintf(stderr, "Value of errno: %d\n", errno);
        fprintf(stderr, "Error opening file: %s\n", strerror( errnum ));
        handle_error("Could not mmap /dev/uio4 dst");
    }

    /*
    * Copy perms to src addr
    */
    for(int i = 0; i < PERMS_SIZE; i++) {
        perm_addr[i] = test_perms[i];
    }
    /*
    * Copy plain text to src addr
    */
    for(int i = 0; i < DAT_SIZE; i++) {
        src_addr[i] = test_data[i];
    }
    // dst_addr[0] = 0xFFFFFFFF;

    while(!qpp_ready);

    // Load Perms
    qpp_set_control(qpp_addr, LD_PERMS);
    qpp_set_size(qpp_addr, PERMS_SIZE);
    qpp_set_src(qpp_addr, PERMS_ADDR);
    qpp_start(qpp_addr);
    while(!qpp_done(qpp_addr));
    while(!qpp_ready(qpp_addr));



    // Begin Encryption
    qpp_set_control(qpp_addr, ENCRYPT);
    qpp_set_size(qpp_addr, DAT_SIZE);
    qpp_set_src(qpp_addr, SRC_ADDR);
    qpp_set_dst(qpp_addr, DST_ADDR);
    qpp_start(qpp_addr);
    while(!qpp_done(qpp_addr));
    while(!qpp_ready(qpp_addr));


    /*
    * Copy test perms to perms addr
    */
    for(int i = 0; i < PERMS_SIZE; i++) {
        perm_addr[i] = test_t_perms[i];
    }

    // for(int i = 0; i < DAT_SIZE; i++) {
    //     src_addr[i] = (volatile unsigned)dst_addr[i];
    // }

    // Load Transpose Perms
    qpp_set_control(qpp_addr, LD_T_PERMS);
    qpp_set_size(qpp_addr, PERMS_SIZE);
    qpp_set_src(qpp_addr, PERMS_ADDR);
    qpp_start(qpp_addr);
    while(!qpp_done(qpp_addr));
    while(!qpp_ready(qpp_addr));

    // Begin Decryption
    qpp_set_control(qpp_addr, DECRYPT);
    qpp_set_size(qpp_addr, DAT_SIZE);
    qpp_set_src(qpp_addr, DST_ADDR);
    qpp_set_dst(qpp_addr, SRC_ADDR);
    qpp_start(qpp_addr);
    while(!qpp_done(qpp_addr));
    while(!qpp_ready(qpp_addr));

    for(int i = 0; i < DAT_SIZE; i++) {
        result[i] = (volatile unsigned)src_addr[i];
    }

    for(int i = 0; i < DAT_SIZE; i++) {
        printf("%08x\n", result[i]);
    }

    return 0;
}

