/**
 * Copyright (c) 2006 SAP AG
 * ====================================================================
 * This is an example program that shows how a program can be developed
 * that uses the ApiVCF library to calculate Volume Correction factors
 * and works with the QCI interface.
 * This source coding is provided “AS IS” and designed to work under
 * SunOS.
 * It can be copied and used as an example to build a customer program.
 *
 * SAP shall not be liable for any damages arising out of your
 * non-compliance with or improper execution of said checking
 * requirements.
 * =================================================================== 
*/

#include "apiVCF.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
/*
* Perform a calculation and pass back the results. 
*
* @param calcReq the calculation request describing the result
* @param unit the unit for the base density to print
* @param file the file to which to append the data
* @param if dens the density is reported, otherwise the ctpl with an
*        eol is reported
*/ 

int main(int argv, char **arg) {
  int len = 5000;
  char buffer[len];
  int errorCode = 0;
  ApiCalculationRequest * request;
  FILE *input,*output;
  int byte,index;
  bool firstLine = TRUE;
  int commodityName = API_COMMODITY_NOT_GIVEN;
  double errorValue;

  ApiUnit *i_tub = NULL, *i_pub = NULL, *i_au  = NULL, *i_tuo = NULL,
          *i_puo = NULL, *i_duo = NULL, *i_tua = NULL, *i_pua = NULL,
          *i_vuo = NULL, *i_vua = NULL, *i_vub = NULL, *i_dua = NULL,
          *i_dub = NULL, *cu    = NULL;
  
/* input parameters */
  double alpha60,
         baseTemp,basePres, baseVol, baseDens,
         obTemp,  obPres,   obVol,   obDens,
         altTemp, altPres,  altVol;
  char *procedure;

/* output parameters */
  double r_altVol, 
         r_baseVol, 
         r_altDens,
         r_scaledCompAlt,
         r_baseDens,
         r_ctlObToBase,
         r_cplObToBase,
         r_ctplObToBase,
         r_scaledCompOb,
         r_ctlBaseToAlt,
         r_cplBaseToAlt,
         r_ctplBaseToAlt,
         r_vcf1,
         r_vcf2;
  char *r_vua, *r_vub, *r_dua, *r_dub;
  int  r_cstat; 

  errorValue = -999.9;

/* check number of arguments 
-----------------------------*/
  if( argv < 27){
    printf("ERROR:TO_FEW_PARAMETERS\n");
    return -1;
  }

  if( argv > 27){
    printf("ERROR:TO_MANY_PARAMETERS\n");
    return -1;
  }

/** Create and initialize request 
----------------------------------*/ 
  errorCode = ApiCalculationRequest_initialize();
  if( errorCode != 0){
    printf("ERROR:INITIALIZE\n");
  //buffer[0] = '\0';
  //ApiCalculationRequest_appendBuffer(buffer, len,"Initialization Error: ");
  //ApiCalculation_getErrorDescription(errorCode, buffer, len);
  //printf("%s\n",buffer);
    return -1;
  }
  request = ApiCalculationRequest_init(&errorCode);
  if( errorCode != 0){
    printf("ERROR:INITIALIZATION\n");
  //buffer[0] = '\0';
  //ApiCalculationRequest_appendBuffer(buffer, len,"Initialization Error: ");
  //ApiCalculation_getErrorDescription(errorCode, buffer, len);
  //printf("%s\n",buffer);
  //return -1;
  }


/* check and take over the parameters 
--------------------------------------*/
  procedure     = arg[1];
  baseTemp      = atof(arg[2]);
  alpha60       = atof(arg[3]);
  obTemp        = atof(arg[4]);
  obPres        = atof(arg[5]);
  obDens        = atof(arg[6]);
  altTemp       = atof(arg[7]);
  altPres       = atof(arg[8]);
  obVol         = atof(arg[9]);
  altVol        = atof(arg[10]);
  baseVol       = atof(arg[11]);
  baseDens      = atof(arg[12]);
  commodityName = atoi(arg[13]);

/* base temperature unit */
  i_tub = NULL;
  if( strcmp(arg[14],"TEMPERATURE_F") == 0) {
    i_tub = ApiUnit_Temperature_F();
  }
  if( strcmp(arg[14],"TEMPERATURE_C") == 0) {
    i_tub = ApiUnit_Temperature_C();
  }

/* base pressure unit */
  i_pub = NULL;
  if( strcmp(arg[15],"PRESSURE_PSI") == 0) {
    i_pub = ApiUnit_Pressure_PSI();
    cu    = ApiUnit_ScaledComp_REV_PSI();
  }
  if( strcmp(arg[15],"PRESSURE_KPA") == 0) {
    i_pub = ApiUnit_Pressure_KPA();
    cu    = ApiUnit_ScaledComp_REV_KPA();
    
  }
  if( strcmp(arg[15],"PRESSURE_BAR") == 0) {
    i_pub = ApiUnit_Pressure_BAR();
    cu    = ApiUnit_ScaledComp_REV_BAR();

  }

/* alpha60 unit */
  i_au = NULL;
  if( strcmp(arg[16],"THERMAL_EXP_REV_F") == 0) {
    i_au = ApiUnit_ThermalExp_REV_F();
  }
  if( strcmp(arg[16],"THERMAL_EXP_REV_C") == 0) {
    i_au = ApiUnit_ThermalExp_REV_C();
  }

/* observed temperature unit */
  i_tuo = NULL;
  if( strcmp(arg[17],"TEMPERATURE_F") == 0) {
    i_tuo = ApiUnit_Temperature_F();
  }
  if( strcmp(arg[17],"TEMPERATURE_C") == 0) {
    i_tuo = ApiUnit_Temperature_C();
  }

/* observed pressure unit */
  i_puo = NULL;
  if( strcmp(arg[18],"PRESSURE_PSI") == 0) {
    i_puo = ApiUnit_Pressure_PSI();
    cu    = ApiUnit_ScaledComp_REV_PSI();

  }
  if( strcmp(arg[18],"PRESSURE_KPA") == 0) {
    i_puo = ApiUnit_Pressure_KPA();
    cu    = ApiUnit_ScaledComp_REV_KPA();

  }
  if( strcmp(arg[18],"PRESSURE_BAR") == 0) {
    i_puo = ApiUnit_Pressure_BAR();
    cu    = ApiUnit_ScaledComp_REV_BAR();

  }

/* observed density unit */
  i_duo = NULL;
  if( strcmp(arg[19],"DENSITY_KGM3") == 0) {
    i_duo = ApiUnit_Density_KGM3();
  };
  if( strcmp(arg[19],"DENSITY_API") == 0) {
    i_duo = ApiUnit_Density_API();
  };
  if( strcmp(arg[19],"DENSITY_RELATIVE") == 0) {
    i_duo = ApiUnit_Density_RELATIVE();
  }; 


/* alternate temperature unit */
  i_tua = NULL;
  if( strcmp(arg[20],"TEMPERATURE_F") == 0) {
    i_tua = ApiUnit_Temperature_F();
  }
  if( strcmp(arg[20],"TEMPERATURE_C") == 0) {
    i_tua = ApiUnit_Temperature_C();
  }

/* alternate pressure unit */
  i_pua = NULL;
  if( strcmp(arg[21],"PRESSURE_PSI") == 0) {
    i_pua = ApiUnit_Pressure_PSI();
    cu    = ApiUnit_ScaledComp_REV_PSI();

  }
  if( strcmp(arg[21],"PRESSURE_KPA") == 0) {
    i_pua = ApiUnit_Pressure_KPA();
    cu    = ApiUnit_ScaledComp_REV_KPA();

  }
  if( strcmp(arg[21],"PRESSURE_BAR") == 0) {
    i_pua = ApiUnit_Pressure_BAR();
    cu    = ApiUnit_ScaledComp_REV_BAR();

  }

/* observed volume unit */
  i_vuo = NULL;
  if( strcmp(arg[22],"VOLUME_M3") == 0) {
    i_vuo = ApiUnit_Volume_M3();
  }
  if( strcmp(arg[22],"VOLUME_LITER") == 0) {
    i_vuo = ApiUnit_Volume_LITER();
  }
  if( strcmp(arg[22],"VOLUME_BARREL") == 0) {
    i_vuo = ApiUnit_Volume_BARREL();
  }

/* alternate volume unit */
  i_vua = NULL;
  if( strcmp(arg[23],"VOLUME_M3") == 0) {
    i_vua = ApiUnit_Volume_M3();
  }
  if( strcmp(arg[23],"VOLUME_LITER") == 0) {
    i_vua = ApiUnit_Volume_LITER();
  }
  if( strcmp(arg[23],"VOLUME_BARREL") == 0) {
    i_vua = ApiUnit_Volume_BARREL();
  }

/* base volume unit */
  i_vub = NULL;
  if( strcmp(arg[24],"VOLUME_M3") == 0) {
    i_vub = ApiUnit_Volume_M3();
  }
  if( strcmp(arg[24],"VOLUME_LITER") == 0) {
    i_vub = ApiUnit_Volume_LITER();
  }
  if( strcmp(arg[24],"VOLUME_BARREL") == 0) {
    i_vub = ApiUnit_Volume_BARREL();
  }

/* alternate density unit */
  i_dua = NULL;
  if( strcmp(arg[25],"DENSITY_KGM3") == 0) {
    i_dua = ApiUnit_Density_KGM3();
  }
  if( strcmp(arg[25],"DENSITY_API") == 0) {
    i_dua = ApiUnit_Density_API();
  }
  if( strcmp(arg[25],"DENSITY_RELATIVE") == 0) {
    i_dua = ApiUnit_Density_RELATIVE();
  }

/* base density unit */
  i_dub = NULL;
  if( strcmp(arg[26],"DENSITY_KGM3") == 0) {
    i_dub = ApiUnit_Density_KGM3();
  }
  if( strcmp(arg[26],"DENSITY_API") == 0) {
    i_dub = ApiUnit_Density_API();
  }
  if( strcmp(arg[26],"DENSITY_RELATIVE") == 0) {
    i_dub = ApiUnit_Density_RELATIVE();
  }
/*
  printf("%s \n",procedure); 
  printf("%f \n",baseTemp);
  printf("%f \n",alpha60);
  printf("%f \n",obTemp);
  printf("%f \n",obPres);
  printf("%f \n",obDens);
  printf("%f \n",altTemp);
  printf("%f \n",altPres);
  printf("%f \n",obVol);
  printf("%f \n",altVol);
  printf("%f \n",baseVol);
  printf("%f \n",baseDens);
  printf("%d \n",commodityName);
  printf("%s \n",arg[14]);
  printf("%s \n",arg[15]);
  printf("%s \n",arg[16]);
  printf("%s \n",arg[17]);
  printf("%s \n",arg[18]);
  printf("%s \n",arg[19]);
  printf("%s \n",arg[20]);
  printf("%s \n",arg[21]);
  printf("%s \n",arg[22]);
  printf("%s \n",arg[23]);
  printf("%s \n",arg[24]);
  printf("%s \n",arg[25]);
  printf("%s \n",arg[26]);

  printf("Parameters are o.k. \n  \n");
*/

/* set parameters for calculation 
----------------------------------*/
  if( strcmp(procedure,"F_111_61") == 0) {
   
    errorCode = ApiCalculation_set11_1_6_1Parameters(request,
                                          commodityName,
                                          alpha60,  i_au,
                                          altTemp,  i_tua,
                                          altPres,  i_pua,  
                                          baseDens, i_dub,
                                          altVol,   i_vua);

    if( errorCode != 0) {
      printf("ERROR:CALCULATION_ERROR\n");
      printf("%d\n",errorCode);
      return -1;
    }


  }                                         
  else if( strcmp(procedure,"F_111_62") == 0) {
    
    errorCode = ApiCalculation_set11_1_6_2Parameters(request,
                                          commodityName,
                                          alpha60,  i_au,
                                          obTemp,   i_tuo,
                                          obPres,   i_puo,  
                                          obDens,   i_duo,
                                          obVol,    i_vuo);

    if( errorCode != 0) {
      printf("ERROR:CALCULATION_ERROR\n");
      printf("%d\n",errorCode);
      return -1;        }
    
    }
    else if( strcmp(procedure,"F_111_63") == 0) {

    errorCode = ApiCalculation_set11_1_6_3Parameters(request,
                                          commodityName,
                                          alpha60,  i_au,
                                          obTemp,   i_tuo,
                                          obPres,   i_puo,  
                                          obDens,   i_duo,
                                          altTemp,  i_tua,
                                          altPres,  i_pua,
                                          obVol,    i_vuo);

    if( errorCode != 0) {
      printf("ERROR:CALCULATION_ERROR\n");
      printf("%d\n",errorCode);
      return -1;        }

      }
      else if( strcmp(procedure,"F_111_71") == 0) {   

    errorCode = ApiCalculation_set11_1_7_1Parameters(request,
                                          commodityName,
                                          alpha60,  i_au,
                                          altTemp,  i_tua,
                                          altPres,  i_pua,  
                                          baseDens, i_dub,
                                          baseTemp, i_tub, 
                                          altVol,   i_vua);
    if( errorCode != 0) {
      printf("ERROR:CALCULATION_ERROR\n");
      printf("%d\n",errorCode);
      return -1;        }

        }
        else if( strcmp(procedure,"F_111_72") == 0) {
  
    errorCode = ApiCalculation_set11_1_7_2Parameters(request,
                                          commodityName,
                                          alpha60,  i_au,
                                          obTemp,   i_tuo,
                                          obPres,   i_puo,  
                                          obDens,   i_duo,
                                          baseTemp, i_tub, 
                                          obVol,    i_vuo);



    if( errorCode != 0) {
      printf("ERROR:CALCULATION_ERROR\n");
      printf("%d\n",errorCode);
      return -1;        }


          }
          else if( strcmp(procedure,"F_111_73") == 0) {
    
    errorCode = ApiCalculation_setParameters(request,
                                          commodityName,
                                          alpha60,  i_au,
                                          obTemp,   i_tuo,
                                          altTemp,  i_tua,
                                          obPres,   i_puo,
                                          altPres,  i_pua,  
                                          obDens,   i_duo,
                                          baseTemp, i_tub,
                                          0,        i_pub,
                                          0,        i_vuo,
                                          0,        i_vuo, 
                                          obVol,    i_vuo);

    if( errorCode != 0) {
      printf("ERROR:CALCULATION_ERROR_1\n");
      printf("%d\n",errorCode);
      return -1;        }

            }
            else {
              
              printf("ERROR:FUNCTION_NOT_FOUND\n");
              return -1;
            }

/* now perform the calculation 
-------------------------------*/
  errorCode = ApiCalculationRequest_performCalculation(request);
  r_cstat   = errorCode;
   
  if( errorCode != 0) {
    printf("ERROR:CALCULATION_ERROR\n");
    printf("%d\n",errorCode);
    return -1;
  }

/* get results and pass them back 
----------------------------------*/

/* double r_altVol, 
          r_baseVol, 
          r_altDens,
          r_scaledCompAlt,
          r_baseDens,
          r_ctlObToBase,
          r_cplObToBase,
          r_ctplObToBase,
          r_scaledCompOb,
          r_ctlBaseToAlt,
          r_cplBaseToAlt,
          r_ctplBaseToAlt,
          r_vcf1,
          r_vcf2;       
  char *r_vua, *r_vub, *r_dua, *r_dub;
  int  r_cstat; 
*/

/* 11.1.6.1. and 11.1.7.1 */
if ( ( strcmp(procedure,"F_111_61") == 0) ||
     ( strcmp(procedure,"F_111_71") == 0) ) {

/* scaled compression alternate  */
  r_scaledCompAlt  = ApiQuantity_getValue( 
                               (ApiQuantity*)request->scaledCompAlt,
                                cu,
	  		 	        FALSE,
				        &errorCode);  
  if( errorCode != 0) {
    r_scaledCompAlt = errorValue;
  }
/* CTL base alternate  */
  r_ctlBaseToAlt   = ApiQuantity_getValue( 
                               (ApiQuantity*)request->ctlBaseToAlt,
                                ApiUnit_Expansion_DIMLESS(),
	  		 	        FALSE,
				        &errorCode);  
  if( errorCode != 0) {
    r_ctlBaseToAlt  = errorValue;
  }
/* CPL base alternate  */
  r_cplBaseToAlt   = ApiQuantity_getValue( 
                               (ApiQuantity*)request->cplBaseToAlt,
                                ApiUnit_Expansion_DIMLESS(),
	  		 	        FALSE,
				        &errorCode);  
  if( errorCode != 0) {
    r_cplBaseToAlt  = errorValue;
  }

/* CTPL base alternate  */
  r_ctplBaseToAlt   = ApiQuantity_getValue( 
                               (ApiQuantity*)request->ctplBaseToAlt,
                                ApiUnit_Expansion_DIMLESS(),
	  		 	        TRUE,
				        &errorCode);  
  if( errorCode != 0) {
    r_ctplBaseToAlt  = errorValue;
  }

/* volume correction factors ob to base = 1 */
  r_ctlObToBase  = 1;
  r_cplObToBase  = 1;
  r_ctplObToBase = 1;

/* calculated volumes */
  r_altVol  = altVol;
  r_baseVol = errorValue;
  if( i_vua != NULL ) {
    r_baseVol        = ApiQuantity_getValue( 
                                 (ApiQuantity*)request->baseVol,
                                  i_vua,
	  		 	          FALSE,
				          &errorCode);  
    if( errorCode != 0) {
      r_baseVol = errorValue;
    }
  }

/* alternate density */
  r_altDens        = ApiQuantity_getValue( 
                               (ApiQuantity*)request->altDens,
                                i_dub,
                                FALSE,
                                &errorCode);  
  if( errorCode != 0) {
    r_altDens = errorValue;
  }

/* base density */
  r_baseDens = baseDens;

/* scaled compression factor at observed */
  r_scaledCompOb = errorValue;

/* VCF1, VCF2 */
  r_vcf1 = r_ctplBaseToAlt;
  r_vcf2 = r_ctplBaseToAlt;

/* units of measurement */
  r_vua = arg[23]; //vua
  r_vub = arg[23]; //vua
  r_dua = arg[26]; //dub
  r_dub = arg[26]; //dub

}

/* 11.1.6.2 and 11.1.7.2 */
if ( ( strcmp(procedure,"F_111_62") == 0) ||
     ( strcmp(procedure,"F_111_72") == 0) ) {

/* scaled compression alternate  */
    r_scaledCompAlt = errorValue;
  
/* CTL observed base */
  r_ctlObToBase    = ApiQuantity_getValue( 
                               (ApiQuantity*)request->ctlObToBase,
                                ApiUnit_Expansion_DIMLESS(),
	  		 	        FALSE,
				        &errorCode);  
  if( errorCode != 0) {
    r_ctlObToBase  = errorValue;
  }

/* CPL observed base */
  r_cplObToBase    = ApiQuantity_getValue( 
                               (ApiQuantity*)request->cplObToBase,
                                ApiUnit_Expansion_DIMLESS(),
	  		 	        FALSE,
				        &errorCode);  
  if( errorCode != 0) {
    r_cplObToBase  = errorValue;
  }
  
/* CTPL observed base */
  r_ctplObToBase    = ApiQuantity_getValue( 
                               (ApiQuantity*)request->ctplObToBase,
                                ApiUnit_Expansion_DIMLESS(),
	  		 	        TRUE,
				        &errorCode);  
  if( errorCode != 0) {
    r_ctplObToBase  = errorValue;
  }

/* volume correction factors base to alternate = 1 */
  r_ctlBaseToAlt  = 1;
  r_cplBaseToAlt  = 1;
  r_ctplBaseToAlt = 1;

/* calculated volumes */
  r_altVol  = errorValue;
  if( i_vuo != NULL ) {
    r_baseVol        = ApiQuantity_getValue( 
                                 (ApiQuantity*)request->baseVol,
                                  i_vuo,
	  		 	          FALSE,
				          &errorCode);  
    if( errorCode != 0) {
      r_baseVol = errorValue;
    }
  }

/* base density */
  r_baseDens         = ApiQuantity_getValue( 
                               (ApiQuantity*)request->baseDens,
                                i_duo,
                                FALSE,
                                &errorCode);  
  if( errorCode != 0) {
    r_baseDens = errorValue;
  }


/* alternate density */
    r_altDens = errorValue;

/* scaled compression factor at observed */
  r_scaledCompOb   = ApiQuantity_getValue( 
                               (ApiQuantity*)request->scaledCompOb,
                                cu,
	  		 	        FALSE,
				        &errorCode);  
  if( errorCode != 0) {
    r_scaledCompOb = errorValue;
  }

/* VCF1, VCF2 */
  r_vcf1 = r_ctplBaseToAlt;
  r_vcf2 = r_ctplBaseToAlt;


/* units of measurement */
  r_vua = arg[23]; //vua
  r_vub = arg[22]; //vuo
  r_dua = arg[26]; //dub
  r_dub = arg[19]; //duo

}

/* 11.1.6.3 and 11.1.7.3 */
if ( ( strcmp(procedure,"F_111_63") == 0) ||
     ( strcmp(procedure,"F_111_73") == 0) ) {

/* scaled compression alternate  */
  r_scaledCompAlt = errorValue;
  r_scaledCompAlt  = ApiQuantity_getValue( 
                               (ApiQuantity*)request->scaledCompAlt,
                                cu,
	  		 	        FALSE,
				        &errorCode);  
  if( errorCode != 0) {
    r_scaledCompAlt= errorValue;
  }


/* scaled compression factor at observed */
  r_scaledCompOb   = ApiQuantity_getValue( 
                               (ApiQuantity*)request->scaledCompOb,
                                cu,
	  		 	        FALSE,
				        &errorCode);  
  if( errorCode != 0) {
    r_scaledCompOb = errorValue;
  }
  
/* CTL observed base */
  r_ctlObToBase    = ApiQuantity_getValue( 
                               (ApiQuantity*)request->ctlObToBase,
                                ApiUnit_Expansion_DIMLESS(),
	  		 	        FALSE,
				        &errorCode);  
  if( errorCode != 0) {
    r_ctlObToBase  = errorValue;
  }

/* CPL observed base */
  r_cplObToBase    = ApiQuantity_getValue( 
                               (ApiQuantity*)request->cplObToBase,
                                ApiUnit_Expansion_DIMLESS(),
	  		 	        FALSE,
				        &errorCode);  
  if( errorCode != 0) {
    r_cplObToBase  = errorValue;
  }
  
/* CTPL observed base */
  r_ctplObToBase    = ApiQuantity_getValue( 
                               (ApiQuantity*)request->ctplObToBase,
                                ApiUnit_Expansion_DIMLESS(),
	  		 	        TRUE,
				        &errorCode);  
  if( errorCode != 0) {
    r_ctplObToBase  = errorValue;
  }

/* CTL  base alternate */
  r_ctlBaseToAlt    = ApiQuantity_getValue( 
                               (ApiQuantity*)request-> ctlBaseToAlt,
                                ApiUnit_Expansion_DIMLESS(),
	  		 	        FALSE,
				        &errorCode);  
  if( errorCode != 0) {
    r_ctlBaseToAlt  = errorValue;
  }

/* CPL base alternate */
  r_cplBaseToAlt    = ApiQuantity_getValue( 
                               (ApiQuantity*)request->cplBaseToAlt,
                                ApiUnit_Expansion_DIMLESS(),
	  		 	        FALSE,
				        &errorCode);  
  if( errorCode != 0) {
    r_cplBaseToAlt  = errorValue;
  }
  
/* CTPL base alternate */
  r_ctplBaseToAlt    = ApiQuantity_getValue( 
                               (ApiQuantity*)request->ctplBaseToAlt,
                                ApiUnit_Expansion_DIMLESS(),
	  		 	        TRUE,
				        &errorCode);  
  if( errorCode != 0) {
    r_ctplBaseToAlt  = errorValue;
  }


  r_vcf1 = r_ctplBaseToAlt;
  r_vcf2 = r_ctplBaseToAlt;


/* calculated volumes */
  r_baseVol = errorValue;
  if( i_vuo != NULL ) {
    r_altVol        = ApiQuantity_getValue( 
                                 (ApiQuantity*)request->altVol,
                                  i_vuo,
	  		 	          FALSE,
				          &errorCode);  
    if( errorCode != 0) {
      r_altVol = errorValue;
    }
  }

/* base density */
  r_baseDens        = ApiQuantity_getValue( 
                               (ApiQuantity*)request->baseDens,
                                i_duo,
                                FALSE,
                                &errorCode);  
  if( errorCode != 0) {
    r_baseDens = errorValue;
  }


/* alternate density */
  r_altDens         = ApiQuantity_getValue( 
                               (ApiQuantity*)request->altDens,
                                i_duo,
                                FALSE,
                                &errorCode);  
  if( errorCode != 0) {
    r_altDens = errorValue;
  }

/* units of measurement */
  r_vua = arg[22]; //vuo
  r_vub = arg[24]; //vub
  r_dua = arg[19]; //duo
  r_dub = arg[26]; //dub

}

/* return results of calculation 
---------------------------------*/
//printf("Results of calculation\n");  
  printf("%30.16e \n",r_altVol);  
  printf("%30.16e \n",r_baseVol);
  printf("%30.16e \n",r_altDens);
  printf("%30.16e \n",r_scaledCompAlt);
  printf("%30.16e \n",r_baseDens);
  printf("%30.16e \n",r_ctlObToBase);
  printf("%30.16e \n",r_cplObToBase);
  printf("%30.16e \n",r_ctplObToBase);
  printf("%30.16e \n",r_scaledCompOb);
  printf("%30.16e \n",r_ctlBaseToAlt);
  printf("%30.16e \n",r_cplBaseToAlt);
  printf("%30.16e \n",r_ctplBaseToAlt);
  printf("%30.16e \n",r_vcf1); 
  printf("%30.16e \n",r_vcf2);   
  printf("%s \n",r_vua);  
  printf("%s \n",r_vub);  
  printf("%s \n",r_dua);  
  printf("%s \n",r_dub);  
  printf("%d \n",r_cstat);  
 
  return 0;

  /** dispose of the request and clean all resources */
  ApiCalculationRequest_cleanUp( request);
  ApiCalculationRequest_destroy();
}
