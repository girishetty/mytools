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
*  A convienence method to set the results for the 11.1.6.1 results
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
int  ApiCalculationTest_setResults11_1_6_1Test(ApiCalculationRequest *result,
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
  return errorCode;
};

/**
* Run the test for the 11.1.6.1 calculations
*
*/
bool  ApiCalculationTest_run11_1_6_1Tests(ApiCalculationRequest * request, 
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

  resultRequest->altDens = ApiDensity_init(1,  ApiUnit_Density_KGM3(), TRUE, errorCode);
  if( *errorCode != 0) return FALSE;

  resultRequest->baseVol = ApiVolume_init(1, ApiUnit_Volume_M3(), TRUE, errorCode);
  if( *errorCode != 0) return FALSE;

  /** Example 1 */
  *errorCode = ApiCalculation_set11_1_6_1Parameters(request,
						    API_CRUDE_OIL_NAME,
						    0, NULL,
						    -27.7, ApiUnit_Temperature_F(),
						    0, ApiUnit_Pressure_PSI(),
						    17.785, ApiUnit_Density_API(),
						    0,NULL);
  if( *errorCode != 0) return FALSE;
  request->altVol = ApiVolume_init(987.99,  ApiUnit_Volume_M3(), TRUE, errorCode);
  if( *errorCode != 0) return FALSE;
  *errorCode = ApiCalculationTest_setResults11_1_6_1Test(resultRequest,
							0.305779891997,
							ApiUnit_ScaledComp_REV_PSI(),
							1.033011591958,
							1.0,
							1.033011591958,
							978.1780343640,
							ApiUnit_Density_KGM3(),
							1020.6035499, 
							ApiUnit_Volume_M3() );

  if( *errorCode != 0) return FALSE;	
  ApiCalculationRequest_appendBuffer(request->intermediate, request->intLength, API_EOL);
  ApiCalculationRequest_appendBuffer(request->intermediate,
				     request->intLength,
				     "11.1.6.1 Example 1");
  ApiCalculationRequest_appendBuffer(request->intermediate, request->intLength, API_EOL);
  tmp  =  ApiCalculationTest_checkResult(request,
					 resultRequest,
					 FALSE,
					 "11.1.6.1 Example 1",
					 errBuf,
					 errLength,
					 errorCode);
  if( tmp == FALSE || *errorCode != 0) result = FALSE;


  /** Example 2 */
  *errorCode = ApiCalculation_set11_1_6_1Parameters(request,
						    API_CRUDE_OIL_NAME,
						    0, NULL,
						    301.93, ApiUnit_Temperature_F(),
						    1500, ApiUnit_Pressure_PSI(),
						    -10, ApiUnit_Density_API(),
						    0, NULL);
  if( *errorCode != 0) return FALSE;
  request->altVol = ApiVolume_init(285.5,  ApiUnit_Volume_M3(), TRUE, errorCode);
  if( *errorCode != 0) return FALSE;
  *errorCode = ApiCalculationTest_setResults11_1_6_1Test(resultRequest,
							0.427958509999,
							ApiUnit_ScaledComp_REV_PSI(),
							0.938051116886,
							1.006460852301,
							0.944111726603,
							1098.4391355882,
							ApiUnit_Density_KGM3(),
							269.543405, 
							ApiUnit_Volume_M3() );

  if( *errorCode != 0) return FALSE;	
  ApiCalculationRequest_appendBuffer(request->intermediate, request->intLength, API_EOL);
  ApiCalculationRequest_appendBuffer(request->intermediate,
				     request->intLength,
				     "11.1.6.1 Example 2");
  ApiCalculationRequest_appendBuffer(request->intermediate, request->intLength, API_EOL);
  tmp  =  ApiCalculationTest_checkResult(request,
					 resultRequest,
					 FALSE,
					 "11.1.6.1 Example 2",
					 errBuf,
					 errLength,
					 errorCode);
  if( tmp == FALSE || *errorCode != 0) result = FALSE;


  /** Example 3 */
  *errorCode = ApiCalculation_set11_1_6_1Parameters(request,
						    API_REFINED_PRODUCTS_NAME,
						    0, NULL,
						    48.04, ApiUnit_Temperature_F(),
						    -7.3, ApiUnit_Pressure_PSI(),
						    19.4, ApiUnit_Density_API(),
						    0, NULL);
  if( *errorCode != 0) return FALSE;
  request->altVol = ApiVolume_init(12002,  ApiUnit_Volume_M3(), TRUE, errorCode);
  if( *errorCode != 0) return FALSE;
  *errorCode = ApiCalculationTest_setResults11_1_6_1Test(resultRequest,
							0.384339609206,
							ApiUnit_ScaledComp_REV_PSI(),
							1.004858068990,
							1.0,
							1.004858068990,
							941.3353501926,
							ApiUnit_Density_KGM3(),
							12060.32972, 
							ApiUnit_Volume_M3() );

  if( *errorCode != 0) return FALSE;	
  ApiCalculationRequest_appendBuffer(request->intermediate, request->intLength, API_EOL);
  ApiCalculationRequest_appendBuffer(request->intermediate,
				     request->intLength,
				     "11.1.6.1 Example 3");
  ApiCalculationRequest_appendBuffer(request->intermediate, request->intLength, API_EOL);
  tmp  =  ApiCalculationTest_checkResult(request,
					 resultRequest,
					 FALSE,
					 "11.1.6.1 Example 3",
					 errBuf,
					 errLength,
					 errorCode);
  if( tmp == FALSE || *errorCode != 0) result = FALSE;

  /** Example 4 */
  *errorCode = ApiCalculation_set11_1_6_1Parameters(request,
						    API_REFINED_PRODUCTS_NAME,
						    0, NULL,
						    85.0, ApiUnit_Temperature_F(),
						    247.3, ApiUnit_Pressure_PSI(),
						    0.7943, ApiUnit_Density_RELATIVE(),
						    0, NULL);
  if( *errorCode != 0) return FALSE;
  request->altVol = ApiVolume_init(1000,  ApiUnit_Volume_M3(), TRUE, errorCode);
  if( *errorCode != 0) return FALSE;
  *errorCode = ApiCalculationTest_setResults11_1_6_1Test(resultRequest,
							 0.664706197066,
							 ApiUnit_ScaledComp_REV_PSI(),
						         0.986832406683,
							 1.001646525013,
							 0.988457250925,
							 784.3590249208,
							 ApiUnit_Density_KGM3(),
							 988.46, 
							 ApiUnit_Volume_M3() );

  if( *errorCode != 0) return FALSE;	
  ApiCalculationRequest_appendBuffer(request->intermediate, request->intLength, API_EOL);
  ApiCalculationRequest_appendBuffer(request->intermediate,
				     request->intLength,
				     "11.1.6.1 Example 4");
  ApiCalculationRequest_appendBuffer(request->intermediate, request->intLength, API_EOL);
  tmp  =  ApiCalculationTest_checkResult(request,
					 resultRequest,
					 FALSE,
					 "11.1.6.1 Example 4",
					 errBuf,
					 errLength,
					 errorCode);
  if( tmp == FALSE || *errorCode != 0) result = FALSE;


  /** Example 5 */
  *errorCode = ApiCalculation_set11_1_6_1Parameters(request,
						    API_REFINED_PRODUCTS_NAME,
						    0, NULL,
						    -32.8, ApiUnit_Temperature_F(),
						    124.63, ApiUnit_Pressure_PSI(),
						    772.32, ApiUnit_Density_KGM3(),
						    0, NULL);
  if( *errorCode != 0) return FALSE;
  request->altVol = ApiVolume_init(327468.0,  ApiUnit_Volume_M3(), TRUE, errorCode);
  if( *errorCode != 0) return FALSE;
  *errorCode = ApiCalculationTest_setResults11_1_6_1Test(resultRequest,
							 0.451102239378,
							 ApiUnit_ScaledComp_REV_PSI(),
						         1.057147343671,
							 1.000562524977,
							 1.057742015457,
							 816.9153133776,
							 ApiUnit_Density_KGM3(),
							 346376.00232, 
							 ApiUnit_Volume_M3() );

  if( *errorCode != 0) return FALSE;	
  ApiCalculationRequest_appendBuffer(request->intermediate, request->intLength, API_EOL);
  ApiCalculationRequest_appendBuffer(request->intermediate,
				     request->intLength,
				     "11.1.6.1 Example 5");
  ApiCalculationRequest_appendBuffer(request->intermediate, request->intLength, API_EOL);
  tmp  =  ApiCalculationTest_checkResult(request,
					 resultRequest,
					 FALSE,
					 "11.1.6.1 Example 5",
					 errBuf,
					 errLength,
					 errorCode);
  if( tmp == FALSE || *errorCode != 0) result = FALSE;


  /** Example 6 */
  *errorCode = ApiCalculation_set11_1_6_1Parameters(request,
						    API_REFINED_PRODUCTS_NAME,
						    0, NULL,
						    27.3, ApiUnit_Temperature_F(),
						    1234.5, ApiUnit_Pressure_PSI(),
						    657.3, ApiUnit_Density_KGM3(),
						    0, NULL);
  if( *errorCode != 0) return FALSE;
  request->altVol = ApiVolume_init(14.72,  ApiUnit_Volume_M3(), TRUE, errorCode);
  if( *errorCode != 0) return FALSE;
  *errorCode = ApiCalculationTest_setResults11_1_6_1Test(resultRequest,
							 0.993527440282,
							 ApiUnit_ScaledComp_REV_PSI(),
						         1.026475833518,
							 1.012417396817,
							 1.039221991267,
							 683.0806148596,
							 ApiUnit_Density_KGM3(),
							 15.2973184, 
							 ApiUnit_Volume_M3() );

  if( *errorCode != 0) return FALSE;	
  ApiCalculationRequest_appendBuffer(request->intermediate, request->intLength, API_EOL);
  ApiCalculationRequest_appendBuffer(request->intermediate,
				     request->intLength,
				     "11.1.6.1 Example 6");
  ApiCalculationRequest_appendBuffer(request->intermediate, request->intLength, API_EOL);
  tmp  =  ApiCalculationTest_checkResult(request,
					 resultRequest,
					 FALSE,
					 "11.1.6.1 Example 6",
					 errBuf,
					 errLength,
					 errorCode);
  if( tmp == FALSE || *errorCode != 0) result = FALSE;

  /** Example 7 */
  *errorCode = ApiCalculation_set11_1_6_1Parameters(request,
						    API_COMMODITY_NOT_GIVEN,
						    0.000732185,
						    ApiUnit_ThermalExp_REV_F(),
						    97.7, ApiUnit_Temperature_F(),
						    287.4, ApiUnit_Pressure_PSI(),
						    1.0537, ApiUnit_Density_RELATIVE(),
						    0, NULL);
  if( *errorCode != 0) return FALSE;
  request->altVol = ApiVolume_init(203.85,  ApiUnit_Volume_M3(), TRUE, errorCode);
  if( *errorCode != 0) return FALSE;
  *errorCode = ApiCalculationTest_setResults11_1_6_1Test(resultRequest,
							 0.346454796729,
							 ApiUnit_ScaledComp_REV_PSI(),
						         0.9721737956968,
							 1.000996703515,
							 0.973142764735,
							 1024.3915370787,
							 ApiUnit_Density_KGM3(),
							 198.374589, 
							 ApiUnit_Volume_M3() );

  if( *errorCode != 0) return FALSE;	
  ApiCalculationRequest_appendBuffer(request->intermediate, request->intLength, API_EOL);
  ApiCalculationRequest_appendBuffer(request->intermediate,
				     request->intLength,
				     "11.1.6.1 Example 7");
  ApiCalculationRequest_appendBuffer(request->intermediate, request->intLength, API_EOL);
  tmp  =  ApiCalculationTest_checkResult(request,
					 resultRequest,
					 FALSE,
					 "11.1.6.1 Example 7",
					 errBuf,
					 errLength,
					 errorCode);
  if( tmp == FALSE || *errorCode != 0) result = FALSE;


  /** Example 8 */
  *errorCode = ApiCalculation_set11_1_6_1Parameters(request,
						    API_LUBRICATING_OIL_NAME,
						    0, NULL,
						    129.3, ApiUnit_Temperature_F(),
						    138.8, ApiUnit_Pressure_PSI(),
						    42.9, ApiUnit_Density_API(),
						    0, NULL);
  if( *errorCode != 0) return FALSE;
  request->altVol = ApiVolume_init(99.98,  ApiUnit_Volume_M3(), TRUE, errorCode);
  if( *errorCode != 0) return FALSE;
  *errorCode = ApiCalculationTest_setResults11_1_6_1Test(resultRequest,
							 0.732650236105,
							 ApiUnit_ScaledComp_REV_PSI(),
						         0.969922293864,
							 1.001017953704,
							 0.970909629855,
							 786.9754991473,
							 ApiUnit_Density_KGM3(),
							 97.0715818, 
							 ApiUnit_Volume_M3() );

  if( *errorCode != 0) return FALSE;	
  ApiCalculationRequest_appendBuffer(request->intermediate, request->intLength, API_EOL);
  ApiCalculationRequest_appendBuffer(request->intermediate,
				     request->intLength,
				     "11.1.6.1 Example 8");
  ApiCalculationRequest_appendBuffer(request->intermediate, request->intLength, API_EOL);
  tmp  =  ApiCalculationTest_checkResult(request,
					 resultRequest,
					 FALSE,
					 "11.1.6.1 Example 8",
					 errBuf,
					 errLength,
					 errorCode);
  if( tmp == FALSE || *errorCode != 0) result = FALSE;


  /** Example 9 */
  *errorCode = ApiCalculation_set11_1_6_1Parameters(request,
						    API_CRUDE_OIL_NAME,
						    0, NULL,
						    -58.05, ApiUnit_Temperature_F(),
						    0.0, ApiUnit_Pressure_PSI(),
						    100, ApiUnit_Density_API(),
						    0, NULL);
  if( *errorCode != 0) return FALSE;
  request->altVol = ApiVolume_init(1000,  ApiUnit_Volume_M3(), TRUE, errorCode);
  if( *errorCode != 0) return FALSE;
  ApiCalculationRequest_appendBuffer(request->intermediate, request->intLength, API_EOL);
  ApiCalculationRequest_appendBuffer(request->intermediate,
				     request->intLength,
				     "11.1.6.1 Example 9");
  ApiCalculationRequest_appendBuffer(request->intermediate, request->intLength, API_EOL);
  tmp  =  ApiCalculationTest_checkResult(request,
					 resultRequest,
					 TRUE,
					 "11.1.6.1 Example 9",
					 errBuf,
					 errLength,
					 errorCode);
  if( tmp == FALSE || *errorCode != 0) result = FALSE;


  /** Example 10 */
  *errorCode = ApiCalculation_set11_1_6_1Parameters(request,
						    API_CRUDE_OIL_NAME,
						    0, NULL,
						    302.05, ApiUnit_Temperature_F(),
						    0.0, ApiUnit_Pressure_PSI(),
						    100, ApiUnit_Density_API(),
						    0, NULL);
  if( *errorCode != 0) return FALSE;
  request->altVol = ApiVolume_init(1000,  ApiUnit_Volume_M3(), TRUE, errorCode);
  if( *errorCode != 0) return FALSE;
  ApiCalculationRequest_appendBuffer(request->intermediate, request->intLength, API_EOL);
  ApiCalculationRequest_appendBuffer(request->intermediate,
				     request->intLength,
				     "11.1.6.1 Example 10");
  ApiCalculationRequest_appendBuffer(request->intermediate, request->intLength, API_EOL);
  tmp  =  ApiCalculationTest_checkResult(request,
					 resultRequest,
					 TRUE,
					 "11.1.6.1 Example 10",
					 errBuf,
					 errLength,
					 errorCode);
  if( tmp == FALSE || *errorCode != 0) result = FALSE;

  /** Example 11 */
  *errorCode = ApiCalculation_set11_1_6_1Parameters(request,
						    API_CRUDE_OIL_NAME,
						    0, NULL,
						    92.0, ApiUnit_Temperature_F(),
						    1501, ApiUnit_Pressure_PSI(),
						    100, ApiUnit_Density_API(),
						    0, NULL);
  if( *errorCode != 0) return FALSE;
  request->altVol = ApiVolume_init(1000,  ApiUnit_Volume_M3(), TRUE, errorCode);
  if( *errorCode != 0) return FALSE;
  ApiCalculationRequest_appendBuffer(request->intermediate, request->intLength, API_EOL);
  ApiCalculationRequest_appendBuffer(request->intermediate,
				     request->intLength,
				     "11.1.6.1 Example 11");
  ApiCalculationRequest_appendBuffer(request->intermediate, request->intLength, API_EOL);
  tmp  =  ApiCalculationTest_checkResult(request,
					 resultRequest,
					 TRUE,
					 "11.1.6.1 Example 11",
					 errBuf,
					 errLength,
					 errorCode);
  if( tmp == FALSE || *errorCode != 0) result = FALSE;


  /** Example 12 */
  *errorCode = ApiCalculation_set11_1_6_1Parameters(request,
						    API_CRUDE_OIL_NAME,
						    0, NULL,
						    72.0, ApiUnit_Temperature_F(),
						    0, ApiUnit_Pressure_PSI(),
						    100.06, ApiUnit_Density_API(),
						    0, NULL);
  if( *errorCode != 0) return FALSE;
  request->altVol = ApiVolume_init(1000,  ApiUnit_Volume_M3(), TRUE, errorCode);
  if( *errorCode != 0) return FALSE;
  ApiCalculationRequest_appendBuffer(request->intermediate, request->intLength, API_EOL);
  ApiCalculationRequest_appendBuffer(request->intermediate,
				     request->intLength,
				     "11.1.6.1 Example 12");
  ApiCalculationRequest_appendBuffer(request->intermediate, request->intLength, API_EOL);
  tmp  =  ApiCalculationTest_checkResult(request,
					 resultRequest,
					 TRUE,
					 "11.1.6.1 Example 12",
					 errBuf,
					 errLength,
					 errorCode);
  if( tmp == FALSE || *errorCode != 0) result = FALSE;


  /** Example 13 */
  *errorCode = ApiCalculation_set11_1_6_1Parameters(request,
						    API_REFINED_PRODUCTS_NAME,
						    0, NULL,
						    302.0, ApiUnit_Temperature_F(),
						    0, ApiUnit_Pressure_PSI(),
						    -10.08, ApiUnit_Density_API(),
						    0, NULL);
  if( *errorCode != 0) return FALSE;
  request->altVol = ApiVolume_init(1000,  ApiUnit_Volume_M3(), TRUE, errorCode);
  if( *errorCode != 0) return FALSE;
  ApiCalculationRequest_appendBuffer(request->intermediate, request->intLength, API_EOL);
  ApiCalculationRequest_appendBuffer(request->intermediate,
				     request->intLength,
				     "11.1.6.1 Example 13");
  ApiCalculationRequest_appendBuffer(request->intermediate, request->intLength, API_EOL);
  tmp  =  ApiCalculationTest_checkResult(request,
					 resultRequest,
					 TRUE,
					 "11.1.6.1 Example 13",
					 errBuf,
					 errLength,
					 errorCode);
  if( tmp == FALSE || *errorCode != 0) result = FALSE;


  /** Example 14 */
  *errorCode = ApiCalculation_set11_1_6_1Parameters(request,
						    API_LUBRICATING_OIL_NAME,
						    0, NULL,
						    0.0, ApiUnit_Temperature_F(),
						    0, ApiUnit_Pressure_PSI(),
						    45.08, ApiUnit_Density_API(),
						    0, NULL);
  if( *errorCode != 0) return FALSE;
  request->altVol = ApiVolume_init(1000,  ApiUnit_Volume_M3(), TRUE, errorCode);
  if( *errorCode != 0) return FALSE;
  ApiCalculationRequest_appendBuffer(request->intermediate, request->intLength, API_EOL);
  ApiCalculationRequest_appendBuffer(request->intermediate,
				     request->intLength,
				     "11.1.6.1 Example 14");
  ApiCalculationRequest_appendBuffer(request->intermediate, request->intLength, API_EOL);
  tmp  =  ApiCalculationTest_checkResult(request,
					 resultRequest,
					 TRUE,
					 "11.1.6.1 Example 14",
					 errBuf,
					 errLength,
					 errorCode);
  if( tmp == FALSE || *errorCode != 0) result = FALSE;


  /** Example 15 */
  *errorCode = ApiCalculation_set11_1_6_1Parameters(request,
						    API_CRUDE_OIL_NAME,
						    0, NULL,
						    200, ApiUnit_Temperature_F(),
						    0, ApiUnit_Pressure_PSI(),
						    100, ApiUnit_Density_API(),
						    0, NULL);
  if( *errorCode != 0) return FALSE;
  request->altVol = ApiVolume_init(1000,  ApiUnit_Volume_M3(), TRUE, errorCode);
  if( *errorCode != 0) return FALSE;
  *errorCode = ApiCalculationTest_setResults11_1_6_1Test(resultRequest,
							 4.092820997603,
							 ApiUnit_ScaledComp_REV_PSI(),
						         0.868288296907,
							 1.0,
							 0.868288296907,
							 530.2025789330,
							 ApiUnit_Density_KGM3(),
							 868.29, 
							 ApiUnit_Volume_M3() );

  if( *errorCode != 0) return FALSE;	
  ApiCalculationRequest_appendBuffer(request->intermediate, request->intLength, API_EOL);
  ApiCalculationRequest_appendBuffer(request->intermediate,
				     request->intLength,
				     "11.1.6.1 Example 15");
  ApiCalculationRequest_appendBuffer(request->intermediate, request->intLength, API_EOL);
  tmp  =  ApiCalculationTest_checkResult(request,
					 resultRequest,
					 FALSE,
					 "11.1.6.1 Example 15",
					 errBuf,
					 errLength,
					 errorCode);
  if( tmp == FALSE || *errorCode != 0) result = FALSE;

  /** Clean up the result object */
  ApiCalculationRequest_cleanUp(resultRequest);
  resultRequest = NULL;

  return result;
}
