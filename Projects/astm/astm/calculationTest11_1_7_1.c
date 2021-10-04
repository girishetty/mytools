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


#include "calculationTest.h"
#include <math.h>


/** The resulting rounded density (in units of the base density) */
static double roundedDensity;

/** 
*  A convienence method to set the results for the 11.1.7.1 results.
*
* The examples in Chapter 11 give as a result the  ROUNDED (!) alternate density
* (calculated via multiplying the ROUNDED (!) value of ctpl with the 
*  given base density.)
* 
* @param result the CalculationRequest instance to set
* @param scaled the Scaled Compensation Factor expected
* @param su the unit to use for the Scaled Compensation Factor
* @param ctlBaseToAlt the ctl factor expected
* @param cplBaseToAlt the cpl factor expected
* @param ctplBaseToAlt the ctpl factor expected
* @param altDens the expected density
* @param du  the unit of the expected density
* @param vol the expected volume
* @param vu the unit of the expected volume
*/
int  ApiCalculationTest_setResults11_1_7_1Test(ApiCalculationRequest *result,
					       double scaled, ApiUnit* su,
					       double ctlBaseToAlt,
					       double cplBaseToAlt,
					       double ctplBaseToAlt,
					       double altDens, ApiUnit * du,
					       double vol, ApiUnit* vu){
  int errorCode = 0;

  errorCode =  ApiCalculationTest_setResult(result,
					    0, NULL,
					    1.0,
					    1.0, NULL,
					    1.0,
					    1.0,
					    vol,vu,

					    ctlBaseToAlt,
					    scaled, su,
					    cplBaseToAlt,
					    ctplBaseToAlt,
					    altDens, du,
					    0, NULL,

					    0, NULL);
  roundedDensity = altDens;
  return errorCode;
};


/** 
* Do a calculation and test that the results for a 11.1.7.1 calculation.
* 
*
* The examples in Chapter 11 give as a result the  ROUNDED (!) alternate density
* (calculated via multiplying the ROUNDED (!) value of ctpl with the 
*  given base density.) Hence the checkResult function needs to be sliglty different
* 
*/
bool ApiCalculationTest_check11_1_7_1(ApiCalculationRequest * request,
				      ApiCalculationRequest * result,
				      bool  wantException,
				      char * desc,
				      char * error,
				      int errorLength,
				      int * errorCode){
  bool test = ApiCalculationTest_checkResult(request,
					     result,
					     wantException,
					     desc,
					     error,
					     errorLength,
					     errorCode);
  double value,comp;
  ApiUnit * u;

  if( *errorCode != 0) return FALSE;

  if( wantException == FALSE){   /** Compare the rounded alt. Dens. */
      value = ApiQuantity_getValue((ApiQuantity*)request->ctplBaseToAlt, 
				   ApiUnit_Expansion_DIMLESS(), 
				   TRUE,
				   errorCode);
      if( *errorCode != 0) return FALSE;
      u = ApiQuantity_givenUnit( (ApiQuantity*)request->obDens, errorCode);
      if( *errorCode != 0) return FALSE;
      comp = ApiQuantity_getValue((ApiQuantity*)request->obDens, u, FALSE, errorCode); 
      if( *errorCode != 0) return FALSE;
      value *= comp;
      if( fabs(value - roundedDensity) >=  API_EPSILON ){
	test = FALSE;
        if( error != NULL){
 	   ApiCalculationRequest_appendBuffer(error, errorLength,desc);
	   ApiCalculationRequest_appendBuffer(error, errorLength,API_EOL);
	   ApiCalculationRequest_appendBuffer(error, errorLength,"For ");
	   ApiCalculation_getParameterName(API_ALT_DENS_NAME, error, errorLength);
	   ApiCalculationRequest_appendBuffer(error, errorLength," Expected ");
	   if( *errorCode != 0){	
	      ApiCalculationRequest_appendBuffer(error, errorLength,"Unexpected error ");
	     return test;
	   }
    	   ApiCalculationRequest_appendDouble(error, errorLength,roundedDensity);
	   ApiUnit_unitName(u, error, errorLength);
	   ApiCalculationRequest_appendBuffer(error, errorLength," got ");
  	   ApiCalculationRequest_appendDouble(error, errorLength,value);
	   ApiUnit_unitName(u, error, errorLength);
	   ApiCalculationRequest_appendBuffer(error, errorLength,API_EOL);
	}
      }
  }

  return test;
}



/**
* Run the test for the 11.1.7.1 calculations
*
*/
bool  ApiCalculationTest_run11_1_7_1Tests(ApiCalculationRequest * request, 
					  char *errBuf,
					  int errLength,                       
					  int * errorCode){
  bool result = TRUE;
  bool tmp = TRUE;
  ApiCalculationRequest * resultRequest;

  *errorCode = 0;

  if( request == NULL) {
    *errorCode = NULL_POINTER_EXCEPTION;
    return FALSE;
  }
  resultRequest = ApiCalculationRequest_init(errorCode);
  if( *errorCode != 0) return FALSE;


  /** 
   * Set the values we want to test for. Since the rounded value for the
   * alternate density is given in Chapter 11, we will compare the alternate 
   * density separately 
   */
  resultRequest->scaledCompAlt = ApiScaledFactor_init(1, 
						      ApiUnit_ScaledComp_REV_PSI(),
						      TRUE,
						      errorCode);
  if( *errorCode != 0) return FALSE;

  resultRequest->ctlBaseToAlt = ApiExpansionFactor_init(1, 
							ApiUnit_Expansion_DIMLESS(),
							TRUE,
							errorCode);
  if( *errorCode != 0) return FALSE;

  resultRequest->cplBaseToAlt = ApiExpansionFactor_init(1, 
							ApiUnit_Expansion_DIMLESS(),
							TRUE,
							errorCode);
  if( *errorCode != 0) return FALSE;

  resultRequest->ctplBaseToAlt = ApiExpansionFactor_init(1, 
							 ApiUnit_Expansion_DIMLESS(),
							 TRUE,
							 errorCode);
  if( *errorCode != 0) return FALSE;

  resultRequest->baseVol = ApiVolume_init(1, ApiUnit_Volume_M3(), TRUE, errorCode);
  if( *errorCode != 0) return FALSE;

  /** Example 1 */
  *errorCode = ApiCalculation_set11_1_7_1Parameters(request,
						    API_CRUDE_OIL_NAME,
						    0, NULL,
						    -32.8, ApiUnit_Temperature_C(),
						    24.6, ApiUnit_Pressure_BAR(),
						    772.3, ApiUnit_Density_KGM3(),

						    15, ApiUnit_Temperature_C(),

						    0,NULL);
  if( *errorCode != 0) return FALSE;
  request->altVol = ApiVolume_init(9885,  ApiUnit_Volume_M3(), TRUE, errorCode);
  if( *errorCode != 0) return FALSE;
  *errorCode = ApiCalculationTest_setResults11_1_7_1Test(resultRequest,
							 6.706348860257,
							 ApiUnit_ScaledComp_REV_BAR(),
							 1.048420095319,
							 1.001652488031,
							 1.050152596978,
							 811.030845,
							 ApiUnit_Density_KGM3(),
							 10380.73275, 
							 ApiUnit_Volume_M3() );

  if( *errorCode != 0) return FALSE;	
  ApiCalculationRequest_appendBuffer(request->intermediate, request->intLength, API_EOL);
  ApiCalculationRequest_appendBuffer(request->intermediate,
				     request->intLength,
				     "11.1.7.1 Example 1");
  ApiCalculationRequest_appendBuffer(request->intermediate, request->intLength, API_EOL);
  tmp  =  ApiCalculationTest_check11_1_7_1(request,
					   resultRequest,
					   FALSE,
					   "11.1.7.1 Example 1",
					   errBuf,
					   errLength,
					   errorCode);
  if( tmp == FALSE || *errorCode != 0) result = FALSE;

  /** Example 2 */
  *errorCode = ApiCalculation_set11_1_7_1Parameters(request,
						    API_CRUDE_OIL_NAME,
						    0, NULL,
						    -32.8, ApiUnit_Temperature_C(),
						    24.65, ApiUnit_Pressure_BAR(),
						    772.3, ApiUnit_Density_KGM3(),

						    20, ApiUnit_Temperature_C(),

						    0,NULL);
  if( *errorCode != 0) return FALSE;
  request->altVol = ApiVolume_init(397498,  ApiUnit_Volume_M3(), TRUE, errorCode);
  if( *errorCode != 0) return FALSE;
  *errorCode = ApiCalculationTest_setResults11_1_7_1Test(resultRequest,
							 6.622983078151,
							 ApiUnit_ScaledComp_REV_BAR(),
							 1.05331266825,
							 1.001635234957,
							 1.055035081947,
							 814.807392,
							 ApiUnit_Density_KGM3(),
							 419376.28992, 
							 ApiUnit_Volume_M3() );

  if( *errorCode != 0) return FALSE;	
  ApiCalculationRequest_appendBuffer(request->intermediate, request->intLength, API_EOL);
  ApiCalculationRequest_appendBuffer(request->intermediate,
				     request->intLength,
				     "11.1.7.1 Example 2");
  ApiCalculationRequest_appendBuffer(request->intermediate, request->intLength, API_EOL);
  tmp  =  ApiCalculationTest_check11_1_7_1(request,
					   resultRequest,
					   FALSE,
					   "11.1.7.1 Example 2",
					   errBuf,
					   errLength,
					   errorCode);
  if( tmp == FALSE || *errorCode != 0) result = FALSE;

  /** Example 3 */
  *errorCode = ApiCalculation_set11_1_7_1Parameters(request,
						    API_REFINED_PRODUCTS_NAME,
						    0, NULL,
						    87.32, ApiUnit_Temperature_C(),
						    75, ApiUnit_Pressure_KPA(),
						    865.6, ApiUnit_Density_KGM3(),

						    15, ApiUnit_Temperature_C(),

						    0,NULL);
  if( *errorCode != 0) return FALSE;
  request->altVol = ApiVolume_init(48.75,  ApiUnit_Volume_M3(), TRUE, errorCode);
  if( *errorCode != 0) return FALSE;
  *errorCode = ApiCalculationTest_setResults11_1_7_1Test(resultRequest,
							 0.105233071273,
							 ApiUnit_ScaledComp_REV_KPA(),
							 0.940416747194,
							 1.000078931033,
							 0.940490975260,
							 814.088144,
							 ApiUnit_Density_KGM3(),
							 45.8488875, 
							 ApiUnit_Volume_M3() );

  if( *errorCode != 0) return FALSE;	
  ApiCalculationRequest_appendBuffer(request->intermediate, request->intLength, API_EOL);
  ApiCalculationRequest_appendBuffer(request->intermediate,
				     request->intLength,
				     "11.1.7.1 Example 3");
  ApiCalculationRequest_appendBuffer(request->intermediate, request->intLength, API_EOL);
  tmp  =  ApiCalculationTest_check11_1_7_1(request,
					   resultRequest,
					   FALSE,
					   "11.1.7.1 Example 3",
					   errBuf,
					   errLength,
					   errorCode);
  if( tmp == FALSE || *errorCode != 0) result = FALSE;


  /** Example 4 */
  *errorCode = ApiCalculation_set11_1_7_1Parameters(request,
						    API_REFINED_PRODUCTS_NAME,
						    0, NULL,
						    27.37, ApiUnit_Temperature_C(),
						    17.05, ApiUnit_Pressure_BAR(),
						    793.5, ApiUnit_Density_KGM3(),

						    15, ApiUnit_Temperature_C(),

						    0,NULL);
  if( *errorCode != 0) return FALSE;
  request->altVol = ApiVolume_init(200.2,  ApiUnit_Volume_M3(), TRUE, errorCode);
  if( *errorCode != 0) return FALSE;
  *errorCode = ApiCalculationTest_setResults11_1_7_1Test(resultRequest,
							 9.520173044783,
							 ApiUnit_ScaledComp_REV_BAR(),
							 0.988274000560,
							 1.001625828532,
							 0.989880764627,
							 785.46978,
							 ApiUnit_Density_KGM3(),
							 198.173976, 
							 ApiUnit_Volume_M3() );

  if( *errorCode != 0) return FALSE;	
  ApiCalculationRequest_appendBuffer(request->intermediate, request->intLength, API_EOL);
  ApiCalculationRequest_appendBuffer(request->intermediate,
				     request->intLength,
				     "11.1.7.1 Example 4");
  ApiCalculationRequest_appendBuffer(request->intermediate, request->intLength, API_EOL);
  tmp  =  ApiCalculationTest_check11_1_7_1(request,
					   resultRequest,
					   FALSE,
					   "11.1.7.1 Example 4",
					   errBuf,
					   errLength,
					   errorCode);
  if( tmp == FALSE || *errorCode != 0) result = FALSE;

  /** Example 5 */
  *errorCode = ApiCalculation_set11_1_7_1Parameters(request,
						    API_REFINED_PRODUCTS_NAME,
						    0, NULL,
						    -36, ApiUnit_Temperature_C(),
						    8.6, ApiUnit_Pressure_BAR(),
						    772.3, ApiUnit_Density_KGM3(),

						    20, ApiUnit_Temperature_C(),

						    0,NULL);
  if( *errorCode != 0) return FALSE;
  request->altVol = ApiVolume_init(1502.3, ApiUnit_Volume_M3(), TRUE, errorCode);
  if( *errorCode != 0) return FALSE;
  *errorCode = ApiCalculationTest_setResults11_1_7_1Test(resultRequest,
							 6.467511380549,
							 ApiUnit_ScaledComp_REV_BAR(),
							 1.060143267269,
							 1.000556515516,
							 1.06073325446,
							 819.201779,
							 ApiUnit_Density_KGM3(),
							 1593.534679, 
							 ApiUnit_Volume_M3() );

  if( *errorCode != 0) return FALSE;	
  ApiCalculationRequest_appendBuffer(request->intermediate, request->intLength, API_EOL);
  ApiCalculationRequest_appendBuffer(request->intermediate,
				     request->intLength,
				     "11.1.7.1 Example 5");
  ApiCalculationRequest_appendBuffer(request->intermediate, request->intLength, API_EOL);
  tmp  =  ApiCalculationTest_check11_1_7_1(request,
					   resultRequest,
					   FALSE,
					   "11.1.7.1 Example 5",
					   errBuf,
					   errLength,
					   errorCode);
  if( tmp == FALSE || *errorCode != 0) result = FALSE;

  /** Example 6 */
  *errorCode = ApiCalculation_set11_1_7_1Parameters(request,
						    API_REFINED_PRODUCTS_NAME,
						    0, NULL,
						    27.35, ApiUnit_Temperature_C(),
						    1235, ApiUnit_Pressure_KPA(),
						    657.3, ApiUnit_Density_KGM3(),

						    15, ApiUnit_Temperature_C(),

						    0,NULL);
  if( *errorCode != 0) return FALSE;
  request->altVol = ApiVolume_init(1000, ApiUnit_Volume_M3(), TRUE, errorCode);
  if( *errorCode != 0) return FALSE;
  *errorCode = ApiCalculationTest_setResults11_1_7_1Test(resultRequest,
							 0.194781276352,
							 ApiUnit_ScaledComp_REV_KPA(),
							 0.981752516460,
							 1.002411349381,
							 0.984119864783,
							 646.862076,
							 ApiUnit_Density_KGM3(),
							 984.12, 
							 ApiUnit_Volume_M3() );

  if( *errorCode != 0) return FALSE;	
  ApiCalculationRequest_appendBuffer(request->intermediate, request->intLength, API_EOL);
  ApiCalculationRequest_appendBuffer(request->intermediate,
				     request->intLength,
				     "11.1.7.1 Example 6");
  ApiCalculationRequest_appendBuffer(request->intermediate, request->intLength, API_EOL);
  tmp  =  ApiCalculationTest_check11_1_7_1(request,
					   resultRequest,
					   FALSE,
					   "11.1.7.1 Example 6",
					   errBuf,
					   errLength,
					   errorCode);
  if( tmp == FALSE || *errorCode != 0) result = FALSE;

  /** Example 7 */
  *errorCode = ApiCalculation_set11_1_7_1Parameters(request,
						    API_COMMODITY_NOT_GIVEN,
						    0.000446759, 
						    ApiUnit_ThermalExp_REV_F(),
						    89.9, ApiUnit_Temperature_C(),
						    45.35, ApiUnit_Pressure_BAR(),
						    641.8, ApiUnit_Density_KGM3(),

						    15, ApiUnit_Temperature_C(),

						    0,NULL);
  if( *errorCode != 0) return FALSE;
  request->altVol = ApiVolume_init(47.85, ApiUnit_Volume_M3(), TRUE, errorCode);
  if( *errorCode != 0) return FALSE;
  *errorCode = ApiCalculationTest_setResults11_1_7_1Test(resultRequest,
							 41.702651081010,
							 ApiUnit_ScaledComp_REV_BAR(),
							 0.938841277179,
							 1.019276716462,
							 0.956939054283,
							 614.164092,
							 ApiUnit_Density_KGM3(),
							 45.789579, 
							 ApiUnit_Volume_M3() );

  if( *errorCode != 0) return FALSE;	
  ApiCalculationRequest_appendBuffer(request->intermediate, request->intLength, API_EOL);
  ApiCalculationRequest_appendBuffer(request->intermediate,
				     request->intLength,
				     "11.1.7.1 Example 7");
  ApiCalculationRequest_appendBuffer(request->intermediate, request->intLength, API_EOL);
  tmp  =  ApiCalculationTest_check11_1_7_1(request,
					   resultRequest,
					   FALSE,
					   "11.1.7.1 Example 7",
					   errBuf,
					   errLength,
					   errorCode);
  if( tmp == FALSE || *errorCode != 0) result = FALSE;


  /** Example 8 */
  *errorCode = ApiCalculation_set11_1_7_1Parameters(request,
						    API_CRUDE_OIL_NAME,
						    0, NULL, 
						    -50.025, ApiUnit_Temperature_C(),
						    -5, ApiUnit_Pressure_KPA(),
						    610.46, ApiUnit_Density_KGM3(),

						    15, ApiUnit_Temperature_C(),

						    0,NULL);
  if( *errorCode != 0) return FALSE;
  request->altVol = ApiVolume_init(1000, ApiUnit_Volume_M3(), TRUE, errorCode);
  if( *errorCode != 0) return FALSE;
  ApiCalculationRequest_appendBuffer(request->intermediate, request->intLength, API_EOL);
  ApiCalculationRequest_appendBuffer(request->intermediate,
				     request->intLength,
				     "11.1.7.1 Example 8");
  ApiCalculationRequest_appendBuffer(request->intermediate, request->intLength, API_EOL);
  tmp  =  ApiCalculationTest_check11_1_7_1(request,
					   resultRequest,
					   TRUE,
					   "11.1.7.1 Example 8",
					   errBuf,
					   errLength,
					   errorCode);
  if( tmp == FALSE || *errorCode != 0) result = FALSE;

  /** Example 9 */
  *errorCode = ApiCalculation_set11_1_7_1Parameters(request,
						    API_REFINED_PRODUCTS_NAME,
						    0, NULL, 
						    150.025, ApiUnit_Temperature_C(),
						    10343, ApiUnit_Pressure_KPA(),
						    1163.55, ApiUnit_Density_KGM3(),

						    20, ApiUnit_Temperature_C(),

						    0,NULL);
  if( *errorCode != 0) return FALSE;
  request->altVol = ApiVolume_init(1000, ApiUnit_Volume_M3(), TRUE, errorCode);
  if( *errorCode != 0) return FALSE;
  ApiCalculationRequest_appendBuffer(request->intermediate, request->intLength, API_EOL);
  ApiCalculationRequest_appendBuffer(request->intermediate,
				     request->intLength,
				     "11.1.7.1 Example 9");
  ApiCalculationRequest_appendBuffer(request->intermediate, request->intLength, API_EOL);
  tmp  =  ApiCalculationTest_check11_1_7_1(request,
					   resultRequest,
					   TRUE,
					   "11.1.7.1 Example 9",
					   errBuf,
					   errLength,
					   errorCode);
  if( tmp == FALSE || *errorCode != 0) result = FALSE;

  /** Example 10 */
  *errorCode = ApiCalculation_set11_1_7_1Parameters(request,
						    API_LUBRICATING_OIL_NAME,
						    0, NULL, 
						    -50.025, ApiUnit_Temperature_C(),
						    0, ApiUnit_Pressure_KPA(),
						    800.849, ApiUnit_Density_KGM3(),

						    15, ApiUnit_Temperature_C(),

						    0,NULL);
  if( *errorCode != 0) return FALSE;
  request->altVol = ApiVolume_init(1000, ApiUnit_Volume_M3(), TRUE, errorCode);
  if( *errorCode != 0) return FALSE;
  ApiCalculationRequest_appendBuffer(request->intermediate, request->intLength, API_EOL);
  ApiCalculationRequest_appendBuffer(request->intermediate,
				     request->intLength,
				     "11.1.7.1 Example 10");
  ApiCalculationRequest_appendBuffer(request->intermediate, request->intLength, API_EOL);
  tmp  =  ApiCalculationTest_check11_1_7_1(request,
					   resultRequest,
					   TRUE,
					   "11.1.7.1 Example 10",
					   errBuf,
					   errLength,
					   errorCode);
  if( tmp == FALSE || *errorCode != 0) result = FALSE;

  /** Example 11 */
  *errorCode = ApiCalculation_set11_1_7_1Parameters(request,
						    API_LUBRICATING_OIL_NAME,
						    0, NULL, 
						    150.025, ApiUnit_Temperature_C(),
						    103.43, ApiUnit_Pressure_BAR(),
						    1163.55, ApiUnit_Density_KGM3(),

						    20, ApiUnit_Temperature_C(),

						    0,NULL);
  if( *errorCode != 0) return FALSE;
  request->altVol = ApiVolume_init(1000, ApiUnit_Volume_M3(), TRUE, errorCode);
  if( *errorCode != 0) return FALSE;
  ApiCalculationRequest_appendBuffer(request->intermediate, request->intLength, API_EOL);
  ApiCalculationRequest_appendBuffer(request->intermediate,
				     request->intLength,
				     "11.1.7.1 Example 11");
  ApiCalculationRequest_appendBuffer(request->intermediate, request->intLength, API_EOL);
  tmp  =  ApiCalculationTest_check11_1_7_1(request,
					   resultRequest,
					   TRUE,
					   "11.1.7.1 Example 11",
					   errBuf,
					   errLength,
					   errorCode);
  if( tmp == FALSE || *errorCode != 0) result = FALSE;

  /** Clean up the result object */
  ApiCalculationRequest_cleanUp(resultRequest);
  resultRequest = NULL;

  return result;
}
