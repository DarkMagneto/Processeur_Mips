#include "deasm.h"
#include <string.h> 
#include <stdio.h>


// Macros de test des fonctions 

// Macros de test des fonctions 
#define testEQ(ok,res) printf("Attendu: %d,Trouvé:%d",res,ok); if (ok==res){ printf("->Correct\n");resOK++;} else {printf("->Erreur\n");resErreur++;}


#define testSTREQ(ok,res) printf("Attendu:\"%s\",Trouvé:\"%s\"",res,ok);if (strcmp(ok,res)==0){ printf("->Correct\n");resOK++;} else {printf("->Erreur\n");resErreur++;}


#define testEQ2(var1,var11,var2,var22) printf("Attendu:%d,%d,Trouvé:%d,%d",var1,var2,var11,var22);\
    if ((var1==var11)&&(var2==var22)) { printf("->Correct\n");resOK++;}  else {printf("->Erreur\n");resErreur++;}

#define testSTREQ2(var1,var11,var2,var22) printf("Attendu:%u,%u,Trouvé:%d,%d",var1,var2,var11,var22);\
    if ((strcmp(var1,var11)==0)&&(var2==var22)) { printf("->Correct\n");resOK++;}  else {printf("->Erreur\n");resErreur++;}


unsigned pgm[]={ 0x24040006,0x0c100006,0x00022021,0x24020001,0x0000000c,0x1000ffff,0x24020001,0xafa40000,\
0x23bdfffc,0x00820018,0x00001012,0x2084ffff,0x1080fffa,0x23bdfffc,0x8fa40000,0x03e00008};

int test(){
	unsigned resOK=0,resErreur=0;
	char forTest[1000];
	unsigned res,val,Co;
	int Nf;
	printf("%s","-------Strcpy---------\n");
	strCpy("hello ",forTest);
	testSTREQ(forTest,"hello ");
	printf("%s","--------StrAdd---------\n");	
	strAdd("world ",forTest);
	testSTREQ(forTest,"hello world ");
	printf("%s","-------getRegName-------\n");		
	res= getRegName(0,forTest);
	testSTREQ(forTest,"$0");
	res= getRegName(31,forTest);
	testSTREQ(forTest,"$ra");
	printf("%s","---------getRS------------\n");
	res=getRs(0x011f2020,&val,forTest);
	testSTREQ(forTest,"$t0");
	testEQ(val,8);
	printf("%s","---------getRT ?-----------\n");
	res=getRt(0x011f2020,&val,forTest);
	testSTREQ(forTest,"$ra");
	testEQ(val,31);
	printf("%s","---------getRD ?-----------\n");
	res=getRd(0x011f2020,&val,forTest);
	testSTREQ(forTest,"$a0");
	testEQ(val,4);
	printf("%s","---------getImm? ?-----------\n");
	printf("%s","---------getImmNS16 ?-----------\n");
	getImmNs16(0x3128FFFE,&val,forTest);
	testSTREQ(forTest,"0xFFFE");
	testEQ(val,0xFFFE);
	printf("%s","---------getImmS16 ?-----------\n");
	getImmS16(0x3421FFFE,&Nf,forTest);
	testSTREQ(forTest,"-2");
	testEQ(Nf,-2);
	printf("%s","---------getImmS16HEX ?-----------\n");
	getImmS16Hexa(0x3421FFFE,&Nf,forTest);
	testSTREQ(forTest,"0xFFFFFFFE");
	testEQ(Nf,-2);
	printf("%s","---------getImmS26 ?-----------\n");
	getImmNs26(0xFC100003,&val,forTest);
	testSTREQ(forTest,"0x40000C");
	testEQ(val,0x40000C);
	printf("%s","---------getImmSHAMT ?-----------\n");
	getShamt(0x00094600,&val,forTest);
	testSTREQ(forTest,"24");	
	printf("%s","------getInstructionName-----\n");
	res=getInstructionName(0x0000000c,&Co,&Nf,forTest);
	testSTREQ(forTest,"syscall");
	testEQ(Co,0);
	testEQ(Nf,0xC);
	res=getInstructionName(0x23bdfffc,&Co,&Nf,forTest);
	testSTREQ(forTest,"addi ");
	testEQ(Co,8);
	testEQ(Nf,-1);
	printf("%s","------decodeInstruction-----\n");
	res=decodeInstruction(0x0c10000d,forTest);testSTREQ(forTest,"jal 0x400034");
		printf("%s","\n");
	res=decodeInstruction(0x1001000b,forTest);testSTREQ(forTest,"beq $0,$at,11");
		printf("%s","\n");
	res=decodeInstruction(0x1443000a,forTest);testSTREQ(forTest,"bne $v0,$v1,10");
		printf("%s","\n");
	res=decodeInstruction(0x18800009,forTest);testSTREQ(forTest,"blez $a0,9");
		printf("%s","\n");
	res=decodeInstruction(0x1ca00008,forTest);testSTREQ(forTest,"bgtz $a1,8");
		printf("%s","\n");

	res=decodeInstruction(0x20c5fffe,forTest);testSTREQ(forTest,"addi $a1,$a2,-2");
		printf("%s","\n");
	res=decodeInstruction(0x2507fffe,forTest);testSTREQ(forTest,"addiu $a3,$t0,0xFFFFFFFE");
	res=decodeInstruction(0x2949fffd,forTest);testSTREQ(forTest,"slti $t1,$t2,-3");
		printf("%s","\n");
	res=decodeInstruction(0x2d8bfffd,forTest);testSTREQ(forTest,"sltiu $t3,$t4,0xFFFFFFFD");
	printf("%s","\n");
	res=decodeInstruction(0x31cdfffe,forTest);testSTREQ(forTest,"andi $t5,$t6,0xFFFE");
	res=decodeInstruction(0x37388000,forTest);testSTREQ(forTest,"ori $t8,$t9,0x8000");
	res=decodeInstruction(0x3a308001,forTest);testSTREQ(forTest,"xori $s0,$s1,0x8001");
	res=decodeInstruction(0x3c131001,forTest);testSTREQ(forTest,"lui $s3,0x1001");
	printf("%s","\n");

	res=decodeInstruction(0x8128fffc,forTest);testSTREQ(forTest,"lb $t0,-4($t1)");
	res=decodeInstruction(0x8d6a000a,forTest);testSTREQ(forTest,"lw $t2,10($t3)");
	res=decodeInstruction(0x91ac0000,forTest);testSTREQ(forTest,"lbu $t4,0($t5)");
	res=decodeInstruction(0xa1ee0000,forTest);testSTREQ(forTest,"sb $t6,0($t7)");
	res=decodeInstruction(0xaf0f0000,forTest);testSTREQ(forTest,"sw $t7,0($t8)");	
	printf("%s","\n");
	res=decodeInstruction(0x0015a700,forTest);testSTREQ(forTest,"sll $s4,$s5,28");
	res=decodeInstruction(0x0015a700,forTest);testSTREQ(forTest,"sll $s4,$s5,28");
	res=decodeInstruction(0x001ab842,forTest);testSTREQ(forTest,"srl $s7,$k0,1");
	res=decodeInstruction(0x001de7c3,forTest);testSTREQ(forTest,"sra $gp,$sp,31");
	printf("%s","\n");
	res=decodeInstruction(0x03e00008,forTest);testSTREQ(forTest,"jr $ra");
	res=decodeInstruction(0x0000000c,forTest);testSTREQ(forTest,"syscall");
	res=decodeInstruction(0x00000810,forTest);testSTREQ(forTest,"mfhi $at");
	res=decodeInstruction(0x00001012,forTest);testSTREQ(forTest,"mflo $v0");
	printf("%s","\n");
	res=decodeInstruction(0x00640018,forTest);testSTREQ(forTest,"mult $v1,$a0");
	res=decodeInstruction(0x00a60019,forTest);testSTREQ(forTest,"multu $a1,$a2");
	res=decodeInstruction(0x00e8001a,forTest);testSTREQ(forTest,"div $a3,$t0");
	res=decodeInstruction(0x012a001b,forTest);testSTREQ(forTest,"divu $t1,$t2");
	printf("%s","\n");

	res=decodeInstruction(0x018d5820,forTest);testSTREQ(forTest,"add $t3,$t4,$t5");
		printf("%s","\n");
	res=decodeInstruction(0x018d5821,forTest);testSTREQ(forTest,"addu $t3,$t4,$t5");
		printf("%s","\n");
	res=decodeInstruction(0x01f87022,forTest);testSTREQ(forTest,"sub $t6,$t7,$t8");
		printf("%s","\n");
	res=decodeInstruction(0x0211c823,forTest);testSTREQ(forTest,"subu $t9,$s0,$s1");
		printf("%s","\n");
	res=decodeInstruction(0x02749024,forTest);testSTREQ(forTest,"and $s2,$s3,$s4");
		printf("%s","\n");
	res=decodeInstruction(0x02d7a825,forTest);testSTREQ(forTest,"or $s5,$s6,$s7");
		printf("%s","\n");
	res=decodeInstruction(0x037cd026,forTest);testSTREQ(forTest,"xor $k0,$k1,$gp");
		printf("%s","\n");
	res=decodeInstruction(0x03dfe827,forTest);testSTREQ(forTest,"nor $sp,$fp,$ra");
		printf("%s","\n");
	res=decodeInstruction(0x0043082a,forTest);testSTREQ(forTest,"slt $at,$v0,$v1");
		printf("%s","\n");
	res=decodeInstruction(0x00a6202b,forTest);testSTREQ(forTest,"sltu $a0,$a1,$a2");
	testEQ(res,0);	
	res=decodeInstruction(0x7c000000,forTest);testSTREQ(forTest,"#Instruction inconnue");
	testEQ(res,1);
	res=decodeInstruction(0x1,forTest);testSTREQ(forTest,"#Instruction inconnue");	
	testEQ(res,1);

	printf("%s","------decodePgm-----\n");
	decodePgm(pgm,16,forTest);
	testSTREQ(forTest,"addiu $a0,$0,0x6\njal 0x400018\naddu $a0,$0,$v0\naddiu $v0,$0,0x1\nsyscall\nbeq $0,$0,-1\naddiu $v0,$0,0x1\
\nsw $a0,0($sp)\naddi $sp,$sp,-4\nmult $a0,$v0\nmflo $v0\naddi $a0,$a0,-1\nbeq $a0,$0,-6\naddi $sp,$sp,-4\nlw $a0,0($sp)\njr $ra\n");




	printf("Nombre de test(s) faux : %d \n",resErreur);
	printf("Nombre de test(s) correct(s) : %d \n",resOK);	
	return(resErreur);

}

int main(){
	test();
    //decodePgm(pgm,16);
    return 0;
}

