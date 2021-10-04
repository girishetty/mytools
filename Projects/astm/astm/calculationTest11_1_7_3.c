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
* Run the test for the 11.1.7.3 calculations
*
*/
bool  ApiCalculationTest_run11_1_7_3Tests(ApiCalculationRequest * request, 
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
  resultRequest->scaledCompAlt = ApiScaledFactor_init(1, 
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


  resultRequest->baseDens = ApiDensity_init(1,  ApiUnit_Density_KGM3(), TRUE, errorCode);
  if( *errorCode != 0) return FALSE;
  resultRequest->altDens = ApiDensity_init(1,  ApiUnit_Density_KGM3(), TRUE, errorCode);
  if( *errorCode != 0) return FALSE;


  resultRequest->baseVol = ApiVolume_init(1, ApiUnit_Volume_M3(), TRUE, errorCode);
  if( *errorCode != 0) return FALSE;
  resultRequest->altVol = ApiVolume_init(1, ApiUnit_Volume_M3(), TRUE, errorCode);
  if( *errorCode != 0) return FALSE;


  /** Example 1 */
  *errorCode =  ApiCalculation_setParameters(request,
					     API_CRUDE_OIL_NAME,
					     0, NULL,
					     26.82, ApiUnit_Temperature_C(),
					     90.0, ApiUnit_Temperature_C(),
					     -5, ApiUnit_Pressure_KPA(),
					     0, ApiUnit_Pressure_KPA(),
					     823.687, ApiUnit_Density_KGM3(),

					     15, ApiUnit_Temperature_C(),
					     0, ApiUnit_Pressure_KPA(),

					     0, NULL,
					     0, NULL,
					     1247.65, ApiUnit_Volume_M3());
  if( *errorCode != 0) return FALSE;
  *errorCode = ApiCalculationTest_setResult(resultRequest,
					    832.435846991832, ApiUnit_Density_KGM3(),
					    0.989490064587,
					    0.082242442766,
					    ApiUnit_ScaledComp_REV_KPA(),
					    1.0,
					    0.989490064587,
					    1234.5371985, ApiUnit_Volume_M3(),

					    0.932371562866,
					    0.122382642306,
					    ApiUnit_ScaledComp_REV_KPA(),
					    1.0,
					    0.932371562866,
					    776.139511645102, ApiUnit_Density_KGM3(),
					    1324.085071914, ApiUnit_Volume_M3(),

					    0, NULL);
  if( *errorCode != 0) return FALSE;	
  ApiCalculationRequest_appendBuffer(request->intermediate, request->intLength, API_EOL);
  ApiCalculationRequest_appendBuffer(request->intermediate,
				     request->intLength,
				     "11.1.7.3 Example 1");
  ApiCalculationRequest_appendBuffer(request->intermediate, request->intLength, API_EOL);
  tmp  =  ApiCalculationTest_checkResult(request,
					 resultRequest,
					 FALSE,
					 "11.1.7.3 Example 1",
					 errBuf,
					 errLength,
					 errorCode);
  if( tmp == FALSE || *errorCode != 0) result = FALSE;



  /** Example 2 */
  *errorCode =  ApiCalculation_setParameters(request,
					     API_CRUDE_OIL_NAME,
					     0, NULL,
					     -40, ApiUnit_Temperature_C(),
					     90.0, ApiUnit_Temperature_C(),
					     2.15, ApiUnit_Pressure_BAR(),
					     24.35, ApiUnit_Pressure_BAR(),
					     758.7, ApiUnit_Density_KGM3(),

					     15, ApiUnit_Temperature_C(),
					     0, ApiUnit_Pressure_KPA(),

					     0, NULL,
					     0, NULL,
					     1000, ApiUnit_Volume_M3());
  if( *errorCode != 0) return FALSE;
  *errorCode = ApiCalculationTest_setResult(resultRequest,
					    712.223958926801, ApiUnit_Density_KGM3(),
					    1.065075510252,
					    7.828785016773,
					    ApiUnit_ScaledComp_REV_BAR(),
					    1.000168347214,
					    1.065254812747,
					    1065.25, ApiUnit_Volume_M3(),

					    0.907158012893,
					    23.662962293959,
					    ApiUnit_ScaledComp_REV_BAR(),
					    1.005795323575,
					    0.912415287111,
					    649.844027971673, ApiUnit_Density_KGM3(),
					    1167.49961640, ApiUnit_Volume_M3(),

					    0, NULL);
  if( *errorCode != 0) return FALSE;	
  ApiCalculationRequest_appendBuffer(request->intermediate, request->intLength, API_EOL);
  ApiCalculationRequest_appendBuffer(request->intermediate,
				     request->intLength,
				     "11.1.7.3 Example 2");
  ApiCalculationRequest_appendBuffer(request->intermediate, request->intLength, API_EOL);
  tmp  =  ApiCalculationTest_checkResult(request,
					 resultRequest,
					 FALSE,
					 "11.1.7.3 Example 2",
					 errBuf,
					 errLength,
					 errorCode);
  if( tmp == FALSE || *errorCode != 0) result = FALSE;


  /** Example 3 */
  *errorCode =  ApiCalculation_setParameters(request,
					     API_CRUDE_OIL_NAME,
					     0, NULL,
					     26.82, ApiUnit_Temperature_C(),
					     68.45, ApiUnit_Temperature_C(),
					     -5, ApiUnit_Pressure_BAR(),
					     0, ApiUnit_Pressure_BAR(),
					     823.687, ApiUnit_Density_KGM3(),

					     20, ApiUnit_Temperature_C(),
					     0, ApiUnit_Pressure_KPA(),

					     0, NULL,
					     0, NULL,
					     243.85, ApiUnit_Volume_M3());
  if( *errorCode != 0) return FALSE;
  *errorCode = ApiCalculationTest_setResult(resultRequest,
					    828.741499381895, ApiUnit_Density_KGM3(),
					    0.993900994000,
					    0.082242442766,
					    ApiUnit_ScaledComp_REV_KPA(),
					    1.0,
					    0.993900994000,
					    242.362515, ApiUnit_Volume_M3(),

					    0.956254897955,
					    0.106866171700,
					    ApiUnit_ScaledComp_REV_KPA(),
					    1.0,
					    0.956254897955,
					    792.488117922186, ApiUnit_Density_KGM3(),
					    253.450996078, ApiUnit_Volume_M3(),

					    0, NULL);
  if( *errorCode != 0) return FALSE;	
  ApiCalculationRequest_appendBuffer(request->intermediate, request->intLength, API_EOL);
  ApiCalculationRequest_appendBuffer(request->intermediate,
				     request->intLength,
				     "11.1.7.3 Example 3");
  ApiCalculationRequest_appendBuffer(request->intermediate, request->intLength, API_EOL);
  tmp  =  ApiCalculationTest_checkResult(request,
					 resultRequest,
					 FALSE,
					 "11.1.7.3 Example 3",
					 errBuf,
					 errLength,
					 errorCode);
  if( tmp == FALSE || *errorCode != 0) result = FALSE;


  /** Example 4 */
  *errorCode =  ApiCalculation_setParameters(request,
					     API_REFINED_PRODUCTS_NAME,
					     0, NULL,
					     22.25, ApiUnit_Temperature_C(),
					     102.35, ApiUnit_Temperature_C(),
					     2585, ApiUnit_Pressure_KPA(),
					     3505, ApiUnit_Pressure_KPA(),
					     817.55, ApiUnit_Density_KGM3(),

					     15, ApiUnit_Temperature_C(),
					     0, ApiUnit_Pressure_KPA(),

					     0, NULL,
					     0, NULL,
					     9987.5, ApiUnit_Volume_M3());
  if( *errorCode != 0) return FALSE;
  *errorCode = ApiCalculationTest_setResult(resultRequest,
					    821.056637883991, ApiUnit_Density_KGM3(),
					    0.993590934183,
					    0.083069734480,
					    ApiUnit_ScaledComp_REV_KPA(),
					    1.002151973683,
					    0.995729115724,
					    9944.853375, ApiUnit_Volume_M3(),

					    0.921427460327,
					    0.139369604520,
					    ApiUnit_ScaledComp_REV_KPA(),
					    1.004908884069,
					    0.925950640907,
					    760.257920070010, ApiUnit_Density_KGM3(),
					    10740.16240078, ApiUnit_Volume_M3(),

					    0, NULL);
  if( *errorCode != 0) return FALSE;	
  ApiCalculationRequest_appendBuffer(request->intermediate, request->intLength, API_EOL);
  ApiCalculationRequest_appendBuffer(request->intermediate,
				     request->intLength,
				     "11.1.7.3 Example 4");
  ApiCalculationRequest_appendBuffer(request->intermediate, request->intLength, API_EOL);
  tmp  =  ApiCalculationTest_checkResult(request,
					 resultRequest,
					 FALSE,
					 "11.1.7.3 Example 4",
					 errBuf,
					 errLength,
					 errorCode);
  if( tmp == FALSE || *errorCode != 0) result = FALSE;


  /** Example 5 */
  *errorCode =  ApiCalculation_setParameters(request,
					     API_REFINED_PRODUCTS_NAME,
					     0, NULL,
					     1.85, ApiUnit_Temperature_C(),
					     45.95, ApiUnit_Temperature_C(),
					     1835, ApiUnit_Pressure_KPA(),
					     10342, ApiUnit_Pressure_KPA(),
					     799, ApiUnit_Density_KGM3(),

					     20, ApiUnit_Temperature_C(),
					     0, ApiUnit_Pressure_KPA(),

					     0, NULL,
					     0, NULL,
					     15.85, ApiUnit_Volume_M3());
  if( *errorCode != 0) return FALSE;
  *errorCode = ApiCalculationTest_setResult(resultRequest,
					    784.134681415189, ApiUnit_Density_KGM3(),
					    1.017435032590,
					    0.081430476547,
					    ApiUnit_ScaledComp_REV_KPA(),
					    1.001496485367,
					    1.018957609228,
					    16.150516, ApiUnit_Volume_M3(),

					    0.974768180331,
					    0.110854271298,
					    ApiUnit_ScaledComp_REV_KPA(),
					    1.011597508944,
					    0.986073063021,
					    773.214087124006, ApiUnit_Density_KGM3(),
					    16.378670885, ApiUnit_Volume_M3(),

					    0, NULL);
  if( *errorCode != 0) return FALSE;	
  ApiCalculationRequest_appendBuffer(request->intermediate, request->intLength, API_EOL);
  ApiCalculationRequest_appendBuffer(request->intermediate,
				     request->intLength,
				     "11.1.7.3 Example 5");
  ApiCalculationRequest_appendBuffer(request->intermediate, request->intLength, API_EOL);
  tmp  =  ApiCalculationTest_checkResult(request,
					 resultRequest,
					 FALSE,
					 "11.1.7.3 Example 5",
					 errBuf,
					 errLength,
					 errorCode);
  if( tmp == FALSE || *errorCode != 0) result = FALSE;


  /** Example 6 */
  *errorCode =  ApiCalculation_setParameters(request,
					     API_REFINED_PRODUCTS_NAME,
					     0, NULL,
					     5.45, ApiUnit_Temperature_C(),
					     -44.95, ApiUnit_Temperature_C(),
					     1708, ApiUnit_Pressure_KPA(),
					     348, ApiUnit_Pressure_KPA(),
					     779.6, ApiUnit_Density_KGM3(),

					     15, ApiUnit_Temperature_C(),
					     0, ApiUnit_Pressure_KPA(),

					     0, NULL,
					     0, NULL,
					     201.5, ApiUnit_Volume_M3());
  if( *errorCode != 0) return FALSE;
  *errorCode = ApiCalculationTest_setResult(resultRequest,
					    769.949157909619, ApiUnit_Density_KGM3(),
					    1.010988324649,
					    0.089398422944,
					    ApiUnit_ScaledComp_REV_KPA(),
					    1.001529260130,
					    1.012534388785,
					    204.024795, ApiUnit_Volume_M3(),

					    1.067555939919,
					    0.061833401901,
					    ApiUnit_ScaledComp_REV_KPA(),
					    1.000215226551,
					    1.067785706302,
					    822.140705395444, ApiUnit_Density_KGM3(),
					    191.072022589, ApiUnit_Volume_M3(),

					    0, NULL);
  if( *errorCode != 0) return FALSE;	
  ApiCalculationRequest_appendBuffer(request->intermediate, request->intLength, API_EOL);
  ApiCalculationRequest_appendBuffer(request->intermediate,
				     request->intLength,
				     "11.1.7.3 Example 6");
  ApiCalculationRequest_appendBuffer(request->intermediate, request->intLength, API_EOL);
  tmp  =  ApiCalculationTest_checkResult(request,
					 resultRequest,
					 FALSE,
					 "11.1.7.3 Example 6",
					 errBuf,
					 errLength,
					 errorCode);
  if( tmp == FALSE || *errorCode != 0) result = FALSE;


  /** Example 7 */
  *errorCode =  ApiCalculation_setParameters(request,
					     API_COMMODITY_NOT_GIVEN,
					     0.00057634, ApiUnit_ThermalExp_REV_F(),
					     29.2, ApiUnit_Temperature_C(),
					     55.05, ApiUnit_Temperature_C(),
					     395, ApiUnit_Pressure_KPA(),
					     6505, ApiUnit_Pressure_KPA(),
					     853.7, ApiUnit_Density_KGM3(),

					     15, ApiUnit_Temperature_C(),
					     0, ApiUnit_Pressure_KPA(),

					     0, NULL,
					     0, NULL,
					     1000, ApiUnit_Volume_M3());
  if( *errorCode != 0) return FALSE;
  *errorCode = ApiCalculationTest_setResult(resultRequest,
					    866.255119751516, ApiUnit_Density_KGM3(),
					    0.985215119619,
					    0.074836882253,
					    ApiUnit_ScaledComp_REV_KPA(),
					    1.000295694093,
					    0.985506440926,
					    985.51, ApiUnit_Volume_M3(),

					    0.958004304551,
					    0.087007330735,
					    ApiUnit_ScaledComp_REV_KPA(),
					    1.005692042841,
					    0.963457306095,
					    834.599824066622, ApiUnit_Density_KGM3(),
					    1022.886264090, ApiUnit_Volume_M3(),

					    0, NULL);
  if( *errorCode != 0) return FALSE;	
  ApiCalculationRequest_appendBuffer(request->intermediate, request->intLength, API_EOL);
  ApiCalculationRequest_appendBuffer(request->intermediate,
				     request->intLength,
				     "11.1.7.3 Example 7");
  ApiCalculationRequest_appendBuffer(request->intermediate, request->intLength, API_EOL);
  tmp  =  ApiCalculationTest_checkResult(request,
					 resultRequest,
					 FALSE,
					 "11.1.7.3 Example 7",
					 errBuf,
					 errLength,
					 errorCode);
  if( tmp == FALSE || *errorCode != 0) result = FALSE;

  /** Example 8 */
  *errorCode =  ApiCalculation_setParameters(request,
					     API_LUBRICATING_OIL_NAME,
					     0.0, NULL,
					     -49.95, ApiUnit_Temperature_C(),
					     67.65, ApiUnit_Temperature_C(),
					     31.1, ApiUnit_Pressure_BAR(),
					     13.65, ApiUnit_Pressure_BAR(),
					     1204.65, ApiUnit_Density_KGM3(),

					     15, ApiUnit_Temperature_C(),
					     0, ApiUnit_Pressure_KPA(),

					     0, NULL,
					     0, NULL,
					     200.04, ApiUnit_Volume_M3());
  if( *errorCode != 0) return FALSE;
  *errorCode = ApiCalculationTest_setResult(resultRequest,
					    1163.140406745777, ApiUnit_Density_KGM3(),
					    1.034660599640,
					    3.188206499661,
					    ApiUnit_ScaledComp_REV_BAR(),
					    1.000992516333,
					    1.035687517185,
					    207.1794276, ApiUnit_Volume_M3(),

					    0.971351374304,
					    4.721492010499,
					    ApiUnit_ScaledComp_REV_BAR(),
					    1.000644899286,
					    0.971977798112,
					    1.13054665144e+3, ApiUnit_Density_KGM3(),
					    213.151945102, ApiUnit_Volume_M3(),

					    0, NULL);
  if( *errorCode != 0) return FALSE;	
  ApiCalculationRequest_appendBuffer(request->intermediate, request->intLength, API_EOL);
  ApiCalculationRequest_appendBuffer(request->intermediate,
				     request->intLength,
				     "11.1.7.3 Example 8");
  ApiCalculationRequest_appendBuffer(request->intermediate, request->intLength, API_EOL);
  tmp  =  ApiCalculationTest_checkResult(request,
					 resultRequest,
					 FALSE,
					 "11.1.7.3 Example 8",
					 errBuf,
					 errLength,
					 errorCode);
  if( tmp == FALSE || *errorCode != 0) result = FALSE;

  /** Example 9 */
  *errorCode =  ApiCalculation_setParameters(request,
					     API_LUBRICATING_OIL_NAME,
					     0.0, NULL,
					     35.8, ApiUnit_Temperature_C(),
					     150, ApiUnit_Temperature_C(),
					     140, ApiUnit_Pressure_KPA(),
					     10340, ApiUnit_Pressure_KPA(),
					     734.29, ApiUnit_Density_KGM3(),

					     20, ApiUnit_Temperature_C(),
					     0, ApiUnit_Pressure_KPA(),

					     0, NULL,
					     0, NULL,
					     12501, ApiUnit_Volume_M3());
  if( *errorCode != 0) return FALSE;
  ApiCalculationRequest_appendBuffer(request->intermediate, request->intLength, API_EOL);
  ApiCalculationRequest_appendBuffer(request->intermediate,
				     request->intLength,
				     "11.1.7.3 Example 9");
  ApiCalculationRequest_appendBuffer(request->intermediate, request->intLength, API_EOL);
  tmp  =  ApiCalculationTest_checkResult(request,
					 resultRequest,
					 TRUE,
					 "11.1.7.3 Example 9",
					 errBuf,
					 errLength,
					 errorCode);
  if( tmp == FALSE || *errorCode != 0) result = FALSE;


  /** Example 10 */
  *errorCode =  ApiCalculation_setParameters(request,
					     API_CRUDE_OIL_NAME,
					     0.0, NULL,
					     -50.05, ApiUnit_Temperature_C(),
					     -50.05, ApiUnit_Temperature_C(),
					     0, ApiUnit_Pressure_KPA(),
					     0, ApiUnit_Pressure_KPA(),
					     470.27, ApiUnit_Density_KGM3(),

					     15, ApiUnit_Temperature_C(),
					     0, ApiUnit_Pressure_KPA(),

					     0, NULL,
					     0, NULL,
					     1000, ApiUnit_Volume_M3());
  if( *errorCode != 0) return FALSE;
  ApiCalculationRequest_appendBuffer(request->intermediate, request->intLength, API_EOL);
  ApiCalculationRequest_appendBuffer(request->intermediate,
				     request->intLength,
				     "11.1.7.3 Example 10");
  ApiCalculationRequest_appendBuffer(request->intermediate, request->intLength, API_EOL);
  tmp  =  ApiCalculationTest_checkResult(request,
					 resultRequest,
					 TRUE,
					 "11.1.7.3 Example 10",
					 errBuf,
					 errLength,
					 errorCode);
  if( tmp == FALSE || *errorCode != 0) result = FALSE;


  /** Example 11 */
  *errorCode =  ApiCalculation_setParameters(request,
					     API_CRUDE_OIL_NAME,
					     0.0, NULL,
					     150.03, ApiUnit_Temperature_C(),
					     150.03, ApiUnit_Temperature_C(),
					     103.425, ApiUnit_Pressure_BAR(),
					     103.425, ApiUnit_Pressure_BAR(),
					     1201.85, ApiUnit_Density_KGM3(),

					     15, ApiUnit_Temperature_C(),
					     0, ApiUnit_Pressure_KPA(),

					     0, NULL,
					     0, NULL,
					     1000, ApiUnit_Volume_M3());
  if( *errorCode != 0) return FALSE;
  ApiCalculationRequest_appendBuffer(request->intermediate, request->intLength, API_EOL);
  ApiCalculationRequest_appendBuffer(request->intermediate,
				     request->intLength,
				     "11.1.7.3 Example 11");
  ApiCalculationRequest_appendBuffer(request->intermediate, request->intLength, API_EOL);
  tmp  =  ApiCalculationTest_checkResult(request,
					 resultRequest,
					 TRUE,
					 "11.1.7.3 Example 11",
					 errBuf,
					 errLength,
					 errorCode);
  if( tmp == FALSE || *errorCode != 0) result = FALSE;

  /** Example 12 */
  *errorCode =  ApiCalculation_setParameters(request,
					     API_REFINED_PRODUCTS_NAME,
					     0.0, NULL,
					     150.02, ApiUnit_Temperature_C(),
					     150.02, ApiUnit_Temperature_C(),
					     10342.5, ApiUnit_Pressure_KPA(),
					     10342.5, ApiUnit_Pressure_KPA(),
					     1209.56, ApiUnit_Density_KGM3(),

					     15, ApiUnit_Temperature_C(),
					     0, ApiUnit_Pressure_KPA(),

					     0, NULL,
					     0, NULL,
					     1000, ApiUnit_Volume_M3());
  if( *errorCode != 0) return FALSE;
  ApiCalculationRequest_appendBuffer(request->intermediate, request->intLength, API_EOL);
  ApiCalculationRequest_appendBuffer(request->intermediate,
				     request->intLength,
				     "11.1.7.3 Example 12");
  ApiCalculationRequest_appendBuffer(request->intermediate, request->intLength, API_EOL);
  tmp  =  ApiCalculationTest_checkResult(request,
					 resultRequest,
					 TRUE,
					 "11.1.7.3 Example 12",
					 errBuf,
					 errLength,
					 errorCode);
  if( tmp == FALSE || *errorCode != 0) result = FALSE;


  /** Example 13 */
  *errorCode =  ApiCalculation_setParameters(request,
					     API_LUBRICATING_OIL_NAME,
					     0.0, NULL,
					     25.56, ApiUnit_Temperature_C(),
					     36.85, ApiUnit_Temperature_C(),
					     10342.5, ApiUnit_Pressure_KPA(),
					     0, ApiUnit_Pressure_KPA(),
					     1209.56, ApiUnit_Density_KGM3(),

					     15, ApiUnit_Temperature_C(),
					     0, ApiUnit_Pressure_KPA(),

					     0, NULL,
					     0, NULL,
					     1000, ApiUnit_Volume_M3());
  if( *errorCode != 0) return FALSE;
  ApiCalculationRequest_appendBuffer(request->intermediate, request->intLength, API_EOL);
  ApiCalculationRequest_appendBuffer(request->intermediate,
				     request->intLength,
				     "11.1.7.3 Example 13");
  ApiCalculationRequest_appendBuffer(request->intermediate, request->intLength, API_EOL);
  tmp  =  ApiCalculationTest_checkResult(request,
					 resultRequest,
					 TRUE,
					 "11.1.7.3 Example 13",
					 errBuf,
					 errLength,
					 errorCode);
  if( tmp == FALSE || *errorCode != 0) result = FALSE;


  /** Clean up the result object */
  ApiCalculationRequest_cleanUp(resultRequest);
  resultRequest = NULL;


  return result;
}


