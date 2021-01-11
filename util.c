/*
Armator - simulateur de jeu d'instruction ARMv5T à but pédagogique
Copyright (C) 2011 Guillaume Huard
Ce programme est libre, vous pouvez le redistribuer et/ou le modifier selon les
termes de la Licence Publique Générale GNU publiée par la Free Software
Foundation (version 2 ou bien toute autre version ultérieure choisie par vous).

Ce programme est distribué car potentiellement utile, mais SANS AUCUNE
GARANTIE, ni explicite ni implicite, y compris les garanties de
commercialisation ou d'adaptation dans un but spécifique. Reportez-vous à la
Licence Publique Générale GNU pour plus de détails.

Vous devez avoir reçu une copie de la Licence Publique Générale GNU en même
temps que ce programme ; si ce n'est pas le cas, écrivez à la Free Software
Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307,
États-Unis.

Contact: Guillaume.Huard@imag.fr
	 Bâtiment IMAG
	 700 avenue centrale, domaine universitaire
	 38401 Saint Martin d'Hères
*/
#include "util.h"

uint32_t regCPSR;

/* We implement asr because shifting a signed is non portable in ANSI C */
uint32_t asr(uint32_t value, uint8_t shift) {
    return (value >> shift) | (get_bit(value, 31) ? ~0<<(32-shift) : 0);
}

uint32_t ror(uint32_t value, uint8_t rotation) {
    return (value >> rotation) | (value << (32-rotation));
}

int is_big_endian() {
    static uint32_t one = 1;
    return ((* (uint8_t *) &one) == 0);
}

int check_fs(int flag, int state){
	return (get_bit(regCPSR, flag) == state);
}

int cond_valid(uint8_t cond, uint32_t reg){
	regCPSR = reg;
	switch(cond){
		case 0b0000: return check_fs(Z, 1);
		break;
		case 0b0001: return check_fs(Z, 0);
		break;
		case 0b0010: return check_fs(C, 1);
		break;
		case 0b0011: return check_fs(C, 0);
		break;
		case 0b0100: return check_fs(N, 1);
		break;
		case 0b0101: return check_fs(N, 0);
		break;
		case 0b0110: return check_fs(V, 1);
		break;
		case 0b0111: return check_fs(V, 0);
		break;
		case 0b1000: return (check_fs(C, 1) && check_fs(Z, 0));
		break;
		case 0b1001: return (check_fs(C, 0) || check_fs(Z, 1));
		break;
		case 0b1010: return ((check_fs(N, 1) && check_fs(V, 1)) || (check_fs(N, 0) && check_fs(V, 0)));
		break;
		case 0b1011: return ((check_fs(N, 1) && check_fs(V, 0)) || (check_fs(N, 0) && check_fs(V, 1)));
		break;
		case 0b1100: return (check_fs(Z, 0) && ((check_fs(N, 1) && check_fs(V, 1)) || (check_fs(N, 0) && check_fs(V, 0))));
		break;
		case 0b1101: return (check_fs(Z, 1) || (check_fs(N, 1) && check_fs(V, 0)) || (check_fs(N, 0) && check_fs(V, 1)));
		break;
		case 0b1110: return 1;
		break;
		default: return -1;
	}
	regCPSR = 0;
}

uint32_t jmp_calc(uint32_t jmpTarget, int sign){
	if(sign){
		return ((~0 << 24) | jmpTarget) << 2;
	}
	return jmpTarget << 2;
}

void extract_data_proc_fields(uint32_t ins, reg_fields *rf, uint32_t RnVal, uint32_t Sop){
	rf->cond = get_bits(ins, 31, 28);
	rf->opcode = get_bits(ins, 24, 21);
	rf->S = get_bit(ins, 20);
	rf->Rn = get_bits(ins, 19, 16);
	rf->Rd = get_bits(ins, 15, 12);
	rf->RnVal = RnVal;
	rf->Sop = Sop;
}

uint32_t update_flags(uint32_t reg, uint8_t opcode, uint32_t res, uint8_t sco){
	get_bit(res, 31) ? (reg = set_bit(reg, N)) : (reg = clr_bit(reg, N));
	res == 0 ? (reg = set_bit(reg, Z)) : (reg = clr_bit(reg, Z));
	switch(opcode){
		case 0b0000: //AND
		case 0b0001: //EOR
		case 0b1000: //TST
		case 0b1001: //TEQ
		case 0b1100: //ORR
		case 0b1101: //MOV
		case 0b1110: //BIC
		case 0b1111: //MVN
			mod_bit(&reg, sco, C);
		break;

		case 0b0010: //SUB
		case 0b0011: //RSB
		case 0b1010: //CMP
		break;

		case 0b0100: //ADD
		case 0b1011: //CMN
		break;

		case 0b0110: //SBC
		case 0b0111: //RSC
		break;

		case 0b0101: //ADC
		break;
	}
	return reg;
}