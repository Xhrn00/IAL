/*
 * Binární vyhledávací strom — rekurzivní varianta
 *
 * S využitím datových typů ze souboru btree.h a připravených koster funkcí
 * implementujte binární vyhledávací strom pomocí rekurze.
 */

#include "../btree.h"
#include <stdio.h>
#include <stdlib.h>

/*
 * Inicializace stromu.
 *
 * Uživatel musí zajistit, že inicializace se nebude opakovaně volat nad
 * inicializovaným stromem. V opačném případě může dojít k úniku paměti (memory
 * leak). Protože neinicializovaný ukazatel má nedefinovanou hodnotu, není
 * možné toto detekovat ve funkci. 
 */
void bst_init(bst_node_t **tree) {
  *tree = NULL;
}

/*
 * Vyhledání uzlu v stromu.
 *
 * V případě úspěchu vrátí funkce hodnotu true a do proměnné value zapíše
 * hodnotu daného uzlu. V opačném případě funkce vrátí hodnotu false a proměnná
 * value zůstává nezměněná.
 * 
 * Funkci implementujte rekurzivně bez použité vlastních pomocných funkcí.
 */
bool bst_search(bst_node_t *tree, char key, int *value) {
  if (tree == NULL) {
    return false; // Key not found
  }

  if (tree->key == key) {
    *value = tree->value;  // Set the value
    return true;  // Key found
  }
  
  if (key < tree->key) {
    return bst_search(tree->left, key, value); // Recursively search in the left subtree
  } else {
    return bst_search(tree->right, key, value); // Recursively search in the right subtree
  }
}

/*
 * Vložení uzlu do stromu.
 *
 * Pokud uzel se zadaným klíče už ve stromu existuje, nahraďte jeho hodnotu.
 * Jinak vložte nový listový uzel.
 *
 * Výsledný strom musí splňovat podmínku vyhledávacího stromu — levý podstrom
 * uzlu obsahuje jenom menší klíče, pravý větší. 
 *
 * Funkci implementujte rekurzivně bez použití vlastních pomocných funkcí.
 */
void bst_insert(bst_node_t **tree, char key, int value) {
    bst_node_t *current = *tree;

    // Check if the current node is null
    if (current == NULL) {
        // Allocate memory for a new node
        current = malloc(sizeof(bst_node_t));
        if (current == NULL)return;
        
        // Initialization new node to insert
        current->key = key;
        current->value = value;
        current->left = NULL;
        current->right = NULL;

        *tree = current;
    } else if (current->key == key) {
        // If the current node has the given key, update its value
        current->value = value;
    } else if (key < current->key) {
        // If the key is smaller, recursively insert into the left subtree
        bst_insert(&(current->left), key, value);
    } else {
        // If the key is larger, recursively insert into the right subtree
        bst_insert(&(current->right), key, value);
    }
}

/*
 * Pomocná funkce která nahradí uzel nejpravějším potomkem.
 * 
 * Klíč a hodnota uzlu target budou nahrazeny klíčem a hodnotou nejpravějšího
 * uzlu podstromu tree. Nejpravější potomek bude odstraněný. Funkce korektně
 * uvolní všechny alokované zdroje odstraněného uzlu.
 *
 * Funkce předpokládá, že hodnota tree není NULL.
 * 
 * Tato pomocná funkce bude využitá při implementaci funkce bst_delete.
 *
 * Funkci implementujte rekurzivně bez použití vlastních pomocných funkcí.
 */
void bst_replace_by_rightmost(bst_node_t *target, bst_node_t **tree) {
  bst_node_t *current = *tree;


  // Check if the current node has a right child
  if (current->right == NULL) {
    // The rightmost node found, replace target's key and value
      target->key = current->key;
      target->value = current->value;

      *tree = current->left;  // Rightmost node can only have a left child or no child
      free(current);  // Free the memory of the rightmost node
  } else {
      bst_replace_by_rightmost(target, &(current->right));
  }
}

/*
 * Odstranění uzlu ze stromu.
 *
 * Pokud uzel se zadaným klíčem neexistuje, funkce nic nedělá.
 * Pokud má odstraněný uzel jeden podstrom, zdědí ho rodič odstraněného uzlu.
 * Pokud má odstraněný uzel oba podstromy, je nahrazený nejpravějším uzlem
 * levého podstromu. Nejpravější uzel nemusí být listem.
 * 
 * Funkce korektně uvolní všechny alokované zdroje odstraněného uzlu.
 * 
 * Funkci implementujte rekurzivně pomocí bst_replace_by_rightmost a bez
 * použití vlastních pomocných funkcí.
 */
void bst_delete(bst_node_t **tree, char key) {
    if (*tree == NULL) return;
    

    bst_node_t *current = *tree;

    if (key < current->key) {
        bst_delete(&(current->left), key);
    } else if (key > current->key) {
        bst_delete(&(current->right), key);
    } else {
        // Key is found
        int cldnum = (current->left != NULL) + (current->right != NULL);

        // Children number
        switch (cldnum) {
            case 0:
                // Leaf node
                free(current);
                *tree = NULL;
                break;

            case 1:
                // One child
                {
                    bst_node_t *temp;
                    if (current->left != NULL) {
                        temp = current->left;
                    } else {
                        temp = current->right;
                    }
                    *current = *temp;  // Copy the contents of the non-empty child
                    free(temp);
                }
                break;

            case 2:
                // Two children
                bst_replace_by_rightmost(current, &(current->left));
                // No need to free current here
                break;
        }
    }
}

/*
 * Zrušení celého stromu.
 * 
 * Po zrušení se celý strom bude nacházet ve stejném stavu jako po 
 * inicializaci. Funkce korektně uvolní všechny alokované zdroje rušených 
 * uzlů.
 * 
 * Funkci implementujte rekurzivně bez použití vlastních pomocných funkcí.
 */
void bst_dispose(bst_node_t **tree) {
    if (tree == NULL || *tree == NULL) return; // If the tree pointer is NULL or points to NULL, there's nothing to dispose
    

    // Recursively dispose of the left and right subtrees
    bst_dispose(&((*tree)->left));
    bst_dispose(&((*tree)->right));

    free(*tree);

    *tree = NULL;
}

/*
 * Preorder průchod stromem.
 *
 * Pro aktuálně zpracovávaný uzel zavolejte funkci bst_add_node_to_items.
 *
 * Funkci implementujte rekurzivně bez použití vlastních pomocných funkcí.
 */
void bst_preorder(bst_node_t *tree, bst_items_t *items) {
  if (tree == NULL) return;
    

  bst_add_node_to_items(tree, items);  // Process the current node

  bst_preorder(tree->left, items);     // Recursively process the left subtree
  bst_preorder(tree->right, items);    // Recursively process the right subtree
}

/*
 * Inorder průchod stromem.
 *
 * Pro aktuálně zpracovávaný uzel zavolejte funkci bst_add_node_to_items.
 *
 * Funkci implementujte rekurzivně bez použití vlastních pomocných funkcí.
 */
void bst_inorder(bst_node_t *tree, bst_items_t *items) {
  if (tree == NULL) return;
    

  bst_inorder(tree->left, items);    // Recursively process the left subtree

  bst_add_node_to_items(tree, items); // Process the current node
  bst_inorder(tree->right, items);   // Recursively process the right subtree
}

/*
 * Postorder průchod stromem.
 *
 * Pro aktuálně zpracovávaný uzel zavolejte funkci bst_add_node_to_items.
 *
 * Funkci implementujte rekurzivně bez použití vlastních pomocných funkcí.
 */
void bst_postorder(bst_node_t *tree, bst_items_t *items) {
  if (tree == NULL)return;
    

  bst_postorder(tree->left, items);    // Recursively process the left subtree
  bst_postorder(tree->right, items);   // Recursively process the right subtree

  bst_add_node_to_items(tree, items);  // Process the current node
}
