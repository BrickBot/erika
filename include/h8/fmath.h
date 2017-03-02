/*
 * Project: E.R.I.K.A. - Embedded Real tIme Kernel Architecture
 *
 * http://erika.sssup.it
 *
 * Copyright (C) 2002 Davide Cantini
 *
 * This program is distributed under the GPL license contained into
 * the license.txt file on the E.R.I.K.A. root directory.
 *
 * CVS: $Id: fmath.h,v 1.2 2003/10/20 10:13:18 pj Exp $
 */


/*****************************
 ****  FIXED POINT MATH   ****
 *****************************/

/* Three types of fixed point are available:						*
 * Unsigned Fixed: 16 bit, where N bit of which are for fractional part.		*
 *		   RANGE      : from  0.0  to  (2^(16-N))-1.(((2^N)-1)/(2^N))		*
 *		   GRANULARITY: 1/(2^N)							*
 *		   Example: N=10 => from 0.0 to 63.999023   (Granularity= 0.00097656)  	*			*
 *		   N=UF_NUM_BIT_FRAC is application dependent: see 'fmath_set.s' file.  *
 *											*
 * Signed Fixed:   16 bit, where N bit of which are for fractional part.		*
 *		   RANGE      : from -2^(16-N-1).0 to (2^(16-N-1))-1.(((2^N)-1)/(2^N))	*
 *		   GRANULARITY: 1/(2^N)							* 
 *		   Example: N=10 => from -32.0 to 31.999023				*
 *		   N=SF_NUM_BIT_FRAC is application dependent: see 'fmath_set.s' file. 	*
 *											*
 * Double Fixed:   32 bit, where 16 bit of which are for fractional part.		*
 *		   RANGE: from -32768.0 to 32767.999984					*
 *		   GRANULARITY: 1/(2^16)=0.00001526					*
 *											*
 * NOTES:										*
 * - You can think unsigned fixed and signed fixed just like as natural and integer	*
 *   numbers. So for example you can freely compare them with standard operators.	*
 * - Double fixed are slower than signed and unsigned; in particular division is slow   *
 *   I hope to find out some faster alghoritm,... Any idea about it?		  	*/
 
#include "erikahal.h"
	
#ifdef __UNSIGNED_FIXED_USED__
    /***********************
     *** UNSIGNED  FIXED ***
     ***********************/

    typedef ERIKA_UINT16  ERIKA_UF_TYPE;
    
    /* converts unsigned to fixed */
    ERIKA_UF_TYPE ERIKA_uint2uf(ERIKA_UINT16 u);

    /* converts fixed to unsigned */
    ERIKA_UINT16 ERIKA_uf2uint(ERIKA_UF_TYPE uf);

    /* returns a+b */    
//    __INLINE__ ERIKA_UF_TYPE ERIKA_uf_add(ERIKA_UF_TYPE a, ERIKA_UF_TYPE b)
//    {
//	return a+b;
//    }

    /* returns a-b */    
//    __INLINE__ ERIKA_UF_TYPE ERIKA_uf_sub(ERIKA_UF_TYPE a, ERIKA_UF_TYPE b)
//    {
//	return a-b;
//    }

    /* returns a*b */
    ERIKA_UF_TYPE ERIKA_uf_mul(ERIKA_UF_TYPE a, ERIKA_UF_TYPE b);

    /* returns a/b (quozient) */
    ERIKA_UF_TYPE ERIKA_uf_div(ERIKA_UF_TYPE a, ERIKA_UF_TYPE b);
#endif 


#ifdef __SIGNED_FIXED_USED__
    /***********************
     **** SIGNED  FIXED ****
     ***********************/
     
    typedef ERIKA_INT16   ERIKA_SF_TYPE;
    
#ifdef __SF_TRIG_USED__
    extern ERIKA_INT16 ERIKA_sf_table_cos[];

    /* calculate cos(theta) */    
    ERIKA_SF_TYPE ERIKA_sf_cos (ERIKA_UINT16 theta); 

    /* calculate sin(theta) */
    ERIKA_SF_TYPE ERIKA_sf_sin (ERIKA_UINT16 theta); 
        
#endif
    
    /* converts integer to fixed */
    ERIKA_SF_TYPE ERIKA_int2sf(ERIKA_INT16 i);

    /* converts fixed to integer */
    ERIKA_INT16 ERIKA_sf2int(ERIKA_SF_TYPE f);

    /* returns a+b */
//    __INLINE__ ERIKA_SF_TYPE ERIKA_sf_add(ERIKA_SF_TYPE a, ERIKA_SF_TYPE b)
//    {
//	return a+b;
//    }
    
    /* returns -a */
    ERIKA_SF_TYPE ERIKA_sf_negate(ERIKA_SF_TYPE a);	
    

    /* returns a-b */
//    __INLINE__ ERIKA_SF_TYPE ERIKA_sf_sub(ERIKA_SF_TYPE a, ERIKA_SF_TYPE b)
//    {
//	return a-b;
//    }

    /* returns a*b */
    ERIKA_SF_TYPE ERIKA_sf_mul(ERIKA_SF_TYPE a, ERIKA_SF_TYPE b);

    /* returns a/b (quozient) */
    ERIKA_SF_TYPE ERIKA_sf_div(ERIKA_SF_TYPE a, ERIKA_SF_TYPE b);
#endif

#ifdef __DOUBLE_FIXED_USED__
    /********************************
     **** DOUBLE  FIXED (signed) ****
     ********************************/

    typedef struct DF_STRUCT {        
	ERIKA_INT16 ip;    
	ERIKA_UINT16 fp;        	
    } ERIKA_DF_TYPE;

#ifdef __DF_TRIG_USED__
    extern ERIKA_UINT16 ERIKA_table_cos[];
    
    /* internally used (called by ERIKA_cos and ERIKA_sin) */
    void _df_trigon (ERIKA_DF_TYPE * dfs, ERIKA_DF_TYPE * theta, ERIKA_UINT16 ph); 
    
    /* calculate cos(theta) and store it into dfs */
    __INLINE__ void ERIKA_df_cos(ERIKA_DF_TYPE * dfs, ERIKA_DF_TYPE * theta)
    {
	_df_trigon(dfs, theta, 0);
    }
    /* calculate sin(theta) and store it into dfs */
    __INLINE__ void ERIKA_df_sin(ERIKA_DF_TYPE * dfs, ERIKA_DF_TYPE * theta)
    {
	_df_trigon(dfs, theta, 90);
    }
#endif

    /* converts integer to fixed and store it into res*/
    void ERIKA_int2df(ERIKA_DF_TYPE * res, ERIKA_INT16 i);

    /* converts fixed to integer and return it */
    ERIKA_INT16 ERIKA_df2int(const ERIKA_DF_TYPE * dfa);
    
    /* return -1, 0, 1 if dfa<dfb, dfa==dfb, dfa>dfb respectively */
    ERIKA_INT16 ERIKA_df_cmp(const ERIKA_DF_TYPE * dfa, const ERIKA_DF_TYPE * dfb);
    
    /* negate dfa and store it into res */    
    void ERIKA_df_negate(ERIKA_DF_TYPE * res, const ERIKA_DF_TYPE * dfa);
    
    /* calculate (dfa+dfb) and store it into res */    
    void ERIKA_df_add(ERIKA_DF_TYPE * res, const ERIKA_DF_TYPE * dfa, const ERIKA_DF_TYPE * dfb);
    
    /* calculate (dfa-dfb) and store it into res */        
    void ERIKA_df_sub(ERIKA_DF_TYPE * res, const ERIKA_DF_TYPE * dfa, const ERIKA_DF_TYPE * dfb);
    
    /* calculate (dfa*dfb) and store it into res */        
    void ERIKA_df_mul(ERIKA_DF_TYPE * res, const ERIKA_DF_TYPE * dfa, const ERIKA_DF_TYPE * dfb);
    
    /* calculate (dfa/dfb) and store it into res */        
    void ERIKA_df_div(ERIKA_DF_TYPE * res, const ERIKA_DF_TYPE * dfa, const ERIKA_DF_TYPE * dfb);
#endif
