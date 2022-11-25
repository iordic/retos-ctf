#include <stdio.h>
#include <stdlib.h>

/* Titulo:      whack-a-mole
 * Descripción: smash smash smash! Bonk the stack
 * Categoria:   reversing
 * Puntuación:  100
 * Flag:        moonCTF{02ac91378c50b043426347a065ef24f5}
 * Autor:       iordic
 * Pista:       fun & profit
 * Solución:    Desbordar el buffer que almacena la contraseña para modificar la variable
 *              "want_flag" y que entre en el if, imprimiendo por pantalla el flag.
 *              Ejemplo con python: python3 -c "print('a' * 100)" | ./whack-a-mole
 *              (si no quieres que salga violación de segmento ajusta el tamaño)
 * 
 * Compilar sin protección de pila: gcc -o whack-a-mole whack-a-mole.c -f-no-stack-protector
 * 
 */
void give_flag() {
    // md5 generado con: echo "Smashing The Stack For Fun And Profit" | md5sum
    // flag: moonCTF{02ac91378c50b043426347a065ef24f5}
    
    // Algoritmo para codificarla usado: se ha invertido la cadena y se le ha restado 'a' a cada caracter
    char flag[] = "\x1c\xd4\x05\xd3\xd1\x05\x04\xd4\xd5\xcf\x00\xd6\xd3\xd2\xd5\xd1\xd3\xd2" \
                  "\xd3\xcf\x01\xcf\xd4\x02\xd7\xd6\xd2\xd0\xd8\x02\x00\xd1\xcf\x1a\xe5\xf3" \
                  "\xe2\x0d\x0e\x0e\x0c\x9f";

    for (int i = 0; i < sizeof(flag); i++) {
        flag[i] = flag[i] + 'a';
    }
    printf("\nGood job! here u go: ");
    
    // Ahora hay que darle la vuelta a: }5f42fe560a743624340b05c87319ca20{FTCnoom
    for (int i = sizeof(flag) - 2; i >= 0; i--) printf("%c", flag[i]);
    printf("\n");
}

int main() {
    // La memoria reservada debería estar entre ~32 bytes
    int want_flag = 0;
    char password[16];
    printf("Do you have the hammer ready? Try to guess the password: ");
    gets(password);
    if (want_flag) {
        give_flag();
    } else {
        printf("Bad luck, maybe next time ^^\n");
    }
    return 0;
}
