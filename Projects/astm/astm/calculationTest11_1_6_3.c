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
* Run the test for the 11.1.6.3 calculations
*
*/
bool  ApiCalculationTest_run11_1_6_3Tests(ApiCalculationRequest * request, 
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
  *errorCode = ApiCalculation_set11_1_6_3Parameters(request,
						    API_CRUDE_OIL_NAME,
						    0, NULL,
						    80.25, ApiUnit_Temperature_F(),
						    -5.0, ApiUnit_Pressure_PSI(),
						    823.7, ApiUnit_Density_KGM3(),
						    -57.9,ApiUnit_Temperature_F(),
						    0, ApiUnit_Pressure_PSI(),
						    1000, ApiUnit_Volume_M3());
  if( *errorCode != 0) return FALSE;
  *errorCode = ApiCalculationTest_setResult(resultRequest,
					    832.028099977475, ApiUnit_Density_KGM3(),
					    0.989990602513,
					    0.566985780172,
					    ApiUnit_ScaledComp_REV_PSI(),
					    1.0,
					    0.989990602513,
					    989.99, ApiUnit_Volume_M3(),

					    1.056966235844,
					    0.349850873727,
					    ApiUnit_ScaledComp_REV_PSI(),
					    1.0,
					    1.056966235844,
					    879.425608949795, ApiUnit_Density_KGM3(),
					    936.630178718, ApiUnit_Volume_M3(),

					    0, NULL);
  if( *errorCode != 0) return FALSE;	
  ApiCalculationRequest_appendBuffer(request->intermediate, request->intLength, API_EOL);
  ApiCalculationRequest_appendBuffer(request->intermediate,
				     request->intLength,
				     "11.1.6.3 Example 1");
  ApiCalculationRequest_appendBuffer(request->intermediate, request->intLength, API_EOL);
  tmp  =  ApiCalculationTest_checkResult(request,
					 resultRequest,
					 FALSE,
					 "11.1.6.3 Example 1",
					 errBuf,
					 errLength,
					 errorCode);
  if( tmp == FALSE || *errorCode != 0) result = FALSE;


  /** Example 2 */
  *errorCode = ApiCalculation_set11_1_6_3Parameters(request,
						    API_CRUDE_OIL_NAME,
						    0, NULL,
						    -57.95, ApiUnit_Temperature_F(),
						    113.5, ApiUnit_Pressure_PSI(),
						    0.7233, ApiUnit_Density_RELATIVE(),
						    301.95,ApiUnit_Temperature_F(),
						    1342.0, ApiUnit_Pressure_PSI(),
						    9988.7, ApiUnit_Volume_M3());
  if( *errorCode != 0) return FALSE;
  *errorCode = ApiCalculationTest_setResult(resultRequest,
					    663.423272118035, ApiUnit_Density_KGM3(),
					    1.088435322387,
					    0.603495901031,
					    ApiUnit_ScaledComp_REV_PSI(),
					    1.000685437350,
					    1.089181376610,
					    10879.492266, ApiUnit_Volume_M3(),

					    0.805983918837,
					    4.244916846250,
					    ApiUnit_ScaledComp_REV_PSI(),
					    1.060408035597,
					    0.854671824096,
					    567.009178129011, ApiUnit_Density_KGM3(),
					    12729.4654849, ApiUnit_Volume_M3(),

					    0, NULL);
  if( *errorCode != 0) return FALSE;	
  ApiCalculationRequest_appendBuffer(request->intermediate, request->intLength, API_EOL);
  ApiCalculationRequest_appendBuffer(request->intermediate,
				     request->intLength,
				     "11.1.6.3 Example 2");
  ApiCalculationRequest_appendBuffer(request->intermediate, request->intLength, API_EOL);
  tmp  =  ApiCalculationTest_checkResult(request,
					 resultRequest,
					 FALSE,
					 "11.1.6.3 Example 2",
					 errBuf,
					 errLength,
					 errorCode);
  if( tmp == FALSE || *errorCode != 0) result = FALSE;


  /** Example 3 */
  *errorCode = ApiCalculation_set11_1_6_3Parameters(request,
						    API_REFINED_PRODUCTS_NAME,
						    0, NULL,
						    68.02, ApiUnit_Temperature_F(),
						    11, ApiUnit_Pressure_PSI(),
						    0.8665, ApiUnit_Density_RELATIVE(),
						    150.3,ApiUnit_Temperature_F(),
						    534, ApiUnit_Pressure_PSI(),
						    285.45, ApiUnit_Volume_M3());
  if( *errorCode != 0) return FALSE;
  *errorCode = ApiCalculationTest_setResult(resultRequest,
					    868.730481427765, ApiUnit_Density_KGM3(),
					    0.996397864500,
					    0.4848444736631,
					    ApiUnit_ScaledComp_REV_PSI(),
					    1.000053335737,
					    0.996451008114,
					    284.4366525, ApiUnit_Volume_M3(),

					    0.959034901244,
					    0.631776484622,
					    ApiUnit_ScaledComp_REV_PSI(),
					    1.003385106716,
					    0.962281336729,
					    835.963128925946, ApiUnit_Density_KGM3(),
					    295.586162551, ApiUnit_Volume_M3(),

					    0, NULL);
  if( *errorCode != 0) return FALSE;	
  ApiCalculationRequest_appendBuffer(request->intermediate, request->intLength, API_EOL);
  ApiCalculationRequest_appendBuffer(request->intermediate,
				     request->intLength,
				     "11.1.6.3 Example 3");
  ApiCalculationRequest_appendBuffer(request->intermediate, request->intLength, API_EOL);
  tmp  =  ApiCalculationTest_checkResult(request,
					 resultRequest,
					 FALSE,
					 "11.1.6.3 Example 3",
					 errBuf,
					 errLength,
					 errorCode);
  if( tmp == FALSE || *errorCode != 0) result = FALSE;

  /** Example 4 */
  *errorCode = ApiCalculation_set11_1_6_3Parameters(request,
						    API_REFINED_PRODUCTS_NAME,
						    0, NULL,
						    72.04, ApiUnit_Temperature_F(),
						    375, ApiUnit_Pressure_PSI(),
						    41.4, ApiUnit_Density_API(),
						    -58,ApiUnit_Temperature_F(),
						    47.75, ApiUnit_Pressure_PSI(),
						    14.95, ApiUnit_Volume_M3());
  if( *errorCode != 0) return FALSE;
  *errorCode = ApiCalculationTest_setResult(resultRequest,
					    820.686533964218, ApiUnit_Density_KGM3(),
					    0.994083677273,
					    0.572660475665,
					    ApiUnit_ScaledComp_REV_PSI(),
					    1.002152098365,
					    0.996223043130,
					    14.893489, ApiUnit_Volume_M3(),

					    1.056751557035,
					    0.359117236149,
					    ApiUnit_ScaledComp_REV_PSI(),
					    1.000171507890,
					    1.056932798265,
					    867.410514841381, ApiUnit_Density_KGM3(),
					    14.0912728374, ApiUnit_Volume_M3(),

					    0, NULL);
  if( *errorCode != 0) return FALSE;	
  ApiCalculationRequest_appendBuffer(request->intermediate, request->intLength, API_EOL);
  ApiCalculationRequest_appendBuffer(request->intermediate,
				     request->intLength,
				     "11.1.6.3 Example 4");
  ApiCalculationRequest_appendBuffer(request->intermediate, request->intLength, API_EOL);
  tmp  =  ApiCalculationTest_checkResult(request,
					 resultRequest,
					 FALSE,
					 "11.1.6.3 Example 4",
					 errBuf,
					 errLength,
					 errorCode);
  if( tmp == FALSE || *errorCode != 0) result = FALSE;


  /** Example 5 */
  *errorCode = ApiCalculation_set11_1_6_3Parameters(request,
						    API_REFINED_PRODUCTS_NAME,
						    0, NULL,
						    35.34, ApiUnit_Temperature_F(),
						    38.4, ApiUnit_Pressure_PSI(),
						    790.53, ApiUnit_Density_KGM3(),
						    127.98,ApiUnit_Temperature_F(),
						    121.8, ApiUnit_Pressure_PSI(),
						    99998.0, ApiUnit_Volume_M3());
  if( *errorCode != 0) return FALSE;
  *errorCode = ApiCalculationTest_setResult(resultRequest,
					    779.174880755919, ApiUnit_Density_KGM3(),
					    1.014347780630,
					    0.578757914964,
					    ApiUnit_ScaledComp_REV_PSI(),
					    1.000222292442,
					    1.014573262475,
					    101454.97086, ApiUnit_Volume_M3(),

					    0.959835974243,
					    0.835760463789,
					    ApiUnit_ScaledComp_REV_PSI(),
					    1.001018993536,
					    0.960814040896,
					    748.642165743914, ApiUnit_Density_KGM3(),
				/** Due to rounding slightly different than text */
					    105593.1670777, ApiUnit_Volume_M3(),

					    0, NULL);
  if( *errorCode != 0) return FALSE;	
  ApiCalculationRequest_appendBuffer(request->intermediate, request->intLength, API_EOL);
  ApiCalculationRequest_appendBuffer(request->intermediate,
				     request->intLength,
				     "11.1.6.3 Example 5");
  ApiCalculationRequest_appendBuffer(request->intermediate, request->intLength, API_EOL);
  tmp  =  ApiCalculationTest_checkResult(request,
					 resultRequest,
					 FALSE,
					 "11.1.6.3 Example 5",
					 errBuf,
					 errLength,
					 errorCode);
  if( tmp == FALSE || *errorCode != 0) result = FALSE;


  /** Example 6 */
  *errorCode = ApiCalculation_set11_1_6_3Parameters(request,
						    API_REFINED_PRODUCTS_NAME,
						    0, NULL,
						    74.33, ApiUnit_Temperature_F(),
						    2, ApiUnit_Pressure_PSI(),
						    602.6, ApiUnit_Density_KGM3(),
						    -50.0,ApiUnit_Temperature_F(),
						    -2.33, ApiUnit_Pressure_PSI(),
						    501.7, ApiUnit_Volume_M3());
  if( *errorCode != 0) return FALSE;
  *errorCode = ApiCalculationTest_setResult(resultRequest,
					    610.620806310613, ApiUnit_Density_KGM3(),
					    0.986828242232,
					    1.837290338603,
					    ApiUnit_ScaledComp_REV_PSI(),
					    1.000036747157,
					    0.986864505365,
					    495.107662, ApiUnit_Volume_M3(),

					    1.09702673068,
					    0.831793615513,
					    ApiUnit_ScaledComp_REV_PSI(),
					    1.0,
					    1.097026730683,
					    669.867346834158, ApiUnit_Density_KGM3(),
					    451.316428903, ApiUnit_Volume_M3(),

					    0, NULL);
  if( *errorCode != 0) return FALSE;	
  ApiCalculationRequest_appendBuffer(request->intermediate, request->intLength, API_EOL);
  ApiCalculationRequest_appendBuffer(request->intermediate,
				     request->intLength,
				     "11.1.6.3 Example 6");
  ApiCalculationRequest_appendBuffer(request->intermediate, request->intLength, API_EOL);
  tmp  =  ApiCalculationTest_checkResult(request,
					 resultRequest,
					 FALSE,
					 "11.1.6.3 Example 6",
					 errBuf,
					 errLength,
					 errorCode);
  if( tmp == FALSE || *errorCode != 0) result = FALSE;


  /** Example 7 */
  *errorCode = ApiCalculation_set11_1_6_3Parameters(request,
						    API_COMMODITY_NOT_GIVEN,
						     0.00057634,
						    ApiUnit_ThermalExp_REV_F(),
						    84.5, ApiUnit_Temperature_F(),
						    157, ApiUnit_Pressure_PSI(),
						    853.7, ApiUnit_Density_KGM3(),
						    97.4,ApiUnit_Temperature_F(),
						    0, ApiUnit_Pressure_PSI(),
						    10000, ApiUnit_Volume_M3());
  if( *errorCode != 0) return FALSE;
  *errorCode = ApiCalculationTest_setResult(resultRequest,
					    865.279061018129, ApiUnit_Density_KGM3(),
					    0.985817857839,
					    0.516634323827,
					    ApiUnit_ScaledComp_REV_PSI(),
					    1.000811774331,
					    0.986618119472,
					    9866.2, ApiUnit_Volume_M3(),

					    0.978305995393,
					    0.538697628351,
					    ApiUnit_ScaledComp_REV_PSI(),
					    1.0,
					    0.978305995393,
					    846.507693082447, ApiUnit_Density_KGM3(),
					    10084.9424007, ApiUnit_Volume_M3(),

					    0, NULL);
  if( *errorCode != 0) return FALSE;	
  ApiCalculationRequest_appendBuffer(request->intermediate, request->intLength, API_EOL);
  ApiCalculationRequest_appendBuffer(request->intermediate,
				     request->intLength,
				     "11.1.6.3 Example 7");
  ApiCalculationRequest_appendBuffer(request->intermediate, request->intLength, API_EOL);
  tmp  =  ApiCalculationTest_checkResult(request,
					 resultRequest,
					 FALSE,
					 "11.1.6.3 Example 7",
					 errBuf,
					 errLength,
					 errorCode);
  if( tmp == FALSE || *errorCode != 0) result = FALSE;



  /** Example 8 */
  *errorCode = ApiCalculation_set11_1_6_3Parameters(request,
						    API_LUBRICATING_OIL_NAME ,
						    0, NULL,
						    -57.9, ApiUnit_Temperature_F(),
						    233, ApiUnit_Pressure_PSI(),
						    -14.1, ApiUnit_Density_API(),
						    60,ApiUnit_Temperature_F(),
						    245, ApiUnit_Pressure_PSI(),
						    251.2, ApiUnit_Volume_M3());
  if( *errorCode != 0) return FALSE;
  *errorCode = ApiCalculationTest_setResult(resultRequest,
					    1.16281779744e3, ApiUnit_Density_KGM3(),
					    1.034967289693,
					    0.219818333493,
					    ApiUnit_ScaledComp_REV_PSI(),
					    1.000512439176,
					    1.035497647478,
					    260.1176, ApiUnit_Volume_M3(),

					    1.000000000001,
					    0.273551802459,
					    ApiUnit_ScaledComp_REV_PSI(),
					    1.000670651388,
					    1.000670651389,
					    1.16359764281e3, ApiUnit_Density_KGM3(),
					    259.943437897, ApiUnit_Volume_M3(),

					    0, NULL);
  if( *errorCode != 0) return FALSE;	
  ApiCalculationRequest_appendBuffer(request->intermediate, request->intLength, API_EOL);
  ApiCalculationRequest_appendBuffer(request->intermediate,
				     request->intLength,
				     "11.1.6.3 Example 8");
  ApiCalculationRequest_appendBuffer(request->intermediate, request->intLength, API_EOL);
  tmp  =  ApiCalculationTest_checkResult(request,
					 resultRequest,
					 FALSE,
					 "11.1.6.3 Example 8",
					 errBuf,
					 errLength,
					 errorCode);
  if( tmp == FALSE || *errorCode != 0) result = FALSE;


  /** Example 9 */
  *errorCode = ApiCalculation_set11_1_6_3Parameters(request,
						    API_LUBRICATING_OIL_NAME ,
						    0, NULL,
						    302, ApiUnit_Temperature_F(),
						    1499.97, ApiUnit_Pressure_PSI(),
						    36.43, ApiUnit_Density_API(),
						    98.65,ApiUnit_Temperature_F(),
						    568.33, ApiUnit_Pressure_PSI(),
						    999.6, ApiUnit_Volume_M3());
  if( *errorCode != 0) return FALSE;
  *errorCode = ApiCalculationTest_setResult(resultRequest,
					    917.505498028854, ApiUnit_Density_KGM3(),
					    0.905934133257,
					    0.838261567336,
					    ApiUnit_ScaledComp_REV_PSI(),
					    1.012733782436,
					    0.917470101411,
					    917.103012, ApiUnit_Volume_M3(),

					    0.985240961815,
					    0.464994043354,
					    ApiUnit_ScaledComp_REV_PSI(),
					    1.002649703018,
					    0.987851557765,
					    906.359235485755, ApiUnit_Density_KGM3(),
					    928.382863795, ApiUnit_Volume_M3(),

					    0, NULL);
  if( *errorCode != 0) return FALSE;	
  ApiCalculationRequest_appendBuffer(request->intermediate, request->intLength, API_EOL);
  ApiCalculationRequest_appendBuffer(request->intermediate,
				     request->intLength,
				     "11.1.6.3 Example 9");
  ApiCalculationRequest_appendBuffer(request->intermediate, request->intLength, API_EOL);
  tmp  =  ApiCalculationTest_checkResult(request,
					 resultRequest,
					 FALSE,
					 "11.1.6.3 Example 9",
					 errBuf,
					 errLength,
					 errorCode);
  if( tmp == FALSE || *errorCode != 0) result = FALSE;


  /** Example 10 */
  *errorCode = ApiCalculation_set11_1_6_3Parameters(request,
						    API_CRUDE_OIL_NAME,
						    0, NULL,
						    -58.05, ApiUnit_Temperature_F(),
						    -5, ApiUnit_Pressure_PSI(),
						    169.16, ApiUnit_Density_API(),
						    -58.05,ApiUnit_Temperature_F(),
						    -5, ApiUnit_Pressure_PSI(),
						    1000, ApiUnit_Volume_M3());
  if( *errorCode != 0) return FALSE;
  ApiCalculationRequest_appendBuffer(request->intermediate, request->intLength, API_EOL);
  ApiCalculationRequest_appendBuffer(request->intermediate,
				     request->intLength,
				     "11.1.6.3 Example 10");
  ApiCalculationRequest_appendBuffer(request->intermediate, request->intLength, API_EOL);
  tmp  =  ApiCalculationTest_checkResult(request,
					 resultRequest,
					 TRUE,
					 "11.1.6.3 Example 10",
					 errBuf,
					 errLength,
					 errorCode);
  if( tmp == FALSE || *errorCode != 0) result = FALSE;


  /** Example 11 */
  *errorCode = ApiCalculation_set11_1_6_3Parameters(request,
						    API_CRUDE_OIL_NAME,
						    0, NULL,
						    302.8, ApiUnit_Temperature_F(),
						    1500.43, ApiUnit_Pressure_PSI(),
						    -13.87, ApiUnit_Density_API(),
						    302.8,ApiUnit_Temperature_F(),
						    0, ApiUnit_Pressure_PSI(),
						    1000, ApiUnit_Volume_M3());
  if( *errorCode != 0) return FALSE;
  ApiCalculationRequest_appendBuffer(request->intermediate, request->intLength, API_EOL);
  ApiCalculationRequest_appendBuffer(request->intermediate,
				     request->intLength,
				     "11.1.6.3 Example 11");
  ApiCalculationRequest_appendBuffer(request->intermediate, request->intLength, API_EOL);
  tmp  =  ApiCalculationTest_checkResult(request,
					 resultRequest,
					 TRUE,
					 "11.1.6.3 Example 11",
					 errBuf,
					 errLength,
					 errorCode);
  if( tmp == FALSE || *errorCode != 0) result = FALSE;


  /** Example 12 */
  *errorCode = ApiCalculation_set11_1_6_3Parameters(request,
						    API_CRUDE_OIL_NAME,
						    0, NULL,
						    89.08, ApiUnit_Temperature_F(),
						    0.43, ApiUnit_Pressure_PSI(),
						    -9.1, ApiUnit_Density_API(),
						    97.06,ApiUnit_Temperature_F(),
						    27.75, ApiUnit_Pressure_PSI(),
						    1000, ApiUnit_Volume_M3());
  if( *errorCode != 0) return FALSE;
  *errorCode = ApiCalculationTest_setResult(resultRequest,
					    1.16345298090e3, ApiUnit_Density_KGM3(),
					    0.992654442380,
					    0.288480047418,
					    ApiUnit_ScaledComp_REV_PSI(),
					    1.000001240466,
					    0.992655673734,
					    992.66, ApiUnit_Volume_M3(),

					    0.990633258118,
					    0.292777732234,
					    ApiUnit_ScaledComp_REV_PSI(),
					    1.000081252422,
					    0.990713749470,
					    1.15264886504e3, ApiUnit_Density_KGM3(),
					    1001.96828537, ApiUnit_Volume_M3(),

					    0, NULL);
  if( *errorCode != 0) return FALSE;	
  ApiCalculationRequest_appendBuffer(request->intermediate, request->intLength, API_EOL);
  ApiCalculationRequest_appendBuffer(request->intermediate,
				     request->intLength,
				     "11.1.6.3 Example 12");
  ApiCalculationRequest_appendBuffer(request->intermediate, request->intLength, API_EOL);
  tmp  =  ApiCalculationTest_checkResult(request,
					 resultRequest,
					 FALSE,
					 "11.1.6.3 Example 12",
					 errBuf,
					 errLength,
					 errorCode);
  if( tmp == FALSE || *errorCode != 0) result = FALSE;
 

  /** Example 13 */
  *errorCode = ApiCalculation_set11_1_6_3Parameters(request,
						    API_CRUDE_OIL_NAME,
						    0, NULL,
						    87.9, ApiUnit_Temperature_F(),
						    172.34, ApiUnit_Pressure_PSI(),
						    105.3, ApiUnit_Density_API(),
						    237.8,ApiUnit_Temperature_F(),
						    0, ApiUnit_Pressure_PSI(),
						    1000, ApiUnit_Volume_M3());
  if( *errorCode != 0) return FALSE;
  *errorCode = ApiCalculationTest_setResult(resultRequest,
					    610.603878491534, ApiUnit_Density_KGM3(),
					    0.974283669212,
					    2.003584457334,
					    ApiUnit_ScaledComp_REV_PSI(),
					    1.003464941820,
					    0.977659505442,
					    977.66, ApiUnit_Volume_M3(),

					    0.832034523163,
					    5.209412010593,
					    ApiUnit_ScaledComp_REV_PSI(),
					    1.0,
					    0.832034523163,
					    508.043506882197, ApiUnit_Density_KGM3(),
					    1175.02974652, ApiUnit_Volume_M3(),

					    0, NULL);
  if( *errorCode != 0) return FALSE;	
  ApiCalculationRequest_appendBuffer(request->intermediate, request->intLength, API_EOL);
  ApiCalculationRequest_appendBuffer(request->intermediate,
				     request->intLength,
				     "11.1.6.3 Example 13");
  ApiCalculationRequest_appendBuffer(request->intermediate, request->intLength, API_EOL);
  tmp  =  ApiCalculationTest_checkResult(request,
					 resultRequest,
					 FALSE,
					 "11.1.6.3 Example 13",
					 errBuf,
					 errLength,
					 errorCode);
  if( tmp == FALSE || *errorCode != 0) result = FALSE;

  /** Example 14 */
  *errorCode = ApiCalculation_set11_1_6_3Parameters(request,
						    API_REFINED_PRODUCTS_NAME,
						    0, NULL,
						    -58.05, ApiUnit_Temperature_F(),
						    0, ApiUnit_Pressure_PSI(),
						    470.17, ApiUnit_Density_KGM3(),
						    -58.05,ApiUnit_Temperature_F(),
						    0, ApiUnit_Pressure_PSI(),
						    1000, ApiUnit_Volume_M3());
  if( *errorCode != 0) return FALSE;
  ApiCalculationRequest_appendBuffer(request->intermediate, request->intLength, API_EOL);
  ApiCalculationRequest_appendBuffer(request->intermediate,
				     request->intLength,
				     "11.1.6.3 Example 14");
  ApiCalculationRequest_appendBuffer(request->intermediate, request->intLength, API_EOL);
  tmp  =  ApiCalculationTest_checkResult(request,
					 resultRequest,
					 TRUE,
					 "11.1.6.3 Example 14",
					 errBuf,
					 errLength,
					 errorCode);
  if( tmp == FALSE || *errorCode != 0) result = FALSE;

  /** Example 15 */
  *errorCode = ApiCalculation_set11_1_6_3Parameters(request,
						    API_REFINED_PRODUCTS_NAME,
						    0, NULL,
						    302.8, ApiUnit_Temperature_F(),
						    1500.5, ApiUnit_Pressure_PSI(),
						    1209.56, ApiUnit_Density_KGM3(),
						    302.8,ApiUnit_Temperature_F(),
						    0, ApiUnit_Pressure_PSI(),
						    1000, ApiUnit_Volume_M3());
  if( *errorCode != 0) return FALSE;
  ApiCalculationRequest_appendBuffer(request->intermediate, request->intLength, API_EOL);
  ApiCalculationRequest_appendBuffer(request->intermediate,
				     request->intLength,
				     "11.1.6.3 Example 15");
  ApiCalculationRequest_appendBuffer(request->intermediate, request->intLength, API_EOL);
  tmp  =  ApiCalculationTest_checkResult(request,
					 resultRequest,
					 TRUE,
					 "11.1.6.3 Example 15",
					 errBuf,
					 errLength,
					 errorCode);
  if( tmp == FALSE || *errorCode != 0) result = FALSE;


  /** Example 16 */
  *errorCode = ApiCalculation_set11_1_6_3Parameters(request,
						    API_LUBRICATING_OIL_NAME,
						    0, NULL,
						    -58.05, ApiUnit_Temperature_F(),
						    0, ApiUnit_Pressure_PSI(),
						    -10.05, ApiUnit_Density_API(),
						    -58.05,ApiUnit_Temperature_F(),
						    0, ApiUnit_Pressure_PSI(),
						    1000, ApiUnit_Volume_M3());
  if( *errorCode != 0) return FALSE;
  ApiCalculationRequest_appendBuffer(request->intermediate, request->intLength, API_EOL);
  ApiCalculationRequest_appendBuffer(request->intermediate,
				     request->intLength,
				     "11.1.6.3 Example 16");
  ApiCalculationRequest_appendBuffer(request->intermediate, request->intLength, API_EOL);
  tmp  =  ApiCalculationTest_checkResult(request,
					 resultRequest,
					 TRUE,
					 "11.1.6.3 Example 16",
					 errBuf,
					 errLength,
					 errorCode);
  if( tmp == FALSE || *errorCode != 0) result = FALSE;


  /** Example 17 */
  *errorCode = ApiCalculation_set11_1_6_3Parameters(request,
						    API_LUBRICATING_OIL_NAME,
						    0, NULL,
						    302.05, ApiUnit_Temperature_F(),
						    1500.5, ApiUnit_Pressure_PSI(),
						    45.08, ApiUnit_Density_API(),
						    302.05,ApiUnit_Temperature_F(),
						    0, ApiUnit_Pressure_PSI(),
						    1000, ApiUnit_Volume_M3());
  if( *errorCode != 0) return FALSE;
  ApiCalculationRequest_appendBuffer(request->intermediate, request->intLength, API_EOL);
  ApiCalculationRequest_appendBuffer(request->intermediate,
				     request->intLength,
				     "11.1.6.3 Example 17");
  ApiCalculationRequest_appendBuffer(request->intermediate, request->intLength, API_EOL);
  tmp  =  ApiCalculationTest_checkResult(request,
					 resultRequest,
					 TRUE,
					 "11.1.6.3 Example 17",
					 errBuf,
					 errLength,
					 errorCode);
  if( tmp == FALSE || *errorCode != 0) result = FALSE;


  /** Example 18 */
  *errorCode = ApiCalculation_set11_1_6_3Parameters(request,
						    API_LUBRICATING_OIL_NAME,
						    0, NULL,
						    -57.95, ApiUnit_Temperature_F(),
						    233.7, ApiUnit_Pressure_PSI(),
						    -14.15, ApiUnit_Density_API(),
						    60.0,ApiUnit_Temperature_F(),
						    245.66, ApiUnit_Pressure_PSI(),
						    1000, ApiUnit_Volume_M3());
  if( *errorCode != 0) return FALSE;
  *errorCode = ApiCalculationTest_setResult(resultRequest,
					    1.16331178744e3, ApiUnit_Density_KGM3(),
					    1.034967263424,
					    0.219706997262,
					    ApiUnit_ScaledComp_REV_PSI(),
					    1.000513719024,
					    1.035498945797,
					    1035.5, ApiUnit_Volume_M3(),

					    1.000000000001,
					    0.273391493229,
					    ApiUnit_ScaledComp_REV_PSI(),
					    1.000672064910,
					    1.000672064911,
					    1.16409360847e3, ApiUnit_Density_KGM3(),
					    1034.8066795, ApiUnit_Volume_M3(),

					    0, NULL);
  if( *errorCode != 0) return FALSE;	
  ApiCalculationRequest_appendBuffer(request->intermediate, request->intLength, API_EOL);
  ApiCalculationRequest_appendBuffer(request->intermediate,
				     request->intLength,
				     "11.1.6.3 Example 18");
  ApiCalculationRequest_appendBuffer(request->intermediate, request->intLength, API_EOL);
  tmp  =  ApiCalculationTest_checkResult(request,
					 resultRequest,
					 FALSE,
					 "11.1.6.3 Example 18",
					 errBuf,
					 errLength,
					 errorCode);
  if( tmp == FALSE || *errorCode != 0) result = FALSE;

  /** Clean up the result object */
  ApiCalculationRequest_cleanUp(resultRequest);
  resultRequest = NULL;


  return result;
}


