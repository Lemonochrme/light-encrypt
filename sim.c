#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include "aes.h"

#define BLOCK_SIZE 16
#define PACKET_SIZE 256
// #define DEBUG

// Clé partagée entre A et B
const uint8_t shared_key[BLOCK_SIZE] = { 0x2b, 0x7e, 0x15, 0x16, 0x28, 0xae, 0xd2, 0xa6, 0xab, 0xf7, 0x4d, 0x4d, 0x9f, 0x24, 0x3b, 0x7a };

// IV fixe partagé entre A et B
const uint8_t shared_iv[BLOCK_SIZE] = { 0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0a, 0x0b, 0x0c, 0x0d, 0x0e, 0x0f };

typedef struct {
    uint8_t iv[16];
    uint16_t node_id;
    uint32_t timestamp;
    uint8_t battery_level;
    uint8_t status;
    char reduced_fft[128];
    char data[104];
} payload_t;

void print_hex(const char *label, const uint8_t *data, size_t len) {
    printf("%s", label);
    for (size_t i = 0; i < len; i++) {
        printf("%02x", data[i]);
    }
    printf("\n");
}

void node_sender(const char *message, uint8_t *encrypted_message, size_t *encrypted_len) {
    struct AES_ctx ctx;
    size_t message_len = strlen(message);
    uint8_t padded_message[PACKET_SIZE];

    // Copie le message et ajoute des zéros pour remplir jusqu'à PACKET_SIZE
    memset(padded_message, 0, PACKET_SIZE);
    memcpy(padded_message, message, message_len);

    AES_init_ctx_iv(&ctx, shared_key, shared_iv);
    AES_CBC_encrypt_buffer(&ctx, padded_message, PACKET_SIZE);
    memcpy(encrypted_message, padded_message, PACKET_SIZE);

    *encrypted_len = PACKET_SIZE;

#ifdef DEBUG
    printf("[Node A] Encrypted message: ");
    print_hex("", encrypted_message, *encrypted_len);
#endif
}

void node_receiver(const uint8_t *encrypted_message, size_t encrypted_len) {
#ifdef DEBUG
    printf("[Node B] Received encrypted message: ");
    print_hex("", encrypted_message, encrypted_len);
#endif

    struct AES_ctx ctx;
    uint8_t decrypted_message[PACKET_SIZE];

    AES_init_ctx_iv(&ctx, shared_key, shared_iv);
    memcpy(decrypted_message, encrypted_message, PACKET_SIZE);
    AES_CBC_decrypt_buffer(&ctx, decrypted_message, PACKET_SIZE);

#ifdef DEBUG
    printf("[Node B] Decrypted message: ");
    print_hex("", decrypted_message, PACKET_SIZE);
#endif

    // Suppression des zéros ajoutés
    size_t message_len = strnlen((char *)decrypted_message, PACKET_SIZE);

    printf("[Node B] Decrypted message: %.*s\n", (int)message_len, decrypted_message);
}

int main() {
    const char *message = "Les représentants du peuple français, constitués en Assemblée nationale, considérant que l'ignorance, l'oubli ou le mépris des droits de l'homme sont les seules causes des malheurs publics et de la corruption des gouvernements, ont résolu d'exposer.";
    uint8_t encrypted_message[PACKET_SIZE];
    size_t encrypted_len;

    // Sizeof message
    printf("[Main] Message size: %lu\n", strlen(message));

    printf("[Main] Message to send: %s\n", message);

    node_sender(message, encrypted_message, &encrypted_len);
    node_receiver(encrypted_message, encrypted_len);

    return 0;
}
