# eZ80 and Agon Light 2 cryptographic implementations
In this repository, you'll find some cryptographic implementations for oficial algorithms (both for hashing and encryption). The algorithms are the following one:

1. For hashing: MD5, SHA-1, SHA-224, SHA-256, SHA-384 and SHA-512.
2. for encryption: RC4, AES-128, AES-192, AES-256 and ChaCha20.

Consider that these algorithms are implemented in C language for CEDev/AgDev toolchains (oriented for Agon Light 2); they're not optimal in execution time or code size, but works with different input sizes of a plaintext, key and nonce (where applicable).

Clearly, these implementations could be improved (both in size and execution speed), but can be considered as a base for future developments.
