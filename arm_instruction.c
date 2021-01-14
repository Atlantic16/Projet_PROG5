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
#include "arm_instruction.h"
#include "arm_exception.h"
#include "arm_data_processing.h"
#include "arm_load_store.h"
#include "arm_branch_other.h"
#include "arm_constants.h"
#include "util.h"

static int arm_execute_instruction(arm_core p) {
    uint32_t ins;
    uint8_t class, bit4, bit7;

    if(arm_fetch(p, &ins) == 1) {
    	printf("Fetching error.\n");
    	return 0;
    }

    class = get_bits(ins, 27, 25);
    bit4 = get_bit(ins, 4);
    bit7 = get_bit(ins, 7);

    switch(class){
    	case 0b000:
    		if(bit4 == 1 && bit7 == 1)
    			//return arm_miscellaneous(p, ins);
    			return arm_load_store(p, ins);
    		else
    			return arm_data_processing_shift(p, ins);
    	break;
    	case 0b001: return arm_data_processing_immediate_msr(p, ins);
    	break;
    	case 0b010:
    	case 0b011: return arm_load_store(p, ins);
    	break;
    	case 0b100: return arm_load_store_multiple(p, ins);
    	break;
    	case 0b101: return arm_branch(p, ins);
    	break;
    	case 0b110: return arm_coprocessor_load_store(p, ins);
    	break;
    	case 0b111: return arm_coprocessor_others_swi(p, ins);
    	break;
    	default:
    		printf("Unknown instruction.\n");
    		return 1;
    }
}

int arm_step(arm_core p) {
    int result;

    result = arm_execute_instruction(p);
    if (result)
        arm_exception(p, result);
    return result;
}
