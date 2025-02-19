/*
 * Použití binárních vyhledávacích stromů.
 *
 * S využitím Vámi implementovaného binárního vyhledávacího stromu (soubory ../iter/btree.c a ../rec/btree.c)
 * implementujte funkci letter_count. Výstupní strom může být značně degradovaný (až na úroveň lineárního seznamu) 
 * a tedy implementujte i druhou funkci (bst_balance), která strom, na požadavek uživatele, vybalancuje.
 * Funkce jsou na sobě nezávislé a tedy automaticky NEVOLEJTE bst_balance v letter_count.
 * 
 */

#include "../btree.h"
#include <stdio.h>
#include <stdlib.h>




// Help function
char classify_character(char c) {
    if (c >= 'A' && c <= 'Z') {
        return c + ('a' - 'A'); // Converting uppercase to lowercase
    } else if (c >= 'a' && c <= 'z') {
        return c; // Lowercase letters are kept as is
    } else if (c == ' ') {
        return ' '; // Space character
    } else {
        return '_'; // All other characters
    }
}



/**
 * Vypočítání frekvence výskytů znaků ve vstupním řetězci.
 * 
 * Funkce inicilializuje strom a následně zjistí počet výskytů znaků a-z (case insensitive), znaku 
 * mezery ' ', a ostatních znaků (ve stromu reprezentováno znakem podtržítka '_'). Výstup je v 
 * uložen ve stromu.
 * 
 * Například pro vstupní řetězec: "abBccc_ 123 *" bude strom po běhu funkce obsahovat:
 * 
 * key | value
 * 'a'     1
 * 'b'     2
 * 'c'     3
 * ' '     2
 * '_'     5
 * 
 * Pro implementaci si můžete v tomto souboru nadefinovat vlastní pomocné funkce.
*/
void letter_count(bst_node_t **tree, char *input) {
    bst_init(tree); // Initialize the tree

    int i = 0;
    while (input[i] != '\0') { // Loop through each character of the string
        char key = classify_character(input[i]); // Classify the character
        int count = 0;

        if (bst_search(*tree, key, &count)) {
            bst_insert(tree, key, count + 1); // If found, increment the count
        } else {
            bst_insert(tree, key, 1); // If not found, insert with a count of 1
        }

        i++; // Move to the next character
    }
}



// Help function to build a balanced tree 
bst_node_t *build_balanced_tree(bst_items_t *items, int start, int end) {
    if (start > end) return NULL;

    int middle = start + (end - start) / 2;
    bst_node_t *current = items->nodes[middle];

    current->left = build_balanced_tree(items, start, middle - 1);
    current->right = build_balanced_tree(items, middle + 1, end);

    return current;
}

/**
 * Vyvážení stromu.
 * 
 * Vyvážený binární vyhledávací strom je takový binární strom, kde hloubka podstromů libovolného uzlu se od sebe liší maximálně o jedna.
 * 
 * Předpokládejte, že strom je alespoň inicializován. K získání uzlů stromu využijte vhodnou verzi vámi naimplmentovaného průchodu stromem.
 * Následně můžete například vytvořit nový strom, kde pořadím vkládaných prvků zajistíte vyváženost.
 *  
 * Pro implementaci si můžete v tomto souboru nadefinovat vlastní pomocné funkce. Není nutné, aby funkce fungovala *in situ* (in-place).
*/
void bst_balance(bst_node_t **tree) {
    if (*tree == NULL) return;

    int start = 0;

    // Initialization bst_items_t for storing nodes
    bst_items_t items;
    items.capacity = 0;
    items.nodes = NULL;
    items.size = 0;

    // Collecting nodes in sorted order
    bst_inorder(*tree, &items);

    // Building a new balanced tree
    *tree = build_balanced_tree(&items, start, items.size - 1);

    free(items.nodes);
}