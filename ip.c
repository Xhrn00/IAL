/* ******************************* c204.c *********************************** */
/*  Předmět: Algoritmy (IAL) - FIT VUT v Brně                                 */
/*  Úkol: c204 - Převod infixového výrazu na postfixový (s využitím c202)     */
/*  Referenční implementace: Petr Přikryl, listopad 1994                      */
/*  Přepis do jazyka C: Lukáš Maršík, prosinec 2012                           */
/*  Upravil: Kamil Jeřábek, září 2019                                         */
/*           Daniel Dolejška, září 2021                                       */
/* ************************************************************************** */
/*
** Implementujte proceduru pro
** převod infixového výrazu na postfixový.
 * Pro převod využijte zásobník (Stack), který byl
** implementován v rámci příkladu c202. Bez správného vyřešení příkladu c202
** se o řešení tohoto příkladu nepokoušejte.
**
** Implementujte následující funkci:
**
**    infix2postfix ... konverzní funkce pro převod infixového výrazu
**                      na postfixový
**
** Pro lepší přehlednost kódu implementujte následující pomocné funkce:
**    
**    untilLeftPar ... vyprázdnění zásobníku až po levou závorku
**    doOperation .... zpracování operátoru konvertovaného výrazu
**
** Své řešení účelně komentujte.
**
** Terminologická poznámka: Jazyk C nepoužívá pojem procedura.
** Proto zde používáme pojem funkce i pro operace, které by byly
** v algoritmickém jazyce Pascalovského typu implemenovány jako procedury
** (v jazyce C procedurám odpovídají funkce vracející typ void).
**
**/

#include "c204.h"

bool solved;

typedef struct operator {
    char op;
    int priority;
}Operator;

Operator plus = {'+', 1};
Operator minus = {'-', 1};
Operator multiply = {'*', 2};
Operator divide = {'/', 2};
Operator leftPar = {'(', 0};
Operator rightPar = {')', 0};




/**
 * Pomocná funkce untilLeftPar.
 * Slouží k vyprázdnění zásobníku až po levou závorku, přičemž levá závorka bude
 * také odstraněna.
 * Pokud je zásobník prázdný, provádění funkce se ukončí.
 *
 * Operátory odstraňované ze zásobníku postupně vkládejte do výstupního pole
 * znaků postfixExpression.
 * Délka převedeného výrazu a též ukazatel na první volné místo, na které se má
 * zapisovat, představuje parametr postfixExpressionLength.
 *
 * Aby se minimalizoval počet přístupů ke struktuře zásobníku, můžete zde
 * nadeklarovat a používat pomocnou proměnnou typu char.
 *
 * @param stack Ukazatel na inicializovanou strukturu zásobníku
 * @param postfixExpression Znakový řetězec obsahující výsledný postfixový výraz
 * @param postfixExpressionLength Ukazatel na aktuální délku výsledného postfixového výrazu
 */
void untilLeftPar( Stack *stack, char *postfixExpression, unsigned *postfixExpressionLength ) {
    char operand;
    // Remove operators from the stack until we hit the left bracket
    while (!Stack_IsEmpty(stack)) {
        Stack_Top(stack, &operand);
        if (operand == '(') {
            Stack_Pop(stack);
            break;
        }
        Stack_Pop(stack);
        postfixExpression[(*postfixExpressionLength)++] = operand;;// Add an operator to the postfix expression
    }
}

/**
 * Pomocná funkce doOperation.
 * Zpracuje operátor, který je předán parametrem c po načtení znaku ze
 * vstupního pole znaků.
 *
 * Dle priority předaného operátoru a případně priority operátoru na vrcholu
 * zásobníku rozhodneme o dalším postupu.
 * Délka převedeného výrazu a taktéž ukazatel na první volné místo, do kterého
 * se má zapisovat, představuje parametr postfixExpressionLength, výstupním
 * polem znaků je opět postfixExpression.
 *
 * @param stack Ukazatel na inicializovanou strukturu zásobníku
 * @param c Znak operátoru ve výrazu
 * @param postfixExpression Znakový řetězec obsahující výsledný postfixový výraz
 * @param postfixExpressionLength Ukazatel na aktuální délku výsledného postfixového výrazu
 */
void doOperation( Stack *stack, char c, char *postfixExpression, unsigned *postfixExpressionLength ) {
    char operand;;

    if (!Stack_IsEmpty(stack)) {
        Stack_Top(stack, &operand);
    }

    Operator currentOperator = c == '+' ? plus : c == '-' ? minus : c == '*' ? multiply : divide;
    Operator topOperator = operand == '+' ? plus : operand == '-' ? minus : operand == '*' ? multiply : divide;

    if (Stack_IsEmpty(stack) || operand == '(' || currentOperator.priority > topOperator.priority) {
        Stack_Push(stack, c);
    } else {
        postfixExpression[(*postfixExpressionLength)++] = operand;
        Stack_Pop(stack);
        doOperation(stack, c, postfixExpression, postfixExpressionLength);
    }
}
/**
 * Konverzní funkce infix2postfix.
 * Čte infixový výraz ze vstupního řetězce infixExpression a generuje
 * odpovídající postfixový výraz do výstupního řetězce (postup převodu hledejte
 * v přednáškách nebo ve studijní opoře).
 * Paměť pro výstupní řetězec (o velikosti MAX_LEN) je třeba alokovat. Volající
 * funkce, tedy příjemce konvertovaného řetězce, zajistí korektní uvolnění zde
 * alokované paměti.
 *
 * Tvar výrazu:
 * 1. Výraz obsahuje operátory + - * / ve významu sčítání, odčítání,
 *    násobení a dělení. Sčítání má stejnou prioritu jako odčítání,
 *    násobení má stejnou prioritu jako dělení. Priorita násobení je
 *    větší než priorita sčítání. Všechny operátory jsou binární
 *    (neuvažujte unární mínus).
 *
 * 2. Hodnoty ve výrazu jsou reprezentovány jednoznakovými identifikátory
 *    a číslicemi - 0..9, a..z, A..Z (velikost písmen se rozlišuje).
 *
 * 3. Ve výrazu může být použit předem neurčený počet dvojic kulatých
 *    závorek. Uvažujte, že vstupní výraz je zapsán správně (neošetřujte
 *    chybné zadání výrazu).
 *
 * 4. Každý korektně zapsaný výraz (infixový i postfixový) musí být uzavřen
 *    ukončovacím znakem '='.
 *
 * 5. Při stejné prioritě operátorů se výraz vyhodnocuje zleva doprava.
 *
 * Poznámky k implementaci
 * -----------------------
 * Jako zásobník použijte zásobník znaků Stack implementovaný v příkladu c202.
 * Pro práci se zásobníkem pak používejte výhradně operace z jeho rozhraní.
 *
 * Při implementaci využijte pomocné funkce untilLeftPar a doOperation.
 *
 * Řetězcem (infixového a postfixového výrazu) je zde myšleno pole znaků typu
 * char, jenž je korektně ukončeno nulovým znakem dle zvyklostí jazyka C.
 *
 * Na vstupu očekávejte pouze korektně zapsané a ukončené výrazy. Jejich délka
 * nepřesáhne MAX_LEN-1 (MAX_LEN i s nulovým znakem) a tedy i výsledný výraz
 * by se měl vejít do alokovaného pole. Po alokaci dynamické paměti si vždycky
 * ověřte, že se alokace skutečně zdrařila. V případě chyby alokace vraťte namísto
 * řetězce konstantu NULL.
 *
 * @param infixExpression vstupní znakový řetězec obsahující infixový výraz k převedení
 *
 * @returns znakový řetězec obsahující výsledný postfixový výraz
 */
char *infix2postfix( const char *infixExpression ) {
    char *postfixExpression;
    Stack stack;
    unsigned postfixExpressionLength = 0;

    if ((postfixExpression = malloc(MAX_LEN * sizeof(char))) == NULL)return NULL;

    Stack_Init(&stack);

    for (int i = 0; infixExpression[i] != '\0'; i++) {
        char currentChar = infixExpression[i];
        // Your logic to handle operands and operators goes here
        if ((currentChar >= 'a' && currentChar <= 'z') || (currentChar >= 'A' && currentChar <= 'Z') || (currentChar >= '0' && currentChar <= '9')) {
            // Operand, add to postfix expression
            postfixExpression[postfixExpressionLength++] = currentChar;
        } else if (currentChar == '(') {
            // Left parenthesis, push onto the stack
            Stack_Push(&stack, currentChar);
        } else if (currentChar == ')') {
            // Right parenthesis, process until left parenthesis
            untilLeftPar(&stack, postfixExpression, &postfixExpressionLength);
        } else if (currentChar == '+' || currentChar == '-') {
            // Operator + or -, process operators on the stack
            doOperation(&stack, currentChar, postfixExpression, &postfixExpressionLength);
        } else if (currentChar == '*' || currentChar == '/') {
            // Operator * or /, process operators on the stack
            doOperation(&stack, currentChar, postfixExpression, &postfixExpressionLength);

        }else{
            // End of expression, process remaining operators on the stack
            while (!Stack_IsEmpty(&stack)) {
                Stack_Top(&stack, &currentChar);
                Stack_Pop(&stack);
                postfixExpression[postfixExpressionLength++] = currentChar;
            }
            postfixExpression[postfixExpressionLength++] = '=';
            postfixExpression[postfixExpressionLength++] = '\0';// Null-terminate the postfix expression
            break;
        }
    }
    // Clean up and free the stack memory
    Stack_Dispose(&stack);
    return postfixExpression;
}


/**
 * Pomocná metoda pro vložení celočíselné hodnoty na zásobník.
 *
 * Použitá implementace zásobníku aktuálně umožňuje vkládání pouze
 * hodnot o velikosti jednoho byte (char). Využijte této metody
 * k rozdělení a postupné vložení celočíselné (čtyřbytové) hodnoty
 * na vrchol poskytnutého zásobníku.
 *
 * @param stack ukazatel na inicializovanou strukturu zásobníku
 * @param value hodnota k vložení na zásobník
 */
void expr_value_push( Stack *stack, int value ) {

    char byte1 = (value >> 24) & 0xFF;
    char byte2 = (value >> 16) & 0xFF;
    char byte3 = (value >> 8) & 0xFF;
    char byte4 = value & 0xFF;

    Stack_Push(stack, byte1);
    Stack_Push(stack, byte2);
    Stack_Push(stack, byte3);
    Stack_Push(stack, byte4);
}

/**
 * Pomocná metoda pro extrakci celočíselné hodnoty ze zásobníku.
 *
 * Využijte této metody k opětovnému načtení a složení celočíselné
 * hodnoty z aktuálního vrcholu poskytnutého zásobníku. Implementujte
 * tedy algoritmus opačný k algoritmu použitému v metodě
 * `expr_value_push`.
 *
 * @param stack ukazatel na inicializovanou strukturu zásobníku
 * @param value ukazatel na celočíselnou proměnnou pro uložení
 *   výsledné celočíselné hodnoty z vrcholu zásobníku
 */
void expr_value_pop( Stack *stack, int *value ) {

    char byte1, byte2, byte3, byte4;

    Stack_Top(stack, &byte4);
    Stack_Pop(stack);
    Stack_Top(stack, &byte3);
    Stack_Pop(stack);
    Stack_Top(stack, &byte2);
    Stack_Pop(stack);
    Stack_Top(stack, &byte1);
    Stack_Pop(stack);

    *value = (byte1 << 24) | (byte2 << 16) | (byte3 << 8) | byte4;
}


/**
 * Tato metoda provede vyhodnocení výrazu zadaném v `infixExpression`,
 * kde hodnoty proměnných použitých v daném výrazu jsou definovány
 * v poli `variableValues`.
 *
 * K vyhodnocení vstupního výrazu využijte implementaci zásobníku
 * ze cvičení c202. Dále také využijte pomocných funkcí `expr_value_push`,
 * respektive `expr_value_pop`. Při řešení si můžete definovat libovolné
 * množství vlastních pomocných funkcí.
 *
 * Předpokládejte, že hodnoty budou vždy definovány
 * pro všechy proměnné použité ve vstupním výrazu.
 *
 * @param infixExpression vstpní infixový výraz s proměnnými
 * @param variableValues hodnoty proměnných ze vstupního výrazu
 * @param variableValueCount počet hodnot (unikátních proměnných
 *   ve vstupním výrazu)
 * @param value ukazatel na celočíselnou proměnnou pro uložení
 *   výsledné hodnoty vyhodnocení vstupního výrazu
 *
 * @return výsledek vyhodnocení daného výrazu na základě poskytnutých hodnot proměnných
 */
bool eval( const char *infixExpression, VariableValue variableValues[], int variableValueCount, int *value ) {
        char *postfixExpression = infix2postfix(infixExpression);
        Stack stack;
        Stack_Init(&stack);

        for (int i = 0; postfixExpression[i] != '\0'; i++) {
            //here we check if the character a letter
            if ((postfixExpression[i] >= 'a' && postfixExpression[i] <= 'z') || (postfixExpression[i] >= 'A' && postfixExpression[i] <= 'Z')) {
            //if it is a letter we search for the value of the variable
                for (int j = 0; j < variableValueCount; j++) {

                    if (variableValues[j].name == postfixExpression[i]){
                        expr_value_push(&stack, variableValues[j].value);
                        break ;
                        }

                }

            } else if(postfixExpression[i] == '='){
                expr_value_pop(&stack, value);
            }
            else {
                //order of operands was changed to match the order of operands in the stack
                int var1, var2;//variables for the operands
                expr_value_pop(&stack, &var2);
                expr_value_pop(&stack, &var1);
                //this switch is used to evaluate the expression
                switch (postfixExpression[i]) {
                    case '+':
                        expr_value_push(&stack, var1 + var2);
                        break;
                    case '-':
                        expr_value_push(&stack, var1 - var2);
                        break;
                    case '*':
                        expr_value_push(&stack, var1 * var2);
                        break;
                    case '/':
                        if (var2 == 0) {
                            Stack_Dispose(&stack);
                            free(postfixExpression);
                            fprintf(stderr, "Division by zero\n");
                            return false;
                        }
                        expr_value_push(&stack, var1 / var2);
                        break;
                }

            }
        }
        Stack_Dispose(&stack);
        free(postfixExpression);
    return value;
}


/* Konec c204.c */
