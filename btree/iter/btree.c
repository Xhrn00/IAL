/*
 * Binární vyhledávací strom — iterativní varianta
 *
 * S využitím datových typů ze souboru btree.h, zásobníku ze souboru stack.h 
 * a připravených koster funkcí implementujte binární vyhledávací 
 * strom bez použití rekurze.
 */

#include "../btree.h"
#include "stack.h"
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
 * Funkci implementujte iterativně bez použité vlastních pomocných funkcí.
 */
bool bst_search(bst_node_t *tree, char key, int *value) {
  while (tree) {
        if (tree->key == key) {
            *value = tree->value;
            return true;
        } else if (key < tree->key) {
            tree = tree->left;
        } else { // key > tree->key
            tree = tree->right;
        }
    }
    return false;
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
 * Funkci implementujte iterativně bez použití vlastních pomocných funkcí.
 */
void bst_insert(bst_node_t **tree, char key, int value) {
  bst_node_t *current = *tree;

  //Crafting the node
  bst_node_t *insert_node = malloc(sizeof(bst_node_t));
  if (insert_node == NULL)return;
  
  insert_node->key = key;
  insert_node->left = NULL;
  insert_node->right = NULL;
  insert_node->value = value;

 while (current) {
    // Result of comparing bt->key and key
    int compare_result;

    // Comparing current->key(key of current element) and key
    if (current->key > key) {
        compare_result = 1;
    } else if (current->key < key) {
        compare_result = -1;
    } else {
        compare_result = 0; // current->key == key
    }

 switch (compare_result) {
        
    case 0: // current->key == key
            current->value = value;
            
            free(insert_node);
            return;

    case 1: // current->key > key
            if (current->left == NULL) {
                current->left = insert_node;
                return;
            }
            
            current = current->left;
            break;

    case -1: // current->key < key
            if (current->right == NULL) {
              current->right = insert_node;
                return;
            }
            
            current = current->right;
            break;
    }
}
  // Updating the current node
  *tree = insert_node;
  return;
}

/*
 * Pomocná funkce která nahradí uzel nejpravějším potomkem.
 * 
 * Klíč a hodnota uzlu target budou nahrazené klíčem a hodnotou nejpravějšího
 * uzlu podstromu tree. Nejpravější potomek bude odstraněný. Funkce korektně
 * uvolní všechny alokované zdroje odstraněného uzlu.
 *
 * Funkce předpokládá, že hodnota tree není NULL.
 * 
 * Tato pomocná funkce bude využita při implementaci funkce bst_delete.
 *
 * Funkci implementujte iterativně bez použití vlastních pomocných funkcí.
 */
void bst_replace_by_rightmost(bst_node_t *target, bst_node_t **tree) {
   bst_node_t *current = *tree;
   bst_node_t *parent = NULL;
   
    // Finding the rightmost node
    while (current && current->right) {
        parent = current;
        current = current->right;
    }

    // If the rightmost node has a left child, move it up
    if (current->left) {
        if (parent) {
            parent->right = current->left;
        } else {
            *tree = current->left;
        }
    } else {
        // If the rightmost node has no left child, simply remove it
        if (parent) {
            parent->right = NULL;
        } else {
            *tree = NULL;
        }
    }

    // Replace the target node with the rightmost node
    target->key = current->key;
    target->value = current->value;

    // Free the rightmost node
    free(current);
  
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
 * Funkci implementujte iterativně pomocí bst_replace_by_rightmost a bez
 * použití vlastních pomocných funkcí.
 */
void bst_delete(bst_node_t **tree, char key) {
  if (!tree || !*tree) return; // Check for null pointer
    

    bst_node_t *current = *tree;
    bst_node_t *parent = NULL;

    // Finding the node and its parent
    while (current != NULL && current->key != key) {
        parent = current;
        if (key < current->key) {
            current = current->left;
        } else {
            current = current->right;
        }
    }

    // Node with the key was't found
    if (!current) return;
    

    // Node with two children
    if (current->left != NULL && current->right != NULL) {
        bst_replace_by_rightmost(current, &current->left);
    } else {
        // Node with zero or one child
        bst_node_t *child;
        if (current->left != NULL) {
            child = current->left;
        } else {
            child = current->right;
        }

        if (parent == NULL) {
            // Removing root node
            *tree = child;
        } else if (parent->left == current) {
            parent->left = child;
        } else {
            parent->right = child;
        }

        free(current); // Free the removed node
    }
}



/*
 * Zrušení celého stromu.
 * 
 * Po zrušení se celý strom bude nacházet ve stejném stavu jako po 
 * inicializaci. Funkce korektně uvolní všechny alokované zdroje rušených 
 * uzlů.
 * 
 * Funkci implementujte iterativně s pomocí zásobníku a bez použití 
 * vlastních pomocných funkcí.
 */
void bst_dispose(bst_node_t **tree) {
  bst_node_t *current = *tree; // Current node
  if (current == NULL)return;
  
  // Inicialization of stack
  stack_bst_t stack;
  stack_bst_init(&stack);
  stack_bst_push(&stack, current);

  while(!stack_bst_empty(&stack))
    {
      bst_node_t *node = stack_bst_pop(&stack);

        // Pushing the children of the current node to the stack
        if (node->left != NULL) {
            stack_bst_push(&stack, node->left);
        }
        if (node->right != NULL) {
            stack_bst_push(&stack, node->right);
        }

        // Free the current node
        free(node);
    }
    *tree = NULL;
}

/*
 * Pomocná funkce pro iterativní preorder.
 *
 * Prochází po levé větvi k nejlevějšímu uzlu podstromu.
 * Nad zpracovanými uzly zavolá bst_add_node_to_items a uloží je do zásobníku uzlů.
 *
 * Funkci implementujte iterativně s pomocí zásobníku a bez použití 
 * vlastních pomocných funkcí.
 */
void bst_leftmost_preorder(bst_node_t *tree, stack_bst_t *to_visit, bst_items_t *items) {
  while (tree != NULL) {
        // Adding the current node to the items and push it into stack
        bst_add_node_to_items(tree, items);
        stack_bst_push(to_visit, tree);

        // Moving to the left child
        tree = tree->left;
    }
}

/*
 * Preorder průchod stromem.
 *
 * Pro aktuálně zpracovávaný uzel zavolejte funkci bst_add_node_to_items.
 *
 * Funkci implementujte iterativně pomocí funkce bst_leftmost_preorder a
 * zásobníku uzlů a bez použití vlastních pomocných funkcí.
 */
void bst_preorder(bst_node_t *tree, bst_items_t *items) {
    if (tree == NULL) return; 
    

    // Inicialization of stack
    stack_bst_t to_visit;
    stack_bst_init(&to_visit);

    // Starting the traversal by going down the leftmost path from the root
    bst_leftmost_preorder(tree, &to_visit, items);

    while (!stack_bst_empty(&to_visit)) {
        bst_node_t *node = stack_bst_pop(&to_visit);

        // If the node has a right child, apply bst_leftmost_preorder to the right child
        if (node->right != NULL) {
            bst_leftmost_preorder(node->right, &to_visit, items);
        }
    }

    stack_bst_empty(&to_visit);
}


/*
 * Pomocná funkce pro iterativní inorder.
 * 
 * Prochází po levé větvi k nejlevějšímu uzlu podstromu a ukládá uzly do
 * zásobníku uzlů.
 *
 * Funkci implementujte iterativně s pomocí zásobníku a bez použití 
 * vlastních pomocných funkcí.
 */
void bst_leftmost_inorder(bst_node_t *tree, stack_bst_t *to_visit) {
  while (tree != NULL) {
        // Pushing the current node into stack
        stack_bst_push(to_visit, tree);

        // Move to the left child
        tree = tree->left;
    }
}

/*
 * Inorder průchod stromem.
 *
 * Pro aktuálně zpracovávaný uzel zavolejte funkci bst_add_node_to_items.
 *
 * Funkci implementujte iterativně pomocí funkce bst_leftmost_inorder a
 * zásobníku uzlů a bez použití vlastních pomocných funkcí.
 */
void bst_inorder(bst_node_t *tree, bst_items_t *items) {
  if (tree == NULL) return; 

  // Inicialization of stack
  stack_bst_t stack;
  stack_bst_init(&stack);

    bst_leftmost_inorder(tree, &stack);

    while (!stack_bst_empty(&stack)) {
        bst_node_t *node = stack_bst_pop(&stack);

        // Process the current node
        bst_add_node_to_items(node, items);

        // If the node has a right child, push the leftmost nodes of its right subtree onto the stack
        if (node->right != NULL) {
            bst_leftmost_inorder(node->right, &stack);
        }
    }

    stack_bst_empty(&stack);
}

/*
 * Pomocná funkce pro iterativní postorder.
 *
 * Prochází po levé větvi k nejlevějšímu uzlu podstromu a ukládá uzly do
 * zásobníku uzlů. Do zásobníku bool hodnot ukládá informaci, že uzel
 * byl navštíven poprvé.
 *
 * Funkci implementujte iterativně pomocí zásobníku uzlů a bool hodnot a bez použití
 * vlastních pomocných funkcí.
 */
void bst_leftmost_postorder(bst_node_t *tree, stack_bst_t *to_visit,
                            stack_bool_t *first_visit) {
    while (tree != NULL) {
        // Pushing the current node into the stack
        stack_bst_push(to_visit, tree);
        stack_bool_push(first_visit, true);

        // Move to the left child
        tree = tree->left;
    }
}

/*
 * Postorder průchod stromem.
 *
 * Pro aktuálně zpracovávaný uzel zavolejte funkci bst_add_node_to_items.
 *
 * Funkci implementujte iterativně pomocí funkce bst_leftmost_postorder a
 * zásobníku uzlů a bool hodnot a bez použití vlastních pomocných funkcí.
 */
void bst_postorder(bst_node_t *tree, bst_items_t *items) {
    if (tree == NULL) return;
    

    stack_bst_t to_visit;
    stack_bool_t first_visit;

    // Inicialization of stack
    stack_bst_init(&to_visit);
    stack_bool_init(&first_visit);

    bst_leftmost_postorder(tree, &to_visit, &first_visit);

    while (!stack_bst_empty(&to_visit)) {
        bst_node_t *current = stack_bst_top(&to_visit);
        bool visiting = stack_bool_top(&first_visit);
        stack_bool_pop(&first_visit);

    switch (visiting) {
        case true: // first visiting
            // If this is the first visit, go to its right child
            stack_bool_push(&first_visit, false);
            bst_leftmost_postorder(current->right, &to_visit, &first_visit);
            break;

        case false: // second visiting
            // If this is't the first visit, it means we have already visited left and right children
            // So we process the node
            bst_add_node_to_items(current, items);
            stack_bst_pop(&to_visit);
            break;
    }
    }

    stack_bst_empty(&to_visit);
    stack_bool_empty(&first_visit);
}

