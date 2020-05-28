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


#include "calculation.h"
#include "parameternames.h"
#include <string.h>
#include <math.h>
#include <stdlib.h>
#include <stdio.h>

/** 
* The commodity group used in the calculations.
* The user supplies a name from which this commodity is
* determined. 
*/
static ApiOilProduct *CALC_REQ_COMMODITY = NULL;

/** 
* The commodity  group used during calculations.
* In the case of Crude Oil and Lubrication Oil this is the same 
* as {@link #commodity commodity}. However, in the case of 
* Refined Products, this is the sub-category, i.e. Fuel Oils, 
* Jet Fuel, etc.
*/
static ApiOilProduct * CALC_REQ_DET_COM = NULL;

/** 
* A CalculationRequest object used if the base conditions are not
* 60 F and 0 psi to buffer intermediate calculations
*/
static ApiCalculationRequest * CALC_REQ_INTER = NULL;


/**
* The Base temperature to which all calculations are done
*/ 
static ApiTemperature * CALC_REQ_BASE_TEMP = NULL;


/**
* The Base pressure to which all calculations are done
*/ 
static ApiPressure *  CALC_REQ_BASE_PRES = NULL;

/**
* Initialize all resources needed to do a calculation
*/
int ApiCalculationRequest_initialize(){
  int errorCode = 0;

  errorCode =  ApiUnit_initalize();
  if( errorCode != 0){
    ApiCalculationRequest_destroy();
    return errorCode;
  }

  errorCode =  ApiOilProduct_initalize();
  if( errorCode != 0){
    ApiCalculationRequest_destroy();
    return errorCode;
  } 
  
  return errorCode;
}

/**
* Free all resources needed to do a calculation
*/

/* Modified by ian mcdavid, Flow-Cal development
   Previously, ApiOilProduct_destory, ApiUnit_destroy, and
   ApiQuantity_destroy, were invoked before the remainder
   of the imlpementation. This was a temporal coupling problem.
   ApiQuantity would destory the contents of the static reference
   to the VTable objects (eg. ApiTemperatureVTable) and then the 
   subsequent methods would resurrect the VTable objects. More
   specifically, a call to ApiQuantity_cleanUp(CALC_REQ_BASE_TEMP)
   would resurrect the ApiTemperatureVTable with corrupt data at
   this point, since all of the unit related static references
   had already been destroyed. The corrupted static references to the
   VTable objects would persist through multiple calls to the application
   and never be cleaned up or created properly. 
*/

void ApiCalculationRequest_destroy(){  
  if( CALC_REQ_BASE_TEMP != NULL){ 
    ApiQuanity_cleanUp( (ApiQuantity*)CALC_REQ_BASE_TEMP);
  }
  CALC_REQ_BASE_TEMP = NULL;

  if( CALC_REQ_BASE_PRES != NULL){ 
    ApiQuanity_cleanUp( (ApiQuantity*)CALC_REQ_BASE_PRES);
  }
  CALC_REQ_BASE_PRES = NULL;
 
  if( CALC_REQ_INTER != NULL){
     ApiCalculationRequest_cleanUp(CALC_REQ_INTER);
  }
  CALC_REQ_INTER = NULL;
    
  ApiOilProduct_destroy();
  ApiUnit_destroy();
  ApiQuantity_destroy();
}



/**
* Create and initialize an  ApiCalculationRequest object.
*/
ApiCalculationRequest * ApiCalculationRequest_init(int * errorCode){
   ApiCalculationRequest * request = 0;

   *errorCode = 0;

   /** Create the object itself */
   request = (ApiCalculationRequest*)malloc( sizeof(ApiCalculationRequest));
   if( request == 0){
        *errorCode = INITIALIZE_FAILED;
	return request;
   }
   memset( request, 0, sizeof(ApiCalculationRequest));

   /** Initialize the base parameters */
   
   request->baseTemp =  ApiTemperature_init(API_BASE_TEMP,
					    ApiUnit_Temperature_F(),
					    TRUE,
					    errorCode);

   if( *errorCode != 0){
     request->baseTemp = NULL;
     return request;
   }

   request->basePres =  ApiPressure_init(API_BASE_PRES,
					 ApiUnit_Pressure_PSI(),
					 TRUE,
					 errorCode);
   if( *errorCode != 0){
     if( request->baseTemp != NULL) {
       ApiQuanity_cleanUp((ApiQuantity*)request->baseTemp);
     }
     request->baseTemp = NULL;
     request->basePres = NULL;
     return request;
   }

   /** By default do not report intermediate results */
   request->reportIntermediate = FALSE;

   /** And return the new object*/
   return request;    
}

/**
* Clean all the resources associated with an ApiCalcualtionRequest.
*/

/* Modified by ian mcdavid, Flow-Cal development
   The request members were only cleaned up if they were already null
   in the previous version. For example:
   if( request->baseTemp == NULL) { ApiQuantity_cleanUp( ... ) }
   this makes no sense to only clean up members which are already null.

   This method now cleans up members which are not null and hence require
   clean up.
*/

void  ApiCalculationRequest_cleanUp(ApiCalculationRequest * request){
  if( request == NULL) return;
  ApiCalculationRequest_setErrorStatus( request);

  if( request->baseTemp != NULL){
     ApiQuanity_cleanUp((ApiQuantity*)request->baseTemp);
  }
  request->baseTemp = NULL;

  if( request->basePres != NULL){
     ApiQuanity_cleanUp((ApiQuantity*)request->basePres);
  }
  request->basePres = NULL;

  if( request->alpha60 != NULL){
     ApiQuanity_cleanUp((ApiQuantity*)request->alpha60);
  }
  request->alpha60 = NULL;

  if( request->obTemp != NULL){
     ApiQuanity_cleanUp((ApiQuantity*)request->obTemp);
  }
  request->obTemp = NULL;

  if( request->obPres != NULL){
     ApiQuanity_cleanUp((ApiQuantity*)request->obPres);
  }
  request->obPres = NULL;

  if( request->obDens != NULL){
     ApiQuanity_cleanUp((ApiQuantity*)request->obDens);
  }
  request->obDens = NULL;

  if( request->altTemp != NULL){
     ApiQuanity_cleanUp((ApiQuantity*)request->altTemp);
  }
  request->altTemp = NULL;

  if( request->altPres != NULL){
     ApiQuanity_cleanUp((ApiQuantity*)request->altPres);
  }
  request->altPres = NULL;

  if( request->obVol != NULL){
     ApiQuanity_cleanUp((ApiQuantity*)request->obVol);
  }
  request->obVol = NULL;

  if( request->baseVol != NULL){
     ApiQuanity_cleanUp((ApiQuantity*)request->baseVol);
  }
  request->baseVol = NULL;

  if( request->altVol != NULL){
     ApiQuanity_cleanUp((ApiQuantity*)request->altVol);
  }
  request->altVol = NULL;
 
  if( request->comments != NULL){
    free( request->comments);
  }
  request->comments = NULL;

  free(request);
}


/**
* Sets all the output parameter to null to indicate
* that a calculation error occured
*/
void ApiCalculationRequest_setErrorStatus(ApiCalculationRequest * request){
  if( request == NULL) return;

  if( request->ctlAlt != NULL){
    ApiQuanity_cleanUp((ApiQuantity*)request->ctlAlt);
  }
  request->ctlAlt = NULL;

  if( request->cplAlt != NULL){
    ApiQuanity_cleanUp((ApiQuantity*)request->cplAlt);
  }
  request->cplAlt = NULL;

  if( request->ctplAlt != NULL){
    ApiQuanity_cleanUp((ApiQuantity*)request->ctplAlt);
  }
  request->ctplAlt = NULL;

  if( request->altDens != NULL){
    ApiQuanity_cleanUp((ApiQuantity*)request->altDens);
  }
  request->altDens = NULL;

  if( request->scaledCompAlt != NULL){
    ApiQuanity_cleanUp((ApiQuantity*)request->scaledCompAlt);
  }
  request->scaledCompAlt = NULL;



  if( request->baseDens != NULL){
    ApiQuanity_cleanUp((ApiQuantity*)request->baseDens);
  }
  request->baseDens = NULL;

  if( request->ctlObToBase != NULL){
    ApiQuanity_cleanUp((ApiQuantity*)request->ctlObToBase);
  }
  request->ctlObToBase = NULL;

  if( request->cplObToBase != NULL){
    ApiQuanity_cleanUp((ApiQuantity*)request->cplObToBase);
  }
  request->cplObToBase = NULL;

  if( request->ctplObToBase != NULL){
    ApiQuanity_cleanUp((ApiQuantity*)request->ctplObToBase);
  }
  request->ctplObToBase = NULL;

  if( request->scaledCompOb != NULL){
    ApiQuanity_cleanUp((ApiQuantity*)request->scaledCompOb);
  }
  request->scaledCompOb = NULL;



  if( request->ctlBaseToAlt != NULL){
    ApiQuanity_cleanUp((ApiQuantity*)request->ctlBaseToAlt);
  }
  request->ctlBaseToAlt = NULL;

  if( request->cplBaseToAlt != NULL){
    ApiQuanity_cleanUp((ApiQuantity*)request->cplBaseToAlt);
  }
  request->cplBaseToAlt = NULL;

  if( request->ctplBaseToAlt != NULL){
    ApiQuanity_cleanUp((ApiQuantity*)request->ctplBaseToAlt);
  }
  request->ctplBaseToAlt = NULL;
}

/**
* Append data to a buffer.
*/
int ApiCalculationRequest_appendBuffer(char * buffer,
				       int len,
				       const char * result){
  int errorCode = 0;

  if( buffer != NULL && result != NULL &&
      ( (strlen(buffer) + strlen(result) + 1 < (size_t)len)) ){
    strcat(buffer,result);
  } 
  else errorCode = BUFFER_OVERFLOW;
  return errorCode;
}


/**
* Append a double to a buffer.
*/
int  ApiCalculationRequest_appendDouble(char * buffer,
				  int len,
				  double value){
  int errorCode = 0;

  if( buffer != NULL && (strlen(buffer) + 27 < (size_t)len) ){
    sprintf(buffer + strlen(buffer),"%25.15f",value);
  } 
  else errorCode = BUFFER_OVERFLOW;
 return errorCode;
}


/**
* Set all the paramters in the CalculationRequest 
* passed in. This way we do no need to check whether any of
* the results is null. 
*
* @param calcReq the CalculationRequest to initialize
* 
* @return 0 if all is well or an errorcode otherwise
*/
int ApiCalculationRequest_initializeRequest(ApiCalculationRequest * calcReq){
    int errorCode = 0;

    if( calcReq == NULL){
      errorCode = NULL_POINTER_EXCEPTION;
      return errorCode;
    }

    if( calcReq->obTemp == NULL) {
  	calcReq->obTemp = ApiTemperature_init(API_BASE_TEMP,
					      ApiUnit_Temperature_F(),
					      TRUE,
					      &errorCode);
	if( errorCode  != 0) return errorCode;
    }
    if( calcReq->obPres == NULL){
        calcReq->obPres = ApiPressure_init(API_BASE_PRES,
					   ApiUnit_Pressure_PSI(),
					   TRUE,
					   &errorCode);
	if( errorCode  != 0) return errorCode;
    }
    if( calcReq->obDens == NULL){
      calcReq->obDens = ApiDensity_init(0,
					ApiUnit_Density_KGM3(),
					TRUE,
					&errorCode);
	if( errorCode  != 0) return errorCode;
    }

    if( calcReq->altTemp == NULL) {
        calcReq->altTemp =  ApiTemperature_init(API_BASE_TEMP,
						ApiUnit_Temperature_F(),
						TRUE,
						&errorCode);
	if( errorCode  != 0) return errorCode;
    }
    if( calcReq->altPres == NULL) {
        calcReq->altPres = ApiPressure_init(API_BASE_PRES,
					   ApiUnit_Pressure_PSI(),
					   TRUE,
					   &errorCode);
	if( errorCode  != 0) return errorCode;
    }

    if( calcReq->ctlAlt == NULL){
        calcReq->ctlAlt = ApiExpansionFactor_init(1.0,
						  ApiUnit_Expansion_DIMLESS(),
						  TRUE,
						  &errorCode);
	if( errorCode  != 0) return errorCode;
    }
    if( calcReq->cplAlt == NULL){
        calcReq->cplAlt = ApiExpansionFactor_init(1.0,
						  ApiUnit_Expansion_DIMLESS(),
						  TRUE,
						  &errorCode);
	if( errorCode  != 0) return errorCode;
    }
    if( calcReq->ctplAlt == NULL){
        calcReq->ctplAlt = ApiExpansionFactor_init(1.0,
						  ApiUnit_Expansion_DIMLESS(),
						  TRUE,
						  &errorCode);
	if( errorCode  != 0) return errorCode;
    }
    if( calcReq->altDens == NULL){
        calcReq->altDens = ApiDensity_init(0,
					ApiUnit_Density_KGM3(),
					TRUE,
					&errorCode);
	if( errorCode  != 0) return errorCode;
    }
    if( calcReq->scaledCompAlt == NULL){
        calcReq->scaledCompAlt =  ApiScaledFactor_init(1.0,
						       ApiUnit_ScaledComp_REV_PSI(),
						       TRUE,
						       &errorCode);
	if( errorCode  != 0) return errorCode;
    }


     if( calcReq->baseDens == NULL){
          calcReq->baseDens = ApiDensity_init(0,
					      ApiUnit_Density_KGM3(),
					      TRUE,
					      &errorCode);
	if( errorCode  != 0) return errorCode;
     }
     if( calcReq->ctlObToBase == NULL){
         calcReq->ctlObToBase = ApiExpansionFactor_init(1.0,
							ApiUnit_Expansion_DIMLESS(),
							TRUE,
							&errorCode);
	   if( errorCode  != 0) return errorCode;
     } 
      if( calcReq->cplObToBase == NULL){
         calcReq->cplObToBase = ApiExpansionFactor_init(1.0,
							ApiUnit_Expansion_DIMLESS(),
							TRUE,
							&errorCode);
	   if( errorCode  != 0) return errorCode;
     }
     if( calcReq->ctplObToBase == NULL){
         calcReq->ctplObToBase = ApiExpansionFactor_init(1.0,
							ApiUnit_Expansion_DIMLESS(),
							TRUE,
							&errorCode);
	   if( errorCode  != 0) return errorCode;
     }

     if( calcReq->scaledCompOb == NULL){
          calcReq->scaledCompOb = ApiScaledFactor_init(1.0,
						       ApiUnit_ScaledComp_REV_PSI(),
						       TRUE,
						       &errorCode);
	  if( errorCode  != 0) return errorCode;
     }
     if( calcReq->cplBaseToAlt == NULL){
         calcReq->cplBaseToAlt =  ApiExpansionFactor_init(1.0,
							  ApiUnit_Expansion_DIMLESS(),
							  TRUE,
							  &errorCode);
	 if( errorCode  != 0) return errorCode;
     }
     if( calcReq->ctlBaseToAlt == NULL){
         calcReq->ctlBaseToAlt =  ApiExpansionFactor_init(1.0,
							  ApiUnit_Expansion_DIMLESS(),
							  TRUE,
							  &errorCode);
	 if( errorCode  != 0) return errorCode;
     }
     if( calcReq->ctplBaseToAlt == NULL){
         calcReq->ctplBaseToAlt =  ApiExpansionFactor_init(1.0,
							  ApiUnit_Expansion_DIMLESS(),
							  TRUE,
							  &errorCode);
	 if( errorCode  != 0) return errorCode;
     }

     return errorCode;
};


/** 
* If the volume is given, update the values for the volume.
* The updated volumes are all calculated using the rounded(!)
* value of the ctpl factor 
*
* @param calcReq the CalculationRequest containing the
*                calculated data
* @return 0 or an error code if an error occured
* 
*/
int ApiCalculationRequest_getVolumeData(ApiCalculationRequest *calcReq){

  int errorCode = 0;
  ApiUnit * vu;
  double rounded,value;

  if( calcReq == NULL){
    errorCode = NULL_POINTER_EXCEPTION;
    return errorCode;
  }
  
  /**  the observed volume is supplied */
  if(calcReq->obVol != NULL ){ 
     vu =  ApiQuantity_givenUnit( (ApiQuantity*)calcReq->obVol, &errorCode); 
     if( errorCode != 0) return errorCode;

     /** Get the observed volume */
     value = ApiQuantity_getValue((ApiQuantity*)calcReq->obVol,
				   vu,
				   FALSE,
				  &errorCode);
     if( errorCode != 0) return errorCode;

     /** Calculate and set the base volume */
     rounded = ApiQuantity_getValue((ApiQuantity*)calcReq->ctplObToBase,
				    ApiUnit_Expansion_DIMLESS(),
				    TRUE,
				    &errorCode);
     if( errorCode != 0) return errorCode;
     value *= rounded;
     calcReq->baseVol = ApiVolume_init(value, vu, TRUE, &errorCode);
     if( errorCode != 0) return errorCode;

     /** Calculate and set the alternate volume */
     rounded = ApiQuantity_getValue((ApiQuantity*)calcReq->ctplBaseToAlt,
				    ApiUnit_Expansion_DIMLESS(),
				    TRUE,
				    &errorCode);
     if( errorCode != 0) return errorCode;
     value /= rounded;
     calcReq->altVol = ApiVolume_init(value, vu, TRUE, &errorCode);
     if( errorCode != 0) return errorCode;
  }


  /**  The base volume is supplied */
  else if(calcReq->baseVol != NULL ){
     vu =  ApiQuantity_givenUnit( (ApiQuantity*)calcReq->baseVol, &errorCode); 
     if( errorCode != 0) return errorCode;

     /** Get the base volume */
     value = ApiQuantity_getValue((ApiQuantity*)calcReq->baseVol,
				   vu,
				   FALSE,
				  &errorCode);
     if( errorCode != 0) return errorCode;

     /** Calculate and set the alternate volume */
     rounded = ApiQuantity_getValue((ApiQuantity*)calcReq->ctplBaseToAlt,
				    ApiUnit_Expansion_DIMLESS(),
				    TRUE,
				    &errorCode);
     if( errorCode != 0) return errorCode;
     value /= rounded;
     calcReq->altVol = ApiVolume_init(value, vu, TRUE, &errorCode);
     if( errorCode != 0) return errorCode;

     /** Calculate and set the observed volume */
     rounded = ApiQuantity_getValue((ApiQuantity*)calcReq->ctplObToBase,
				    ApiUnit_Expansion_DIMLESS(),
				    TRUE,
				    &errorCode);
     if( errorCode != 0) return errorCode;
     value /= rounded;
     calcReq->obVol = ApiVolume_init(value, vu, TRUE, &errorCode);
     if( errorCode != 0) return errorCode;
  }
    
  /** The alternate volume is supplied */
  else if(calcReq->altVol != NULL ){
      vu =  ApiQuantity_givenUnit( (ApiQuantity*)calcReq->altVol, &errorCode); 
     if( errorCode != 0) return errorCode;

     /** Get the alternate volume */
     value = ApiQuantity_getValue((ApiQuantity*)calcReq->altVol,
				   vu,
				   FALSE,
				  &errorCode);
     if( errorCode != 0) return errorCode;

     /** Calculate and set the base volume */
     rounded = ApiQuantity_getValue((ApiQuantity*)calcReq->ctplBaseToAlt,
				    ApiUnit_Expansion_DIMLESS(),
				    TRUE,
				    &errorCode);
     if( errorCode != 0) return errorCode;
     value *= rounded;
     calcReq->baseVol = ApiVolume_init(value, vu, TRUE, &errorCode);
     if( errorCode != 0) return errorCode;

     /** Calculate and set the observed volume */
     rounded = ApiQuantity_getValue((ApiQuantity*)calcReq->ctplObToBase,
				    ApiUnit_Expansion_DIMLESS(),
				    TRUE,
				    &errorCode);
     if( errorCode != 0) return errorCode;
     value /= rounded;
     calcReq->obVol = ApiVolume_init(value, vu, TRUE, &errorCode);
     if( errorCode != 0) return errorCode;
  }

  return errorCode;
};

/**
* Report an string and an accompanying  string value
* 
* @param calcReq the structure that contains the buffer in which to write the result
* @param isIntermediate is this an intermediate result
* @param result a descriptive string
* @param value a double value to report
*/
void ApiCalculationRequest_reportQuantityValue(ApiCalculationRequest *calcReq,
					       bool isIntermediate,
					       char * result,
					       double value){
   size_t len,max;

   if( calcReq->intermediate == NULL) return;
   if( isIntermediate == TRUE && calcReq->reportIntermediate == FALSE) return;

   len  = strlen(calcReq->intermediate);
   ApiCalculationRequest_appendBuffer(calcReq->intermediate, 
				      calcReq->intLength,
				      result);
   max = len + 40;
   while( (size_t)calcReq->intLength - 1  >  max &&
          strlen(calcReq->intermediate) < (size_t)max ){
       ApiCalculationRequest_appendBuffer(calcReq->intermediate,
					  calcReq->intLength,
					  ".");
   }
   ApiCalculationRequest_appendDouble(calcReq->intermediate, 
				      calcReq->intLength,
				      value);
   ApiCalculationRequest_appendBuffer(calcReq->intermediate,
				      calcReq->intLength,
				      API_EOL);
};



/** 
* Report a quantity  value 
*/
void ApiCalculationRequest_reportQuantity(ApiCalculationRequest *calcReq,
					  bool isIntermediate,
					  int name,
					  ApiQuantity * value,
					  bool rounded,
					  ApiUnit * unit){
  int errorCode = 0;
  size_t len,max;
  double result;
  ApiUnit * u = unit;

  if( calcReq->intermediate == NULL || value == NULL) return;
  if( isIntermediate == TRUE && calcReq->reportIntermediate == FALSE) return;

  if( u == NULL) {
    u = ApiQuantity_givenUnit( value, &errorCode);
    if( errorCode != 0) return;
  }
  result = ApiQuantity_getValue(value, u, rounded, &errorCode);
  if(  errorCode != 0) return;
 
  len  = strlen(calcReq->intermediate);
  if( calcReq->getParameterName != NULL){
    calcReq->getParameterName(name, calcReq->intermediate,calcReq->intLength);   
  }
  if( rounded ) ApiCalculationRequest_appendBuffer(calcReq->intermediate,
						   calcReq->intLength,
						   " (rounded)" );  
  max = len + 40;
  while( (size_t)calcReq->intLength - 1  > max &&
         strlen(calcReq->intermediate) < (size_t)max ){
       ApiCalculationRequest_appendBuffer(calcReq->intermediate,
					  calcReq->intLength,
					  ".");
   }
   ApiCalculationRequest_appendDouble(calcReq->intermediate, 
				      calcReq->intLength,
				      result);
  ApiUnit_unitName(u, calcReq->intermediate, calcReq->intLength);
  ApiCalculationRequest_appendBuffer(calcReq->intermediate,
				      calcReq->intLength,
				      API_EOL);
}



/**
* Report the input data 
*
* @param calcReq the structure that contains the buffer in which to write the result
*/
void ApiCalculationRequest_reportInputData(ApiCalculationRequest *calcReq){
  if( calcReq->intermediate == NULL) return;

   ApiCalculationRequest_appendBuffer(calcReq->intermediate,
				      calcReq->intLength,
				      "Input Data");
   ApiCalculationRequest_appendBuffer(calcReq->intermediate,
				      calcReq->intLength,
				      API_EOL);

      
   if( calcReq->commodityName != API_COMMODITY_NOT_GIVEN ){
      if( calcReq->getParameterName != NULL){
         calcReq->getParameterName(API_COMMODITY_NAME, 
				   calcReq->intermediate,
				   calcReq->intLength);
       }
       ApiCalculationRequest_appendBuffer(calcReq->intermediate,
					  calcReq->intLength,
					  " : ");
       if( calcReq->getOilProductName != NULL){
         calcReq->getOilProductName(calcReq->commodityName, 
				    calcReq->intermediate,
				    calcReq->intLength);
       }
       else if( calcReq->intermediate != NULL){
          ApiCalculationRequest_appendDouble(calcReq->intermediate,
					     calcReq->intLength,
					     calcReq->commodityName);
       }
       ApiCalculationRequest_appendBuffer(calcReq->intermediate,
					  calcReq->intLength,
					  API_EOL);
   }
   if( calcReq->alpha60 != NULL){
      ApiCalculationRequest_reportQuantity(calcReq,
					   FALSE,
					   API_ALPHA_60_NAME,
					   (ApiQuantity*)calcReq->alpha60,
					   FALSE,
					   NULL);
   }
   if( calcReq->obTemp != NULL){
      ApiCalculationRequest_reportQuantity(calcReq,
					   FALSE,
					   API_OB_TEMP_NAME,
					   (ApiQuantity*)calcReq->obTemp,
					   FALSE,
					   NULL);
   }
   if( calcReq->obPres != NULL){
      ApiCalculationRequest_reportQuantity(calcReq,
					   FALSE,
					   API_OB_PRES_NAME,
					   (ApiQuantity*)calcReq->obPres,
					   FALSE,
					   NULL);
   }
   if( calcReq->obDens != NULL){
      ApiCalculationRequest_reportQuantity(calcReq,
					   FALSE,
					   API_OB_DENS_NAME,
					   (ApiQuantity*)calcReq->obDens,
					   FALSE,
					   NULL);
   }
   if( calcReq->altTemp != NULL){
      ApiCalculationRequest_reportQuantity(calcReq,
					   FALSE,
					   API_ALT_TEMP_NAME,
					   (ApiQuantity*)calcReq->altTemp,
					   FALSE,
					   NULL);
   }
   if( calcReq->altPres != NULL){
      ApiCalculationRequest_reportQuantity(calcReq,
					   FALSE,
					   API_ALT_PRES_NAME,
					   (ApiQuantity*)calcReq->altPres,
					   FALSE,
					   NULL);
   }

   if( calcReq->altVol != NULL){
      ApiCalculationRequest_reportQuantity(calcReq,
					   FALSE,
					   API_ALT_VOL_NAME,
					   (ApiQuantity*)calcReq->altVol,
					   FALSE,
					   NULL);
   }
   if( calcReq->baseVol != NULL){
      ApiCalculationRequest_reportQuantity(calcReq,
					   FALSE,
					   API_BASE_VOL_NAME,
					   (ApiQuantity*)calcReq->baseVol,
					   FALSE,
					   NULL);
   }
   if( calcReq->obVol != NULL){
      ApiCalculationRequest_reportQuantity(calcReq,
					   FALSE,
					   API_OB_VOL_NAME,
					   (ApiQuantity*)calcReq->obVol,
					   FALSE,
					   NULL);
   }
};


/**
* Report the output data 
*
* @param calcReq the structure that contains the buffer in which to write the result
*/
void ApiCalculationRequest_reportOutputData(ApiCalculationRequest *calcReq){
  ApiUnit * u;
  int errorCode = 0;

  if( calcReq->intermediate == NULL) return;

   ApiCalculationRequest_appendBuffer(calcReq->intermediate,
				      calcReq->intLength,
				      "Output Data");
   ApiCalculationRequest_appendBuffer(calcReq->intermediate,
				      calcReq->intLength,
				      API_EOL);

   ApiCalculationRequest_appendBuffer(calcReq->intermediate,
				      calcReq->intLength,
				      "Base Values");
   ApiCalculationRequest_appendBuffer(calcReq->intermediate,
				      calcReq->intLength,
				      API_EOL);
   if( calcReq->baseDens != NULL){
     u = ApiQuantity_givenUnit( (ApiQuantity*)calcReq->obDens, &errorCode);
     if( errorCode != 0) u = NULL;
     ApiCalculationRequest_reportQuantity(calcReq,
					  FALSE,
					  API_BASE_DENS_NAME,
					  (ApiQuantity*)calcReq->baseDens,
					  FALSE,
					  u);
   }
   if( calcReq->ctlObToBase != NULL){ 
     ApiCalculationRequest_reportQuantity(calcReq,
					  FALSE,
					  API_CTL_OB_TO_BASE_NAME,
					  (ApiQuantity*)calcReq->ctlObToBase,
					  FALSE,
					  NULL);
 
   }
   if( calcReq->scaledCompOb != NULL){
       ApiCalculationRequest_reportQuantity(calcReq,
					    FALSE,
					    API_SCALED_COMP_OB_NAME,
					    (ApiQuantity*)calcReq->scaledCompOb,
					    FALSE,
					    NULL);
   }
   if( calcReq->cplObToBase != NULL){ 
     ApiCalculationRequest_reportQuantity(calcReq,
					  FALSE,
					  API_CPL_OB_TO_BASE_NAME,
					  (ApiQuantity*)calcReq->cplObToBase,
					  FALSE,
					  NULL);
 
   }
   if( calcReq->ctplObToBase != NULL){ 
     ApiCalculationRequest_reportQuantity(calcReq,
					  FALSE,
					  API_CTPL_OB_TO_BASE_NAME,
					  (ApiQuantity*)calcReq->ctplObToBase,
					  FALSE,
					  NULL);
     ApiCalculationRequest_reportQuantity(calcReq,
					  FALSE,
					  API_CTPL_OB_TO_BASE_NAME,
					  (ApiQuantity*)calcReq->ctplObToBase,
					  TRUE,
					  NULL);
 
   }
   if( calcReq->baseVol != NULL){
      ApiCalculationRequest_reportQuantity(calcReq,
					   FALSE,
					   API_BASE_VOL_NAME,
					   (ApiQuantity*)calcReq->baseVol,
					   FALSE,
					   NULL);
   }



   ApiCalculationRequest_appendBuffer(calcReq->intermediate,
				      calcReq->intLength,
				      "Alternate Values");
   ApiCalculationRequest_appendBuffer(calcReq->intermediate,
				      calcReq->intLength,
				      API_EOL);
   if( calcReq->altDens != NULL){
     u = ApiQuantity_givenUnit( (ApiQuantity*)calcReq->obDens, &errorCode);
     if( errorCode != 0) u = NULL;
     ApiCalculationRequest_reportQuantity(calcReq,
					  FALSE,
					  API_ALT_DENS_NAME,
					  (ApiQuantity*)calcReq->altDens,
					  FALSE,
					  u);
   }
   if( calcReq->ctlBaseToAlt != NULL){ 
     ApiCalculationRequest_reportQuantity(calcReq,
					  FALSE,
					  API_CTL_BASE_TO_ALT_NAME,
					  (ApiQuantity*)calcReq->ctlBaseToAlt,
					  FALSE,
					  NULL);
 
   }
   if( calcReq->scaledCompAlt != NULL){
       ApiCalculationRequest_reportQuantity(calcReq,
					    FALSE,
					    API_SCALED_COMP_ALT_NAME,
					    (ApiQuantity*)calcReq->scaledCompAlt,
					    FALSE,
					    NULL);
   }
   if( calcReq->cplBaseToAlt != NULL){ 
     ApiCalculationRequest_reportQuantity(calcReq,
					  FALSE,
					  API_CPL_BASE_TO_ALT_NAME,
					  (ApiQuantity*)calcReq->cplBaseToAlt,
					  FALSE,
					  NULL);
 
   }
   if( calcReq->ctplBaseToAlt != NULL){ 
     ApiCalculationRequest_reportQuantity(calcReq,
					  FALSE,
					  API_CTPL_BASE_TO_ALT_NAME,
					  (ApiQuantity*)calcReq->ctplBaseToAlt,
					  FALSE,
					  NULL);
     ApiCalculationRequest_reportQuantity(calcReq,
					  FALSE,
					  API_CTPL_BASE_TO_ALT_NAME,
					  (ApiQuantity*)calcReq->ctplBaseToAlt,
					  TRUE,
					  NULL);
 
   }
   if( calcReq->altVol != NULL){
       ApiCalculationRequest_reportQuantity(calcReq,
					    FALSE,
					   API_ALT_VOL_NAME,
					   (ApiQuantity*)calcReq->altVol,
					   FALSE,
					   NULL);
   }
   if( calcReq->obVol != NULL){
      ApiCalculationRequest_reportQuantity(calcReq,
					   FALSE,
					   API_OB_VOL_NAME,
					   (ApiQuantity*)calcReq->obVol,
					   FALSE,
					   NULL);
   }
};

/**
* Check that all input values are within range.
* <p>
* We check that either calcReq->commodity commodity or
* calcReq->alpha60  is supplied and that all 
* input parameters are set.
* <p>
* Next we check that if the commodity is supplied it is within in range.
* For this the range data in Paragraph 11.1.6.1, p. 33 are used, if the
* observed conditions are equal to the base conditions, i.e. 60 F and
* 0 psi. Otherwise the  range data in Paragraph 11.1.6.2, p 50 are used.
* <p>
* Then  we check that the pressure is in the range 0 -  1500 psi as
* given in Paragraph 11.1.6.1, p. 33. Negative pressure values are 
* automatically set to 0 psi.
* <p>
* Temperature values have to be in the range -58 F and 302 F, according to
* Paragraph 11.1.6.1, p. 33
* 
* @param calcReq the structure that contains the data to check
* @param errorCode a variable to report any errors
* @return TRUE if all data pass the test, FALSE otherwise
*/
bool  ApiCalculationRequest_checkLimits(ApiCalculationRequest *calcReq,
					int * errorCode){
  bool tmp;
  int c1,c2;

  *errorCode  = 0;

   /** Check that either commodity or alpha60 is supplied **/
   /** Check that they are not both non-null  */
   if( calcReq->commodityName != API_COMMODITY_NOT_GIVEN  && 
       calcReq->alpha60 != NULL){
      *errorCode = COMMODITY_AND_ALPHA60_SUPPLIED;
      return FALSE;
   }
   /**  At least one is supplied */
   if( calcReq->commodityName == API_COMMODITY_NOT_GIVEN  && 
       calcReq->alpha60 == NULL){
     *errorCode = COMMODITY_AND_ALPHA60_NULL;
      return FALSE;
   }
   /**  If alpha60 is supplied, make sure it is in range */
   if( calcReq->alpha60 != NULL){
      tmp = ApiQuantity_inAllowedRange( (ApiQuantity*)calcReq->alpha60, errorCode);
      if( tmp == FALSE || *errorCode != 0){
  	 *errorCode = ALPHA60_OUT_OF_RANGE;
	 return FALSE;
      }
   }


   /**  Ensure that an observed density is given */ 
   if(  calcReq->obDens == NULL){
      *errorCode = OBSERVED_DENSITY_VALUE_MISSING;
      return FALSE;
   }
   /**  Check that the  commodity values are correct */
   if( CALC_REQ_COMMODITY  != NULL ){
     c1 = ApiQuantity_compareTo( (ApiQuantity*)CALC_REQ_BASE_TEMP, 
				 (ApiQuantity*)calcReq->obTemp,
				 errorCode);
     if( *errorCode != 0) return FALSE;
     c2 = ApiQuantity_compareTo( (ApiQuantity*)CALC_REQ_BASE_PRES, 
				 (ApiQuantity*)calcReq->obPres,
				 errorCode);
     if( *errorCode != 0) return FALSE;
     if( c1 == 0 && c2 == 0){     /* Type I calculation */
       tmp =  ApiDensity_inRhoRange(calcReq->obDens, CALC_REQ_COMMODITY, FALSE, errorCode);
       if( tmp == FALSE || *errorCode != 0){
 	   *errorCode = OBSERVED_DENSITY_OUT_OF_RANGE_TYPE_I;
	   return FALSE;
       }
     }
     else{
       tmp =  ApiDensity_inRhoRange(calcReq->obDens, CALC_REQ_COMMODITY, TRUE, errorCode);
       if( tmp == FALSE || *errorCode != 0){
 	   *errorCode = OBSERVED_DENSITY_OUT_OF_RANGE_TYPE_II;
	   return FALSE;
       }
     }
   }
     
   /** Check that temperature and pressure are in range */
   /**  Observed and alternate pressure are in range */
   if(  calcReq->obPres == NULL){
        *errorCode = OBSERVED_PRESSURE_VALUE_MISSING;
	return FALSE;
   }
   else{
        tmp = ApiQuantity_inAllowedRange( (ApiQuantity*)calcReq->obPres, errorCode);
	if( tmp == FALSE || *errorCode != 0){
       	    *errorCode = OBSERVED_PRESSURE_OUT_OF_RANGE;
	    return FALSE;
	}
   }
   if(  calcReq->altPres == NULL){
        *errorCode = ALTERNATE_PRESSURE_VALUE_MISSING;
	return FALSE;
   }
   else{
        tmp = ApiQuantity_inAllowedRange( (ApiQuantity*)calcReq->altPres, errorCode);
	if( tmp == FALSE || *errorCode != 0){
       	    *errorCode = ALTERNATE_PRESSURE_OUT_OF_RANGE;
	    return FALSE;
	}
   }



   /**  Observed and alternate temperature are in range */
   if(  calcReq->obTemp == NULL){
        *errorCode = OBSERVED_TEMPERATURE_VALUE_MISSING;
	return FALSE;
   }
   else{
        tmp = ApiQuantity_inAllowedRange( (ApiQuantity*)calcReq->obTemp, errorCode);
	if( tmp == FALSE || *errorCode != 0){
       	    *errorCode = OBSERVED_TEMPERATURE_OUT_OF_RANGE;;
	    return FALSE;
	}
   }
   if(  calcReq->altTemp == NULL){
        *errorCode = ALTERNATE_TEMPERATURE_VALUE_MISSING;
	return FALSE;
   }
   else{
        tmp = ApiQuantity_inAllowedRange( (ApiQuantity*)calcReq->altTemp, errorCode);
	if( tmp == FALSE || *errorCode != 0){
       	    *errorCode = ALTERNATE_TEMPERATURE_OUT_OF_RANGE;
	    return FALSE;
	}
   }

   /*  Make sure only one of the volumes is supplied */
   c1 = 0;
   if( calcReq->obVol != NULL) c1++;
   if( calcReq->altVol != NULL) c1++;
   if( calcReq->baseVol != NULL) c1++;
   if( c1 > 1){
       *errorCode = MORE_THAN_ONE_VOLUME_SUPPLIED;
       return FALSE;
   }

   return TRUE;
}


/**
* Do the calculations in subsection 11.1.6.1
*
* @param calcReq the structure that contains the data for which to calculate
* @param substep is this calculation a substep in
*                one of the other subsection, or 
*                is its output used directly 
* @return 0 if all is well,  an error code otherwise
*/
int ApiCalculationRequest_transformBaseToDesired(ApiCalculationRequest *calcReq,
						  bool substep){
   int  errorCode = 0, c1,c2;
   double rhostar = 0;
   double alpha60 = 0;
   double rho60;
   double k[3];
   double tstar = 0;
   double deltat,ctl,cpl,ctpl,fp, density;
   double fpStep6[4] =  API_FP_STEP_6_FACTOR;


   if( substep == FALSE){    /** Set the result values that are identical to the 
                                 input values */
      c1 = ApiQuantity_compareTo( (ApiQuantity*)CALC_REQ_BASE_TEMP, 
 				  (ApiQuantity*)calcReq->altTemp,
				   &errorCode);
     if( errorCode != 0) return errorCode;
     c2 = ApiQuantity_compareTo( (ApiQuantity*)CALC_REQ_BASE_PRES, 
				 (ApiQuantity*)calcReq->altPres,
				  &errorCode);
     if( errorCode != 0) return errorCode;
     if( c1 == 0 &&  c2 == 0){     
       /** Set the alternate density to the base density */
       errorCode = ApiQuantity_setValueFromParent((ApiQuantity*)calcReq->altDens,
						  (ApiQuantity*)calcReq->baseDens);
       if( errorCode != 0) return errorCode;

       /** Set base to alternate expansion factors to 1.0 */
       errorCode = ApiQuantity_setValue((ApiQuantity*)calcReq->ctlBaseToAlt, 
					1.0,
					ApiUnit_Expansion_DIMLESS());
       if( errorCode != 0) return errorCode;
       errorCode = ApiQuantity_setValue((ApiQuantity*)calcReq->cplBaseToAlt, 
					1.0,
					ApiUnit_Expansion_DIMLESS());
       if( errorCode != 0) return errorCode;
       errorCode = ApiQuantity_setValue((ApiQuantity*)calcReq->ctplBaseToAlt, 
					1.0,
					ApiUnit_Expansion_DIMLESS());
       if( errorCode != 0) return errorCode;
       errorCode  = ApiQuantity_setValueFromParent((ApiQuantity*)calcReq->scaledCompAlt,
						   (ApiQuantity*)calcReq->scaledCompOb);
       return errorCode;
     }
   }

     /** Get the base density value and determine the current commodity to use */
     rho60 =  ApiQuantity_getValue((ApiQuantity*)calcReq->baseDens,
				   ApiUnit_Density_KGM3(),
				   FALSE,
				   &errorCode);
     if( errorCode != 0) return errorCode;
     if( CALC_REQ_COMMODITY != NULL && CALC_REQ_DET_COM == NULL){
         CALC_REQ_DET_COM = ApiOilProduct_getProductByDensity( calcReq->commodityName,
							       calcReq->obDens,
							       &errorCode);
	 if( errorCode != 0) return errorCode;
     }

     /**  Step 2:  shift temperature to IPTS-68 basis */
     if( substep == TRUE){     /** Only shift the temperature if not already shifted */
       bool result;
       result = ApiTemperature_wasIPTS68Corrected(calcReq->obTemp, &errorCode);
       if( errorCode != 0) return errorCode;
       if( result == FALSE){
           ApiTemperature_convertToIPTS68(calcReq->obTemp, &errorCode);
           if( errorCode != 0) return errorCode;
       }
       tstar = ApiQuantity_getValue((ApiQuantity*)calcReq->obTemp,
				     ApiUnit_Temperature_F(),
				     FALSE,
				     &errorCode);
       if( errorCode != 0) return errorCode;
     }
     else{
       bool result;
       result = ApiTemperature_wasIPTS68Corrected(calcReq->altTemp, &errorCode);
       if( errorCode != 0) return errorCode;
       if( result == FALSE){
           ApiTemperature_convertToIPTS68(calcReq->altTemp, &errorCode);
           if( errorCode != 0) return errorCode;
       }
       tstar = ApiQuantity_getValue((ApiQuantity*)calcReq->altTemp,
				     ApiUnit_Temperature_F(),
				     FALSE,
				     &errorCode);
       if( errorCode != 0) return errorCode;
       ApiCalculationRequest_reportQuantityValue(calcReq,
						 TRUE,
						 "t corrected to IPTS-68 (in F)",
						 tstar);
     }

     /**  Step 3: Shift input rho-60 to IPTS-68 basis */
     if( calcReq->alpha60 != NULL){
        alpha60 = ApiQuantity_getValue((ApiQuantity*)calcReq->alpha60,
				       ApiUnit_ThermalExp_REV_F(),
				       FALSE,
				       &errorCode);
	if( errorCode != 0) return errorCode;
	rhostar = 1.0 + 0.4 * alpha60 * API_DELTA_60;
	rhostar *= 0.5 * alpha60 * API_DELTA_60; 
	rhostar = rho60 * exp(rhostar);
	ApiCalculationRequest_reportQuantityValue(calcReq,TRUE,"Rho60*",rhostar);
     }
     else{
        double a,b;

        errorCode =  ApiOilProduct_getKValues(CALC_REQ_DET_COM, k);
        if( errorCode != 0) return errorCode;

	ApiCalculationRequest_reportQuantityValue(calcReq,TRUE,"K0",k[0]);
	ApiCalculationRequest_reportQuantityValue(calcReq,TRUE,"K1",k[1]);
	ApiCalculationRequest_reportQuantityValue(calcReq,TRUE,"K2",k[2]);
	a = k[1] + k[0]/rho60;
	a /= rho60;
	a += k[2];
	a *= API_DELTA_60/2.0;
	ApiCalculationRequest_reportQuantityValue(calcReq,TRUE,"A",a);

	b = k[0] + (k[1] + k[2] * rho60) * rho60;
	b = ( 2.0 * k[0] + k[1] * rho60) / b;
	ApiCalculationRequest_reportQuantityValue(calcReq,TRUE,"B",b);

	rhostar = a * ( 1.0 + 0.8 * a);
	rhostar = exp(rhostar) - 1.0;
	rhostar /= 1.0 + a * ( 1.0 + 1.6 * a) * b;
	rhostar = rho60 * ( 1.0 + rhostar);
	ApiCalculationRequest_reportQuantityValue(calcReq,TRUE,"Rho60*",rhostar);
     }
   
     /** Step 4: Calculate alpha-60 if not supplied */
     if(  calcReq->alpha60 == NULL){    
       alpha60 = k[0]/rhostar + k[1];
       alpha60 /= rhostar;
       alpha60 += k[2];
       if( calcReq->alpha60 != NULL){
	   errorCode = ApiQuantity_setValue((ApiQuantity*)calcReq->alpha60,
		  			    alpha60,
					    ApiUnit_ThermalExp_REV_F());
	   if( errorCode != 0) return errorCode;
       }
       else{
          calcReq->alpha60 = ApiAlpha60_init(alpha60,
					     ApiUnit_ThermalExp_REV_F(),
					     TRUE,
					     &errorCode);
	  if( errorCode != 0) return errorCode;
       }
       ApiCalculationRequest_reportQuantity(calcReq,
					    TRUE,
					    API_ALPHA_60_NAME,
					    (ApiQuantity*)calcReq->alpha60,
					    FALSE,
					    ApiUnit_ThermalExp_REV_F());
     }
        
     /**  Step 5: Calculcate the CTL */
     deltat = tstar - API_IPTS_68_BASE;
     ApiCalculationRequest_reportQuantityValue(calcReq,TRUE,"delta t",deltat);
     ctl  = deltat +  API_DELTA_60;
     ctl = 1.0 +  0.8 * alpha60 * ctl;    
     ctl = alpha60 * deltat * ctl;
     ctl = exp(-ctl);
     errorCode = ApiQuantity_setValue((ApiQuantity*)calcReq->ctlBaseToAlt,
				      ctl,
				      ApiUnit_Expansion_DIMLESS());
     if( errorCode != 0) return errorCode;
     ApiCalculationRequest_reportQuantity(calcReq,
					  TRUE,
					  API_CTL_BASE_TO_ALT_NAME,
					  (ApiQuantity*)calcReq->ctlBaseToAlt,
					  FALSE,
					  ApiUnit_Expansion_DIMLESS());

     /** Step 6: Calculate the scale compressibility factor */
    fp = fpStep6[2] + fpStep6[3] * tstar; 
    fp /= rhostar * rhostar;
    fp += fpStep6[1] * tstar;
    fp += fpStep6[0];
    fp = exp(fp);
    errorCode = ApiQuantity_setValue((ApiQuantity*)calcReq->scaledCompAlt,
				      fp,
				      ApiUnit_ScaledComp_REV_PSI());
    if( errorCode != 0) return errorCode;
    ApiCalculationRequest_reportQuantity(calcReq,
					 TRUE,
					 API_SCALED_COMP_ALT_NAME,
					 (ApiQuantity*)calcReq->scaledCompAlt,
					 FALSE,
					 ApiUnit_ScaledComp_REV_PSI());

   
    /**  Step 7: Calculate the CPL factor */
    if( substep == FALSE){
         cpl =  ApiQuantity_getValue((ApiQuantity*)calcReq->altPres,
				     ApiUnit_Pressure_PSI(),
				     FALSE,
				     &errorCode);
    }
    else{
         cpl =  ApiQuantity_getValue((ApiQuantity*)calcReq->obPres,
				     ApiUnit_Pressure_PSI(),
				     FALSE,
				     &errorCode);
    }
    if( errorCode != 0) return errorCode;
    cpl = 1.0 - 1e-5 * fp * cpl;
    cpl = 1.0 / cpl;
    errorCode = ApiQuantity_setValue((ApiQuantity*)calcReq->cplBaseToAlt,
				      cpl,
				      ApiUnit_Expansion_DIMLESS());
    if( errorCode != 0) return errorCode;
    ApiCalculationRequest_reportQuantity(calcReq,
					 TRUE,
					 API_CPL_BASE_TO_ALT_NAME,
					 (ApiQuantity*)calcReq->cplBaseToAlt,
					 FALSE,
					 ApiUnit_Expansion_DIMLESS());
     
    /** Step 8: Calculate CTPL */
    ctpl = cpl * ctl;
    errorCode = ApiQuantity_setValue((ApiQuantity*)calcReq->ctplBaseToAlt,
				      ctpl,
				      ApiUnit_Expansion_DIMLESS());
    if( errorCode != 0) return errorCode;
    ApiCalculationRequest_reportQuantity(calcReq,
					 TRUE,
					 API_CTPL_BASE_TO_ALT_NAME,
					 (ApiQuantity*)calcReq->ctplBaseToAlt,
					 FALSE,
					 ApiUnit_Expansion_DIMLESS());
 
    /**  Step 9: Calculate density and volume */
    density = ctl * cpl * rho60;
    errorCode = ApiQuantity_setValue((ApiQuantity*)calcReq->altDens,
				      density,
				      ApiUnit_Density_KGM3());
    if( errorCode != 0) return errorCode;
    ApiCalculationRequest_reportQuantity(calcReq,
					 TRUE,
					 API_ALT_DENS_NAME,
					 (ApiQuantity*)calcReq->altDens,
					 FALSE,
					 ApiUnit_Density_KGM3());
    return errorCode;
};


/**
* This function is called if conversion is not reached.
* It checks whether the note of step 4 in the 11.1.6.2 calculation applies.
* If it applies, an errorCode of 0 is returned and convergence is assumed to
* have been reached. A suitable comment is put into calcReq to indicate that
* a special convergence case has been encountered. Otherwise desiredError is
* returned as the errorCode.
* 
* @param calcReq the ApiCalculationRequest to check for special 
*                convergence cases
* @param desiredError the errorCode that should be given if this is not a
*                     special exception from the failed convergence
*
* @return 0 if this is a special exception from the failed convergence,
*        desiredError otherwise
*/
int ApiCalculationRequest_checkConversion( ApiCalculationRequest *calcReq,
					   int desiredError){
  int errorCode = desiredError;

  /* Check whether we are calculation for Refined products */
  if( calcReq->commodityName == API_REFINED_PRODUCTS_NAME){
    double limits[] = API_TRANSITION_ZONE_DENSITY_LIMITS;
    double value;
    int error;
    
    /* Get the current base value */
     value = ApiQuantity_getValue( (ApiQuantity*)calcReq->baseDens,
	 			  ApiUnit_Density_KGM3(),
				  FALSE,
				  &error);
    if(error != 0) return errorCode;

    /* Compare whether we are close to the limits of the Transition Zone */
    if( fabs(value - limits[0]) < API_CONVERSION_NOTE_11161_CUTOFF ||
        fabs(value - limits[1]) < API_CONVERSION_NOTE_11161_CUTOFF){
       int length = sizeof(char) * strlen(API_CONVERGENCE_NOTE_11162)  + 1;

       /* Set the comment to indicate a special convergence case */
       if( calcReq->comments == NULL){
	 calcReq->comments =(char*)malloc(length);
	 if( calcReq->comments == NULL) return errorCode;
	 calcReq->commentLength = length;
       }
       else if( calcReq->commentLength < length){
	 calcReq->comments =(char*)realloc(calcReq->comments,length);
	 if( calcReq->comments == NULL) return errorCode;
	 calcReq->commentLength = length;
       } 
       calcReq->comments[0] = '\0';
       strcat(calcReq->comments,API_CONVERGENCE_NOTE_11162);

       /* Return 0 to indicate that we have an exception to convergence failure */
       errorCode = 0;
    }

  }

  /* No exception, convergence failed, return the desired errorCode */
  return errorCode;
};


/**
* Transforms the values to base condition, i.e
* run  procedure 11.1.6.2 if the observed  conditions are not
* equal to the base conditions. We check first that this is the 
* case.
*
* @param calcReq the structure that contains the data for which to calculate
* @return 0 if all is well,  an error code otherwise
*/
int ApiCalculationRequest_transformToBaseCondition( ApiCalculationRequest *calcReq){
  int c1, c2;
  int errorCode = 0;
  int m = 0;
  double em,dtm,dpm,deltaT,deltaR;
  double rho60;
  double unshiftedT0;
  bool alphaSupplied = FALSE;
  ApiDensity ** limits;
  double value;
  bool tmp;
  double dpStepfactor[2] = API_DP_STEP_5_FACTOR;


   c1 = ApiQuantity_compareTo( (ApiQuantity*)CALC_REQ_BASE_TEMP, 
			       (ApiQuantity*)calcReq->obTemp,
			       &errorCode);
   if( errorCode != 0) return errorCode;
   c2 = ApiQuantity_compareTo( (ApiQuantity*)CALC_REQ_BASE_PRES,
			       (ApiQuantity*)calcReq->obPres,
			       &errorCode);
   if( errorCode != 0) return errorCode;
   if( c1 == 0 &&  c2 == 0){
       /** Set the base  density to the observed density */
       errorCode = ApiQuantity_setValueFromParent((ApiQuantity*)calcReq->baseDens,
						  (ApiQuantity*)calcReq->obDens);
       if( errorCode != 0) return errorCode;

       /** Set observered to base expansion factors to 1.0 */
       errorCode = ApiQuantity_setValue((ApiQuantity*)calcReq->ctlObToBase, 
					1.0,
					ApiUnit_Expansion_DIMLESS());
       if( errorCode != 0) return errorCode;
       errorCode = ApiQuantity_setValue((ApiQuantity*)calcReq->cplObToBase, 
					1.0,
					ApiUnit_Expansion_DIMLESS());
       if( errorCode != 0) return errorCode;
       errorCode = ApiQuantity_setValue((ApiQuantity*)calcReq->ctplObToBase, 
					1.0,
					ApiUnit_Expansion_DIMLESS());
       if( errorCode != 0) return errorCode;
       return errorCode;
   }

   /** 
   * Check whether alpha60 is supplied. 
   * We need to persist this between calls to 
   * transformBaseToDesired() as that method calculates 
   * and intermediate alpha60. 
   */
    if( calcReq->alpha60 != NULL) alphaSupplied = TRUE;

    /**
    * The temperature uncorrected for IPTS-68 needs to be
    * persisted, as a call to transformBaseToDesired()  will
    * shift the obTemp value. However, The 11.1.6.2 calculations 
    * need the uncorrected value.
    */
    unshiftedT0 = ApiQuantity_getValue((ApiQuantity*)calcReq->obTemp,
				       ApiUnit_Temperature_F(),
				       FALSE,
				       &errorCode);
    if( errorCode != 0) return errorCode;

    /**
    *   Step 2: Initialize the value for rho_60(m), i.e. calcReq.baseDens.
    *           The observed density is used as the starting value.
    *           If the commodity and not alpha60 is supplied, we need to
    *           restrict the initial rho60(m) so it stays within allowed 
    *           limits.
    */
    errorCode = ApiQuantity_setValueFromParent((ApiQuantity*)calcReq->baseDens,
					       (ApiQuantity*)calcReq->obDens);
    if( errorCode != 0) return errorCode;

    if( alphaSupplied == FALSE){
       tmp =  ApiOilProduct_isInRange(CALC_REQ_COMMODITY, 
				      calcReq->baseDens,
				      &errorCode);
       if( errorCode != 0) return errorCode;
       if(  tmp == FALSE){
           limits = ApiOilProduct_getLimits(CALC_REQ_COMMODITY, &errorCode);
	   if( errorCode != 0) return errorCode;
	   c1 = ApiQuantity_compareTo((ApiQuantity*)limits[0],
				      (ApiQuantity*)calcReq->baseDens,
				      &errorCode);
	   if( errorCode != 0) return errorCode;

	   if( c1 > 0){
	     errorCode = ApiQuantity_setValueFromParent((ApiQuantity*)calcReq->baseDens,
							(ApiQuantity*)limits[0]);
	     if( errorCode != 0) return errorCode;
	   }
	   c1 = ApiQuantity_compareTo((ApiQuantity*)limits[1],
				      (ApiQuantity*)calcReq->baseDens,
				      &errorCode);
	   if( errorCode != 0) return errorCode;
	   if( c1 < 0){
	     errorCode = ApiQuantity_setValueFromParent((ApiQuantity*)calcReq->baseDens,
							(ApiQuantity*)limits[1]);
	     if( errorCode != 0) return errorCode;
	   }
       }
    }

    /**  Start the iteration */
    while(TRUE){
         if( calcReq->reportIntermediate){
           ApiCalculationRequest_appendBuffer(calcReq->intermediate,
					      calcReq->intLength,
					      "Step ");
	   ApiCalculationRequest_appendDouble(calcReq->intermediate,
					      calcReq->intLength,
					      m+1);
	   ApiCalculationRequest_appendBuffer(calcReq->intermediate,
					      calcReq->intLength,
					      API_EOL);
	 }
	 value = ApiQuantity_getValue((ApiQuantity*)calcReq->baseDens, 
				     ApiUnit_Density_KGM3(),
				     FALSE,
				     &errorCode);
	if( errorCode != 0) return errorCode;
	ApiCalculationRequest_reportQuantityValue(calcReq,TRUE,"Rho60(m) in kg/m^3",value);

	/**
	*  Step 3: Perform a 11.1.6.1 calculation
	*          If the commodity is "Refined Products", the 
	*           commodity may change during iteration.
	*           We determine it here once(!) per iteration
	*           and save it as detCommodity
	*/
        if( alphaSupplied == FALSE ){
  	    c1 = ApiOilProduct_productName(CALC_REQ_COMMODITY, &errorCode);
	    if( errorCode != 0) return errorCode;
	    CALC_REQ_DET_COM = ApiOilProduct_getProductByDensity( c1,
								  calcReq->baseDens,
								  &errorCode);
	    if( errorCode != 0) return errorCode;
	    if( calcReq->alpha60 != NULL){
	      ApiQuanity_cleanUp( (ApiQuantity*)calcReq->alpha60 );
	      calcReq->alpha60 = NULL;
	    }
	}
        errorCode =ApiCalculationRequest_transformBaseToDesired(calcReq, TRUE);
	if( errorCode != 0) return errorCode;

        /**  Step 4: Check whether we need to terminate the iteration */
	value = ApiQuantity_getValue((ApiQuantity*)calcReq->obDens,
				     ApiUnit_Density_KGM3(),
				     FALSE,
				     &errorCode);
	if( errorCode != 0) return errorCode;
	deltaR = ApiQuantity_getValue((ApiQuantity*)calcReq->altDens,
				      ApiUnit_Density_KGM3(),
				      FALSE,
				      &errorCode);
	if( errorCode != 0) return errorCode;
	deltaR = value - deltaR;
	ApiCalculationRequest_reportQuantityValue(calcReq,
						  TRUE,
						  "delta Rho60 in kg/m^3",
						  deltaR);
        if( fabs(deltaR) < API_CONVERSION_CRITERIA ) break;

        /**  Step 5: Calculate the new value for rho_60(m) */
        /* determine em  */
	em = ApiQuantity_getValue((ApiQuantity*)calcReq->obDens,
				  ApiUnit_Density_KGM3(),
				  FALSE,
				  &errorCode);
	if( errorCode != 0) return errorCode;
	value = ApiQuantity_getValue((ApiQuantity*)calcReq->ctplBaseToAlt,
				     ApiUnit_Expansion_DIMLESS(),
				     FALSE,
				     &errorCode);
	if( errorCode != 0) return errorCode;
	em /= value;
	value = ApiQuantity_getValue((ApiQuantity*)calcReq->baseDens,
				     ApiUnit_Density_KGM3(),
				     FALSE,
				     &errorCode);
	if( errorCode != 0) return errorCode;
	em -= value;
	ApiCalculationRequest_reportQuantityValue(calcReq,TRUE,"E(m)",em);

	/**  determine dpm */ 
	dpm = dpStepfactor[0] + dpStepfactor[1] * unshiftedT0;
	value = ApiQuantity_getValue((ApiQuantity*)calcReq->cplBaseToAlt,
				     ApiUnit_Expansion_DIMLESS(),
				     FALSE,
				     &errorCode);
	if( errorCode != 0) return errorCode;
	dpm *= 2.0 * value;
	value = ApiQuantity_getValue((ApiQuantity*)calcReq->obPres,
				     ApiUnit_Pressure_PSI(),
				     FALSE,
				     &errorCode);
	if( errorCode != 0) return errorCode;
	dpm *= value;
	value = ApiQuantity_getValue((ApiQuantity*)calcReq->scaledCompAlt,
				     ApiUnit_ScaledComp_REV_PSI(),
				     FALSE,
				     &errorCode);
	if( errorCode != 0) return errorCode;
	dpm *= value;
	value = ApiQuantity_getValue((ApiQuantity*)calcReq->baseDens,
				     ApiUnit_Density_KGM3(),		     
				     FALSE,
				     &errorCode);
	if( errorCode != 0) return errorCode;
	dpm /= value * value;
	dpm *= -1.0;
	ApiCalculationRequest_reportQuantityValue(calcReq,TRUE,"Dp(m)",dpm);

        /**  determine dtm - but only if alpha_60 was not supplied */
        if( alphaSupplied == TRUE ) dtm = 0;
	else{
	  deltaT =  unshiftedT0 - 60.0;
	  value = ApiQuantity_getValue((ApiQuantity*)calcReq->alpha60,
				       ApiUnit_ThermalExp_REV_F(),     
				       FALSE,
				       &errorCode);
	  if( errorCode != 0) return errorCode;
	  dtm = 1.0 + 1.6 * deltaT * value;
	  dtm *= deltaT *  value;
	  dtm  *=  ApiOilProduct_getDalpha( CALC_REQ_DET_COM , &errorCode);
	  if( errorCode != 0) return errorCode;
	}
	ApiCalculationRequest_reportQuantityValue(calcReq,TRUE,"Dt(m)",dtm);

	/**  determine deltaR and the new value for rho(m) */
	deltaR = em / (1.0 + dtm + dpm);
	ApiCalculationRequest_reportQuantityValue(calcReq,TRUE,"delta Rho(m)",deltaR);
	value =  ApiQuantity_getValue((ApiQuantity*)calcReq->baseDens,
				      ApiUnit_Density_KGM3(),		     
				      FALSE,
				      &errorCode);
	if( errorCode != 0) return errorCode;
	rho60 =  value + deltaR;

	/**  check for special cases in Newton's method */
       if( alphaSupplied == FALSE){
	 limits = ApiOilProduct_getLimits(CALC_REQ_COMMODITY, &errorCode);
	 if( errorCode != 0) return errorCode;
	 value = ApiQuantity_getValue((ApiQuantity*)limits[0],
				      ApiUnit_Density_KGM3(),
				      FALSE,
				      &errorCode);
	 if( errorCode != 0) return errorCode;
	 if( rho60 < value) {
              rho60 = value;
	      if( m >= API_ITERATION_STEPS - 1){
		errorCode = ITERATION_DENSITY_VALUE_OUT_OF_RANGE;
		return errorCode;
	      } 
	 }
	 value = ApiQuantity_getValue((ApiQuantity*)limits[1],
				      ApiUnit_Density_KGM3(),
				      FALSE,
				      &errorCode);
	 if( errorCode != 0) return errorCode;
	 if( rho60 > value) {
              rho60 = value;
	      if( m >= API_ITERATION_STEPS - 1){
                errorCode = ApiCalculationRequest_checkConversion(calcReq,
				     ITERATION_DENSITY_VALUE_OUT_OF_RANGE);
	         if( errorCode == 0) break;
 	         else return errorCode;
	      } 
	 }
       }

       /**  initialize for the next iteration */
       errorCode = ApiQuantity_setValue( (ApiQuantity*)calcReq->baseDens,
					 rho60,
					 ApiUnit_Density_KGM3());
       if( errorCode != 0) return errorCode;

       /**  Step 6: Increment the counter */
       m++;
       if(  m >= API_ITERATION_STEPS -1) {   /* Procedure is not converging */
	 errorCode = ApiCalculationRequest_checkConversion(calcReq,
							   CONVERGENCE_NOT_REACHED);
	 if( errorCode == 0) break;
	 else return errorCode;
       }
    }
    if( calcReq->reportIntermediate){
      ApiCalculationRequest_appendBuffer(calcReq->intermediate,
					 calcReq->intLength,
					 "Convergence reached");
      ApiCalculationRequest_appendBuffer(calcReq->intermediate,
					 calcReq->intLength,
					 API_EOL);
    }
    ApiCalculationRequest_reportQuantity(calcReq,
					 TRUE,
					 API_BASE_DENS_NAME,
					 (ApiQuantity*)calcReq->baseDens,
					 FALSE,
					 ApiUnit_Density_KGM3());

    /**  
     * Set the intermediate values to the values
     * calculated by the 11.1.6.2 procedure
     */
    value = ApiQuantity_getValue((ApiQuantity*)calcReq->ctlBaseToAlt,
				 ApiUnit_Expansion_DIMLESS(),
				 FALSE,
				 &errorCode);
    if( errorCode != 0) return errorCode;
    errorCode = ApiQuantity_setValue((ApiQuantity*)calcReq->ctlObToBase,
				     value,
				     ApiUnit_Expansion_DIMLESS());
    if( errorCode != 0) return errorCode;
    ApiCalculationRequest_reportQuantity(calcReq,
					 TRUE,
					 API_CTL_OB_TO_BASE_NAME,
					 (ApiQuantity*)calcReq->ctlObToBase,
					 FALSE,
					 NULL);


    value = ApiQuantity_getValue((ApiQuantity*)calcReq->cplBaseToAlt,
				 ApiUnit_Expansion_DIMLESS(),
				 FALSE,
				 &errorCode);
    if( errorCode != 0) return errorCode;
    errorCode = ApiQuantity_setValue((ApiQuantity*)calcReq->cplObToBase,
				     value,
				     ApiUnit_Expansion_DIMLESS());
    if( errorCode != 0) return errorCode;
    ApiCalculationRequest_reportQuantity(calcReq,
					 TRUE,
					 API_CPL_OB_TO_BASE_NAME,
					 (ApiQuantity*)calcReq->cplObToBase,
					 FALSE,
					 NULL);

    value = ApiQuantity_getValue((ApiQuantity*)calcReq->ctplBaseToAlt,
				 ApiUnit_Expansion_DIMLESS(),
				 FALSE,
				 &errorCode);
    if( errorCode != 0) return errorCode;
    errorCode = ApiQuantity_setValue((ApiQuantity*)calcReq->ctplObToBase,
				     value,
				     ApiUnit_Expansion_DIMLESS());
    if( errorCode != 0) return errorCode;
    ApiCalculationRequest_reportQuantity(calcReq,
					 TRUE,
					 API_CTPL_OB_TO_BASE_NAME,
					 (ApiQuantity*)calcReq->ctplObToBase,
					 FALSE,
					 NULL);


    value = ApiQuantity_getValue((ApiQuantity*)calcReq->scaledCompAlt,
				 ApiUnit_ScaledComp_REV_PSI(),
				 FALSE,
				 &errorCode);
    if( errorCode != 0) return errorCode;
    errorCode = ApiQuantity_setValue((ApiQuantity*)calcReq->scaledCompOb,
				     value,
				     ApiUnit_ScaledComp_REV_PSI());
    if( errorCode != 0) return errorCode;
    ApiCalculationRequest_reportQuantity(calcReq,
					 TRUE,
					 API_SCALED_COMP_OB_NAME,
					 (ApiQuantity*)calcReq->scaledCompOb,
					 FALSE,
					 NULL);

    if( calcReq->reportIntermediate){
      ApiCalculationRequest_appendBuffer(calcReq->intermediate,
					 calcReq->intLength,
					 "Correct to 60 F and 0 psi");
      ApiCalculationRequest_appendBuffer(calcReq->intermediate,
					 calcReq->intLength,
					 API_EOL);
    }
    return errorCode;
};

/**
* Do the actual calculation
* 
* @param calcReq the CalculationRequest  describing the desired
*                calculation
* @param transformToBase should we transform to base
* @return 0 if all is well and an error code otherwise
*/
int ApiCalculationRequest_doCalculation(ApiCalculationRequest * calcReq,
					bool transformToBase){
    int errorCode = 0;
    double v1,v2;
    bool tmp;

    CALC_REQ_COMMODITY = NULL;
    CALC_REQ_DET_COM = NULL;
      
    /** Determine the commodity if calcReq.alpha60 is not given */
    if( calcReq->alpha60 == NULL && 
        calcReq->commodityName != API_COMMODITY_NOT_GIVEN ){
      CALC_REQ_COMMODITY = ApiOilProduct_getProductByName(calcReq->commodityName,
							  &errorCode);
      if( errorCode != 0) return errorCode;
    }

    if( transformToBase ){
           /**  first check that all input parameters are in range */
	   tmp = ApiCalculationRequest_checkLimits(calcReq, &errorCode);
  	   if( tmp == FALSE || errorCode != 0){
	      return errorCode;
	   }
           if( calcReq->reportIntermediate){
	     ApiCalculationRequest_appendBuffer(calcReq->intermediate,
						calcReq->intLength,
						"All values are in range "),
	       ApiCalculationRequest_appendBuffer(calcReq->intermediate,
						  calcReq->intLength,
						  API_EOL);
	   }
            
	   errorCode = ApiCalculationRequest_initializeRequest(calcReq);
	   if( errorCode != 0) return errorCode;

            /* 
	    *  First we need to calculate all values to base condition.
            *  The procedure 11.1.6.2 is used for this 
            */
            errorCode = ApiCalculationRequest_transformToBaseCondition(calcReq);
	    if( errorCode != 0) return errorCode;
    }


    /* 
    * Now we need to run procedure 11.1.6.1 to transform to
    * alternate conditions 
    */
    errorCode = ApiCalculationRequest_transformBaseToDesired(calcReq, FALSE);
    if( errorCode != 0) return errorCode;


    /**  Calculate the final correction factors */
    /** ctlAlt  = ctlBaseToAlt / ctlObToBase */
    v1 =  ApiQuantity_getValue((ApiQuantity*)calcReq->ctlBaseToAlt,
			       ApiUnit_Expansion_DIMLESS(),
		       FALSE,
			       &errorCode);
    if( errorCode != 0) return errorCode;
    v2 =  ApiQuantity_getValue((ApiQuantity*)calcReq->ctlObToBase,
			       ApiUnit_Expansion_DIMLESS(),
			       FALSE,
			       &errorCode);
    if( errorCode != 0) return errorCode;
    v1 = v1 / v2;
    errorCode = ApiQuantity_setValue((ApiQuantity*)calcReq->ctlAlt,
				     v1,
				     ApiUnit_Expansion_DIMLESS());
    if( errorCode != 0) return errorCode;

     /** cplAlt  = cplBaseToAlt / cplObToBase */
    v1 =  ApiQuantity_getValue((ApiQuantity*)calcReq->cplBaseToAlt,
			       ApiUnit_Expansion_DIMLESS(),
			       FALSE,
			       &errorCode);
    if( errorCode != 0) return errorCode;
    v2 =  ApiQuantity_getValue((ApiQuantity*)calcReq->cplObToBase,
			       ApiUnit_Expansion_DIMLESS(),
			       FALSE,
			       &errorCode);
    if( errorCode != 0) return errorCode;
    v1 = v1 / v2;
    errorCode = ApiQuantity_setValue((ApiQuantity*)calcReq->cplAlt,
				     v1,
				     ApiUnit_Expansion_DIMLESS());
    if( errorCode != 0) return errorCode;

    /** ctplAlt = cplAlt * ctlAlt */
    v1 =  ApiQuantity_getValue((ApiQuantity*)calcReq->ctlAlt,
			       ApiUnit_Expansion_DIMLESS(),
			       FALSE,
			       &errorCode);
    if( errorCode != 0) return errorCode;
    v2 =  ApiQuantity_getValue((ApiQuantity*)calcReq->cplAlt,
			       ApiUnit_Expansion_DIMLESS(),
			       FALSE,
			       &errorCode);
    if( errorCode != 0) return errorCode;
    v1 = v1 *  v2;
    errorCode = ApiQuantity_setValue((ApiQuantity*)calcReq->ctplAlt,
				     v1,
				     ApiUnit_Expansion_DIMLESS());
    if( errorCode != 0) return errorCode;

    return errorCode;
};


/**
*  Start the calculation. 
*/
int  ApiCalculationRequest_calculationResult(ApiCalculationRequest * calcReq){
   ApiUnit *u;
   int errorCode = 0;
   int c1,c2;
   double value;

   ApiCalculationRequest_reportInputData(calcReq);


   /** 
    * Check that the base values are set 
    */
   if( CALC_REQ_BASE_TEMP  == NULL){
       CALC_REQ_BASE_TEMP = ApiTemperature_init(API_BASE_TEMP,
						ApiUnit_Temperature_F(),
						FALSE,
						&errorCode);
       if( errorCode != 0) return errorCode;
   }
   if( CALC_REQ_BASE_PRES  == NULL){
       CALC_REQ_BASE_PRES = ApiPressure_init(API_BASE_PRES,
					     ApiUnit_Pressure_PSI(),
					     FALSE,
					     &errorCode);
       if( errorCode != 0) return errorCode;
   }


   /**  
    *  If the base conditions are 60 F and 0 psi, 
    *  no extra calculation is needed
    */
   c1 = ApiQuantity_compareTo((ApiQuantity*)calcReq->baseTemp,
			      (ApiQuantity*)CALC_REQ_BASE_TEMP,
			      &errorCode);
   if( errorCode != 0) return errorCode;
   c2 = ApiQuantity_compareTo((ApiQuantity*)calcReq->basePres,
			      (ApiQuantity*)CALC_REQ_BASE_PRES,
			      &errorCode);
   if( errorCode != 0) return errorCode;
   if( c1 == 0 && c2 == 0){
      errorCode = ApiCalculationRequest_doCalculation(calcReq, TRUE);
      if( errorCode != 0) return errorCode;
   }


   else{   /**  We need to transform to the desired base condition */
     /**  intialize the intermediate  CalculationRequest object */
     if( calcReq->reportIntermediate){
       ApiCalculationRequest_appendBuffer(calcReq->intermediate,
					  calcReq->intLength,
					  "Transform observed to alternate");
       ApiCalculationRequest_appendBuffer(calcReq->intermediate,
					  calcReq->intLength,
					  API_EOL);
     }

     if(CALC_REQ_INTER  ==  NULL){
          CALC_REQ_INTER = ApiCalculationRequest_init(&errorCode);
	  if( errorCode != 0) return errorCode;
     }

     /** Transfer the values for the intermediate reporting buffer */
     CALC_REQ_INTER->intermediate = calcReq->intermediate;
     CALC_REQ_INTER->intLength = calcReq->intLength;
     CALC_REQ_INTER->getOilProductName = calcReq->getOilProductName;
     CALC_REQ_INTER->getParameterName = calcReq->getParameterName;
     CALC_REQ_INTER->reportIntermediate = calcReq->reportIntermediate;
   
     /** Initialize all relevant quantities */
     errorCode =  ApiCalculationRequest_initializeRequest(calcReq);
     if( errorCode != 0) return errorCode;
     errorCode =  ApiCalculationRequest_initializeRequest(CALC_REQ_INTER);
     if( errorCode != 0) return errorCode;

     /** Transfer the commodity or alpha60 information */
     CALC_REQ_INTER->commodityName = calcReq->commodityName;
     if( calcReq->alpha60 != NULL){
         if(  CALC_REQ_INTER->alpha60 != NULL){
	   errorCode = ApiQuantity_setValueFromParent((ApiQuantity*)CALC_REQ_INTER->alpha60,
						      (ApiQuantity*)calcReq->alpha60);
	 }
  	 else{
            u =  ApiQuantity_givenUnit( (ApiQuantity*)calcReq->alpha60, &errorCode); 
            if( errorCode != 0) return errorCode;
            value = ApiQuantity_getValue((ApiQuantity*)calcReq->alpha60,
					 u,
					 FALSE, &errorCode);
	    if( errorCode != 0) return errorCode;
	    CALC_REQ_INTER->alpha60 = ApiAlpha60_init(value,u, TRUE, &errorCode);
	}
	if( errorCode != 0) return errorCode;
     }
     else{
       if( CALC_REQ_INTER->alpha60 != NULL){
            ApiQuanity_cleanUp((ApiQuantity*)CALC_REQ_INTER->alpha60);
       }
       CALC_REQ_INTER->alpha60 = NULL;
     }
     

     /** Transfer the relevant values from calcReq */
     errorCode = ApiQuantity_setValueFromParent((ApiQuantity*)CALC_REQ_INTER->obTemp,
						(ApiQuantity*)calcReq->obTemp);
     if( errorCode != 0) return errorCode;
     errorCode = ApiQuantity_setValueFromParent((ApiQuantity*)CALC_REQ_INTER->obPres,
						(ApiQuantity*)calcReq->obPres);
     if( errorCode != 0) return errorCode;

     errorCode = ApiQuantity_setValueFromParent((ApiQuantity*)CALC_REQ_INTER->altTemp,
						(ApiQuantity*)calcReq->altTemp);
     if( errorCode != 0) return errorCode;
     errorCode = ApiQuantity_setValueFromParent((ApiQuantity*)CALC_REQ_INTER->altPres,
						(ApiQuantity*)calcReq->altPres);
     if( errorCode != 0) return errorCode;

     errorCode = ApiQuantity_setValueFromParent((ApiQuantity*)CALC_REQ_INTER->obDens,
						(ApiQuantity*)calcReq->obDens);
     if( errorCode != 0) return errorCode;

     /**  do the calculation for the final values */
     errorCode =  ApiCalculationRequest_doCalculation(CALC_REQ_INTER, TRUE);
     if( errorCode != 0) return errorCode;
     errorCode = ApiQuantity_setValueFromParent((ApiQuantity*)calcReq->altDens,
						(ApiQuantity*)CALC_REQ_INTER->altDens);
     if( errorCode != 0) return errorCode;
     errorCode = ApiQuantity_setValueFromParent((ApiQuantity*)calcReq->ctlAlt,
						(ApiQuantity*)CALC_REQ_INTER->ctlAlt);
     if( errorCode != 0) return errorCode;
     errorCode = ApiQuantity_setValueFromParent((ApiQuantity*)calcReq->cplAlt,
						(ApiQuantity*)CALC_REQ_INTER->cplAlt);
     if( errorCode != 0) return errorCode;
     errorCode = ApiQuantity_setValueFromParent((ApiQuantity*)calcReq->ctplAlt,
						(ApiQuantity*)CALC_REQ_INTER->ctplAlt);
     if( errorCode != 0) return errorCode;
     errorCode = ApiQuantity_setValueFromParent((ApiQuantity*)calcReq->scaledCompAlt,
						(ApiQuantity*)CALC_REQ_INTER->scaledCompAlt);
     if( errorCode != 0) return errorCode;


     /** now initialize for base conditions */
     c1 = ApiQuantity_compareTo((ApiQuantity*)calcReq->baseTemp,
				(ApiQuantity*)calcReq->obTemp,
				&errorCode);
     if( errorCode != 0) return errorCode;
     c2 = ApiQuantity_compareTo((ApiQuantity*)calcReq->basePres,
				(ApiQuantity*)calcReq->obPres,
				&errorCode);
     if( errorCode != 0) return errorCode;

     if( c1 != 0 || c2 != 0){ 
	/** do the calculation for the base conditions */
        if( calcReq->reportIntermediate){
	  ApiCalculationRequest_appendBuffer(calcReq->intermediate,
					     calcReq->intLength,
					     "Transform 60 F, 0 psi to base");
	  ApiCalculationRequest_appendBuffer(calcReq->intermediate,
					     calcReq->intLength,
					     API_EOL);
	}
        errorCode = ApiQuantity_setValueFromParent((ApiQuantity*)CALC_REQ_INTER->altTemp,
						   (ApiQuantity*)calcReq->baseTemp);
	if( errorCode != 0) return errorCode;
        errorCode = ApiQuantity_setValueFromParent((ApiQuantity*)CALC_REQ_INTER->altPres,
						   (ApiQuantity*)calcReq->basePres);
	if( errorCode != 0) return errorCode;

	errorCode = ApiCalculationRequest_doCalculation(CALC_REQ_INTER, FALSE);
	if( errorCode != 0) return errorCode;

	/** and set the parameters */
        errorCode = ApiQuantity_setValueFromParent((ApiQuantity*)calcReq->baseDens,
						   (ApiQuantity*)CALC_REQ_INTER->altDens);
	if( errorCode != 0) return errorCode;
        errorCode = ApiQuantity_setValueFromParent((ApiQuantity*)calcReq->scaledCompOb,
                                           (ApiQuantity*)CALC_REQ_INTER->scaledCompOb);
	if( errorCode != 0) return errorCode;

	value = ApiQuantity_getValue((ApiQuantity*)CALC_REQ_INTER->ctlAlt,
				     ApiUnit_Expansion_DIMLESS(),
				     FALSE,
				     &errorCode);
	if( errorCode != 0) return errorCode;
	value = 1.0/ value;
	errorCode = ApiQuantity_setValue((ApiQuantity*)calcReq->ctlObToBase,
					 value,
					 ApiUnit_Expansion_DIMLESS());
	if( errorCode != 0) return errorCode;

	value = ApiQuantity_getValue((ApiQuantity*)CALC_REQ_INTER->cplAlt,
				     ApiUnit_Expansion_DIMLESS(),
				     FALSE,
				     &errorCode);
	if( errorCode != 0) return errorCode;
	value = 1.0/ value;
	errorCode = ApiQuantity_setValue((ApiQuantity*)calcReq->cplObToBase,
					 value,
					 ApiUnit_Expansion_DIMLESS());
	if( errorCode != 0) return errorCode;

	value = ApiQuantity_getValue((ApiQuantity*)CALC_REQ_INTER->ctplAlt,
				     ApiUnit_Expansion_DIMLESS(),
				     FALSE,
				     &errorCode);
	if( errorCode != 0) return errorCode;
	value = 1.0/ value;
	errorCode = ApiQuantity_setValue((ApiQuantity*)calcReq->ctplObToBase,
					 value,
					 ApiUnit_Expansion_DIMLESS());
	if( errorCode != 0) return errorCode;
     }
     else{    /**   no calculation needed */
         errorCode = ApiQuantity_setValueFromParent((ApiQuantity*)calcReq->baseDens,
						    (ApiQuantity*)calcReq->obDens);
	 if( errorCode != 0) return errorCode;        
	 errorCode = ApiQuantity_setValue((ApiQuantity*)calcReq->ctlObToBase,
					  1.0,
					 ApiUnit_Expansion_DIMLESS());
	 if( errorCode != 0) return errorCode;
	 errorCode = ApiQuantity_setValue((ApiQuantity*)calcReq->cplObToBase,
					  1.0,
					 ApiUnit_Expansion_DIMLESS());
	 if( errorCode != 0) return errorCode;
	 errorCode = ApiQuantity_setValue((ApiQuantity*)calcReq->ctplObToBase,
					  1.0,
					 ApiUnit_Expansion_DIMLESS());
	 if( errorCode != 0) return errorCode;
	 errorCode = ApiQuantity_setValue((ApiQuantity*)calcReq->scaledCompOb,
					  1.0,
					  ApiUnit_ScaledComp_REV_PSI());
	 if( errorCode != 0) return errorCode;
     }


    /** Initialize for alternate conditions */
     c1 = ApiQuantity_compareTo((ApiQuantity*)calcReq->altTemp,
				(ApiQuantity*)calcReq->baseTemp,
				&errorCode);
     if( errorCode != 0) return errorCode;
     c2 = ApiQuantity_compareTo((ApiQuantity*)calcReq->altPres,
				(ApiQuantity*)calcReq->basePres,
				&errorCode);
     if( errorCode != 0) return errorCode;
     if( c1 != 0 || c2 != 0){ 
         double f1,f2;

         if( calcReq->reportIntermediate){
	   ApiCalculationRequest_appendBuffer(calcReq->intermediate,
					      calcReq->intLength,
					      "Transform 60 F, 0 psi to alternate");
	   ApiCalculationRequest_appendBuffer(calcReq->intermediate,
					      calcReq->intLength,
					      API_EOL);
	 }
	 f1 = ApiQuantity_getValue((ApiQuantity*)calcReq->ctlObToBase,
				     ApiUnit_Expansion_DIMLESS(),
				     FALSE,
				     &errorCode);
	 if( errorCode != 0) return errorCode;
	 f2 = ApiQuantity_getValue((ApiQuantity*)calcReq->ctlAlt,
				     ApiUnit_Expansion_DIMLESS(),
				     FALSE,
				     &errorCode);
	 if( errorCode != 0) return errorCode;
	 errorCode = ApiQuantity_setValue((ApiQuantity*)calcReq->ctlBaseToAlt,
					  f1 * f2,
					  ApiUnit_Expansion_DIMLESS());
	 if( errorCode != 0) return errorCode;
	 ApiCalculationRequest_reportQuantity(calcReq,
					      TRUE, 
					      API_CTL_BASE_TO_ALT_NAME,
					      (ApiQuantity*)calcReq->ctlBaseToAlt,
					      FALSE,
					      ApiUnit_Expansion_DIMLESS());

	 f1 = ApiQuantity_getValue((ApiQuantity*)calcReq->cplObToBase,
				     ApiUnit_Expansion_DIMLESS(),
				     FALSE,
				     &errorCode);
	 if( errorCode != 0) return errorCode;
	 f2 = ApiQuantity_getValue((ApiQuantity*)calcReq->cplAlt,
				     ApiUnit_Expansion_DIMLESS(),
				     FALSE,
				     &errorCode);
	 if( errorCode != 0) return errorCode;
	 errorCode = ApiQuantity_setValue((ApiQuantity*)calcReq->cplBaseToAlt,
					  f1 * f2,
					  ApiUnit_Expansion_DIMLESS());	 
	 if( errorCode != 0) return errorCode;
	 ApiCalculationRequest_reportQuantity(calcReq,
					      TRUE,
					      API_CPL_BASE_TO_ALT_NAME,
					      (ApiQuantity*)calcReq->cplBaseToAlt,
					      FALSE,
					      ApiUnit_Expansion_DIMLESS());

	 f1 = ApiQuantity_getValue((ApiQuantity*)calcReq->ctplObToBase,
				     ApiUnit_Expansion_DIMLESS(),
				     FALSE,
				     &errorCode);
	 if( errorCode != 0) return errorCode;
	 f2 = ApiQuantity_getValue((ApiQuantity*)calcReq->ctplAlt,
				     ApiUnit_Expansion_DIMLESS(),
				     FALSE,
				     &errorCode);
	 if( errorCode != 0) return errorCode;
	 errorCode = ApiQuantity_setValue((ApiQuantity*)calcReq->ctplBaseToAlt,
					  f1 * f2,
					  ApiUnit_Expansion_DIMLESS());
	 if( errorCode != 0) return errorCode;
	 ApiCalculationRequest_reportQuantity(calcReq,
					      TRUE, 
					      API_CTPL_BASE_TO_ALT_NAME,
					      (ApiQuantity*)calcReq->ctplBaseToAlt,
					      FALSE,
					      ApiUnit_Expansion_DIMLESS());
     }
     else{
         errorCode = ApiQuantity_setValue((ApiQuantity*)calcReq->ctlBaseToAlt,
					  1.0,
					  ApiUnit_Expansion_DIMLESS());
	 if( errorCode != 0) return errorCode;
         errorCode = ApiQuantity_setValue((ApiQuantity*)calcReq->cplBaseToAlt,
					  1.0,
					  ApiUnit_Expansion_DIMLESS());
	 if( errorCode != 0) return errorCode;
         errorCode = ApiQuantity_setValue((ApiQuantity*)calcReq->ctplBaseToAlt,
					  1.0,
					  ApiUnit_Expansion_DIMLESS());
	 if( errorCode != 0) return errorCode;
     }
   }

   /** Set the volume data and report the output data  */
   errorCode =  ApiCalculationRequest_getVolumeData(calcReq);
   if( errorCode != 0 ) return errorCode;
   ApiCalculationRequest_reportOutputData(calcReq);

   return errorCode;
};

int  ApiCalculationRequest_performCalculation(ApiCalculationRequest * request){
  int errorCode = ApiCalculationRequest_calculationResult(request);

  if( errorCode != 0) ApiCalculationRequest_setErrorStatus(request);
  return errorCode;
}
