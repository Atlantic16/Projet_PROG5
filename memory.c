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
#include <stdlib.h>
#include <stdio.h>
#include "memory.h"
#include "util.h"

struct memory_data {
  	int is_big_endian;
	size_t size;
	uint8_t * memo;
};

memory memory_create(size_t size, int is_big_endian) {
    
    memory mem;
    mem = malloc(sizeof(struct memory_data));
    mem->memo = malloc(size);
    if(mem != NULL && mem->memo != NULL ){
      mem->is_big_endian = is_big_endian;
      mem->size = size ;
    }
    return mem;
}

size_t memory_get_size(memory mem) {
    if( mem !=NULL )
    	return mem->size ;
    else
    	return -1 ;
}

void memory_destroy(memory mem) {
	 free(mem->memo);
	 free(mem);
}

int memory_read_byte(memory mem, uint32_t address, uint8_t *value) {

	if( address >= 0 && address < mem->size ){
		*value = mem->memo[address] ;
		return 0; 
	}
    else return -1;
}

int memory_read_half(memory mem, uint32_t address, uint16_t *value) {
	uint16_t tmp1=0, tmp2=0 ;
	if( address >= 0 && address < mem->size ){
		if((mem->is_big_endian) ){
			tmp1 = (uint16_t)*(mem->memo + address) ;
			tmp2 = (uint16_t)(tmp2 << 8) | tmp1 ;
			tmp1 = (uint16_t)*(mem->memo + address+1) ;
			tmp2 = (uint16_t)(tmp2 << 8) | tmp1 ;
			*value = tmp2 ;
		}
		else{
			*value = *((uint16_t *)(mem->memo) + address);
			
		}
		return 0; 
	}
    else return -1;
}

int memory_read_word(memory mem, uint32_t address, uint32_t *value) {
	uint32_t tmp1=0,tmp2=0 ;
	if( address >= 0 && address < mem->size ){
		if((mem->is_big_endian) ){
			for(int i=0;i<4;i++){
				tmp1 = (uint32_t)*(mem->memo + address + i) ;
				tmp2 = (uint32_t)(tmp2 << 8) | tmp1 ;
			}
			*value = tmp2 ;
		}
		else{
			
			
			*value = *((uint32_t *)(mem->memo) + address);
			
		}
		return 0; 
	}
    else return -1;
}

int memory_write_byte(memory mem, uint32_t address, uint8_t value) {

	if( address >= 0 && address < mem->size ){
		mem->memo[address] = value ;
		return 0; 
	}
    else return -1;
}

int memory_write_half(memory mem, uint32_t address, uint16_t value) {
	if( address >= 0 && address < mem->size ){
		if((mem->is_big_endian) ){
			*(mem->memo + address) = (uint8_t)(value >> 8) ; 
			*(mem->memo + address+1) = (uint8_t)value ;
		}
		else{
			*((uint16_t *)(mem->memo) + address) = value ;
			
		}
		return 0; 
	}
    else return -1;
}

int memory_write_word(memory mem, uint32_t address, uint32_t value) {
	if( address >= 0 && address < mem->size ){
		if((mem->is_big_endian) ){
			for(int i=0;i<4;i++){
				*(mem->memo + address + i ) = (uint8_t)(value >> 8*(3-i)) ;
			}
		}
		else{
			*((uint32_t *)(mem->memo) + address) = value ;
			
		}
		return 0; 
	}
    else return -1;
}









