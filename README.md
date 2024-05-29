# RiccioWare
Simple System of Encryption and Decryption file

## Installation
1. Prerequisites
Before compiling Riccioware, ensure you have the OpenSSL library installed on your system. You can install it using the package manager of your operating system. Below are instructions for some common platforms:

- MacOS
```bash
brew install openssl
```
- Ubuntu/Debian
```bash
sudo apt-get install libssl-dev
```

2. Replace the path of the files that you wanna encrypt and decrypt:

- File <b>main.c</b>

Replace
```c
const char *home_directory = getenv("HOME");
```
With the your target env

Replace
```c
snprintf(desktop_directory, sizeof(desktop_directory), "%s/Desktop", home_directory);
```
With your target directory

Replace 
```c
if (strcmp(file->d_name, "prova") == 0)
```
With your target folder

3. Clone the repo
```bash
git clone https://github.com/your_username/riccioware.git
```

4. Navigate to the cloned directory
```bash
cd riccioware
```

5. Compile the 2 files
```bash
gcc -o riccioware riccioware.c -lcrypto #replace with the real path of your library
```

6. Execute the encryption files
```bash
./main
./decrypt
```