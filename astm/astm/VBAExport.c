/**
 * Copyright (c) 2003 American Petroleum Institute
 *
 * THIS SOFTWARE IS PROVIDED ``AS IS'' AND ANY EXPRESSED OR IMPLIED
 * WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
 * OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED.  IN NO EVENT SHALL WIARDA AND
 * ITS CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF
 * USE, DATA, OR PROFITS; BUSINESS INTERRUPTION; OR ENVIRONMENTAL IMPACTS) 
 * HOWEVER CAUSED AND
 * ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
 * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT
 * OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 * ====================================================================
 */

#include "VBAExport.h"
#include "apiVCF.h"

#undef API_ALPHA60_REV_F_UNIT
#undef API_ALPHA60_REV_C_UNIT
#undef API_TEMPERATURE_F_UNIT
#undef API_TEMPERATURE_C_UNIT
#undef API_PRESSURE_PSI_UNIT 
#undef API_PRESSURE_KPA_UNIT 
#undef API_PRESSURE_BAR_UNIT 
#undef API_DENSITY_API    
#undef API_DENSITY_RELATIVE
#undef API_DENSITY_KGM3   
#undef API_UNIT_NOT_DEFINED

#define API_ALPHA60_REV_F_UNIT 0
#define API_ALPHA60_REV_C_UNIT 1
#define API_TEMPERATURE_F_UNIT 2
#define API_TEMPERATURE_C_UNIT 3
#define API_PRESSURE_PSI_UNIT 4
#define API_PRESSURE_KPA_UNIT 5
#define API_PRESSURE_BAR_UNIT 6
#define API_DENSITY_API 7
#define API_DENSITY_RELATIVE 8
#define API_DENSITY_KGM3 9
#define API_UNIT_NOT_DEFINED 10

static ApiCalculationRequest *calculationRequest = NULL;

int _ApiCalculation_Perform(int commodity,
 				        double alpha60,  int au,
   				        double obTemp,   int tuo,
				        double altTemp,  int tua,
				        double obPres,   int puo,
				        double altPres,  int  pua,
				        double obDens,   int duo,
				        double baseTemp, int tub,
				        double basePres, int pub,
						double *ctlObToBase, double *cplObToBase,
						double *ctlBaseToAlt, double *cplBaseToAlt,
						double *baseDens, double *altDens){
  int errorCode = 0;
  ApiUnit *aUnit, *toUnit, *taUnit, *poUnit, *paUnit, *oDUnit;
  ApiUnit *tbUnit,*pbUnit;

  if( calculationRequest == NULL){
     calculationRequest = ApiCalculationRequest_init(&errorCode);
     if( errorCode != 0) return errorCode;
  }

  /* set the unit for alpha60 */
  switch( au){
    case API_ALPHA60_REV_F_UNIT:
      aUnit = ApiUnit_ThermalExp_REV_F();
      break;
    case API_ALPHA60_REV_C_UNIT:
      aUnit = ApiUnit_ThermalExp_REV_C();
      break;
    default:
      aUnit = NULL;
  }
  
  /* set the unit for the observed temperature */
  switch( tuo){
     case API_TEMPERATURE_F_UNIT:
       toUnit =  ApiUnit_Temperature_F();
       break;
     case API_TEMPERATURE_C_UNIT:
       toUnit =  ApiUnit_Temperature_C();
       break;
    default:
      toUnit = NULL;
  }

  /* set the unit for the alternate temperature */
  switch( tua){
     case API_TEMPERATURE_F_UNIT:
       taUnit =  ApiUnit_Temperature_F();
       break;
     case API_TEMPERATURE_C_UNIT:
       taUnit =  ApiUnit_Temperature_C();
       break;
    default:
      taUnit = NULL;
  }


  /* set the unit for the base temperature */
  switch( tub){
     case API_TEMPERATURE_F_UNIT:
       tbUnit =  ApiUnit_Temperature_F();
       break;
     case API_TEMPERATURE_C_UNIT:
       tbUnit =  ApiUnit_Temperature_C();
       break;
    default:
      tbUnit = NULL;
  }

  /* set the unit for the observed pressure */
  switch( puo){
     case API_PRESSURE_PSI_UNIT:
       poUnit =   ApiUnit_Pressure_PSI();
       break;
     case API_PRESSURE_KPA_UNIT:
       poUnit =   ApiUnit_Pressure_KPA();
       break;
     case API_PRESSURE_BAR_UNIT:
       poUnit =   ApiUnit_Pressure_BAR();
       break;
     default:
       poUnit = NULL;
  }
     

  /* set the unit for the alternate pressure */
  switch( pua){
     case API_PRESSURE_PSI_UNIT:
       paUnit =   ApiUnit_Pressure_PSI();
       break;
     case API_PRESSURE_KPA_UNIT:
       paUnit =   ApiUnit_Pressure_KPA();
       break;
     case API_PRESSURE_BAR_UNIT:
       paUnit =   ApiUnit_Pressure_BAR();
       break;
     default:
       paUnit = NULL;
  }
     

  /* set the unit for the base pressure */
  switch( pub){
     case API_PRESSURE_PSI_UNIT:
       pbUnit =   ApiUnit_Pressure_PSI();
       break;
     case API_PRESSURE_KPA_UNIT:
       pbUnit =   ApiUnit_Pressure_KPA();
       break;
     case API_PRESSURE_BAR_UNIT:
       pbUnit =   ApiUnit_Pressure_BAR();
       break;
     default:
       pbUnit = NULL;
  }
     
  /** Set the unit for the observed density */
  switch(duo){
     case API_DENSITY_API:
        oDUnit = ApiUnit_Density_API();
        break;
     case API_DENSITY_RELATIVE:
        oDUnit = ApiUnit_Density_RELATIVE();
        break;
     case API_DENSITY_KGM3:
        oDUnit = ApiUnit_Density_KGM3();
        break;
     default:
        oDUnit = NULL;
  }

  /** Set the calculation parameters */
  errorCode = ApiCalculation_setParameters(calculationRequest,
				           commodity,
				           alpha60, aUnit,
					   obTemp, toUnit,
					   altTemp, taUnit,
					   obPres, poUnit,
				           altPres,paUnit,
				           obDens, oDUnit,
				           baseTemp, tbUnit,
				           basePres, pbUnit,

					   0, NULL,
					   0, NULL,
					   0, NULL);
  if( errorCode != 0) return errorCode;
  
  /** Do the calculation */
  errorCode = ApiCalculationRequest_performCalculation(calculationRequest);
  if( errorCode != 0) return errorCode;   
  
  *ctlBaseToAlt = ApiQuantity_getValue((ApiQuantity*)calculationRequest->ctlBaseToAlt,
			      ApiUnit_Expansion_DIMLESS(),
			      FALSE,
			      &errorCode);
				  
  *cplBaseToAlt = ApiQuantity_getValue((ApiQuantity*)calculationRequest->cplBaseToAlt,
			      ApiUnit_Expansion_DIMLESS(),
			      FALSE,
			      &errorCode);
				  				  
  *ctlObToBase = ApiQuantity_getValue((ApiQuantity*)calculationRequest->ctlObToBase,
			      ApiUnit_Expansion_DIMLESS(),
			      FALSE,
			      &errorCode);
   
  *cplObToBase = ApiQuantity_getValue((ApiQuantity*)calculationRequest->cplObToBase,
			      ApiUnit_Expansion_DIMLESS(),
			      FALSE,
			      &errorCode);
						
  *baseDens = ApiQuantity_getValue((ApiQuantity*)calculationRequest->baseDens,
			      oDUnit,
			      FALSE,
			      &errorCode);
				  
  *altDens = ApiQuantity_getValue((ApiQuantity*)calculationRequest->altDens,
			      oDUnit,
			      FALSE,
			      &errorCode);
				 
  return errorCode;
};

/**
* Sets all possible input parameters and performs a CVS calculation.
*
* @param commodity the commodity for which to do the calculation. If set to -1,
*                  it is assumed that an alpha60 value is supplied
* @param alpha60 the value for  alpha60  
* @param au the unit for alpha60. If API_UNIT_NOT_DEFINED, we set 
*        alpha60 to NULL and assume that a  commodity is supplied.
*
* @param obTemp the observed temperature to use
* @param tuo the unit to use for the observed temperature
* @param altTemp the alternate temperature to use
* @param tua the unit to use for the alternate temperature
* @param obPres the observed pressure to use
* @param puo the unit to use for the observed pressure
* @param altPres the alternate pressure to use
* @param pua the unit to use for the alternate pressure
* @param obDens the observed density
* @param duo the unit to use for the observed density
*
* @param baseTemp the base temperature to which to calculate the 
*        expansion factor
* @param tub  the unit for the the base temperature
*
* @param basePres the base pressure to which to calculate the 
*        expansion factors
* @param pub  the unit for the the base pressure
*
* @return 0 if calculation was successful, an errorCode otherwise
*/

int ApiCalculation_VBAExportCalculation(int commodity,
 				        double alpha60,  int au,
   				        double obTemp,   int tuo,
				        double altTemp,  int tua,
				        double obPres,   int puo,
				        double altPres,  int  pua,
				        double obDens,   int duo,
				        double baseTemp, int tub,
				        double basePres, int pub){
  int errorCode = 0;
  ApiUnit *aUnit, *toUnit, *taUnit, *poUnit, *paUnit, *oDUnit;
  ApiUnit *tbUnit,*pbUnit;

  if( calculationRequest == NULL){
     calculationRequest = ApiCalculationRequest_init(&errorCode);
     if( errorCode != 0) return errorCode;
  }

  /* set the unit for alpha60 */
  switch( au){
    case API_ALPHA60_REV_F_UNIT:
      aUnit = ApiUnit_ThermalExp_REV_F();
      break;
    case API_ALPHA60_REV_C_UNIT:
      aUnit = ApiUnit_ThermalExp_REV_C();
      break;
    default:
      aUnit = NULL;
  }
  
  /* set the unit for the observed temperature */
  switch( tuo){
     case API_TEMPERATURE_F_UNIT:
       toUnit =  ApiUnit_Temperature_F();
       break;
     case API_TEMPERATURE_C_UNIT:
       toUnit =  ApiUnit_Temperature_C();
       break;
    default:
      toUnit = NULL;
  }

  /* set the unit for the alternate temperature */
  switch( tua){
     case API_TEMPERATURE_F_UNIT:
       taUnit =  ApiUnit_Temperature_F();
       break;
     case API_TEMPERATURE_C_UNIT:
       taUnit =  ApiUnit_Temperature_C();
       break;
    default:
      taUnit = NULL;
  }


  /* set the unit for the base temperature */
  switch( tub){
     case API_TEMPERATURE_F_UNIT:
       tbUnit =  ApiUnit_Temperature_F();
       break;
     case API_TEMPERATURE_C_UNIT:
       tbUnit =  ApiUnit_Temperature_C();
       break;
    default:
      tbUnit = NULL;
  }

  /* set the unit for the observed pressure */
  switch( puo){
     case API_PRESSURE_PSI_UNIT:
       poUnit =   ApiUnit_Pressure_PSI();
       break;
     case API_PRESSURE_KPA_UNIT:
       poUnit =   ApiUnit_Pressure_KPA();
       break;
     case API_PRESSURE_BAR_UNIT:
       poUnit =   ApiUnit_Pressure_BAR();
       break;
     default:
       poUnit = NULL;
  }
     

  /* set the unit for the alternate pressure */
  switch( pua){
     case API_PRESSURE_PSI_UNIT:
       paUnit =   ApiUnit_Pressure_PSI();
       break;
     case API_PRESSURE_KPA_UNIT:
       paUnit =   ApiUnit_Pressure_KPA();
       break;
     case API_PRESSURE_BAR_UNIT:
       paUnit =   ApiUnit_Pressure_BAR();
       break;
     default:
       paUnit = NULL;
  }
     

  /* set the unit for the base pressure */
  switch( pub){
     case API_PRESSURE_PSI_UNIT:
       pbUnit =   ApiUnit_Pressure_PSI();
       break;
     case API_PRESSURE_KPA_UNIT:
       pbUnit =   ApiUnit_Pressure_KPA();
       break;
     case API_PRESSURE_BAR_UNIT:
       pbUnit =   ApiUnit_Pressure_BAR();
       break;
     default:
       pbUnit = NULL;
  }
     
  /** Set the unit for the observed density */
  switch(duo){
     case API_DENSITY_API:
        oDUnit = ApiUnit_Density_API();
        break;
     case API_DENSITY_RELATIVE:
        oDUnit = ApiUnit_Density_RELATIVE();
        break;
     case API_DENSITY_KGM3:
        oDUnit = ApiUnit_Density_KGM3();
        break;
     default:
        oDUnit = NULL;
  }

  /** Set the calculation parameters */
  errorCode = ApiCalculation_setParameters(calculationRequest,
				           commodity,
				           alpha60, aUnit,
					   obTemp, toUnit,
					   altTemp, taUnit,
					   obPres, poUnit,
				           altPres,paUnit,
				           obDens, oDUnit,
				           baseTemp, tbUnit,
				           basePres, pbUnit,

					   0, NULL,
					   0, NULL,
					   0, NULL);
  if( errorCode != 0) return errorCode;
  
  /** Do the calculation */
  errorCode = ApiCalculationRequest_performCalculation(calculationRequest);
  return errorCode;
};


/** 
 * Do clean up 
 */
void ApiCalculation_cleanVBAExport(){
  if( calculationRequest != NULL){
    ApiCalculationRequest_cleanUp( calculationRequest);
    calculationRequest = NULL;
  }
}


/**
* Get the ctplBaseToAlt parameter
*
* @param commodity the commodity for which to do the calculation. If set to -1,
*                  it is assumed that an alpha60 value is supplied
* @param alpha60 the value for  alpha60  
* @param au the unit for alpha60. If API_UNIT_NOT_DEFINED, we set 
*        alpha60 to NULL and assume that a  commodity is supplied.
*
* @param altTemp the alternate temperature to use
* @param tua the unit to use for the alternate temperature
* @param altPres the alternate pressure to use
* @param pua the unit to use for the alternate pressure
* @param obDens the observed density
* @param duo the unit to use for the observed density
*
* @param baseTemp the base temperature to which to calculate the 
*        expansion factor
* @param tub  the unit for the the base temperature
*
* @param basePres the base pressure to which to calculate the 
*        expansion factors
* @param pub  the unit for the the base pressure
*
* @param errorCode the errorCode to reprot
*
* @return 0 if calculation was successful, an errorCode otherwise
*/
double ApiCalculation_getCtplBaseToAlt(int commodity,
				       double alpha60,  int au,
				       double altTemp,  int tua,
				       double altPres,  int  pua,
				       double obDens,   int duo,
				       double baseTemp, int tub,
				       double basePres, int pub,
                                       int *errorCode){
  double ctpl = 0;
  *errorCode = ApiCalculation_VBAExportCalculation(commodity,
						   alpha60, au,
                           baseTemp, tub,
						   altTemp, tua, 
						   basePres, pub,
						   altPres,  pua,
						   obDens, duo,
						   baseTemp, tub,
						   basePres, pub);
  if( *errorCode != 0) return ctpl;
  ctpl = ApiQuantity_getValue((ApiQuantity*)calculationRequest->ctplBaseToAlt,
			      ApiUnit_Expansion_DIMLESS(),
			      FALSE,
			      errorCode);
  if( *errorCode != 0) ctpl = 0;
  return ctpl;   
};

/**
* Get the ctplBaseToAlt parameter
*
* @param commodity the commodity for which to do the calculation. If set to -1,
*                  it is assumed that an alpha60 value is supplied
* @param alpha60 the value for  alpha60  
* @param au the unit for alpha60. If API_UNIT_NOT_DEFINED, we set 
*        alpha60 to NULL and assume that a  commodity is supplied.
*
* @param altTemp the alternate temperature to use
* @param tua the unit to use for the alternate temperature
* @param altPres the alternate pressure to use
* @param pua the unit to use for the alternate pressure
* @param obDens the observed density
* @param duo the unit to use for the observed density
*
* @param baseTemp the base temperature to which to calculate the 
*        expansion factor
* @param tub  the unit for the the base temperature
*
* @param basePres the base pressure to which to calculate the 
*        expansion factors
* @param pub  the unit for the the base pressure
*
* @param errorCode the errorCode to reprot
*
* @return 0 if calculation was successful, an errorCode otherwise
*/      
double _ApiCalculation_getCtplBaseToAlt(int commodity,
				       double alpha60,  int au,
				       double altTemp,  int tua,
				       double altPres,  int  pua,
				       double obDens,   int duo,
				       double baseTemp, int tub,
				       double basePres, int pub,
					   double *ctl, double *cpl,
                       int *errorCode){
  double ctpl = 0;
  *errorCode = ApiCalculation_VBAExportCalculation(commodity,
						   alpha60, au,
                           baseTemp, tub,
						   altTemp, tua, 
						   basePres, pub,
						   altPres,  pua,
						   obDens, duo,
						   baseTemp, tub,
						   basePres, pub);
  if( *errorCode != 0) return ctpl;
  ctpl = ApiQuantity_getValue((ApiQuantity*)calculationRequest->ctplBaseToAlt,
			      ApiUnit_Expansion_DIMLESS(),
			      FALSE,
			      errorCode);
				  
  *ctl = ApiQuantity_getValue((ApiQuantity*)calculationRequest->ctlBaseToAlt,
			      ApiUnit_Expansion_DIMLESS(),
			      FALSE,
			      errorCode);
				  
  *cpl = ApiQuantity_getValue((ApiQuantity*)calculationRequest->cplBaseToAlt,
			      ApiUnit_Expansion_DIMLESS(),
			      FALSE,
			      errorCode);
				  
  if( *errorCode != 0) ctpl = 0;
  return ctpl;   
};



/*--------------------------------------------------------------
*   11.1.6.1 type calculations - get ctpl
*--------------------------------------------------------------*/

/*-------- Density in API ------------------------------------------*/

/**
* Calculate the ctpl factor for transformation from 60F, 0psi to
* the desired alternate conditions for Crude Oil. Density is in API
*/
double ctplA11161API(double density, double temp, double pres, int *errorCode){
  return  ApiCalculation_getCtplBaseToAlt( API_CRUDE_OIL_NAME,
					   0,  API_UNIT_NOT_DEFINED,
					   temp,  API_TEMPERATURE_F_UNIT,
					   pres, API_PRESSURE_PSI_UNIT,
					   density, API_DENSITY_API,
					   API_BASE_TEMP, API_TEMPERATURE_F_UNIT,
					   API_BASE_PRES, API_PRESSURE_PSI_UNIT,
					   errorCode);
}

double _ctplA11161API(double density, double temp, double pres, double *ctl, double *cpl, int *errorCode){
  return  _ApiCalculation_getCtplBaseToAlt( API_CRUDE_OIL_NAME,
					   0,  API_UNIT_NOT_DEFINED,
					   temp,  API_TEMPERATURE_F_UNIT,
					   pres, API_PRESSURE_PSI_UNIT,
					   density, API_DENSITY_API,
					   API_BASE_TEMP, API_TEMPERATURE_F_UNIT,
					   API_BASE_PRES, API_PRESSURE_PSI_UNIT,
					   ctl, cpl,
					   errorCode);
}


/**
* Calculate the ctpl factor for transformation from 60F, 0psi to
* the desired alternate conditions for Refined Products. Density is in API
*/
double ctplB11161API(double density, double temp, double pres, int *errorCode){
  double ctpl;
  ctpl =  ApiCalculation_getCtplBaseToAlt( API_REFINED_PRODUCTS_NAME,
					   0,  API_UNIT_NOT_DEFINED,
					   temp,  API_TEMPERATURE_F_UNIT,
					   pres, API_PRESSURE_PSI_UNIT,
					   density, API_DENSITY_API,
					   API_BASE_TEMP, API_TEMPERATURE_F_UNIT,
					   API_BASE_PRES, API_PRESSURE_PSI_UNIT,
					   errorCode);
  return ctpl;
}

double _ctplB11161API(double density, double temp, double pres, double *ctl, double *cpl, int *errorCode){
  double ctpl;
  ctpl =  _ApiCalculation_getCtplBaseToAlt( API_REFINED_PRODUCTS_NAME,
					   0,  API_UNIT_NOT_DEFINED,
					   temp,  API_TEMPERATURE_F_UNIT,
					   pres, API_PRESSURE_PSI_UNIT,
					   density, API_DENSITY_API,
					   API_BASE_TEMP, API_TEMPERATURE_F_UNIT,
					   API_BASE_PRES, API_PRESSURE_PSI_UNIT,
					   ctl, cpl,
					   errorCode);
  return ctpl;
}

/**
* Calculate the ctpl factor for transformation from 60F, 0psi to
* the desired alternate conditions for Lubrication Oil. Density is in API
*/
double ctplD11161API(double density, double temp, double pres, int *errorCode){
  return  ApiCalculation_getCtplBaseToAlt( API_LUBRICATING_OIL_NAME,
					   0,  API_UNIT_NOT_DEFINED,
					   temp,  API_TEMPERATURE_F_UNIT,
					   pres, API_PRESSURE_PSI_UNIT,
					   density, API_DENSITY_API,
					   API_BASE_TEMP, API_TEMPERATURE_F_UNIT,
					   API_BASE_PRES, API_PRESSURE_PSI_UNIT,
					   errorCode);
}

double _ctplD11161API(double density, double temp, double pres, double *ctl, double *cpl, int *errorCode){
  return  _ApiCalculation_getCtplBaseToAlt( API_LUBRICATING_OIL_NAME,
					   0,  API_UNIT_NOT_DEFINED,
					   temp,  API_TEMPERATURE_F_UNIT,
					   pres, API_PRESSURE_PSI_UNIT,
					   density, API_DENSITY_API,
					   API_BASE_TEMP, API_TEMPERATURE_F_UNIT,
					   API_BASE_PRES, API_PRESSURE_PSI_UNIT,
					   ctl, cpl,
					   errorCode);
}


/**
* Calculate the ctpl factor for transformation from 60F, 0psi to
* the desired alternate conditions for supplied alpha values.
* Alpha60 is in 1/F  Density is in API
*/
double ctplC11161API(double alpha60, double density, double temp, double pres, int *errorCode){ 
  return  ApiCalculation_getCtplBaseToAlt( API_COMMODITY_NOT_GIVEN,
					   alpha60, API_ALPHA60_REV_F_UNIT,
					   temp,  API_TEMPERATURE_F_UNIT,
					   pres, API_PRESSURE_PSI_UNIT,
					   density, API_DENSITY_API,
					   API_BASE_TEMP, API_TEMPERATURE_F_UNIT,
					   API_BASE_PRES, API_PRESSURE_PSI_UNIT,
					   errorCode);
}

double _ctplC11161API(double alpha60, double density, double temp, double pres, double *ctl, double *cpl, int *errorCode){ 
  return  _ApiCalculation_getCtplBaseToAlt( API_COMMODITY_NOT_GIVEN,
					   alpha60, API_ALPHA60_REV_F_UNIT,
					   temp,  API_TEMPERATURE_F_UNIT,
					   pres, API_PRESSURE_PSI_UNIT,
					   density, API_DENSITY_API,
					   API_BASE_TEMP, API_TEMPERATURE_F_UNIT,
					   API_BASE_PRES, API_PRESSURE_PSI_UNIT,
					   ctl, cpl,
					   errorCode);
}

/*-------- Density in Rel. Dens ------------------------------------------*/

/**
* Calculate the ctpl factor for transformation from 60F, 0psi to
* the desired alternate conditions for Crude Oil. Density is in Rel. Dens
*/
double ctplA11161RelDens(double density, double temp, double pres, int *errorCode){
  return  ApiCalculation_getCtplBaseToAlt( API_CRUDE_OIL_NAME,
					   0,  API_UNIT_NOT_DEFINED,
					   temp,  API_TEMPERATURE_F_UNIT,
					   pres, API_PRESSURE_PSI_UNIT,
					   density, API_DENSITY_RELATIVE,
					   API_BASE_TEMP, API_TEMPERATURE_F_UNIT,
					   API_BASE_PRES, API_PRESSURE_PSI_UNIT,
					   errorCode);
}

double _ctplA11161RelDens(double density, double temp, double pres, double *ctl, double *cpl, int *errorCode){
  return  _ApiCalculation_getCtplBaseToAlt( API_CRUDE_OIL_NAME,
					   0,  API_UNIT_NOT_DEFINED,
					   temp,  API_TEMPERATURE_F_UNIT,
					   pres, API_PRESSURE_PSI_UNIT,
					   density, API_DENSITY_RELATIVE,
					   API_BASE_TEMP, API_TEMPERATURE_F_UNIT,
					   API_BASE_PRES, API_PRESSURE_PSI_UNIT,
					   ctl, cpl,
					   errorCode);
}

/**
* Calculate the ctpl factor for transformation from 60F, 0psi to
* the desired alternate conditions for Refined Products. Density is in 
* Rel. Dens
*/
double ctplB11161RelDens(double density, double temp, double pres, int *errorCode){
  return  ApiCalculation_getCtplBaseToAlt( API_REFINED_PRODUCTS_NAME,
					   0,  API_UNIT_NOT_DEFINED,
					   temp,  API_TEMPERATURE_F_UNIT,
					   pres, API_PRESSURE_PSI_UNIT,
					   density, API_DENSITY_RELATIVE,
					   API_BASE_TEMP, API_TEMPERATURE_F_UNIT,
					   API_BASE_PRES, API_PRESSURE_PSI_UNIT,
					   errorCode);
}

double _ctplB11161RelDens(double density, double temp, double pres, double *ctl, double *cpl, int *errorCode){
  return  _ApiCalculation_getCtplBaseToAlt( API_REFINED_PRODUCTS_NAME,
					   0,  API_UNIT_NOT_DEFINED,
					   temp,  API_TEMPERATURE_F_UNIT,
					   pres, API_PRESSURE_PSI_UNIT,
					   density, API_DENSITY_RELATIVE,
					   API_BASE_TEMP, API_TEMPERATURE_F_UNIT,
					   API_BASE_PRES, API_PRESSURE_PSI_UNIT,
					   ctl, cpl,
					   errorCode);
}

/**
* Calculate the ctpl factor for transformation from 60F, 0psi to
* the desired alternate conditions for Lubrication Oil. Density is in Rel. Dens.
*/
double ctplD11161RelDens(double density, double temp, double pres, int *errorCode){
  return  ApiCalculation_getCtplBaseToAlt( API_LUBRICATING_OIL_NAME,
					   0,  API_UNIT_NOT_DEFINED,
					   temp,  API_TEMPERATURE_F_UNIT,
					   pres, API_PRESSURE_PSI_UNIT,
					   density, API_DENSITY_RELATIVE,
					   API_BASE_TEMP, API_TEMPERATURE_F_UNIT,
					   API_BASE_PRES, API_PRESSURE_PSI_UNIT,
					   errorCode);
}

double _ctplD11161RelDens(double density, double temp, double pres, double *ctl, double *cpl, int *errorCode){
  return  _ApiCalculation_getCtplBaseToAlt( API_LUBRICATING_OIL_NAME,
					   0,  API_UNIT_NOT_DEFINED,
					   temp,  API_TEMPERATURE_F_UNIT,
					   pres, API_PRESSURE_PSI_UNIT,
					   density, API_DENSITY_RELATIVE,
					   API_BASE_TEMP, API_TEMPERATURE_F_UNIT,
					   API_BASE_PRES, API_PRESSURE_PSI_UNIT,
					   ctl, cpl,
					   errorCode);
}

/**
* Calculate the ctpl factor for transformation from 60F, 0psi to
* the desired alternate conditions for supplied alpha values.
* Alpha60 is in 1/F  Density is in Rel. Dens.
*/
double ctplC11161RelDens(double alpha60, double density, double temp, double pres, int *errorCode){ 
  return  ApiCalculation_getCtplBaseToAlt( API_COMMODITY_NOT_GIVEN,
					   alpha60, API_ALPHA60_REV_F_UNIT,
					   temp,  API_TEMPERATURE_F_UNIT,
					   pres, API_PRESSURE_PSI_UNIT,
					   density, API_DENSITY_RELATIVE,
					   API_BASE_TEMP, API_TEMPERATURE_F_UNIT,
					   API_BASE_PRES, API_PRESSURE_PSI_UNIT,
					   errorCode);
}

double _ctplC11161RelDens(double alpha60, double density, double temp, double pres, double *ctl, double *cpl, int *errorCode){ 
  return  _ApiCalculation_getCtplBaseToAlt( API_COMMODITY_NOT_GIVEN,
					   alpha60, API_ALPHA60_REV_F_UNIT,
					   temp,  API_TEMPERATURE_F_UNIT,
					   pres, API_PRESSURE_PSI_UNIT,
					   density, API_DENSITY_RELATIVE,
					   API_BASE_TEMP, API_TEMPERATURE_F_UNIT,
					   API_BASE_PRES, API_PRESSURE_PSI_UNIT,
					   ctl, cpl,
					   errorCode);
}

/*--------------------------------------------------------------
*   11.1.7.1 type calculations - get ctpl
*--------------------------------------------------------------*/

/**
* Calculate the ctpl factor for transformation from 15C and 0kPa
* the desired alternate conditions for Crude Oil. 
*/
double ctplA11171bar(double density, double temp, double pres, double baseTemp, int *errorCode){ 
  return  ApiCalculation_getCtplBaseToAlt( API_CRUDE_OIL_NAME,
					   0,  API_UNIT_NOT_DEFINED,
					   temp,  API_TEMPERATURE_C_UNIT,
					   pres, API_PRESSURE_BAR_UNIT,
					   density, API_DENSITY_KGM3,
					   baseTemp, API_TEMPERATURE_C_UNIT,
					   0, API_PRESSURE_BAR_UNIT,
					   errorCode);
}

double _ctplA11171bar(double density, double temp, double pres, double baseTemp, double *ctl, double *cpl, int *errorCode){ 
  return  _ApiCalculation_getCtplBaseToAlt( API_CRUDE_OIL_NAME,
					   0,  API_UNIT_NOT_DEFINED,
					   temp,  API_TEMPERATURE_C_UNIT,
					   pres, API_PRESSURE_BAR_UNIT,
					   density, API_DENSITY_KGM3,
					   baseTemp, API_TEMPERATURE_C_UNIT,
					   0, API_PRESSURE_BAR_UNIT,
					   ctl, cpl,
					   errorCode);
}

/**
* Calculate the ctpl factor for transformation from 15C and 0kPa
* the desired alternate conditions for Refined products
*/
double ctplB11171bar(double density, double temp, double pres, double baseTemp, int *errorCode){ 
  return  ApiCalculation_getCtplBaseToAlt(API_REFINED_PRODUCTS_NAME,
					  0,  API_UNIT_NOT_DEFINED,
					  temp,  API_TEMPERATURE_C_UNIT,
					  pres, API_PRESSURE_BAR_UNIT,
					  density, API_DENSITY_KGM3,
					   baseTemp, API_TEMPERATURE_C_UNIT,
					   0, API_PRESSURE_BAR_UNIT,
					   errorCode);
}

double _ctplB11171bar(double density, double temp, double pres, double baseTemp, double *ctl, double *cpl, int *errorCode){ 
  return  _ApiCalculation_getCtplBaseToAlt(API_REFINED_PRODUCTS_NAME,
					  0,  API_UNIT_NOT_DEFINED,
					  temp,  API_TEMPERATURE_C_UNIT,
					  pres, API_PRESSURE_BAR_UNIT,
					  density, API_DENSITY_KGM3,
					   baseTemp, API_TEMPERATURE_C_UNIT,
					   0, API_PRESSURE_BAR_UNIT,
					   ctl, cpl,
					   errorCode);
}

/**
* Calculate the ctpl factor for transformation from 15C and 0kPa
* the desired alternate conditions for Lubrication oil.
*/
double ctplD11171bar(double density, double temp, double pres, double baseTemp, int *errorCode){ 
  return  ApiCalculation_getCtplBaseToAlt(API_LUBRICATING_OIL_NAME,
					  0,  API_UNIT_NOT_DEFINED,
					  temp,  API_TEMPERATURE_C_UNIT,
					  pres, API_PRESSURE_BAR_UNIT,
					  density, API_DENSITY_KGM3,
					   baseTemp, API_TEMPERATURE_C_UNIT,
					   0, API_PRESSURE_BAR_UNIT,
					   errorCode);
}

double _ctplD11171bar(double density, double temp, double pres, double baseTemp, double *ctl, double *cpl, int *errorCode){ 
  return  _ApiCalculation_getCtplBaseToAlt(API_LUBRICATING_OIL_NAME,
					  0,  API_UNIT_NOT_DEFINED,
					  temp,  API_TEMPERATURE_C_UNIT,
					  pres, API_PRESSURE_BAR_UNIT,
					  density, API_DENSITY_KGM3,
					   baseTemp, API_TEMPERATURE_C_UNIT,
					   0, API_PRESSURE_BAR_UNIT,
					   ctl, cpl,
					   errorCode);
}

/**
* Calculate the ctpl factor for transformation from 15C and 0kPa
* the desired alternate conditions for alpha60 supplied
*/
double ctplC11171bar(double alpha60, double density, double temp, double pres, double baseTemp, int *errorCode){ 
  return  ApiCalculation_getCtplBaseToAlt(API_COMMODITY_NOT_GIVEN,
					  alpha60, API_ALPHA60_REV_C_UNIT,
					  temp,  API_TEMPERATURE_C_UNIT,
					  pres, API_PRESSURE_BAR_UNIT,
					  density, API_DENSITY_KGM3,
					  baseTemp, API_TEMPERATURE_C_UNIT,
					  0, API_PRESSURE_BAR_UNIT,
					  errorCode);
}

double _ctplC11171bar(double alpha60, double density, double temp, double pres, double baseTemp, double *ctl, double *cpl, int *errorCode){ 
  return  _ApiCalculation_getCtplBaseToAlt(API_COMMODITY_NOT_GIVEN,
					  alpha60, API_ALPHA60_REV_C_UNIT,
					  temp,  API_TEMPERATURE_C_UNIT,
					  pres, API_PRESSURE_BAR_UNIT,
					  density, API_DENSITY_KGM3,
					  baseTemp, API_TEMPERATURE_C_UNIT,
					  0, API_PRESSURE_BAR_UNIT,
					  ctl, cpl,
					  errorCode);
}

/**
* Calculate the ctpl factor for transformation from 15C and 0kPa
* the desired alternate conditions for Crude Oil. 
*/
double ctplA11171kPa(double density, double temp, double pres, double baseTemp, int *errorCode){ 
  return  ApiCalculation_getCtplBaseToAlt( API_CRUDE_OIL_NAME,
					   0,  API_UNIT_NOT_DEFINED,
					   temp,  API_TEMPERATURE_C_UNIT,
					   pres, API_PRESSURE_KPA_UNIT,
					   density, API_DENSITY_KGM3,
					   baseTemp, API_TEMPERATURE_C_UNIT,
					   0, API_PRESSURE_KPA_UNIT,
					   errorCode);
}

double _ctplA11171kPa(double density, double temp, double pres, double baseTemp, double *ctl, double *cpl, int *errorCode){ 
  return  _ApiCalculation_getCtplBaseToAlt( API_CRUDE_OIL_NAME,
					   0,  API_UNIT_NOT_DEFINED,
					   temp,  API_TEMPERATURE_C_UNIT,
					   pres, API_PRESSURE_KPA_UNIT,
					   density, API_DENSITY_KGM3,
					   baseTemp, API_TEMPERATURE_C_UNIT,
					   0, API_PRESSURE_KPA_UNIT,
					   ctl, cpl,
					   errorCode);
}

/**
* Calculate the ctpl factor for transformation from 15C and 0kPa
* the desired alternate conditions for Refined products
*/
double ctplB11171kPa(double density, double temp, double pres, double baseTemp, int *errorCode){ 
  return  ApiCalculation_getCtplBaseToAlt(API_REFINED_PRODUCTS_NAME,
					  0,  API_UNIT_NOT_DEFINED,
					  temp,  API_TEMPERATURE_C_UNIT,
					  pres, API_PRESSURE_KPA_UNIT,
					  density, API_DENSITY_KGM3,
					  baseTemp, API_TEMPERATURE_C_UNIT,
					   0, API_PRESSURE_KPA_UNIT,
					   errorCode);
}

double _ctplB11171kPa(double density, double temp, double pres, double baseTemp, double *ctl, double *cpl, int *errorCode){ 
  return  _ApiCalculation_getCtplBaseToAlt(API_REFINED_PRODUCTS_NAME,
					  0,  API_UNIT_NOT_DEFINED,
					  temp,  API_TEMPERATURE_C_UNIT,
					  pres, API_PRESSURE_KPA_UNIT,
					  density, API_DENSITY_KGM3,
					  baseTemp, API_TEMPERATURE_C_UNIT,
					  0, API_PRESSURE_KPA_UNIT,
					  ctl, cpl,
					  errorCode);
}

/**
* Calculate the ctpl factor for transformation from 15C and 0kPa
* the desired alternate conditions for Lubrication oil.
*/
double ctplD11171kPa(double density, double temp, double pres, double baseTemp, int *errorCode){ 
  return  ApiCalculation_getCtplBaseToAlt(API_LUBRICATING_OIL_NAME,
					  0,  API_UNIT_NOT_DEFINED,
					  temp,  API_TEMPERATURE_C_UNIT,
					  pres, API_PRESSURE_KPA_UNIT,
					  density, API_DENSITY_KGM3,
					   baseTemp, API_TEMPERATURE_C_UNIT,
					   0, API_PRESSURE_KPA_UNIT,
					   errorCode);
}

double _ctplD11171kPa(double density, double temp, double pres, double baseTemp, double *ctl, double *cpl, int *errorCode){ 
  return  _ApiCalculation_getCtplBaseToAlt(API_LUBRICATING_OIL_NAME,
					  0,  API_UNIT_NOT_DEFINED,
					  temp,  API_TEMPERATURE_C_UNIT,
					  pres, API_PRESSURE_KPA_UNIT,
					  density, API_DENSITY_KGM3,
					   baseTemp, API_TEMPERATURE_C_UNIT,
					   0, API_PRESSURE_KPA_UNIT,
					   ctl, cpl,
					   errorCode);
}

/**
* Calculate the ctpl factor for transformation from 15C and 0kPa
* the desired alternate conditions for alpha60 supplied
*/
double ctplC11171kPa(double alpha60, double density, double temp, double pres, double baseTemp, int *errorCode){ 
  return  ApiCalculation_getCtplBaseToAlt(API_COMMODITY_NOT_GIVEN,
					  alpha60, API_ALPHA60_REV_C_UNIT,
					  temp,  API_TEMPERATURE_C_UNIT,
					  pres, API_PRESSURE_KPA_UNIT,
					  density, API_DENSITY_KGM3,
					   baseTemp, API_TEMPERATURE_C_UNIT,
					   0, API_PRESSURE_KPA_UNIT,
					   errorCode);
}

double _ctplC11171kPa(double alpha60, double density, double temp, double pres, double baseTemp, double *ctl, double *cpl, int *errorCode){ 
  return  _ApiCalculation_getCtplBaseToAlt(API_COMMODITY_NOT_GIVEN,
					  alpha60, API_ALPHA60_REV_C_UNIT,
					  temp,  API_TEMPERATURE_C_UNIT,
					  pres, API_PRESSURE_KPA_UNIT,
					  density, API_DENSITY_KGM3,
					   baseTemp, API_TEMPERATURE_C_UNIT,
					   0, API_PRESSURE_KPA_UNIT,
					   ctl, cpl,
					   errorCode);
}

/**
* Get the ctplAltToBase parameter
*
* @param commodity the commodity for which to do the calculation. If set to -1,
*                  it is assumed that an alpha60 value is supplied
* @param alpha60 the value for  alpha60  
* @param au the unit for alpha60. If API_UNIT_NOT_DEFINED, we set 
*        alpha60 to NULL and assume that a  commodity is supplied.
*
* @param obTemp the temperature at which we observed the data
* @param tuo the unit to use for the observed temperature
* @param obPres the  pressure at which the data was observed
* @param puo the unit to use for the observed pressure
* @param obDens the observed density
* @param duo the unit to use for the observed density
*
* @param baseTemp the base temperature to which to calculate the 
*        expansion factor
* @param tub  the unit for the the base temperature
*
* @param basePres the base pressure to which to calculate the 
*        expansion factors
* @param pub  the unit for the the base pressure
*
* @param errorCode the errorCode to reprot
*
* @return 0 if calculation was successful, an errorCode otherwise
*/
double ApiCalculation_getCtplAltToBase(int commodity,
				       double alpha60,  int au,
				       double obTemp,   int tuo,
				       double obPres,  int  puo,
				       double obDens,   int duo,
				       double baseTemp, int tub,
				       double basePres, int pub,
                                       int *errorCode){
  double ctpl = 0;
  *errorCode = ApiCalculation_VBAExportCalculation(commodity,
						   alpha60, au,
                           obTemp, tub, 
						   baseTemp, tub, 
						   obPres, puo,
						   basePres,  puo,
						   obDens, duo,
						   baseTemp, tub,
						   basePres, pub);
  if( *errorCode != 0) return ctpl;
  ctpl = ApiQuantity_getValue((ApiQuantity*)calculationRequest->ctplObToBase,
			      ApiUnit_Expansion_DIMLESS(),
			      FALSE,
			      errorCode);
  if( *errorCode != 0) ctpl = 0;
  return ctpl;   
};

/**
* Get the ctplAltToBase parameter
*
* @param commodity the commodity for which to do the calculation. If set to -1,
*                  it is assumed that an alpha60 value is supplied
* @param alpha60 the value for  alpha60  
* @param au the unit for alpha60. If API_UNIT_NOT_DEFINED, we set 
*        alpha60 to NULL and assume that a  commodity is supplied.
*
* @param obTemp the temperature at which we observed the data
* @param tuo the unit to use for the observed temperature
* @param obPres the  pressure at which the data was observed
* @param puo the unit to use for the observed pressure
* @param obDens the observed density
* @param duo the unit to use for the observed density
*
* @param baseTemp the base temperature to which to calculate the 
*        expansion factor
* @param tub  the unit for the the base temperature
*
* @param basePres the base pressure to which to calculate the 
*        expansion factors
* @param pub  the unit for the the base pressure
*
* @param errorCode the errorCode to reprot
*
* @return 0 if calculation was successful, an errorCode otherwise
*/
double _ApiCalculation_getCtplAltToBase(int commodity,
				       double alpha60,  int au,
				       double obTemp,   int tuo,
				       double obPres,  int  puo,
				       double obDens,   int duo,
				       double baseTemp, int tub,
				       double basePres, int pub,
					   double *ctl, double *cpl,
                                       int *errorCode){
  double ctpl = 0;
  *errorCode = ApiCalculation_VBAExportCalculation(commodity,
						   alpha60, au,
                           obTemp, tub, 
						   baseTemp, tub, 
						   obPres, puo,
						   basePres,  puo,
						   obDens, duo,
						   baseTemp, tub,
						   basePres, pub);
  if( *errorCode != 0) return ctpl;
  ctpl = ApiQuantity_getValue((ApiQuantity*)calculationRequest->ctplObToBase,
			      ApiUnit_Expansion_DIMLESS(),
			      FALSE,
			      errorCode);
				  
   *ctl = ApiQuantity_getValue((ApiQuantity*)calculationRequest->ctlObToBase,
			      ApiUnit_Expansion_DIMLESS(),
			      FALSE,
			      errorCode);

   *cpl = ApiQuantity_getValue((ApiQuantity*)calculationRequest->cplObToBase,
			      ApiUnit_Expansion_DIMLESS(),
			      FALSE,
			      errorCode);
				  
  if( *errorCode != 0) ctpl = 0;
  return ctpl;   
};


/*--------------------------------------------------------------
*   11.1.6.2 type calculations - get ctpl
*--------------------------------------------------------------*/

/*-------- Density in API ------------------------------------------*/

/**
* Calculate the ctpl factor for transformation from 60F, 0psi to
* the desired alternate conditions for Crude Oil. Density is in API
*/
double ctplA11162API(double density, double temp, double pres, int *errorCode){
  return  ApiCalculation_getCtplAltToBase( API_CRUDE_OIL_NAME,
					   0,  API_UNIT_NOT_DEFINED,
					   temp,  API_TEMPERATURE_F_UNIT,
					   pres, API_PRESSURE_PSI_UNIT,
					   density, API_DENSITY_API,
					   API_BASE_TEMP, API_TEMPERATURE_F_UNIT,
					   API_BASE_PRES, API_PRESSURE_PSI_UNIT,
					   errorCode);
}

double _ctplA11162API(double density, double temp, double pres, double *ctl, double *cpl, int *errorCode){
  return  _ApiCalculation_getCtplAltToBase( API_CRUDE_OIL_NAME,
					   0,  API_UNIT_NOT_DEFINED,
					   temp,  API_TEMPERATURE_F_UNIT,
					   pres, API_PRESSURE_PSI_UNIT,
					   density, API_DENSITY_API,
					   API_BASE_TEMP, API_TEMPERATURE_F_UNIT,
					   API_BASE_PRES, API_PRESSURE_PSI_UNIT,
					   ctl, cpl,
					   errorCode);
}

/**
* Calculate the ctpl factor for transformation from 60F, 0psi to
* the desired alternate conditions for Refined Products. Density is in API
*/
double ctplB11162API(double density, double temp, double pres, int *errorCode){
  double ctpl;
  ctpl =  ApiCalculation_getCtplAltToBase( API_REFINED_PRODUCTS_NAME,
					   0,  API_UNIT_NOT_DEFINED,
					   temp,  API_TEMPERATURE_F_UNIT,
					   pres, API_PRESSURE_PSI_UNIT,
					   density, API_DENSITY_API,
					   API_BASE_TEMP, API_TEMPERATURE_F_UNIT,
					   API_BASE_PRES, API_PRESSURE_PSI_UNIT,
					   errorCode);
  return ctpl;
}

double _ctplB11162API(double density, double temp, double pres, double *ctl, double *cpl, int *errorCode){
  double ctpl;
  ctpl =  _ApiCalculation_getCtplAltToBase( API_REFINED_PRODUCTS_NAME,
					   0,  API_UNIT_NOT_DEFINED,
					   temp,  API_TEMPERATURE_F_UNIT,
					   pres, API_PRESSURE_PSI_UNIT,
					   density, API_DENSITY_API,
					   API_BASE_TEMP, API_TEMPERATURE_F_UNIT,
					   API_BASE_PRES, API_PRESSURE_PSI_UNIT,
					   ctl, cpl,
					   errorCode);
  return ctpl;
}


/**
* Calculate the ctpl factor for transformation from 60F, 0psi to
* the desired alternate conditions for Lubrication Oil. Density is in API
*/
double ctplD11162API(double density, double temp, double pres, int *errorCode){
  return  ApiCalculation_getCtplAltToBase( API_LUBRICATING_OIL_NAME,
					   0,  API_UNIT_NOT_DEFINED,
					   temp,  API_TEMPERATURE_F_UNIT,
					   pres, API_PRESSURE_PSI_UNIT,
					   density, API_DENSITY_API,
					   API_BASE_TEMP, API_TEMPERATURE_F_UNIT,
					   API_BASE_PRES, API_PRESSURE_PSI_UNIT,
					   errorCode);
}

double _ctplD11162API(double density, double temp, double pres, double *ctl, double *cpl, int *errorCode){
  return  _ApiCalculation_getCtplAltToBase( API_LUBRICATING_OIL_NAME,
					   0,  API_UNIT_NOT_DEFINED,
					   temp,  API_TEMPERATURE_F_UNIT,
					   pres, API_PRESSURE_PSI_UNIT,
					   density, API_DENSITY_API,
					   API_BASE_TEMP, API_TEMPERATURE_F_UNIT,
					   API_BASE_PRES, API_PRESSURE_PSI_UNIT,
					   ctl, cpl,
					   errorCode);
}

/**
* Calculate the ctpl factor for transformation from 60F, 0psi to
* the desired alternate conditions for supplied alpha values.
* Alpha60 is in 1/F  Density is in API
*/
double ctplC11162API(double alpha60, double density, double temp, double pres, int *errorCode){ 
  return  ApiCalculation_getCtplAltToBase( API_COMMODITY_NOT_GIVEN,
					   alpha60, API_ALPHA60_REV_F_UNIT,
					   temp,  API_TEMPERATURE_F_UNIT,
					   pres, API_PRESSURE_PSI_UNIT,
					   density, API_DENSITY_API,
					   API_BASE_TEMP, API_TEMPERATURE_F_UNIT,
					   API_BASE_PRES, API_PRESSURE_PSI_UNIT,
					   errorCode);
}

double _ctplC11162API(double alpha60, double density, double temp, double pres, double *ctl, double *cpl, int *errorCode){ 
  return  _ApiCalculation_getCtplAltToBase( API_COMMODITY_NOT_GIVEN,
					   alpha60, API_ALPHA60_REV_F_UNIT,
					   temp,  API_TEMPERATURE_F_UNIT,
					   pres, API_PRESSURE_PSI_UNIT,
					   density, API_DENSITY_API,
					   API_BASE_TEMP, API_TEMPERATURE_F_UNIT,
					   API_BASE_PRES, API_PRESSURE_PSI_UNIT,
					   ctl, cpl,
					   errorCode);
}

/*-------- Density in Rel. Dens ------------------------------------------*/

/**
* Calculate the ctpl factor for transformation from 60F, 0psi to
* the desired alternate conditions for Crude Oil. Density is in Rel. Dens
*/
double ctplA11162RelDens(double density, double temp, double pres, int *errorCode){
  return  ApiCalculation_getCtplAltToBase( API_CRUDE_OIL_NAME,
					   0,  API_UNIT_NOT_DEFINED,
					   temp,  API_TEMPERATURE_F_UNIT,
					   pres, API_PRESSURE_PSI_UNIT,
					   density, API_DENSITY_RELATIVE,
					   API_BASE_TEMP, API_TEMPERATURE_F_UNIT,
					   API_BASE_PRES, API_PRESSURE_PSI_UNIT,
					   errorCode);
}

double _ctplA11162RelDens(double density, double temp, double pres, double *ctl, double *cpl, int *errorCode){
  return  _ApiCalculation_getCtplAltToBase( API_CRUDE_OIL_NAME,
					   0,  API_UNIT_NOT_DEFINED,
					   temp,  API_TEMPERATURE_F_UNIT,
					   pres, API_PRESSURE_PSI_UNIT,
					   density, API_DENSITY_RELATIVE,
					   API_BASE_TEMP, API_TEMPERATURE_F_UNIT,
					   API_BASE_PRES, API_PRESSURE_PSI_UNIT,
					   ctl, cpl,
					   errorCode);
}

/**
* Calculate the ctpl factor for transformation from 60F, 0psi to
* the desired alternate conditions for Refined Products. Density is in 
* Rel. Dens
*/
double ctplB11162RelDens(double density, double temp, double pres, int *errorCode){
  return  ApiCalculation_getCtplAltToBase( API_REFINED_PRODUCTS_NAME,
					   0,  API_UNIT_NOT_DEFINED,
					   temp,  API_TEMPERATURE_F_UNIT,
					   pres, API_PRESSURE_PSI_UNIT,
					   density, API_DENSITY_RELATIVE,
					   API_BASE_TEMP, API_TEMPERATURE_F_UNIT,
					   API_BASE_PRES, API_PRESSURE_PSI_UNIT,
					   errorCode);
}

double _ctplB11162RelDens(double density, double temp, double pres, double *ctl, double *cpl, int *errorCode){
  return  _ApiCalculation_getCtplAltToBase( API_REFINED_PRODUCTS_NAME,
					   0,  API_UNIT_NOT_DEFINED,
					   temp,  API_TEMPERATURE_F_UNIT,
					   pres, API_PRESSURE_PSI_UNIT,
					   density, API_DENSITY_RELATIVE,
					   API_BASE_TEMP, API_TEMPERATURE_F_UNIT,
					   API_BASE_PRES, API_PRESSURE_PSI_UNIT,
					   ctl, cpl,
					   errorCode);
}

/**
* Calculate the ctpl factor for transformation from 60F, 0psi to
* the desired alternate conditions for Lubrication Oil. Density is in Rel. Dens.
*/
double ctplD11162RelDens(double density, double temp, double pres, int *errorCode){
  return  ApiCalculation_getCtplAltToBase( API_LUBRICATING_OIL_NAME,
					   0,  API_UNIT_NOT_DEFINED,
					   temp,  API_TEMPERATURE_F_UNIT,
					   pres, API_PRESSURE_PSI_UNIT,
					   density, API_DENSITY_RELATIVE,
					   API_BASE_TEMP, API_TEMPERATURE_F_UNIT,
					   API_BASE_PRES, API_PRESSURE_PSI_UNIT,
					   errorCode);
}

double _ctplD11162RelDens(double density, double temp, double pres, double *ctl, double *cpl, int *errorCode){
  return  _ApiCalculation_getCtplAltToBase( API_LUBRICATING_OIL_NAME,
					   0,  API_UNIT_NOT_DEFINED,
					   temp,  API_TEMPERATURE_F_UNIT,
					   pres, API_PRESSURE_PSI_UNIT,
					   density, API_DENSITY_RELATIVE,
					   API_BASE_TEMP, API_TEMPERATURE_F_UNIT,
					   API_BASE_PRES, API_PRESSURE_PSI_UNIT,
					   ctl, cpl,
					   errorCode);
}

/**
* Calculate the ctpl factor for transformation from 60F, 0psi to
* the desired alternate conditions for supplied alpha values.
* Alpha60 is in 1/F  Density is in Rel. Dens.
*/
double ctplC11162RelDens(double alpha60, double density, double temp, double pres, int *errorCode){
  return  ApiCalculation_getCtplAltToBase( API_COMMODITY_NOT_GIVEN,
					   alpha60, API_ALPHA60_REV_F_UNIT,
					   temp,  API_TEMPERATURE_F_UNIT,
					   pres, API_PRESSURE_PSI_UNIT,
					   density, API_DENSITY_RELATIVE,
					   API_BASE_TEMP, API_TEMPERATURE_F_UNIT,
					   API_BASE_PRES, API_PRESSURE_PSI_UNIT,
					   errorCode);
}

double _ctplC11162RelDens(double alpha60, double density, double temp, double pres, double *ctl, double *cpl, int *errorCode){
  return  _ApiCalculation_getCtplAltToBase( API_COMMODITY_NOT_GIVEN,
					   alpha60, API_ALPHA60_REV_F_UNIT,
					   temp,  API_TEMPERATURE_F_UNIT,
					   pres, API_PRESSURE_PSI_UNIT,
					   density, API_DENSITY_RELATIVE,
					   API_BASE_TEMP, API_TEMPERATURE_F_UNIT,
					   API_BASE_PRES, API_PRESSURE_PSI_UNIT,
					   ctl, cpl,
					   errorCode);
}

/*--------------------------------------------------------------
*   11.1.7.2 type calculations - get ctpl
*--------------------------------------------------------------*/

/**
* Calculate the ctpl factor for transformation from 15C and 0kPa
* the desired alternate conditions for Crude Oil. 
*/
double ctplA11172bar(double density, double temp, double pres, double baseTemp, int *errorCode){ 
  return  ApiCalculation_getCtplAltToBase( API_CRUDE_OIL_NAME,
					   0,  API_UNIT_NOT_DEFINED,
					   temp,  API_TEMPERATURE_C_UNIT,
					   pres, API_PRESSURE_BAR_UNIT,
					   density, API_DENSITY_KGM3,
					   baseTemp, API_TEMPERATURE_C_UNIT,
					   0, API_PRESSURE_BAR_UNIT,
					   errorCode);
}

double _ctplA11172bar(double density, double temp, double pres, double baseTemp, double *ctl, double *cpl, int *errorCode){ 
  return  _ApiCalculation_getCtplAltToBase( API_CRUDE_OIL_NAME,
					   0,  API_UNIT_NOT_DEFINED,
					   temp,  API_TEMPERATURE_C_UNIT,
					   pres, API_PRESSURE_BAR_UNIT,
					   density, API_DENSITY_KGM3,
					   baseTemp, API_TEMPERATURE_C_UNIT,
					   0, API_PRESSURE_BAR_UNIT,
					   ctl, cpl,
					   errorCode);
}

/**
* Calculate the ctpl factor for transformation from 15C and 0kPa
* the desired alternate conditions for Refined products
*/
double ctplB11172bar(double density, double temp, double pres, double baseTemp, int *errorCode){ 
  return  ApiCalculation_getCtplAltToBase(API_REFINED_PRODUCTS_NAME,
					  0,  API_UNIT_NOT_DEFINED,
					  temp,  API_TEMPERATURE_C_UNIT,
					  pres, API_PRESSURE_BAR_UNIT,
					  density, API_DENSITY_KGM3,
					   baseTemp, API_TEMPERATURE_C_UNIT,
					   0, API_PRESSURE_BAR_UNIT,
					   errorCode);
}

double _ctplB11172bar(double density, double temp, double pres, double baseTemp, double *ctl, double *cpl, int *errorCode){ 
  return  _ApiCalculation_getCtplAltToBase(API_REFINED_PRODUCTS_NAME,
					  0,  API_UNIT_NOT_DEFINED,
					  temp,  API_TEMPERATURE_C_UNIT,
					  pres, API_PRESSURE_BAR_UNIT,
					  density, API_DENSITY_KGM3,
					   baseTemp, API_TEMPERATURE_C_UNIT,
					   0, API_PRESSURE_BAR_UNIT,
					   ctl, cpl,
					   errorCode);
}

/**
* Calculate the ctpl factor for transformation from 15C and 0kPa
* the desired alternate conditions for Lubrication oil.
*/
double ctplD11172bar(double density, double temp, double pres, double baseTemp, int *errorCode){ 
  return  ApiCalculation_getCtplAltToBase(API_LUBRICATING_OIL_NAME,
					  0,  API_UNIT_NOT_DEFINED,
					  temp,  API_TEMPERATURE_C_UNIT,
					  pres, API_PRESSURE_BAR_UNIT,
					  density, API_DENSITY_KGM3,
					   baseTemp, API_TEMPERATURE_C_UNIT,
					   0, API_PRESSURE_BAR_UNIT,
					   errorCode);
}

double _ctplD11172bar(double density, double temp, double pres, double baseTemp, double *ctl, double *cpl, int *errorCode){ 
  return  _ApiCalculation_getCtplAltToBase(API_LUBRICATING_OIL_NAME,
					  0,  API_UNIT_NOT_DEFINED,
					  temp,  API_TEMPERATURE_C_UNIT,
					  pres, API_PRESSURE_BAR_UNIT,
					  density, API_DENSITY_KGM3,
					   baseTemp, API_TEMPERATURE_C_UNIT,
					   0, API_PRESSURE_BAR_UNIT,
					   ctl, cpl,
					   errorCode);
}

/**
* Calculate the ctpl factor for transformation from 15C and 0kPa
* the desired alternate conditions for alpha60 supplied
*/
double ctplC11172bar(double alpha60, double density, double temp, double pres, double baseTemp, int *errorCode){ 
  return  ApiCalculation_getCtplAltToBase(API_COMMODITY_NOT_GIVEN,
					  alpha60, API_ALPHA60_REV_C_UNIT,
					  temp,  API_TEMPERATURE_C_UNIT,
					  pres, API_PRESSURE_BAR_UNIT,
					  density, API_DENSITY_KGM3,
					  baseTemp, API_TEMPERATURE_C_UNIT,
					  0, API_PRESSURE_BAR_UNIT,
					  errorCode);
}

double _ctplC11172bar(double alpha60, double density, double temp, double pres, double baseTemp, double *ctl, double *cpl, int *errorCode){ 
  return  _ApiCalculation_getCtplAltToBase(API_COMMODITY_NOT_GIVEN,
					  alpha60, API_ALPHA60_REV_C_UNIT,
					  temp,  API_TEMPERATURE_C_UNIT,
					  pres, API_PRESSURE_BAR_UNIT,
					  density, API_DENSITY_KGM3,
					  baseTemp, API_TEMPERATURE_C_UNIT,
					  0, API_PRESSURE_BAR_UNIT,
					  ctl, cpl,
					  errorCode);
}

/**
* Calculate the ctpl factor for transformation from 15C and 0kPa
* the desired alternate conditions for Crude Oil. 
*/
double ctplA11172kPa(double density, double temp, double pres, double baseTemp, int *errorCode){ 
  return  ApiCalculation_getCtplAltToBase( API_CRUDE_OIL_NAME,
					   0,  API_UNIT_NOT_DEFINED,
					   temp,  API_TEMPERATURE_C_UNIT,
					   pres, API_PRESSURE_KPA_UNIT,
					   density, API_DENSITY_KGM3,
					   baseTemp, API_TEMPERATURE_C_UNIT,
					   0, API_PRESSURE_KPA_UNIT,
					   errorCode);
}

double _ctplA11172kPa(double density, double temp, double pres, double baseTemp, double *ctl, double *cpl, int *errorCode){ 
  return _ApiCalculation_getCtplAltToBase( API_CRUDE_OIL_NAME,
					   0,  API_UNIT_NOT_DEFINED,
					   temp,  API_TEMPERATURE_C_UNIT,
					   pres, API_PRESSURE_KPA_UNIT,
					   density, API_DENSITY_KGM3,
					   baseTemp, API_TEMPERATURE_C_UNIT,
					   0, API_PRESSURE_KPA_UNIT,
					   ctl, cpl,
					   errorCode);
}

/**
* Calculate the ctpl factor for transformation from 15C and 0kPa
* the desired alternate conditions for Refined products
*/
double ctplB11172kPa(double density, double temp, double pres, double baseTemp, int *errorCode){ 
  return  ApiCalculation_getCtplAltToBase(API_REFINED_PRODUCTS_NAME,
					  0,  API_UNIT_NOT_DEFINED,
					  temp,  API_TEMPERATURE_C_UNIT,
					  pres, API_PRESSURE_KPA_UNIT,
					  density, API_DENSITY_KGM3,
					  baseTemp, API_TEMPERATURE_C_UNIT,
					   0, API_PRESSURE_KPA_UNIT,
					   errorCode);
}

double _ctplB11172kPa(double density, double temp, double pres, double baseTemp, double *ctl, double *cpl, int *errorCode){ 
  return  _ApiCalculation_getCtplAltToBase(API_REFINED_PRODUCTS_NAME,
					  0,  API_UNIT_NOT_DEFINED,
					  temp,  API_TEMPERATURE_C_UNIT,
					  pres, API_PRESSURE_KPA_UNIT,
					  density, API_DENSITY_KGM3,
					  baseTemp, API_TEMPERATURE_C_UNIT,
					   0, API_PRESSURE_KPA_UNIT,
					   ctl, cpl,
					   errorCode);
}

/**
* Calculate the ctpl factor for transformation from 15C and 0kPa
* the desired alternate conditions for Lubrication oil.
*/
double ctplD11172kPa(double density, double temp, double pres, double baseTemp, int *errorCode){ 
  return  ApiCalculation_getCtplAltToBase(API_LUBRICATING_OIL_NAME,
					  0,  API_UNIT_NOT_DEFINED,
					  temp,  API_TEMPERATURE_C_UNIT,
					  pres, API_PRESSURE_KPA_UNIT,
					  density, API_DENSITY_KGM3,
					   baseTemp, API_TEMPERATURE_C_UNIT,
					   0, API_PRESSURE_KPA_UNIT,
					   errorCode);
}

double _ctplD11172kPa(double density, double temp, double pres, double baseTemp, double *ctl, double *cpl, int *errorCode){ 
  return  _ApiCalculation_getCtplAltToBase(API_LUBRICATING_OIL_NAME,
					  0,  API_UNIT_NOT_DEFINED,
					  temp,  API_TEMPERATURE_C_UNIT,
					  pres, API_PRESSURE_KPA_UNIT,
					  density, API_DENSITY_KGM3,
					   baseTemp, API_TEMPERATURE_C_UNIT,
					   0, API_PRESSURE_KPA_UNIT,
					   ctl, cpl,
					   errorCode);
}

/**
* Calculate the ctpl factor for transformation from 15C and 0kPa
* the desired alternate conditions for alpha60 supplied
*/
double ctplC11172kPa(double alpha60, double density, double temp, double pres, double baseTemp, int *errorCode){ 
  return  ApiCalculation_getCtplAltToBase(API_COMMODITY_NOT_GIVEN,
					  alpha60, API_ALPHA60_REV_C_UNIT,
					  temp,  API_TEMPERATURE_C_UNIT,
					  pres, API_PRESSURE_KPA_UNIT,
					  density, API_DENSITY_KGM3,

					   baseTemp, API_TEMPERATURE_C_UNIT,
					   0, API_PRESSURE_KPA_UNIT,
					   errorCode);
}

double _ctplC11172kPa(double alpha60, double density, double temp, double pres, double baseTemp, double *ctl, double *cpl, int *errorCode){ 
  return  _ApiCalculation_getCtplAltToBase(API_COMMODITY_NOT_GIVEN,
					  alpha60, API_ALPHA60_REV_C_UNIT,
					  temp,  API_TEMPERATURE_C_UNIT,
					  pres, API_PRESSURE_KPA_UNIT,
					  density, API_DENSITY_KGM3,
					   baseTemp, API_TEMPERATURE_C_UNIT,
					   0, API_PRESSURE_KPA_UNIT,
					   ctl, cpl,
					   errorCode);
}

/**
* Get the alternate density in the unit given for the base density
*
* @param commodity the commodity for which to do the calculation. If set to -1,
*                  it is assumed that an alpha60 value is supplied
* @param alpha60 the value for  alpha60  
* @param au the unit for alpha60. If API_UNIT_NOT_DEFINED, we set 
*        alpha60 to NULL and assume that a  commodity is supplied.
*
* @param altTemp the alternate temperature to use
* @param tua the unit to use for the alternate temperature
* @param altPres the alternate pressure to use
* @param pua the unit to use for the alternate pressure
* @param obDens the observed density
* @param duo the unit to use for the observed density
*
* @param baseTemp the base temperature to which to calculate the 
*        expansion factor
* @param tub  the unit for the the base temperature
*
* @param basePres the base pressure to which to calculate the 
*        expansion factors
* @param pub  the unit for the the base pressure
*
* @param errorCode the errorCode to reprot
*
* @return 0 if calculation was successful, an errorCode otherwise
*/
double ApiCalculation_getAltDens(int commodity,
				  double alpha60,  int au,
				  double altTemp,  int tua,
				  double altPres,  int  pua,
				  double obDens,   int duo,
				  double baseTemp, int tub,
				  double basePres, int pub,
				  int *errorCode){
  double dens = 0;
  ApiUnit *unit = NULL;

  *errorCode = ApiCalculation_VBAExportCalculation(commodity,
						   alpha60, au,
                                                   baseTemp, tub,
						   altTemp, tua, 
						   basePres, pub,
						   altPres,  pua,
						   obDens, duo,

						   baseTemp, tub,
						   basePres, pub);
  if( *errorCode != 0) return dens;

  /** Set the unit for the base density */
  switch(duo){
     case API_DENSITY_API:
        unit = ApiUnit_Density_API();
        break;
     case API_DENSITY_RELATIVE:
        unit = ApiUnit_Density_RELATIVE();
        break;
     case API_DENSITY_KGM3:
        unit = ApiUnit_Density_KGM3();
        break;
     default:
        unit = NULL;
  }

  /** Get the value */
  dens = ApiQuantity_getValue((ApiQuantity*)calculationRequest->altDens,
			      unit,
			      FALSE,
			      errorCode);
  if( *errorCode != 0) dens = 0;
  return dens;   
};

double _ApiCalculation_getAltDens(int commodity,
				  double alpha60,  int au,
				  double altTemp,  int tua,
				  double altPres,  int  pua,
				  double obDens,   int duo,
				  double baseTemp, int tub,
				  double basePres, int pub,
				  double *ctl, double *cpl,
				  int *errorCode){
  double dens = 0;
  ApiUnit *unit = NULL;

  *errorCode = ApiCalculation_VBAExportCalculation(commodity,
						   alpha60, au,
                                                   baseTemp, tub,
						   altTemp, tua, 
						   basePres, pub,
						   altPres,  pua,
						   obDens, duo,

						   baseTemp, tub,
						   basePres, pub);
  if( *errorCode != 0) return dens;

  /** Set the unit for the base density */
  switch(duo){
     case API_DENSITY_API:
        unit = ApiUnit_Density_API();
        break;
     case API_DENSITY_RELATIVE:
        unit = ApiUnit_Density_RELATIVE();
        break;
     case API_DENSITY_KGM3:
        unit = ApiUnit_Density_KGM3();
        break;
     default:
        unit = NULL;
  }

  /** Get the value */
  dens = ApiQuantity_getValue((ApiQuantity*)calculationRequest->altDens,
			      unit,
			      FALSE,
			      errorCode);
				  
  *ctl = ApiQuantity_getValue((ApiQuantity*)calculationRequest->ctlBaseToAlt,
			      ApiUnit_Expansion_DIMLESS(),
			      FALSE,
			      errorCode);
				  
  *cpl = ApiQuantity_getValue((ApiQuantity*)calculationRequest->cplBaseToAlt,
			      ApiUnit_Expansion_DIMLESS(),
			      FALSE,
			      errorCode);
				  
  if( *errorCode != 0) dens = 0;
  return dens;   
};

/*--------------------------------------------------------------
*   11.1.6.1 type calculations - get dens
*--------------------------------------------------------------*/

/*-------- Density in API ------------------------------------------*/

/**
* Calculate the dens factor for transformation from 60F, 0psi to
* the desired alternate conditions for Crude Oil. Density is in API
*/
double densA11161API(double density, double temp, double pres, int *errorCode){
  return  ApiCalculation_getAltDens( API_CRUDE_OIL_NAME,
					   0,  API_UNIT_NOT_DEFINED,
					   temp,  API_TEMPERATURE_F_UNIT,
					   pres, API_PRESSURE_PSI_UNIT,
					   density, API_DENSITY_API,
					   API_BASE_TEMP, API_TEMPERATURE_F_UNIT,
					   API_BASE_PRES, API_PRESSURE_PSI_UNIT,
					   errorCode);
}

double _densA11161API(double density, double temp, double pres, double *ctl, double *cpl, int *errorCode){
  return  _ApiCalculation_getAltDens( API_CRUDE_OIL_NAME,
					   0,  API_UNIT_NOT_DEFINED,
					   temp,  API_TEMPERATURE_F_UNIT,
					   pres, API_PRESSURE_PSI_UNIT,
					   density, API_DENSITY_API,
					   API_BASE_TEMP, API_TEMPERATURE_F_UNIT,
					   API_BASE_PRES, API_PRESSURE_PSI_UNIT,
					   ctl, cpl,
					   errorCode);
}

/**
* Calculate the dens factor for transformation from 60F, 0psi to
* the desired alternate conditions for Refined Products. Density is in API
*/
double densB11161API(double density, double temp, double pres, int *errorCode){
  double dens;
  dens =  ApiCalculation_getAltDens( API_REFINED_PRODUCTS_NAME,
					   0,  API_UNIT_NOT_DEFINED,
					   temp,  API_TEMPERATURE_F_UNIT,
					   pres, API_PRESSURE_PSI_UNIT,
					   density, API_DENSITY_API,
					   API_BASE_TEMP, API_TEMPERATURE_F_UNIT,
					   API_BASE_PRES, API_PRESSURE_PSI_UNIT,
					   errorCode);
  return dens;
}

double _densB11161API(double density, double temp, double pres, double *ctl, double *cpl, int *errorCode){
  double dens;
  dens =  _ApiCalculation_getAltDens( API_REFINED_PRODUCTS_NAME,
					   0,  API_UNIT_NOT_DEFINED,
					   temp,  API_TEMPERATURE_F_UNIT,
					   pres, API_PRESSURE_PSI_UNIT,
					   density, API_DENSITY_API,
					   API_BASE_TEMP, API_TEMPERATURE_F_UNIT,
					   API_BASE_PRES, API_PRESSURE_PSI_UNIT,
					   ctl, cpl,
					   errorCode);
  return dens;
}


/**
* Calculate the dens factor for transformation from 60F, 0psi to
* the desired alternate conditions for Lubrication Oil. Density is in API
*/
double densD11161API(double density, double temp, double pres, int *errorCode){
  return  ApiCalculation_getAltDens( API_LUBRICATING_OIL_NAME,
					   0,  API_UNIT_NOT_DEFINED,
					   temp,  API_TEMPERATURE_F_UNIT,
					   pres, API_PRESSURE_PSI_UNIT,
					   density, API_DENSITY_API,
					   API_BASE_TEMP, API_TEMPERATURE_F_UNIT,
					   API_BASE_PRES, API_PRESSURE_PSI_UNIT,
					   errorCode);
}

double _densD11161API(double density, double temp, double pres, double *ctl, double *cpl, int *errorCode){
  return  _ApiCalculation_getAltDens( API_LUBRICATING_OIL_NAME,
					   0,  API_UNIT_NOT_DEFINED,
					   temp,  API_TEMPERATURE_F_UNIT,
					   pres, API_PRESSURE_PSI_UNIT,
					   density, API_DENSITY_API,
					   API_BASE_TEMP, API_TEMPERATURE_F_UNIT,
					   API_BASE_PRES, API_PRESSURE_PSI_UNIT,
					   ctl, cpl,
					   errorCode);
}

/**
* Calculate the dens factor for transformation from 60F, 0psi to
* the desired alternate conditions for supplied alpha values.
* Alpha60 is in 1/F  Density is in API
*/
double densC11161API(double alpha60, double density, double temp, double pres, int *errorCode){ 
  return  ApiCalculation_getAltDens( API_COMMODITY_NOT_GIVEN,
					   alpha60, API_ALPHA60_REV_F_UNIT,
					   temp,  API_TEMPERATURE_F_UNIT,
					   pres, API_PRESSURE_PSI_UNIT,
					   density, API_DENSITY_API,
					   API_BASE_TEMP, API_TEMPERATURE_F_UNIT,
					   API_BASE_PRES, API_PRESSURE_PSI_UNIT,
					   errorCode);
}

double _densC11161API(double alpha60, double density, double temp, double pres, double *ctl, double *cpl, int *errorCode){ 
  return  _ApiCalculation_getAltDens( API_COMMODITY_NOT_GIVEN,
					   alpha60, API_ALPHA60_REV_F_UNIT,
					   temp,  API_TEMPERATURE_F_UNIT,
					   pres, API_PRESSURE_PSI_UNIT,
					   density, API_DENSITY_API,
					   API_BASE_TEMP, API_TEMPERATURE_F_UNIT,
					   API_BASE_PRES, API_PRESSURE_PSI_UNIT,
					   ctl, cpl,
					   errorCode);
}

/*-------- Density in Rel. Dens ------------------------------------------*/

/**
* Calculate the dens factor for transformation from 60F, 0psi to
* the desired alternate conditions for Crude Oil. Density is in Rel. Dens
*/
double densA11161RelDens(double density, double temp, double pres, int *errorCode){
  return  ApiCalculation_getAltDens( API_CRUDE_OIL_NAME,
					   0,  API_UNIT_NOT_DEFINED,
					   temp,  API_TEMPERATURE_F_UNIT,
					   pres, API_PRESSURE_PSI_UNIT,
					   density, API_DENSITY_RELATIVE,
					   API_BASE_TEMP, API_TEMPERATURE_F_UNIT,
					   API_BASE_PRES, API_PRESSURE_PSI_UNIT,
					   errorCode);
}

double _densA11161RelDens(double density, double temp, double pres, double *ctl, double *cpl, int *errorCode){
  return  _ApiCalculation_getAltDens( API_CRUDE_OIL_NAME,
					   0,  API_UNIT_NOT_DEFINED,
					   temp,  API_TEMPERATURE_F_UNIT,
					   pres, API_PRESSURE_PSI_UNIT,
					   density, API_DENSITY_RELATIVE,
					   API_BASE_TEMP, API_TEMPERATURE_F_UNIT,
					   API_BASE_PRES, API_PRESSURE_PSI_UNIT,
					   ctl, cpl,
					   errorCode);
}

/**
* Calculate the dens factor for transformation from 60F, 0psi to
* the desired alternate conditions for Refined Products. Density is in 
* Rel. Dens
*/
double densB11161RelDens(double density, double temp, double pres, int *errorCode){
  return  ApiCalculation_getAltDens( API_REFINED_PRODUCTS_NAME,
					   0,  API_UNIT_NOT_DEFINED,
					   temp,  API_TEMPERATURE_F_UNIT,
					   pres, API_PRESSURE_PSI_UNIT,
					   density, API_DENSITY_RELATIVE,
					   API_BASE_TEMP, API_TEMPERATURE_F_UNIT,
					   API_BASE_PRES, API_PRESSURE_PSI_UNIT,
					   errorCode);
}


double _densB11161RelDens(double density, double temp, double pres, double *ctl, double *cpl, int *errorCode){
  return  _ApiCalculation_getAltDens( API_REFINED_PRODUCTS_NAME,
					   0,  API_UNIT_NOT_DEFINED,
					   temp,  API_TEMPERATURE_F_UNIT,
					   pres, API_PRESSURE_PSI_UNIT,
					   density, API_DENSITY_RELATIVE,
					   API_BASE_TEMP, API_TEMPERATURE_F_UNIT,
					   API_BASE_PRES, API_PRESSURE_PSI_UNIT,
					   ctl, cpl,
					   errorCode);
}

/**
* Calculate the dens factor for transformation from 60F, 0psi to
* the desired alternate conditions for Lubrication Oil. Density is in Rel. Dens.
*/
double densD11161RelDens(double density, double temp, double pres, int *errorCode){
  return  ApiCalculation_getAltDens( API_LUBRICATING_OIL_NAME,
					   0,  API_UNIT_NOT_DEFINED,
					   temp,  API_TEMPERATURE_F_UNIT,
					   pres, API_PRESSURE_PSI_UNIT,
					   density, API_DENSITY_RELATIVE,
					   API_BASE_TEMP, API_TEMPERATURE_F_UNIT,
					   API_BASE_PRES, API_PRESSURE_PSI_UNIT,
					   errorCode);
}

double _densD11161RelDens(double density, double temp, double pres, double *ctl, double *cpl, int *errorCode){
  return  _ApiCalculation_getAltDens( API_LUBRICATING_OIL_NAME,
					   0,  API_UNIT_NOT_DEFINED,
					   temp,  API_TEMPERATURE_F_UNIT,
					   pres, API_PRESSURE_PSI_UNIT,
					   density, API_DENSITY_RELATIVE,
					   API_BASE_TEMP, API_TEMPERATURE_F_UNIT,
					   API_BASE_PRES, API_PRESSURE_PSI_UNIT,
					   ctl, cpl,
					   errorCode);
}

/**
* Calculate the dens factor for transformation from 60F, 0psi to
* the desired alternate conditions for supplied alpha values.
* Alpha60 is in 1/F  Density is in Rel. Dens.
*/
double densC11161RelDens(double alpha60, double density, double temp, double pres, int *errorCode){ 
  return  ApiCalculation_getAltDens( API_COMMODITY_NOT_GIVEN,
					   alpha60, API_ALPHA60_REV_F_UNIT,
					   temp,  API_TEMPERATURE_F_UNIT,
					   pres, API_PRESSURE_PSI_UNIT,
					   density, API_DENSITY_RELATIVE,
					   API_BASE_TEMP, API_TEMPERATURE_F_UNIT,
					   API_BASE_PRES, API_PRESSURE_PSI_UNIT,
					   errorCode);
}

double _densC11161RelDens(double alpha60, double density, double temp, double pres, double *ctl, double *cpl, int *errorCode){ 
  return  _ApiCalculation_getAltDens( API_COMMODITY_NOT_GIVEN,
					   alpha60, API_ALPHA60_REV_F_UNIT,
					   temp,  API_TEMPERATURE_F_UNIT,
					   pres, API_PRESSURE_PSI_UNIT,
					   density, API_DENSITY_RELATIVE,
					   API_BASE_TEMP, API_TEMPERATURE_F_UNIT,
					   API_BASE_PRES, API_PRESSURE_PSI_UNIT,
					   ctl, cpl, 
					   errorCode);
}

/*--------------------------------------------------------------
*   11.1.7.1 type calculations - get dens
*--------------------------------------------------------------*/

/**
* Calculate the dens factor for transformation from 15C and 0kPa
* the desired alternate conditions for Crude Oil. 
*/
double densA11171bar(double density, double temp, double pres, double baseTemp, int *errorCode){ 
  return  ApiCalculation_getAltDens( API_CRUDE_OIL_NAME,
					   0,  API_UNIT_NOT_DEFINED,
					   temp,  API_TEMPERATURE_C_UNIT,
					   pres, API_PRESSURE_BAR_UNIT,
					   density, API_DENSITY_KGM3,
					   baseTemp, API_TEMPERATURE_C_UNIT,
					   0, API_PRESSURE_BAR_UNIT,
					   errorCode);
}

double _densA11171bar(double density, double temp, double pres, double baseTemp, double *ctl, double *cpl, int *errorCode){ 
  return  _ApiCalculation_getAltDens( API_CRUDE_OIL_NAME,
					   0,  API_UNIT_NOT_DEFINED,
					   temp,  API_TEMPERATURE_C_UNIT,
					   pres, API_PRESSURE_BAR_UNIT,
					   density, API_DENSITY_KGM3,
					   baseTemp, API_TEMPERATURE_C_UNIT,
					   0, API_PRESSURE_BAR_UNIT,
					   ctl, cpl,
					   errorCode);
}

/**
* Calculate the dens factor for transformation from 15C and 0kPa
* the desired alternate conditions for Refined products
*/
double densB11171bar(double density, double temp, double pres, double baseTemp, int *errorCode){ 
  return  ApiCalculation_getAltDens(API_REFINED_PRODUCTS_NAME,
					  0,  API_UNIT_NOT_DEFINED,
					  temp,  API_TEMPERATURE_C_UNIT,
					  pres, API_PRESSURE_BAR_UNIT,
					  density, API_DENSITY_KGM3,
					   baseTemp, API_TEMPERATURE_C_UNIT,
					   0, API_PRESSURE_BAR_UNIT,
					   errorCode);
}

double _densB11171bar(double density, double temp, double pres, double baseTemp, double *ctl, double *cpl, int *errorCode){ 
  return  _ApiCalculation_getAltDens(API_REFINED_PRODUCTS_NAME,
					  0,  API_UNIT_NOT_DEFINED,
					  temp,  API_TEMPERATURE_C_UNIT,
					  pres, API_PRESSURE_BAR_UNIT,
					  density, API_DENSITY_KGM3,
					   baseTemp, API_TEMPERATURE_C_UNIT,
					   0, API_PRESSURE_BAR_UNIT,
					   ctl, cpl,
					   errorCode);
}

/**
* Calculate the dens factor for transformation from 15C and 0kPa
* the desired alternate conditions for Lubrication oil.
*/
double densD11171bar(double density, double temp, double pres, double baseTemp, int *errorCode){ 
  return  ApiCalculation_getAltDens(API_LUBRICATING_OIL_NAME,
					  0,  API_UNIT_NOT_DEFINED,
					  temp,  API_TEMPERATURE_C_UNIT,
					  pres, API_PRESSURE_BAR_UNIT,
					  density, API_DENSITY_KGM3,
					   baseTemp, API_TEMPERATURE_C_UNIT,
					   0, API_PRESSURE_BAR_UNIT,
					   errorCode);
}

double _densD11171bar(double density, double temp, double pres, double baseTemp, double *ctl, double *cpl, int *errorCode){ 
  return  _ApiCalculation_getAltDens(API_LUBRICATING_OIL_NAME,
					  0,  API_UNIT_NOT_DEFINED,
					  temp,  API_TEMPERATURE_C_UNIT,
					  pres, API_PRESSURE_BAR_UNIT,
					  density, API_DENSITY_KGM3,
					   baseTemp, API_TEMPERATURE_C_UNIT,
					   0, API_PRESSURE_BAR_UNIT,
					   ctl, cpl,
					   errorCode);
}

/**
* Calculate the dens factor for transformation from 15C and 0kPa
* the desired alternate conditions for alpha60 supplied
*/
double densC11171bar(double alpha60, double density, double temp, double pres, double baseTemp, int *errorCode){ 
  return  ApiCalculation_getAltDens(API_COMMODITY_NOT_GIVEN,
					  alpha60, API_ALPHA60_REV_C_UNIT,
					  temp,  API_TEMPERATURE_C_UNIT,
					  pres, API_PRESSURE_BAR_UNIT,
					  density, API_DENSITY_KGM3,
					  baseTemp, API_TEMPERATURE_C_UNIT,
					  0, API_PRESSURE_BAR_UNIT,
					  errorCode);
}

double _densC11171bar(double alpha60, double density, double temp, double pres, double baseTemp, double *ctl, double *cpl, int *errorCode){ 
  return  _ApiCalculation_getAltDens(API_COMMODITY_NOT_GIVEN,
					  alpha60, API_ALPHA60_REV_C_UNIT,
					  temp,  API_TEMPERATURE_C_UNIT,
					  pres, API_PRESSURE_BAR_UNIT,
					  density, API_DENSITY_KGM3,
					  baseTemp, API_TEMPERATURE_C_UNIT,
					  0, API_PRESSURE_BAR_UNIT,
					  ctl, cpl,
					  errorCode);
}

/**
* Calculate the dens factor for transformation from 15C and 0kPa
* the desired alternate conditions for Crude Oil. 
*/
double densA11171kPa(double density, double temp, double pres, double baseTemp, int *errorCode){ 
  return  ApiCalculation_getAltDens( API_CRUDE_OIL_NAME,
					   0,  API_UNIT_NOT_DEFINED,
					   temp,  API_TEMPERATURE_C_UNIT,
					   pres, API_PRESSURE_KPA_UNIT,
					   density, API_DENSITY_KGM3,
					   baseTemp, API_TEMPERATURE_C_UNIT,
					   0, API_PRESSURE_KPA_UNIT,
					   errorCode);
}

double _densA11171kPa(double density, double temp, double pres, double baseTemp, double *ctl, double *cpl, int *errorCode){ 
  return  _ApiCalculation_getAltDens( API_CRUDE_OIL_NAME,
					   0,  API_UNIT_NOT_DEFINED,
					   temp,  API_TEMPERATURE_C_UNIT,
					   pres, API_PRESSURE_KPA_UNIT,
					   density, API_DENSITY_KGM3,
					   baseTemp, API_TEMPERATURE_C_UNIT,
					   0, API_PRESSURE_KPA_UNIT,
					   ctl, cpl,
					   errorCode);
}

/**
* Calculate the dens factor for transformation from 15C and 0kPa
* the desired alternate conditions for Refined products
*/
double densB11171kPa(double density, double temp, double pres, double baseTemp, int *errorCode){ 
  return  ApiCalculation_getAltDens(API_REFINED_PRODUCTS_NAME,
					  0,  API_UNIT_NOT_DEFINED,
					  temp,  API_TEMPERATURE_C_UNIT,
					  pres, API_PRESSURE_KPA_UNIT,
					  density, API_DENSITY_KGM3,
					  baseTemp, API_TEMPERATURE_C_UNIT,
					   0, API_PRESSURE_KPA_UNIT,
					   errorCode);
}

double _densB11171kPa(double density, double temp, double pres, double baseTemp, double *ctl, double *cpl, int *errorCode){ 
  return  _ApiCalculation_getAltDens(API_REFINED_PRODUCTS_NAME,
					  0,  API_UNIT_NOT_DEFINED,
					  temp,  API_TEMPERATURE_C_UNIT,
					  pres, API_PRESSURE_KPA_UNIT,
					  density, API_DENSITY_KGM3,
					  baseTemp, API_TEMPERATURE_C_UNIT,
					   0, API_PRESSURE_KPA_UNIT,
					   ctl, cpl, 
					   errorCode);
}

/**
* Calculate the dens factor for transformation from 15C and 0kPa
* the desired alternate conditions for Lubrication oil.
*/
double densD11171kPa(double density, double temp, double pres, double baseTemp, int *errorCode){ 
  return  ApiCalculation_getAltDens(API_LUBRICATING_OIL_NAME,
					  0,  API_UNIT_NOT_DEFINED,
					  temp,  API_TEMPERATURE_C_UNIT,
					  pres, API_PRESSURE_KPA_UNIT,
					  density, API_DENSITY_KGM3,
					   baseTemp, API_TEMPERATURE_C_UNIT,
					   0, API_PRESSURE_KPA_UNIT,
					   errorCode);
}

double _densD11171kPa(double density, double temp, double pres, double baseTemp, double *ctl, double *cpl, int *errorCode){ 
  return  _ApiCalculation_getAltDens(API_LUBRICATING_OIL_NAME,
					  0,  API_UNIT_NOT_DEFINED,
					  temp,  API_TEMPERATURE_C_UNIT,
					  pres, API_PRESSURE_KPA_UNIT,
					  density, API_DENSITY_KGM3,
					   baseTemp, API_TEMPERATURE_C_UNIT,
					   0, API_PRESSURE_KPA_UNIT,
					   ctl, cpl, 
					   errorCode);
}

/**
* Calculate the dens factor for transformation from 15C and 0kPa
* the desired alternate conditions for alpha60 supplied
*/
double densC11171kPa(double alpha60, double density, double temp, double pres, double baseTemp, int *errorCode){ 
  return  ApiCalculation_getAltDens(API_COMMODITY_NOT_GIVEN,
					  alpha60, API_ALPHA60_REV_C_UNIT,
					  temp,  API_TEMPERATURE_C_UNIT,
					  pres, API_PRESSURE_KPA_UNIT,
					  density, API_DENSITY_KGM3,
					   baseTemp, API_TEMPERATURE_C_UNIT,
					   0, API_PRESSURE_KPA_UNIT,
					   errorCode);
}

double _densC11171kPa(double alpha60, double density, double temp, double pres, double baseTemp, double *ctl, double *cpl, int *errorCode){ 
  return  _ApiCalculation_getAltDens(API_COMMODITY_NOT_GIVEN,
					  alpha60, API_ALPHA60_REV_C_UNIT,
					  temp,  API_TEMPERATURE_C_UNIT,
					  pres, API_PRESSURE_KPA_UNIT,
					  density, API_DENSITY_KGM3,
					   baseTemp, API_TEMPERATURE_C_UNIT,
					   0, API_PRESSURE_KPA_UNIT,
					   ctl, cpl, 
					   errorCode);
}

/**
* Get the base density
*
* @param commodity the commodity for which to do the calculation. If set to -1,
*                  it is assumed that an alpha60 value is supplied
* @param alpha60 the value for  alpha60  
* @param au the unit for alpha60. If API_UNIT_NOT_DEFINED, we set 
*        alpha60 to NULL and assume that a  commodity is supplied.
*
* @param obTemp the temperature at which we observed the data
* @param tuo the unit to use for the observed temperature
* @param obPres the  pressure at which the data was observed
* @param puo the unit to use for the observed pressure
* @param obDens the observed density
* @param duo the unit to use for the observed density
*
* @param baseTemp the base temperature to which to calculate the 
*        expansion factor
* @param tub  the unit for the the base temperature
*
* @param basePres the base pressure to which to calculate the 
*        expansion factors
* @param pub  the unit for the the base pressure
*
* @param errorCode the errorCode to reprot
*
* @return 0 if calculation was successful, an errorCode otherwise
*/
double ApiCalculation_getBaseDens(int commodity,
				  double alpha60,  int au,
				  double obTemp,   int tuo,
				  double obPres,  int  puo,
				  double obDens,   int duo,
				  double baseTemp, int tub,
				  double basePres, int pub,
				  int *errorCode){
  double dens = 0;
  ApiUnit * unit = NULL;

  *errorCode = ApiCalculation_VBAExportCalculation(commodity,
						   alpha60, au,
                                                   obTemp, tub, 
						   baseTemp, tub, 
						   obPres, puo,
						   basePres,  puo,
						   obDens, duo,

						   baseTemp, tub,
						   basePres, pub);
  if( *errorCode != 0) return dens;


  /** Set the unit for the base density */
  switch(duo){
     case API_DENSITY_API:
        unit = ApiUnit_Density_API();
        break;
     case API_DENSITY_RELATIVE:
        unit = ApiUnit_Density_RELATIVE();
        break;
     case API_DENSITY_KGM3:
        unit = ApiUnit_Density_KGM3();
        break;
     default:
        unit = NULL;
  }

  dens = ApiQuantity_getValue((ApiQuantity*)calculationRequest->baseDens,
			      unit,
			      FALSE,
			      errorCode);
  if( *errorCode != 0) dens = 0;
  return dens;   
};

double _ApiCalculation_getBaseDens(int commodity,
				  double alpha60,  int au,
				  double obTemp,   int tuo,
				  double obPres,  int  puo,
				  double obDens,   int duo,
				  double baseTemp, int tub,
				  double basePres, int pub,
				  double *ctl, double *cpl,
				  int *errorCode){
  double dens = 0;
  ApiUnit * unit = NULL;

  *errorCode = ApiCalculation_VBAExportCalculation(commodity,
						   alpha60, au,
                           obTemp, tub, 
						   baseTemp, tub, 
						   obPres, puo,
						   basePres,  puo,
						   obDens, duo,
						   baseTemp, tub,
						   basePres, pub);
  if( *errorCode != 0) return dens;


  /** Set the unit for the base density */
  switch(duo){
     case API_DENSITY_API:
        unit = ApiUnit_Density_API();
        break;
     case API_DENSITY_RELATIVE:
        unit = ApiUnit_Density_RELATIVE();
        break;
     case API_DENSITY_KGM3:
        unit = ApiUnit_Density_KGM3();
        break;
     default:
        unit = NULL;
  }	
  dens = ApiQuantity_getValue((ApiQuantity*)calculationRequest->baseDens,
			      unit,
			      FALSE,
			      errorCode);
				  
  *ctl = ApiQuantity_getValue((ApiQuantity*)calculationRequest->ctlObToBase,
			      ApiUnit_Expansion_DIMLESS(), 
			      FALSE,
			      errorCode);
				  
  *cpl = ApiQuantity_getValue((ApiQuantity*)calculationRequest->cplObToBase,
			      ApiUnit_Expansion_DIMLESS(),
			      FALSE,
			      errorCode);
				  
  if( *errorCode != 0) dens = 0;
  return dens;   
};


/*--------------------------------------------------------------
*   11.1.6.2 type calculations - get dens
*--------------------------------------------------------------*/

/*-------- Density in API ------------------------------------------*/

/**
* Calculate the dens factor for transformation from 60F, 0psi to
* the desired alternate conditions for Crude Oil. Density is in API
*/
double densA11162API(double density, double temp, double pres, int *errorCode){
  return  ApiCalculation_getBaseDens( API_CRUDE_OIL_NAME,
					   0,  API_UNIT_NOT_DEFINED,
					   temp,  API_TEMPERATURE_F_UNIT,
					   pres, API_PRESSURE_PSI_UNIT,
					   density, API_DENSITY_API,
					   API_BASE_TEMP, API_TEMPERATURE_F_UNIT,
					   API_BASE_PRES, API_PRESSURE_PSI_UNIT,
					   errorCode);
}

double _densA11162API(double density, double temp, double pres, double *ctl, double *cpl, int *errorCode){
  return  _ApiCalculation_getBaseDens( API_CRUDE_OIL_NAME,
					   0,  API_UNIT_NOT_DEFINED,
					   temp,  API_TEMPERATURE_F_UNIT,
					   pres, API_PRESSURE_PSI_UNIT,
					   density, API_DENSITY_API,
					   API_BASE_TEMP, API_TEMPERATURE_F_UNIT,
					   API_BASE_PRES, API_PRESSURE_PSI_UNIT,
					   ctl, cpl,
					   errorCode);
}

/**
* Calculate the dens factor for transformation from 60F, 0psi to
* the desired alternate conditions for Refined Products. Density is in API
*/
double densB11162API(double density, double temp, double pres, int *errorCode){
  double dens;
  dens =  ApiCalculation_getBaseDens( API_REFINED_PRODUCTS_NAME,
					   0,  API_UNIT_NOT_DEFINED,
					   temp,  API_TEMPERATURE_F_UNIT,
					   pres, API_PRESSURE_PSI_UNIT,
					   density, API_DENSITY_API,
					   API_BASE_TEMP, API_TEMPERATURE_F_UNIT,
					   API_BASE_PRES, API_PRESSURE_PSI_UNIT,
					   errorCode);
  return dens;
}

double _densB11162API(double density, double temp, double pres, double *ctl, double *cpl, int *errorCode){
  double dens;
  dens =  _ApiCalculation_getBaseDens( API_REFINED_PRODUCTS_NAME,
					   0,  API_UNIT_NOT_DEFINED,
					   temp,  API_TEMPERATURE_F_UNIT,
					   pres, API_PRESSURE_PSI_UNIT,
					   density, API_DENSITY_API,
					   API_BASE_TEMP, API_TEMPERATURE_F_UNIT,
					   API_BASE_PRES, API_PRESSURE_PSI_UNIT,
					   ctl, cpl,
					   errorCode);
  return dens;
}

/**
* Calculate the dens factor for transformation from 60F, 0psi to
* the desired alternate conditions for Lubrication Oil. Density is in API
*/
double densD11162API(double density, double temp, double pres, int *errorCode){
  return  ApiCalculation_getBaseDens( API_LUBRICATING_OIL_NAME,
					   0,  API_UNIT_NOT_DEFINED,
					   temp,  API_TEMPERATURE_F_UNIT,
					   pres, API_PRESSURE_PSI_UNIT,
					   density, API_DENSITY_API,
					   API_BASE_TEMP, API_TEMPERATURE_F_UNIT,
					   API_BASE_PRES, API_PRESSURE_PSI_UNIT,
					   errorCode);
}

double _densD11162API(double density, double temp, double pres, double *ctl, double *cpl, int *errorCode){
  return  _ApiCalculation_getBaseDens( API_LUBRICATING_OIL_NAME,
					   0,  API_UNIT_NOT_DEFINED,
					   temp,  API_TEMPERATURE_F_UNIT,
					   pres, API_PRESSURE_PSI_UNIT,
					   density, API_DENSITY_API,
					   API_BASE_TEMP, API_TEMPERATURE_F_UNIT,
					   API_BASE_PRES, API_PRESSURE_PSI_UNIT,
					   ctl, cpl,
					   errorCode);
}

/**
* Calculate the dens factor for transformation from 60F, 0psi to
* the desired alternate conditions for supplied alpha values.
* Alpha60 is in 1/F  Density is in API
*/
double densC11162API(double alpha60, double density, double temp,  double pres, int *errorCode){ 
  return  ApiCalculation_getBaseDens( API_COMMODITY_NOT_GIVEN,
					   alpha60, API_ALPHA60_REV_F_UNIT,
					   temp,  API_TEMPERATURE_F_UNIT,
					   pres, API_PRESSURE_PSI_UNIT,
					   density, API_DENSITY_API,
					   API_BASE_TEMP, API_TEMPERATURE_F_UNIT,
					   API_BASE_PRES, API_PRESSURE_PSI_UNIT,
					   errorCode);
}

double _densC11162API(double alpha60, double density, double temp,  double pres, double *ctl, double *cpl, int *errorCode){ 
  return  _ApiCalculation_getBaseDens( API_COMMODITY_NOT_GIVEN,
					   alpha60, API_ALPHA60_REV_F_UNIT,
					   temp,  API_TEMPERATURE_F_UNIT,
					   pres, API_PRESSURE_PSI_UNIT,
					   density, API_DENSITY_API,
					   API_BASE_TEMP, API_TEMPERATURE_F_UNIT,
					   API_BASE_PRES, API_PRESSURE_PSI_UNIT,
					   ctl, cpl,
					   errorCode);
}

/*-------- Density in Rel. Dens ------------------------------------------*/

/**
* Calculate the dens factor for transformation from 60F, 0psi to
* the desired alternate conditions for Crude Oil. Density is in Rel. Dens
*/
double densA11162RelDens(double density, double temp, double pres, int *errorCode){
  return  ApiCalculation_getBaseDens( API_CRUDE_OIL_NAME,
					   0,  API_UNIT_NOT_DEFINED,
					   temp,  API_TEMPERATURE_F_UNIT,
					   pres, API_PRESSURE_PSI_UNIT,
					   density, API_DENSITY_RELATIVE,
					   API_BASE_TEMP, API_TEMPERATURE_F_UNIT,
					   API_BASE_PRES, API_PRESSURE_PSI_UNIT,
					   errorCode);
}

double _densA11162RelDens(double density, double temp, double pres, double *ctl, double *cpl, int *errorCode){
  return  _ApiCalculation_getBaseDens( API_CRUDE_OIL_NAME,
					   0,  API_UNIT_NOT_DEFINED,
					   temp,  API_TEMPERATURE_F_UNIT,
					   pres, API_PRESSURE_PSI_UNIT,
					   density, API_DENSITY_RELATIVE,
					   API_BASE_TEMP, API_TEMPERATURE_F_UNIT,
					   API_BASE_PRES, API_PRESSURE_PSI_UNIT,
					   ctl, cpl, 
					   errorCode);
}

/**
* Calculate the dens factor for transformation from 60F, 0psi to
* the desired alternate conditions for Refined Products. Density is in 
* Rel. Dens
*/
double densB11162RelDens(double density, double temp, double pres, int *errorCode){
  return  ApiCalculation_getBaseDens( API_REFINED_PRODUCTS_NAME,
					   0,  API_UNIT_NOT_DEFINED,
					   temp,  API_TEMPERATURE_F_UNIT,
					   pres, API_PRESSURE_PSI_UNIT,
					   density, API_DENSITY_RELATIVE,
					   API_BASE_TEMP, API_TEMPERATURE_F_UNIT,
					   API_BASE_PRES, API_PRESSURE_PSI_UNIT,
					   errorCode);
}

double _densB11162RelDens(double density, double temp, double pres, double *ctl, double *cpl, int *errorCode){
  return  _ApiCalculation_getBaseDens( API_REFINED_PRODUCTS_NAME,
					   0,  API_UNIT_NOT_DEFINED,
					   temp,  API_TEMPERATURE_F_UNIT,
					   pres, API_PRESSURE_PSI_UNIT,
					   density, API_DENSITY_RELATIVE,
					   API_BASE_TEMP, API_TEMPERATURE_F_UNIT,
					   API_BASE_PRES, API_PRESSURE_PSI_UNIT,
					   ctl, cpl, 
					   errorCode);
}

/**
* Calculate the dens factor for transformation from 60F, 0psi to
* the desired alternate conditions for Lubrication Oil. Density is in Rel. Dens.
*/
double densD11162RelDens(double density, double temp, double pres, int *errorCode){
  return  ApiCalculation_getBaseDens( API_LUBRICATING_OIL_NAME,
					   0,  API_UNIT_NOT_DEFINED,
					   temp,  API_TEMPERATURE_F_UNIT,
					   pres, API_PRESSURE_PSI_UNIT,
					   density, API_DENSITY_RELATIVE,
					   API_BASE_TEMP, API_TEMPERATURE_F_UNIT,
					   API_BASE_PRES, API_PRESSURE_PSI_UNIT,
					   errorCode);
}

double _densD11162RelDens(double density, double temp, double pres, double *ctl, double *cpl, int *errorCode){
  return  _ApiCalculation_getBaseDens( API_LUBRICATING_OIL_NAME,
					   0,  API_UNIT_NOT_DEFINED,
					   temp,  API_TEMPERATURE_F_UNIT,
					   pres, API_PRESSURE_PSI_UNIT,
					   density, API_DENSITY_RELATIVE,
					   API_BASE_TEMP, API_TEMPERATURE_F_UNIT,
					   API_BASE_PRES, API_PRESSURE_PSI_UNIT,
					   ctl, cpl,
					   errorCode);
}

/**
* Calculate the dens factor for transformation from 60F, 0psi to
* the desired alternate conditions for supplied alpha values.
* Alpha60 is in 1/F  Density is in Rel. Dens.
*/
double densC11162RelDens(double alpha60, double density, double temp, double pres, int *errorCode){ 
  return  ApiCalculation_getBaseDens( API_COMMODITY_NOT_GIVEN,
					   alpha60, API_ALPHA60_REV_F_UNIT,
					   temp,  API_TEMPERATURE_F_UNIT,
					   pres, API_PRESSURE_PSI_UNIT,
					   density, API_DENSITY_RELATIVE,
					   API_BASE_TEMP, API_TEMPERATURE_F_UNIT,
					   API_BASE_PRES, API_PRESSURE_PSI_UNIT,
					   errorCode);
}

double _densC11162RelDens(double alpha60, double density, double temp, double pres, double *ctl, double *cpl, int *errorCode){ 
  return  _ApiCalculation_getBaseDens( API_COMMODITY_NOT_GIVEN,
					   alpha60, API_ALPHA60_REV_F_UNIT,
					   temp,  API_TEMPERATURE_F_UNIT,
					   pres, API_PRESSURE_PSI_UNIT,
					   density, API_DENSITY_RELATIVE,
					   API_BASE_TEMP, API_TEMPERATURE_F_UNIT,
					   API_BASE_PRES, API_PRESSURE_PSI_UNIT,
					   ctl, cpl, 
					   errorCode);
}

/*--------------------------------------------------------------
*   11.1.7.2 type calculations - get dens
*--------------------------------------------------------------*/

/**
* Calculate the dens factor for transformation from 15C and 0kPa
* the desired alternate conditions for Crude Oil. 
*/
double densA11172bar(double density, double temp, double pres, double baseTemp, int *errorCode){ 
  return  ApiCalculation_getBaseDens( API_CRUDE_OIL_NAME,
					   0,  API_UNIT_NOT_DEFINED,
					   temp,  API_TEMPERATURE_C_UNIT,
					   pres, API_PRESSURE_BAR_UNIT,
					   density, API_DENSITY_KGM3,
					   baseTemp, API_TEMPERATURE_C_UNIT,
					   0, API_PRESSURE_BAR_UNIT,
					   errorCode);
}

double _densA11172bar(double density, double temp, double pres, double baseTemp, double *ctl, double *cpl, int *errorCode){ 
  return  _ApiCalculation_getBaseDens( API_CRUDE_OIL_NAME,
					   0,  API_UNIT_NOT_DEFINED,
					   temp,  API_TEMPERATURE_C_UNIT,
					   pres, API_PRESSURE_BAR_UNIT,
					   density, API_DENSITY_KGM3,
					   baseTemp, API_TEMPERATURE_C_UNIT,
					   0, API_PRESSURE_BAR_UNIT,
					   ctl, cpl,
					   errorCode);
}

/**
* Calculate the dens factor for transformation from 15C and 0kPa
* the desired alternate conditions for Refined products
*/
double densB11172bar(double density, double temp, double pres, double baseTemp, int *errorCode){ 
  return  ApiCalculation_getBaseDens(API_REFINED_PRODUCTS_NAME,
					  0,  API_UNIT_NOT_DEFINED,
					  temp,  API_TEMPERATURE_C_UNIT,
					  pres, API_PRESSURE_BAR_UNIT,
					  density, API_DENSITY_KGM3,
					  baseTemp, API_TEMPERATURE_C_UNIT,
					  0, API_PRESSURE_BAR_UNIT,
					  errorCode);
}

double _densB11172bar(double density, double temp, double pres, double baseTemp, double *ctl, double *cpl, int *errorCode){ 
  return  _ApiCalculation_getBaseDens(API_REFINED_PRODUCTS_NAME,
					  0,  API_UNIT_NOT_DEFINED,
					  temp,  API_TEMPERATURE_C_UNIT,
					  pres, API_PRESSURE_BAR_UNIT,
					  density, API_DENSITY_KGM3,
					  baseTemp, API_TEMPERATURE_C_UNIT,
					  0, API_PRESSURE_BAR_UNIT,
					  ctl, cpl, 
					  errorCode);
}

/**
* Calculate the dens factor for transformation from 15C and 0kPa
* the desired alternate conditions for Lubrication oil.
*/
double densD11172bar(double density, double temp, double pres, double baseTemp, int *errorCode){ 
  return  ApiCalculation_getBaseDens(API_LUBRICATING_OIL_NAME,
					  0,  API_UNIT_NOT_DEFINED,
					  temp,  API_TEMPERATURE_C_UNIT,
					  pres, API_PRESSURE_BAR_UNIT,
					  density, API_DENSITY_KGM3,
					   baseTemp, API_TEMPERATURE_C_UNIT,
					   0, API_PRESSURE_BAR_UNIT,
					   errorCode);
}

double _densD11172bar(double density, double temp, double pres, double baseTemp, double *ctl, double *cpl, int *errorCode){ 
  return  _ApiCalculation_getBaseDens(API_LUBRICATING_OIL_NAME,
					  0,  API_UNIT_NOT_DEFINED,
					  temp,  API_TEMPERATURE_C_UNIT,
					  pres, API_PRESSURE_BAR_UNIT,
					  density, API_DENSITY_KGM3,
					   baseTemp, API_TEMPERATURE_C_UNIT,
					   0, API_PRESSURE_BAR_UNIT,
					   ctl, cpl,
					   errorCode);
}

/**
* Calculate the dens factor for transformation from 15C and 0kPa
* the desired alternate conditions for alpha60 supplied
*/
double densC11172bar(double alpha60, double density, double temp, double pres, double baseTemp, int *errorCode){ 
  return  ApiCalculation_getBaseDens(API_COMMODITY_NOT_GIVEN,
					  alpha60, API_ALPHA60_REV_C_UNIT,
					  temp,  API_TEMPERATURE_C_UNIT,
					  pres, API_PRESSURE_BAR_UNIT,
					  density, API_DENSITY_KGM3,
					  baseTemp, API_TEMPERATURE_C_UNIT,
					  0, API_PRESSURE_BAR_UNIT,
					  errorCode);
}

double _densC11172bar(double alpha60, double density, double temp, double pres, double baseTemp, double *ctl, double *cpl, int *errorCode){ 
  return  _ApiCalculation_getBaseDens(API_COMMODITY_NOT_GIVEN,
					  alpha60, API_ALPHA60_REV_C_UNIT,
					  temp,  API_TEMPERATURE_C_UNIT,
					  pres, API_PRESSURE_BAR_UNIT,
					  density, API_DENSITY_KGM3,
					  baseTemp, API_TEMPERATURE_C_UNIT,
					  0, API_PRESSURE_BAR_UNIT,
					  ctl, cpl, 
					  errorCode);
}

/**
* Calculate the dens factor for transformation from 15C and 0kPa
* the desired alternate conditions for Crude Oil. 
*/
double densA11172kPa(double density, double temp, double pres, double baseTemp, int *errorCode){ 
  return  ApiCalculation_getBaseDens( API_CRUDE_OIL_NAME,
					   0,  API_UNIT_NOT_DEFINED,
					   temp,  API_TEMPERATURE_C_UNIT,
					   pres, API_PRESSURE_KPA_UNIT,
					   density, API_DENSITY_KGM3,
					   baseTemp, API_TEMPERATURE_C_UNIT,
					   0, API_PRESSURE_KPA_UNIT,
					   errorCode);
}

double _densA11172kPa(double density, double temp, double pres, double baseTemp, double *ctl, double *cpl, int *errorCode){ 
  return  _ApiCalculation_getBaseDens( API_CRUDE_OIL_NAME,
					   0,  API_UNIT_NOT_DEFINED,
					   temp,  API_TEMPERATURE_C_UNIT,
					   pres, API_PRESSURE_KPA_UNIT,
					   density, API_DENSITY_KGM3,
					   baseTemp, API_TEMPERATURE_C_UNIT,
					   0, API_PRESSURE_KPA_UNIT,
					   ctl, cpl,
					   errorCode);
}

/**
* Calculate the dens factor for transformation from 15C and 0kPa
* the desired alternate conditions for Refined products
*/
double densB11172kPa(double density, double temp, double pres, double baseTemp, int *errorCode){
  return  ApiCalculation_getBaseDens(API_REFINED_PRODUCTS_NAME,
					  0,  API_UNIT_NOT_DEFINED,
					  temp,  API_TEMPERATURE_C_UNIT,
					  pres, API_PRESSURE_KPA_UNIT,
					  density, API_DENSITY_KGM3,
					  baseTemp, API_TEMPERATURE_C_UNIT,
					  0, API_PRESSURE_KPA_UNIT,
					  errorCode);
}

double _densB11172kPa(double density, double temp, double pres, double baseTemp, double *ctl, double *cpl, int *errorCode){
  return  _ApiCalculation_getBaseDens(API_REFINED_PRODUCTS_NAME,
					  0,  API_UNIT_NOT_DEFINED,
					  temp,  API_TEMPERATURE_C_UNIT,
					  pres, API_PRESSURE_KPA_UNIT,
					  density, API_DENSITY_KGM3,
					  baseTemp, API_TEMPERATURE_C_UNIT,
					  0, API_PRESSURE_KPA_UNIT,
					  ctl, cpl,
					  errorCode);
}

/**
* Calculate the dens factor for transformation from 15C and 0kPa
* the desired alternate conditions for Lubrication oil.
*/
double densD11172kPa(double density, double temp, double pres, double baseTemp, int *errorCode){
  return  ApiCalculation_getBaseDens(API_LUBRICATING_OIL_NAME,
					  0,  API_UNIT_NOT_DEFINED,
					  temp,  API_TEMPERATURE_C_UNIT,
					  pres, API_PRESSURE_KPA_UNIT,
					  density, API_DENSITY_KGM3,
					   baseTemp, API_TEMPERATURE_C_UNIT,
					   0, API_PRESSURE_KPA_UNIT,
					   errorCode);
}

double _densD11172kPa(double density, double temp, double pres, double baseTemp, double *ctl, double *cpl, int *errorCode){
  return  _ApiCalculation_getBaseDens(API_LUBRICATING_OIL_NAME,
					  0,  API_UNIT_NOT_DEFINED,
					  temp,  API_TEMPERATURE_C_UNIT,
					  pres, API_PRESSURE_KPA_UNIT,
					  density, API_DENSITY_KGM3,
					   baseTemp, API_TEMPERATURE_C_UNIT,
					   0, API_PRESSURE_KPA_UNIT,
					   ctl, cpl, 
					   errorCode);
}

/**
* Calculate the dens factor for transformation from 15C and 0kPa
* the desired alternate conditions for alpha60 supplied
*/
double densC11172kPa(double alpha60, double density, double temp, double pres, double baseTemp, int *errorCode){
  return  ApiCalculation_getBaseDens(API_COMMODITY_NOT_GIVEN,
					  alpha60, API_ALPHA60_REV_C_UNIT,
					  temp,  API_TEMPERATURE_C_UNIT,
					  pres, API_PRESSURE_KPA_UNIT,
					  density, API_DENSITY_KGM3,
					   baseTemp, API_TEMPERATURE_C_UNIT,
					   0, API_PRESSURE_KPA_UNIT,
					   errorCode);
}

double _densC11172kPa(double alpha60, double density, double temp, double pres, double baseTemp, double *ctl, double *cpl, int *errorCode){
  return  _ApiCalculation_getBaseDens(API_COMMODITY_NOT_GIVEN,
					  alpha60, API_ALPHA60_REV_C_UNIT,
					  temp,  API_TEMPERATURE_C_UNIT,
					  pres, API_PRESSURE_KPA_UNIT,
					  density, API_DENSITY_KGM3,
					   baseTemp, API_TEMPERATURE_C_UNIT,
					   0, API_PRESSURE_KPA_UNIT,
					   ctl, cpl, 
					   errorCode);
}

/**
* Get the scaled compensation factor  in indicated unit for
* transformation from base to alternate conditions
*
* @param commodity the commodity for which to do the calculation. If set to -1,
*                  it is assumed that an alpha60 value is supplied
* @param alpha60 the value for  alpha60  
* @param au the unit for alpha60. If API_UNIT_NOT_DEFINED, we set 
*        alpha60 to NULL and assume that a  commodity is supplied.
*
* @param altTemp the alternate temperature to use
* @param tua the unit to use for the alternate temperature
* @param altPres the alternate pressure to use
* @param pua the unit to use for the alternate pressure
* @param obDens the observed density
* @param duo the unit to use for the observed density
*
* @param baseTemp the base temperature to which to calculate the 
*        expansion factor
* @param tub  the unit for the the base temperature
*
* @param basePres the base pressure to which to calculate the 
*        expansion factors
* @param pub  the unit for the the base pressure
*
* @param fpu the unit for the scaled compensation factor
*
* @param errorCode the errorCode to reprot
*
* @return 0 if calculation was successful, an errorCode otherwise
*/

double ApiCalculation_getScaledCompAlt(int commodity,
				       double alpha60,  int au,
				       double altTemp,  int tua,
				       double altPres,  int  pua,
				       double obDens,   int duo,
				       double baseTemp, int tub,
				       double basePres, int pub,
                       ApiUnit *fpu,
				       int *errorCode){
  double fp = 0;

  *errorCode = ApiCalculation_VBAExportCalculation(commodity,
						   alpha60, au,
                           baseTemp, tub,
						   altTemp, tua, 
						   basePres, pub,
						   altPres,  pua,
						   obDens, duo,
						   baseTemp, tub,
						   basePres, pub);
  if( *errorCode != 0) return fp;

  fp = ApiQuantity_getValue((ApiQuantity*)calculationRequest->scaledCompAlt,
			      fpu,
			      FALSE,
			      errorCode);
  if( *errorCode != 0) fp = 0;
  return fp;   
};

double _ApiCalculation_getScaledCompAlt(int commodity,
				       double alpha60,  int au,
				       double altTemp,  int tua,
				       double altPres,  int  pua,
				       double obDens,   int duo,
				       double baseTemp, int tub,
				       double basePres, int pub,
                       ApiUnit *fpu,
					   double *ctl, double *cpl,
				       int *errorCode){
  double fp = 0;

  *errorCode = ApiCalculation_VBAExportCalculation(commodity,
						   alpha60, au,
                           baseTemp, tub,
						   altTemp, tua, 
						   basePres, pub,
						   altPres,  pua,
						   obDens, duo,
						   baseTemp, tub,
						   basePres, pub);
  if( *errorCode != 0) return fp;

  fp = ApiQuantity_getValue((ApiQuantity*)calculationRequest->scaledCompAlt,
			      fpu,
			      FALSE,
			      errorCode);
				  
  *ctl = ApiQuantity_getValue((ApiQuantity*)calculationRequest->ctlBaseToAlt,
			      ApiUnit_Expansion_DIMLESS(), 
			      FALSE,
			      errorCode);
				  
  *cpl = ApiQuantity_getValue((ApiQuantity*)calculationRequest->cplBaseToAlt,
			  ApiUnit_Expansion_DIMLESS(),
			  FALSE,
			  errorCode);
  if( *errorCode != 0) fp = 0;
  return fp;   
};

/*--------------------------------------------------------------
*   11.1.6.1 type calculations - get scaled compensation factor
*--------------------------------------------------------------*/

/*-------- Density in API ------------------------------------------*/

/**
* Calculate the scaled comp. factor for transformation from 60F, 0psi to
* the desired alternate conditions for Crude Oil. Density is in API
*/
double fpA11161API(double density, double temp, double pres, int *errorCode){
  return  ApiCalculation_getScaledCompAlt( API_CRUDE_OIL_NAME,
					   0,  API_UNIT_NOT_DEFINED,
					   temp,  API_TEMPERATURE_F_UNIT,
					   pres, API_PRESSURE_PSI_UNIT,
					   density, API_DENSITY_API,
					   API_BASE_TEMP, API_TEMPERATURE_F_UNIT,
					   API_BASE_PRES, API_PRESSURE_PSI_UNIT,
					   ApiUnit_ScaledComp_REV_PSI(),
					   errorCode);
}

double _fpA11161API(double density, double temp, double pres, double *ctl, 
	double *cpl, int *errorCode){
  return  _ApiCalculation_getScaledCompAlt( API_CRUDE_OIL_NAME,
					   0,  API_UNIT_NOT_DEFINED,
					   temp,  API_TEMPERATURE_F_UNIT,
					   pres, API_PRESSURE_PSI_UNIT,
					   density, API_DENSITY_API,
					   API_BASE_TEMP, API_TEMPERATURE_F_UNIT,
					   API_BASE_PRES, API_PRESSURE_PSI_UNIT,
					   ApiUnit_ScaledComp_REV_PSI(),
					   ctl, cpl,
					   errorCode);
}


/**
* Calculate the dens factor for transformation from 60F, 0psi to
* the desired alternate conditions for Refined Products. Density is in API
*/
double fpB11161API(double density, double temp, double pres, int *errorCode){
  return  ApiCalculation_getScaledCompAlt( API_REFINED_PRODUCTS_NAME,
					   0,  API_UNIT_NOT_DEFINED,
					   temp,  API_TEMPERATURE_F_UNIT,
					   pres, API_PRESSURE_PSI_UNIT,
					   density, API_DENSITY_API,
					   API_BASE_TEMP, API_TEMPERATURE_F_UNIT,
					   API_BASE_PRES, API_PRESSURE_PSI_UNIT,
					   ApiUnit_ScaledComp_REV_PSI(),
					   errorCode);
}

double _fpB11161API(double density, double temp, double pres, 
	double *ctl, double *cpl, int *errorCode){
  return  _ApiCalculation_getScaledCompAlt( API_REFINED_PRODUCTS_NAME,
					   0,  API_UNIT_NOT_DEFINED,
					   temp,  API_TEMPERATURE_F_UNIT,
					   pres, API_PRESSURE_PSI_UNIT,
					   density, API_DENSITY_API,
					   API_BASE_TEMP, API_TEMPERATURE_F_UNIT,
					   API_BASE_PRES, API_PRESSURE_PSI_UNIT,
					   ApiUnit_ScaledComp_REV_PSI(),
					   ctl, cpl,
					   errorCode);
}


/**
* Calculate the dens factor for transformation from 60F, 0psi to
* the desired alternate conditions for Lubrication Oil. Density is in API
*/
double fpD11161API(double density, double temp, double pres, int *errorCode){
  return ApiCalculation_getScaledCompAlt( API_LUBRICATING_OIL_NAME,
					   0,  API_UNIT_NOT_DEFINED,
					   temp,  API_TEMPERATURE_F_UNIT,
					   pres, API_PRESSURE_PSI_UNIT,
					   density, API_DENSITY_API,
					   API_BASE_TEMP, API_TEMPERATURE_F_UNIT,
					   API_BASE_PRES, API_PRESSURE_PSI_UNIT,
					   ApiUnit_ScaledComp_REV_PSI(),
					   errorCode);
}

double _fpD11161API(double density, double temp, double pres, 
	double *ctl, double *cpl, int *errorCode){
  return _ApiCalculation_getScaledCompAlt( API_LUBRICATING_OIL_NAME,
					   0,  API_UNIT_NOT_DEFINED,
					   temp,  API_TEMPERATURE_F_UNIT,
					   pres, API_PRESSURE_PSI_UNIT,
					   density, API_DENSITY_API,
					   API_BASE_TEMP, API_TEMPERATURE_F_UNIT,
					   API_BASE_PRES, API_PRESSURE_PSI_UNIT,
					   ApiUnit_ScaledComp_REV_PSI(),
					   ctl, cpl,
					   errorCode);
}
/**
* Calculate the dens factor for transformation from 60F, 0psi to
* the desired alternate conditions for supplied alpha values.
* Alpha60 is in 1/F  Density is in API
*/
double fpC11161API(double alpha60, double density, double temp, double pres, int *errorCode){ 
  return ApiCalculation_getScaledCompAlt( API_COMMODITY_NOT_GIVEN,
					   alpha60, API_ALPHA60_REV_F_UNIT,
					   temp,  API_TEMPERATURE_F_UNIT,
					   pres, API_PRESSURE_PSI_UNIT,
					   density, API_DENSITY_API,

					   API_BASE_TEMP, API_TEMPERATURE_F_UNIT,
					   API_BASE_PRES, API_PRESSURE_PSI_UNIT,
					   ApiUnit_ScaledComp_REV_PSI(),
					   errorCode);
}

double _fpC11161API(double alpha60, double density, double temp, double pres, 
	double *ctl, double *cpl, int *errorCode){ 
  return _ApiCalculation_getScaledCompAlt( API_COMMODITY_NOT_GIVEN,
					   alpha60, API_ALPHA60_REV_F_UNIT,
					   temp,  API_TEMPERATURE_F_UNIT,
					   pres, API_PRESSURE_PSI_UNIT,
					   density, API_DENSITY_API,

					   API_BASE_TEMP, API_TEMPERATURE_F_UNIT,
					   API_BASE_PRES, API_PRESSURE_PSI_UNIT,
					   ApiUnit_ScaledComp_REV_PSI(),
					   ctl, cpl,
					   errorCode);
}

/*-------- Density in Rel. Dens ------------------------------------------*/

/**
* Calculate the dens factor for transformation from 60F, 0psi to
* the desired alternate conditions for Crude Oil. Density is in Rel. Dens
*/
double fpA11161RelDens(double density, double temp, double pres, int *errorCode){
  return ApiCalculation_getScaledCompAlt( API_CRUDE_OIL_NAME,
					   0,  API_UNIT_NOT_DEFINED,
					   temp,  API_TEMPERATURE_F_UNIT,
					   pres, API_PRESSURE_PSI_UNIT,
					   density, API_DENSITY_RELATIVE,

					   API_BASE_TEMP, API_TEMPERATURE_F_UNIT,
					   API_BASE_PRES, API_PRESSURE_PSI_UNIT,
					   ApiUnit_ScaledComp_REV_PSI(),
					   errorCode);
}

double _fpA11161RelDens(double density, double temp, double pres, 
	double *ctl, double* cpl, int *errorCode){
  return _ApiCalculation_getScaledCompAlt( API_CRUDE_OIL_NAME,
					   0,  API_UNIT_NOT_DEFINED,
					   temp,  API_TEMPERATURE_F_UNIT,
					   pres, API_PRESSURE_PSI_UNIT,
					   density, API_DENSITY_RELATIVE,

					   API_BASE_TEMP, API_TEMPERATURE_F_UNIT,
					   API_BASE_PRES, API_PRESSURE_PSI_UNIT,
					   ApiUnit_ScaledComp_REV_PSI(),
					   ctl, cpl,
					   errorCode);
}

/**
* Calculate the dens factor for transformation from 60F, 0psi to
* the desired alternate conditions for Refined Products. Density is in 
* Rel. Dens
*/
double fpB11161RelDens(double density, double temp, double pres, int *errorCode){
  return ApiCalculation_getScaledCompAlt( API_REFINED_PRODUCTS_NAME,
					   0,  API_UNIT_NOT_DEFINED,
					   temp,  API_TEMPERATURE_F_UNIT,
					   pres, API_PRESSURE_PSI_UNIT,
					   density, API_DENSITY_RELATIVE,
					   API_BASE_TEMP, API_TEMPERATURE_F_UNIT,
					   API_BASE_PRES, API_PRESSURE_PSI_UNIT,
					   ApiUnit_ScaledComp_REV_PSI(),
					   errorCode);
}

double _fpB11161RelDens(double density, double temp, double pres, 
	double *ctl, double *cpl, int *errorCode){
  return _ApiCalculation_getScaledCompAlt( API_REFINED_PRODUCTS_NAME,
					   0,  API_UNIT_NOT_DEFINED,
					   temp,  API_TEMPERATURE_F_UNIT,
					   pres, API_PRESSURE_PSI_UNIT,
					   density, API_DENSITY_RELATIVE,
					   API_BASE_TEMP, API_TEMPERATURE_F_UNIT,
					   API_BASE_PRES, API_PRESSURE_PSI_UNIT,
					   ApiUnit_ScaledComp_REV_PSI(),
					   ctl, cpl,
					   errorCode);
}

/**
* Calculate the dens factor for transformation from 60F, 0psi to
* the desired alternate conditions for Lubrication Oil. Density is in Rel. Dens.
*/
double fpD11161RelDens(double density, double temp, double pres, int *errorCode){
  return ApiCalculation_getScaledCompAlt( API_LUBRICATING_OIL_NAME,
					   0,  API_UNIT_NOT_DEFINED,
					   temp,  API_TEMPERATURE_F_UNIT,
					   pres, API_PRESSURE_PSI_UNIT,
					   density, API_DENSITY_RELATIVE,

					   API_BASE_TEMP, API_TEMPERATURE_F_UNIT,
					   API_BASE_PRES, API_PRESSURE_PSI_UNIT,
					   ApiUnit_ScaledComp_REV_PSI(),
					   errorCode);
}

double _fpD11161RelDens(double density, double temp, double pres, 
	double *ctl, double *cpl, int *errorCode){
  return _ApiCalculation_getScaledCompAlt( API_LUBRICATING_OIL_NAME,
					   0,  API_UNIT_NOT_DEFINED,
					   temp,  API_TEMPERATURE_F_UNIT,
					   pres, API_PRESSURE_PSI_UNIT,
					   density, API_DENSITY_RELATIVE,
					   API_BASE_TEMP, API_TEMPERATURE_F_UNIT,
					   API_BASE_PRES, API_PRESSURE_PSI_UNIT,
					   ApiUnit_ScaledComp_REV_PSI(),
					   ctl, cpl,
					   errorCode);
}

/**
* Calculate the dens factor for transformation from 60F, 0psi to
* the desired alternate conditions for supplied alpha values.
* Alpha60 is in 1/F  Density is in Rel. Dens.
*/
double fpC11161RelDens(double alpha60,
			 double density, 
			 double temp,
			 double pres,
			 int *errorCode){
  return ApiCalculation_getScaledCompAlt( API_COMMODITY_NOT_GIVEN,
					   alpha60, API_ALPHA60_REV_F_UNIT,
					   temp,  API_TEMPERATURE_F_UNIT,
					   pres, API_PRESSURE_PSI_UNIT,
					   density, API_DENSITY_RELATIVE,

					   API_BASE_TEMP, API_TEMPERATURE_F_UNIT,
					   API_BASE_PRES, API_PRESSURE_PSI_UNIT,
					   ApiUnit_ScaledComp_REV_PSI(),
					   errorCode);
}

double _fpC11161RelDens(double alpha60,
			 double density, 
			 double temp,
			 double pres,
			 double *ctl, 
			 double *cpl,
			 int *errorCode){
  return _ApiCalculation_getScaledCompAlt( API_COMMODITY_NOT_GIVEN,
					   alpha60, API_ALPHA60_REV_F_UNIT,
					   temp,  API_TEMPERATURE_F_UNIT,
					   pres, API_PRESSURE_PSI_UNIT,
					   density, API_DENSITY_RELATIVE,
					   API_BASE_TEMP, API_TEMPERATURE_F_UNIT,
					   API_BASE_PRES, API_PRESSURE_PSI_UNIT,
					   ApiUnit_ScaledComp_REV_PSI(),
					   ctl, cpl,
					   errorCode);
}

/*--------------------------------------------------------------
*   11.1.7.1 type calculations - get dens
*--------------------------------------------------------------*/

/**
* Calculate the dens factor for transformation from 15C and 0kPa
* the desired alternate conditions for Crude Oil. 
*/
double fpA11171bar(double density, 
                     double temp, 
                     double pres,
                     double baseTemp, 
                     int *errorCode){
  return ApiCalculation_getScaledCompAlt( API_CRUDE_OIL_NAME,
					   0,  API_UNIT_NOT_DEFINED,
					   temp,  API_TEMPERATURE_C_UNIT,
					   pres, API_PRESSURE_BAR_UNIT,
					   density, API_DENSITY_KGM3,

					   baseTemp, API_TEMPERATURE_C_UNIT,
					   0, API_PRESSURE_BAR_UNIT,
					   ApiUnit_ScaledComp_REV_BAR(),
					   errorCode);
}

double _fpA11171bar(double density, 
                     double temp, 
                     double pres,
                     double baseTemp, 
					 double *ctl,
					 double *cpl,
                     int *errorCode){
  return _ApiCalculation_getScaledCompAlt( API_CRUDE_OIL_NAME,
					   0,  API_UNIT_NOT_DEFINED,
					   temp,  API_TEMPERATURE_C_UNIT,
					   pres, API_PRESSURE_BAR_UNIT,
					   density, API_DENSITY_KGM3,
					   baseTemp, API_TEMPERATURE_C_UNIT,
					   0, API_PRESSURE_BAR_UNIT,
					   ApiUnit_ScaledComp_REV_BAR(),
					   ctl, cpl,
					   errorCode);
}

/**
* Calculate the dens factor for transformation from 15C and 0kPa
* the desired alternate conditions for Refined products
*/
double fpB11171bar(double density, 
                     double temp,
                     double pres,
                     double baseTemp, 
                     int *errorCode){
  return ApiCalculation_getScaledCompAlt(API_REFINED_PRODUCTS_NAME,
					  0,  API_UNIT_NOT_DEFINED,
					  temp,  API_TEMPERATURE_C_UNIT,
					  pres, API_PRESSURE_BAR_UNIT,
					  density, API_DENSITY_KGM3,

					   baseTemp, API_TEMPERATURE_C_UNIT,
					   0, API_PRESSURE_BAR_UNIT,
					   ApiUnit_ScaledComp_REV_BAR(),
					   errorCode);
}

double _fpB11171bar(double density, 
                     double temp,
                     double pres,
                     double baseTemp, 
					 double *ctl,
					 double *cpl,
                     int *errorCode){
  return _ApiCalculation_getScaledCompAlt(API_REFINED_PRODUCTS_NAME,
					  0,  API_UNIT_NOT_DEFINED,
					  temp,  API_TEMPERATURE_C_UNIT,
					  pres, API_PRESSURE_BAR_UNIT,
					  density, API_DENSITY_KGM3,
					   baseTemp, API_TEMPERATURE_C_UNIT,
					   0, API_PRESSURE_BAR_UNIT,
					   ApiUnit_ScaledComp_REV_BAR(),
					   ctl, cpl,
					   errorCode);
}

/**
* Calculate the dens factor for transformation from 15C and 0kPa
* the desired alternate conditions for Lubrication oil.
*/
double fpD11171bar(double density,
                     double temp,
                     double pres,
                     double baseTemp, 
                     int *errorCode){
  return ApiCalculation_getScaledCompAlt(API_LUBRICATING_OIL_NAME,
					  0,  API_UNIT_NOT_DEFINED,
					  temp,  API_TEMPERATURE_C_UNIT,
					  pres, API_PRESSURE_BAR_UNIT,
					  density, API_DENSITY_KGM3,

					   baseTemp, API_TEMPERATURE_C_UNIT,
					   0, API_PRESSURE_BAR_UNIT,
					   ApiUnit_ScaledComp_REV_BAR(),
					   errorCode);
}

double _fpD11171bar(double density,
                     double temp,
                     double pres,
                     double baseTemp, 
					 double *ctl, 
					 double *cpl, 
                     int *errorCode){
  return _ApiCalculation_getScaledCompAlt(API_LUBRICATING_OIL_NAME,
					  0,  API_UNIT_NOT_DEFINED,
					  temp,  API_TEMPERATURE_C_UNIT,
					  pres, API_PRESSURE_BAR_UNIT,
					  density, API_DENSITY_KGM3,
					   baseTemp, API_TEMPERATURE_C_UNIT,
					   0, API_PRESSURE_BAR_UNIT,
					   ApiUnit_ScaledComp_REV_BAR(),
					   ctl, cpl,
					   errorCode);
}

/**
* Calculate the dens factor for transformation from 15C and 0kPa
* the desired alternate conditions for alpha60 supplied
*/
double fpC11171bar(double alpha60,
                     double density, 
                     double temp, 
                     double pres,
                     double baseTemp, 
                     int *errorCode){
  return ApiCalculation_getScaledCompAlt(API_COMMODITY_NOT_GIVEN,
					  alpha60, API_ALPHA60_REV_C_UNIT,
					  temp,  API_TEMPERATURE_C_UNIT,
					  pres, API_PRESSURE_BAR_UNIT,
					  density, API_DENSITY_KGM3,

					  baseTemp, API_TEMPERATURE_C_UNIT,
					  0, API_PRESSURE_BAR_UNIT,
					   ApiUnit_ScaledComp_REV_BAR(),
					  errorCode);
}

double _fpC11171bar(double alpha60,
                     double density, 
                     double temp, 
                     double pres,
                     double baseTemp, 
					 double *ctl,
					 double *cpl,
                     int *errorCode){
  return _ApiCalculation_getScaledCompAlt(API_COMMODITY_NOT_GIVEN,
					  alpha60, API_ALPHA60_REV_C_UNIT,
					  temp,  API_TEMPERATURE_C_UNIT,
					  pres, API_PRESSURE_BAR_UNIT,
					  density, API_DENSITY_KGM3,
					  baseTemp, API_TEMPERATURE_C_UNIT,
					  0, API_PRESSURE_BAR_UNIT,
					   ApiUnit_ScaledComp_REV_BAR(),
					  ctl, cpl,  
					  errorCode);
}



/**
* Calculate the dens factor for transformation from 15C and 0kPa
* the desired alternate conditions for Crude Oil. 
*/
double fpA11171kPa(double density, 
                     double temp, 
                     double pres,
                     double baseTemp,
                     int *errorCode){
  return ApiCalculation_getScaledCompAlt( API_CRUDE_OIL_NAME,
					   0,  API_UNIT_NOT_DEFINED,
					   temp,  API_TEMPERATURE_C_UNIT,
					   pres, API_PRESSURE_KPA_UNIT,
					   density, API_DENSITY_KGM3,

					   baseTemp, API_TEMPERATURE_C_UNIT,
					   0, API_PRESSURE_KPA_UNIT,
					   ApiUnit_ScaledComp_REV_KPA(),
					   errorCode);
}

double _fpA11171kPa(double density, 
                     double temp, 
                     double pres,
                     double baseTemp,
					 double *ctl,
					 double *cpl,
                     int *errorCode){
  return _ApiCalculation_getScaledCompAlt( API_CRUDE_OIL_NAME,
					   0,  API_UNIT_NOT_DEFINED,
					   temp,  API_TEMPERATURE_C_UNIT,
					   pres, API_PRESSURE_KPA_UNIT,
					   density, API_DENSITY_KGM3,
					   baseTemp, API_TEMPERATURE_C_UNIT,
					   0, API_PRESSURE_KPA_UNIT,
					   ApiUnit_ScaledComp_REV_KPA(),
					   ctl, cpl,
					   errorCode);
}

/**
* Calculate the dens factor for transformation from 15C and 0kPa
* the desired alternate conditions for Refined products
*/
double fpB11171kPa(double density, 
                     double temp, 
                     double pres,
                     double baseTemp,
                     int *errorCode){
  return ApiCalculation_getScaledCompAlt(API_REFINED_PRODUCTS_NAME,
					  0,  API_UNIT_NOT_DEFINED,
					  temp,  API_TEMPERATURE_C_UNIT,
					  pres, API_PRESSURE_KPA_UNIT,
					  density, API_DENSITY_KGM3,

					  baseTemp, API_TEMPERATURE_C_UNIT,
					   0, API_PRESSURE_KPA_UNIT,
					   ApiUnit_ScaledComp_REV_KPA(),
					   errorCode);
}

double _fpB11171kPa(double density, 
                     double temp, 
                     double pres,
                     double baseTemp,
					 double *ctl,
					 double *cpl,
                     int *errorCode){
  return _ApiCalculation_getScaledCompAlt(API_REFINED_PRODUCTS_NAME,
					  0,  API_UNIT_NOT_DEFINED,
					  temp,  API_TEMPERATURE_C_UNIT,
					  pres, API_PRESSURE_KPA_UNIT,
					  density, API_DENSITY_KGM3,
					  baseTemp, API_TEMPERATURE_C_UNIT,
					   0, API_PRESSURE_KPA_UNIT,
					   ApiUnit_ScaledComp_REV_KPA(),
					   ctl, cpl,
					   errorCode);
}

/**
* Calculate the dens factor for transformation from 15C and 0kPa
* the desired alternate conditions for Lubrication oil.
*/
double fpD11171kPa(double density, 
                     double temp, 
                     double pres,
                     double baseTemp,
                     int *errorCode){
  return ApiCalculation_getScaledCompAlt(API_LUBRICATING_OIL_NAME,
					  0,  API_UNIT_NOT_DEFINED,
					  temp,  API_TEMPERATURE_C_UNIT,
					  pres, API_PRESSURE_KPA_UNIT,
					  density, API_DENSITY_KGM3,

					   baseTemp, API_TEMPERATURE_C_UNIT,
					   0, API_PRESSURE_KPA_UNIT,
					   ApiUnit_ScaledComp_REV_KPA(),
					   errorCode);
}

double _fpD11171kPa(double density, 
                     double temp, 
                     double pres,
                     double baseTemp,
					 double *ctl,
					 double *cpl,
                     int *errorCode){
  return _ApiCalculation_getScaledCompAlt(API_LUBRICATING_OIL_NAME,
					  0,  API_UNIT_NOT_DEFINED,
					  temp,  API_TEMPERATURE_C_UNIT,
					  pres, API_PRESSURE_KPA_UNIT,
					  density, API_DENSITY_KGM3,
					   baseTemp, API_TEMPERATURE_C_UNIT,
					   0, API_PRESSURE_KPA_UNIT,
					   ApiUnit_ScaledComp_REV_KPA(),
					   ctl, cpl,
					   errorCode);
}

/**
* Calculate the dens factor for transformation from 15C and 0kPa
* the desired alternate conditions for alpha60 supplied
*/
double fpC11171kPa(double alpha60,
                     double density, 
                     double temp, 
                     double pres,
                     double baseTemp, 
                     int *errorCode){
  return ApiCalculation_getScaledCompAlt(API_COMMODITY_NOT_GIVEN,
					  alpha60, API_ALPHA60_REV_C_UNIT,
					  temp,  API_TEMPERATURE_C_UNIT,
					  pres, API_PRESSURE_KPA_UNIT,
					  density, API_DENSITY_KGM3,
					  baseTemp, API_TEMPERATURE_C_UNIT,
					  0, API_PRESSURE_KPA_UNIT,
					  ApiUnit_ScaledComp_REV_KPA(),
					  errorCode);
}

double _fpC11171kPa(double alpha60,
                     double density, 
                     double temp, 
                     double pres,
                     double baseTemp, 
					 double *ctl,
					 double *cpl, 
                     int *errorCode){
  return _ApiCalculation_getScaledCompAlt(API_COMMODITY_NOT_GIVEN,
					  alpha60, API_ALPHA60_REV_C_UNIT,
					  temp,  API_TEMPERATURE_C_UNIT,
					  pres, API_PRESSURE_KPA_UNIT,
					  density, API_DENSITY_KGM3,
					  baseTemp, API_TEMPERATURE_C_UNIT,
					  0, API_PRESSURE_KPA_UNIT,
					  ApiUnit_ScaledComp_REV_KPA(),
					  ctl, cpl,
					  errorCode);
}

/**
* Get the scaled compensation factor to transform from alternate to base
*
* @param commodity the commodity for which to do the calculation. If set to -1,
*                  it is assumed that an alpha60 value is supplied
* @param alpha60 the value for  alpha60  
* @param au the unit for alpha60. If API_UNIT_NOT_DEFINED, we set 
*        alpha60 to NULL and assume that a  commodity is supplied.
*
* @param obTemp the temperature at which we observed the data
* @param tuo the unit to use for the observed temperature
* @param obPres the  pressure at which the data was observed
* @param puo the unit to use for the observed pressure
* @param obDens the observed density
* @param duo the unit to use for the observed density
*
* @param baseTemp the base temperature to which to calculate the 
*        expansion factor
* @param tub  the unit for the the base temperature
*
* @param basePres the base pressure to which to calculate the 
*        expansion factors
* @param pub  the unit for the the base pressure
*
* @param fpu the unit for the scaled compensation factor
*
* @param errorCode the errorCode to reprot
*
* @return 0 if calculation was successful, an errorCode otherwise
*/
double ApiCalculation_getScaledCompOb(int commodity,
				      double alpha60,  int au,
				      double obTemp,   int tuo,
				      double obPres,  int  puo,
				      double obDens,   int duo,
				      double baseTemp, int tub,
				      double basePres, int pub,
                      ApiUnit * fpu,
				      int *errorCode){
  double fp = 0;

  *errorCode = ApiCalculation_VBAExportCalculation(commodity,
						   alpha60, au,
                           obTemp, tub, 
						   baseTemp, tub, 
						   obPres, puo,
						   basePres,  puo,
						   obDens, duo,
						   baseTemp, tub,
						   basePres, pub);
  if( *errorCode != 0) return fp;

  fp = ApiQuantity_getValue((ApiQuantity*)calculationRequest->scaledCompOb,
			     fpu,
			    FALSE,
			    errorCode);
  if( *errorCode != 0) fp = 0;
  return fp;   
};

/* fp type 2 */

double _ApiCalculation_getScaledCompOb(int commodity,
				      double alpha60,  int au,
				      double obTemp,   int tuo,
				      double obPres,  int  puo,
				      double obDens,   int duo,
				      double baseTemp, int tub,
				      double basePres, int pub,
                      ApiUnit * fpu,
					  double *ctl, double *cpl,
				      int *errorCode){
  double fp = 0;

  *errorCode = ApiCalculation_VBAExportCalculation(commodity,
						   alpha60, au,
                           obTemp, tub, 
						   baseTemp, tub, 
						   obPres, puo,
						   basePres,  puo,
						   obDens, duo,
						   baseTemp, tub,
						   basePres, pub);
  if( *errorCode != 0) return fp;

  fp = ApiQuantity_getValue((ApiQuantity*)calculationRequest->scaledCompOb,
			     fpu,
			    FALSE,
			    errorCode);
				
  *ctl = ApiQuantity_getValue((ApiQuantity*)calculationRequest->ctlObToBase,
			      ApiUnit_Expansion_DIMLESS(), 
			      FALSE,
			      errorCode);
				  
  *cpl = ApiQuantity_getValue((ApiQuantity*)calculationRequest->cplObToBase,
			  ApiUnit_Expansion_DIMLESS(),
			  FALSE,
			  errorCode);
			  
  if( *errorCode != 0) fp = 0;
  return fp;   
};

/*--------------------------------------------------------------
*   11.1.6.2 type calculations - get dens
*--------------------------------------------------------------*/

/*-------- Density in API ------------------------------------------*/

/**
* Calculate the dens factor for transformation from 60F, 0psi to
* the desired alternate conditions for Crude Oil. Density is in API
*/
double fpA11162API(double density, double temp, double pres, int *errorCode){
  return ApiCalculation_getScaledCompOb( API_CRUDE_OIL_NAME,
					 0,  API_UNIT_NOT_DEFINED,
					 temp,  API_TEMPERATURE_F_UNIT,
					 pres, API_PRESSURE_PSI_UNIT,
					 density, API_DENSITY_API,
					 API_BASE_TEMP, API_TEMPERATURE_F_UNIT,
					 API_BASE_PRES, API_PRESSURE_PSI_UNIT,
					 ApiUnit_ScaledComp_REV_PSI(),
					 errorCode);
}

double _fpA11162API(double density, double temp, double pres, 
	double *ctl, double *cpl, int *errorCode){
  return _ApiCalculation_getScaledCompOb( API_CRUDE_OIL_NAME,
					 0,  API_UNIT_NOT_DEFINED,
					 temp,  API_TEMPERATURE_F_UNIT,
					 pres, API_PRESSURE_PSI_UNIT,
					 density, API_DENSITY_API,
					 API_BASE_TEMP, API_TEMPERATURE_F_UNIT,
					 API_BASE_PRES, API_PRESSURE_PSI_UNIT,
					 ApiUnit_ScaledComp_REV_PSI(),
					 ctl, cpl,
					 errorCode);
}

/**
* Calculate the dens factor for transformation from 60F, 0psi to
* the desired alternate conditions for Refined Products. Density is in API
*/
double fpB11162API(double density, double temp, double pres, int *errorCode){
  double dens;
  dens = ApiCalculation_getScaledCompOb( API_REFINED_PRODUCTS_NAME,
					 0,  API_UNIT_NOT_DEFINED,
					 temp,  API_TEMPERATURE_F_UNIT,
					 pres, API_PRESSURE_PSI_UNIT,
					 density, API_DENSITY_API,
					 API_BASE_TEMP, API_TEMPERATURE_F_UNIT,
					 API_BASE_PRES, API_PRESSURE_PSI_UNIT,
					 ApiUnit_ScaledComp_REV_PSI(),
					 errorCode);
  return dens;
}

double _fpB11162API(double density, double temp, double pres, 
	double *ctl, double *cpl, int *errorCode){
  double dens;
  dens = _ApiCalculation_getScaledCompOb( API_REFINED_PRODUCTS_NAME,
					 0,  API_UNIT_NOT_DEFINED,
					 temp,  API_TEMPERATURE_F_UNIT,
					 pres, API_PRESSURE_PSI_UNIT,
					 density, API_DENSITY_API,
					 API_BASE_TEMP, API_TEMPERATURE_F_UNIT,
					 API_BASE_PRES, API_PRESSURE_PSI_UNIT,
					 ApiUnit_ScaledComp_REV_PSI(),
					 ctl, cpl,
					 errorCode);
  return dens;
}

/**
* Calculate the dens factor for transformation from 60F, 0psi to
* the desired alternate conditions for Lubrication Oil. Density is in API
*/
double fpD11162API(double density, double temp, double pres, int *errorCode){	
  return ApiCalculation_getScaledCompOb( API_LUBRICATING_OIL_NAME,
					 0,  API_UNIT_NOT_DEFINED,
					 temp,  API_TEMPERATURE_F_UNIT,
					 pres, API_PRESSURE_PSI_UNIT,
					 density, API_DENSITY_API,
					 API_BASE_TEMP, API_TEMPERATURE_F_UNIT,
					 API_BASE_PRES, API_PRESSURE_PSI_UNIT,
					 ApiUnit_ScaledComp_REV_PSI(),
					 errorCode);
}

double _fpD11162API(double density, double temp, double pres, 
	double *ctl, double *cpl, int *errorCode){
  return _ApiCalculation_getScaledCompOb( API_LUBRICATING_OIL_NAME,
					 0,  API_UNIT_NOT_DEFINED,
					 temp,  API_TEMPERATURE_F_UNIT,
					 pres, API_PRESSURE_PSI_UNIT,
					 density, API_DENSITY_API,
					 API_BASE_TEMP, API_TEMPERATURE_F_UNIT,
					 API_BASE_PRES, API_PRESSURE_PSI_UNIT,
					 ApiUnit_ScaledComp_REV_PSI(),
					 ctl, cpl,
					 errorCode);
}


/**
* Calculate the dens factor for transformation from 60F, 0psi to
* the desired alternate conditions for supplied alpha values.
* Alpha60 is in 1/F  Density is in API
*/
double fpC11162API(double alpha60, double density, double temp, double pres, int *errorCode){
  return ApiCalculation_getScaledCompOb( API_COMMODITY_NOT_GIVEN,
					 alpha60, API_ALPHA60_REV_F_UNIT,
					 temp,  API_TEMPERATURE_F_UNIT,
					 pres, API_PRESSURE_PSI_UNIT,
					 density, API_DENSITY_API,
					 API_BASE_TEMP, API_TEMPERATURE_F_UNIT,
					 API_BASE_PRES, API_PRESSURE_PSI_UNIT,
					 ApiUnit_ScaledComp_REV_PSI(),
					 errorCode);
}

double _fpC11162API(double alpha60, double density, double temp, double pres, 
	double *ctl, double *cpl, int *errorCode){
  return _ApiCalculation_getScaledCompOb( API_COMMODITY_NOT_GIVEN,
					 alpha60, API_ALPHA60_REV_F_UNIT,
					 temp,  API_TEMPERATURE_F_UNIT,
					 pres, API_PRESSURE_PSI_UNIT,
					 density, API_DENSITY_API,
					 API_BASE_TEMP, API_TEMPERATURE_F_UNIT,
					 API_BASE_PRES, API_PRESSURE_PSI_UNIT,
					 ApiUnit_ScaledComp_REV_PSI(),
					 ctl, cpl,
					 errorCode);
}

/*-------- Density in Rel. Dens ------------------------------------------*/

/**
* Calculate the dens factor for transformation from 60F, 0psi to
* the desired alternate conditions for Crude Oil. Density is in Rel. Dens
*/
double fpA11162RelDens(double density, double temp, double pres, int *errorCode){
  return ApiCalculation_getScaledCompOb( API_CRUDE_OIL_NAME,
					 0,  API_UNIT_NOT_DEFINED,
					 temp,  API_TEMPERATURE_F_UNIT,
					 pres, API_PRESSURE_PSI_UNIT,
					 density, API_DENSITY_RELATIVE,
					 API_BASE_TEMP, API_TEMPERATURE_F_UNIT,
					 API_BASE_PRES, API_PRESSURE_PSI_UNIT,
					 ApiUnit_ScaledComp_REV_PSI(),
					 errorCode);
}

double _fpA11162RelDens(double density, double temp, double pres, 
	double *ctl, double *cpl, int *errorCode){
  return _ApiCalculation_getScaledCompOb( API_CRUDE_OIL_NAME,
					 0,  API_UNIT_NOT_DEFINED,
					 temp,  API_TEMPERATURE_F_UNIT,
					 pres, API_PRESSURE_PSI_UNIT,
					 density, API_DENSITY_RELATIVE,
					 API_BASE_TEMP, API_TEMPERATURE_F_UNIT,
					 API_BASE_PRES, API_PRESSURE_PSI_UNIT,
					 ApiUnit_ScaledComp_REV_PSI(),
					 ctl, cpl,
					 errorCode);
}

/**
* Calculate the dens factor for transformation from 60F, 0psi to
* the desired alternate conditions for Refined Products. Density is in 
* Rel. Dens
*/
double fpB11162RelDens(double density, double temp, double pres, int *errorCode){
  return ApiCalculation_getScaledCompOb( API_REFINED_PRODUCTS_NAME,
					 0,  API_UNIT_NOT_DEFINED,
					 temp,  API_TEMPERATURE_F_UNIT,
					 pres, API_PRESSURE_PSI_UNIT,
					 density, API_DENSITY_RELATIVE,
					 API_BASE_TEMP, API_TEMPERATURE_F_UNIT,
					 API_BASE_PRES, API_PRESSURE_PSI_UNIT,
					 ApiUnit_ScaledComp_REV_PSI(),
					 errorCode);
}

double _fpB11162RelDens(double density, double temp, double pres, 
	double *ctl, double *cpl, int *errorCode){
  return _ApiCalculation_getScaledCompOb( API_REFINED_PRODUCTS_NAME,
					 0,  API_UNIT_NOT_DEFINED,
					 temp,  API_TEMPERATURE_F_UNIT,
					 pres, API_PRESSURE_PSI_UNIT,
					 density, API_DENSITY_RELATIVE,
					 API_BASE_TEMP, API_TEMPERATURE_F_UNIT,
					 API_BASE_PRES, API_PRESSURE_PSI_UNIT,
					 ApiUnit_ScaledComp_REV_PSI(),
					 ctl, cpl,
					 errorCode);
}

/**
* Calculate the dens factor for transformation from 60F, 0psi to
* the desired alternate conditions for Lubrication Oil. Density is in Rel. Dens.
*/
double fpD11162RelDens(double density, double temp, double pres, int *errorCode){
  return ApiCalculation_getScaledCompOb( API_LUBRICATING_OIL_NAME,
					 0,  API_UNIT_NOT_DEFINED,
					 temp,  API_TEMPERATURE_F_UNIT,
					 pres, API_PRESSURE_PSI_UNIT,
					 density, API_DENSITY_RELATIVE,
					 API_BASE_TEMP, API_TEMPERATURE_F_UNIT,
					 API_BASE_PRES, API_PRESSURE_PSI_UNIT,
					 ApiUnit_ScaledComp_REV_PSI(),
					 errorCode);
}

double _fpD11162RelDens(double density, double temp, double pres, 
	double *ctl, double *cpl, int *errorCode){
  return _ApiCalculation_getScaledCompOb( API_LUBRICATING_OIL_NAME,
					 0,  API_UNIT_NOT_DEFINED,
					 temp,  API_TEMPERATURE_F_UNIT,
					 pres, API_PRESSURE_PSI_UNIT,
					 density, API_DENSITY_RELATIVE,
					 API_BASE_TEMP, API_TEMPERATURE_F_UNIT,
					 API_BASE_PRES, API_PRESSURE_PSI_UNIT,
					 ApiUnit_ScaledComp_REV_PSI(),
					 ctl, cpl,
					 errorCode);
}

/**
* Calculate the dens factor for transformation from 60F, 0psi to
* the desired alternate conditions for supplied alpha values.
* Alpha60 is in 1/F  Density is in Rel. Dens.
*/
double fpC11162RelDens(double alpha60, double density, double temp, double pres, int *errorCode){ 
  return ApiCalculation_getScaledCompOb( API_COMMODITY_NOT_GIVEN,
					 alpha60, API_ALPHA60_REV_F_UNIT,
					 temp,  API_TEMPERATURE_F_UNIT,
					 pres, API_PRESSURE_PSI_UNIT,
					 density, API_DENSITY_RELATIVE,
					 API_BASE_TEMP, API_TEMPERATURE_F_UNIT,
					 API_BASE_PRES, API_PRESSURE_PSI_UNIT,
					 ApiUnit_ScaledComp_REV_PSI(),
					 errorCode);
}

double _fpC11162RelDens(double alpha60, double density, double temp, double pres, double *ctl, double *cpl, int *errorCode){ 
  return _ApiCalculation_getScaledCompOb( API_COMMODITY_NOT_GIVEN,
					 alpha60, API_ALPHA60_REV_F_UNIT,
					 temp,  API_TEMPERATURE_F_UNIT,
					 pres, API_PRESSURE_PSI_UNIT,
					 density, API_DENSITY_RELATIVE,
					 API_BASE_TEMP, API_TEMPERATURE_F_UNIT,
					 API_BASE_PRES, API_PRESSURE_PSI_UNIT,
					 ApiUnit_ScaledComp_REV_PSI(),
					 ctl, cpl,
					 errorCode);
}


/*--------------------------------------------------------------
*   11.1.7.2 type calculations - get dens
*--------------------------------------------------------------*/

/**
* Calculate the dens factor for transformation from 15C and 0kPa
* the desired alternate conditions for Crude Oil. 
*/
double fpA11172bar(double density, double temp, double pres, double baseTemp, 
	int *errorCode){ 
  return ApiCalculation_getScaledCompOb( API_CRUDE_OIL_NAME,
					 0,  API_UNIT_NOT_DEFINED,
					 temp,  API_TEMPERATURE_C_UNIT,
					 pres, API_PRESSURE_BAR_UNIT,
					 density, API_DENSITY_KGM3,
					 baseTemp, API_TEMPERATURE_C_UNIT,
					 0, API_PRESSURE_BAR_UNIT,
					 ApiUnit_ScaledComp_REV_BAR(),
					 errorCode);
}
					 
double _fpA11172bar(double density, double temp, double pres, double baseTemp, 
	double *ctl, double *cpl, int *errorCode){ 
  return _ApiCalculation_getScaledCompOb( API_CRUDE_OIL_NAME,
					 0,  API_UNIT_NOT_DEFINED,
					 temp,  API_TEMPERATURE_C_UNIT,
					 pres, API_PRESSURE_BAR_UNIT,
					 density, API_DENSITY_KGM3,
					 baseTemp, API_TEMPERATURE_C_UNIT,
					 0, API_PRESSURE_BAR_UNIT,
					 ApiUnit_ScaledComp_REV_BAR(),
					 ctl, cpl,
					 errorCode);
}

/**
* Calculate the dens factor for transformation from 15C and 0kPa
* the desired alternate conditions for Refined products
*/
double fpB11172bar(double density, double temp, double pres, double baseTemp, int *errorCode){ 
  return ApiCalculation_getScaledCompOb(API_REFINED_PRODUCTS_NAME,
					0,  API_UNIT_NOT_DEFINED,
					temp,  API_TEMPERATURE_C_UNIT,
					pres, API_PRESSURE_BAR_UNIT,
					density, API_DENSITY_KGM3,
					baseTemp, API_TEMPERATURE_C_UNIT,
					0, API_PRESSURE_BAR_UNIT,
					ApiUnit_ScaledComp_REV_BAR(),
					errorCode);
}

double _fpB11172bar(double density, double temp, double pres, double baseTemp, 
	double *ctl, double *cpl, int *errorCode){ 
  return _ApiCalculation_getScaledCompOb(API_REFINED_PRODUCTS_NAME,
					0,  API_UNIT_NOT_DEFINED,
					temp,  API_TEMPERATURE_C_UNIT,
					pres, API_PRESSURE_BAR_UNIT,
					density, API_DENSITY_KGM3,
					baseTemp, API_TEMPERATURE_C_UNIT,
					0, API_PRESSURE_BAR_UNIT,
					ApiUnit_ScaledComp_REV_BAR(),
					ctl, cpl,
					errorCode);
}

/**
* Calculate the dens factor for transformation from 15C and 0kPa
* the desired alternate conditions for Lubrication oil.
*/
double fpD11172bar(double density, double temp, double pres, double baseTemp, int *errorCode){ 
  return ApiCalculation_getScaledCompOb(API_LUBRICATING_OIL_NAME,
					0,  API_UNIT_NOT_DEFINED,
					temp,  API_TEMPERATURE_C_UNIT,
					pres, API_PRESSURE_BAR_UNIT,
					density, API_DENSITY_KGM3,
					baseTemp, API_TEMPERATURE_C_UNIT,
					0, API_PRESSURE_BAR_UNIT,
					 ApiUnit_ScaledComp_REV_BAR(),
					errorCode);
}

double _fpD11172bar(double density, double temp, double pres, double baseTemp, 
	double *ctl, double *cpl, int *errorCode){ 
  return _ApiCalculation_getScaledCompOb(API_LUBRICATING_OIL_NAME,
					0,  API_UNIT_NOT_DEFINED,
					temp,  API_TEMPERATURE_C_UNIT,
					pres, API_PRESSURE_BAR_UNIT,
					density, API_DENSITY_KGM3,
					baseTemp, API_TEMPERATURE_C_UNIT,
					0, API_PRESSURE_BAR_UNIT,
					 ApiUnit_ScaledComp_REV_BAR(),
					 ctl, cpl,
					errorCode);
}

/**
* Calculate the dens factor for transformation from 15C and 0kPa
* the desired alternate conditions for alpha60 supplied
*/
double fpC11172bar(double alpha60, double density, double temp, double pres, double baseTemp, int *errorCode){
  return ApiCalculation_getScaledCompOb(API_COMMODITY_NOT_GIVEN,
					alpha60, API_ALPHA60_REV_C_UNIT,
					temp,  API_TEMPERATURE_C_UNIT,
					pres, API_PRESSURE_BAR_UNIT,
					density, API_DENSITY_KGM3,
					baseTemp, API_TEMPERATURE_C_UNIT,
					0, API_PRESSURE_BAR_UNIT,
					ApiUnit_ScaledComp_REV_BAR(),
					errorCode);
}

double _fpC11172bar(double alpha60, double density, double temp, double pres, 
	double baseTemp, double *ctl, double *cpl, int *errorCode){
  return _ApiCalculation_getScaledCompOb(API_COMMODITY_NOT_GIVEN,
					alpha60, API_ALPHA60_REV_C_UNIT,
					temp,  API_TEMPERATURE_C_UNIT,
					pres, API_PRESSURE_BAR_UNIT,
					density, API_DENSITY_KGM3,
					baseTemp, API_TEMPERATURE_C_UNIT,
					0, API_PRESSURE_BAR_UNIT,
					ApiUnit_ScaledComp_REV_BAR(),
					ctl, cpl,
					errorCode);
}

/**
* Calculate the dens factor for transformation from 15C and 0kPa
* the desired alternate conditions for Crude Oil. 
*/
double fpA11172kPa(double density, double temp, double pres, double baseTemp, 
	int *errorCode){ 
  return ApiCalculation_getScaledCompOb( API_CRUDE_OIL_NAME,
					 0,  API_UNIT_NOT_DEFINED,
					 temp,  API_TEMPERATURE_C_UNIT,
					 pres, API_PRESSURE_KPA_UNIT,
					 density, API_DENSITY_KGM3,
					 baseTemp, API_TEMPERATURE_C_UNIT,
					 0, API_PRESSURE_KPA_UNIT,
					 ApiUnit_ScaledComp_REV_KPA(),
					 errorCode);
}

double _fpA11172kPa(double density, double temp, double pres, double baseTemp, 
	double *ctl, double *cpl, int *errorCode){ 
  return _ApiCalculation_getScaledCompOb( API_CRUDE_OIL_NAME,
					 0,  API_UNIT_NOT_DEFINED,
					 temp,  API_TEMPERATURE_C_UNIT,
					 pres, API_PRESSURE_KPA_UNIT,
					 density, API_DENSITY_KGM3,
					 baseTemp, API_TEMPERATURE_C_UNIT,
					 0, API_PRESSURE_KPA_UNIT,
					 ApiUnit_ScaledComp_REV_KPA(),
					 ctl, cpl,
					 errorCode);
}

/**
* Calculate the dens factor for transformation from 15C and 0kPa
* the desired alternate conditions for Refined products
*/
double fpB11172kPa(double density, double temp, double pres, double baseTemp, int *errorCode){ 
  return ApiCalculation_getScaledCompOb(API_REFINED_PRODUCTS_NAME,
					0,  API_UNIT_NOT_DEFINED,
					temp,  API_TEMPERATURE_C_UNIT,
					pres, API_PRESSURE_KPA_UNIT,
					density, API_DENSITY_KGM3,
					baseTemp, API_TEMPERATURE_C_UNIT,
					0, API_PRESSURE_KPA_UNIT,
					ApiUnit_ScaledComp_REV_KPA(),
					errorCode);
}

double _fpB11172kPa(double density, double temp, double pres, double baseTemp, 
	double *ctl, double *cpl, int *errorCode){ 
  return _ApiCalculation_getScaledCompOb(API_REFINED_PRODUCTS_NAME,
					0,  API_UNIT_NOT_DEFINED,
					temp,  API_TEMPERATURE_C_UNIT,
					pres, API_PRESSURE_KPA_UNIT,
					density, API_DENSITY_KGM3,
					baseTemp, API_TEMPERATURE_C_UNIT,
					0, API_PRESSURE_KPA_UNIT,
					ApiUnit_ScaledComp_REV_KPA(),
					ctl, cpl,
					errorCode);
}

/**
* Calculate the dens factor for transformation from 15C and 0kPa
* the desired alternate conditions for Lubrication oil.
*/
double fpD11172kPa(double density, double temp, double pres, double baseTemp, int *errorCode){ 
  return ApiCalculation_getScaledCompOb(API_LUBRICATING_OIL_NAME,
					0,  API_UNIT_NOT_DEFINED,
					temp,  API_TEMPERATURE_C_UNIT,
					pres, API_PRESSURE_KPA_UNIT,
					density, API_DENSITY_KGM3,
					baseTemp, API_TEMPERATURE_C_UNIT,
					0, API_PRESSURE_KPA_UNIT,
					ApiUnit_ScaledComp_REV_KPA(),
					errorCode);
}

double _fpD11172kPa(double density, double temp, double pres, double baseTemp, 
	double *ctl, double *cpl, int *errorCode){ 
  return _ApiCalculation_getScaledCompOb(API_LUBRICATING_OIL_NAME,
					0,  API_UNIT_NOT_DEFINED,
					temp,  API_TEMPERATURE_C_UNIT,
					pres, API_PRESSURE_KPA_UNIT,
					density, API_DENSITY_KGM3,
					baseTemp, API_TEMPERATURE_C_UNIT,
					0, API_PRESSURE_KPA_UNIT,
					ApiUnit_ScaledComp_REV_KPA(),
					ctl, cpl,
					errorCode);
}

/**
* Calculate the dens factor for transformation from 15C and 0kPa
* the desired alternate conditions for alpha60 supplied
*/
double fpC11172kPa(double alpha60, double density, double temp, double pres, double baseTemp,  int *errorCode){ 
  return ApiCalculation_getScaledCompOb(API_COMMODITY_NOT_GIVEN,
					alpha60, API_ALPHA60_REV_C_UNIT,
					temp,  API_TEMPERATURE_C_UNIT,
					pres, API_PRESSURE_KPA_UNIT,
					density, API_DENSITY_KGM3,
					baseTemp, API_TEMPERATURE_C_UNIT,
					0, API_PRESSURE_KPA_UNIT,
					ApiUnit_ScaledComp_REV_KPA(),
					errorCode);
}

double _fpC11172kPa(double alpha60, double density, double temp, double pres, 
	double baseTemp, double *ctl, double *cpl, int *errorCode){ 
  return _ApiCalculation_getScaledCompOb(API_COMMODITY_NOT_GIVEN,
					alpha60, API_ALPHA60_REV_C_UNIT,
					temp,  API_TEMPERATURE_C_UNIT,
					pres, API_PRESSURE_KPA_UNIT,
					density, API_DENSITY_KGM3,
					baseTemp, API_TEMPERATURE_C_UNIT,
					0, API_PRESSURE_KPA_UNIT,
					ApiUnit_ScaledComp_REV_KPA(),
					ctl, cpl, 
					errorCode);
}