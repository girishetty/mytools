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


#ifndef API_QUANTITY_H
#define API_QUANTITY_H

#include "unit.h"
#include "error.h"
#include "constants.h"

#ifdef _WIN32
#define dll_export __declspec(dllexport)
#else
#define dll_export
#endif

/* protect code so it can be compiled with a C++ compiler */
#ifdef __cplusplus
extern "C" {
#endif

/** @file quantity.h
* Defines the different quantity structure objects.
*
* Each quantity, like temperature or pressure, is defined by a specific
* quantity structure that allows to access its getValue etc functions.
*/ 


/**  @defgroup quantGroup Quantity and Unit functions  */
/*@{*/

/**
* The global destroy function for all resources connected to the 
* static Quantity objects. This should be called upon program termination
*/
dll_export void ApiQuantity_destroy();


/*-----------------------------------------------------
* The type definitions for all the objects defined in 
* this include file
*-----------------------------------------------------*/

/** A generic quantity object which is the parent of all other quantity objects  */
typedef struct api_quantity_ ApiQuantity; 

/** Type definition for the temperature */
typedef struct api_temperature_ ApiTemperature;

/** Type definition for the pressure */
typedef struct api_pressure_ ApiPressure;

/** Type definition for the density  */
typedef struct api_density_ ApiDensity;

/** Type definition for the volume  */
typedef struct api_volume_ ApiVolume;

/** Type definition for the ExpansionFactor  */
typedef struct api_expansion_factor_ ApiExpansionFactor;

/** Type definition for the Scaled Compression factor */
typedef struct api_scaled_factor_ ApiScaledFactor;

/** Type definition for the Thermal Expansion Factor alpha_60  */
typedef struct api_alpha60_ ApiAlpha60;
 

/*-----------------------------------------------------/
* 
* Public interfaces for the Quantity objects
*
*-----------------------------------------------------*/

/**
* Clean up a ApiQuantity object. This is a "virtual" function, i.e.
* calling this function with any derived ApiQuantity pointer will
* clean up all resources.
*
* @param quant the quantity object to clean up
*/
dll_export void ApiQuanity_cleanUp(ApiQuantity * quant);

/**
* Return the current value of this quantity. 
* This is a "virtual" function, i.e. returns the correct 
* value for any derived ApiQuantity pointer.
*
*  @param  quant the quantity on which to perform
*                   the operation
*  @param  unit the unit we want the value in
*  @param rounded 0 if  we want to get the unrounded value or
*        1 if we want to get the  rounded value.
*  @param errorCode if all is well it is set to 0, otherwise to
*          an errorCode value
*/
dll_export double ApiQuantity_getValue(ApiQuantity * quant,
			    ApiUnit *unit,
			    bool rounded,
			    int * errorCode);


/**
* Set the new value for this quantity.
* This is a "virtual" function, i.e.,  it sets  the correct 
* value for any derived ApiQuantity pointer.
*  @param  quant the quantity on which to perform
*                   the operation
*  @param value the value to which to set this quantity
*  @param  unit the unit we want the value in
*  @return  0 if the succeed, an error code otherwise
*/
dll_export int ApiQuantity_setValue(ApiQuantity * quant,
			 double value,
			 ApiUnit *unit);

/**
* Set the new value for this quantity.
* This is a "virtual" function, i.e.,  it sets  the correct 
* value for any derived ApiQuantity pointer.
*  @param  quant the quantity on which to perform
*                   the operation
*  @param parent from which to transfer the data
*  @return  0 if the succeed, an error code otherwise
*/
dll_export int ApiQuantity_setValueFromParent(ApiQuantity * quant, ApiQuantity * parent);


/**
* Compare whether two quantity objects are identical
*
* @param quant1 one of the ApiQuantity objects to compare
* @param quant2 one of the ApiQuantity objects to compare
* @param errorCode set to 0 if all is well  and to an error code otherwise
*
* @return 0 if the two values as returned by ApiQuantity_standardValue
*           are equal within CONSTANTS_EPSION
*        -1 if  quant2 is larger than quant1
*        +1 if  quant2 is smaller than quant1
*/
dll_export int ApiQuantity_compareTo(ApiQuantity * quant1,
							ApiQuantity * quant2,
                          int *errorCode);


/**
* Get the standard unit of this quantity.
* This is a "virtual" function, i.e.. it sets  the correct 
* value for any derived ApiQuantity pointer.
*
* @param quant the quantity for which to get the standard unit
*
* @return the standard unit to use for this quantity
*/
dll_export ApiUnit * ApiQuantity_standardUnit(ApiQuantity * quant);
  


/**
* Get the given unit for this quantity.
*
* @param quant one of the ApiQuantity object for which to get the given valuee
* @param errorCode set to 0 if all is well  and to an error code otherwise
*
* @return the unit this quantity was initialized with
*/
dll_export ApiUnit * ApiQuantity_givenUnit(ApiQuantity *quant,
				int * errorCode);


/** 
* Some quantities (Temperature, Density and Thermal Expansion Factor)
* that are used as input parameters need to fall into a predefined
* range. For those functions this  method should check the range 
* and throw an ApiExeption when they fall not into the range.
* Otherwise this method just returns true, since there are no
* range restrictions. This implementation always returns true.
*
* @param quant one of the ApiQuantity object for which to get the given value
* @param errorCode set to 0 if all is well  and to an error code otherwise
* 
* @return 1 if the value of the quantity falls within the allowed
*         range 0 otherwise
*/
dll_export bool ApiQuantity_inAllowedRange(ApiQuantity *quant,
			       int * errorCode);

/**
* Return a human readable name for the quantity
*
* @param quant the quantity from which to extract the name
* @param buf the buffer to which to append the data
* @param len the length of the buffer
*
* @return 0 if all is well, an error code otherwise
*/
dll_export int ApiQuantity_getName(ApiQuantity * quant, char * buf, int len); 


/*-----------------------------------------------------
* 
* Public interfaces for the Temperature object
* In addition all functions for Quantity can also
* be called
*
-----------------------------------------------------*/

/**
* Create a new ApiTemperature object
*
* @param value the value to which to set the quantity
* @param unit the unit in which the value is given
* @param isMutable 0 if this quantity is not mutable 1 if it is
*        mutable
* @param errorCode is set to 0 if all is well and to a defined
*          error code if an error occured.
*/ 
dll_export ApiTemperature * ApiTemperature_init(double value,
				     ApiUnit *unit,
				     bool isMutable,
				     int * errorCode);

/**
* Correct the temperature from ITS-90 to IPTS-68 basis.
* This is described in Paragraph 1.1.5.3, p. 28. All the 
* constants in the series are defined in 
* constants.h. We only allow the correction to
* be applied once after setValue} was
* called or since construction.
* 
* @param temp the temperuture to correct
* @param  errorCode is set to 0 if all is well and to a defined
*          error code if an error occured.
*/ 
dll_export void ApiTemperature_convertToIPTS68(ApiTemperature * temp,
                                    int *errorCode); 


/**
* Was the temperature already corrected from ITS-90 to IPTS-68 basis.
* 
* @param temp the temperuture to correct
* @param  errorCode is set to 0 if all is well and to a defined
*          error code if an error occured.
* @return TRUE if if was already converted, false otherwise
*/ 
dll_export bool ApiTemperature_wasIPTS68Corrected(ApiTemperature * temp,
				       int *errorCode);


/*-----------------------------------------------------
* 
* Public interfaces for the Pressure object
* In addition all functions for Quantity can also
* be called
*
-----------------------------------------------------*/

/**
* Create a new ApiPressure object
*
* @param value the value to which to set the quantity
* @param unit the unit in which the value is given
* @param isMutable 0 if this quantity is not mutable 1 if it is
*        mutable
* @param errorCode is set to 0 if all is well and to a defined
*          error code if an error occured.
*/ 
dll_export ApiPressure * ApiPressure_init(double value,
			       ApiUnit *unit,
			       bool isMutable,
			       int * errorCode);


/*-----------------------------------------------------
* 
* Public interfaces for the Density object
* In addition all functions for Quantity can also
* be called
*
-----------------------------------------------------*/

/**
* Create a new ApiDensity object
*
* @param value the value to which to set the quantity
* @param unit the unit in which the value is given
* @param isMutable 0 if this quantity is not mutable 1 if it is
*        mutable
* @param errorCode is set to 0 if all is well and to a defined
*          error code if an error occured.
*/ 
dll_export ApiDensity * ApiDensity_init(double value,
			     ApiUnit *unit,
			     bool isMutable,
			     int * errorCode);

struct api_oilproduct_;   /** Short description for compiler so we can reference 
                             ApiOilProduct object here.Full definition is in
                             oilproducts.h */

/**
* The allowed range for the density depends on the Commodity type
* as well as on the  type of calculation to be done. If 
* a Type II calculation is required the range of allowed values is
* larger.
*
* @param density the density object which to check
* @param product the ApiOilProduct object describing the Commodity
* @param type2  true if this range check is for a type II calculation
*               false otherwise.
* @param errorCode a variable to report errors
* @return TTRUE if the value of the pressure falls within the allowed
*                range
*/
dll_export bool ApiDensity_inRhoRange(ApiDensity * density,
			   struct api_oilproduct_ * product,
			   bool type2,
			   int *errorCode);

/*-----------------------------------------------------
* 
* Public interfaces for the Volume  object
* In addition all functions for Quantity can also
* be called
*
-----------------------------------------------------*/

/**
* Create a new ApiVolume object
*
* @param value the value to which to set the quantity
* @param unit the unit in which the value is given
* @param isMutable 0 if this quantity is not mutable 1 if it is
*        mutable
* @param errorCode is set to 0 if all is well and to a defined
*          error code if an error occured.
*/ 
dll_export ApiVolume  * ApiVolume_init(double value,
			    ApiUnit *unit,
			    bool isMutable,
			    int * errorCode);


/*-----------------------------------------------------
* 
* Public interfaces for the Expansion factor  object
* In addition all functions for Quantity can also
* be called
*
-----------------------------------------------------*/

/**
* Create a new ApiExpansionFactor object
*
* @param value the value to which to set the quantity
* @param unit the unit in which the value is given
* @param isMutable 0 if this quantity is not mutable 1 if it is
*        mutable
* @param errorCode is set to 0 if all is well and to a defined
*          error code if an error occured.
*/ 
dll_export ApiExpansionFactor  * ApiExpansionFactor_init(double value,
					      ApiUnit *unit,
					      bool isMutable,
					      int * errorCode);


/*-----------------------------------------------------
* 
* Public interfaces for the Expansion factor  object
* In addition all functions for Quantity can also
* be called
*
*-----------------------------------------------------*/

/**
* Create a new ApiScaledFactor object
*
* @param value the value to which to set the quantity
* @param unit the unit in which the value is given
* @param isMutable 0 if this quantity is not mutable 1 if it is
*        mutable
* @param errorCode is set to 0 if all is well and to a defined
*          error code if an error occured.
*/ 
dll_export ApiScaledFactor  * ApiScaledFactor_init(double value,
					ApiUnit *unit,
					bool isMutable,
					int * errorCode);


/*-----------------------------------------------------
* 
* Public interfaces for the Thermal  Expansion factor  object
* In addition all functions for Quantity can also
* be called
*
*-----------------------------------------------------*/


/**
* Create a new ApiAlpha60Factor object
*
* @param value the value to which to set the quantity
* @param unit the unit in which the value is given
* @param isMutable 0 if this quantity is not mutable 1 if it is
*        mutable
* @param errorCode is set to 0 if all is well and to a defined
*          error code if an error occured.
*/ 
dll_export ApiAlpha60   * ApiAlpha60_init(double value,
			       ApiUnit *unit,
			       bool isMutable,
			       int * errorCode);

/*@}*/
/* close protected C++ bracket */
#ifdef __cplusplus
}
#endif

#endif /* API_QUANTITY_H  */
