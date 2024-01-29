# Toy_Kyber: Post-Quantum Cryptography

This repository contains an implementation of a toy-version of the Kyber Post-Quantum Public-Key Cryptosystem using polynomials from \( Z_{97}/(X^4+1) \). The cryptosystem is implemented in C and uses naive polynomial multiplication.

## Features
- Implementation of the Kyber toy cryptosystem.
- Support for polynomial multiplication in \( Z_{97}[X]/(X^4+1) \).
- Key generation, encryption, and decryption functionalities.

## Contents
- `toy.h`: Header file containing macro definitions for the toy cryptosystem.
- `toy.c`: Source file containing the implementation of the toy cryptosystem.
- `README.md`: README file providing an overview of the repository and instructions for usage.

## Implementation Details
### Helper Functions
- `toy_polmul_naive`: Implements polynomial multiplication in \( Z_{97}[X]/(X^4+1) \).
- Other helper functions for key generation, encryption, and decryption as specified in the Kyber toy cryptosystem.

### Main API
- `toy_gen`: Generates public and private keys.
- `toy_enc`: Encrypts a plaintext message.
- `toy_dec`: Decrypts a ciphertext message.

## Usage
1. Clone the repository: `git clone https://github.com/your_username/Toy_Kyber.git`.
2. Compile the source files: `gcc toy.c -o toy`.
3. Run the executable: `./toy`.

## References
- Kyber: A CCA-Secure Module-Lattice-Based KEM ([Paper](https://eprint.iacr.org/2017/634.pdf)).
- [Assignment 2](https://drive.google.com/file/d/1N3xhvvjZjx8e1Q2GY4ly4aqq2RJGiase/view)
