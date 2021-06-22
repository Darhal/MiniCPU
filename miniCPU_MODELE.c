/*
 * -------------------------- TP du module Archi -------------------------
 *
 * ATTENTION : un outil de d�tection de plagiat logiciel sera utilis� lors de la correction, vous avez donc tout int�r�t � effectuer un travail PERSONNEL
 *
 * Un mot/registre de NBITS bits (par d�faut NBITS=16) est repr�sent� par un tableau d'entiers �gaux � 0 ou 1.
 * Une ALU est repr�sent�e par une structure ALU, avec registre accumulateur et registre d'�tat.
 * Un CPU (tr�s tr�s simplifi�) est repr�sent� par une ALU et quelques registres n�cessaires pour stocker les r�sultats interm�diaires.
 *
 * Certaines fonctions vous sont fournies, d'autres sont � implanter ou � compl�ter, de pr�f�rence dans l'ordre o� eles sont indiqu�es.
 * Il vous est fortement conseill� de lire attentivement l'ensemble des commentaires.
 *
 * Parmi les op�rations arithm�tiques et logiques, seules 4 op�rations de base sont directement fournies par l'ALU, les autres doivent �tre d�crites comme des algorithmes
 * travaillant � l'aide des op�rateurs de base de l'ALU simplifi�e et pouvant utiliser les registres du CPU.
 *
 * La fonction main() vous permet de tester au fur et � mesure les fonctions que vous implantez.
 *
 * ----------------------------------------------------------------------------------------------
 *
 * author: B. Girau
 * version: 2018-19
 */
#include <stdio.h>
#include <stdlib.h>

#pragma warning(disable:4996)

#define NBITS 16  // attention, votre programme doit pouvoir �tre adapt� � d'autres tailles juste en modifiant la valeur de cette constante
 // en ayant toujours NBITS < 32

char* toString(int* word);

/////////////////////////////////////////////////////////
// d�finition de types
/////////////////////////////////////////////////////////

typedef struct {
	int* accu;
	int* flags; // indicateurs ZF CF OF NF
} ALU;

typedef struct {
	ALU alu;
	int* R0;
	int* R1;
	int* R2;
} CPU;

/////////////////////////////////////////////////////////
// fonctions d'initialisation
/////////////////////////////////////////////////////////

/*
 * allocation d'un mot entier de NBITS bits initialis� � 0
 */
int* word() {
	int* tab;
	int i;
	tab = (int*)malloc(NBITS * sizeof(int));
	for (i = 0; i < NBITS; i++) tab[i] = 0;
	// poids faible : tab[0]
	// poids fort : tab[NBITS-1]
	return tab;
}

/*
 * Initialisation du mot (mot de NBITS bits, codant un entier en C�2) avec une valeur enti�re.
 */
void setValue(int* word, int n) {
	int n_low = n & 0x7FFFFFFF;
	// revient � mettre � 0 le bit de poids fort en 32 bits
	// on peut alors travailler sur la partie positive du codage de n
	// remarque : si n est bien codable en Ca2 sur NBITS, et si n est n�gatif, 
	// on r�cup�re quand m�me le codage de n sur NBITS en Ca2 en prenant les NBITS de 
	// poids faible de n_low
	// � compl�ter
	int i, c;
	char neg = 0;
	if (n < 0) { neg = 1; n = -n; }
	for (i = NBITS - 1; i >= 0; i--) {
		c = n >> i;
		if (c & 1)
			word[i] = 1;
		else
			word[i] = 0;
	}
	if (neg) {
		char dontChange = 0;
		for (i = 0; i < NBITS; i++) {
			word[i] = (word[i] == 0) ? 1 : 0; // C�1
			if (!dontChange) { // C�2
				if (word[i] == 1)
					word[i] = 0;
				else {
					word[i] = 1;
					dontChange = 1;
				}
			}
		}
	}
}

/*
 * instanciation d'un mot de NBITS bits initialis� avec la valeur n
 */
int* initWord(int n) {
	int* tab = word();
	setValue(tab, n);
	return tab;
}

/*
 * Initialisation du mot (mot de NBITS bits) par recopie des bits du mot en param�tre.
 */
void copyValue(int* word, int* src) {
	// � compl�ter
	int i;
	for(i=0;i<NBITS;i++){
		word[i] = src[i];
	}
}

/*
 * instanciation d'un mot de NBITS bits initialis� par recopie d'un mot
 */
int* copyWord(int* src) {
	int* tab = word();
	copyValue(tab, src);
	return tab;
}

/*
 * initialise l'ALU
 */
ALU initALU() {
	ALU res;
	res.accu = word();
	res.flags = (int*)malloc(4 * sizeof(int));
	return res;
}

/*
 * initialise le CPU
 */
CPU initCPU() {
	CPU res;
	res.alu = initALU();
	res.R0 = (int*)malloc(NBITS * sizeof(int));
	res.R1 = (int*)malloc(NBITS * sizeof(int));
	res.R2 = (int*)malloc(NBITS * sizeof(int));
	return res;
}

/////////////////////////////////////////////////////////
// fonctions de lecture
/////////////////////////////////////////////////////////

/*
 * Retourne la valeur enti�re sign�e repr�sent�e par le mot (compl�ment � 2).
 */
int intValue(int* word) {
	// � compl�ter
	int res = 0;
	int i = 0;
	int* InitialWorld = word;
	while (i < NBITS-1) {
		res += (*word)*((2 << i) / 2);
		i++;
		word++;
	}
	res += -(*word)*((2 << i) / 2);
	return res;
}

/*
 * Retourne une cha�ne de caract�res d�crivant les NBITS bits
 */
char* toString(int* word) {
	int i, j = 0;
	char* s = (char*)malloc((10 + NBITS) * sizeof(char));
	for (i = NBITS - 1; i >= 0; i--) {
		if (word[i] == 1) s[j] = '1';
		else s[j] = '0';
		j++;
	}
	return s;
}

/*
 * Retourne l'�criture des indicateurs associ�s � l'ALU.
 */
char* flagsToString(ALU alu) {
	char *string = (char*)malloc(10 * sizeof(char));
	sprintf(string, "z%dc%do%dn%d", alu.flags[0], alu.flags[1], alu.flags[2], alu.flags[3]);
	return string;
}

/*
 * affiche � l'�cran le contenu d'une ALU
 */
void printing(ALU alu) {
	// � compl�ter
	printf("accu = %s - ", toString(alu.accu));
	printf("flags = %s\n", flagsToString(alu));
}

/////////////////////////////////////////////////////////
// fonctions de manipulations �l�mentaires
/////////////////////////////////////////////////////////

/*
 * Mise � la valeur b du bit sp�cifi� dans le mot
 */
void set(int* word, int bitIndex, int b) {
	if ((bitIndex > NBITS - 1) || (bitIndex < 0))
		printf("valeur d'index incorrecte\n");
	word[bitIndex] = b;
}

/*
 * Retourne la valeur du bit sp�cifi� dans le mot
 */
int get(int* word, int bitIndex) {
	if ((bitIndex > NBITS - 1) || (bitIndex < 0))
		printf("valeur d'index incorrecte\n");
	return word[bitIndex];
}

/*
 * Positionne l'indicateur ZF en fonction de l'�tat de l'accumulateur
 */
void setZ(ALU alu) {
	// � compl�ter
	int i = 0;
	while (i < NBITS) {
		if (get(alu.accu, i) != 0) {
			alu.flags[0] = 0;
			return;
		}
		i++;
	}
	alu.flags[0] = 1;
}

void setC(ALU alu, int bitCF)
{
	alu.flags[1] = bitCF;
}

void setO(ALU alu, int bitOF)
{
	alu.flags[2] = bitOF;
}

void setN(ALU alu)
{
	alu.flags[3] = alu.accu[NBITS-1];
}
/////////////////////////////////////////////////////////
// op�rateurs de base de l'ALU
// IMPORTANT : les indicateurs doivent �tre mis � jour
/////////////////////////////////////////////////////////

/*
 * Stocke le param�tre dans le registre accumulateur
 */
void pass(ALU alu, int* B) {
	// � compl�ter
	copyValue(alu.accu, B);
	setZ(alu);
	setC(alu, 0);
	setO(alu, 0);
	setN(alu);
}

/*
 * Effectue un NAND (NON-ET) entre le contenu de l'accumulateur et le param�tre.
 */
void nand(ALU alu, int* B) {
	// � compl�ter
	int i = 0;
	while (i < NBITS) {
		int bitAccu = get(alu.accu, i);
		int bitB = get(B, i);
		set(alu.accu, i, !(bitAccu && bitB));
		i++;
	}
	setZ(alu);
	setC(alu, 0);
	setO(alu, 0);
	setN(alu);
}

/*
 * D�cale le contenu de l'accumulateur de 1 bit vers la droite
 */
void shift(ALU alu) {
	// � compl�ter
	int temp[NBITS];
	copyValue(temp, alu.accu);
	int i = 0;
	while (i < NBITS - 1) {
		set(alu.accu, i, temp[i + 1]);
		i++;
	}
	set(alu.accu, NBITS - 1, 0);
	setZ(alu);
	setC(alu, 0);
	setO(alu, 0);
	setN(alu);
}

/*
 * module Full Adder : a+b+c_in = s + 2 c_out
 * retourne un tableau contenant s et c_out
 */
int* fullAdder(int a, int b, int c_in) {
	int* res = (int*)malloc(2 * sizeof(int));
	// � compl�ter
	res[0] = (a + b + c_in) % 2;			// res
	res[1] = ((a + b + c_in) - res[0]) / 2;	// carry
	return res;
}

/*
 * Additionne le param�tre au contenu de l'accumulateur (addition enti�re C�2).
 * Les indicateurs sont positionn�s conform�ment au r�sultat de l'op�ration.
 */
void add(ALU alu, int* B) {
	// � compl�ter
	int c = 0; // carry
	int i = 0; // current bit
	int lastCarry = 0; // last carry
	while (i < NBITS) {
		int* res = fullAdder(get(alu.accu, i), get(B, i), c); //call fulladder
		if (get(alu.accu, i) != 0 && get(B, i) != 0) { //if its not gonna consume the carry then save the carry
			lastCarry = c;
		}
		c = res[1];
		set(alu.accu, i, res[0]);
		i++;
		free(res);
	}
	setZ(alu); // auto detect
	setC(alu, lastCarry);
	if (lastCarry && get(alu.accu, NBITS - 1)) { // if there is a carry and last bit is 1 then there is an overflow!
		setO(alu, 1);
	}else {
		setO(alu, 0);
	}
	setN(alu);
}

////////////////////////////////////////////////////////////////////
// Op�rations logiques :
////////////////////////////////////////////////////////////////////

/*
 * N�gation.
 */
void not(CPU cpu) {
	// � compl�ter
	nand(cpu.alu, cpu.alu.accu); // not(accu)=nand(accu, accu)
}

/*
 * Et.
 */
void and(CPU cpu, int* B) {
	// � compl�ter
	nand(cpu.alu, B);	//alu.accu = nand(alu.accu, B);
	not(cpu);			//and(alu.accu, B)=not(nand(alu.accu, B));
}

/*
 * Ou.
 */
void or (CPU cpu, int* B) {
	// � compl�ter
	copyValue(cpu.R0, cpu.alu.accu);	// take copy of those
	copyValue(cpu.R1, B);
	nand(cpu.alu, cpu.R0);				// nand(alu.accu, alu.accu)
	copyValue(cpu.R0, cpu.alu.accu);	// save the result
	pass(cpu.alu, cpu.R1);
	nand(cpu.alu, cpu.R1);				// calculer nand(B,B)
	copyValue(cpu.R1, cpu.alu.accu);
	pass(cpu.alu, cpu.R0);
	nand(cpu.alu, cpu.R1);				// nand(nand(alu.acuu, alu.accu), (B, B))
}

/*
 * Xor.
 */
void xor(CPU cpu, int* B) { // or( and( not(a), b), and( a, not(b) ) )  (Uses R0, R1,R2)
	// � compl�ter
	copyValue(cpu.R0, cpu.alu.accu);		// R0 <- accu
	copyValue(cpu.R2, B);					// R2 <- B
	not(cpu);								// not(cpu.alu.accu)
	and (cpu, cpu.R2);						// and(not(accu), B)
	copyValue(cpu.R1, cpu.alu.accu);		// R1 <- and(not(accu), B)
	pass(cpu.alu, cpu.R2);					// accu <- B
	not(cpu);								// not(B) 
	copyValue(cpu.R2, cpu.alu.accu);		// R2 <- not(B)
	pass(cpu.alu, cpu.R0);					// accu <- R0
	and (cpu, cpu.R2);						// and(R2, not(B))
	copyValue(cpu.R2, cpu.alu.accu);		// R2 <- and(R2, not(B))
	pass(cpu.alu, cpu.R1);					// accu = and(not(accu), B)
	or (cpu, cpu.R2);						// or( and( not(accu), B), and(accu, not(B)) )
}

/*
 * D�cale le receveur de |n| positions.
 * Le d�calage s'effectue vers la gauche si n>0 vers la droite dans le cas contraire.
 * C'est un d�calage logique (pas de report du bit de signe dans les positions
 * lib�r�es en cas de d�calage � droite).
 * L'indicateur CF est positionn� avec le dernier bit "perdu".
 */
void logicalShift(CPU cpu, int n) {
	// � compl�ter
	int i = n;
	while (i != 0) {
		if (n > 0) {
			shift(cpu.alu);
			i--;
		}else if (n < 0) {
			add(cpu.alu, cpu.alu.accu);
			i++;
		}
	}
}

/////////////////////////////////////////////////////////
// Op�rations arithm�tiques enti�res
/////////////////////////////////////////////////////////

/*
 * Oppos�.
 */
void opp(CPU cpu) { // Uses R2
	// � compl�ter
	not(cpu);
	setValue(cpu.R2, 1);
	add(cpu.alu, cpu.R2);
	setZ(cpu.alu);
	setC(cpu.alu, 0);
	setO(cpu.alu, 0);
	setN(cpu.alu);
}

/*
 * Soustraction.
 */
void sub(CPU cpu, int* B) { // Uses R0
	// � compl�ter
	copyValue(cpu.R0, cpu.alu.accu);	// R0 <- acuu
	pass(cpu.alu, B);					// accu <- B
	opp(cpu);							// opp(cpu)
	add(cpu.alu, cpu.R0);				// opp(B) + cpu.alu.acuu
	setZ(cpu.alu);
	setC(cpu.alu, 0);
	setO(cpu.alu, 0);
	setN(cpu.alu);
}

/*
 * Multiplication.
 */
void mul(CPU cpu, int* B) {
	// � compl�ter
	setValue(cpu.R0, 0);
	copyValue(cpu.R1, cpu.alu.accu);
	int i;
	for (i = 0; i < NBITS - 1; i++) {
		if (get(B, i)) {
			pass(cpu.alu, cpu.R0);
			add(cpu.alu, cpu.R1);
			copyValue(cpu.R0, cpu.alu.accu);
		}
		pass(cpu.alu, cpu.R1);
		logicalShift(cpu, -1);
		copyValue(cpu.R1, cpu.alu.accu);
	}
	pass(cpu.alu, cpu.R0);
	setZ(cpu.alu);
	setN(cpu.alu);
}
/////////////////////////////////////////////////////////
// Programme de test
/////////////////////////////////////////////////////////

int main(int argc, char *argv[]) {

	/*
	  Ce programme est fourni � titre d'exemple pour permettre de tester simplement vos fonctions.
	  Il vous est bien entendu possible de le modifier/compl�ter, ou encore d'�crire vos propres fonctions de test.
	*/

	int* operand;
	ALU alu;
	CPU cpu;

	int chosenInt, integer;
	int go_on = 1;

	char* menu =
		"              Programme de test\n\n0  Quitter\n1  setValue(operande,int)\n2  pass(alu,operande)\n3  printing(alu)\n4  afficher toString(operande)\n5  afficher intValue(operande)\n6  afficher intValue(accu)\n7  accu=nand(accu,operande)\n8  accu=add(accu,operande)\n9  accu=sub(accu,operande)\n10  accu=and(accu,operande)\n11 accu=or(accu,operande)\n12 accu=xor(accu,operande)\n13 accu=not(accu)\n14 accu=opp(accu)\n15 accu=shift(accu)\n16 accu=logicalShift(accu,int)\n17 accu=mul(accu,operande)\n\n";

	char* invite = "--> Quel est votre choix  ? ";

	printf("%s", menu);

	operand = word();
	cpu = initCPU();
	alu = cpu.alu;

	while (go_on == 1) {
		printf("%s", invite);
		scanf("%d", &chosenInt);
		switch (chosenInt) {
		case 0:
			go_on = 0;
			break;
		case 1:
			printf("Entrez un nombre :");
			scanf("%d", &integer);
			setValue(operand, integer);
			break;
		case 2:
			pass(alu, operand);
			break;
		case 3:
			printing(alu);
			break;
		case 4:
			printf("%s\n", toString(operand));
			break;
		case 5:
			printf("intValue(operand)=%d\n", intValue(operand));
			break;
		case 6:
			printf("intValue(accu)=%d\n", intValue(alu.accu));
			break;
		case 7:
			nand(alu, operand);
			printf("apres nand(), accu = ");
			printing(alu);
			break;
		case 8:
			add(alu, operand);
			printf("apres add(), accu = ");
			printing(alu);
			break;
		case 9:
			sub(cpu, operand);
			printf("apres sub(), A = ");
			printing(alu);
			break;
		case 10:
			and (cpu, operand);
			printf("apres and(), A = ");
			printing(alu);
			break;
		case 11:
			or (cpu, operand);
			printf("apres or(), A = ");
			printing(alu);
			break;
		case 12:
			xor (cpu, operand);
			printf("apres xor(), A = ");
			printing(alu);
			break;
		case 13:
			not(cpu);
			printf("apres not(), A = ");
			printing(alu);
			break;
		case 14:
			opp(cpu);
			printf("apres opp(), A = ");
			printing(alu);
			break;
		case 15:
			shift(alu);
			printf("apres shift(), A = ");
			printing(alu);
			break;
		case 16:
			printf("Entrez un entier :");
			scanf("%d", &integer);
			logicalShift(cpu, integer);
			printf("apres logicalshift(%d), A = ", integer);
			printing(alu);
			break;
		case 17:
			mul(cpu, operand);
			printf("apres mul(), A = ");
			printing(alu);
			break;
		default:
			printf("Choix inexistant !!!!\n");
			printf("%s", menu);
		}
	}
	printf("Au revoir et a bientot\n");
	return 0;
}



