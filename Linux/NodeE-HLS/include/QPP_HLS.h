#pragma once

#include "string"
#include "vector"
#include "array"

#include "Constants.h"
#include "Timing.h"
#include "IOManagement.h"

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
#define PERMS_ADDR      0x3C000000

#define QPP_MAP_SIZE    0x10000
#define SRC_MAP_SIZE    0x04000000
#define DST_MAP_SIZE    0x04000000
#define SEED_MAP_SIZE   0x04000000
#define PERM_MAP_SIZE   0x04000000

#define handle_error(msg) \
    do { perror(msg); exit(-1); } while (0)

void write_reg(volatile unsigned *addr, unsigned offset, unsigned value);
unsigned read_reg(volatile unsigned *addr, unsigned offset);
int qpp_ready(volatile unsigned *qpp_addr);
int qpp_done(volatile unsigned *qpp_addr);
void qpp_set_size(volatile unsigned *qpp_addr, unsigned value);
void qpp_set_dst(volatile unsigned *qpp_addr, unsigned value);
void qpp_set_src(volatile unsigned *qpp_addr, unsigned value);
void qpp_start(volatile unsigned *qpp_addr);
void qpp_set_control(volatile unsigned *qpp_addr, unsigned value);


namespace Xceed {

    using namespace Constants;

    class QPP_HLS {
    public:
        QPP_HLS(uint8_t *in_seed);

        ~QPP_HLS();

        uint8_t* encrypt();
        uint8_t* decrypt();

        void setPlainText(const uint8_t *plain_text, int in_text_size);
        void setPlainText(std::string &plain_text);
        void setCipherText(const uint8_t* cipher_text, int in_text_size);

        void setSeed(const uint8_t* in_seed);

        int getStringLength();

    protected:
        void generateMatrix();
        void generateInvMatrix();
        void generatingDispatchSequence();

    private:
        int text_size;

        std::vector<std::vector<uint8_t>> pmat_list;
        std::vector<std::vector<uint8_t>> inv_pmat_list;
        std::vector<uint8_t> dispatch_sequence;

        uint8_t* bit_rep_plain_text;
        uint8_t* bit_rep_cipher_text;
        uint8_t* seed;
    };
}
