#include "QPP_HLS.h"

#include <cstring>
#include "Statistics.h"
#include <stdio.h>
#include <errno.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <iostream>

int errnum;
int qpp_fd;
int src_fd;
int dst_fd;
int seed_fd;
int perm_fd;
volatile uint32_t *qpp_addr;
volatile uint32_t *dst_addr;
volatile uint32_t *src_addr;
volatile uint32_t *seed_addr;
volatile uint32_t *perm_addr;

void write_reg(volatile uint32_t *addr, uint32_t offset, uint32_t value) {
    addr[offset >> 2] = value;
}

uint32_t read_reg(volatile uint32_t *addr, uint32_t offset) {
    return addr[offset >> 2];
}

int qpp_ready(volatile uint32_t *qpp_addr) {
    uint32_t Data = read_reg(qpp_addr, CTRL_REG);
    return !(Data & 0x1);
}

int qpp_done(volatile uint32_t *qpp_addr) {
    uint32_t Data = read_reg(qpp_addr, CTRL_REG);
    return (Data >> 1) & 0x1;
}

void qpp_set_size(volatile uint32_t *qpp_addr, uint32_t value) {
    write_reg(qpp_addr, SIZE_REG, value);
}

void qpp_set_dst(volatile uint32_t *qpp_addr, uint32_t value) {
    write_reg(qpp_addr, DST_REG, value);
}

void qpp_set_src(volatile uint32_t *qpp_addr, uint32_t value) {
    write_reg(qpp_addr, SRC_REG, value);
}

void qpp_start(volatile uint32_t *qpp_addr) {
    uint32_t Data = read_reg(qpp_addr, CTRL_REG) & 0x80;
    write_reg(qpp_addr, CTRL_REG, Data | 0x01);
}

void qpp_set_control(volatile uint32_t *qpp_addr, uint32_t value) {
    write_reg(qpp_addr, CONTROL_REG, value);
}

using namespace Xceed::Constants;

Xceed::QPP_HLS::QPP_HLS(uint8_t *in_seed) {

    #ifdef _TRACK_STATS
    Shell379::Utilities::TotalTiming<std::micro> totalTiming(&Statistics::setup_time);
    #endif

    qpp_fd = open("/dev/uio0", O_RDWR | O_SYNC);
	if (qpp_fd < 1) {
        handle_error("Could not open /dev/uio0");
	}

    /* mmap the qpp UIO device */
    qpp_addr = (volatile uint32_t *)mmap(NULL, QPP_MAP_SIZE, PROT_READ|PROT_WRITE, MAP_SHARED,
                    qpp_fd, 0);
    if (!qpp_addr) {
        handle_error("Could not mmap /dev/uio0");
    }

    src_fd = open("/dev/uio1", O_RDWR | O_SYNC);
	if (src_fd < 1) {
        handle_error("Could not open /dev/uio1");
	}

    dst_fd = open("/dev/uio2", O_RDWR | O_SYNC);
	if (dst_fd < 1) {
        handle_error("Could not open /dev/uio2");
	}

    seed_fd = open("/dev/uio3", O_RDWR | O_SYNC);
	if (seed_fd < 1) {
        handle_error("Could not open /dev/uio3");
	}

    perm_fd = open("/dev/uio4", O_RDWR | O_SYNC);
	if (perm_fd < 1) {
        handle_error("Could not open /dev/uio4");
	}

    /* mmap the reserved mem UIO device */
    src_addr = (volatile uint32_t *)mmap(NULL, SRC_MAP_SIZE, PROT_READ|PROT_WRITE, MAP_SHARED,
                    src_fd, 0);
    if (src_addr == (void*)-1) {
        errnum = errno;
        fprintf(stderr, "Value of errno: %d\n", errno);
        fprintf(stderr, "Error opening file: %s\n", strerror( errnum ));
        handle_error("Could not mmap /dev/uio1 src");
    }
    // int size_reg = 0x20;
    // qpp_addr[size_reg >> 2] = 100;

    dst_addr = (volatile uint32_t *)mmap(NULL, DST_MAP_SIZE, PROT_READ|PROT_WRITE, MAP_SHARED,
                    dst_fd, 0);
    if (dst_addr == (void*)-1) {
        errnum = errno;
        fprintf(stderr, "Value of errno: %d\n", errno);
        fprintf(stderr, "Error opening file: %s\n", strerror( errnum ));
        handle_error("Could not mmap /dev/uio2 dst");
    }

    seed_addr = (volatile uint32_t *)mmap(NULL, SEED_MAP_SIZE, PROT_READ|PROT_WRITE, MAP_SHARED,
                    seed_fd, 0);
    if (seed_addr == (void*)-1) {
        errnum = errno;
        fprintf(stderr, "Value of errno: %d\n", errno);
        fprintf(stderr, "Error opening file: %s\n", strerror( errnum ));
        handle_error("Could not mmap /dev/uio3 dst");
    }

    perm_addr = (volatile uint32_t *)mmap(NULL, PERM_MAP_SIZE, PROT_READ|PROT_WRITE, MAP_SHARED,
                    perm_fd, 0);
    if (perm_addr == (void*)-1) {
        errnum = errno;
        fprintf(stderr, "Value of errno: %d\n", errno);
        fprintf(stderr, "Error opening file: %s\n", strerror( errnum ));
        handle_error("Could not mmap /dev/uio4 dst");
    }

    bit_rep_plain_text = new uint8_t[block_size];
    bit_rep_cipher_text = new uint8_t[block_size];
    seed = new uint8_t[block_size];

    std::memset(bit_rep_plain_text, 0, block_size);
    std::memset(bit_rep_cipher_text, 0, block_size);

    std::memcpy(seed, in_seed, block_size);
    std::memset((uint8_t*)seed_addr, 0, block_size);
    for(int i = 0, right_offset = 3*8; i < 4; i++, right_offset -= 8) {
        *(seed_addr) |= (in_seed[i] << right_offset);
    }
    while(!qpp_ready(qpp_addr));
    // Load SEED
    qpp_set_control(qpp_addr, LD_SEED);
    qpp_set_size(qpp_addr, 1);
    qpp_set_src(qpp_addr, SEED_ADDR);
    qpp_start(qpp_addr);
    while(!qpp_done(qpp_addr));
    while(!qpp_ready(qpp_addr));


    generateMatrix();
    generatingDispatchSequence();


    while(!qpp_ready(qpp_addr));
    // Load Perms
    qpp_set_control(qpp_addr, LD_PERMS);
    qpp_set_size(qpp_addr, 4*n*M);
    qpp_set_src(qpp_addr, PERMS_ADDR); 
    qpp_start(qpp_addr);
    while(!qpp_done(qpp_addr));
    while(!qpp_ready(qpp_addr));

    generateInvMatrix();

    while(!qpp_ready(qpp_addr));
    // Load Transpose Perms
    qpp_set_control(qpp_addr, LD_T_PERMS);
    qpp_set_size(qpp_addr, 4*n*M);
    qpp_set_src(qpp_addr, PERMS_ADDR);
    qpp_start(qpp_addr);
    while(!qpp_done(qpp_addr));
    while(!qpp_ready(qpp_addr));
}

uint8_t* Xceed::QPP_HLS::encrypt() {

    #ifdef _TRACK_STATS
    Shell379::Utilities::TotalTiming<std::micro> totalTiming(&Statistics::encrypt_time);
    #endif

    while(!qpp_ready(qpp_addr));
    // Begin Encryption
    qpp_set_control(qpp_addr, ENCRYPT);
    qpp_set_size(qpp_addr, Xceed::Constants::block_size/4);
    qpp_set_src(qpp_addr, SRC_ADDR);
    qpp_set_dst(qpp_addr, DST_ADDR);
    qpp_start(qpp_addr);
    while(!qpp_done(qpp_addr));
    while(!qpp_ready(qpp_addr));

    uint8_t* result = new uint8_t[block_size];
    std::cout << "Encryption complete. Ciphertext = " << std::endl;
    std::memset(result, 0, block_size);
    for(int i = 0; i < block_size/4; i++) {
        int moving_mask = 0xFF000000;
        for(int j = 0, right_offset = 3*8; j < 4; j++, right_offset -= 8) {
            result[(i*4)+j] = (*(dst_addr+i) & moving_mask) >> right_offset;
            printf("%02x", result[(i*4)+j]);
            moving_mask = moving_mask >> 8;
        }
    }
    std::cout << std::endl;
    std::cout << std::endl;
    return result;
}


uint8_t* Xceed::QPP_HLS::decrypt() {

    #ifdef _TRACK_STATS
    Shell379::Utilities::TotalTiming<std::micro> totalTiming(&Statistics::decrypt_time);
    #endif
    
    while(!qpp_ready(qpp_addr));
    // Load SEED
    qpp_set_control(qpp_addr, LD_SEED);
    qpp_set_size(qpp_addr, 1);
    qpp_set_src(qpp_addr, SEED_ADDR);
    qpp_start(qpp_addr);
    while(!qpp_done(qpp_addr));
    while(!qpp_ready(qpp_addr));

    qpp_set_control(qpp_addr, DECRYPT);
    qpp_set_size(qpp_addr, Xceed::Constants::block_size/4);
    qpp_set_src(qpp_addr, SRC_ADDR);
    qpp_set_dst(qpp_addr, DST_ADDR);
    qpp_start(qpp_addr);
    while(!qpp_done(qpp_addr));
    while(!qpp_ready(qpp_addr));

    uint8_t* result = new uint8_t[block_size];
    std::memset(result, 0, block_size);
    std::cout << "Decryption complete. Plaintext = " << std::endl;
    for(int i = 0; i < block_size/4; i++) {
        int moving_mask = 0xFF000000;
        for(int j = 0, right_offset = 3*8; j < 4; j++, right_offset -= 8) {
            result[(i*4)+j] = (*(dst_addr+i) & moving_mask) >> right_offset;
            printf("%02x", result[(i*4)+j]);
            moving_mask = moving_mask >> 8;
        }
    }
    std::cout << std::endl;
    std::cout << std::endl;
    return result;
}

void Xceed::QPP_HLS::generateMatrix() {

    srand(seed[0]);
    std::vector<uint8_t> reference(mat_size);
    for (int i = 0; i < mat_size; i++) {
        reference[i] = i;
    }
    std::memset((uint8_t*)perm_addr, 0, block_size);
	int block_num = 0;
	int block_pos = 0;
	int right_offset = 15*2;
    for (int k = 0; k < M; k++) {
        for (int i = mat_size - 1; i > 0; i--) {
            int j = random() % (i + 1);
            {
                uint8_t temp = reference[i];
                reference[i] = reference[j];
                reference[j] = temp;
            }
        }
        block_pos++;
        for(int l = 0; l < mat_size; l++, right_offset -= 2) {
            *(perm_addr+(block_num)) |= (reference[l] << right_offset);
            if(block_pos == 16) {
                block_num++;
                right_offset = 15*2;
                block_pos = 0;
            }
        }
        pmat_list.emplace_back(reference);

    }

}

void Xceed::QPP_HLS::generateInvMatrix() {
    std::memset((uint8_t*)perm_addr, 0, block_size);
    int block_num = 0;
	int block_pos = 0;
	int right_offset = 15*2;
    for (int i = 0; i < M; i++) {
        uint8_t* temp = new uint8_t[mat_size];
        for (int row = 0; row < mat_size; row++) {
            uint8_t col = pmat_list[i][row];
            temp[col] = row;
        }
        block_pos++;
        for(int l = 0; l < mat_size; l++, right_offset -= 2) {
            *(perm_addr+(block_num)) |= (temp[l] << right_offset);
            if(block_pos == 16) {
                block_num++;
                right_offset = 15*2;
                block_pos = 0;
            }
        }

        inv_pmat_list.emplace_back(temp, temp + mat_size);
        delete[] temp;
    }

}

int Xceed::QPP_HLS::getStringLength() {
    return text_size;
}

void Xceed::QPP_HLS::setPlainText(std::string &plain_text) {

    bit_rep_plain_text = new uint8_t[plain_text.size()];
    bit_rep_cipher_text = new uint8_t[plain_text.size()];

    std::memcpy(bit_rep_plain_text, plain_text.c_str(), plain_text.size());
    this->text_size = (int)plain_text.size();
    for(int i = 0; i < this->text_size; i++) {
        std::cout << (uint32_t)bit_rep_plain_text[i];
    }
}

void Xceed::QPP_HLS::setPlainText(const uint8_t *plain_text, int in_text_size) {
    std::memset((uint8_t*)src_addr, 0, block_size);

    std::cout << "Text Size : " << in_text_size << std::endl;
    for(int i = 0; i < in_text_size; i++) {
        std::cout << plain_text[i];
    }
    std::cout << std::endl;
    for(int i = 0; i < in_text_size; i++) {
        printf("%02x", plain_text[i]);
    }
    std::cout << std::endl;

    int i;
    for(i = 0; i < in_text_size/4; i++) {
        for(int j = 0, right_offset = 3*8; j < 4; j++, right_offset -= 8) {
            *(src_addr+i) |= (plain_text[(i*4)+j] << right_offset);
        }
    }

    for(int k = 0, right_offset = 3*8; k < (in_text_size % 4); k++, right_offset -= 8) {
        *(src_addr+i) |= (plain_text[(i*4)+k] << right_offset);    
    }


    this->text_size = in_text_size;
}

void Xceed::QPP_HLS::setSeed(const uint8_t* in_seed) {
    std::memcpy(seed, in_seed, block_size);
}

void Xceed::QPP_HLS::setCipherText(const uint8_t *cipher_text, int in_text_size) {

    std::memset((uint8_t*)src_addr, 0, block_size);
    for(int i = 0; i < block_size/4; i++) {
        for(int j = 0, right_offset = 3*8; j < 4; j++, right_offset -= 8) {
            *(src_addr+i) |= (cipher_text[(i*4)+j] << right_offset);
        }
    }
    this->text_size = in_text_size;
}

void Xceed::QPP_HLS::generatingDispatchSequence() {

    srand(seed[1]);
    for (int i = 0; i < block_size; i++) {
        dispatch_sequence.push_back((uint8_t)rand() % M);
    }
}

Xceed::QPP_HLS::~QPP_HLS() {
    delete[] bit_rep_plain_text;
    delete[] bit_rep_cipher_text;
    delete[] seed;

    #ifdef _TRACK_STATS
    std::vector<double> summary;
    summary.push_back(Statistics::encrypt_time);
    summary.push_back(Statistics::decrypt_time);
    summary.push_back(Statistics::setup_time);
    std::string path = "../stat/stat.txt";
    ProdCon::IOManagement ioManagement(path);
    ioManagement.printSummary(summary);
    ioManagement.release();

    std::cout << "Logged" << std::endl;
    #endif
}