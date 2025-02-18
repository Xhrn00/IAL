/* ******************************* c206.c *********************************** */
/*  Předmět: Algoritmy (IAL) - FIT VUT v Brně                                 */
/*  Úkol: c206 - Dvousměrně vázaný lineární seznam                            */
/*  Návrh a referenční implementace: Bohuslav Křena, říjen 2001               */
/*  Vytvořil: Martin Tuček, říjen 2004                                        */
/*  Upravil: Kamil Jeřábek, září 2020                                         */
/*           Daniel Dolejška, září 2021                                       */
/*           Daniel Dolejška, září 2022                                       */
/* ************************************************************************** */
/*
** Implementujte abstraktní datový typ.
** Užitečným obsahem prvku seznamu je hodnota typu int. Seznam bude jako datová
** abstrakce reprezentován proměnnou typu DLList (DL znamená Doubly-Linked
** a slouží pro odlišení jmen konstant, typů a funkcí od jmen u jednosměrně
** vázaného lineárního seznamu). Definici konstant a typů naleznete
** v hlavičkovém souboru c206.h.
**
** Vaším úkolem je implementovat následující operace, které spolu s výše
** uvedenou datovou částí abstrakce tvoří abstraktní datový typ obousměrně
** vázaný lineární seznam:
**
**      DLL_Init ........... inicializace seznamu před prvním použitím,
**      DLL_Dispose ........ zrušení všech prvků seznamu,
**      DLL_InsertFirst .... vložení prvku na začátek seznamu,
**      DLL_InsertLast ..... vložení prvku na konec seznamu,
**      DLL_First .......... nastavení aktivity na první prvek,
**      DLL_Last ........... nastavení aktivity na poslední prvek,
**      DLL_GetFirst ....... vrací hodnotu prvního prvku,
**      DLL_GetLast ........ vrací hodnotu posledního prvku,
**      DLL_DeleteFirst .... zruší první prvek seznamu,
**      DLL_DeleteLast ..... zruší poslední prvek seznamu,
**      DLL_DeleteAfter .... ruší prvek za aktivním prvkem,
**      DLL_DeleteBefore ... ruší prvek před aktivním prvkem,
**      DLL_InsertAfter .... vloží nový prvek za aktivní prvek seznamu,
**      DLL_InsertBefore ... vloží nový prvek před aktivní prvek seznamu,
**      DLL_GetValue ....... vrací hodnotu aktivního prvku,
**      DLL_SetValue ....... přepíše obsah aktivního prvku novou hodnotou,
**      DLL_Previous ....... posune aktivitu na předchozí prvek seznamu,
**      DLL_Next ........... posune aktivitu na další prvek seznamu,
**      DLL_IsActive ....... zjišťuje aktivitu seznamu.
**
** Při implementaci jednotlivých funkcí nevolejte žádnou z funkcí
** implementovaných v rámci tohoto příkladu, není-li u funkce explicitně
 * uvedeno něco jiného.
**
** Nemusíte ošetřovat situaci, kdy místo legálního ukazatele na seznam
** předá někdo jako parametr hodnotu NULL.
**
** Svou implementaci vhodně komentujte!
**
** Terminologická poznámka: Jazyk C nepoužívá pojem procedura.
** Proto zde používáme pojem funkce i pro operace, které by byly
** v algoritmickém jazyce Pascalovského typu implemenovány jako procedury
** (v jazyce C procedurám odpovídají funkce vracející typ void).
**
**/

#include "c206.h"

bool error_flag;
bool solved;

/**
 * Vytiskne upozornění na to, že došlo k chybě.
 * Tato funkce bude volána z některých dále implementovaných operací.
 */
void DLL_Error(void) {
	printf("*ERROR* The program has performed an illegal operation.\n");
	error_flag = true;
}

/**
 * Provede inicializaci seznamu list před jeho prvním použitím (tzn. žádná
 * z následujících funkcí nebude volána nad neinicializovaným seznamem).
 * Tato inicializace se nikdy nebude provádět nad již inicializovaným seznamem,
 * a proto tuto možnost neošetřujte.
 * Vždy předpokládejte, že neinicializované proměnné mají nedefinovanou hodnotu.
 *
 * @param list Ukazatel na strukturu dvousměrně vázaného seznamu
 */
void DLL_Init( DLList *list ) {
    list->activeElement = NULL;//initialization of the active element
    list->firstElement = NULL;//initialization of the first element
    list->lastElement = NULL;//initialization of the last element
}

/**
 * Zruší všechny prvky seznamu list a uvede seznam do stavu, v jakém se nacházel
 * po inicializaci.
 * Rušené prvky seznamu budou korektně uvolněny voláním operace free.
 *
 * @param list Ukazatel na inicializovanou strukturu dvousměrně vázaného seznamu
 */
void DLL_Dispose( DLList *list ) {
    DLLElementPtr element = list->firstElement;
    // Free all elements
    while (element != NULL) {
        DLLElementPtr nextElement = element->nextElement;
        free(element);
        element = nextElement;
    }
    // Reset the list
    list->activeElement = NULL;
    list->firstElement = NULL;
    list->lastElement = NULL;
}

/**
 * Vloží nový prvek na začátek seznamu list.
 * V případě, že není dostatek paměti pro nový prvek při operaci malloc,
 * volá funkci DLL_Error().
 *
 * @param list Ukazatel na inicializovanou strukturu dvousměrně vázaného seznamu
 * @param data Hodnota k vložení na začátek seznamu
 */
void DLL_InsertFirst( DLList *list, int data ) {
    // Allocate memory for the new element
    DLLElementPtr new_element = malloc(sizeof(struct DLLElement));
    if (new_element == NULL) {
        DLL_Error();  // Handle memory allocation failure
        return;
    }
    // Set the data for the new element
    new_element->data = data;
    new_element->previousElement = NULL;
    // Update pointers to properly insert the new element at the beginning
    if (list->firstElement == NULL) {
        // The list is empty
        list->firstElement = new_element;
        list->lastElement = new_element;
        new_element->nextElement = NULL;
    } else {
        // The list is not empty
        new_element->nextElement = list->firstElement;
        list->firstElement->previousElement = new_element;
        list->firstElement = new_element;
    }
}

/**
 * Vloží nový prvek na konec seznamu list (symetrická operace k DLL_InsertFirst).
 * V případě, že není dostatek paměti pro nový prvek při operaci malloc,
 * volá funkci DLL_Error().
 *
 * @param list Ukazatel na inicializovanou strukturu dvousměrně vázaného seznamu
 * @param data Hodnota k vložení na konec seznamu
 */
void DLL_InsertLast( DLList *list, int data ) {
    // Allocate memory for the new element
    DLLElementPtr new_element = malloc(sizeof(struct DLLElement));
    if (new_element == NULL) {
        DLL_Error();  // Handle memory allocation failure
        return;
    }
    // Set the data for the new element
    new_element->data = data;
    new_element->nextElement = NULL;

    // Update pointers to properly insert the new element at the end
    if (list->lastElement == NULL) {
        // The list is empty
        list->firstElement = new_element;
        list->lastElement = new_element;
        new_element->previousElement = NULL;
    } else {
        // The list is not empty
        new_element->previousElement = list->lastElement;
        list->lastElement->nextElement = new_element;
        list->lastElement = new_element;
    }
}

/**
 * Nastaví první prvek seznamu list jako aktivní.
 * Funkci implementujte jako jediný příkaz, aniž byste testovali,
 * zda je seznam list prázdný.
 *
 * @param list Ukazatel na inicializovanou strukturu dvousměrně vázaného seznamu
 */
void DLL_First( DLList *list ) {
	    list->activeElement = list->firstElement;
}

/**
 * Nastaví poslední prvek seznamu list jako aktivní.
 * Funkci implementujte jako jediný příkaz, aniž byste testovali,
 * zda je seznam list prázdný.
 *
 * @param list Ukazatel na inicializovanou strukturu dvousměrně vázaného seznamu
 */
void DLL_Last( DLList *list ) {
	    list->activeElement = list->lastElement;
}

/**
 * Prostřednictvím parametru dataPtr vrátí hodnotu prvního prvku seznamu list.
 * Pokud je seznam list prázdný, volá funkci DLL_Error().
 *
 * @param list Ukazatel na inicializovanou strukturu dvousměrně vázaného seznamu
 * @param dataPtr Ukazatel na cílovou proměnnou
 */
void DLL_GetFirst( DLList *list, int *dataPtr ) {
	    if (list->firstElement == NULL) {
        DLL_Error();
        return;
    }
    *dataPtr = list->firstElement->data;//return the value of the first element
}

/**
 * Prostřednictvím parametru dataPtr vrátí hodnotu posledního prvku seznamu list.
 * Pokud je seznam list prázdný, volá funkci DLL_Error().
 *
 * @param list Ukazatel na inicializovanou strukturu dvousměrně vázaného seznamu
 * @param dataPtr Ukazatel na cílovou proměnnou
 */
void DLL_GetLast( DLList *list, int *dataPtr ) {
	    if (list->lastElement == NULL) {
        DLL_Error();
        return;
    }
    *dataPtr = list->lastElement->data;//return the value of the last element
}

/**
 * Zruší první prvek seznamu list.
 * Pokud byl první prvek aktivní, aktivita se ztrácí.
 * Pokud byl seznam list prázdný, nic se neděje.
 *
 * @param list Ukazatel na inicializovanou strukturu dvousměrně vázaného seznamu
 */
void DLL_DeleteFirst( DLList *list ) {
    if (list->firstElement == NULL) return;
    // Save a reference to the first element
    DLLElementPtr element_to_delete = list->firstElement;

    // Update the pointers to remove the first element
    list->firstElement = list->firstElement->nextElement;
    if (list->firstElement != NULL) {
        list->firstElement->previousElement = NULL;
        if(list->activeElement == list->firstElement)list->activeElement = NULL;
    } else {
        // The list is now empty, update the lastElement pointer
        list->lastElement = NULL;
    }

    // Free the memory of the deleted element
    free(element_to_delete);
}

/**
 * Zruší poslední prvek seznamu list.
 * Pokud byl poslední prvek aktivní, aktivita seznamu se ztrácí.
 * Pokud byl seznam list prázdný, nic se neděje.
 *
 * @param list Ukazatel na inicializovanou strukturu dvousměrně vázaného seznamu
 */
void DLL_DeleteLast( DLList *list ) {
    if (list->lastElement == NULL) return;
    // Save a reference to the first element
    DLLElementPtr element_to_delete = list->lastElement;
    // Update the pointers to remove the first element
    list->lastElement = list->lastElement->previousElement;
    if (list->lastElement != NULL) {
        list->lastElement->nextElement = NULL;
        if(list->activeElement == list->lastElement)list->activeElement = NULL;
    } else {
        // The list is now empty, update the lastElement pointer
        list->firstElement = NULL;
    }
    // Free the memory of the deleted element
    free(element_to_delete);
}

/**
 * Zruší prvek seznamu list za aktivním prvkem.
 * Pokud je seznam list neaktivní nebo pokud je aktivní prvek
 * posledním prvkem seznamu, nic se neděje.
 *
 * @param list Ukazatel na inicializovanou strukturu dvousměrně vázaného seznamu
 */
void DLL_DeleteAfter( DLList *list ) {
    if (list->activeElement == NULL || list->activeElement->nextElement == NULL) return;
    // Save a reference to the element to delete
    DLLElementPtr element_to_delete = list->activeElement->nextElement;
    // Update the pointers to remove the element
    list->activeElement->nextElement = element_to_delete->nextElement;
    if (element_to_delete->nextElement != NULL) {
        element_to_delete->nextElement->previousElement = list->activeElement;
    } else {
        // The deleted element was the last element, update the lastElement pointer
        list->lastElement = list->activeElement;
    }
    // Free the memory of the deleted element
    free(element_to_delete);
}

/**
 * Zruší prvek před aktivním prvkem seznamu list .
 * Pokud je seznam list neaktivní nebo pokud je aktivní prvek
 * prvním prvkem seznamu, nic se neděje.
 *
 * @param list Ukazatel na inicializovanou strukturu dvousměrně vázaného seznamu
 */
void DLL_DeleteBefore( DLList *list ) {
    if (list->activeElement == NULL || list->activeElement->previousElement == NULL) return;
    // Save a reference to the element to delete
    DLLElementPtr element_to_delete = list->activeElement->previousElement;
    // Update the pointers to remove the element
    list->activeElement->previousElement = element_to_delete->previousElement;
    if (element_to_delete->previousElement != NULL) {
        element_to_delete->previousElement->nextElement = list->activeElement;
    } else {
        // The deleted element was the first element, update the firstElement pointer
        list->firstElement = list->activeElement;
    }
    // Free the memory of the deleted element
    free(element_to_delete);
}

/**
 * Vloží prvek za aktivní prvek seznamu list.
 * Pokud nebyl seznam list aktivní, nic se neděje.
 * V případě, že není dostatek paměti pro nový prvek při operaci malloc,
 * volá funkci DLL_Error().
 *
 * @param list Ukazatel na inicializovanou strukturu dvousměrně vázaného seznamu
 * @param data Hodnota k vložení do seznamu za právě aktivní prvek
 */
void DLL_InsertAfter( DLList *list, int data ) {
    if (list->activeElement == NULL) return;
    // Allocate memory for the new element
    DLLElementPtr new_element = malloc(sizeof(struct DLLElement));
    if (new_element == NULL) {
        DLL_Error();  // Handle memory allocation failure
        return;
    }
    // Set the data for the new element
    new_element->data = data;
    // Update pointers to properly insert the new element after the active element
    new_element->nextElement = list->activeElement->nextElement;
    new_element->previousElement = list->activeElement;
    list->activeElement->nextElement = new_element;
    if (new_element->nextElement != NULL) {
        new_element->nextElement->previousElement = new_element;
    } else {
        // The new element is the last element, update the lastElement pointer
        list->lastElement = new_element;
    }
}

/**
 * Vloží prvek před aktivní prvek seznamu list.
 * Pokud nebyl seznam list aktivní, nic se neděje.
 * V případě, že není dostatek paměti pro nový prvek při operaci malloc,
 * volá funkci DLL_Error().
 *
 * @param list Ukazatel na inicializovanou strukturu dvousměrně vázaného seznamu
 * @param data Hodnota k vložení do seznamu před právě aktivní prvek
 */
void DLL_InsertBefore( DLList *list, int data ) {
    if (list->activeElement == NULL) return;
    // Allocate memory for the new element
    DLLElementPtr new_element = malloc(sizeof(struct DLLElement));
    if (new_element == NULL) {
        DLL_Error();  // Handle memory allocation failure
        return;
    }
    // Set the data for the new element
    new_element->data = data;
    // Update pointers to properly insert the new element before the active element
    new_element->nextElement = list->activeElement;
    new_element->previousElement = list->activeElement->previousElement;
    list->activeElement->previousElement = new_element;
    if (new_element->previousElement != NULL) {
        new_element->previousElement->nextElement = new_element;
    } else {
        // The new element is the first element, update the firstElement pointer
        list->firstElement = new_element;
    }
}

/**
 * Prostřednictvím parametru dataPtr vrátí hodnotu aktivního prvku seznamu list.
 * Pokud seznam list není aktivní, volá funkci DLL_Error ().
 *
 * @param list Ukazatel na inicializovanou strukturu dvousměrně vázaného seznamu
 * @param dataPtr Ukazatel na cílovou proměnnou
 */
void DLL_GetValue( DLList *list, int *dataPtr ) {
    if (list->activeElement == NULL) {
        DLL_Error();
        return;
    }
    *dataPtr = list->activeElement->data;
}

/**
 * Přepíše obsah aktivního prvku seznamu list.
 * Pokud seznam list není aktivní, nedělá nic.
 *
 * @param list Ukazatel na inicializovanou strukturu dvousměrně vázaného seznamu
 * @param data Nová hodnota právě aktivního prvku
 */
void DLL_SetValue( DLList *list, int data ) {
	    if (list->activeElement == NULL) return;
    list->activeElement->data = data;
}

/**
 * Posune aktivitu na následující prvek seznamu list.
 * Není-li seznam aktivní, nedělá nic.
 * Všimněte si, že při aktivitě na posledním prvku se seznam stane neaktivním.
 *
 * @param list Ukazatel na inicializovanou strukturu dvousměrně vázaného seznamu
 */
void DLL_Next( DLList *list ) {
	    if (list->activeElement == NULL) return;
    list->activeElement = list->activeElement->nextElement;
}


/**
 * Posune aktivitu na předchozí prvek seznamu list.
 * Není-li seznam aktivní, nedělá nic.
 * Všimněte si, že při aktivitě na prvním prvku se seznam stane neaktivním.
 *
 * @param list Ukazatel na inicializovanou strukturu dvousměrně vázaného seznamu
 */
void DLL_Previous( DLList *list ) {
	    if (list->activeElement != NULL)
        list->activeElement = list->activeElement->previousElement;
}

/**
 * Je-li seznam list aktivní, vrací nenulovou hodnotu, jinak vrací 0.
 * Funkci je vhodné implementovat jedním příkazem return.
 *
 * @param list Ukazatel na inicializovanou strukturu dvousměrně vázaného seznamu
 *
 * @returns Nenulovou hodnotu v případě aktivity prvku seznamu, jinak nulu
 */
int DLL_IsActive( DLList *list ) {
	    return list->activeElement != NULL;
}

/* Konec c206.c */
