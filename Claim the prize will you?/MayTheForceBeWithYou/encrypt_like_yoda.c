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
        printf("Usage: %s <input.jpg> <output.yoda>\n", argv[0]);
        return 1;
    }

    const unsigned char key[] = "Yoda"; // 4-byte key
    const size_t key_len = strlen((const char *)key);

    FILE *in = fopen(argv[1], "rb");
    if (!in) { perror("fopen input"); return 1; }

    fseek(in, 0, SEEK_END);
    long size = ftell(in);
    rewind(in);

    unsigned char *data = malloc(size);
    if (!data) { perror("malloc"); fclose(in); return 1; }

    fread(data, 1, size, in);
    fclose(in);

    // Step 1: Swap 4-byte blocks [A B C D] -> [C D A B]
    for (long i = 0; i + 3 < size; i += 4) {
        swap(&data[i+0], &data[i+2]);
        swap(&data[i+1], &data[i+3]);
    }

    // Step 2: Swap every adjacent byte [x y] -> [y x]
    for (long i = 0; i + 1 < size; i += 2) {
        swap(&data[i], &data[i+1]);
    }
    // Step 3: XOR each byte with repeating key
    for (long i = 0; i < size; i++) {
        data[i] ^= key[i % key_len];
    }

    FILE *out = fopen(argv[2], "wb");
    if (!out) { perror("fopen output"); free(data); return 1; }

    fwrite(data, 1, size, out);
    fclose(out);
    free(data);

    printf("[+] Obfuscated file written to: %s using XOR key \"Yoda\"\n", argv[2]);
    return 0;
}

