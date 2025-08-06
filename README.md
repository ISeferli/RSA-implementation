# RSA (Rivest-Shamir-Adleman) Algorithm

This project was created for a course in my Electrical and Computer Engineering (ECE) studies at [TUC](https://www.tuc.gr/) (Technical University of Crete). It implements the RSA algorithm for encryption and decryption, relying on a pair of mathematically linked keys: a public key for encryption and a private key for decryption. 

---

## 🎓 Project Info

- **Course**: Cyber Security
- **Institution**: Technical University of Crete (TUC)
- **Department**: School of Electrical and Computer Engineering
- **Semester**: Fall 2020
- **Project Type**: Individual assignment
- **Language**: C
- **Platform**: Linux

---

## ✨ Features

- **Keys generation**: Generates both random public and private keys by providing the argument *-e*
- **File Encryption**: Encrypts an input file using either the public or private key provided by the specific argument in the command.
- **File Decryprion**: Decrypts an input file using either the public or private key (according to which key was used for encryption) provided by the specific argument in the command.

---

## 🧱 File Structure

| File Name           | Description                                              |
|---------------------|----------------------------------------------------------|
| **main.c**  |  Main program where the RSA algorithm is tested |
| **rsa.c**  | Implementation of all the function used for RSA algorithm |
| **rsa.h**           | C function declarations for RSA |
| **utils.c**  |  Implementation of helping tools |
| **utils.h**           | C function declarations for helping functions |
| **Makefile**           | Makefile of the program |

---

## 🛠 Build and Run

### Dependencies

This project requires:
- **GCC** for compilation

Ensure that the necessary libraries are installed:
```bash
sudo apt-get install libssl-dev
```

### Build

To compile and build the project:
1. Clone this repository to your local machine.

2. Use the Makefile provided to build the project.
   ```bash
   make [all]
   ```

---

## Usage

Once the project is built, you can use it as follows:

1. Generate the key files.

    ```bash
    ./assign_3 -g
    ```

2. Encrypt any file given (name provided as first argument) by the name of the file that the encryption will be saved in and the public key file.

    ```bash
    ./assign_3 -i [input_file_name].txt -o [output_file_name].txt -k public.key -e
    ```


3. Decrypt any file given by name followed by the name of the file that the output will be saved in and private key file.

    ```bash
    ./assign_3 -i [input_file_name].txt -o [output_file_name].txt -k public.key -d
    ```
 
4. Clean up the generated files

    You can clean up all compiled files by running:

    ```bash
    make clean
    ```

---



