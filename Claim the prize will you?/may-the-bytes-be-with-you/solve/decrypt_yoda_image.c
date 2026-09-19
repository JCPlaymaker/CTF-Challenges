#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void swap(unsigned char *a, unsigned char *b) {
    unsigned char tmp = *a;
    *a = *b;
    *b = tmp;
}

int main(int argc, char *argv[]) {
    if (argc != 3) {
        printf("Usage: %s <scrambled.yoda> <output.jpg>\n", argv[0]);
        return 1;
    }

    const unsigned char key[] = "Yoda";
    const size_t key_len = strlen((const char *)key);

    FILE *in = fopen(argv[1], "rb");
    if (!in) {
        perror("fopen input");
        return 1;
    }

    if (fseek(in, 0, SEEK_END) != 0) {
        perror("fseek");
        fclose(in);
        return 1;
    }

    long size = ftell(in);

    if (size < 0) {
        perror("ftell");
        fclose(in);
        return 1;
    }

    rewind(in);

    unsigned char *data = malloc((size_t)size);

    if (!data) {
        perror("malloc");
        fclose(in);
        return 1;
    }

    size_t bytes_read = fread(data, 1, (size_t)size, in);
    fclose(in);

    if (bytes_read != (size_t)size) {
        fprintf(stderr, "Error: could not read entire input file.\n");
        free(data);
        return 1;
    }

    /*
     * Encryption:
     *
     * 1. Adjacent swap
     * 2. 4-byte block reorder
     * 3. XOR with "Yoda"
     *
     * Therefore decryption must happen in reverse.
     */

    // Step 1: Undo XOR
    for (long i = 0; i < size; i++) {
        data[i] ^= key[i % key_len];
    }

    // Step 2: Undo 4-byte block reorder
    // [C D A B] -> [A B C D]
    for (long i = 0; i + 3 < size; i += 4) {
        swap(&data[i],     &data[i + 2]);
        swap(&data[i + 1], &data[i + 3]);
    }

    // Step 3: Undo adjacent byte swaps
    // [B A D C] -> [A B C D]
    for (long i = 0; i + 1 < size; i += 2) {
        swap(&data[i], &data[i + 1]);
    }

    FILE *out = fopen(argv[2], "wb");

    if (!out) {
        perror("fopen output");
        free(data);
        return 1;
    }

    size_t bytes_written = fwrite(data, 1, (size_t)size, out);
    fclose(out);
    free(data);

    if (bytes_written != (size_t)size) {
        fprintf(stderr, "Error: could not write entire output file.\n");
        return 1;
    }

    printf("[+] Image restored: %s\n", argv[2]);

    return 0;
}