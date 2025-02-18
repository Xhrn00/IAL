/*
 * Tabulka s rozptýlenými položkami
 *
 * S využitím datových typů ze souboru hashtable.h a připravených koster
 * funkcí implementujte tabulku s rozptýlenými položkami s explicitně
 * zretězenými synonymy.
 *
 * Při implementaci uvažujte velikost tabulky HT_SIZE.
 */

#include "hashtable.h"
#include <stdlib.h>
#include <string.h>

int HT_SIZE = MAX_HT_SIZE;

/*
 * Rozptylovací funkce která přidělí zadanému klíči index z intervalu
 * <0,HT_SIZE-1>. Ideální rozptylovací funkce by měla rozprostírat klíče
 * rovnoměrně po všech indexech. Zamyslete sa nad kvalitou zvolené funkce.
 */
int get_hash(char *key) {
  int result = 1;
  int length = strlen(key);
  for (int i = 0; i < length; i++) {
    result += key[i];
  }
  return (result % HT_SIZE);
}

/*
 * Inicializace tabulky — zavolá sa před prvním použitím tabulky.
 */
void ht_init(ht_table_t *table) {  
  for (int i = 0; i < HT_SIZE; i++) {
        (*table)[i] = NULL;
  }
}

/*
 * Vyhledání prvku v tabulce.
 *
 * V případě úspěchu vrací ukazatel na nalezený prvek; v opačném případě vrací
 * hodnotu NULL.
 */
ht_item_t *ht_search(ht_table_t *table, char *key) {
   int index = get_hash(key);  // Calculate the hash index

    // Traverse the linked list at the calculated index
    ht_item_t *current = (*table)[index];
    while (current != NULL) {
        if (strcmp(key, current->key) == 0) {
            // The keys are the same and the match has found
            return current;
        }
        current = current->next;
    }
  return NULL;
}

/*
 * Vložení nového prvku do tabulky.
 *
 * Pokud prvek s daným klíčem už v tabulce existuje, nahraďte jeho hodnotu.
 *
 * Při implementaci využijte funkci ht_search. Pri vkládání prvku do seznamu
 * synonym zvolte nejefektivnější možnost a vložte prvek na začátek seznamu.
 */
void ht_insert(ht_table_t *table, char *key, float value) {
    int index = get_hash(key);  // Calculate the hash index

    // Search for the searching item with the specified key
    ht_item_t *searching_item = ht_search(table, key);
    if (searching_item != NULL)
    {
        // Updating the value 
        searching_item->value = value;
        return;
    }
    
    // Creating new item
    ht_item_t *new_item = (ht_item_t *)malloc(sizeof(ht_item_t));
    if (new_item == NULL) return;
    
    // Allocating the memory for the key 
    new_item->key = (char *)malloc(strlen(key) + 1);
    
    if (new_item->key == NULL) {
        free(new_item);
        return;
    }

    // Crafting new_item
    strcpy(new_item->key, key);
    new_item->next = NULL;
    new_item->value = value;

    // Inserting new item at the beginning of the list
    new_item->next = (*table)[index];
    (*table)[index] = new_item;
}

/*
 * Získání hodnoty z tabulky.
 *
 * V případě úspěchu vrací funkce ukazatel na hodnotu prvku, v opačném
 * případě hodnotu NULL.
 *
 * Při implementaci využijte funkci ht_search.
 */
float *ht_get(ht_table_t *table, char *key) {
  ht_item_t *item = ht_search(table, key);

    if (item != NULL) {
        // Return a pointer to the value of the found item
        return &item->value;
    } else {
        // Key wasn't found
        return NULL;
    }
}

/*
 * Smazání prvku z tabulky.
 *
 * Funkce korektně uvolní všechny alokované zdroje přiřazené k danému prvku.
 * Pokud prvek neexistuje, funkce nedělá nic.
 *
 * Při implementaci NEPOUŽÍVEJTE funkci ht_search.
 */
void ht_delete(ht_table_t *table, char *key) {
    int index = get_hash(key);  // Calculate the hash index
    
    // Сhecking if an element exists
    ht_item_t *current = (*table)[index];
    if (current == NULL)return;
    
    // Count of synonyms
    int count = 0;
    while (current != NULL){
        count++;
        current = current->next;
    }
        
    if (count == 0) return;

    // Create an array of synonyms
    ht_item_t **synonymsArray = malloc(count * sizeof(ht_item_t*));
    current = (*table)[index]; // Reset current to the start of the list

    // Filling the array
    for (int i = 0; i < count; i++) {
        synonymsArray[i] = current;
        current = current->next;
    }

    // Search for the item to delete in the array
    for (int i = 0; i < count; i++) {
        if (strcmp(synonymsArray[i]->key, key) == 0) {
            free(synonymsArray[i]->key);
            free(synonymsArray[i]);

            for (int j = i; j < count - 1; j++) {
                synonymsArray[j] = synonymsArray[j + 1];
            }
            count--; 
            break; 
        }
    }

    // Rebuilding the linked list from the array
    for (int i = 0; i < count; i++) {
        synonymsArray[i]->next = (i < count - 1) ? synonymsArray[i + 1] : NULL;
    }

    // Updating the table
    if (count > 0) {
    (*table)[index] = synonymsArray[0];
    } else {
    (*table)[index] = NULL;
    }

    // Free the array
    free(synonymsArray);
}

/*
 * Smazání všech prvků z tabulky.
 *
 * Funkce korektně uvolní všechny alokované zdroje a uvede tabulku do stavu po 
 * inicializaci.
 */
void ht_delete_all(ht_table_t *table) {
   for (int i = 0; i < HT_SIZE; i++) {
        for (ht_item_t *current = (*table)[i]; current != NULL; ) {
            ht_item_t *next = current->next;
            free(current->key);
            free(current);
            current = next;
        }
        (*table)[i] = NULL;
    }
}
