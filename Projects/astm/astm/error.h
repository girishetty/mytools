/*
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
#ifndef API_ERROR_H
#define API_ERROR_H

#ifdef _WIN32
#define dll_export __declspec(dllexport)
#else
#define dll_export
#endif

/* protect code so it can be compiled with a C++ compiler */
#ifdef __cplusplus
extern "C" {
#endif

/** @file error.h
* Defines all error codes that the program can generate.
* @par
* The error codes can be translated to a human readable  form
* using the function 
* ApiCalculation_getErrorDescription(int name, char * buffer, int length),
* which appends the human readable error message to the indicated buffer.
*/ 

/**  @defgroup errorGroup Error reporting and error conditions  */
/*@{*/


/**
* Error signal if an object cannot be  initialized.
*/
#define  INITIALIZE_FAILED 1

/**
* Error signaled if an unexpected  null pointer was encountered
*/
#define NULL_POINTER_EXCEPTION 2

/** 
*  Error signaled if an unexpected  unit was encountered.
*  This happens if   
*  ApiQuantity_getValue(ApiQuantity * quant,  ApiUnit *unit,  bool rounded, int * errorCode)
*  or
*  ApiQuantity_setValue(ApiQuantity * quant, double value, ApiUnit *unit)
*  are called with incompatible units.
*
*/
#define INVALID_UNIT  3

/**
*  The user tried to set an immutable quantity.
* This happens if  
* ApiQuantity_setValue(ApiQuantity * quant, double value, ApiUnit *unit)
* or ApiQuantity_setValueFromParent(ApiQuantity * quant, ApiQuantity * parent)
* are called on a quantity that has been declared as immutable.
*/ 
#define CHANGED_IMMUTABLE 4

/** 
* Attempt to write a value to a string buffer that is to large.
* In this case an error is signaled and nothing is written to the buffer.
*/
#define BUFFER_OVERFLOW 5

/** 
* Attempt to compare two  different quantities.
* This is signaled if for example an attempt is made to compare
* an ApiDensity with an ApiTemperature object using the function
* ApiQuantity_compareTo(ApiQuantity * quant1, ApiQuantity * quant2, int *errorCode)
*/
#define INVALID_COMPARISON 6

/**
* A quantity is outside the range of the allowed values.
* Before an actual calculation a check is performed to ensure that 
* all data are in range. The check might return a more specific 
* out of range message. This error signals a general out of range conditions.
*/ 
#define QUANTITY_OUT_OFF_RANGE 7

/** 
* Attempt to call an  unsupported function.
* Some functions might not be defined for all quantities, for example
* ApiQuantity_inAllowedRange(ApiQuantity *quant, int * errorCode) 
* is not defined for ApiDensity since the allowed density range depends on
* the commodity used and the type of calculation performed.
*/ 
#define UNSUPPORTED_FUNCTION 8

/**
 K-Values are not defined  */ 
#define UNDEFINED_K_VALUES 9

/**Rho density limits not defined are not defined  */ 
#define UNDEFINED_RHO_LIMITS 10

/** Undefined oil product  */ 
#define UNDEFINED_OIL_PRODUCT 11

/** Commidity and Alpha_60 values are supplied for calculation */
#define COMMODITY_AND_ALPHA60_SUPPLIED 12

/** Commidity and Alpha_60 values are both null */
#define COMMODITY_AND_ALPHA60_NULL 13

/** Alpha_60 value  is out of range */
#define ALPHA60_OUT_OF_RANGE 14

/** Observed density value is missing */
#define OBSERVED_DENSITY_VALUE_MISSING 15

/** Observed density not in range of given commodity for Type I calculation */
#define OBSERVED_DENSITY_OUT_OF_RANGE_TYPE_I 16

/** Observed density not in range of given commodity for Type II calculation */
#define OBSERVED_DENSITY_OUT_OF_RANGE_TYPE_II 17

/** Observed pressure value is missing */
#define OBSERVED_PRESSURE_VALUE_MISSING 18

/** Observed pressure value is out of range */
#define OBSERVED_PRESSURE_OUT_OF_RANGE 19

/** Alternate pressure value is missing */
#define ALTERNATE_PRESSURE_VALUE_MISSING 20

/** Alternate pressure value is out of range */
#define ALTERNATE_PRESSURE_OUT_OF_RANGE 21

/** Observed temperature value is missing */
#define OBSERVED_TEMPERATURE_VALUE_MISSING 22

/** Observed temperature value is out of range */
#define OBSERVED_TEMPERATURE_OUT_OF_RANGE 23

/** Alternate temperature value is missing */
#define ALTERNATE_TEMPERATURE_VALUE_MISSING 24

/** Alternate temperature value is out of range */
#define ALTERNATE_TEMPERATURE_OUT_OF_RANGE 25

/** More than one volume value was supplied */  
#define MORE_THAN_ONE_VOLUME_SUPPLIED 26
 
/** In the iteration step the density value is out of range */
#define ITERATION_DENSITY_VALUE_OUT_OF_RANGE 27

/** Convergence was not reached */
#define CONVERGENCE_NOT_REACHED 28

/**  Array index out of range */
#define ARRAY_INDEX_OUT_OF_RANGE 29

#define MAX_ERROR_NUMBER 29

/*@}*/

/* close protective C++ bracket */
#ifdef __cplusplus
}
#endif

#endif /* API_ERROR_H  */
