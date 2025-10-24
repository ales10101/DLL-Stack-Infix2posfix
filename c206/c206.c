/* ******************************* c206.c *********************************** */
/*  Předmět: Algoritmy (IAL) - FIT VUT v Brně                                 */
/*  Úkol: c206 - Dvousměrně vázaný lineární seznam                            */
/*  Návrh a referenční implementace: Bohuslav Křena, říjen 2001               */
/*  Vytvořil: Martin Tuček, říjen 2004                                        */
/*  Upravil: Kamil Jeřábek, září 2020                                         */
/*           Daniel Dolejška, září 2021                                       */
/*           Daniel Dolejška, září 2022                                       */
/* ************************************************************************** */

/*Implementácia obsahu funkcií*/
/*Aleš Kubiš, xkubisa00, 269924*/
/*VUT FIT 2bit*/
/*
** Implementujte abstraktní datový typ dvousměrně vázaný lineární seznam.
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
**      DLL_Init ........... inicializace seznamu před prvním použitím, DONE
**      DLL_Dispose ........ zrušení všech prvků seznamu, DONE
**      DLL_InsertFirst .... vložení prvku na začátek seznamu, DONE
**      DLL_InsertLast ..... vložení prvku na konec seznamu, DONE
**      DLL_First .......... nastavení aktivity na první prvek, DONE
**      DLL_Last ........... nastavení aktivity na poslední prvek, DONE
**      DLL_GetFirst ....... vrací hodnotu prvního prvku, DONE
**      DLL_GetLast ........ vrací hodnotu posledního prvku, DONE
**      DLL_DeleteFirst .... zruší první prvek seznamu, DONE
**      DLL_DeleteLast ..... zruší poslední prvek seznamu, DONE
**      DLL_DeleteAfter .... ruší prvek za aktivním prvkem, DONE
**      DLL_DeleteBefore ... ruší prvek před aktivním prvkem,DONE
**      DLL_InsertAfter .... vloží nový prvek za aktivní prvek seznamu, DONE
**      DLL_InsertBefore ... vloží nový prvek před aktivní prvek seznamu, DONE
**      DLL_GetValue ....... vrací hodnotu aktivního prvku, DONE
**      DLL_SetValue ....... přepíše obsah aktivního prvku novou hodnotou, DONE
**      DLL_Previous ....... posune aktivitu na předchozí prvek seznamu, DONE 
**      DLL_Next ........... posune aktivitu na další prvek seznamu, DONE
**      DLL_IsActive ....... zjišťuje aktivitu seznamu. DONE
**
** Při implementaci jednotlivých funkcí nevolejte žádnou z funkcí
** implementovaných v rámci tohoto příkladu, není-li u funkce explicitně
** uvedeno něco jiného.
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
	if(list){	//makes sure that *list != NULL
		list->activeElement = NULL;
		list->firstElement = NULL;
		list->lastElement = NULL;
		list->currentLength = 0;
	}
}

/**
 * Zruší všechny prvky seznamu list a uvede seznam do stavu, v jakém se nacházel
 * po inicializaci.
 * Rušené prvky seznamu budou korektně uvolněny voláním operace free.
 *
 * @param list Ukazatel na inicializovanou strukturu dvousměrně vázaného seznamu
 */
void DLL_Dispose( DLList *list ) {
	if(list && list->firstElement){ //checks if there are elements stored in list
		DLLElementPtr tmp_ptr;
		while(list->currentLength > 0){
			if(list->currentLength > 1){	//if there are multiple elements stored
				tmp_ptr = list->firstElement;
				list->firstElement = list->firstElement->nextElement;
				list->firstElement->previousElement = NULL;
			}else{	//if there is last element stored - we don't care about next/previous pointers
				tmp_ptr = list->firstElement;
			}
			free(tmp_ptr);
			list->currentLength--;
		}


		list->activeElement = NULL;	//sets all pointers to default value
		list->firstElement = NULL;
		list->lastElement = NULL;
	}
}

/**
 * Vloží nový prvek na začátek seznamu list.
 * V případě, že není dostatek paměti pro nový prvek při operaci malloc,
 * volá funkci DLL_Error().
 *
 * @param list Ukazatel na inicializovanou strukturu dvousměrně vázaného seznamu
 * @param data Hodnota k vložení na začátek seznamu
 */
void DLL_InsertFirst( DLList *list, long data ) {
	if(list){
		//allocates memmory for new element
		DLLElementPtr new_element = (DLLElementPtr)  malloc(sizeof(*new_element));
		if(!new_element){
			DLL_Error();
		}else{ //allocation was successful
			new_element->data = data;
			if(list->currentLength == 0){ //if this is the first and only element
				list->firstElement = new_element;
				list->lastElement = new_element;
				new_element->previousElement = NULL;
				new_element->nextElement = NULL;
			}else{ //if there alredy are elements stored
				list->firstElement->previousElement = new_element;
				new_element->nextElement = list->firstElement;
				list->firstElement = new_element;
				new_element->previousElement = NULL;
			}
			//updates length
			list->currentLength++;
		}
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
void DLL_InsertLast( DLList *list, long data ) {
		if(list){
		//allocates memmory for new element
		DLLElementPtr new_element = (DLLElementPtr) malloc(sizeof(*new_element));
		if(!new_element){
			DLL_Error();
		}else{ //allocation was successful
			new_element->data = data;
			if(list->currentLength == 0){ //if this is the first and only element
				list->firstElement = new_element;
				list->lastElement = new_element;
				new_element->previousElement = NULL;
				new_element->nextElement = NULL;
			}else{ //if there alredy are elements stored
				list->lastElement->nextElement = new_element;
				new_element->previousElement = list->lastElement;
				list->lastElement = new_element;
				new_element->nextElement = NULL;
			}
			//updates length
			list->currentLength++;
		}
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
void DLL_GetFirst( DLList *list, long *dataPtr ) {
	if(list && dataPtr){
		if(!list->firstElement){
			DLL_Error();
		}else{
			*dataPtr = list->firstElement->data;
		}
	}
}

/**
 * Prostřednictvím parametru dataPtr vrátí hodnotu posledního prvku seznamu list.
 * Pokud je seznam list prázdný, volá funkci DLL_Error().
 *
 * @param list Ukazatel na inicializovanou strukturu dvousměrně vázaného seznamu
 * @param dataPtr Ukazatel na cílovou proměnnou
 */
void DLL_GetLast( DLList *list, long *dataPtr ) {
	if(list && dataPtr){
		if(!list->lastElement){
			DLL_Error();
		}else{
			*dataPtr = list->lastElement->data;
		}
	}
}

/**
 * Zruší první prvek seznamu list.
 * Pokud byl první prvek aktivní, aktivita se ztrácí.
 * Pokud byl seznam list prázdný, nic se neděje.
 *
 * @param list Ukazatel na inicializovanou strukturu dvousměrně vázaného seznamu
 */
void DLL_DeleteFirst( DLList *list ) {
	if(list && list->firstElement){  //checks if there are elements stored in list
		DLLElementPtr tmp_ptr;
		if(list->activeElement == list->firstElement){
			list->activeElement = NULL;
		}
		if(list->currentLength > 1){	//if there are multiple elements sotred
			tmp_ptr = list->firstElement;
			list->firstElement = list->firstElement->nextElement;
			list->firstElement->previousElement = NULL;
		}else{	//if there is last element stored
			tmp_ptr = list->firstElement;
			list->firstElement = NULL;
			list->lastElement = NULL;
			list->activeElement = NULL;
		}
		free(tmp_ptr);
		list->currentLength--;
	}
}

/**
 * Zruší poslední prvek seznamu list.
 * Pokud byl poslední prvek aktivní, aktivita seznamu se ztrácí.
 * Pokud byl seznam list prázdný, nic se neděje.
 *
 * @param list Ukazatel na inicializovanou strukturu dvousměrně vázaného seznamu
 */
void DLL_DeleteLast( DLList *list ) {
	if(list && list->lastElement){ //checks if there are elements stored in list
		DLLElementPtr tmp_ptr;
		if(list->lastElement == list->activeElement){
			list->activeElement = NULL;
		}
		if(list->currentLength > 1){	//if there are multiple elements sotred
			tmp_ptr = list->lastElement;
			list->lastElement = list->lastElement->previousElement;
			tmp_ptr->previousElement->nextElement = NULL;
			tmp_ptr->previousElement = NULL;
		}else{	//if there is last element stored
			tmp_ptr = list->lastElement;
			list->firstElement = NULL;
			list->lastElement = NULL;
			list->activeElement = NULL;
		}
		free(tmp_ptr);
		list->currentLength--;
	}
}

/**
 * Zruší prvek seznamu list za aktivním prvkem.
 * Pokud je seznam list neaktivní nebo pokud je aktivní prvek
 * posledním prvkem seznamu, nic se neděje.
 *
 * @param list Ukazatel na inicializovanou strukturu dvousměrně vázaného seznamu
 */
void DLL_DeleteAfter( DLList *list ) {
	if(list && list->activeElement && list->activeElement != list->lastElement && list->currentLength > 0){ //makes sure that there is an active element and that it is not the last element
		DLLElementPtr tmp_ptr = list->activeElement->nextElement;
		if(list->activeElement->nextElement == list->lastElement){ //if we are deleting the last element
			list->lastElement = list->activeElement;
			list->activeElement->nextElement = NULL;
		}else{ //we are deleting element stored somewhere else
			list->activeElement->nextElement = tmp_ptr->nextElement;
			tmp_ptr->nextElement->previousElement = list->activeElement;
		}
		free(tmp_ptr);
		list->currentLength--;
	}
}

/**
 * Zruší prvek před aktivním prvkem seznamu list .
 * Pokud je seznam list neaktivní nebo pokud je aktivní prvek
 * prvním prvkem seznamu, nic se neděje.
 *
 * @param list Ukazatel na inicializovanou strukturu dvousměrně vázaného seznamu
 */
void DLL_DeleteBefore( DLList *list ) {
	if(list && list->activeElement && list->activeElement != list->firstElement && list->currentLength > 0){//makes sure that there is an active element and that it is not the first element
		DLLElementPtr tmp_ptr = list->activeElement->previousElement;
		if(list->activeElement->previousElement == list->firstElement){
			list->firstElement = list->activeElement;
			list->firstElement->previousElement = NULL;
		}else{	//we are deleting element stored somewhere else
			list->activeElement->previousElement = tmp_ptr->previousElement;
			tmp_ptr->previousElement->nextElement = list->activeElement;
		}
		free(tmp_ptr);
		list->currentLength--;
	}
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
void DLL_InsertAfter( DLList *list, long data ) {
	if(list && list->activeElement){
		// allocates memmory for new element
		DLLElementPtr new_element = (DLLElementPtr) malloc(sizeof(*new_element));
		if(!new_element){
			DLL_Error();
		}else{ //allocation was successful
			new_element->data = data;
			if(list->activeElement == list->lastElement){ //if the current active element is last
				new_element->previousElement = list->activeElement;
				list->activeElement->nextElement = new_element;
				list->lastElement = new_element;
				new_element->nextElement = NULL;
			}else{	//if the current active element is not last
				new_element->nextElement = list->activeElement->nextElement;
				list->activeElement->nextElement->previousElement = new_element;
				list->activeElement->nextElement = new_element;
				new_element->previousElement = list->activeElement;
			}
			list->currentLength++;
		}
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
void DLL_InsertBefore( DLList *list, long data ) {
	if(list && list->activeElement){
		DLLElementPtr new_element = (DLLElementPtr) malloc(sizeof(*new_element));
		if(!new_element){
			DLL_Error();
		}else{ //allocation was successful
			new_element->data = data;
			if(list->activeElement == list->firstElement){//if the current active element is first
				new_element->nextElement = list->activeElement;
				list->activeElement->previousElement = new_element;
				list->firstElement = new_element;
				new_element->previousElement = NULL;
			}else{	//if the current active element is not first
				new_element->previousElement = list->activeElement->previousElement;
				list->activeElement->previousElement->nextElement = new_element;
				new_element->nextElement = list->activeElement;
				list->activeElement->previousElement = new_element;
			}
			list->currentLength++;
		}
	}
}

/**
 * Prostřednictvím parametru dataPtr vrátí hodnotu aktivního prvku seznamu list.
 * Pokud seznam list není aktivní, volá funkci DLL_Error ().
 *
 * @param list Ukazatel na inicializovanou strukturu dvousměrně vázaného seznamu
 * @param dataPtr Ukazatel na cílovou proměnnou
 */
void DLL_GetValue( DLList *list, long *dataPtr ) {
	if(list && dataPtr){
		if(!list->activeElement){
			DLL_Error();
		}else{
			*dataPtr = list->activeElement->data;
		}
	}
}

/**
 * Přepíše obsah aktivního prvku seznamu list.
 * Pokud seznam list není aktivní, nedělá nic.
 *
 * @param list Ukazatel na inicializovanou strukturu dvousměrně vázaného seznamu
 * @param data Nová hodnota právě aktivního prvku
 */
void DLL_SetValue( DLList *list, long data ) {
	if(list && list->activeElement){
		list->activeElement->data = data;
	}
}

/**
 * Posune aktivitu na následující prvek seznamu list.
 * Není-li seznam aktivní, nedělá nic.
 * Všimněte si, že při aktivitě na posledním prvku se seznam stane neaktivním.
 *
 * @param list Ukazatel na inicializovanou strukturu dvousměrně vázaného seznamu
 */
void DLL_Next( DLList *list ) {
	if(list && list->activeElement){
		if(list->activeElement == list->lastElement){
			list->activeElement = NULL;
		}else{
			list->activeElement = list->activeElement->nextElement;
		}
	}
}


/**
 * Posune aktivitu na předchozí prvek seznamu list.
 * Není-li seznam aktivní, nedělá nic.
 * Všimněte si, že při aktivitě na prvním prvku se seznam stane neaktivním.
 *
 * @param list Ukazatel na inicializovanou strukturu dvousměrně vázaného seznamu
 */
void DLL_Previous( DLList *list ) {
	if(list && list->activeElement){
		if(list->activeElement == list->firstElement){
			list->activeElement = NULL;
		}else{
			list->activeElement = list->activeElement->previousElement;
		}
	}
}

/**
 * Je-li seznam list aktivní, vrací nenulovou hodnotu, jinak vrací 0.
 * Funkci je vhodné implementovat jedním příkazem return.
 *
 * @param list Ukazatel na inicializovanou strukturu dvousměrně vázaného seznamu
 *
 * @returns Nenulovou hodnotu v případě aktivity prvku seznamu, jinak nulu
 */
bool DLL_IsActive( DLList *list ) {
	return list->activeElement != NULL;
}

/* Konec c206.c */
