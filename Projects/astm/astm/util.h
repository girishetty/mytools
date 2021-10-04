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
#ifndef CALCULATION_UTIL_FUNCTIONS_H
#define CALCULATION_UTIL_FUNCTIONS_H

#ifdef _WIN32
#define dll_export __declspec(dllexport)
#else
#define dll_export
#endif

/* protect code so it can be compiled with a C++ compiler */
#ifdef __cplusplus
extern "C" {
#endif


#include "apiVCF.h"

/** @file util.h
* Defines utility function to set calculation parameters.
*/ 

/**
* Report the names for the input parameters
*
* @param name the number for the parameter
* @param buffer the buffer to which to append the name
* @param length the length of the buffer to which to append the data
* @return 0 if the data where written successfully, false otherwise
*/
dll_export int ApiCalculation_getParameterName(int name, char * buffer, int length);


/** 
* A function that converts Oil product numbers into chars 
*
* @param name the number for the Oil product
* @param buffer the buffer to which to append the name
* @param length the length of the buffer to which to append the data
* @return 0 if the data where written successfully, false otherwise
*/
dll_export int ApiCalculation_getProductName(int name, char * buffer, int length);

/** 
* A function that converts the error code to a description 
*
* @param name the numerical value of the error
* @param buffer the buffer to which to append the name
* @param length the length of the buffer to which to append the data
* @return 0 if the data where written successfully, false otherwise
*
*/
dll_export int ApiCalculation_getErrorDescription(int name, char * buffer, int length);


/**
* A convienence function to set all potential input parameters for
* a VCF calculation. If one or more of the volume units are NULL, the corresponding
* value of the volume will be set to NULL
*
* @param request the request for which to fill in the data
*
* @param commodity the commodity for which to do the calculation. If set to -1,
*                  it is assumed that an alpha60 value is supplied
* @param alpha60 the value for  alpha60  
* @param au the unit for alpha60. If NULL, we set alpha60 to NULL and assume that a
*            commodity is supplied.
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
* @return 0 if all parmeters where set successfully, an errorCode otherwise
* @param basePres the base pressure to which to calculate the 
*        expansion factors
* @param pub  the unit for the the base pressure
*
* @return 0 if all parmeters where set successfully, an errorCode otherwise
*
* @param baseVol the value for the base volume
* @param vub  the unit for the base volume. If it is null, the base volume
*             will be set to NULL
* @param altVol the value for the alternate volume
* @param vua  the unit for the alternate volume. If it is null, the alternate volume
*             will be set to NULL
* @param obVol the value for the observed volume
* @param vuo  the unit for the observed volume. If it is null, the observed volume
*             will be set to NULL
*/
dll_export int ApiCalculation_setParameters(ApiCalculationRequest * request,
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
				 double obVol,    ApiUnit *vuo);


/**
* A convienence function to set parameters for a 11.1.6.1 type calculation.
* If the volume unit is NULL, the volume is not calculated
*
* @param request the request for which to fill in the data
*
* @param commodity the commodity for which to do the calculation. If set to -1,
*                  it is assumed that an alpha60 value is supplied
* @param alpha60 the value for  alpha60  
* @param au the unit for alpha60. If NULL, we set alpha60 to NULL and assume that a
*            commodity is supplied.
*
* @param altTemp the alternate temperature to use
* @param tua the unit to use for the alternate temperature
* @param altPres the alternate pressure to use
* @param pua the unit to use for the alternate pressure
* @param baseDens the base density
* @param dub the unit to use for the base density
*
* @param baseVol the value for the base volume
* @param vub  the unit for the base volume. If it is null, the base volume
*             will be set to NULL
*/
dll_export int ApiCalculation_set11_1_6_1Parameters(ApiCalculationRequest * request,
					 int commodity,
					 double alpha60,  ApiUnit *au,
					 double altTemp,  ApiUnit *tua,
					 double altPres,  ApiUnit* pua,
					 double baseDens, ApiUnit* dub,
	
					 double baseVol,  ApiUnit *vub);

/**
* A convienence function to set parameters for a 11.1.6.2  type calculation.
* If the volume unit is NULL, the volume is not calculated
*
* @param request the request for which to fill in the data
*
* @param commodity the commodity for which to do the calculation. If set to -1,
*                  it is assumed that an alpha60 value is supplied
* @param alpha60 the value for  alpha60  
* @param au the unit for alpha60. If NULL, we set alpha60 to NULL and assume that a
*            commodity is supplied.
*
* @param obTemp the observed temperature to use
* @param tuo the unit to use for the observed temperature
* @param obPres the observed  pressure to use
* @param puo the unit to use for the observed pressure
* @param obDens the observed  density
* @param duo the unit to use for the observed density
*
* @param obVol the value for the observed volume
* @param vuo  the unit for the observed volume. If it is null, the observed volume
*             will be set to NULL
*/
dll_export int ApiCalculation_set11_1_6_2Parameters(ApiCalculationRequest * request,
					 int commodity,
					 double alpha60,  ApiUnit * au,
					 double obTemp,   ApiUnit * tuo,
					 double obPres,   ApiUnit * puo,
					 double obDens,   ApiUnit * duo,
	
					 double obVol,  ApiUnit *vuo);

/**
* A convienence function to set parameters for a 11.1.6.3  type calculation.
* If the volume unit is NULL, the volume is not calculated
*
* @param request the request for which to fill in the data
*
* @param commodity the commodity for which to do the calculation. If set to -1,
*                  it is assumed that an alpha60 value is supplied
* @param alpha60 the value for  alpha60  
* @param au the unit for alpha60. If NULL, we set alpha60 to NULL and assume that a
*            commodity is supplied.
*
* @param obTemp the observed temperature to use
* @param tuo the unit to use for the observed temperature
* @param obPres the observed  pressure to use
* @param puo the unit to use for the observed pressure
* @param obDens the observed  density
* @param duo the unit to use for the observed density
* @param altTemp the alternate temperature to use
* @param tua the unit to use for the alternate temperature
* @param altPres the alternate  pressure to use
* @param pua the unit to use for the alternate pressure
*
* @param obVol the value for the observed volume
* @param vuo  the unit for the observed volume. If it is null, the observed volume
*             will be set to NULL
*/
dll_export int ApiCalculation_set11_1_6_3Parameters(ApiCalculationRequest * request,
					 int commodity,
					 double alpha60,  ApiUnit * au,
					 double obTemp,   ApiUnit * tuo,
					 double obPres,   ApiUnit * puo,
					 double obDens,   ApiUnit * duo,
	
					 double altTemp,   ApiUnit * tua,
					 double altPres,   ApiUnit * pua,

					 double obVol,  ApiUnit *vuo);


/**
* A convienence function to set parameters for a 11.1.7.1 type calculation.
* If the volume unit is NULL, the volume is not calculated
*
* @param request the request for which to fill in the data
*
* @param commodity the commodity for which to do the calculation. If set to -1,
*                  it is assumed that an alpha60 value is supplied
* @param alpha60 the value for  alpha60  
* @param au the unit for alpha60. If NULL, we set alpha60 to NULL and assume that a
*            commodity is supplied.
*
* @param altTemp the alternate temperature to use
* @param tua the unit to use for the alternate temperature
* @param altPres the alternate pressure to use
* @param pua the unit to use for the alternate pressure
* @param baseDens the base density
* @param dub the unit to use for the base density
*
* @param baseTemp the base temperature to which to adjust
* @param tub   the unit of the base temperature to use
*
* @param baseVol the value for the base volume
* @param vub  the unit for the base volume. If it is null, the base volume
*             will be set to NULL
*/
dll_export int ApiCalculation_set11_1_7_1Parameters(ApiCalculationRequest * request,
					 int commodity,
					 double alpha60,  ApiUnit *au,
					 double altTemp,  ApiUnit *tua,
					 double altPres,  ApiUnit* pua,
					 double baseDens, ApiUnit* dub,

					 double baseTemp, ApiUnit * tub,
	
					 double baseVol,  ApiUnit *vub);

/**
* A convienence function to set parameters for a 11.1.7.2  type calculation.
* If the volume unit is NULL, the volume is not calculated
*
* @param request the request for which to fill in the data
*
* @param commodity the commodity for which to do the calculation. If set to -1,
*                  it is assumed that an alpha60 value is supplied
* @param alpha60 the value for  alpha60  
* @param au the unit for alpha60. If NULL, we set alpha60 to NULL and assume that a
*            commodity is supplied.
*
* @param obTemp the observed temperature to use
* @param tuo the unit to use for the observed temperature
* @param obPres the observed  pressure to use
* @param puo the unit to use for the observed pressure
* @param obDens the observed  density
* @param duo the unit to use for the observed density
*
* @param baseTemp the base temperature to which to do the calculation
* @param tub the unit for the base temperature
*
* @param obVol the value for the observed volume
* @param vuo  the unit for the observed volume. If it is null, the observed volume
*             will be set to NULL
*/
dll_export int ApiCalculation_set11_1_7_2Parameters(ApiCalculationRequest * request,
					 int commodity,
					 double alpha60,  ApiUnit * au,
					 double obTemp,   ApiUnit * tuo,
					 double obPres,   ApiUnit * puo,
					 double obDens,   ApiUnit * duo,

					 double baseTemp, ApiUnit *tub,
	
					 double obVol,  ApiUnit *vuo);


/**
* Run all  the tests
*
* @param buffer the buffer in which to report intermediate results
* @param bufLength the length of the buffer
*
* @param errBuf the buffer in which to report errors
* @param errLength the length of the buffer to report errors 
*
* @param errorCode a variable to report errors
*
* @return TRUE if all tests where run successfully, FALSE otherwise
*/
dll_export bool ApiCalculation_runTests(char * buffer, 
			     int bufLength,
			     char * errBuf,
			     int errLength,
			     int *errorCode);

/* close protective C++ bracket */
#ifdef __cplusplus
}
#endif

#endif /* CALCULATION_UTIL_FUNCTIONS_H  */
