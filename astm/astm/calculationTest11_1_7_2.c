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
*  A convienence method to set the results for the 11.1.7.2 results
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
int  ApiCalculationTest_setResults11_1_7_2Test(ApiCalculationRequest *result,
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
* Run the test for the 11.1.7.2 calculations
*
*/
bool  ApiCalculationTest_run11_1_7_2Tests(ApiCalculationRequest * request, 
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
  *errorCode = ApiCalculation_set11_1_7_2Parameters(request,
						    API_REFINED_PRODUCTS_NAME,
						    0, NULL,
						    27.35, ApiUnit_Temperature_C(),
						    1234, ApiUnit_Pressure_KPA(),
						    657.3, ApiUnit_Density_KGM3(),

						    15.0, ApiUnit_Temperature_C(),

						    10000, ApiUnit_Volume_M3());
  if( *errorCode != 0) return FALSE;
  *errorCode = ApiCalculationTest_setResults11_1_7_2Test(resultRequest,
							 0.181496814240,
							 ApiUnit_ScaledComp_REV_KPA(),
							 0.982192367319,
							 1.002244698072,
							 0.984397092633,
							 667.718347529613,
							 ApiUnit_Density_KGM3(),
							 9844,
							 ApiUnit_Volume_M3() );

  if( *errorCode != 0) return FALSE;	
  ApiCalculationRequest_appendBuffer(request->intermediate, request->intLength, API_EOL);
  ApiCalculationRequest_appendBuffer(request->intermediate,
				     request->intLength,
				     "11.1.7.2 Example 1");
  ApiCalculationRequest_appendBuffer(request->intermediate, request->intLength, API_EOL);
  tmp  =  ApiCalculationTest_checkResult(request,
					 resultRequest,
					 FALSE,
					 "11.1.7.2 Example 1",
					 errBuf,
					 errLength,
					 errorCode);
  if( tmp == FALSE || *errorCode != 0) result = FALSE;


  /** Example 2 */
  *errorCode = ApiCalculation_set11_1_7_2Parameters(request,
						    API_CRUDE_OIL_NAME ,
						    0, NULL,
						    26.8, ApiUnit_Temperature_C(),
						    -5, ApiUnit_Pressure_KPA(),
						    823.7, ApiUnit_Density_KGM3(),

						    15.0, ApiUnit_Temperature_C(),

						    1000, ApiUnit_Volume_M3());
  if( *errorCode != 0) return FALSE;
  *errorCode = ApiCalculationTest_setResults11_1_7_2Test(resultRequest,
							 0.082232607523,
							 ApiUnit_ScaledComp_REV_KPA(),
							 0.989507855128,
							 1.0,
							 0.989507855128,
							 832.434018321149,
							 ApiUnit_Density_KGM3(),
							 989.51,
							 ApiUnit_Volume_M3() );

  if( *errorCode != 0) return FALSE;	
  ApiCalculationRequest_appendBuffer(request->intermediate, request->intLength, API_EOL);
  ApiCalculationRequest_appendBuffer(request->intermediate,
				     request->intLength,
				     "11.1.7.2 Example 2");
  ApiCalculationRequest_appendBuffer(request->intermediate, request->intLength, API_EOL);
  tmp  =  ApiCalculationTest_checkResult(request,
					 resultRequest,
					 FALSE,
					 "11.1.7.2 Example 2",
					 errBuf,
					 errLength,
					 errorCode);
  if( tmp == FALSE || *errorCode != 0) result = FALSE;

  /** Example 3 */
  *errorCode = ApiCalculation_set11_1_7_2Parameters(request,
						    API_CRUDE_OIL_NAME,
						    0, NULL,
						    -50, ApiUnit_Temperature_C(),
						    115, ApiUnit_Pressure_KPA(),
						    722.6, ApiUnit_Density_KGM3(),

						    15.0, ApiUnit_Temperature_C(),

						    145902, ApiUnit_Volume_M3());
  if( *errorCode != 0) return FALSE;
  *errorCode = ApiCalculationTest_setResults11_1_7_2Test(resultRequest,
							 0.087329935201,
							 ApiUnit_ScaledComp_REV_KPA(),
							 1.087515323806,
							 1.000100439513,
							 1.087624553315,
							 664.383677067826,
							 ApiUnit_Density_KGM3(),
							 158685.93324,
							 ApiUnit_Volume_M3() );

  if( *errorCode != 0) return FALSE;	
  ApiCalculationRequest_appendBuffer(request->intermediate, request->intLength, API_EOL);
  ApiCalculationRequest_appendBuffer(request->intermediate,
				     request->intLength,
				     "11.1.7.2 Example 3");
  ApiCalculationRequest_appendBuffer(request->intermediate, request->intLength, API_EOL);
  tmp  =  ApiCalculationTest_checkResult(request,
					 resultRequest,
					 FALSE,
					 "11.1.7.2 Example 3",
					 errBuf,
					 errLength,
					 errorCode);
  if( tmp == FALSE || *errorCode != 0) result = FALSE;

  /** Example 4 */
  *errorCode = ApiCalculation_set11_1_7_2Parameters(request,
						    API_REFINED_PRODUCTS_NAME,
						    0, NULL,
						    20, ApiUnit_Temperature_C(),
						    11.25, ApiUnit_Pressure_BAR(),
						    865.6, ApiUnit_Density_KGM3(),

						    15.0, ApiUnit_Temperature_C(),

						    99873, ApiUnit_Volume_M3());
  if( *errorCode != 0) return FALSE;
  *errorCode = ApiCalculationTest_setResults11_1_7_2Test(resultRequest,
							 7.045834505444,
							 ApiUnit_ScaledComp_REV_BAR(),
							 0.995955841168,
							 1.000793285184,
							 0.996745918181,
							 868.425929024799,
							 ApiUnit_Density_KGM3(),
							 99548.41275,
							 ApiUnit_Volume_M3() );

  if( *errorCode != 0) return FALSE;	
  ApiCalculationRequest_appendBuffer(request->intermediate, request->intLength, API_EOL);
  ApiCalculationRequest_appendBuffer(request->intermediate,
				     request->intLength,
				     "11.1.7.2 Example 4");
  ApiCalculationRequest_appendBuffer(request->intermediate, request->intLength, API_EOL);
  tmp  =  ApiCalculationTest_checkResult(request,
					 resultRequest,
					 FALSE,
					 "11.1.7.2 Example 4",
					 errBuf,
					 errLength,
					 errorCode);
  if( tmp == FALSE || *errorCode != 0) result = FALSE;

  /** Example 5 */
  *errorCode = ApiCalculation_set11_1_7_2Parameters(request,
						    API_REFINED_PRODUCTS_NAME,
						    0, NULL,
						    22.25, ApiUnit_Temperature_C(),
						    2587.3, ApiUnit_Pressure_KPA(),
						    817.59, ApiUnit_Density_KGM3(),

						    15.0, ApiUnit_Temperature_C(),

						    400.15, ApiUnit_Volume_M3());
  if( *errorCode != 0) return FALSE;
  *errorCode = ApiCalculationTest_setResults11_1_7_2Test(resultRequest,
							 0.083058649032,
							 ApiUnit_ScaledComp_REV_KPA(),
							 0.993591533933,
							 1.002153604472,
							 0.995731337103,
							 821.094977665680,
							 ApiUnit_Density_KGM3(),
							 398.4413595,
							 ApiUnit_Volume_M3() );

  if( *errorCode != 0) return FALSE;	
  ApiCalculationRequest_appendBuffer(request->intermediate, request->intLength, API_EOL);
  ApiCalculationRequest_appendBuffer(request->intermediate,
				     request->intLength,
				     "11.1.7.2 Example 5");
  ApiCalculationRequest_appendBuffer(request->intermediate, request->intLength, API_EOL);
  tmp  =  ApiCalculationTest_checkResult(request,
					 resultRequest,
					 FALSE,
					 "11.1.7.2 Example 5",
					 errBuf,
					 errLength,
					 errorCode);
  if( tmp == FALSE || *errorCode != 0) result = FALSE;

  /** Example 6 */
  *errorCode = ApiCalculation_set11_1_7_2Parameters(request,
						    API_REFINED_PRODUCTS_NAME,
						    0, NULL,
						    22.25, ApiUnit_Temperature_C(),
						    2587.3, ApiUnit_Pressure_KPA(),
						    817.59, ApiUnit_Density_KGM3(),

						    15.0, ApiUnit_Temperature_C(),

						    400.15, ApiUnit_Volume_M3());
  if( *errorCode != 0) return FALSE;
  *errorCode = ApiCalculationTest_setResults11_1_7_2Test(resultRequest,
							 0.083058649032,
							 ApiUnit_ScaledComp_REV_KPA(),
							 0.993591533933,
							 1.002153604472,
							 0.995731337103,
							 821.094977665680,
							 ApiUnit_Density_KGM3(),
							 398.4413595,
							 ApiUnit_Volume_M3() );

  if( *errorCode != 0) return FALSE;	
  ApiCalculationRequest_appendBuffer(request->intermediate, request->intLength, API_EOL);
  ApiCalculationRequest_appendBuffer(request->intermediate,
				     request->intLength,
				     "11.1.7.2 Example 6");
  ApiCalculationRequest_appendBuffer(request->intermediate, request->intLength, API_EOL);
  tmp  =  ApiCalculationTest_checkResult(request,
					 resultRequest,
					 FALSE,
					 "11.1.7.2 Example 6",
					 errBuf,
					 errLength,
					 errorCode);
  if( tmp == FALSE || *errorCode != 0) result = FALSE;

  /** Example 7 */
  *errorCode = ApiCalculation_set11_1_7_2Parameters(request,
						    API_REFINED_PRODUCTS_NAME,
						    0, NULL,
						    5.47, ApiUnit_Temperature_C(),
						    1710, ApiUnit_Pressure_KPA(),
						    779.6, ApiUnit_Density_KGM3(),

						    15.0, ApiUnit_Temperature_C(),

						    89987, ApiUnit_Volume_M3());
  if( *errorCode != 0) return FALSE;
  *errorCode = ApiCalculationTest_setResults11_1_7_2Test(resultRequest,
							 0.089405815297,
							 ApiUnit_ScaledComp_REV_KPA(),
							 1.010965045324,
							 1.001531180371,
							 1.012513015157,
							 769.965411139556,
							 ApiUnit_Density_KGM3(),
							 91112.73737,
							 ApiUnit_Volume_M3() );

  if( *errorCode != 0) return FALSE;	
  ApiCalculationRequest_appendBuffer(request->intermediate, request->intLength, API_EOL);
  ApiCalculationRequest_appendBuffer(request->intermediate,
				     request->intLength,
				     "11.1.7.2 Example 7");
  ApiCalculationRequest_appendBuffer(request->intermediate, request->intLength, API_EOL);
  tmp  =  ApiCalculationTest_checkResult(request,
					 resultRequest,
					 FALSE,
					 "11.1.7.2 Example 7",
					 errBuf,
					 errLength,
					 errorCode);
  if( tmp == FALSE || *errorCode != 0) result = FALSE;

  /** Example 8 */
  *errorCode = ApiCalculation_set11_1_7_2Parameters(request,
						    API_COMMODITY_NOT_GIVEN,
						    0.0005763, 
						    ApiUnit_ThermalExp_REV_F(),
						    29.18, ApiUnit_Temperature_C(),
						    395, ApiUnit_Pressure_KPA(),
						    853.7, ApiUnit_Density_KGM3(),

						    15.0, ApiUnit_Temperature_C(),

						    8501.3, ApiUnit_Volume_M3());
  if( *errorCode != 0) return FALSE;
  *errorCode = ApiCalculationTest_setResults11_1_7_2Test(resultRequest,
							 0.074832551126,
							 ApiUnit_ScaledComp_REV_KPA(),
							 0.985237057692,
							 1.000295675975,
							 0.985528368620,
							 866.235845849433,
							 ApiUnit_Density_KGM3(),
							 8378.286189,
							 ApiUnit_Volume_M3() );

  if( *errorCode != 0) return FALSE;	
  ApiCalculationRequest_appendBuffer(request->intermediate, request->intLength, API_EOL);
  ApiCalculationRequest_appendBuffer(request->intermediate,
				     request->intLength,
				     "11.1.7.2 Example 8");
  ApiCalculationRequest_appendBuffer(request->intermediate, request->intLength, API_EOL);
  tmp  =  ApiCalculationTest_checkResult(request,
					 resultRequest,
					 FALSE,
					 "11.1.7.2 Example 8",
					 errBuf,
					 errLength,
					 errorCode);
  if( tmp == FALSE || *errorCode != 0) result = FALSE;

  /** Example 9 */
  *errorCode = ApiCalculation_set11_1_7_2Parameters(request,
						    API_LUBRICATING_OIL_NAME,
						    0, NULL, 
						    -49.97, ApiUnit_Temperature_C(),
						    31.1, ApiUnit_Pressure_BAR(),
						    1204.6, ApiUnit_Density_KGM3(),

						    15.0, ApiUnit_Temperature_C(),

						    100, ApiUnit_Volume_M3());
  if( *errorCode != 0) return FALSE;
  *errorCode = ApiCalculationTest_setResults11_1_7_2Test(resultRequest,
							 3.188159804848,
							 ApiUnit_ScaledComp_REV_BAR(),
							 1.034672971366,
							 1.000992501782,
							 1.035699886134,
							 1163.078239292499,
							 ApiUnit_Density_KGM3(),
							 103.57,
							 ApiUnit_Volume_M3() );

  if( *errorCode != 0) return FALSE;	
  ApiCalculationRequest_appendBuffer(request->intermediate, request->intLength, API_EOL);
  ApiCalculationRequest_appendBuffer(request->intermediate,
				     request->intLength,
				     "11.1.7.2 Example 9");
  ApiCalculationRequest_appendBuffer(request->intermediate, request->intLength, API_EOL);
  tmp  =  ApiCalculationTest_checkResult(request,
					 resultRequest,
					 FALSE,
					 "11.1.7.2 Example 9",
					 errBuf,
					 errLength,
					 errorCode);
  if( tmp == FALSE || *errorCode != 0) result = FALSE;


  /** Example 10 */
  *errorCode = ApiCalculation_set11_1_7_2Parameters(request,
						    API_LUBRICATING_OIL_NAME,
						    0, NULL, 
						    150, ApiUnit_Temperature_C(),
						    100, ApiUnit_Pressure_KPA(),
						    734.3, ApiUnit_Density_KGM3(),

						    15.0, ApiUnit_Temperature_C(),

						    987.37, ApiUnit_Volume_M3());
  if( *errorCode != 0) return FALSE;
  *errorCode = ApiCalculationTest_setResults11_1_7_2Test(resultRequest,
							 0.189627098980,
							 ApiUnit_ScaledComp_REV_KPA(),
							 0.894250233938,
							 1.000189663064,
							 0.894419840177,
							 820.979105094945,
							 ApiUnit_Density_KGM3(),
							 883.1234754,
							 ApiUnit_Volume_M3() );

  if( *errorCode != 0) return FALSE;	
  ApiCalculationRequest_appendBuffer(request->intermediate, request->intLength, API_EOL);
  ApiCalculationRequest_appendBuffer(request->intermediate,
				     request->intLength,
				     "11.1.7.2 Example 10");
  ApiCalculationRequest_appendBuffer(request->intermediate, request->intLength, API_EOL);
  tmp  =  ApiCalculationTest_checkResult(request,
					 resultRequest,
					 FALSE,
					 "11.1.7.2 Example 10",
					 errBuf,
					 errLength,
					 errorCode);
  if( tmp == FALSE || *errorCode != 0) result = FALSE;

  /** Example 11 */
  *errorCode = ApiCalculation_set11_1_7_2Parameters(request,
						    API_CRUDE_OIL_NAME,
						    0, NULL, 
						    -50.05, ApiUnit_Temperature_C(),
						    0, ApiUnit_Pressure_KPA(),
						    470.27, ApiUnit_Density_KGM3(),

						    15.0, ApiUnit_Temperature_C(),

						    1000, ApiUnit_Volume_M3());
  if( *errorCode != 0) return FALSE;
  ApiCalculationRequest_appendBuffer(request->intermediate, request->intLength, API_EOL);
  ApiCalculationRequest_appendBuffer(request->intermediate,
				     request->intLength,
				     "11.1.7.2 Example 11");
  ApiCalculationRequest_appendBuffer(request->intermediate, request->intLength, API_EOL);
  tmp  =  ApiCalculationTest_checkResult(request,
					 resultRequest,
					 TRUE,
					 "11.1.7.2 Example 11",
					 errBuf,
					 errLength,
					 errorCode);
  if( tmp == FALSE || *errorCode != 0) result = FALSE;


  /** Example 12 */
  *errorCode = ApiCalculation_set11_1_7_2Parameters(request,
						    API_CRUDE_OIL_NAME,
						    0, NULL, 
						    150.03, ApiUnit_Temperature_C(),
						    103.425, ApiUnit_Pressure_KPA(),
						    1201.85, ApiUnit_Density_KGM3(),

						    15.0, ApiUnit_Temperature_C(),

						    1000, ApiUnit_Volume_M3());
  if( *errorCode != 0) return FALSE;
  ApiCalculationRequest_appendBuffer(request->intermediate, request->intLength, API_EOL);
  ApiCalculationRequest_appendBuffer(request->intermediate,
				     request->intLength,
				     "11.1.7.2 Example 12");
  ApiCalculationRequest_appendBuffer(request->intermediate, request->intLength, API_EOL);
  tmp  =  ApiCalculationTest_checkResult(request,
					 resultRequest,
					 TRUE,
					 "11.1.7.2 Example 12",
					 errBuf,
					 errLength,
					 errorCode);
  if( tmp == FALSE || *errorCode != 0) result = FALSE;


  /** Example 13 */
  *errorCode = ApiCalculation_set11_1_7_2Parameters(request,
						    API_REFINED_PRODUCTS_NAME,
						    0, NULL, 
						    -50.05, ApiUnit_Temperature_C(),
						    0, ApiUnit_Pressure_KPA(),
						    470.17, ApiUnit_Density_KGM3(),

						    15.0, ApiUnit_Temperature_C(),

						    1000, ApiUnit_Volume_M3());
  if( *errorCode != 0) return FALSE;
  ApiCalculationRequest_appendBuffer(request->intermediate, request->intLength, API_EOL);
  ApiCalculationRequest_appendBuffer(request->intermediate,
				     request->intLength,
				     "11.1.7.2 Example 13");
  ApiCalculationRequest_appendBuffer(request->intermediate, request->intLength, API_EOL);
  tmp  =  ApiCalculationTest_checkResult(request,
					 resultRequest,
					 TRUE,
					 "11.1.7.2 Example 13",
					 errBuf,
					 errLength,
					 errorCode);
  if( tmp == FALSE || *errorCode != 0) result = FALSE;

  /** Example 14 */
  *errorCode = ApiCalculation_set11_1_7_2Parameters(request,
						    API_REFINED_PRODUCTS_NAME,
						    0, NULL, 
						    150.02, ApiUnit_Temperature_C(),
						    10342.5, ApiUnit_Pressure_KPA(),
						    1209.56, ApiUnit_Density_KGM3(),

						    15.0, ApiUnit_Temperature_C(),

						    1000, ApiUnit_Volume_M3());
  if( *errorCode != 0) return FALSE;
  ApiCalculationRequest_appendBuffer(request->intermediate, request->intLength, API_EOL);
  ApiCalculationRequest_appendBuffer(request->intermediate,
				     request->intLength,
				     "11.1.7.2 Example 14");
  ApiCalculationRequest_appendBuffer(request->intermediate, request->intLength, API_EOL);
  tmp  =  ApiCalculationTest_checkResult(request,
					 resultRequest,
					 TRUE,
					 "11.1.7.2 Example 14",
					 errBuf,
					 errLength,
					 errorCode);
  if( tmp == FALSE || *errorCode != 0) result = FALSE;

  /** Example 15 */
  *errorCode = ApiCalculation_set11_1_7_2Parameters(request,
						    API_LUBRICATING_OIL_NAME,
						    0, NULL, 
						    15.56, ApiUnit_Temperature_C(),
						    10342.5, ApiUnit_Pressure_KPA(),
						    1208.52, ApiUnit_Density_KGM3(),

						    15.0, ApiUnit_Temperature_C(),

						    1000, ApiUnit_Volume_M3());
  if( *errorCode != 0) return FALSE;
  ApiCalculationRequest_appendBuffer(request->intermediate, request->intLength, API_EOL);
  ApiCalculationRequest_appendBuffer(request->intermediate,
				     request->intLength,
				     "11.1.7.2 Example 15");
  ApiCalculationRequest_appendBuffer(request->intermediate, request->intLength, API_EOL);
  tmp  =  ApiCalculationTest_checkResult(request,
					 resultRequest,
					 TRUE,
					 "11.1.7.2 Example 15",
					 errBuf,
					 errLength,
					 errorCode);
  if( tmp == FALSE || *errorCode != 0) result = FALSE;


  /** Example 16 */
  *errorCode = ApiCalculation_set11_1_7_2Parameters(request,
						    API_LUBRICATING_OIL_NAME,
						    0, NULL, 
						    22.42, ApiUnit_Temperature_C(),
						    0, ApiUnit_Pressure_KPA(),
						    714.245, ApiUnit_Density_KGM3(),

						    15.0, ApiUnit_Temperature_C(),

						    1000, ApiUnit_Volume_M3());
  if( *errorCode != 0) return FALSE;
  ApiCalculationRequest_appendBuffer(request->intermediate, request->intLength, API_EOL);
  ApiCalculationRequest_appendBuffer(request->intermediate,
				     request->intLength,
				     "11.1.7.2 Example 16");
  ApiCalculationRequest_appendBuffer(request->intermediate, request->intLength, API_EOL);
  tmp  =  ApiCalculationTest_checkResult(request,
					 resultRequest,
					 TRUE,
					 "11.1.7.2 Example 16",
					 errBuf,
					 errLength,
					 errorCode);
  if( tmp == FALSE || *errorCode != 0) result = FALSE;


  /** Example 17 */
  *errorCode = ApiCalculation_set11_1_7_2Parameters(request,
						    API_LUBRICATING_OIL_NAME,
						    0, NULL, 
						    0.42, ApiUnit_Temperature_C(),
						    0, ApiUnit_Pressure_KPA(),
						    723.45, ApiUnit_Density_KGM3(),

						    15.0, ApiUnit_Temperature_C(),

						    1000, ApiUnit_Volume_M3());
  if( *errorCode != 0) return FALSE;
  ApiCalculationRequest_appendBuffer(request->intermediate, request->intLength, API_EOL);
  ApiCalculationRequest_appendBuffer(request->intermediate,
				     request->intLength,
				     "11.1.7.2 Example 17");
  ApiCalculationRequest_appendBuffer(request->intermediate, request->intLength, API_EOL);
  tmp  =  ApiCalculationTest_checkResult(request,
					 resultRequest,
					 TRUE,
					 "11.1.7.2 Example 17",
					 errBuf,
					 errLength,
					 errorCode);
  if( tmp == FALSE || *errorCode != 0) result = FALSE;


  /** Clean up the result object */
  ApiCalculationRequest_cleanUp(resultRequest);
  resultRequest = NULL;


  return result;
}


