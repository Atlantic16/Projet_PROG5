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
#include "registers.h"
#include "arm_constants.h"
#include <stdlib.h>

struct registers_data {
	//37 registre entre tout les modes
	uint32_t regNumber[37];
};

registers registers_create() {
    registers r = malloc(sizeof(struct registers_data));
    return r;
}

void registers_destroy(registers r) {
	free(r);
}

uint8_t get_mode(registers r) {
	uint8_t res = r->regNumber[CPSR] & ~(~0 << 5);
    return res;
} 

int current_mode_has_spsr(registers r) {
	uint8_t mode = get_mode(r);
	int res = 0;
	if(mode != USR && mode != SYS)
		res = 1;
    return res;
}

int in_a_privileged_mode(registers r) {
	uint8_t mode = get_mode(r);
	int res = 1;
	if(mode == USR)
		res = 0;
    return res;
}

uint32_t read_register(registers r, uint8_t reg) {
	uint32_t value= 0;
	uint8_t mode = get_mode(r);
	if((reg >= 0 && reg <= 7) || reg == PC || reg == CPSR)
		value = r->regNumber[reg];
	else if(reg >= 8 && reg <= 12){
		if(mode == FIQ)
			switch(reg){
				case 8: value = r->regNumber[R8_FIQ];
				break;
				case 9: value = r->regNumber[R9_FIQ];
				break;
				case 10: value = r->regNumber[R10_FIQ];
				break;
				case 11: value = r->regNumber[R11_FIQ];
				break;
				case 12: value = r->regNumber[R12_FIQ];
				break;
			}
		else
			value = r->regNumber[reg];
	}
	else if(reg == 13){
		switch(mode){
			case USR:
			case SYS:
				value = r->regNumber[reg];
			break;
			case SVC: value = r->regNumber[R13_SVC];
			break;
			case ABT: value = r->regNumber[R13_ABT];
			break;
			case UND: value = r->regNumber[R13_UND];
			break;
			case IRQ: value = r->regNumber[R13_IRQ];
			break;
			case FIQ: value = r->regNumber[R13_FIQ];
			break;
		}
	}
	else if(reg == 14){
		switch(mode){
			case USR:
			case SYS:
				value = r->regNumber[reg];
			break;
			case SVC: value = r->regNumber[R14_SVC];
			break;
			case ABT: value = r->regNumber[R14_ABT];
			break;
			case UND: value = r->regNumber[R14_UND];
			break;
			case IRQ: value = r->regNumber[R14_IRQ];
			break;
			case FIQ: value = r->regNumber[R14_FIQ];
			break;
		}
	}
	else if(reg == 17)
		value = read_spsr(r);

	return value;
}

uint32_t read_usr_register(registers r, uint8_t reg) {
	uint32_t value = 0;
    if(reg >= 0 && reg <= 16)
    	value = r->regNumber[reg];
    return value;
}

uint32_t read_cpsr(registers r) {
    return r->regNumber[CPSR];
}

uint32_t read_spsr(registers r) {
    uint32_t value=0;
    uint8_t mode = get_mode(r);
    switch(mode){
    	case SVC:
    		value = r->regNumber[SPSR_SVC];
    	break;
    	case ABT:
    		value = r->regNumber[SPSR_ABT];
    	break;
    	case UND:
    		value = r->regNumber[SPSR_UND];
    	break;
    	case IRQ:
    		value = r->regNumber[SPSR_IRQ];
    	break;
    	case FIQ:
    		value = r->regNumber[SPSR_FIQ];
    	break;
    }
    return value;
}

void write_register(registers r, uint8_t reg, uint32_t value) {
	uint8_t mode = get_mode(r);
	if((reg >= 0 && reg <= 7) || reg == PC || reg == CPSR)
		r->regNumber[reg] = value;
	else if(reg >= 8 && reg <= 12){
		if(mode == FIQ)
			switch(mode){
				case 8: r->regNumber[R8_FIQ] = value;
				break;
				case 9: r->regNumber[R9_FIQ] = value;
				break;
				case 10: r->regNumber[R10_FIQ] = value;
				break;
				case 11: r->regNumber[R11_FIQ] = value;
				break;
				case 12: r->regNumber[R12_FIQ]= value;
				break;
		}
		else
			r->regNumber[reg] = value;
	}
	else if(reg == 13){
		switch(mode){
			case USR:
			case SYS:
				r->regNumber[reg] = value;
			break;
			case SVC: r->regNumber[R13_SVC] = value;
			break;
			case ABT: r->regNumber[R13_ABT] = value;
			break;
			case UND: r->regNumber[R13_UND] = value;
			break;
			case IRQ: r->regNumber[R13_IRQ] = value;
			break;
			case FIQ: r->regNumber[R13_FIQ] = value;
			break;
		}
	}
	else if(reg == 14){
		switch(mode){
			case USR:
			case SYS:
				r->regNumber[reg] = value;
			break;
			case SVC: r->regNumber[R14_SVC] = value;
			break;
			case ABT: r->regNumber[R14_ABT] = value;
			break;
			case UND: r->regNumber[R14_UND] = value;
			break;
			case IRQ: r->regNumber[R14_IRQ] = value;
			break;
			case FIQ: r->regNumber[R14_FIQ] = value;
			break;
		}
	}
	else if(reg == 17)
		write_spsr(r, value);
}

void write_usr_register(registers r, uint8_t reg, uint32_t value) {
	if(reg >= 0 && reg <= 16)
		r->regNumber[reg] = value;
}

void write_cpsr(registers r, uint32_t value) {
	r->regNumber[CPSR] = value;
}

void write_spsr(registers r, uint32_t value) {
	uint8_t mode = get_mode(r);
	switch(mode){
    	case SVC:
    		r->regNumber[SPSR_SVC] = value;
    	break;
    	case ABT:
    		r->regNumber[SPSR_ABT] = value;
    	break;
    	case UND:
    		r->regNumber[SPSR_UND] = value;
    	break;
    	case IRQ:
    		r->regNumber[SPSR_IRQ] = value;
    	break;
    	case FIQ:
    		r->regNumber[SPSR_FIQ] = value;
    	break;
    }
}