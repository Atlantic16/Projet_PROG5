/*
Armator - simulateur de jeu d'instruction ARMv5T � but p�dagogique
Copyright (C) 2011 Guillaume Huard
Ce programme est libre, vous pouvez le redistribuer et/ou le modifier selon les
termes de la Licence Publique G�n�rale GNU publi�e par la Free Software
Foundation (version 2 ou bien toute autre version ult�rieure choisie par vous).

Ce programme est distribu� car potentiellement utile, mais SANS AUCUNE
GARANTIE, ni explicite ni implicite, y compris les garanties de
commercialisation ou d'adaptation dans un but sp�cifique. Reportez-vous � la
Licence Publique G�n�rale GNU pour plus de d�tails.

Vous devez avoir re�u une copie de la Licence Publique G�n�rale GNU en m�me
temps que ce programme ; si ce n'est pas le cas, �crivez � la Free Software
Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307,
�tats-Unis.

Contact: Guillaume.Huard@imag.fr
	 B�timent IMAG
	 700 avenue centrale, domaine universitaire
	 38401 Saint Martin d'H�res
*/
#include "arm_data_processing.h"
#include "arm_exception.h"
#include "arm_constants.h"
#include "arm_branch_other.h"
#include "util.h"
#include "debug.h"

int execute_ins(arm_core p, uint32_t ins, uint32_t Sop, uint8_t sco){
	reg_fields rf;
	uint32_t res;
	int x = 0 ;

	extract_data_proc_fields(ins, &rf, arm_read_register(p, rf.Rn), Sop);

	if(cond_valid(rf.cond, arm_read_cpsr(p))){
		switch(rf.opcode){
			case 0b1000: x = 1 ; //TST
			case 0b0000: res = rf.RnVal & rf.Sop; //AND 
			break;
			case 0b1001: x = 1 ; //TEQ
			case 0b0001: res = rf.RnVal ^ rf.Sop; //EOR
			break;
			case 0b1010: x = 1 ; //CMP
			case 0b0010: res = rf.RnVal - rf.Sop; //SUB
			break;
			case 0b0011: res = rf.Sop - rf.RnVal; //RSB
			break;
			case 0b1011: x = 1; //CMN
			case 0b0100: res = rf.RnVal + rf.Sop; //ADD
			break;
			case 0b0101: res = rf.RnVal + rf.Sop + get_bit(arm_read_cpsr(p), C); //ADC
			break;
			case 0b0110: res = rf.RnVal - rf.Sop - !get_bit(arm_read_cpsr(p), C); //SBC
			break;
			case 0b0111: res = rf.Sop - rf.RnVal  - !get_bit(arm_read_cpsr(p), C); //RSC
			break;
			case 0b1100: res = rf.RnVal | rf.Sop; //ORR
			break;
			case 0b1101: res = rf.Sop; //MOV
			break ;
			case 0b1110: res  = rf.RnVal & ~ rf.Sop; //BIC
			break;
			case 0b1111: res = ~ rf.Sop;//MVN
			break;

		}
		if (!x ) arm_write_register(p, rf.Rd, res);
		if(!x && rf.S && rf.Rd == 0b1111){
			if(arm_current_mode_has_spsr(p))
				arm_write_spsr(p, arm_read_cpsr(p)); //Copy CPSR in SPSR
			else return UNDEFINED_INSTRUCTION;
		}
		else if(rf.S || x)
			arm_write_cpsr(p, update_flags(arm_read_cpsr(p), rf, res, sco));
	}
	return 0;
}

/* Decoding functions for different classes of instructions */
int arm_data_processing_shift(arm_core p, uint32_t ins) {
	/*uint8_t Rm, shift, bit4;
	Rm = get_bits(ins, 3, 0);
	bit4 = get_bit(ins, 4);
	shift = get_bits(ins, 6, 5);*/

    return execute_ins(p, ins, 0, 0);
}

int arm_data_processing_immediate_msr(arm_core p, uint32_t ins) {
	uint8_t rotImm, imm;
	uint8_t sco; //Shifter carry out
	uint32_t Sop; //Shifter operand

	rotImm = get_bits(ins, 11, 8);
	imm = get_bits(ins, 7, 0);
	Sop = ror(imm, rotImm * 2);

	imm ? (sco = get_bit(Sop, 31)) : (sco = get_bit(arm_read_cpsr(p), C));

	return execute_ins(p, ins, Sop, sco);
}
