#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include "encrypt.h"

void test_encryption() {
    const char *test_message = "La liberté consiste à pouvoir faire tout ce qui ne nuit pas à autrui.";
    uint8_t encrypted_data[MAX_PAYLOAD_SIZE];
    size_t encrypted_len;

    char decrypted_data[MAX_PAYLOAD_SIZE];
    size_t decrypted_len;

    printf("\n[Encryption test] Original Message: %s\n", test_message);

    if (encrypt_message(test_message, strlen(test_message), encrypted_data, &encrypted_len) == 0) {
        printf("[Encryption test] Encrypted Data:\n");
        for (size_t i = 0; i < encrypted_len; i++) {
            printf("%02x ", encrypted_data[i]);
        }
        printf("\n");
    } else {
        printf("[Encryption test] Encryption failed\n");
        return;
    }

    if (decrypt_message(encrypted_data, encrypted_len, decrypted_data, &decrypted_len) == 0) {
        decrypted_data[decrypted_len] = '\0'; // \0 end the message
        printf("[Encryption test] Decrypted Message: %s\n", decrypted_data);
    } else {
        printf("[Encryption test] Decryption failed\n");
    }
}

int main() {
    test_encryption();
    return 0;
}