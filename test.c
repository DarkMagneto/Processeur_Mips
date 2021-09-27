#include "deasm.h"

unsigned getRegName(unsigned num,char* name){
char reg[32*4]="$0\0 $at\0$v0\0$v1\0$a0\0$a1\0$a2\0$a3\0$t0\0$t1\0$t2\0$t3\0$t4\0$t5\0$t6\0$t7\0"\
                 "$s0\0$s1\0$s2\0$s3\0$s4\0$s5\0$s6\0$s7\0$t8\0$t9\0$k0\0$k1\0$gp\0$sp\0$fp\0$ra";

if (num >=0 && num <= 31){
    strCpy(&reg[num*4],name);
    return 0;
}
return 1;
}

unsigned getRs(unsigned codeInst,unsigned *numRs, char* name){
*numRs = (codeInst << 6 ) >> 27;
return getRegName(*numRs,name);

}
unsigned getRt(unsigned codeInst,unsigned *numRt, char* name){
*numRt = (codeInst << 11 ) >> 27;
return getRegName(*numRt,name);

}

unsigned getRd(unsigned codeInst,unsigned *numRd, char* name){
*numRd = (codeInst << 16 ) >> 27;
return getRegName(*numRd,name);

}
   
// digit -> caractere
char forDigit(unsigned nombre){
    if(nombre > 9) return (char)(nombre+'A'-10);    
    else return (char)(nombre+'0');    
}

void utoha(unsigned nbr,char *chaine){
    unsigned nombre1 = nbr;
    int base = 16;
    //Taille de la chaine
    while(nombre1!=0){
        chaine++;
        nombre1=(unsigned)nombre1/base;
    }
    //Placer le caractere de fin
    chaine++;
    chaine++;
    *chaine=(char)0;
    //Placer les caracteres depuis la fin
    while(nbr!=0){
        chaine--;
        *chaine=(char)forDigit(nbr%base);
        nbr=(unsigned)nbr/base;
    }
    chaine--;
    *chaine='x';
    chaine--;
    *chaine='0';
}
//
void itoa(int val,char *chaine){
int nombreCpy = val;
int base = 10;
    if(val < 0){
    	*chaine = '-';
    	chaine++;

    	do{
	        nombreCpy /= base;
	        chaine++;
	    }
	    while (nombreCpy != 0);
	    
	    *chaine = (char)0;
	    
	    do{
	        chaine--;
	        *chaine = forDigit(-val%base & 0xF);
	        val /= base;
	    }while(val != 0);
    }else{
    	do{
	        nombreCpy /= base;
	        chaine++;
	    }
	    while (nombreCpy != 0);
	    
	    *chaine = (char)0;
	    
	    do{
	        chaine--;
	        *chaine = forDigit(val%base);
	        val /= base;
	    } while(val != 0);
    }
    

}
//pour le operation qui suive tous dabord dans chaque une delle en isole sque en veux avec le décalage apré en passe dans utoha ou itoa selon sque en veux obtenir signer ou pas signe
//dans le cas des nobre signe en verifie le bit de point fort pour savoir si c un nombre possitif ou negatif
//si il est positif en envoi directement la valeur trouver suite au décalage 
//sinan en subsitu -0xFFFF-1//ki va dans ce cas nous transformer notre valeur positif recuperer dans codeInst en valeur négatif quelle est .

void getImmS16(unsigned codeInst,int *imms,char* chaine){
if(((codeInst>>15)&1)==1){
*imms=((codeInst<<16) >>16)- 0xFFFF - 1;
}else{
*imms=((codeInst<<16) >>16);
}
itoa(*imms,chaine);
}

void getImmS16Hexa(unsigned codeInst,int *imms,char* chaine){
if(((codeInst>>15)&1)==1){
    *imms=((codeInst<<16) >>16)- 0xFFFF - 1;
}else{
    *imms=((codeInst<<16) >>16);
}
utoha(*imms,chaine);
}

void getImmNs16(unsigned codeInst,unsigned *immNs,char* chaine){
*immNs=((codeInst<<16) >>16);
utoha(*immNs,chaine);
}

void getImmNs26(unsigned codeInst,unsigned *immNs,char* chaine){
*immNs =((codeInst <<6)>>6)<<2;
utoha(*immNs,chaine);
}

void getShamt(unsigned codeInst,unsigned *shamt,char *chaine){
*shamt = (codeInst <<21)>>27;
itoa(*shamt,chaine);
}

void strCpy(char *src,char *dest){
    char i;
    for(i = 0; src[i] != '\0'; ++i){
        dest[i] = src[i];
    }
    dest[i] = '\0';
} 


//pour recuperer l'instruction du codeInst en recupere Co et Nf avec des simple manipulation de decalage pour les isoler 
//une fois recuperer si Co est differant de 0 alor en charge du tableau nomImm a la ligne co et colone 8 le nom de linstruction et Nf pron -1.
//sinan en charge du tableau nom3reg a la ligne nf et colon 8 l'instruction qui correspond
unsigned getInstructionName(unsigned codeInst,unsigned *Co,int *Nf,char *name){
char nomImm[64*8]=  "X\0      X\0      X\0      jal \0   beq \0   bne \0   blez \0  bgtz \0  "\
                    "addi \0  addiu \0 slti \0  sltiu \0 andi \0  ori \0   xori \0  lui \0   "\
                    "X\0      X\0      X\0      X\0      X\0      X\0      X\0      X\0      "\
                    "X\0      X\0      X\0      X\0      X\0      X\0      X\0      X\0      "\
                    "lb \0    X\0      X\0      lw \0    lbu \0   X\0      X\0      X\0      "\
                    "sb \0    X\0      X\0      sw \0    X\0      X\0      X\0      X\0      "\
                    "X\0      X\0      X\0      X\0      X\0      X\0      X\0      X\0      "\
                    "X\0      X\0      X\0      X\0      X\0      X\0      X\0      X\0      ";  

char nom3reg[64*8]= "sll \0   X\0      srl \0   sra \0   sllv \0  X\0      srlv \0  srav \0  "\
                    "jr \0    X\0      X\0      X\0      syscall\0X\0      X\0      X\0      "\
                    "mfhi \0  X\0      mflo \0  X\0      X\0      X\0      X\0      X\0      "\
                    "mult \0  multu \0 div \0   divu \0  X\0      X\0      X\0      X\0      "\
                    "add \0   addu \0  sub \0   subu \0  and \0   or \0    xor \0   nor \0   "\
                    "X\0      X\0      slt \0   sltu \0  X\0      X\0      X\0      X\0      "\
                    "X\0      X\0      X\0      X\0      X\0      X\0      X\0      X\0      "\
                    "X\0      X\0      X\0      X\0      X\0      X\0      X\0      X\0      ";
    *Co = codeInst >> 26;
    if(*Co != 0){
        strCpy(&nomImm[*Co*8],name);
        *Nf = -1;
        return 0;
    }else{
        *Nf =(codeInst<<26)>>26;
        strCpy(&nom3reg[*Nf*8],name);
        return 0;
    }
return 1;
}


//petite fonction pour recuperais la taille d'une chaine de caractaire simple parcour de la chaine avec un compteur 
int string_length(char* address){
    int len = 0;
    while (*(address + len) != '\0'){
        len++;
    }
    return len;
}

void strAdd(char *src,char *dest){
while (*dest != '\0'){
    dest++;
}
    while (*src != '\0'){
        *dest = *src;
        dest++;
        src++;
    }
*dest = '\0';
}

unsigned decodeInstruction(unsigned codeInst,char *inst){
char chaine[100];
unsigned nbr; 
int nbr2;
//en teste directement le nf si il est a -1 alor en utiliser le Co pour recuperer linstruction et les registre qui vont avec 
//sinan si nf et possitif en recupere l'instruction du desieme tabeau et en la revoi avec les registre correspondant

getInstructionName(codeInst,&nbr,&nbr2,inst);
if(nbr2 == -1){
    if(nbr == 0x3){
    getImmNs26(codeInst,&nbr,chaine);
    strAdd(chaine,inst);
    }else if(nbr == 0x20 || nbr == 0x23 || nbr == 0x24 || nbr == 0x28 || nbr == 0x2B){
    getRt(codeInst, &nbr,chaine);
    strAdd(chaine,inst);
    strAdd(",",inst);
    getImmS16(codeInst, &nbr2,chaine);
    strAdd(chaine,inst);
    strAdd("(",inst);
    getRs(codeInst, &nbr,chaine);
    strAdd(chaine,inst);
    strAdd(")",inst);
    }else if(nbr == 0xF ){
    getRt(codeInst, &nbr,chaine);
    strAdd(chaine,inst);
    strAdd(",",inst);
    getImmS16Hexa(codeInst,&nbr2,chaine);
    strAdd(chaine,inst);
    }else if(nbr == 0xC || nbr == 0xD || nbr == 0xE ){
    getRt(codeInst, &nbr,chaine);
    strAdd(chaine,inst);
    strAdd(",",inst);
    getRs(codeInst, &nbr,chaine);
    strAdd(chaine,inst);
    strAdd(",",inst);
    getImmNs16(codeInst,&nbr,chaine);
    strAdd(chaine,inst);
    }else if(nbr == 0x4 || nbr == 0x5  ){
    getRs(codeInst, &nbr,chaine);
    strAdd(chaine,inst);
    strAdd(",",inst);
    getRt(codeInst, &nbr,chaine);
    strAdd(chaine,inst);
    strAdd(",",inst);
    getImmS16(codeInst,&nbr2,chaine);
    strAdd(chaine,inst);
    }else if(nbr == 0x6 || nbr == 0x7){
    getRs(codeInst, &nbr,chaine);
    strAdd(chaine,inst);
    strAdd(",",inst);
    getImmS16(codeInst,&nbr2,chaine);
    strAdd(chaine,inst);
    }else if(nbr == 0x8 || nbr == 0xA ){
    getRt(codeInst, &nbr,chaine);
    strAdd(chaine,inst);
    strAdd(",",inst);
    getRs(codeInst, &nbr,chaine);
    strAdd(chaine,inst);
    strAdd(",",inst);
    getImmS16(codeInst,&nbr2,chaine);
    strAdd(chaine,inst);
    }else if(nbr == 0x9 || nbr == 0xB ){
    getRt(codeInst, &nbr,chaine);
    strAdd(chaine,inst);
    strAdd(",",inst);
    getRs(codeInst, &nbr,chaine);
    strAdd(chaine,inst);
    strAdd(",",inst);
    getImmS16Hexa(codeInst,&nbr2,chaine);
    strAdd(chaine,inst);
    }else{
    strCpy("#Instruction inconnue",inst);
    return 1;
    }
}else{
    if(nbr2 == 0x18 || nbr2 == 0x19 || nbr2 == 0x1A || nbr2 == 0x1B){
    getRs(codeInst, &nbr,chaine);
    strAdd(chaine,inst);
    strAdd(",",inst);
    getRt(codeInst, &nbr,chaine);
    strAdd(chaine,inst);
    }else if(nbr2 == 0x0 || nbr2 == 0x2 || nbr2 == 0x3){
    getRd(codeInst, &nbr,chaine);
    strAdd(chaine,inst);
    strAdd(",",inst);
    getRt(codeInst, &nbr,chaine);
    strAdd(chaine,inst);
    strAdd(",",inst);
    getShamt(codeInst,&nbr,chaine);
    strAdd(chaine,inst);
    }else if(nbr2 == 0x26 || nbr2 == 0x27 || nbr2 == 0x2A || nbr2 == 0x2B || nbr2 == 0x20 || nbr2 == 0x21 || nbr2==0x22 || nbr2==0x23|| nbr2==0x24|| nbr2==0x25 ){
    getRd(codeInst,&nbr,chaine);
    strAdd(chaine,inst);
    strAdd(",",inst);
    getRs(codeInst, &nbr,chaine);
    strAdd(chaine,inst);
    strAdd(",",inst);   
    getRt(codeInst, &nbr,chaine);
    strAdd(chaine,inst);
    }else if(nbr2 == 0x10 || nbr2 == 0x12){
    getRd(codeInst, &nbr,chaine);
    strAdd(chaine,inst);
    }else if(nbr2 == 0xC ){
    }else if(nbr2 == 0x8){
    getRs(codeInst,&nbr,chaine);
    strAdd(chaine,inst);
    }else{
    strCpy("#Instruction inconnue",inst);
    return 1;
    }
}
return 0;
}


void decodePgm(unsigned *pgm,unsigned taille,char *decode){
char chaine[100];
//normalment tous passe dans la boucle mais vue que le decode que en recois et pas vide en l'initialise avec la premier instruction
decodeInstruction(pgm[0],chaine);
strCpy(chaine,decode);
strAdd("\n",decode);
//apre en charge le reste des instruction
for(int i = 1;i<taille;i++){
    decodeInstruction(pgm[i],chaine);
    strAdd(chaine,decode);
    strAdd("\n",decode);
}
}

void putS(char *s){
char c;
    c = *s;
    while(c!=(char)0){
        putchar (c) ;
        s++;
        c = *s;
    }

}

int main(){
    unsigned pgm[]={ 0x24040006,0x0c100006,0x00022021,0x24020001,0x0000000c,0x1000ffff,0x24020001,0xafa40000,\
0x23bdfffc,0x00820018,0x00001012,0x2084ffff,0x1080fffa,0x23bdfffc,0x8fa40000,0x03e00008};

char decode[100];

decodePgm(pgm,16,decode);

putS(decode);

return 0;
}
