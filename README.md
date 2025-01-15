# light-encrypt
Light low overhead pre-shared key AES encryption for embedded systems based on [tiny-AES-c](https://github.com/kokke/tiny-AES-c). Optimized for 256 bytes LoRa packet size.

# Compile

```c
gcc sim.c tiny-AES-c/aes.c -I./tiny-AES-c
```

# Run

```bash
./a.out
```
