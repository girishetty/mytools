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


#include "quantity.h"
#include "oilproducts.h"
#include <string.h>
#include <math.h>
#include <stdlib.h>


/** Type definitions for the virtual functions in the VTable */
/** The function to get the current value */
typedef double (*ApiQuantity_getValue_proto)(ApiQuantity * quant,
					     ApiUnit *unit,
					     int rounded,
					     int *errorCode);

/** The function to delete all resources used by this quantity */
typedef void (*ApiQuantity_destroy_proto)(ApiQuantity *quant);

/** The function to retrieve the rounding increment */
typedef double (*ApiQuantity_roundingIncrement_proto)(ApiUnit *unit,
						      int *errorCode);

/** The function to get the standard unit for this quantity  */
typedef ApiUnit* (*ApiQuantity_standardUnit_proto)(ApiQuantity *quantity);


/** The function used to retrieve the allowed range */
typedef bool (*ApiQuantity_inAllowedRange_proto)(ApiQuantity *quant,
						int *errorCode);

/**
* The virtual function table for ApiQuantity
*/
typedef struct api_quantity_vtable{
  ApiQuantity_getValue_proto getValue;                   /** get the current value */
  ApiQuantity_destroy_proto destroy;                     /** Release all resources */
  ApiQuantity_roundingIncrement_proto roundingIncrement; /** Get unit specific rouding */
                                                         /*  increment */
  ApiQuantity_inAllowedRange_proto inAllowedRange;       /** Is the quantity in the */
                                                         /*  allowed range */
  ApiQuantity_standardUnit_proto getStandardUnit;        /** The function to get the 
							     standard unit */

  /* other static (for a given quantity type)  values */
  ApiUnit **acceptableUnits;           /** The list of acceptable units */
  ApiUnit *standardUnit;               /** The standard unit of the quantity */
  char *quantityName;                  /** The human readable name of the quantity */
} ApiQuantity_VTable;

/** The function to get the actual VTable to use */
typedef ApiQuantity_VTable * (*ApiQuantity_getVTable)();


/** 
* The actual structure for the quantity 
*/
struct api_quantity_{
  ApiQuantity_getVTable getVTable;     /** The function  to get the VTable */
  double currentValue;                 /** The current value */
  ApiUnit *currentUnit;                /** The current unit */ 
  double givenValue;                   /** The current value of */
  ApiUnit *givenUnit;                  /** The current unit */ 
  bool isMutable;                      /** Is the quantity mutable */
  bool wasCorrected;                   /** If the value can be corrected, was it done*/
};

/**
* The function that initializes the quantity.
* Only defined in this file.
*
* @param quant the quantity to initialize 
* @param value the value to which to set the quantity
* @param unit the unit in which the value is given
* @param isMutable, TRUE if this quantity is mutable, FALSE otherwise
*
* @return 0  if all is well, INITIALIZE_FAILED  if an error occured. 
*/
int ApiQuantity_initialize(ApiQuantity * quant,
			   double value,
			   ApiUnit * unit,
                           bool isMutable){
   int result = 0;

   if( quant == NULL) return INITIALIZE_FAILED;  

   quant->isMutable = TRUE;
   result =  ApiQuantity_setValue(quant,value,unit);
   quant->isMutable = isMutable;
   return result;
};

/**
* The function to clean up the resources taken by an ApiQuantity
* This is dispatched via the VTable
*/
void ApiQuanity_cleanUp(ApiQuantity * quant){
    ApiQuantity_VTable * vtable = NULL;     

   if( quant == NULL) return;
   if( quant->getVTable == NULL)  return;
   vtable =   quant->getVTable();
   if( vtable == NULL)    return;
   if( vtable->destroy == NULL) return;

   vtable->destroy(quant);
}

/**
* The ApiQuantity specific destroy function.
* This is only defined here
* 
* @param quant the quantity to free
*/
void ApiQuantity_destroy_internal(ApiQuantity * quant){
  if( quant != NULL) free(quant);
};


/**
* A function that return wether the given unit is  an acceptable unit.
* This function is only defined in this file.
* 
* @param quant the quantity on which to perform the operation
* @param unit the unit to check
* 
* @return 0 if all is well and INVALID_UNIT if the unit is not valid
*/ 
int ApiQuantity_isAcceptableUnit(ApiQuantity * quant,
				 ApiUnit * unit){ 
  int found = INVALID_UNIT;
  ApiUnit **allowedUnit;
  ApiQuantity_VTable *vtable;

  /** Check that we are correctly initialized */
  if( quant == NULL) return NULL_POINTER_EXCEPTION;
  if( quant->getVTable == NULL)  return NULL_POINTER_EXCEPTION;
  vtable =   quant->getVTable();
  if( vtable == NULL)    return NULL_POINTER_EXCEPTION;

  /** All is initialized now let's check the unit */ 
  allowedUnit = vtable->acceptableUnits;
  if( allowedUnit == NULL) return found;
  while(  allowedUnit[0] != NULL){
      if ( unit == allowedUnit[0]){
           found = 0;
           break;
      }
      allowedUnit++;
  }
  return found;
};


/**
* The function that returns the actual value.
* This is deferred to the child class
*/
double ApiQuantity_getValue(ApiQuantity * quant,
			    ApiUnit *unit,
			    bool rounded,
			    int * errorCode){
    ApiQuantity_VTable * vtable = NULL;     
    double result = 0;

    *errorCode = 0;

    /** Check that all is correctly initialized */
    if( quant == NULL || quant->getVTable == NULL){
         *errorCode =  NULL_POINTER_EXCEPTION;
         return result;
    }
    vtable =   quant->getVTable();
    if( vtable == NULL ||   vtable->getValue == NULL){
         *errorCode =  NULL_POINTER_EXCEPTION;
         return result;
    }

    /** Check that the unit is acceptable  */ 
    *errorCode = ApiQuantity_isAcceptableUnit(quant,unit);
    if( *errorCode != 0){
	return result;
    }


    /** And then dispatch the result */
    result = vtable->getValue(quant,unit,rounded,errorCode);

    /** Do the rounding if need be */
    if( rounded == TRUE){
        double delta;
        double rounded,tmp,sign = 1;
        int trunc;

        /* Step 1: Get the rounding increment delta for the unit */
        if( vtable->roundingIncrement == 0){
            *errorCode =  NULL_POINTER_EXCEPTION;
            return result;
	}
        delta = vtable->roundingIncrement(unit,errorCode);
	if( *errorCode != 0) return result;

        /* Step 2: Normalize the input variable */
        if( result < 0) sign = -1;
        rounded = fabs(result) / delta;
 
        /* Step 3: Find the integer closes to the normalized variable
             Remark: Decimal part is exactly 0.5 has been replaced
	             by |(decimal part) - 0.5| <   API_EPSILON
                     since floating point arithmetic may not result in
                     exactly  0.5 */
	trunc = (int)rounded;
        tmp = rounded - trunc;
        if( fabs( tmp - 0.5) <  API_EPSILON){
            int odd = trunc/2;
	    if( odd * 2 != trunc) {   /* odd  so  trunc is increase by 1 */
		trunc += 1;
	    }
	}
        else{
	    trunc = (int)(rounded + 0.5);
	}
       
        /* Step 4: Rescale the integer from Step 3 */
        rounded = trunc * delta;
	rounded *= sign;        
       
        result = rounded;
    }

    return result;
}


/**
* Set a new value for this quantity
*/
int ApiQuantity_setValue(ApiQuantity * quant,
			 double value,
			 ApiUnit *unit){
  int result = 0;

  /** 
   * Checke that all is correctly initialized and 
   * that we can change the value  
   */
  if( quant == NULL || unit == NULL) return NULL_POINTER_EXCEPTION;
  if( quant->isMutable == FALSE)     return CHANGED_IMMUTABLE;

  
  /** Check that the unit is acceptable */
  result = ApiQuantity_isAcceptableUnit(quant,unit);
  if( result != 0){
    return result;
  }

  /** Now set the value as unit is acceptable */
  quant->currentValue = value;
  quant->currentUnit = unit;
  quant->givenValue = value;
  quant->givenUnit = unit;
  
  quant->wasCorrected = FALSE;

  return result;
}

int ApiQuantity_setValueFromParent(ApiQuantity * quant, ApiQuantity * parent){
  int errorCode = 0;
  ApiUnit * u;
  double value;

  if( quant == NULL || parent == NULL){
    return  NULL_POINTER_EXCEPTION;
  }


  u =  ApiQuantity_givenUnit( parent, &errorCode);
  if( errorCode != 0) return errorCode;
  value = ApiQuantity_getValue(parent, u, FALSE, &errorCode);
  if( errorCode != 0) return errorCode;
  errorCode = ApiQuantity_setValue(quant, value, u);
  return errorCode;
}


bool ApiQuantity_inAllowedRange(ApiQuantity *quant,
  			       int * errorCode){
  bool result = FALSE;
  ApiQuantity_VTable * vtable = NULL;

  *errorCode = 0;
  /** Checke that all is correctly initialize */
  if( quant == NULL || quant->getVTable == NULL) {
    *errorCode = INITIALIZE_FAILED;
     return result;
  }
  vtable =   quant->getVTable();
  if( vtable == NULL ){
    *errorCode =  INITIALIZE_FAILED;
    return result;
  }
  *errorCode = 0;

  if( vtable->inAllowedRange != NULL){
    result = vtable->inAllowedRange( quant, errorCode);
  }
  else {
       *errorCode = QUANTITY_OUT_OFF_RANGE;
       result = FALSE;
  }

  return result;
}



/**
* Get the standard unit of the quantity
*/ 
ApiUnit * ApiQuantity_standardUnit(ApiQuantity * quant){
  ApiUnit *unit = NULL;
  ApiQuantity_VTable * vtable;

  if( quant == NULL || quant->getVTable == NULL) return unit;
  vtable =   quant->getVTable();
  if( vtable == NULL) return unit;
  
  if( vtable->getStandardUnit != NULL){
    unit = vtable->getStandardUnit( quant);
  }
  else unit = vtable->standardUnit;

  return unit;
}


int ApiQuantity_compareTo(ApiQuantity * quant1,
			  ApiQuantity * quant2,
                          int *errorCode){
  int result = -1;
  double q1,q2;
  ApiUnit *u1,*u2;
  ApiQuantity_VTable * vtable1, *vtable2;

  *errorCode = 0;

  if( quant1 == NULL || quant2 == NULL){
     *errorCode = NULL_POINTER_EXCEPTION;
     return result;
  }

  /* First make sure that the  they are the same unit */
  if( quant1->getVTable == NULL || quant1->getVTable == NULL){
     *errorCode = NULL_POINTER_EXCEPTION;
     return result;
  }
  vtable1 = quant1->getVTable();
  vtable2 = quant2->getVTable();
  if( vtable1 == NULL || vtable2 == NULL){
     *errorCode = NULL_POINTER_EXCEPTION;
     return result;
  }
  if( vtable1->quantityName == NULL || vtable2->quantityName == NULL){
     *errorCode =INITIALIZE_FAILED;
     return result;
  }
  if( strcmp(vtable1->quantityName, vtable2->quantityName) != 0){
    *errorCode =  INVALID_COMPARISON;
    return result;
  }
  

  /* Next make sure they are equal */
  u1 =  ApiQuantity_standardUnit(quant1);
  u2 =  ApiQuantity_standardUnit(quant2);
  if( u1 == NULL || u2 == NULL){
     *errorCode = INITIALIZE_FAILED;
     return result;
  }
  if( u1 != u2){
    *errorCode =  INVALID_COMPARISON;
    return result;
  }
  q1 = ApiQuantity_getValue(quant1, u1, FALSE, errorCode);
  if( *errorCode != 0){
    return result;
  }
  q2 = ApiQuantity_getValue(quant2, u2, FALSE, errorCode);
  if( *errorCode != 0){
    return result;
  }


  if( fabs(q1-q2) < API_EPSILON){
    result = 0;
  }
  else if( q2 > q1){
    result = -1;
  }
  else{
    result = 1;
  }
 
  return result;
}


ApiUnit * ApiQuantity_givenUnit(ApiQuantity *quant,
				int * errorCode){
  ApiUnit *result = 0;
  *errorCode = 0;
  if( quant == NULL){
     *errorCode = NULL_POINTER_EXCEPTION;
     return result;
  }
  result = quant->givenUnit;
  return result;
}

int ApiQuantity_getName(ApiQuantity * quant, char * buf, int len){
  int errorCode = 0;
  ApiQuantity_VTable * vtable;

  if( buf == NULL || quant == 0 || quant->getVTable == 0){
     return  NULL_POINTER_EXCEPTION;
  }
  vtable = quant->getVTable();  
  if( vtable == NULL || vtable->quantityName == NULL){
    return  NULL_POINTER_EXCEPTION;
  }
  
  if( strlen(buf) + strlen(vtable->quantityName) + 1 > (size_t)len){
    return BUFFER_OVERFLOW;
  }
  strcat(buf,vtable->quantityName);
  return errorCode;
}


/*------- Specific quantities ---- */

/**
* Implemenation for Temperature 
*/
/** 
* The actual structure for Temperature
*/
struct api_temperature_{
  ApiQuantity quant;
};

/**
* The static instance of the temperature specific VTable 
*/
static ApiQuantity_VTable * ApiTemperatureVTable = 0;


/** The  range of allowed temperatures */
static ApiTemperature **AllowedTemp = 0;


/**
* The Temperature specific getValue function.
* We already checked that quant is not null and that the unit
* given is an acceptable unit
*/
double ApiTemperature_getValue(ApiQuantity * quant,
			       ApiUnit *unit,
			       int rounded,
			       int * errorCode){
  double result = 0;

  *errorCode = 0;

  /** Retrieve the temperature in F - convert if needed */
  if( unit == ApiUnit_Temperature_F()){
    if( quant->currentUnit == ApiUnit_Temperature_F()){
       result = quant->currentValue;
    }
    else {
       result =  API_TEMP_C_TO_F_SLOPE * quant->currentValue + 
                 API_TEMP_C_TO_F_INTERCEPT;
    }
  }

  /** Retrieve the temperature in C - convert if needed */
  else if( unit == ApiUnit_Temperature_C()){
    if( quant->currentUnit == ApiUnit_Temperature_C()){
       result = quant->currentValue;
    }
    else {
       result = quant->currentValue - API_TEMP_C_TO_F_INTERCEPT;
       result /=   API_TEMP_C_TO_F_SLOPE;
    }
  }

  /** Otherwise the unit is not valid */
  else{
    *errorCode = INVALID_UNIT;
  }

  /** The rounding is done in the ApiQuantity function */
  return result;
};

/**
* Return the temperature specific rounding increment.
* We already checked that quant is not null and that the unit
* is an acceptable unit.
*
* @param unit the unit for  which to retrieve the rounding increment
* @param errorCode the error code that gets set if there was an error
*
* @return the correct rounding value
*/
double ApiTemperature_roundingIncrement(ApiUnit *unit,
					int *errorCode){
  double result = 0;

  *errorCode = 0;  

  /** Retrieve the temperature increment for F */
  if( unit == ApiUnit_Temperature_F()){
    result = API_TEMP_F_ROUNDING_INCREMENT;
  }

  /** Retrieve the temperature  increment for C */
  else if( unit == ApiUnit_Temperature_C()){
    result = API_TEMP_C_ROUNDING_INCREMENT;
  }

  /** Otherwise the unit is not valid */
  else{
    *errorCode = INVALID_UNIT;
  }
  
  return result;
};


/**
* Determines whether we are in the correct temperature range.
* We already checked that quant is not null.
*
* @param quant the quantity for which to check the range
* @param errorCode the variable in wich to report errors if any.
*/ 
bool ApiTemperature_inAllowedRange(ApiQuantity *quant,
				  int *errorCode){
  bool result = FALSE;
  int compare;

  /** Initialize AllowedTemp if necessary */
  if( AllowedTemp == NULL){
    double range[] =  API_ALLOWED_TEMPERATURE_RANGE;
    int i;

    *errorCode = 0;
    AllowedTemp = 
	   (ApiTemperature**)malloc( sizeof(ApiTemperature*) * 2  ); 
    if( AllowedTemp == NULL){
      *errorCode = INITIALIZE_FAILED;
      return result;
    }
    for( i = 0; i < 2; i++){
       AllowedTemp[i]  = ApiTemperature_init( range[i],
					      ApiUnit_Temperature_F(),
					      FALSE,
					      errorCode);
       if( *errorCode != 0 || AllowedTemp[i] == NULL){
	 *errorCode  = INITIALIZE_FAILED;
	 return result;
       }
    }
  }

  /** now compare the data */
  compare = ApiQuantity_compareTo((ApiQuantity*)quant,
				  (ApiQuantity*)AllowedTemp[0],
				  errorCode);
  if( *errorCode != 0){
      return result;
  }
  if( compare == -1){
    *errorCode = QUANTITY_OUT_OFF_RANGE;
    return result;
  }
  compare = ApiQuantity_compareTo((ApiQuantity*)quant,
				  (ApiQuantity*)AllowedTemp[1],
				  errorCode);
  if( *errorCode != 0){
      return result;
  }
  if( compare == +1){
    *errorCode = QUANTITY_OUT_OFF_RANGE;
    return result;
  }

  /** Quantity is in range - return TRUE */
  result = TRUE;
  return result;  
};


/**
* The temperature specific function to retrieve the VTable
*/
ApiQuantity_VTable * ApiTemperature_getVTable(){
   /** Initialize the VTable if necessary */
   if( ApiTemperatureVTable == 0){
     /** Create the table and set all values to 0 */
      ApiTemperatureVTable =
          (ApiQuantity_VTable *)malloc( sizeof(ApiQuantity_VTable) );
      if( ApiTemperatureVTable == 0 )  return 0;
      memset( ApiTemperatureVTable, 0, sizeof(ApiQuantity_VTable));

      /** And fill the Vtable */
      ApiTemperatureVTable->getValue = ApiTemperature_getValue;
      ApiTemperatureVTable->destroy = ApiQuantity_destroy_internal;
      ApiTemperatureVTable->roundingIncrement = 
                          ApiTemperature_roundingIncrement;
      ApiTemperatureVTable->inAllowedRange = 
	                      ApiTemperature_inAllowedRange;

      /** And assign the acceptable units */   
      ApiTemperatureVTable->acceptableUnits = 
                (ApiUnit**)malloc( sizeof(ApiUnit*) * 3);
      ApiTemperatureVTable->acceptableUnits[0] = ApiUnit_Temperature_F();
      ApiTemperatureVTable->acceptableUnits[1] = ApiUnit_Temperature_C();
      ApiTemperatureVTable->acceptableUnits[2] = NULL;

      ApiTemperatureVTable->standardUnit = ApiUnit_Temperature_F();
      ApiTemperatureVTable->quantityName = (char*)malloc( sizeof(char) *  12);
      if(   ApiTemperatureVTable->quantityName != NULL){
        memset(ApiTemperatureVTable->quantityName , 0, sizeof(char)*12);
        strcat( ApiTemperatureVTable->quantityName, "Temperature");
      }
   }
   return ApiTemperatureVTable;
};


/**
* The temperature specific initialization routine
*/

ApiTemperature * ApiTemperature_init(double value,
				     ApiUnit *unit,
				     int isMutable,
				     int * errorCode){
   ApiTemperature * temp = 0;

   *errorCode = 0;

   /** Create the object itself */
   temp = (ApiTemperature*)malloc( sizeof(ApiTemperature));
   if( temp == 0){
        *errorCode = INITIALIZE_FAILED;
	return temp;
   }
   memset( temp, 0, sizeof(ApiTemperature));

   /** Initialize our part of quant before we pass it on to parent */
   temp->quant.getVTable = ApiTemperature_getVTable;


   /** And finally let the quant object initialize itself */
   *errorCode =  ApiQuantity_initialize((ApiQuantity*)temp,
					value,
					unit,
					isMutable);

   /** And return the new object*/
   return temp;
}

void ApiTemperature_convertToIPTS68(ApiTemperature * temp,
                                    int *errorCode){
    double value,scaled,delta=0,factor;
    double consts[] = API_TEMP_IPTS68_CONSTANTS;
    int i;

    if( temp == 0){
       *errorCode = INITIALIZE_FAILED;
       return;
    }
    if( temp->quant.wasCorrected != FALSE || 
        temp->quant.isMutable == FALSE){
       *errorCode = CHANGED_IMMUTABLE;
       return;        
    }
    
    *errorCode = 0;    

    /* Step 1: Get temperature in C */
    value = ApiQuantity_getValue((ApiQuantity*)temp,
                                 ApiUnit_Temperature_C(),
				 0,
				 errorCode);
    if( *errorCode != 0) return;
         
    /* Step 3: Get the scaled value */
     scaled = value/API_TEMP_IPTS68_SCALER;

     /* Step 3 cont.: Caluclate correction factor Delta_t */
     factor = scaled;
     delta = 0;
     for( i = 0; i < 8; i++){
	 delta += consts[i] * factor;
         factor *= scaled;
     }

     /* Step 4: Correct the temperature value */
     value -= delta;

     /* Finish: Set the currentValue and currentUnit to the
	corrected values */
     temp->quant.currentValue = value;
     temp->quant.currentUnit = ApiUnit_Temperature_C();
     temp->quant.wasCorrected = TRUE;
}


bool ApiTemperature_wasIPTS68Corrected(ApiTemperature * temp,
				       int *errorCode){
    bool result = FALSE;

    *errorCode = 0;

    if( temp == 0){
       *errorCode = INITIALIZE_FAILED;
       return result;
    }
    result = temp->quant.wasCorrected;
    return result;
}


/**
* Implemenation for Pressure
*/

/** 
* The actual structure for Pressure
*/
struct api_pressure_{
  ApiQuantity quant;
};

/**
* The static instance of the pressure specific VTable 
*/
static ApiQuantity_VTable * ApiPressureVTable = NULL;


/** The  range of allowed Pressures */
static ApiPressure **AllowedPres = 0;

/**
* The Pressure specific getValue function.
* We already checked that quant is not null and that the unit
* given is an acceptable unit
*/
double ApiPressure_getValue(ApiQuantity * quant,
			    ApiUnit *unit,
			    int rounded,
			    int * errorCode){
  double result = 0;

  *errorCode = 0;

  /** Retrieve the pressure in psi - convert if needed */
  if( unit == ApiUnit_Pressure_PSI() ){
    if( quant->currentUnit == ApiUnit_Pressure_PSI() ){
       result = quant->currentValue;
    }
    else if(  quant->currentUnit == ApiUnit_Pressure_KPA() ){
      result =  quant->currentValue / API_PRESSURE_KPA_TO_PSI_FACTOR; 
    }
    else if(  quant->currentUnit == ApiUnit_Pressure_BAR() ){
      result =  quant->currentValue / API_PRESSURE_BAR_TO_PSI_FACTOR; 
    }
  }

  /** Retrieve the pressure in kPa - convert if needed */
  else if( unit == ApiUnit_Pressure_KPA() ){
    if( quant->currentUnit == ApiUnit_Pressure_KPA() ){
       result = quant->currentValue;
    }
    else if(  quant->currentUnit == ApiUnit_Pressure_PSI() ){
      result =  quant->currentValue *  API_PRESSURE_KPA_TO_PSI_FACTOR; 
    }
    else if(  quant->currentUnit == ApiUnit_Pressure_BAR() ){       
      result =  quant->currentValue * API_PRESSURE_KPA_TO_PSI_FACTOR 
                / API_PRESSURE_BAR_TO_PSI_FACTOR; 
    }
  }

  /** Retrieve the pressure in bar - convert if needed */
  else if( unit == ApiUnit_Pressure_BAR() ){
    if( quant->currentUnit == ApiUnit_Pressure_BAR() ){
       result = quant->currentValue;
    }
    else if(  quant->currentUnit == ApiUnit_Pressure_PSI() ){
      result =  quant->currentValue *  API_PRESSURE_BAR_TO_PSI_FACTOR; 
    }
    else if(  quant->currentUnit == ApiUnit_Pressure_KPA() ){
      result =  quant->currentValue * API_PRESSURE_BAR_TO_PSI_FACTOR 
                / API_PRESSURE_KPA_TO_PSI_FACTOR; 
    }
  }


  /** Otherwise the unit is not valid */
  else{
    *errorCode = INVALID_UNIT;
  }

  /** The rounding is done in the ApiQuantity function */
  return result;
};

/**
* Return the pressure specific rounding increment.
* We already checked that quant is not null and that the unit
* is an acceptable unit.
*
* @param unit the unit for  which to retrieve the rounding increment
* @param errorCode the error code that gets set if there was an error
*
* @return the correct rounding value
*/
double ApiPressure_roundingIncrement(ApiUnit *unit,
				     int *errorCode){
  double result = 0;

  *errorCode = 0;

  /** Retrieve the pressure increment for psi */
  if( unit == ApiUnit_Pressure_PSI()){
    result = API_RESSURE_PSI_ROUNDING_INCREMENT;
  }

  /** Retrieve the pressure increment for kPa */
  else if( unit == ApiUnit_Pressure_KPA()){
    result = API_PRESSURE_KPA_ROUNDING_INCREMENT;
  }

  /** Retrieve the pressure increment for bar */
  else if( unit == ApiUnit_Pressure_BAR()){
    result = API_PRESSURE_BAR_ROUNDING_INCREMENT;
  }

  /** Otherwise the unit is not valid */
  else{
    *errorCode = INVALID_UNIT;
  }
  
  return result;
};

/**
* Determines whether we are in the correct pressure range
* Pressures below 0 are set to the lower range.
* We already checked that quant is not null.
*
* @param quant the quantity for which to check the range
* @param errorCode the variable in wich to report errors if any.
*/ 
bool ApiPressure_inAllowedRange(ApiQuantity *quant,
				int *errorCode){
  bool result = FALSE;
  int compare;
  double range[] =  API_ALLOWED_PRESSURE_RANGE;

  /** Initialize AllowedPres if necessary */
  if( AllowedPres == NULL){
    int i;

    *errorCode = 0;
    AllowedPres = (ApiPressure**)malloc( sizeof(ApiPressure*) * 2  );
    if( AllowedPres == NULL){
      *errorCode = INITIALIZE_FAILED;
      return result;
    }
    for( i = 0; i < 2; i++){
       AllowedPres[i]  = ApiPressure_init( range[i],
					   ApiUnit_Pressure_PSI(),
					   FALSE,
					   errorCode);
       if( *errorCode != 0 || AllowedPres[i] == NULL){
	 *errorCode  = INITIALIZE_FAILED;
	 return result;
       }
    }
  }

  /** now compare the data */
  compare = ApiQuantity_compareTo((ApiQuantity*)quant,
				  (ApiQuantity*)AllowedPres[0],
				  errorCode);
  if( *errorCode != 0){
      return result;
  }
  if( compare == -1){
    *errorCode =  ApiQuantity_setValue(quant, range[0],ApiUnit_Pressure_PSI()); 
    if( *errorCode != 0) {   /** We tried to change an immutable value */
         *errorCode = QUANTITY_OUT_OFF_RANGE;
        return result;
    }
  }
  compare = ApiQuantity_compareTo((ApiQuantity*)quant,
				  (ApiQuantity*)AllowedPres[1],
				  errorCode);
  if( *errorCode != 0){
      return result;
  }
  if( compare == +1){
    *errorCode = QUANTITY_OUT_OFF_RANGE;
    return result;
  }

  /** Quantity is in range - return TRUE */
  result = TRUE;
  return result;
    
};


/**
* The pressure specific function to retrieve the VTable
*/
ApiQuantity_VTable * ApiPressure_getVTable(){
   /** Initialize the VTable if necessary */
  if( ApiPressureVTable == 0){
     /** Create the table and set all values to 0 */
      ApiPressureVTable =
          (ApiQuantity_VTable *)malloc( sizeof(ApiQuantity_VTable) );
      if( ApiPressureVTable == NULL )  return NULL;
      memset( ApiPressureVTable, 0, sizeof(ApiQuantity_VTable));

      /** And fill the Vtable */
      ApiPressureVTable->getValue = ApiPressure_getValue;
      ApiPressureVTable->destroy = ApiQuantity_destroy_internal;
      ApiPressureVTable->roundingIncrement = ApiPressure_roundingIncrement;
      ApiPressureVTable->inAllowedRange =  ApiPressure_inAllowedRange;

      /** And assign the acceptable units */   
      ApiPressureVTable->acceptableUnits = 
                (ApiUnit**)malloc( sizeof(ApiUnit*) * 4);
      ApiPressureVTable->acceptableUnits[0] = ApiUnit_Pressure_PSI(); 
      ApiPressureVTable->acceptableUnits[1] = ApiUnit_Pressure_KPA();
      ApiPressureVTable->acceptableUnits[2] = ApiUnit_Pressure_BAR();
      ApiPressureVTable->acceptableUnits[3] = NULL;
 
      ApiPressureVTable->standardUnit = ApiUnit_Pressure_PSI();
      ApiPressureVTable->quantityName = (char*)malloc( sizeof(char) *  9);
      if(   ApiPressureVTable->quantityName != NULL){
        memset(ApiPressureVTable->quantityName , 0, sizeof(char)*9);
        strcat( ApiPressureVTable->quantityName, "Pressure");
      }
   }
   return ApiPressureVTable;
}; 


/**
* The pressure specific initialization routine
*/
ApiPressure * ApiPressure_init(double value,
			       ApiUnit *unit,
			       int isMutable,
			       int * errorCode){
   ApiPressure * temp = 0;

   *errorCode = 0;

   /** Create the object itself */
   temp = (ApiPressure*)malloc( sizeof(ApiPressure));
   if( temp == 0){
        *errorCode = INITIALIZE_FAILED;
	return temp;
   }
   memset( temp, 0, sizeof(ApiPressure));

   /** Initialize our part of quant before we pass it on to parent */
   temp->quant.getVTable = ApiPressure_getVTable;


   /** And finally let the quant object initialize itself */
   *errorCode =  ApiQuantity_initialize((ApiQuantity*)temp,
					value,
					unit,
					isMutable);

   /** And return the new object*/
   return temp;
}


/**
* Implemenation for Density
*/

/** 
* The actual structure for Density
*/
struct api_density_{
  ApiQuantity quant;
};

/**
* The static instance of the density specific VTable 
*/
static ApiQuantity_VTable * ApiDensityVTable = NULL;


/** The  range of allowed Pressures */
static ApiDensity **AllowedDens = 0;

/**
 * Convert the value into kg/m^3
 *
 * @param value the value to convert
 * @param unit the unit in which the value is currently in
 *
 * @return the value converted to kg/m^3
 */
double ApiPressure_convertToKGM3( double value,
				  ApiUnit * unit){
  double result = value;

  if( unit == ApiUnit_Density_API()){
      result += API_DENSITY_API_TO_KGM3_LOWER;
      result  = API_DENSITY_API_TO_KGM3_UPPER/result;
      result *= API_DENSITY_RELATIVE_TO_KGM3_FACTOR;
  }
  else if( unit == ApiUnit_Density_RELATIVE()){
    result *=  API_DENSITY_RELATIVE_TO_KGM3_FACTOR;
  }   

  return result;
};

/**
* The Pressure specific getValue function.
* We already checked that quant is not NULL and that the 
* unit is acceptable.
*/
double ApiDensity_getValue(ApiQuantity * quant,
			   ApiUnit *unit,
			   int rounded,
			   int * errorCode){
  double result = 0,tmp;

  *errorCode = 0;

  /** Retrieve the density in kg/m^3 - convert if needed */
  if( unit == ApiUnit_Density_KGM3() ){
    if( quant->currentUnit == ApiUnit_Density_KGM3() ){
       result = quant->currentValue;
    }
    else{
      result = ApiPressure_convertToKGM3( quant->currentValue, 
					  quant->currentUnit);
    }
  }

  /** Retrieve the density in API - convert if needed */
  else if( unit == ApiUnit_Density_API() ){
    if( quant->currentUnit == ApiUnit_Density_API() ){
       result = quant->currentValue;
    }
    else{
       /** first convert to kg/m^3 */
       result = ApiPressure_convertToKGM3( quant->currentValue, 
					   quant->currentUnit);
 
       /** and then convert to API */
       tmp = API_DENSITY_RELATIVE_TO_KGM3_FACTOR *
  	     API_DENSITY_API_TO_KGM3_UPPER;    
       result  = tmp/result;
       result -= API_DENSITY_API_TO_KGM3_LOWER;
    }
  }

  /** Retrieve the density in Relative Density - convert if needed */
  else if( unit == ApiUnit_Density_RELATIVE()){
    if( quant->currentUnit == ApiUnit_Density_RELATIVE() ){
       result = quant->currentValue;
    }
    else {
        result =   ApiPressure_convertToKGM3( quant->currentValue, 
					      quant->currentUnit);
	result /= API_DENSITY_RELATIVE_TO_KGM3_FACTOR;
    }
  }


  /** Otherwise the unit is not valid */
  else{
    *errorCode = INVALID_UNIT;
  }

  /** The rounding is done in the ApiQuantity function */
  return result;
};


/**
* Return the density specific rounding increment.
* We already checked that quant is not null and that the unit
* is an acceptable unit.
*
* @param unit the unit for  which to retrieve the rounding increment
* @param errorCode the error code that gets set if there was an error
*
* @return the correct rounding value
*/
double ApiDensity_roundingIncrement(ApiUnit *unit,
				    int *errorCode){
  double result = 0;

  *errorCode = 0;

  /** Retrieve the density increment for API */
  if( unit ==  ApiUnit_Density_API()){
    result = API_DENSITY_API_ROUNDING_INCREMENT;
  }

  /** Retrieve the density increment for kg/m^3 */
  else if( unit == ApiUnit_Density_KGM3()){
    result = API_DENSITY_KGM3_ROUNDING_INCREMENT;
  }

  /** Retrieve the density increment for Relative Density */
  else if( unit == ApiUnit_Density_RELATIVE()){
    result = API_DENSITY_RELATIVE_ROUNDING_INCREMENT;
  }

  /** Otherwise the unit is not valid */
  else{
    *errorCode = INVALID_UNIT;
  }
  
  return result;
};


/**
* Determines whether we are in the correct density range
* We already checked that quant is not null.  This function 
* always returns an error as the density range is dependent on the 
* commodity type.
*
* @param quant the quantity for which to check the range
* @param errorCode the variable in wich to report errors if any.
*/ 
bool ApiDensity_inAllowedRange(ApiQuantity *quant,
			       int *errorCode){
  *errorCode = UNSUPPORTED_FUNCTION;
  return FALSE;    
};



/**
* The density specific function to retrieve the VTable
*/
ApiQuantity_VTable * ApiDensity_getVTable(){
   /** Initialize the VTable if necessary */
   if( ApiDensityVTable == 0){
     /** Create the table and set all values to 0 */
      ApiDensityVTable =
          (ApiQuantity_VTable *)malloc( sizeof(ApiQuantity_VTable) );
      if( ApiDensityVTable == NULL )  return NULL;
      memset( ApiDensityVTable, 0, sizeof(ApiQuantity_VTable));

      /** And fill the Vtable */
      ApiDensityVTable->getValue = ApiDensity_getValue;
      ApiDensityVTable->destroy = ApiQuantity_destroy_internal;
      ApiDensityVTable->roundingIncrement = ApiDensity_roundingIncrement;
      ApiDensityVTable->inAllowedRange =  ApiDensity_inAllowedRange;

      /** And assign the acceptable units */   
      ApiDensityVTable->acceptableUnits = 
                (ApiUnit**)malloc( sizeof(ApiUnit*) * 4);
      ApiDensityVTable->acceptableUnits[0] = ApiUnit_Density_KGM3();
      ApiDensityVTable->acceptableUnits[1] = ApiUnit_Density_RELATIVE();
      ApiDensityVTable->acceptableUnits[2] = ApiUnit_Density_API();
      ApiDensityVTable->acceptableUnits[3] = NULL;

      ApiDensityVTable->standardUnit = ApiUnit_Density_KGM3();
      ApiDensityVTable->quantityName = (char*)malloc( sizeof(char) *  8);
      if(   ApiDensityVTable->quantityName != NULL){
        memset(ApiDensityVTable->quantityName , 0, sizeof(char)*8);
        strcat( ApiDensityVTable->quantityName, "Density");
      }
   }
   return ApiDensityVTable;
};


/**
* The density specific initialization routine
*/
ApiDensity * ApiDensity_init(double value,
			     ApiUnit *unit,
			     int isMutable,
			     int * errorCode){
   ApiDensity * temp = 0;

   *errorCode = 0;

   /** Create the object itself */
   temp = (ApiDensity*)malloc( sizeof(ApiDensity));
   if( temp == 0){
        *errorCode = INITIALIZE_FAILED;
	return temp;
   }
   memset( temp, 0, sizeof(ApiDensity));

   /** Initialize our part of quant before we pass it on to parent */
   temp->quant.getVTable = ApiDensity_getVTable;


   /** And finally let the quant object initialize itself */
   *errorCode =  ApiQuantity_initialize((ApiQuantity*)temp,
					value,
					unit,
					isMutable);

   /** And return the new object*/
   return temp;
}


bool ApiDensity_inRhoRange(ApiDensity * density,
                           ApiOilProduct * product, 
                           bool type2, 
                           int *errorCode){
  bool result = FALSE;

  if( density == NULL || product == NULL){
      *errorCode = NULL_POINTER_EXCEPTION;
       return result;
  }

  *errorCode = 0;

  if( type2 == FALSE){    /* use strict limits */
     result = ApiOilProduct_isInRange(product, density, errorCode);
     if( *errorCode != 0){
       result = FALSE;
       return result;
     }
  }

  else{             /* lesser limits for Type II calculation */
     result = ApiOilProduct_isInRhoRange(product, density, errorCode);
     if( *errorCode != 0){
       result = FALSE;
       return result;
     }
  }

  if( result == FALSE) *errorCode = QUANTITY_OUT_OFF_RANGE;

  return result;
}

/**
* Implemenation for Volume
*/

/** 
* The actual structure for Volume
*/
struct api_volume_{
  ApiQuantity quant;
};

/**
* The static instance of the volume specific VTable 
*/
static ApiQuantity_VTable * ApiVolumeVTable = NULL;


/**
* The Volume specific getValue function.
* We already checked that quant is not NULL and that the 
* unit is acceptable.
*/
double ApiVolume_getValue(ApiQuantity * quant,
			  ApiUnit *unit,
			  int rounded,
			  int * errorCode){
  double result = 0;

  *errorCode = 0;

  /** Only the current unit is allowed */
  if( unit != quant->currentUnit){
    *errorCode = INVALID_UNIT;
    return result;
  }
  result = quant->currentValue;

  return result;
};

/**
* Return the volume specific rounding increment.
* We already checked that unit is not NULL and that the unit is valid.
*
* @param unit the unit for  which to retrieve the rounding increment
* @param errorCode the error code that gets set if there was an error
*
* @return the correct rounding value
*/
double ApiVolume_roundingIncrement(ApiUnit *unit,
				   int *errorCode){
  double result = 0;

  *errorCode = UNSUPPORTED_FUNCTION;
  
  return result;
};

/**
* Determines whether we are in the correct pressure range
* This implementation returns always true.
*
* @param quant the quantity for which to check the range
* @param errorCode the variable in wich to report errors if any.

*/ 
bool ApiQuantity_genericInAllowedRange(ApiQuantity *quant,
				int *errorCode){
  return TRUE;
};

/**
* Get the standard unit of the the volume, which is always the given unit
*/ 
ApiUnit * ApiVolume_standardUnit(ApiQuantity * quant){
  ApiUnit *unit = NULL;

  if( quant == NULL) return unit;
  unit = quant->givenUnit;
  return unit;
};


/**
* The volume specific function to retrieve the VTable
*/
ApiQuantity_VTable * ApiVolume_getVTable(){
   /** Initialize the VTable if necessary */
   if( ApiVolumeVTable == 0){
     /** Create the table and set all values to 0 */
      ApiVolumeVTable =
          (ApiQuantity_VTable *)malloc( sizeof(ApiQuantity_VTable) );
      if( ApiVolumeVTable == NULL )  return NULL;
      memset( ApiVolumeVTable, 0, sizeof(ApiQuantity_VTable));

      /** And fill the Vtable */
      ApiVolumeVTable->getValue = ApiVolume_getValue;
      ApiVolumeVTable->destroy = ApiQuantity_destroy_internal;
      ApiVolumeVTable->roundingIncrement = ApiVolume_roundingIncrement;
      ApiVolumeVTable->inAllowedRange =  ApiQuantity_genericInAllowedRange;
      ApiVolumeVTable->getStandardUnit = ApiVolume_standardUnit;

      /** And assign the acceptable units */   
      ApiVolumeVTable->acceptableUnits = 
                (ApiUnit**)malloc( sizeof(ApiUnit*) * 4);
      ApiVolumeVTable->acceptableUnits[0] = ApiUnit_Volume_BARREL(); 
      ApiVolumeVTable->acceptableUnits[1] = ApiUnit_Volume_LITER();
      ApiVolumeVTable->acceptableUnits[2] = ApiUnit_Volume_M3();
      ApiVolumeVTable->acceptableUnits[3] = NULL;

      ApiVolumeVTable->standardUnit = ApiUnit_Volume_BARREL();
      ApiVolumeVTable->quantityName = (char*)malloc( sizeof(char) *  7);
      if(   ApiVolumeVTable->quantityName != NULL){
        memset(ApiVolumeVTable->quantityName , 0, sizeof(char)*7);
        strcat( ApiVolumeVTable->quantityName, "Volume");
      }
   }
   return ApiVolumeVTable;
};


/**
* The Volume specific initialization routine
*/
ApiVolume * ApiVolume_init(double value,
			     ApiUnit *unit,
			     int isMutable,
			     int * errorCode){
   ApiVolume * vol = 0;

   *errorCode = 0;

   /** Create the object itself */
   vol = (ApiVolume*)malloc( sizeof(ApiVolume));
   if( vol == 0){
        *errorCode = INITIALIZE_FAILED;
	return vol;
   }
   memset( vol, 0, sizeof(ApiVolume));

   /** Initialize our part of quant before we pass it on to parent */
   vol->quant.getVTable = ApiVolume_getVTable;


   /** And finally let the quant object initialize itself */
   *errorCode =  ApiQuantity_initialize((ApiQuantity*)vol,
					value,
					unit,
					isMutable);

   /** And return the new object*/
   return vol;
}


/**
* Implemenation for Expansion Factor
*/

/** 
* The actual structure for ExpansionFactor
*/
struct api_expansion_factor_{
  ApiQuantity quant;
};

/**
* The static instance of the expansion factor specific VTable 
*/
static ApiQuantity_VTable * ApiExpansionFactorVTable = NULL;


/**
* The ExpansionFactor specific getValue function.
* We already checked that quant is not NULL and that the 
* unit is acceptable.
*/
double ApiExpansionFactor_getValue(ApiQuantity * quant,
				   ApiUnit *unit,
				   int rounded,
				   int * errorCode){
  double result = 0;

  *errorCode = 0;

  result = quant->currentValue;

  return result;
};


/**
* Return the volume specific rounding increment.
* We already checked that unit is not NULL and that the unit is valid.
*
* @param unit the unit for  which to retrieve the rounding increment
* @param errorCode the error code that gets set if there was an error
*
* @return the correct rounding value
*/
double ApiExpansionFactor_roundingIncrement(ApiUnit *unit,
					    int *errorCode){
  double result = 0;

  *errorCode = 0;

  result =  API_EXPANSION_FACTOR_ROUNDING_INCREMENT;
  
  return result;
};


/**
* The expansion factor specific function to retrieve the VTable
*/
ApiQuantity_VTable * ApiExpansionFactor_getVTable(){
   /** Initialize the VTable if necessary */
   if( ApiExpansionFactorVTable == 0){
     /** Create the table and set all values to 0 */
      ApiExpansionFactorVTable =
          (ApiQuantity_VTable *)malloc( sizeof(ApiQuantity_VTable) );
      if( ApiExpansionFactorVTable == NULL )  return NULL;
      memset( ApiExpansionFactorVTable, 0, sizeof(ApiQuantity_VTable));

      /** And fill the Vtable */
      ApiExpansionFactorVTable->getValue = ApiExpansionFactor_getValue;
      ApiExpansionFactorVTable->destroy = ApiQuantity_destroy_internal;
      ApiExpansionFactorVTable->roundingIncrement = ApiExpansionFactor_roundingIncrement;
      ApiExpansionFactorVTable->inAllowedRange =  ApiQuantity_genericInAllowedRange;

      /** And assign the acceptable units */   
      ApiExpansionFactorVTable->acceptableUnits = 
                (ApiUnit**)malloc( sizeof(ApiUnit*) * 2);
      ApiExpansionFactorVTable->acceptableUnits[0] = ApiUnit_Expansion_DIMLESS(); 
      ApiExpansionFactorVTable->acceptableUnits[1] = NULL;

      ApiExpansionFactorVTable->standardUnit = ApiUnit_Expansion_DIMLESS();
      ApiExpansionFactorVTable->quantityName = (char*)malloc( sizeof(char) *  17);
      if(   ApiExpansionFactorVTable->quantityName != NULL){
        memset(ApiExpansionFactorVTable->quantityName , 0, sizeof(char)*17);
        strcat( ApiExpansionFactorVTable->quantityName, "Expansion Factor");
      }
   }
   return ApiExpansionFactorVTable;
};


/**
* The Expansion Factor  specific initialization routine
*/
ApiExpansionFactor * ApiExpansionFactor_init(double value,
					     ApiUnit *unit,
					     int isMutable,
					     int * errorCode){
   ApiExpansionFactor * vol = 0;

   *errorCode = 0;

   /** Create the object itself */
   vol = (ApiExpansionFactor*)malloc( sizeof(ApiExpansionFactor));
   if( vol == 0){
        *errorCode = INITIALIZE_FAILED;
	return vol;
   }
   memset( vol, 0, sizeof(ApiExpansionFactor));

   /** Initialize our part of quant before we pass it on to parent */
   vol->quant.getVTable = ApiExpansionFactor_getVTable;


   /** And finally let the quant object initialize itself */
   *errorCode =  ApiQuantity_initialize((ApiQuantity*)vol,
					value,
					unit,
					isMutable);

   /** And return the new object*/
   return vol;
}


/**
* Implemenation for the Scaled Compression factor
*/

/** 
* The actual structure for the Scaled Compression factor
*/
struct api_scaled_factor_{
  ApiQuantity quant;
};

/**
* The static instance of the pressure specific VTable 
*/
static ApiQuantity_VTable * ApiScaledFactorVTable = NULL;

/**
* The Scaled Compression Factor specific getValue function.
* We already checked that quant is not NULL and that the 
* unit is acceptable.

*/
double ApiScaledFactor_getValue(ApiQuantity * quant,
				ApiUnit *unit,
				int rounded,
				int * errorCode){
  double result = 0;

  *errorCode = 0;

  /** Retrieve the compression factor  in 1/psi - convert if needed */
  if( unit == ApiUnit_ScaledComp_REV_PSI() ){
    if( quant->currentUnit == ApiUnit_ScaledComp_REV_PSI() ){
       result = quant->currentValue;
    }
    else if(  quant->currentUnit == ApiUnit_ScaledComp_REV_KPA() ){
      result =  quant->currentValue * API_PRESSURE_KPA_TO_PSI_FACTOR ; 
    }
    else if(  quant->currentUnit == ApiUnit_ScaledComp_REV_BAR() ){
      result =  quant->currentValue * API_PRESSURE_BAR_TO_PSI_FACTOR; 
    }
  }

  /** Retrieve the pressure in 1/kPa - convert if needed */
  else if( unit == ApiUnit_ScaledComp_REV_KPA()  ){
    if( quant->currentUnit == ApiUnit_ScaledComp_REV_KPA() ){
       result = quant->currentValue;
    }
    else if(  quant->currentUnit ==  ApiUnit_ScaledComp_REV_PSI() ){
      result =  quant->currentValue /  API_PRESSURE_KPA_TO_PSI_FACTOR; 
    }
    else if(  quant->currentUnit == ApiUnit_ScaledComp_REV_BAR() ){       
      result =  quant->currentValue * API_PRESSURE_BAR_TO_PSI_FACTOR
 	       / API_PRESSURE_KPA_TO_PSI_FACTOR;
    }
  }

  /** Retrieve the pressure in 1/bar - convert if needed */
  else if( unit ==  ApiUnit_ScaledComp_REV_BAR()){
    if( quant->currentUnit == ApiUnit_ScaledComp_REV_BAR() ){
       result = quant->currentValue;
    }
    else if(  quant->currentUnit ==  ApiUnit_ScaledComp_REV_PSI() ){
      result =  quant->currentValue /  API_PRESSURE_BAR_TO_PSI_FACTOR; 
    }
    else if(  quant->currentUnit == ApiUnit_ScaledComp_REV_KPA() ){
      result =  quant->currentValue * API_PRESSURE_KPA_TO_PSI_FACTOR
       	        / API_PRESSURE_BAR_TO_PSI_FACTOR;
    }
  }


  /** Otherwise the unit is not valid */
  else{
    *errorCode = INVALID_UNIT;
  }

  /** The rounding is done in the ApiQuantity function */
  return result;
};


/**
* Return the Scaled Compression Factor specific rounding increment.
* We already checked that unit is not null and that the unit is an 
* acceptable unit.
*
* @param unit the unit for  which to retrieve the rounding increment
* @param errorCode the error code that gets set if there was an error
*
* @return the correct rounding value
*/
double ApiScaledFactor_roundingIncrement( ApiUnit *unit,
					 int *errorCode){
  double result = 0;

  *errorCode = 0;

  /** Retrieve the pressure increment for 1/psi */
  if( unit == ApiUnit_ScaledComp_REV_PSI() ){
    result = API_SCALED_COMP_REV_PSI_ROUNDING_INCREMENT;
  }

  /** Retrieve the pressure increment for 1/kPa */
  else if( unit == ApiUnit_ScaledComp_REV_KPA()){
    result = API_SCALED_COMP_REV_KPA_ROUNDING_INCREMENT;;
  }

  /** Retrieve the pressure increment for 1/bar */
  else if( unit == ApiUnit_ScaledComp_REV_BAR() ){
    result = API_SCALED_COMP_REV_BAR_ROUNDING_INCREMENT;
  }

  /** Otherwise the unit is not valid */
  else{
    *errorCode = INVALID_UNIT;
  }
  
  return result;
};


/**
* The Scaled Compression Factor specific function to retrieve the VTable
*/
ApiQuantity_VTable * ApiScaledFactor_getVTable(){
   /** Initialize the VTable if necessary */
   if( ApiScaledFactorVTable == 0){
     /** Create the table and set all values to 0 */
      ApiScaledFactorVTable =
          (ApiQuantity_VTable *)malloc( sizeof(ApiQuantity_VTable) );
      if( ApiScaledFactorVTable == NULL )  return NULL;
      memset( ApiScaledFactorVTable, 0, sizeof(ApiQuantity_VTable));

      /** And fill the Vtable */
      ApiScaledFactorVTable->getValue = ApiScaledFactor_getValue;
      ApiScaledFactorVTable->destroy = ApiQuantity_destroy_internal;
      ApiScaledFactorVTable->roundingIncrement = ApiScaledFactor_roundingIncrement;
      ApiScaledFactorVTable->inAllowedRange =  ApiQuantity_genericInAllowedRange;

      /** And assign the acceptable units */   
      ApiScaledFactorVTable->acceptableUnits = 
                (ApiUnit**)malloc( sizeof(ApiUnit*) * 4);
      ApiScaledFactorVTable->acceptableUnits[0] = ApiUnit_ScaledComp_REV_PSI(); 
      ApiScaledFactorVTable->acceptableUnits[1] = ApiUnit_ScaledComp_REV_KPA();
      ApiScaledFactorVTable->acceptableUnits[2] = ApiUnit_ScaledComp_REV_BAR();
      ApiScaledFactorVTable->acceptableUnits[3] = NULL;

      ApiScaledFactorVTable->standardUnit = ApiUnit_ScaledComp_REV_PSI();
      ApiScaledFactorVTable->quantityName = (char*)malloc( sizeof(char) *  26);
      if(   ApiScaledFactorVTable->quantityName != NULL){
        memset(ApiScaledFactorVTable->quantityName , 0, sizeof(char)*26);
        strcat( ApiScaledFactorVTable->quantityName, "Scaled Compression Factor");
      }
   }
   return ApiScaledFactorVTable;
};


/**
* The Scaled Compression factor specific initialization routine
*/
ApiScaledFactor * ApiScaledFactor_init(double value,
				       ApiUnit *unit,
				       int isMutable,
				       int * errorCode){
   ApiScaledFactor * temp = 0;

   *errorCode = 0;

   /** Create the object itself */
   temp = (ApiScaledFactor*)malloc( sizeof(ApiScaledFactor));
   if( temp == 0){
        *errorCode = INITIALIZE_FAILED;
	return temp;
   }
   memset( temp, 0, sizeof(ApiScaledFactor));

   /** Initialize our part of quant before we pass it on to parent */
   temp->quant.getVTable = ApiScaledFactor_getVTable;


   /** And finally let the quant object initialize itself */
   *errorCode =  ApiQuantity_initialize((ApiQuantity*)temp,
					value,
					unit,
					isMutable);

   /** And return the new object*/
   return temp;
}


/**
* Implemenation for ApiAlpha60
*/
/** 
* The actual structure for ApiAlpha60
*/
struct api_alpha60_{
  ApiQuantity quant;
};


/**
* The static instance of the ApiAlpha60  specific VTable 
*/
static ApiQuantity_VTable * ApiAlpha60VTable = 0;


/** The  range of allowed temperatures */
static ApiAlpha60 **AllowedAlpha60 = 0;


/**
* The ApiAlpha60 specific getValue function.
* We already checked that quant is not null and that the unit
* given is an acceptable unit
*/
double ApiAlpha60_getValue(ApiQuantity * quant,
			   ApiUnit *unit,
			   int rounded,
			   int * errorCode){
  double result = 0;

  *errorCode = 0;

  /** Retrieve the temperature in 1/F - convert if needed */
  if( unit == ApiUnit_ThermalExp_REV_F()){
    if( quant->currentUnit == ApiUnit_ThermalExp_REV_F()  ){
       result = quant->currentValue;
    }
    else {
      result =  quant->currentValue / API_THERMAL_EXPANSION_REVF_TO_REVC;
    }
  }

  /** Retrieve the temperature in C - convert if needed */
  else if( unit == ApiUnit_ThermalExp_REV_C() ){
    if( quant->currentUnit == ApiUnit_ThermalExp_REV_C() ){
       result = quant->currentValue;
    }
    else {
      result = quant->currentValue * API_THERMAL_EXPANSION_REVF_TO_REVC;
    }
  }

  /** Otherwise the unit is not valid */
  else{
    *errorCode = INVALID_UNIT;
  }

  /** The rounding is done in the ApiQuantity function */
  return result;
};


/**
* Return the Thermal Expansion Factor  specific rounding increment.
* We already checked that quant is not null and that the unit
* is an acceptable unit.
*
* @param unit the unit for  which to retrieve the rounding increment
* @param errorCode the error code that gets set if there was an error
*
* @return the correct rounding value
*/
double ApiAlpha60_roundingIncrement(ApiUnit *unit,
				    int *errorCode){
  double result = 0;

  *errorCode = 0;  

  /** Retrieve the  Thermal Expansion Factor  increment for 1/F */
  if( unit == ApiUnit_ThermalExp_REV_F() ){
    result = API_THERMAL_EXPANSION_F_ROUNDING_INCREMENT;
  }

  /** Retrieve the  Thermal Expansion Factor  increment for 1/C */
  else if( unit ==ApiUnit_ThermalExp_REV_C() ){
    result = API_THERMAL_EXPANSION_C_ROUNDING_INCREMENT;
  }

  /** Otherwise the unit is not valid */
  else{
    *errorCode = INVALID_UNIT;
  }
   
  return result;
};


/**
* Determines whether we are in the correct  range.
* We already checked that quant is not null.
*
* @param quant the quantity for which to check the range
* @param errorCode the variable in wich to report errors if any.
*/ 
bool ApiAlpha60_inAllowedRange(ApiQuantity *quant,
			       int *errorCode){
  bool result = FALSE;
  int compare;

  /** Initialize AllowedAlpha60 if necessary */
  if( AllowedAlpha60 == NULL){
    double range[] =  API_ALLOWED_THERMAL_EXPANSION_RANGE;
    int i;

    *errorCode = 0;
    AllowedAlpha60 = 
	   (ApiAlpha60**)malloc( sizeof(ApiAlpha60*) * 2  );
    if( AllowedAlpha60== NULL){
      *errorCode = INITIALIZE_FAILED;
      return result;
    }
    for( i = 0; i < 2; i++){
       AllowedAlpha60[i]  = ApiAlpha60_init( range[i],
					    ApiUnit_ThermalExp_REV_F(),
					    FALSE,
					    errorCode);
       if( *errorCode != 0 || AllowedAlpha60[i] == NULL){
	 *errorCode  = INITIALIZE_FAILED;
	 return result;
       }
    }
  }

  /** now compare the data */
  compare = ApiQuantity_compareTo((ApiQuantity*)quant,
				  (ApiQuantity*)AllowedAlpha60[0],
				  errorCode);
  if( *errorCode != 0){
      return result;
  }
  if( compare == -1){
    *errorCode = QUANTITY_OUT_OFF_RANGE;
    return result;
  }
  compare = ApiQuantity_compareTo((ApiQuantity*)quant,
				  (ApiQuantity*)AllowedAlpha60[1],
				  errorCode);
  if( *errorCode != 0){
      return result;
  }
  if( compare == +1){
    *errorCode = QUANTITY_OUT_OFF_RANGE;
    return result;
  }

  /** Quantity is in range - return TRUE */
  result = TRUE;
  return result;
    
};


/**
* The Thermal Expansion Factor  specific function to retrieve the VTable
*/
ApiQuantity_VTable * ApiAlpha60_getVTable(){
   /** Initialize the VTable if necessary */
   if( ApiAlpha60VTable == 0){
     /** Create the table and set all values to 0 */
      ApiAlpha60VTable =
          (ApiQuantity_VTable *)malloc( sizeof(ApiQuantity_VTable) );
      if( ApiAlpha60VTable == 0 )  return 0;
      memset( ApiAlpha60VTable, 0, sizeof(ApiQuantity_VTable));

      /** And fill the Vtable */
      ApiAlpha60VTable->getValue = ApiAlpha60_getValue;
      ApiAlpha60VTable->destroy = ApiQuantity_destroy_internal;
      ApiAlpha60VTable->roundingIncrement = 
                          ApiAlpha60_roundingIncrement;
      ApiAlpha60VTable->inAllowedRange = 
	                      ApiAlpha60_inAllowedRange;

      /** And assign the acceptable units */   
      ApiAlpha60VTable->acceptableUnits = 
                (ApiUnit**)malloc( sizeof(ApiUnit*) * 3);
      ApiAlpha60VTable->acceptableUnits[0] = ApiUnit_ThermalExp_REV_F();
      ApiAlpha60VTable->acceptableUnits[1] = ApiUnit_ThermalExp_REV_C();
      ApiAlpha60VTable->acceptableUnits[2] = NULL;

      ApiAlpha60VTable->standardUnit = ApiUnit_ThermalExp_REV_F();
      ApiAlpha60VTable->quantityName = (char*)malloc( sizeof(char) *  25);
      if(   ApiAlpha60VTable->quantityName != NULL){
        memset(ApiAlpha60VTable->quantityName , 0, sizeof(char)*25);
        strcat( ApiAlpha60VTable->quantityName, "Thermal Expansion Factor");
      }
   }
   return ApiAlpha60VTable;
};


/**
* The Thermal Expansion Factor specific initialization routine
*/
ApiAlpha60 * ApiAlpha60_init(double value,
			     ApiUnit *unit,
			     int isMutable,
			     int * errorCode){
   ApiAlpha60 * temp = 0;

   *errorCode = 0;

   /** Create the object itself */
   temp = (ApiAlpha60*)malloc( sizeof(ApiAlpha60));
   if( temp == 0){
        *errorCode = INITIALIZE_FAILED;
	return temp;
   }
   memset( temp, 0, sizeof(ApiAlpha60));

   /** Initialize our part of quant before we pass it on to parent */
   temp->quant.getVTable = ApiAlpha60_getVTable;


   /** And finally let the quant object initialize itself */
   *errorCode =  ApiQuantity_initialize((ApiQuantity*)temp,
					value,
					unit,
					isMutable);

   /** And return the new object*/
   return temp;
}

/* ----------------- Global destroy function -------- */


void ApiQuantity_destroyVtableData(ApiQuantity_VTable * table,
				   ApiQuantity **range){
   if( range != NULL){
     int i;
     for( i = 0; i < 2; i++){
       if( range[i] != NULL){
	  ApiQuanity_cleanUp( range[i] );
      }
    }
    free( range);
  }

  if( table != NULL){
    if( table->acceptableUnits != NULL){
      free( table->acceptableUnits);
    }
    if( table->quantityName != NULL){
      free(table->quantityName);
    }
    free(table);
  } 
}


/**
* This function frees all resources connected to vtables
*/
void ApiQuantity_destroy(){
  /* Temperature related data */
  ApiQuantity_destroyVtableData(  ApiTemperatureVTable, (ApiQuantity**)AllowedTemp);
  ApiTemperatureVTable = NULL;
  AllowedTemp = NULL;

  /* Pressure related data */
  ApiQuantity_destroyVtableData(  ApiPressureVTable, (ApiQuantity**)AllowedPres);
  ApiPressureVTable = NULL;
  AllowedPres = NULL;

  /* Density related data */
  ApiQuantity_destroyVtableData(  ApiDensityVTable,(ApiQuantity**)AllowedDens);
  ApiDensityVTable = NULL;
  AllowedDens = NULL;

  /* Volume related data */
  ApiQuantity_destroyVtableData(  ApiVolumeVTable,NULL);
  ApiVolumeVTable = NULL;

  /* Expansion Factor  related data */
  ApiQuantity_destroyVtableData(  ApiExpansionFactorVTable,NULL);
  ApiExpansionFactorVTable = NULL;

 /* Scaled Compression Factor  related data */
  ApiQuantity_destroyVtableData(  ApiScaledFactorVTable,NULL);
  ApiScaledFactorVTable = NULL;

  /* Thermal Expansion Factor related data */
  ApiQuantity_destroyVtableData(  ApiAlpha60VTable, (ApiQuantity**)AllowedAlpha60);
  ApiAlpha60VTable = NULL;
  AllowedAlpha60 = NULL;   
} 


