
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
#include "arm_load_store.h"
#include "arm_exception.h"
#include "arm_constants.h"
#include "util.h"
#include "debug.h"


int laod_stot_byte_word(arm_core p, uint8_t reg, uint32_t add, uint32_t ins){
	int res;
	uint32_t val ;
	
	if( get_bit(ins, L) ){
		if( get_bit(ins, B) ){
			res = arm_read_byte(p, add, (uint8_t *)(&val) );
			if(res==0)
				arm_write_register(p, reg, val);
			
			// eles##############
		}
		else{ 
			res = arm_read_word(p, add, &val);
			if(res==0)
				arm_write_register(p, reg, val);
			
		}
	}
	else{ 
		if(get_bit(ins, B)){

			val = arm_read_register(p, reg);
			res = arm_write_byte( p,  add, val);

			// eles##############
		}
		else{ 
			val = arm_read_register(p, reg);
			res = arm_write_word(p,  add, val);
		}
	}

	return res ;
}

int laod_stot_halfword(arm_core p, uint8_t reg, uint32_t add, uint32_t ins){
	int res;
	uint32_t val ;
	
	// L=0, S=0, H=1 Store halfword
	// L=1, S=0, H=1 Load unsigned halfword
	if( get_bit(ins, L) ){
		if( get_bit(ins, 6)==0 && get_bit(ins, H) ){
			res = arm_read_half(p, add, (uint16_t *)(&val) );
			if(res==0){
				arm_write_register(p, reg, val);
			}
			// eles##############
		}
		else{
			res = UNDEFINED_INSTRUCTION;
		}
	}
	else{ 
		if(get_bit(ins, 6)==0 && get_bit(ins, H) ){
			val = arm_read_register(p, reg);
			res = arm_write_half( p,  add, (uint16_t)val);
			// eles##############
		}
		else
			res = UNDEFINED_INSTRUCTION;
	}

	return res ;
}


int32_t est_immediate(uint32_t ins, arm_core p){
	int32_t I_bit, bits, c, CPSR_r, ind, Rm, shift_code, shift_imm ;
	int8_t n_Rm ;
	
	I_bit = get_bit(ins, I);
	if(I_bit){
		shift_code = get_bits(ins,6,5);
		shift_imm = get_bits(ins,11,7);
		n_Rm = get_bits(ins,3,0);
		Rm = arm_read_register(p, n_Rm );
		switch(shift_code){
			//0b00  LSL Logical_Shift_Left 
			case 0: ind = Rm << shift_imm ;
			break;
			//0b01  LSR Logical_Shift_Left 
			case 1: ind = Rm >> shift_imm ;
			break;
			//0b10  ASR A_Shift_Left 
			case 2: 
				if (shift_imm == 0){
					if(get_bit(ins, 31) == 1 )
						ind = ~0 ;
					else
						ind = 0 ;
				}
				else{
					bits = get_bit(ins, 31) ; // bit 31 de 
					ind = (Rm >> shift_imm) & bits ;
				}
				
				bits = get_bit(ins, 31) ; // bit 31 de 
				ind = (Rm >> shift_imm) & bits ;
			break;
			//0b11  ROR ou RRX  
			case 3: 
				if (shift_imm == 0){
					CPSR_r = arm_read_cpsr(p);
					c = get_bit(CPSR_r, 29) ;
					ind = (c << 31) | (Rm >> 1 ) ;
				}
				else{
					bits = get_bits(ins,shift_imm+31-shift_imm-1 , shift_imm ) ;
					ind = (Rm >> shift_imm) & bits ;
				}
				
				
			break;
			}
			
	
	}
	else{
		ind = get_bits(ins,11,0);
	}
	return ind ;
}

int32_t half_immediate(uint32_t ins, arm_core p){
	uint32_t immedH, immedL, B_bit, ind ;
	
	B_bit = get_bit(ins, B);
	if(B_bit){
		//offset_8 = (immedH << 4) OR immedL
		
		immedH = get_bits(ins,11,8);
		immedL = get_bits(ins,3,0);
		ind = (immedH << 4) | immedL ;
	}
	else{
		ind = arm_read_register(p, get_bits(ins,3,0) );
	}
	return ind ;
}


int half_word(arm_core p, uint32_t ins){
	uint32_t cond, ind, Rn ;
	uint8_t  n_Rn, n_Rd ;
	int res, cond_v ;
	
	cond = get_bits(ins,31,28);
	cond_v = cond_valid( (uint8_t)cond, ins );
	
	n_Rn = get_bits(ins,19,16);
	n_Rd = get_bits(ins,15,12);
	Rn = arm_read_register(p, n_Rn );
	
	ind = half_immediate(ins, p);
	if( get_bit(ins, P) ){
		if(get_bit(ins, U)){
			if(get_bit(ins, W)){
				if(cond_v){
					arm_write_register(p, n_Rn, Rn + ind);
					Rn = arm_read_register(p, n_Rn );
				}
				res = laod_stot_halfword(p, n_Rd, Rn, ins);
				 
			}
			else{
				res = laod_stot_halfword(p, n_Rd, Rn + ind, ins);
			}
				
		}
		else{
			if(get_bit(ins, W)){
				if(cond_v){
					arm_write_register(p, n_Rn, Rn-ind);
					Rn = arm_read_register(p, n_Rn );
				}
				res = laod_stot_halfword(p, n_Rd, Rn, ins);
				 
			}
			else{
				res = laod_stot_halfword(p, n_Rd, Rn - ind, ins);
			}
			
		
		}	
	}
	else{
		if(get_bit(ins, U)){
			if(get_bit(ins, W)==0){
				res = laod_stot_halfword(p, n_Rd, Rn, ins);
				if(cond_v)
					arm_write_register(p, n_Rn, Rn + ind);
			}	
		}
		else{
			if(get_bit(ins, W)==0){
				res = laod_stot_halfword(p, n_Rd, Rn , ins);
			}
		}
	}
	return res ;
}

int byte_word(arm_core p, uint32_t ins){
	uint32_t cond, ind, Rn ;
	uint8_t  n_Rn, n_Rd ;
	int res, cond_v ;
	
	cond = get_bits(ins,31,28);
	cond_v = cond_valid( (uint8_t)cond, ins );
	n_Rn = get_bits(ins,19,16);
	n_Rd = get_bits(ins,15,12);
	Rn = arm_read_register(p, n_Rn );
	
	ind = est_immediate(ins, p);
	if( get_bit(ins, P) ){
		if(get_bit(ins, U)){
			if(get_bit(ins, W)){
				if(cond_v){
					arm_write_register(p, n_Rn, Rn+ind);
					Rn = arm_read_register(p, n_Rn );
				}
				res = laod_stot_byte_word(p, n_Rd, Rn, ins);
				 
			}
			else{
				res = laod_stot_byte_word(p, n_Rd, Rn + ind, ins);
			}
				
		}
		else{
			if(get_bit(ins, W)){
				if(cond_v){
					arm_write_register(p, n_Rn, Rn - ind);
					Rn = arm_read_register(p, n_Rn );
				}
				res = laod_stot_byte_word(p, n_Rd, Rn, ins);
				 
			}
			else{
				res = laod_stot_byte_word(p, n_Rd, Rn - ind, ins);
			}
			
		
		}	
	}
	else{
		if(get_bit(ins, U)){
			if(get_bit(ins, W)==0){
				res = laod_stot_byte_word(p, n_Rd, Rn, ins);
				if(cond_v)
					arm_write_register(p, n_Rn, Rn + ind);
				 
			}	
		}
		else{
			if(get_bit(ins, W)==0){
				res = laod_stot_byte_word(p, n_Rd, Rn , ins);
			}
			
		
		}
	}
	return res ;
}

int arm_load_store(arm_core p, uint32_t ins){
	int res ;
	uint8_t taille ;
	
	taille = get_bits(ins,27,25);
	//bit4 = get_bit(ins, 4);
	//bit7 = get_bit(ins, 7);
	if( taille == 0b010 || taille == 0b011 ){
		//if(bit4 == 0)
			res = byte_word(p, ins) ;
	}
	else if(taille == 0b000  ) {
		res = half_word(p, ins) ;
	}
	return res;
}
//################################ ABRO ##############################################

uint8_t creer_reglist(uint8_t* reg_tab, uint32_t ins){
      uint8_t n = 0;
      for(int i=0; i<16; i++)
      {
        if (get_bit(ins,i))
        {
          reg_tab[n] = i;
          n++;
        }
      }
      return n;
}
  
int check_reg_plag(uint8_t* reg_tab, int n, uint32_t ins){
      int var_PC, var_SP, var_LR, var_L;
      for(int i=0; i<n; i++){
        if (reg_tab[i] == 13){
          var_SP = 1;
        } 
        if (reg_tab[i] == 14){
          var_LR = 1;
        } 
        if (reg_tab[i] == 15){
          var_PC = 1;
        } 
      }
      var_L = get_bit(ins,20);
      if ((var_PC && !var_L) || (var_SP) || (var_PC && var_LR && var_L) || (n >= 2)){
        return 1;
      }
  return 0;
}

int arm_load_store_multiple(arm_core p, uint32_t ins) {
    uint32_t val;
    uint32_t Rn_num, Rn, cond;
    uint8_t nb_reg, reg;
    uint8_t reg_tab[16];
    int cond_v;
    
    cond = get_bits(ins, 31, 28 );
    cond_v = cond_valid( (uint8_t)cond, ins );
    
    Rn_num = get_bits(ins,19,16);
    nb_reg = creer_reglist(reg_tab,ins);
    
    if (Rn_num > 7 || Rn_num < 0){ return UNDEFINED_INSTRUCTION; }
    // if (check_reg_plag( reg_tab, nb_reg, ins)){ return UNDEFINED_INSTRUCTION; }
    
    switch(get_bits(ins,27,23)){
       case (16):  if ( get_bit(ins,20) ){ // LDM (1)
                       Rn = arm_read_register(p, Rn_num);
                       for(int i=0; i<nb_reg; i++)
                       {
                           reg = reg_tab[i];
                           arm_read_word(p, Rn, &val);
                           arm_write_register(p, reg, val);
                           Rn-=4;
                       }
                   }else{                     // STM (1)
                       for(int i=0; i<nb_reg; i++)
                       {
                           reg = reg_tab[i];
                           val = arm_read_register(p, reg);
                           arm_write_word( p, Rn, val);
                           Rn-=4;
                       }
                   } // DA
                   if ( cond_v && get_bit(ins,21)){
                       arm_write_register(p, Rn_num , (Rn + nb_reg*4) );
                   }
                   return 0;
        break; 
       case (17):  if ( get_bit(ins,20) ){ // LDM (1)
                       Rn = arm_read_register(p, Rn_num);
                       for(int i=0; i<nb_reg; i++)
                       {
                           reg = reg_tab[i];
                           arm_read_word(p, Rn, &val);
                           arm_write_register(p, reg, val);
                           Rn+=4;
                       }
                   }else{                     // STM (1)
                       for(int i=0; i<nb_reg; i++)
                       {
                           reg = reg_tab[i];
                           val = arm_read_register(p, reg);
                           arm_write_word( p, Rn , val);
                           Rn+=4;
                        }
                   }  // IA
                   if ( cond_v && get_bit(ins,21)){
                       arm_write_register(p, Rn_num , (Rn + nb_reg*4));
                   }
                   return 0;
        break;
       case (18):  if ( get_bit(ins,20) ){ // LDM (1)
                       Rn = arm_read_register(p, Rn_num);
                       for(int i=0; i<nb_reg; i++)
                       {
                           reg = reg_tab[i];
                           Rn-=4;
                           arm_read_word(p, Rn, &val);
                           arm_write_register(p, reg, val);
                       }
                   }else{                     // STM (1)
                       for(int i=0; i<nb_reg; i++)
                       {
                           reg = reg_tab[i];
                           Rn-=4;
                           val = arm_read_register(p, reg);
                           arm_write_word( p, Rn , val);
                        }
                   }// DB
                   if ( cond_v && get_bit(ins,21)){
                       arm_write_register(p, Rn_num , (Rn + nb_reg*4));
                   }
                   return 0;
        break;
       case (19):  if ( get_bit(ins,20) ){ // LDM (1)
                       Rn = arm_read_register(p, Rn_num);
                       for(int i=0; i<nb_reg; i++)
                       {
                           reg = reg_tab[i];
                           Rn+=4;
                           arm_read_word(p, Rn, &val);
                           arm_write_register(p, reg, val);
                       }
                   }else{                     // STM (1)
                       for(int i=0; i<nb_reg; i++)
                       {
                           reg = reg_tab[i];
                           Rn+=4;
                           val = arm_read_register(p, reg);
                           arm_write_word(p, Rn, val);
                        }
                   }// IB
                   if ( cond_v && get_bit(ins,21)){
                       arm_write_register(p, Rn_num , (Rn + nb_reg*4));
                   }
                   return 0;
        break;
       default:
        return UNDEFINED_INSTRUCTION; 
    }
}




//####################################################################################
    
int arm_coprocessor_load_store(arm_core p, uint32_t ins) {
    int reg;
    if ((get_bits(ins,27,23) == 2) && get_bits(ins,15,12) != 15){
      reg = get_bits(ins,15,12);
      if (get_bit(ins,22)){
        uint32_t spsr_val = arm_read_spsr(p);
        arm_write_register(p, reg, spsr_val);
      }else{
        uint32_t cpsr_val = arm_read_cpsr(p);
        arm_write_register(p, reg, cpsr_val);
      }        
    }
    return UNDEFINED_INSTRUCTION;
}

