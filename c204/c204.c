/* ******************************* c204.c *********************************** */
/*  Předmět: Algoritmy (IAL) - FIT VUT v Brně                                 */
/*  Úkol: c204 - Převod infixového výrazu na postfixový (s využitím c202)     */
/*  Referenční implementace: Petr Přikryl, listopad 1994                      */
/*  Přepis do jazyka C: Lukáš Maršík, prosinec 2012                           */
/*  Upravil: Kamil Jeřábek, září 2019                                         */
/*           Daniel Dolejška, září 2021                                       */
/* ************************************************************************** */

/*Implementácia obsahu funkcií*/
/*Aleš Kubiš, xkubisa00, 269924*/
/*VUT FIT 2bit*/
/*
** Implementujte proceduru pro převod infixového zápisu matematického výrazu
** do postfixového tvaru. Pro převod využijte zásobník (Stack), který byl
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
	if(stack && postfixExpression && postfixExpressionLength){	//check validity of arguments
		char top_sign;
		while(!Stack_IsEmpty(stack)){ //if the stack is empty then this cycle will not be executed
			Stack_Top(stack, &top_sign);
			if(top_sign != '('){
				//operation with stack
				Stack_Pop(stack);
				//append extracted char to string
				postfixExpression[*postfixExpressionLength] = top_sign;
				//updates index
				*postfixExpressionLength = *postfixExpressionLength + 1;
			}else if(top_sign == '('){
				Stack_Pop(stack);
				return;
			}
		}
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
	if(stack && postfixExpression && postfixExpressionLength){ //checks all pointers validity
		char top_sign;
		if(!Stack_IsEmpty(stack)) Stack_Top(stack, &top_sign);
		switch (c) {
			case '+':
			case '-':
				if((Stack_IsEmpty(stack) || top_sign == '(') && !Stack_IsFull(stack)){ //if we can push operand to stack without poping anything
					Stack_Push(stack, c);
					return; //after push this function can be ended
				}
				while(!Stack_IsEmpty(stack)){	//pops all operands from stack until there is only '(' or until the stack is empty
												//since we are trying to push low priority operand these are the only two scenarios
												//when we can push it to stack
					if(top_sign == '('){
						break;
					}
					postfixExpression[*postfixExpressionLength] = top_sign;
					*postfixExpressionLength = *postfixExpressionLength + 1;
					Stack_Pop(stack);
					if(!Stack_IsEmpty(stack)) Stack_Top(stack, &top_sign);
				}
				Stack_Push(stack, c);
				break;
			case '*':
			case '/': 
				if((Stack_IsEmpty(stack) || top_sign == '(' || top_sign == '+' || top_sign == '-') && !Stack_IsFull(stack)){ //the same as above + now we can also push directly if there is '+' or '-'
					Stack_Push(stack ,c);
					return;
				}
				while((top_sign != '+' || top_sign != '-' || top_sign != '(' ) && !Stack_IsEmpty(stack)){ //pops values from stack and appends them to postfix_expression until we can push 'c' to stack
					postfixExpression[*postfixExpressionLength] = top_sign;
					*postfixExpressionLength = *postfixExpressionLength + 1;
					Stack_Pop(stack);
					if(!Stack_IsEmpty(stack)) Stack_Top(stack, &top_sign);
				}
				Stack_Push(stack, c);
				break;
		}
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
	if(!infixExpression) return NULL;	//checks validity of pointer

	char *postfix_expresion = (char*) malloc(MAX_LEN * sizeof(char)); 
	if(!postfix_expresion) return NULL;

	Stack stack; //stack initialization
	Stack_Init(&stack);

	char c = *infixExpression;
	unsigned postfix_length = 0;
	char top_sign;
	while(c != '\0'){
		if(postfix_length >= MAX_LEN) break; //checks if the postfix expression is not larger than allocated memmory
		switch (c){
			case '=':
				while(!Stack_IsEmpty(&stack)){ //pops everything that is left in stack and appends it to postfix_expression
					Stack_Top(&stack, &top_sign);
					Stack_Pop(&stack);
					postfix_expresion[postfix_length] = top_sign;
					postfix_length++;
				}
				postfix_expresion[postfix_length] = '=';
				postfix_length++;
				postfix_expresion[postfix_length] = '\0';
				postfix_length++;
				break;
			case '(':
				Stack_Push(&stack, c);
				break;
			case ')':
				untilLeftPar(&stack, postfix_expresion, &postfix_length);
				break;
			case '+':
			case '-':
			case '*':
			case '/':
				doOperation(&stack, c, postfix_expresion, &postfix_length);
				break;
			default:
				postfix_expresion[postfix_length] = c;	//operators are automaticly appended to postfix_expresion
				postfix_length++;
				break;
		}

		infixExpression++;
		c = *infixExpression;
	}
	Stack_Dispose(&stack);
	return postfix_expresion;
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
	if(stack){
		const int eight = 8; //this constant is used for shifting through bits
		char one_byte;
		for(int i = 0; i <= 3; i++){
			one_byte = ((((1ull << (eight)) - 1) << (i * eight)) & value) >> (i * eight); //creates mask, maskas of 8bits starting on i*eight,
																						  //performs 'and' operaton with value, shifts masked value all the way right
			Stack_Push(stack, one_byte);
		}
	}
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
	if(stack){
		int eight = 8;
		char top_value;

		for(int i = 3; i >= 0; i--){
			*value = 0; //all bits in value are set to 0 for correct 'or' operation
			if(!Stack_IsEmpty(stack)) Stack_Top(stack, &top_value);
			Stack_Pop(stack);
			*value = ((int)top_value << (eight * i)) | *value;	//creates int form top_value(char), since we are poping values from the
																//oppposite side we have to first shit the bytes all the way left
																//and then perform 'or' operation vith value
		}
	}
}

/**
 * Performs calculation over two operators.
 * 
 * @param operator_1 first operator
 * @param operator_2 second operator
 * @param operand operand to perform calculation with
 * 
 * @return value after calculation
 */
int calculation(int operator_1, int operator_2, char operand){
	switch (operand){
	case '+':
		return (operator_1 + operator_2);
		break;
	case '-':
		return (operator_1 - operator_2);
		break;
	case '*':
		return (operator_1 * operator_2);
		break;
	case '/':
		return (operator_1 / operator_2);
		break;
	}
	return -1;
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
 * @param infixExpression vstupní infixový výraz s proměnnými
 * @param variableValues hodnoty proměnných ze vstupního výrazu
 * @param variableValueCount počet hodnot (unikátních proměnných
 *   ve vstupním výrazu)
 * @param value ukazatel na celočíselnou proměnnou pro uložení
 *   výsledné hodnoty vyhodnocení vstupního výrazu
 *
 * @returns true v případě úspěšného vyhodnocení daného výrazu
 * 	 na základě poskytnutých hodnot proměnných, false jinak
 */
bool eval( const char *infixExpression, VariableValue variableValues[], int variableValueCount, int *value ) {
	if(infixExpression && value){
		char* postfix_expression = infix2postfix(infixExpression);
		
		//declaration or initialization of variables and structures
		char* postfix_expression_beginning = postfix_expression;	//variable used for free()
		Stack stack;
		Stack_Init(&stack);
		int first_operator, second_operator;
		bool var_found;
		
		while(*postfix_expression != '\0'){	//iterates over whole expresion from left to right
			switch (*postfix_expression){
			case '=':
				expr_value_pop(&stack, value);	//the result is stored on top of stack
				break;
			case '+':
			case '-':
			case '*':
			case '/':
				expr_value_pop(&stack, &second_operator);
				expr_value_pop(&stack, &first_operator);	//all our operators are dyadic
				
				if(*postfix_expression == '/' && second_operator == 0){	//division by zero
					free(postfix_expression_beginning);
					Stack_Dispose(&stack);
					return false;
				}

				expr_value_push(&stack, calculation(first_operator, second_operator, *postfix_expression)); //performs calculation and pushes value to stack
				break;
			default:	//operand is found
				var_found = false;
				for(int i = 0; i < variableValueCount; i++){	//iterates over variableValues
					if(variableValues[i].name == *postfix_expression){	//pushes variable value to stack if match is found
						var_found = true;
						expr_value_push(&stack, variableValues[i].value);
						break;
					}
				}
				if(var_found == false){ //we didn't find value for variable in variableValue[]
					free(postfix_expression_beginning);
					Stack_Dispose(&stack);
					return false;
				}
			}


			postfix_expression++; //since we are incrementing poiner we need to store pointer to the beggining of array for free() -> postfix_expression_beginning
		}


		free(postfix_expression_beginning);
		Stack_Dispose(&stack);
		return true;
	}
	return false;
}

/* Konec c204.c */
