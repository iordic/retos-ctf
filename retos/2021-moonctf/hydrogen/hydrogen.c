#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

/* Titulo:      hydrogen
 * Descripción: Your name is not usually a good password :(
 * Categoria:   reversing
 * Puntuación:  250
 * Flag:        moonCTF{0661de560d0fbe73ecf5d894e24f0982}
 * Autor:       iordic
 * Pista:       break the flow!
 * Solución:    Usar debugger para obligar a entrar en el if que imprime la pista,
 *              también se puede parchear sin debugger y luego lanzar, de este modo
 *              nos devuelve la pista. Se puede apreciar que esta está con rotación 13.
 *              Decodificamos la url y en la noticia obtenemos la contraseña.
 * 
 */

void show_hint();
uint8_t genkey(char *password);
void encrypt(char *ctext, uint8_t key);

int main() {
    char password[16];
    int want_hint = 0;
    if (want_hint) {
        printf("Good, here's your hint: ");
        show_hint();
    } else {
        printf("you don't want a hint? Uhm... OK\n");
    }
    //char c_flag[] = "moonCTF{0661de560d0fbe73ecf5d894e24f0982}";
    char c_flag[] = "\x60\x62\x62\x63\x4e\x59\x4b\x76\x3d\x3b\x3b" \
                    "\x3c\x69\x68\x38\x3b\x3d\x69\x3d\x6b\x6f\x68" \
                    "\x3a\x3e\x68\x6e\x6b\x38\x69\x35\x34\x39\x68" \
                    "\x3f\x39\x6b\x3d\x34\x35\x3f\x70";
    int size = strlen(c_flag);
    printf("What's the password?: ");
    scanf("%s", password);
    if (check_password(password)) {
        uint8_t key =  genkey(password);
        encrypt(c_flag, key);
        printf("Oh, well... Here's your flag: %s\n", c_flag);        
    } else {
        printf("Sorry, bad password :(\n");
    }
    return 0;
}

/* Un poco de complejidad y ruido adicional, ni siquiera hace falta entrar a mirar */
int check_password(char *password) {
    char fake[] = "P@ssw0rd";   // :)
    if (!strcmp(fake, password)) {
        printf("Good try xD\n");
    }
    int size = strlen(password);
    int result = 0;
    for (int i = 0; i < size; i++) {
        result += password[i];
    }
    result += size;
    if (result == 1257)
        return 1;
    return 0;
}

void show_hint() {
    /* Algoritmo usado:
     *   1. rot-13 de: https://thehackernews.com/2021/03/solarwinds-blame-intern-for-weak.html
     *      char hint[] = "uggcf://gurunpxrearjf.pbz/2021/03/fbynejvaqf-oynzr-vagrea-sbe-jrnx.ugzy";
     *   2. for i in rango(0, tamaño(hint)) => xor(hint[i], i) 
     */
    char hint[] = "\x75\x66\x65\x60\x62\x3f\x29\x28\x6f\x7c\x78\x7e\x62\x7d" \
                  "\x76\x7d\x75\x70\x60\x79\x72\x3b\x66\x75\x62\x36\x28\x2b" \
                  "\x2e\x2c\x31\x2f\x13\x0e\x44\x41\x5d\x4b\x43\x4d\x5e\x48" \
                  "\x5b\x4d\x01\x42\x57\x41\x4a\x43\x1f\x45\x55\x52\x44\x52" \
                  "\x59\x14\x49\x59\x59\x10\x54\x4d\x2e\x39\x6c\x36\x23\x3f" \
                  "\x3f";

    int h_size = strlen(hint);
    for (int i = 0; i < h_size; i++) {
        hint[i] ^= i;
    }
    printf("%s\n", hint);
}

/* XOR de cada letra de la contraseña 'solarwinds123' */
uint8_t genkey(char *password) {
    uint8_t key = 0;
    for (int i = 0; i < strlen(password); i++) {
        key ^= password[i];
    }
}

/* xor de cada letra con la clave generada */
void encrypt(char *ctext, uint8_t key) {
    int size = strlen(ctext);
    for (int i = 0; i < size; i++) {
        ctext[i] = ctext[i] ^ key;
    }
}
