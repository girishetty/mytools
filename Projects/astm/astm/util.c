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


#include "util.h"

/**
* The names of the input parameters
*/
static const char *ApiParameterNames[API_PARAMETERS_MAX_NUMBER] = 
                   {"Base Temp.",
		    "Base Pres.",
		    "Commodity",
		    "Alpha_60",
		    "Ob. Temp",
		    "Ob. Pres",
		    "Ob. Dens",
		    "Alt.Temp",
		    "Alt. Pres.",
		    "Ob. Vol",
		    "Alt. Vol.",
		    "Base Vol.",
		    "Alt. Dens",
		    "Fp",
		    "Base Dens.",
		    "ctl (obs. to base)",
		    "cpl (obs. to base)",
		    "ctpl (obs. to base)",
		    "Fp (obs. to base)",
		    "ctl (base to alt.)",
		    "cpl (base to alt.)",
		    "ctpl (base to alt.)"};

/**
* The names of the oil products
*/ 
static const char *ApiOilProductNames[API_OIL_PRODUCT_MAX_NUMBER] = 
                    {"Crude Oil",
		     "Lubricating Oil",
		     "Refined Products",
		     "Fuel Oil",
		     "Jet Fuel",
		     "Transition Zone",
		     "Gasoline"};

/**
* The error values
*/
static const char *ApiErrorValues[MAX_ERROR_NUMBER] =
     { "Initialization failed",
       "Nullpointer Exception",
       "Invalid unit for quantity",
       "Attempted to change an immuatable quantity",
       "Buffer overflow for text base char array",
       "Attempt to compare two quantites of different type",
       "Value for quantity is out of range",
       "Attempt to call an unsupported function",
       "K-Values are not defined for the given commodity",
       "rho limits are not defined for the given commodity",
       "Undefined commodity type",
       "Commodity and alpha_60 are both supplied, please give only one",
       "Commodity and alpha_60 are both null. Please supply at least one",
       "Alpha_60 value is out of range",
       "Observed density value is missing",
       "Observed density is out of range for Type I calculation",
       "Observed density is out of range for Type II calculation",
       "Observed pressure value is missing",
       "Observed pressure is out of range",
       "Alternate pressure value is missing",
       "Alternate pressure is out of range",
       "Observed temperature value is missing",
       "Observed temperature is out of range",
       "Alternate temperature value is missing",
       "Alternate temperature is out of range",
       "Please supply only one of these: Observered Volume, Base Volume or Alternate Volume",
       "Density out of range during iteration",
       "Convergence was not reached",
       "Array index out of range"};



/**
* Report the names for the input parameters
*
*/
int ApiCalculation_getParameterName(int name, char * buffer, int length){
  int errorCode = 0;

  if( name >= 0 && name < API_PARAMETERS_MAX_NUMBER){
    errorCode = ApiCalculationRequest_appendBuffer(buffer, 
						   length,
						   ApiParameterNames[name]);
  }
  else errorCode = ARRAY_INDEX_OUT_OF_RANGE;
  return errorCode;
}

/** 
* A function that converts Oil product numbers into chars 
*/
int ApiCalculation_getProductName(int name, char * buffer, int length){
  int errorCode = 0;

  if( name >= 0 && name < API_OIL_PRODUCT_MAX_NUMBER){
    errorCode = ApiCalculationRequest_appendBuffer(buffer, 
						   length,
						   ApiOilProductNames[name]);
  }
  else errorCode = ARRAY_INDEX_OUT_OF_RANGE;
  return errorCode;
}

/** 
* A function that converts the error code to a description 
*/
int ApiCalculation_getErrorDescription(int name, char * buffer, int length){
  int errorCode = 0;

  if( name >  0 && name <= MAX_ERROR_NUMBER ){
    errorCode = ApiCalculationRequest_appendBuffer(buffer, 
						   length,
						   ApiErrorValues[name-1]);
  }
  else errorCode = ARRAY_INDEX_OUT_OF_RANGE;
  return errorCode;
}



/**
* A convienence function to set all potential input parameters for
* a VCF calculation
*/
int ApiCalculation_setParameters(ApiCalculationRequest * request,
				 int commodity,
				 double alpha60,  ApiUnit *au,
				 double obTemp,   ApiUnit *tuo,
				 double altTemp,  ApiUnit *tua,
				 double obPres,   ApiUnit* puo,
				 double altPres,  ApiUnit* pua,
				 double obDens,   ApiUnit* duo,
				 double baseTemp, ApiUnit *tub,
				 double basePres, ApiUnit *pub,

				 double baseVol,  ApiUnit *vub,
				 double altVol,   ApiUnit *vua,
				 double obVol,    ApiUnit *vuo){
  int errorCode;

  if( request == NULL) return NULL_POINTER_EXCEPTION;

  request->commodityName = commodity;
  if( au == NULL){
    if( request->alpha60 != NULL) ApiQuanity_cleanUp((ApiQuantity*)request->alpha60);
    request->alpha60 = NULL;
  }
  else{
    if( request->alpha60 != NULL){
       errorCode =  ApiQuantity_setValue((ApiQuantity*)request->alpha60,alpha60, au);
       if( errorCode != 0) return errorCode;
    }
    else{
       request->alpha60 = ApiAlpha60_init(alpha60, au, TRUE, &errorCode);
       if( errorCode != 0) return errorCode;
    }
  } 


  if( request->obTemp != NULL){
    errorCode =  ApiQuantity_setValue((ApiQuantity*)request->obTemp, obTemp, tuo);
    if( errorCode != 0) return errorCode;
  }
  else{
     request->obTemp = ApiTemperature_init(obTemp, tuo, TRUE, &errorCode);
     if( errorCode != 0) return errorCode;
  }

  if( request->altTemp != NULL){
    errorCode =  ApiQuantity_setValue((ApiQuantity*)request->altTemp, altTemp, tua);
    if( errorCode != 0) return errorCode;
  }
  else{
     request->altTemp = ApiTemperature_init(altTemp, tua, TRUE, &errorCode);
     if( errorCode != 0) return errorCode;
  }

  if( request->obPres != NULL){
    errorCode =  ApiQuantity_setValue((ApiQuantity*)request->obPres, obPres, puo);
    if( errorCode != 0) return errorCode;
  }
  else{
     request->obPres = ApiPressure_init(obPres, puo, TRUE, &errorCode);
     if( errorCode != 0) return errorCode;
  }

  if( request->altPres != NULL){
    errorCode =  ApiQuantity_setValue((ApiQuantity*)request->altPres, altPres, pua);
    if( errorCode != 0) return errorCode;
  }
  else{
     request->altPres = ApiPressure_init(altPres, pua, TRUE, &errorCode);
     if( errorCode != 0) return errorCode;
  }

  if( request->obDens != NULL){
    errorCode =  ApiQuantity_setValue((ApiQuantity*)request->obDens,obDens, duo);
    if( errorCode != 0) return errorCode;
  }
  else{
     request->obDens = ApiDensity_init(obDens, duo, TRUE, &errorCode);
     if( errorCode != 0) return errorCode;
  }

  if( request->baseTemp != NULL){
    errorCode =  ApiQuantity_setValue((ApiQuantity*)request->baseTemp,baseTemp, tub);
    if( errorCode != 0) return errorCode;
  }
  else{
     request->baseTemp = ApiTemperature_init(baseTemp, tub, TRUE, &errorCode);
     if( errorCode != 0) return errorCode;
  }

  if( request->basePres != NULL){
    errorCode =  ApiQuantity_setValue((ApiQuantity*)request->basePres,basePres, pub);
    if( errorCode != 0) return errorCode;
  }
  else{
     request->basePres = ApiPressure_init(basePres, pub, TRUE, &errorCode);
     if( errorCode != 0) return errorCode;
  }

  if( vub == NULL){
    if( request->baseVol != NULL) ApiQuanity_cleanUp((ApiQuantity*)request->baseVol);
    request->baseVol = NULL;
  }
  else{
    if( request->baseVol != NULL){
       errorCode =  ApiQuantity_setValue((ApiQuantity*)request->baseVol,baseVol, vub);
       if( errorCode != 0) return errorCode;
    }
    else{
       request->baseVol = ApiVolume_init(baseVol, vub, TRUE, &errorCode);
       if( errorCode != 0) return errorCode;
    }
  }

  if( vua == NULL){
    if( request->altVol != NULL) ApiQuanity_cleanUp((ApiQuantity*)request->altVol);
    request->altVol = NULL;
  }
  else{
    if( request->altVol != NULL){
       errorCode =  ApiQuantity_setValue((ApiQuantity*)request->altVol,altVol, vua);
       if( errorCode != 0) return errorCode;
    }
    else{
       request->altVol = ApiVolume_init(altVol, vua, TRUE, &errorCode);
       if( errorCode != 0) return errorCode;
    }
  }

  if( vuo == NULL){
    if( request->obVol != NULL) ApiQuanity_cleanUp((ApiQuantity*)request->obVol);
    request->obVol = NULL;
  }
  else{
    if( request->obVol != NULL){
       errorCode =  ApiQuantity_setValue((ApiQuantity*)request->obVol,obVol, vuo);
       if( errorCode != 0) return errorCode;
    }
    else{
       request->obVol = ApiVolume_init(obVol, vuo, TRUE, &errorCode);
       if( errorCode != 0) return errorCode;
    }
  }
  return errorCode;
}


/**
* A convienence function to set parameters for a 11.1.6 type calculation.
* If the volume unit is NULL, the volume is not calculated
*/
int ApiCalculation_set11_1_6_1Parameters(ApiCalculationRequest * request,
					 int commodity,
					 double alpha60,  ApiUnit *au,
					 double altTemp,  ApiUnit *tua,
					 double altPres,  ApiUnit* pua,
					 double baseDens, ApiUnit* dub,
	
				       double baseVol,  ApiUnit *vub){
  int errorCode = 0;

  errorCode = ApiCalculation_setParameters(request,
					   commodity,
					   alpha60, au,
					   API_BASE_TEMP, ApiUnit_Temperature_F(),
					   altTemp, tua,
					   API_BASE_PRES, ApiUnit_Pressure_PSI(),
					   altPres, pua,
					   baseDens, dub,
					   API_BASE_TEMP, ApiUnit_Temperature_F(),
					   API_BASE_PRES, ApiUnit_Pressure_PSI(),
					   baseVol, vub,
					   0, NULL,
					   0, NULL);
  return errorCode;
}

/**
* A convienence function to set parameters for a 11.1.6.2  type calculation.
* If the volume unit is NULL, the volume is not calculated
*/
int ApiCalculation_set11_1_6_2Parameters(ApiCalculationRequest * request,
					 int commodity,
					 double alpha60,  ApiUnit *au,
					 double obTemp,   ApiUnit *tuo,
					 double obPres,   ApiUnit* puo,
					 double obDens,   ApiUnit* duo,
	
					 double obVol,  ApiUnit *vuo){
  int errorCode = 0;

  errorCode =  ApiCalculation_setParameters(request,
					    commodity,
					    alpha60,au,
					    obTemp, tuo,
					    API_BASE_TEMP, ApiUnit_Temperature_F(),
					    obPres, puo,
					    API_BASE_PRES, ApiUnit_Pressure_PSI(),
					    obDens, duo,
                                            API_BASE_TEMP, ApiUnit_Temperature_F(),
					    API_BASE_PRES, ApiUnit_Pressure_PSI(), 
					    0, NULL,
					    0, NULL,
					    obVol, vuo);	
  return errorCode;
}

/**
* A convienence function to set parameters for a 11.1.6.3  type calculation.
*/
int ApiCalculation_set11_1_6_3Parameters(ApiCalculationRequest * request,
					 int commodity,
					 double alpha60,  ApiUnit * au,
					 double obTemp,   ApiUnit * tuo,
					 double obPres,   ApiUnit * puo,
					 double obDens,   ApiUnit * duo,
	
					 double altTemp,   ApiUnit * tua,
					 double altPres,   ApiUnit * pua,

					 double obVol,  ApiUnit *vuo){
  int errorCode = 0;

  errorCode =  ApiCalculation_setParameters(request,
					    commodity,
					    alpha60,au,
					    obTemp, tuo,
					    altTemp, tua,
					    obPres, puo,
					    altPres, pua,
					    obDens, duo,
                                            API_BASE_TEMP, ApiUnit_Temperature_F(),
					    API_BASE_PRES, ApiUnit_Pressure_PSI(), 
					    0, NULL,
					    0, NULL,
					    obVol, vuo);	
  return errorCode;
}

/**
* A convienence function to set parameters for a 11.1.7.1 type calculation.
*/
int ApiCalculation_set11_1_7_1Parameters(ApiCalculationRequest * request,
					 int commodity,
					 double alpha60,  ApiUnit *au,
					 double altTemp,  ApiUnit *tua,
					 double altPres,  ApiUnit* pua,
					 double baseDens, ApiUnit* dub,

					 double baseTemp, ApiUnit * tub,
	
					 double baseVol,  ApiUnit *vub){
  int errorCode = 0;

  errorCode = ApiCalculation_setParameters(request,
					   commodity,
					   alpha60, au,
					   baseTemp, tub,
					   altTemp, tua,
					   0, ApiUnit_Pressure_KPA(),
					   altPres, pua,
					   baseDens, dub,
					   baseTemp, tub,
					   0, ApiUnit_Pressure_KPA(),
					   baseVol, vub,
					   0, NULL,
					   0, NULL);
  return errorCode;
}

/**
* A convienence function to set parameters for a 11.1.7.2  type calculation.
* If the volume unit is NULL, the volume is not calculated
*/
int ApiCalculation_set11_1_7_2Parameters(ApiCalculationRequest * request,
					 int commodity,
					 double alpha60,  ApiUnit * au,
					 double obTemp,   ApiUnit * tuo,
					 double obPres,   ApiUnit * puo,
					 double obDens,   ApiUnit * duo,

					 double baseTemp, ApiUnit *tub,
	
					 double obVol,  ApiUnit *vuo){
  int errorCode = 0;

  errorCode =  ApiCalculation_setParameters(request,
					    commodity,
					    alpha60,au,
					    obTemp, tuo,
					    baseTemp, tub,
					    obPres, puo,
					    0, ApiUnit_Pressure_KPA(),
					    obDens, duo,
                                            baseTemp, tub,
					    0, ApiUnit_Pressure_KPA(),
					    0, NULL,
					    0, NULL,
					    obVol, vuo);	
  return errorCode;
}


/**
* Run all  the tests
*/
bool ApiCalculation_runTests(char * buffer, 
			     int bufLength,
			     char * errBuf,
			     int errLength,
			     int *errorCode){
  bool result = TRUE;
  bool tmp;

  tmp =  ApiQuantity_runTest(errBuf, errLength, errorCode);
  if( *errorCode != 0) return FALSE;
  if( tmp == FALSE) result = FALSE;
  result =  ApiCalculationTest_runTests(NULL, 0, errBuf, errLength, errorCode);
  if( *errorCode != 0) return FALSE;
  if( tmp == FALSE) result = FALSE;
  return result;
}
