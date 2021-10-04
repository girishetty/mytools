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

#include "unit.h"
#include "error.h"
#include "constants.h"
#include <string.h>
#include <stdlib.h>

/* ----------- Definitions for  ApiUnit ---------- */

/**
* The structure defining the actual unit
*/
struct api_unit_{
  char * unitName;     /* the unit name of the unit */
};

/*
* The function that writes the unit information
*/
int ApiUnit_unitName(ApiUnit * unit, char * buffer, int len){
  int result = BUFFER_OVERFLOW;

  bool unit_not_null = unit != 0;

  size_t buffer_len = strlen(buffer);
  size_t unit_name_len = strlen(unit->unitName);

  if( unit_not_null && (buffer_len + unit_name_len < (size_t)len) ){
    result = 0;
    strcat(buffer,unit->unitName);
  } 
  return result;
}


/**
* The initialization function for ApiUnit. This is only defined in
* the source code file as this function should only be called 
* be the initializer in this file, i.e. it is to be considered private
* 
* @param value human readable value for the unit
*/
ApiUnit * ApiUnit_init(char * value){
  ApiUnit * unit;
  /*
  modified by Ian McDavid Flow-Cal development staff
  size_t len = strlen(value + 1);
  */
  size_t len = strlen(value);

  int i;
  /* create the unit object */
  unit = (ApiUnit*)malloc(sizeof(ApiUnit));
  /*  and set its name */
  if( unit != NULL){
    size_t name_size = sizeof(char) * len; /* added by Ian McDavid Flow-Cal development staff */

    unit->unitName = (char*)malloc(name_size);
    if( unit->unitName != 0){
       unit->unitName[0] = '\0';

       for (i = 0; i < len; i++){
          unit->unitName[i] = value[i];
       }
    }
    else{
      /* Could not allocate the string value - lets 
       * clean up and return 0 */ 
      free(unit);
      unit = 0;
    }
  }
 
  /* and return the initialized unit */
  return unit;
};

/**
* A function that cleans up all resources used by a specific
* unit object.
*/
void ApiUnit_cleanUp(ApiUnit * unit){
  if( unit != NULL ){
      if( unit->unitName != NULL){ 
        free(unit->unitName);
        unit->unitName = NULL;
      }
      free(unit);
  }   
};


/* ------ The units for density ----- */

/** The possible values for the Density */
static ApiUnit * DENSITY_API = NULL;
static ApiUnit * DENSITY_RELATIVE = NULL;
static ApiUnit * DENSITY_KGM3 = NULL;

/** 
* The functions that return the static  
* values for the possible choices for the ApiDensityUnit objects
*/ 
ApiUnit * ApiUnit_Density_API(){
  return DENSITY_API;
}

ApiUnit * ApiUnit_Density_RELATIVE(){
  return DENSITY_RELATIVE;
}

ApiUnit * ApiUnit_Density_KGM3(){
  return DENSITY_KGM3;
}


/* ------ The units for pressure ----- */

/** The possible values for the Pressure */
static ApiUnit * PRESSURE_PSI = NULL;
static ApiUnit * PRESSURE_KPA = NULL;
static ApiUnit * PRESSURE_BAR = NULL;

/** 
* The functions that return the static  
* values for the possible choices for the ApiUnit objects
* if dealing with pressure
*/ 
ApiUnit * ApiUnit_Pressure_PSI(){
  return PRESSURE_PSI;
}

ApiUnit * ApiUnit_Pressure_KPA(){
  return PRESSURE_KPA;
}

ApiUnit * ApiUnit_Pressure_BAR(){
  return PRESSURE_BAR;
}


/* ------ The units for Temperature ----- */

/** The possible values for the Temperature */
static ApiUnit * TEMPERATURE_F = NULL;
static ApiUnit * TEMPERATURE_C = NULL;

/** 
* The functions that return the static  
* values for the possible choices for the ApiUnit objects
* if dealing with temperature
*/ 
ApiUnit * ApiUnit_Temperature_F(){
  return TEMPERATURE_F;
}

ApiUnit * ApiUnit_Temperature_C(){
  return TEMPERATURE_C;
}


/* ------ The units for Volume ----- */

/** The possible values for the Volume */
static ApiUnit * VOLUME_BARREL = NULL;
static ApiUnit * VOLUME_LITER = NULL;
static ApiUnit * VOLUME_M3 = NULL;


/** 
* The functions that return the static  
* values for the possible choices for the ApiUnit objects
* if dealing with Volume
*/ 
ApiUnit * ApiUnit_Volume_BARREL(){
  return VOLUME_BARREL;
}

ApiUnit * ApiUnit_Volume_LITER(){
  return VOLUME_LITER;
}

ApiUnit * ApiUnit_Volume_M3(){
  return VOLUME_M3;
}


/* ------ The units for dimensionless quantities ----- */

/** The possible values for the Expansion Factor */
static ApiUnit * EXPANSION_DIMLESS = NULL;

/** 
* The functions that return the static  
* values for the possible choices for the ApiUnit objects
* if dealing with dimensionless quantities
*/ 
ApiUnit * ApiUnit_Expansion_DIMLESS(){
  return EXPANSION_DIMLESS;
}


/* ------ The units for scaled comp Factor ----- */

/** The possible values for the Scaled Comp Factor */
static ApiUnit * SCALED_COMP_REV_PSI = NULL;
static ApiUnit * SCALED_COMP_REV_BAR = NULL;
static ApiUnit * SCALED_COMP_REV_KPA = NULL;

/** 
* The functions that return the static  
* values for the possible choices for the ApiUnit objects
* if dealing with Scaled Comp Factor
*/ 
ApiUnit * ApiUnit_ScaledComp_REV_PSI(){
  return SCALED_COMP_REV_PSI;
}

ApiUnit * ApiUnit_ScaledComp_REV_BAR(){
  return SCALED_COMP_REV_BAR;
}

ApiUnit * ApiUnit_ScaledComp_REV_KPA(){
  return SCALED_COMP_REV_KPA;
}

/* ------ The units for Thermal Expansion Factor ----- */

/** The possible values for Thermal Expansion Factor */
static ApiUnit * THERMAL_EXP_REV_C = NULL;
static ApiUnit * THERMAL_EXP_REV_F = NULL;

/** 
* The functions that return the static  
* values for the possible choices for the ApiUnit objects
* if dealing with Thermal Expansion Factor
*/ 
ApiUnit * ApiUnit_ThermalExp_REV_C(){
  return THERMAL_EXP_REV_C;
}

ApiUnit * ApiUnit_ThermalExp_REV_F(){
  return THERMAL_EXP_REV_F;
}
/* ---- Initialization and clean-up functions ---- */

/**
 * Initialize all  static parameters
 */
int  ApiUnit_initalize(){
  int errorCode = 0;

  /** Density unit objects */
  if( DENSITY_API == NULL){
    DENSITY_API = ApiUnit_init(" API");
    if( DENSITY_API == NULL) errorCode = INITIALIZE_FAILED;
  }
  if( DENSITY_RELATIVE == NULL){
    DENSITY_RELATIVE = ApiUnit_init(" Rel. Dens.");
    if( DENSITY_RELATIVE == NULL) errorCode = INITIALIZE_FAILED;
  }
  if( DENSITY_KGM3 == NULL){
    DENSITY_KGM3 = ApiUnit_init(" Kg/m^3");
    if( DENSITY_KGM3 == NULL) errorCode = INITIALIZE_FAILED;
  }

  /** Pressure unit objects */
  if( PRESSURE_PSI == NULL){
    PRESSURE_PSI = ApiUnit_init(" psi");
    if( PRESSURE_PSI == NULL) errorCode = INITIALIZE_FAILED;
  }
  if( PRESSURE_KPA == NULL){
    PRESSURE_KPA = ApiUnit_init(" kPa");
    if( PRESSURE_KPA == NULL) errorCode = INITIALIZE_FAILED;
  }
  if( PRESSURE_BAR == NULL){
    PRESSURE_BAR = ApiUnit_init(" bar");
    if( PRESSURE_BAR == NULL) errorCode = INITIALIZE_FAILED;
  }

  /** Temperature unit objects */
  if( TEMPERATURE_F == NULL){
    TEMPERATURE_F = ApiUnit_init(" F");
    if( TEMPERATURE_F == NULL) errorCode = INITIALIZE_FAILED;
  }
  if( TEMPERATURE_C == NULL){
    TEMPERATURE_C = ApiUnit_init(" C");
    if( TEMPERATURE_C == NULL) errorCode = INITIALIZE_FAILED;
  }

  /** Volume unit objects */
  if( VOLUME_BARREL == NULL){
    VOLUME_BARREL = ApiUnit_init(" barrel");
    if( VOLUME_BARREL == NULL) errorCode = INITIALIZE_FAILED;
  }
  if( VOLUME_LITER == NULL){
    VOLUME_LITER = ApiUnit_init(" l");
    if( VOLUME_LITER == NULL) errorCode = INITIALIZE_FAILED;
  }
  if( VOLUME_M3 == NULL){
    VOLUME_M3 = ApiUnit_init(" m^3");
    if( VOLUME_M3 == NULL) errorCode = INITIALIZE_FAILED;
  }

  /** Dimensionless unit object */
  if( EXPANSION_DIMLESS == NULL){
    EXPANSION_DIMLESS = ApiUnit_init(" dimensionless");
    if( EXPANSION_DIMLESS == NULL) errorCode = INITIALIZE_FAILED;
  }

  /** Scaled Comp Factor unit object */
  if( SCALED_COMP_REV_PSI == NULL){
    SCALED_COMP_REV_PSI = ApiUnit_init(" 1/psi");
    if( SCALED_COMP_REV_PSI == NULL) errorCode = INITIALIZE_FAILED;
  }
  if( SCALED_COMP_REV_BAR == NULL){
    SCALED_COMP_REV_BAR = ApiUnit_init(" 1/bar");
    if( SCALED_COMP_REV_BAR == NULL) errorCode = INITIALIZE_FAILED;
  }
  if( SCALED_COMP_REV_KPA == NULL){
    SCALED_COMP_REV_KPA = ApiUnit_init(" 1/kPa");
    if( SCALED_COMP_REV_KPA == NULL) errorCode = INITIALIZE_FAILED;
  }

  /** Thermal expansion factor  unit object */
  if( THERMAL_EXP_REV_C == NULL){
     THERMAL_EXP_REV_C  = ApiUnit_init(" 1/C");
     if( THERMAL_EXP_REV_C == NULL) errorCode = INITIALIZE_FAILED;
  }
  if( THERMAL_EXP_REV_F == NULL){
     THERMAL_EXP_REV_F  = ApiUnit_init(" 1/F");
     if( THERMAL_EXP_REV_F == NULL) errorCode = INITIALIZE_FAILED;
  }

  return errorCode;
}

/**
 * Destroy all resources connected to the static parameters
 */
void ApiUnit_destroy(){
  /** Density unit objects */
  if(  DENSITY_API != NULL){
    ApiUnit_cleanUp(DENSITY_API);
    DENSITY_API = NULL;
  }
  if(  DENSITY_RELATIVE != NULL){
    ApiUnit_cleanUp(DENSITY_RELATIVE);
    DENSITY_RELATIVE = NULL;
  }
  if(  DENSITY_KGM3 != NULL){
    ApiUnit_cleanUp(DENSITY_KGM3);
    DENSITY_KGM3 = NULL;
  }

  /** Pressure unit objects */
  if(  PRESSURE_PSI != NULL){
    ApiUnit_cleanUp(PRESSURE_PSI);
    PRESSURE_PSI = NULL;
  }
  if(  PRESSURE_BAR != NULL){
    ApiUnit_cleanUp(PRESSURE_BAR);
    PRESSURE_BAR = NULL;
  }
  if(  PRESSURE_KPA != NULL){
    ApiUnit_cleanUp(PRESSURE_KPA);
    PRESSURE_KPA = NULL;
  }

  /** Temperature unit objects */
  if(  TEMPERATURE_F != NULL){
    ApiUnit_cleanUp(TEMPERATURE_F);
    TEMPERATURE_F = NULL;
  }
  if(  TEMPERATURE_C != NULL){
    ApiUnit_cleanUp(TEMPERATURE_C);
    TEMPERATURE_C = NULL;
  }

  /** Volume unit objects */
  if(  VOLUME_BARREL != NULL){
    ApiUnit_cleanUp( VOLUME_BARREL);
    VOLUME_BARREL = NULL;
  }
  if(  VOLUME_LITER != NULL){
    ApiUnit_cleanUp( VOLUME_LITER);
    VOLUME_LITER = NULL;
  }
  if(  VOLUME_M3 != NULL){
    ApiUnit_cleanUp( VOLUME_M3);
    VOLUME_M3 = NULL;
  }
  
  /** Dimensionless unit objects */
  if(  EXPANSION_DIMLESS != NULL){
    ApiUnit_cleanUp( EXPANSION_DIMLESS);
    EXPANSION_DIMLESS = NULL;
  }

  /** Scaled Comp Factor unit objects */
  if(  SCALED_COMP_REV_PSI != NULL){
    ApiUnit_cleanUp(SCALED_COMP_REV_PSI );
    SCALED_COMP_REV_PSI  = NULL;
  }
  if(  SCALED_COMP_REV_BAR != NULL){
    ApiUnit_cleanUp(SCALED_COMP_REV_BAR );
    SCALED_COMP_REV_BAR  = NULL;
  }
  if(  SCALED_COMP_REV_KPA != NULL){
    ApiUnit_cleanUp(SCALED_COMP_REV_KPA );
    SCALED_COMP_REV_KPA  = NULL;
  }


  /** Thermal Expansion factor unit objects */
  if(  THERMAL_EXP_REV_C != NULL){
    ApiUnit_cleanUp( THERMAL_EXP_REV_C );
    THERMAL_EXP_REV_C  = NULL;
  }
  if(  THERMAL_EXP_REV_F != NULL){
    ApiUnit_cleanUp( THERMAL_EXP_REV_F );
    THERMAL_EXP_REV_F  = NULL;
  }
}
