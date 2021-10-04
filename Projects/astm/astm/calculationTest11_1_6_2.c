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

/** 
*  A convienence method to set the results for the 11.1.6.2 results
* 
* @param result the CalculationRequest instance to set
* @param scaled the Scaled Compensation Factor expected
* @param su the unit to use for the Scaled Compensation Factor
* @param ctlObToBase the ctl factor expected
* @param cplObToBase the cpl factor expected
* @param ctplObToBase the ctpl factor expected
* @param baseDens the expected density
* @param du  the unit of the expected density
* @param vol the expected volume
* @param vu the unit of the expected volume
*/
int  ApiCalculationTest_setResults11_1_6_2Test(ApiCalculationRequest *result,
					       double scaled, ApiUnit* su,
					       double ctlObToBase,
					       double cplObToBase,
					       double ctplObToBase,
					       double baseDens, ApiUnit * du,
					       double vol, ApiUnit* vu){
  int errorCode = 0;

  errorCode =  ApiCalculationTest_setResult(result,

					    baseDens, du,
					    ctlObToBase,
					    scaled, su,
					    cplObToBase,
					    ctplObToBase,
					    vol, vu,

					    1.0,
					    0.0, NULL,
					    1.0,
					    1.0,

					    0.0, NULL,
					    0.0, NULL,
					    0.0, NULL);

  return errorCode;
};



/**
* Run the test for the 11.1.6.2 calculations
*
*/
bool  ApiCalculationTest_run11_1_6_2Tests(ApiCalculationRequest * request, 
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


  /** Set the values we want to test for */
  resultRequest->scaledCompOb = ApiScaledFactor_init(1, 
						     ApiUnit_ScaledComp_REV_PSI(),
						     TRUE,
						     errorCode);
  if( *errorCode != 0) return FALSE;

  resultRequest->ctlObToBase = ApiExpansionFactor_init(1, 
						       ApiUnit_Expansion_DIMLESS(),
						       TRUE,
						       errorCode);
  if( *errorCode != 0) return FALSE;

  resultRequest->cplObToBase = ApiExpansionFactor_init(1, 
							ApiUnit_Expansion_DIMLESS(),
							TRUE,
							errorCode);
  if( *errorCode != 0) return FALSE;

  resultRequest->ctplObToBase = ApiExpansionFactor_init(1, 
							 ApiUnit_Expansion_DIMLESS(),
							 TRUE,
							 errorCode);
  if( *errorCode != 0) return FALSE;

  resultRequest->baseDens = ApiDensity_init(1,  ApiUnit_Density_KGM3(), TRUE, errorCode);
  if( *errorCode != 0) return FALSE;

  resultRequest->baseVol = ApiVolume_init(1, ApiUnit_Volume_M3(), TRUE, errorCode);
  if( *errorCode != 0) return FALSE;

  /** Example 1 */
  *errorCode = ApiCalculation_set11_1_6_2Parameters(request,
						    API_CRUDE_OIL_NAME,
						    0, NULL,
						    80.3, ApiUnit_Temperature_F(),
						    -5, ApiUnit_Pressure_PSI(),
						    823.7, ApiUnit_Density_KGM3(),
						    1000, ApiUnit_Volume_M3());
  if( *errorCode != 0) return FALSE;
  *errorCode = ApiCalculationTest_setResults11_1_6_2Test(resultRequest,
							0.567045450015,
							ApiUnit_ScaledComp_REV_PSI(),
							0.989966310837,
							1.0,
							0.989966310837,
							832.048516184234,
							ApiUnit_Density_KGM3(),
							989.97, 
							ApiUnit_Volume_M3() );

  if( *errorCode != 0) return FALSE;	
  ApiCalculationRequest_appendBuffer(request->intermediate, request->intLength, API_EOL);
  ApiCalculationRequest_appendBuffer(request->intermediate,
				     request->intLength,
				     "11.1.6.2 Example 1");
  ApiCalculationRequest_appendBuffer(request->intermediate, request->intLength, API_EOL);
  tmp  =  ApiCalculationTest_checkResult(request,
					 resultRequest,
					 FALSE,
					 "11.1.6.2 Example 1",
					 errBuf,
					 errLength,
					 errorCode);
  if( tmp == FALSE || *errorCode != 0) result = FALSE;


  /** Example 2 */
  *errorCode = ApiCalculation_set11_1_6_2Parameters(request,
						    API_CRUDE_OIL_NAME,
						    0, NULL,
						    -57.95, ApiUnit_Temperature_F(),
						    113.5, ApiUnit_Pressure_PSI(),
						    0.72332, ApiUnit_Density_RELATIVE(),
						    637483.0, ApiUnit_Volume_M3());
  if( *errorCode != 0) return FALSE;
  *errorCode = ApiCalculationTest_setResults11_1_6_2Test(resultRequest,
							0.603436540820,
							ApiUnit_ScaledComp_REV_PSI(),
							1.088429741690,
							1.000685369884,
						        1.089175718656,
							663.445062852402,
							ApiUnit_Density_KGM3(),
							694333.73394, 
							ApiUnit_Volume_M3() );

  if( *errorCode != 0) return FALSE;	
  ApiCalculationRequest_appendBuffer(request->intermediate, request->intLength, API_EOL);
  ApiCalculationRequest_appendBuffer(request->intermediate,
				     request->intLength,
				     "11.1.6.2 Example 2");
  ApiCalculationRequest_appendBuffer(request->intermediate, request->intLength, API_EOL);
  tmp  =  ApiCalculationTest_checkResult(request,
					 resultRequest,
					 FALSE,
					 "11.1.6.2 Example 2",
					 errBuf,
					 errLength,
					 errorCode);
  if( tmp == FALSE || *errorCode != 0) result = FALSE;

  /** Example 3 */
  *errorCode = ApiCalculation_set11_1_6_2Parameters(request,
						    API_REFINED_PRODUCTS_NAME,
						    0, NULL,
						    68.0, ApiUnit_Temperature_F(),
						    11.0, ApiUnit_Pressure_PSI(),
						    0.8665, ApiUnit_Density_RELATIVE(),
						    28.45, ApiUnit_Volume_M3());
  if( *errorCode != 0) return FALSE;
  *errorCode = ApiCalculationTest_setResults11_1_6_2Test(resultRequest,
							0.484824263921,
							ApiUnit_ScaledComp_REV_PSI(),
							0.996406814700,
							1.000053333513,
						        0.996459956576,
						        868.722680008428,
							ApiUnit_Density_KGM3(),
							28.349287, 
							ApiUnit_Volume_M3() );

  if( *errorCode != 0) return FALSE;	
  ApiCalculationRequest_appendBuffer(request->intermediate, request->intLength, API_EOL);
  ApiCalculationRequest_appendBuffer(request->intermediate,
				     request->intLength,
				     "11.1.6.2 Example 3");
  ApiCalculationRequest_appendBuffer(request->intermediate, request->intLength, API_EOL);
  tmp  =  ApiCalculationTest_checkResult(request,
					 resultRequest,
					 FALSE,
					 "11.1.6.2 Example 3",
					 errBuf,
					 errLength,
					 errorCode);
  if( tmp == FALSE || *errorCode != 0) result = FALSE;


  /** Example 4 */
  *errorCode = ApiCalculation_set11_1_6_2Parameters(request,
						    API_REFINED_PRODUCTS_NAME,
						    0, NULL,
						    72.0, ApiUnit_Temperature_F(),
						    375.0, ApiUnit_Pressure_PSI(),
						    41.4, ApiUnit_Density_API(),
						    200.5, ApiUnit_Volume_M3());
  if( *errorCode != 0) return FALSE;
  *errorCode = ApiCalculationTest_setResults11_1_6_2Test(resultRequest,
							 0.572609969057,
							 ApiUnit_ScaledComp_REV_PSI(),
							 0.994103137645,
							 1.002151908149,
						         0.996242356288,
						         820.670624131605,
							 ApiUnit_Density_KGM3(),
							 199.74612, 
							 ApiUnit_Volume_M3() );

  if( *errorCode != 0) return FALSE;	
  ApiCalculationRequest_appendBuffer(request->intermediate, request->intLength, API_EOL);
  ApiCalculationRequest_appendBuffer(request->intermediate,
				     request->intLength,
				     "11.1.6.2 Example 4");
  ApiCalculationRequest_appendBuffer(request->intermediate, request->intLength, API_EOL);
  tmp  =  ApiCalculationTest_checkResult(request,
					 resultRequest,
					 FALSE,
					 "11.1.6.2 Example 4",
					 errBuf,
					 errLength,
					 errorCode);
  if( tmp == FALSE || *errorCode != 0) result = FALSE;

  /** Example 5 */
  *errorCode = ApiCalculation_set11_1_6_2Parameters(request,
						    API_REFINED_PRODUCTS_NAME,
						    0, NULL,
						    25.3, ApiUnit_Temperature_F(),
						    267.0, ApiUnit_Pressure_PSI(),
						    803.141, ApiUnit_Density_KGM3(),
						    9998.7, ApiUnit_Volume_M3());
  if( *errorCode != 0) return FALSE;
  *errorCode = ApiCalculationTest_setResults11_1_6_2Test(resultRequest,
							 0.539959363768,
							 ApiUnit_ScaledComp_REV_PSI(),
							 1.018381017381,
							 1.001443772976,
						         1.019851328373,
						         787.507922593917,
							 ApiUnit_Density_KGM3(),
							 10197.174195, 
							 ApiUnit_Volume_M3() );

  if( *errorCode != 0) return FALSE;	
  ApiCalculationRequest_appendBuffer(request->intermediate, request->intLength, API_EOL);
  ApiCalculationRequest_appendBuffer(request->intermediate,
				     request->intLength,
				     "11.1.6.2 Example 5");
  ApiCalculationRequest_appendBuffer(request->intermediate, request->intLength, API_EOL);
  tmp  =  ApiCalculationTest_checkResult(request,
					 resultRequest,
					 FALSE,
					 "11.1.6.2 Example 5",
					 errBuf,
					 errLength,
					 errorCode);
  if( tmp == FALSE || *errorCode != 0) result = FALSE;


  /** Example 6 */
  *errorCode = ApiCalculation_set11_1_6_2Parameters(request,
						    API_REFINED_PRODUCTS_NAME,
						    0, NULL,
						    41.85, ApiUnit_Temperature_F(),
						    248.3, ApiUnit_Pressure_PSI(),
						    49.85, ApiUnit_Density_API(),
						    485, ApiUnit_Volume_M3());
  if( *errorCode != 0) return FALSE;
  *errorCode = ApiCalculationTest_setResults11_1_6_2Test(resultRequest,
							 0.616704425784,
							 ApiUnit_ScaledComp_REV_PSI(),
							 1.011613933078,
							 1.001533625495,
						         1.013165369996,
						         769.3624510058947,
							 ApiUnit_Density_KGM3(),
							 491.38745, 
							 ApiUnit_Volume_M3() );

  if( *errorCode != 0) return FALSE;	
  ApiCalculationRequest_appendBuffer(request->intermediate, request->intLength, API_EOL);
  ApiCalculationRequest_appendBuffer(request->intermediate,
				     request->intLength,
				     "11.1.6.2 Example 6");
  ApiCalculationRequest_appendBuffer(request->intermediate, request->intLength, API_EOL);
  tmp  =  ApiCalculationTest_checkResult(request,
					 resultRequest,
					 FALSE,
					 "11.1.6.2 Example 6",
					 errBuf,
					 errLength,
					 errorCode);
  if( tmp == FALSE || *errorCode != 0) result = FALSE;

  /** Example 7 */
  *errorCode = ApiCalculation_set11_1_6_2Parameters(request,
						    API_COMMODITY_NOT_GIVEN,
						    0.00057634,
						    ApiUnit_ThermalExp_REV_F(),
						    84.5, ApiUnit_Temperature_F(),
						    573, ApiUnit_Pressure_PSI(),
						    853.7, ApiUnit_Density_KGM3(),
						    5000, ApiUnit_Volume_M3());
  if( *errorCode != 0) return FALSE;
  *errorCode = ApiCalculationTest_setResults11_1_6_2Test(resultRequest,
							 0.519616156675,
							 ApiUnit_ScaledComp_REV_PSI(),
							 0.985817857839,
							 1.002986291965,
						         0.988761797787,
						         863.403098613648,
							 ApiUnit_Density_KGM3(),
							 4943.8, 
							 ApiUnit_Volume_M3() );

  if( *errorCode != 0) return FALSE;	
  ApiCalculationRequest_appendBuffer(request->intermediate, request->intLength, API_EOL);
  ApiCalculationRequest_appendBuffer(request->intermediate,
				     request->intLength,
				     "11.1.6.2 Example 7");
  ApiCalculationRequest_appendBuffer(request->intermediate, request->intLength, API_EOL);
  tmp  =  ApiCalculationTest_checkResult(request,
					 resultRequest,
					 FALSE,
					 "11.1.6.2 Example 7",
					 errBuf,
					 errLength,
					 errorCode);
  if( tmp == FALSE || *errorCode != 0) result = FALSE;

  /** Example 8 */
  *errorCode = ApiCalculation_set11_1_6_2Parameters(request,
						    API_LUBRICATING_OIL_NAME,
						    0, NULL,
						    78.7, ApiUnit_Temperature_F(),
						    128, ApiUnit_Pressure_PSI(),
						    23.6, ApiUnit_Density_API(),
						    2500, ApiUnit_Volume_M3());
  if( *errorCode != 0) return FALSE;
  *errorCode = ApiCalculationTest_setResults11_1_6_2Test(resultRequest,
							 0.438941943315,
							 ApiUnit_ScaledComp_REV_PSI(),
							 0.992874116475,
							 1.000562161535,
						         0.993432272113,
						         917.442430351448,
							 ApiUnit_Density_KGM3(),
							 2483.575, 
							 ApiUnit_Volume_M3() );

  if( *errorCode != 0) return FALSE;	
  ApiCalculationRequest_appendBuffer(request->intermediate, request->intLength, API_EOL);
  ApiCalculationRequest_appendBuffer(request->intermediate,
				     request->intLength,
				     "11.1.6.2 Example 8");
  ApiCalculationRequest_appendBuffer(request->intermediate, request->intLength, API_EOL);
  tmp  =  ApiCalculationTest_checkResult(request,
					 resultRequest,
					 FALSE,
					 "11.1.6.2 Example 8",
					 errBuf,
					 errLength,
					 errorCode);
  if( tmp == FALSE || *errorCode != 0) result = FALSE;


  /** Example 9 */
  *errorCode = ApiCalculation_set11_1_6_2Parameters(request,
						    API_LUBRICATING_OIL_NAME,
						    0, NULL,
						    302, ApiUnit_Temperature_F(),
						    1499.97, ApiUnit_Pressure_PSI(),
						    62, ApiUnit_Density_API(),
						    989.4, ApiUnit_Volume_M3());
  if( *errorCode != 0) return FALSE;
  *errorCode = ApiCalculationTest_setResults11_1_6_2Test(resultRequest,
							 1.459310590459,
							 ApiUnit_ScaledComp_REV_PSI(),
							 0.892011191342,
							 1.022379081731,
						         0.911973582698,
						         801.061124838869,
							 ApiUnit_Density_KGM3(),
							 902.303118, 
							 ApiUnit_Volume_M3() );

  if( *errorCode != 0) return FALSE;	
  ApiCalculationRequest_appendBuffer(request->intermediate, request->intLength, API_EOL);
  ApiCalculationRequest_appendBuffer(request->intermediate,
				     request->intLength,
				     "11.1.6.2 Example 9");
  ApiCalculationRequest_appendBuffer(request->intermediate, request->intLength, API_EOL);
  tmp  =  ApiCalculationTest_checkResult(request,
					 resultRequest,
					 FALSE,
					 "11.1.6.2 Example 9",
					 errBuf,
					 errLength,
					 errorCode);
  if( tmp == FALSE || *errorCode != 0) result = FALSE;



  /** Example 10 */
  *errorCode = ApiCalculation_set11_1_6_2Parameters(request,
						    API_CRUDE_OIL_NAME,
						    0, NULL,
						    -58.05, ApiUnit_Temperature_F(),
						    0, ApiUnit_Pressure_PSI(),
						    169.16, ApiUnit_Density_API(),
						    1000, ApiUnit_Volume_M3());
  if( *errorCode != 0) return FALSE;
  ApiCalculationRequest_appendBuffer(request->intermediate, request->intLength, API_EOL);
  ApiCalculationRequest_appendBuffer(request->intermediate,
				     request->intLength,
				     "11.1.6.2 Example 10");
  ApiCalculationRequest_appendBuffer(request->intermediate, request->intLength, API_EOL);
  tmp  =  ApiCalculationTest_checkResult(request,
					 resultRequest,
					 TRUE,
					 "11.1.6.2 Example 10",
					 errBuf,
					 errLength,
					 errorCode);
  if( tmp == FALSE || *errorCode != 0) result = FALSE;


  /** Example 11 */
  *errorCode = ApiCalculation_set11_1_6_2Parameters(request,
						    API_CRUDE_OIL_NAME,
						    0, NULL,
						    302.8, ApiUnit_Temperature_F(),
						    1500.12, ApiUnit_Pressure_PSI(),
						    -13.88, ApiUnit_Density_API(),
						    1000, ApiUnit_Volume_M3());
  if( *errorCode != 0) return FALSE;
  ApiCalculationRequest_appendBuffer(request->intermediate, request->intLength, API_EOL);
  ApiCalculationRequest_appendBuffer(request->intermediate,
				     request->intLength,
				     "11.1.6.2 Example 11");
  ApiCalculationRequest_appendBuffer(request->intermediate, request->intLength, API_EOL);
  tmp  =  ApiCalculationTest_checkResult(request,
					 resultRequest,
					 TRUE,
					 "11.1.6.2 Example 11",
					 errBuf,
					 errLength,
					 errorCode);
  if( tmp == FALSE || *errorCode != 0) result = FALSE;


  /** Example 12 */
  *errorCode = ApiCalculation_set11_1_6_2Parameters(request,
						    API_REFINED_PRODUCTS_NAME,
						    0, NULL,
						    -58.05, ApiUnit_Temperature_F(),
						    0, ApiUnit_Pressure_PSI(),
						    470.17, ApiUnit_Density_KGM3(),
						    1000, ApiUnit_Volume_M3());
  if( *errorCode != 0) return FALSE;
  ApiCalculationRequest_appendBuffer(request->intermediate, request->intLength, API_EOL);
  ApiCalculationRequest_appendBuffer(request->intermediate,
				     request->intLength,
				     "11.1.6.2 Example 12");
  ApiCalculationRequest_appendBuffer(request->intermediate, request->intLength, API_EOL);
  tmp  =  ApiCalculationTest_checkResult(request,
					 resultRequest,
					 TRUE,
					 "11.1.6.2 Example 12",
					 errBuf,
					 errLength,
					 errorCode);
  if( tmp == FALSE || *errorCode != 0) result = FALSE;

  /** Example 13 */
  *errorCode = ApiCalculation_set11_1_6_2Parameters(request,
						    API_REFINED_PRODUCTS_NAME,
						    0, NULL,
						    302.8, ApiUnit_Temperature_F(),
						    0, ApiUnit_Pressure_PSI(),
						    1209.52, ApiUnit_Density_KGM3(),
						    1000, ApiUnit_Volume_M3());
  if( *errorCode != 0) return FALSE;
  ApiCalculationRequest_appendBuffer(request->intermediate, request->intLength, API_EOL);
  ApiCalculationRequest_appendBuffer(request->intermediate,
				     request->intLength,
				     "11.1.6.2 Example 13");
  ApiCalculationRequest_appendBuffer(request->intermediate, request->intLength, API_EOL);
  tmp  =  ApiCalculationTest_checkResult(request,
					 resultRequest,
					 TRUE,
					 "11.1.6.2 Example 13",
					 errBuf,
					 errLength,
					 errorCode);
  if( tmp == FALSE || *errorCode != 0) result = FALSE;

  /** Example 14 */
  *errorCode = ApiCalculation_set11_1_6_2Parameters(request,
						    API_LUBRICATING_OIL_NAME,
						    0, NULL,
						    60, ApiUnit_Temperature_F(),
						    0, ApiUnit_Pressure_PSI(),
						    -14.53, ApiUnit_Density_API(),
						    1000, ApiUnit_Volume_M3());
  if( *errorCode != 0) return FALSE;
  ApiCalculationRequest_appendBuffer(request->intermediate, request->intLength, API_EOL);
  ApiCalculationRequest_appendBuffer(request->intermediate,
				     request->intLength,
				     "11.1.6.2 Example 14");
  ApiCalculationRequest_appendBuffer(request->intermediate, request->intLength, API_EOL);
  tmp  =  ApiCalculationTest_checkResult(request,
					 resultRequest,
					 TRUE,
					 "11.1.6.2 Example 14",
					 errBuf,
					 errLength,
					 errorCode);
  if( tmp == FALSE || *errorCode != 0) result = FALSE;


  /** Example 15 */
  *errorCode = ApiCalculation_set11_1_6_2Parameters(request,
						    API_LUBRICATING_OIL_NAME,
						    0, NULL,
						    72.32, ApiUnit_Temperature_F(),
						    0, ApiUnit_Pressure_PSI(),
						    66.48, ApiUnit_Density_API(),
						    1000, ApiUnit_Volume_M3());
  if( *errorCode != 0) return FALSE;
  ApiCalculationRequest_appendBuffer(request->intermediate, request->intLength, API_EOL);
  ApiCalculationRequest_appendBuffer(request->intermediate,
				     request->intLength,
				     "11.1.6.2 Example 15");
  ApiCalculationRequest_appendBuffer(request->intermediate, request->intLength, API_EOL);
  tmp  =  ApiCalculationTest_checkResult(request,
					 resultRequest,
					 TRUE,
					 "11.1.6.2 Example 15",
					 errBuf,
					 errLength,
					 errorCode);
  if( tmp == FALSE || *errorCode != 0) result = FALSE;

  /** Example 16 */
  *errorCode = ApiCalculation_set11_1_6_2Parameters(request,
						    API_LUBRICATING_OIL_NAME,
						    0, NULL,
						    -57.97, ApiUnit_Temperature_F(),
						    240.7, ApiUnit_Pressure_PSI(),
						    -14.51, ApiUnit_Density_API(),
						    1000, ApiUnit_Volume_M3());
  if( *errorCode != 0) return FALSE;
  ApiCalculationRequest_appendBuffer(request->intermediate, request->intLength, API_EOL);
  ApiCalculationRequest_appendBuffer(request->intermediate,
				     request->intLength,
				     "11.1.6.2 Example 16");
  ApiCalculationRequest_appendBuffer(request->intermediate, request->intLength, API_EOL);
  tmp  =  ApiCalculationTest_checkResult(request,
					 resultRequest,
					 TRUE,
					 "11.1.6.2 Example 16",
					 errBuf,
					 errLength,
					 errorCode);
  if( tmp == FALSE || *errorCode != 0) result = FALSE;



  /** Clean up the result object */
  ApiCalculationRequest_cleanUp(resultRequest);
  resultRequest = NULL;


  return result;
}


