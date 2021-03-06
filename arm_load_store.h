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
#ifndef __ARM_LOAD_STORE_H__
#define __ARM_LOAD_STORE_H__
#include <stdint.h>
#include "arm_core.h"

#define I 25
#define P 24
#define U 23
#define B 22
#define W 21
#define L 20
#define H 5

int laod_stot_byte_word(arm_core p, uint8_t reg, uint32_t add, uint32_t ins);
int laod_stot_halfword(arm_core p, uint8_t reg, uint32_t add, uint32_t ins);
int32_t est_immediate(uint32_t ins, arm_core p);
int32_t half_immediate(uint32_t ins, arm_core p);
int byte_word(arm_core p, uint32_t ins);
int half_word(arm_core p, uint32_t ins);

uint8_t creer_reglist(uint8_t* reg_tab, uint32_t ins);
int check_reg_plag(uint8_t* reg_tab, int n, uint32_t ins);

int arm_load_store(arm_core p, uint32_t ins);
int arm_load_store_multiple(arm_core p, uint32_t ins);
int arm_coprocessor_load_store(arm_core p, uint32_t ins);

#endif
