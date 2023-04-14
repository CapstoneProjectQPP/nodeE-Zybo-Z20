#include "QPP.h"

#include <cstring>
#include "Statistics.h"


using namespace Xceed::Constants;

Xceed::QPP::QPP(uint8_t *in_seed) {

    #ifdef _TRACK_STATS
    Shell379::Utilities::TotalTiming<std::micro> totalTiming(&Statistics::setup_time);
    #endif

    bit_rep_plain_text = new uint8_t[block_size];
    bit_rep_cipher_text = new uint8_t[block_size];
    seed = new uint8_t[block_size];

    memset(bit_rep_plain_text, 0, block_size);
    memset(bit_rep_cipher_text, 0, block_size);
    std::memcpy(seed, in_seed, block_size);

    generateMatrix();
    generateInvMatrix();
    generatingDispatchSequence();
}

uint8_t* Xceed::QPP::encrypt() {

    #ifdef _TRACK_STATS
    Shell379::Utilities::TotalTiming<std::micro> totalTiming(&Statistics::encrypt_time);
    #endif

    for (int i = 0; i < block_size; i++) {
        bit_rep_plain_text[i] ^= seed[i];
    }

    for (int i = 0, byte_ptr = 0; i < block_size; i++, byte_ptr++) {

        for (int right_side_offset = 8 - n, j = 0; right_side_offset >= 0; right_side_offset -= n, j++) {

            uint8_t cipher_byte_mask = 0;
            int moving_mask = mask >> (n * j);
            int dispatch_index = ((i * (8/n)) + j) % block_size;
            dispatch_index = dispatch_sequence[dispatch_index];

            // Extracting
            int basis_select = (*(bit_rep_plain_text + byte_ptr) & moving_mask) >> right_side_offset;

            // Writing byte
            uint8_t acquired_value = pmat_list[dispatch_index][basis_select];
            cipher_byte_mask = (uint8_t) acquired_value << right_side_offset;

            // Writing cipher
            *(bit_rep_cipher_text + byte_ptr) &= ~moving_mask;
            *(bit_rep_cipher_text + byte_ptr) |= (cipher_byte_mask & moving_mask);

        }
    }

    uint8_t* result = new uint8_t[block_size];
    std::memcpy(result, bit_rep_cipher_text, block_size);
    return result;
}


uint8_t* Xceed::QPP::decrypt() {

    #ifdef _TRACK_STATS
    Shell379::Utilities::TotalTiming<std::micro> totalTiming(&Statistics::decrypt_time);
    #endif
    for (int i = 0, byte_ptr = 0; i < block_size; i++, byte_ptr++) {

        for (int right_side_offset = 8 - n, j = 0; right_side_offset >= 0; right_side_offset -= n, j++) {

            uint8_t plain_text_byte_mask = 0;
            int moving_mask = mask >> (n * j);
            int dispatch_index = ((i * (8/n)) + j) % M;
            dispatch_index = dispatch_sequence[dispatch_index];

            // Extracting
            int basis_select = (*(bit_rep_cipher_text + byte_ptr) & moving_mask) >> right_side_offset;

            // Writing byte
            uint8_t acquired_value = inv_pmat_list[dispatch_index][basis_select];
            plain_text_byte_mask = (uint8_t) acquired_value << right_side_offset;

            // Writing cipher
            *(bit_rep_plain_text + byte_ptr) &= ~moving_mask;
            *(bit_rep_plain_text + byte_ptr) |= (plain_text_byte_mask & moving_mask);

        }
    }

    for (int i = 0; i < block_size; i++) {
        bit_rep_plain_text[i] ^= seed[i];
    }

    uint8_t* result = new uint8_t[text_size];
    std::memcpy(result, bit_rep_plain_text, text_size);
    return result;
}

void Xceed::QPP::generateMatrix() {

    // uint8_t m1[mat_size] = {1, 2, 0, 3};
    // uint8_t m2[mat_size] = {2, 1, 0, 3};
    // uint8_t m3[mat_size] = {1, 3, 2, 0};
    // uint8_t m4[mat_size] = {0, 3, 2, 1};
    // uint8_t m5[mat_size] = {0, 3, 1, 2};

    // pmat_list.emplace_back(std::vector<uint8_t>({1, 2, 0, 3}));
    // pmat_list.emplace_back(std::vector<uint8_t>({2, 1, 0, 3}));
    // pmat_list.emplace_back(std::vector<uint8_t>({1, 3, 2, 0}));
    // pmat_list.emplace_back(std::vector<uint8_t>({0, 3, 2, 1}));
    // pmat_list.emplace_back(std::vector<uint8_t>({0, 3, 1, 2}));

    srand(seed[0]);
    std::vector<uint8_t> reference(mat_size);
    for (int i = 0; i < mat_size; i++) {
        reference[i] = i;
    }

    for (int k = 0; k < M; k++) {
        for (int i = mat_size - 1; i > 0; i--) {
            int j = random() % (i + 1);
            {
                uint8_t temp = reference[i];
                reference[i] = reference[j];
                reference[j] = temp;
            }
        }

        pmat_list.emplace_back(reference);
    }
}

void Xceed::QPP::generateInvMatrix() {
    for (int i = 0; i < M; i++) {
        uint8_t* temp = new uint8_t[mat_size];
        for (int row = 0; row < mat_size; row++) {
            uint8_t col = pmat_list[i][row];
            temp[col] = row;
        }
        inv_pmat_list.emplace_back(temp, temp + mat_size);
        delete[] temp;
    }
}

int Xceed::QPP::getStringLength() {
    return text_size;
}

void Xceed::QPP::setPlainText(std::string &plain_text) {

    bit_rep_plain_text = new uint8_t[plain_text.size()];
    bit_rep_cipher_text = new uint8_t[plain_text.size()];

    std::memcpy(bit_rep_plain_text, plain_text.c_str(), plain_text.size());
    this->text_size = (int)plain_text.size();
}

void Xceed::QPP::setPlainText(const uint8_t *plain_text, int in_text_size) {

    std::memcpy(bit_rep_plain_text, plain_text, in_text_size);
    this->text_size = in_text_size;

}

void Xceed::QPP::setSeed(const uint8_t* in_seed, int seed_size) {

    text_size = seed_size;
    std::memcpy(seed, in_seed, seed_size);
}

void Xceed::QPP::setCipherText(const uint8_t *cipher_text, int in_text_size) {

    std::memcpy(bit_rep_cipher_text, cipher_text, in_text_size);
    this->text_size = in_text_size;
}

void Xceed::QPP::generatingDispatchSequence() {

    srand(seed[1]);
    for (int i = 0; i < block_size; i++) {
        dispatch_sequence.push_back((uint8_t)rand() % M);
    }
}

Xceed::QPP::~QPP() {
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


