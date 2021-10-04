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


#include "apiVCF.h"
#include "VBAExport.h"
#include "math.h"
#include "calculationTest.h"

#include <stdio.h>
#include <string.h>

typedef double (*valueCalc_proto)(double density, 
				  double temp,
				  double pres,
				  int *errorCode);

bool compareNumber(char * buffer,
		   int bufLength,
		   char * funcName,
		   valueCalc_proto func,
		   double expected,
		   double density,
		   double temp,
		   double pres,
		   int *errorCode){
   double value =  func(density, temp, pres, errorCode);
   if( *errorCode != 0)  return FALSE;
   if( fabs(value - expected) >= API_EPSILON){
     *errorCode = -1;
     ApiCalculationRequest_appendBuffer(buffer, bufLength,API_EOL);
     ApiCalculationRequest_appendBuffer(buffer,
					bufLength,
					funcName);
     ApiCalculationRequest_appendBuffer(buffer,
					bufLength,
					" failed");
       return FALSE;
   }
   return TRUE;
};

typedef double (*valueCalc_proto_alpha60)(double alpha60,
					  double density,
					  double temp,
					  double pres,
					  int *errorCode);

bool compareNumber_alpha60(char * buffer,
			   int bufLength,
			   char * funcName,
			   valueCalc_proto_alpha60 func,
			   double expected,
			   double alpha60,
			   double density,
			   double temp,
			   double pres,
			   int *errorCode){
   double value =  func(alpha60,density, temp, pres, errorCode);
   if( *errorCode != 0)  return FALSE;
   if( fabs(value - expected) >= API_EPSILON){
     *errorCode = -1;
     ApiCalculationRequest_appendBuffer(buffer, bufLength,API_EOL);
     ApiCalculationRequest_appendBuffer(buffer,
					bufLength,
					funcName);
     ApiCalculationRequest_appendBuffer(buffer,
					bufLength,
					" failed");
       return FALSE;
   }
   return TRUE;
};


typedef double (*valueCalc_proto_met)(double density, 
				      double temp,
				      double pres,
				      double baseTemp,
				      int *errorCode);

bool compareNumber_met(char * buffer,
		       int bufLength,
		       char * funcName,
		       valueCalc_proto_met func,
		       double expected,
		       double density,
		       double temp,
		       double pres,
		       double baseTemp,
		       int *errorCode){
   double value =  func(density, temp, pres, baseTemp,errorCode);
   if( *errorCode != 0)  return FALSE;
   if( fabs(value - expected) >= API_EPSILON){
     *errorCode = -1;
     ApiCalculationRequest_appendBuffer(buffer, bufLength,API_EOL);
     ApiCalculationRequest_appendBuffer(buffer,
					bufLength,
					funcName);
     ApiCalculationRequest_appendBuffer(buffer,
					bufLength,
					" failed");
       return FALSE;
   }
   return TRUE;
};

typedef double (*valueCalc_proto_alpha60_met)(double alpha60,
					      double density,
					      double temp,
					      double pres,
					      double baseTemp,
					      int *errorCode);

bool compareNumber_alpha60_met(char * buffer,
			       int bufLength,
			       char * funcName,
			       valueCalc_proto_alpha60_met func,
			       double expected,
			       double alpha60,
			       double density,
			       double temp,
			       double pres,
			       double baseTemp,
			       int *errorCode){
   double value =  func(alpha60,density, temp, pres, baseTemp,errorCode);
   if( *errorCode != 0)  return FALSE;
   if( fabs(value - expected) >= API_EPSILON){
     *errorCode = -1;
     ApiCalculationRequest_appendBuffer(buffer, bufLength,API_EOL);
     ApiCalculationRequest_appendBuffer(buffer,
					bufLength,
					funcName);
     ApiCalculationRequest_appendBuffer(buffer,
					bufLength,
					" failed");
       return FALSE;
   }
   return TRUE;
};


/**  
* A test routine that checks that all functions  that return ctplBaseToAlt
* work correctly.
* 
* @param buffer a buffer in which to report errors
* @param bufLength the length of the buffer
* @param errorCode a variable in which to return errors if any
*
* @return TRUE if all test run successfully, FALSE otherwise
*/
bool VBAExport_RoutineTestctplBase(char * buffer, int bufLength, int * errorCode){
    ApiCalculationRequest * request;
    double density,comp,pres;
    double compDens,compFp;
    bool result;

   request = ApiCalculationRequest_init(errorCode);
   if( *errorCode != 0)  return FALSE;

   /** Test ctpl functions */
   /* 11.1.6.1 routines  for Crude Oil */
   *errorCode = ApiCalculation_set11_1_6_1Parameters(request,
						 API_CRUDE_OIL_NAME,
						 0, NULL,
						 85.0, ApiUnit_Temperature_F(),
						 247.3, ApiUnit_Pressure_PSI(),
						 0.7943,
						 ApiUnit_Density_RELATIVE(),
						 0, NULL);
   if( *errorCode != 0)  return FALSE;
   *errorCode = ApiCalculationRequest_performCalculation(request);
   if( *errorCode != 0)  return FALSE;
   comp = ApiQuantity_getValue((ApiQuantity*)request->ctplBaseToAlt,
			       ApiUnit_Expansion_DIMLESS(),
			       TRUE,
			       errorCode);
   if( *errorCode != 0)  return FALSE;
   compFp = ApiQuantity_getValue((ApiQuantity*)request->scaledCompAlt,
		 	         ApiUnit_ScaledComp_REV_PSI(),
				 FALSE,
				 errorCode);
   if( *errorCode != 0)  return FALSE;
    
   density =  ApiQuantity_getValue((ApiQuantity*)request->baseDens,
				   ApiUnit_Density_API(),
				   FALSE,
				   errorCode);
   if( *errorCode != 0)  return FALSE;
   result =  compareNumber(buffer, bufLength, "ctplA11161API",
                           ctplA11161API, comp, density, 85.0, 247.3,
			   errorCode);
   if( result == FALSE) return result;
   compDens =  ApiQuantity_getValue((ApiQuantity*)request->altDens,
				   ApiUnit_Density_API(),
				   FALSE,
				   errorCode);
   if( *errorCode != 0)  return FALSE;
   result =  compareNumber(buffer, bufLength, "densA11161API",
                           densA11161API, compDens, density, 85.0, 247.3,
			   errorCode);
   if( result == FALSE) return result;
   result =  compareNumber(buffer, bufLength, "fpA11161API",
                           fpA11161API, compFp, density, 85.0, 247.3,
			   errorCode);
   if( result == FALSE) return result;


   density =  ApiQuantity_getValue((ApiQuantity*)request->baseDens,
				   ApiUnit_Density_RELATIVE(),
				   FALSE,
				   errorCode);
   if( *errorCode != 0)  return FALSE;
   result =  compareNumber(buffer, bufLength, "ctplA11161RelDens",
                           ctplA11161RelDens, comp, density, 85.0, 247.3,
			   errorCode);
   if( result == FALSE) return result;
   compDens =  ApiQuantity_getValue((ApiQuantity*)request->altDens,
				   ApiUnit_Density_RELATIVE(),
				   FALSE,
				   errorCode);
   if( *errorCode != 0)  return FALSE;
   result =  compareNumber(buffer, bufLength, "densA11161RelDens",
                           densA11161RelDens, compDens, density, 85.0, 247.3,
			   errorCode);
   if( result == FALSE) return result;
   result =  compareNumber(buffer, bufLength, "fpA11161RelDens",
                           fpA11161RelDens, compFp, density, 85.0, 247.3,
			   errorCode);
   if( result == FALSE) return result;


   /* 11.1.6.1 routines  for Refined Products */
   *errorCode = ApiCalculation_set11_1_6_1Parameters(request,
						 API_REFINED_PRODUCTS_NAME,
						 0, NULL,
						 85.0, ApiUnit_Temperature_F(),
						 247.3, ApiUnit_Pressure_PSI(),
						 0.7943,
						 ApiUnit_Density_RELATIVE(),
						 0, NULL);
   if( *errorCode != 0)  return FALSE;
   *errorCode = ApiCalculationRequest_performCalculation(request);
   if( *errorCode != 0)  return FALSE;
   comp = ApiQuantity_getValue((ApiQuantity*)request->ctplBaseToAlt,
			       ApiUnit_Expansion_DIMLESS(),
			       TRUE,
			       errorCode);
   if( *errorCode != 0)  return FALSE;
   compFp = ApiQuantity_getValue((ApiQuantity*)request->scaledCompAlt,
		 	         ApiUnit_ScaledComp_REV_PSI(),
				 FALSE,
				 errorCode);
   if( *errorCode != 0)  return FALSE;
     
   density =  ApiQuantity_getValue((ApiQuantity*)request->baseDens,
				   ApiUnit_Density_API(),
				   FALSE,
				   errorCode);
   if( *errorCode != 0)  return FALSE;
   result =  compareNumber(buffer, bufLength, "ctplB11161API",
                           ctplB11161API, comp, density, 85.0, 247.3,
			   errorCode);
   if( result == FALSE) return result;
   compDens =  ApiQuantity_getValue((ApiQuantity*)request->altDens,
				    ApiUnit_Density_API(),
				    FALSE,
				    errorCode);
   if( *errorCode != 0)  return FALSE;
   result =  compareNumber(buffer, bufLength, "densB11161API",
                           densB11161API, compDens, density, 85.0, 247.3,
			   errorCode);
   if( result == FALSE) return result;
   result =  compareNumber(buffer, bufLength, "fpB11161API",
                           fpB11161API, compFp, density, 85.0, 247.3,
			   errorCode);
   if( result == FALSE) return result;

   density =  ApiQuantity_getValue((ApiQuantity*)request->baseDens,
				   ApiUnit_Density_RELATIVE(),
				   FALSE,
				   errorCode);
   if( *errorCode != 0)  return FALSE;
   result =  compareNumber(buffer, bufLength, "ctplB11161RelDens",
                           ctplB11161RelDens, comp, density, 85.0, 247.3,
			   errorCode);
   if( result == FALSE) return result;
   compDens =  ApiQuantity_getValue((ApiQuantity*)request->altDens,
				    ApiUnit_Density_RELATIVE(),
				    FALSE,
				    errorCode);
   if( *errorCode != 0)  return FALSE;
   result =  compareNumber(buffer, bufLength, "densB11161RelDens",
                           densB11161RelDens, compDens, density, 85.0, 247.3,
			   errorCode);
   if( result == FALSE) return result;
   result =  compareNumber(buffer, bufLength, "fpB11161RelDens",
                           fpB11161RelDens, compFp, density, 85.0, 247.3,
			   errorCode);
   if( result == FALSE) return result;


   /* 11.1.6.1 routines  for Lubrication Oil */
   *errorCode = ApiCalculation_set11_1_6_1Parameters(request,
						 API_LUBRICATING_OIL_NAME,
						 0, NULL,
						 85.0, ApiUnit_Temperature_F(),
						 247.3, ApiUnit_Pressure_PSI(),
						 42.9, ApiUnit_Density_API(),
						 0, NULL);
   if( *errorCode != 0)  return FALSE;
   *errorCode = ApiCalculationRequest_performCalculation(request);
   if( *errorCode != 0)  return FALSE;
   comp = ApiQuantity_getValue((ApiQuantity*)request->ctplBaseToAlt,
			       ApiUnit_Expansion_DIMLESS(),
			       TRUE,
			       errorCode);
   if( *errorCode != 0)  return FALSE;
   compFp = ApiQuantity_getValue((ApiQuantity*)request->scaledCompAlt,
		 	         ApiUnit_ScaledComp_REV_PSI(),
				 FALSE,
				 errorCode);
   if( *errorCode != 0)  return FALSE;
     
   density =  ApiQuantity_getValue((ApiQuantity*)request->baseDens,
				   ApiUnit_Density_API(),
				   FALSE,
				   errorCode);
   if( *errorCode != 0)  return FALSE;
   result =  compareNumber(buffer, bufLength, "ctplD11161API",
                           ctplD11161API, comp, density, 85.0, 247.3,
			   errorCode);
   if( result == FALSE) return result;
   compDens =  ApiQuantity_getValue((ApiQuantity*)request->altDens,
				   ApiUnit_Density_API(),
				   FALSE,
				   errorCode);
   if( *errorCode != 0)  return FALSE;
   result =  compareNumber(buffer, bufLength, "densD11161API",
                           densD11161API, compDens, density, 85.0, 247.3,
			   errorCode);
   if( result == FALSE) return result;
   result =  compareNumber(buffer, bufLength, "fpD11161API",
                           fpD11161API, compFp, density, 85.0, 247.3,
			   errorCode);
   if( result == FALSE) return result;


   density =  ApiQuantity_getValue((ApiQuantity*)request->baseDens,
				   ApiUnit_Density_RELATIVE(),
				   FALSE,
				   errorCode);
   if( *errorCode != 0)  return FALSE;
   result =  compareNumber(buffer, bufLength, "ctplD11161RelDens",
                           ctplD11161RelDens, comp, density, 85.0, 247.3,
			   errorCode);
   if( result == FALSE) return result;
   compDens =  ApiQuantity_getValue((ApiQuantity*)request->altDens,
				   ApiUnit_Density_RELATIVE(),
				   FALSE,
				   errorCode);
   if( *errorCode != 0)  return FALSE;
   result =  compareNumber(buffer, bufLength, "densD11161RelDens",
                           densD11161RelDens, compDens, density, 85.0, 247.3,
			   errorCode);
   if( result == FALSE) return result;
   result =  compareNumber(buffer, bufLength, "fpD11161RelDens",
                           fpD11161RelDens, compFp, density, 85.0, 247.3,
			   errorCode);
   if( result == FALSE) return result;


   /** The 11.1.6.1 functions for  alpha60 values */
   *errorCode = ApiCalculation_set11_1_6_1Parameters(request,
						 API_COMMODITY_NOT_GIVEN,
						 0.00056, ApiUnit_ThermalExp_REV_F(),
						 85.0, ApiUnit_Temperature_F(),
						 247.3, ApiUnit_Pressure_PSI(),
						 0.7943,
						 ApiUnit_Density_RELATIVE(),
						 0, NULL);
   if( *errorCode != 0)  return FALSE;
   *errorCode = ApiCalculationRequest_performCalculation(request);
   if( *errorCode != 0)  return FALSE;
   comp = ApiQuantity_getValue((ApiQuantity*)request->ctplBaseToAlt,
			       ApiUnit_Expansion_DIMLESS(),
			       TRUE,
			       errorCode);
   if( *errorCode != 0)  return FALSE;
   compFp = ApiQuantity_getValue((ApiQuantity*)request->scaledCompAlt,
		 	         ApiUnit_ScaledComp_REV_PSI(),
				 FALSE,
				 errorCode);
   if( *errorCode != 0)  return FALSE;
    
   density =  ApiQuantity_getValue((ApiQuantity*)request->baseDens,
				   ApiUnit_Density_API(),
				   FALSE,
				   errorCode);
   if( *errorCode != 0)  return FALSE;
   result =  compareNumber_alpha60(buffer, bufLength, "ctplC11161API",
                                   ctplC11161API, comp, 
                                   0.00056, density, 85.0, 247.3,
	  		           errorCode);
   if( result == FALSE) return result;
   compDens =  ApiQuantity_getValue((ApiQuantity*)request->altDens,
		   		    ApiUnit_Density_API(),
				    FALSE,
				    errorCode);
   if( *errorCode != 0)  return FALSE;
   result =  compareNumber_alpha60(buffer, bufLength, "denslC11161API",
                                   densC11161API, compDens, 
                                   0.00056, density, 85.0, 247.3,
	  		           errorCode);
   if( result == FALSE) return result;
   result =  compareNumber_alpha60(buffer, bufLength, "fpC11161API",
                                   fpC11161API, compFp, 
                                   0.00056, density, 85.0, 247.3,
	  		           errorCode);
   if( result == FALSE) return result;

   
   density =  ApiQuantity_getValue((ApiQuantity*)request->baseDens,
				   ApiUnit_Density_RELATIVE(),
				   FALSE,
				   errorCode);
   if( *errorCode != 0)  return FALSE;
   result =  compareNumber_alpha60(buffer, bufLength, "ctplC11161RelDens",
                                   ctplC11161RelDens, comp, 
                                   0.00056, density, 85.0, 247.3,
			           errorCode);
   if( result == FALSE) return result;
   compDens =  ApiQuantity_getValue((ApiQuantity*)request->altDens,
		 	 	    ApiUnit_Density_RELATIVE(),
				    FALSE,
				    errorCode);
   if( *errorCode != 0)  return FALSE;
   result =  compareNumber_alpha60(buffer, bufLength, "densC11161RelDens",
                                   densC11161RelDens, compDens, 
                                   0.00056, density, 85.0, 247.3,
			           errorCode);
   if( result == FALSE) return result;
   result =  compareNumber_alpha60(buffer, bufLength, "fpC11161RelDens",
                                   fpC11161RelDens, compFp, 
                                   0.00056, density, 85.0, 247.3,
			           errorCode);
   if( result == FALSE) return result;


   /** Test ctpl functions */
   /* 11.1.7.1 routines  for Crude Oil */
   *errorCode = ApiCalculation_set11_1_7_1Parameters(request,
						   API_CRUDE_OIL_NAME,
						   0, NULL,
						   -32.8, ApiUnit_Temperature_C(),
						   24.6, ApiUnit_Pressure_BAR(),
						   772.3,
						   ApiUnit_Density_KGM3(),

						   15.0, ApiUnit_Temperature_C(),
						   0, NULL);
   if( *errorCode != 0)  return FALSE;
   *errorCode = ApiCalculationRequest_performCalculation(request);
   if( *errorCode != 0)  return FALSE;
   compDens  = ApiQuantity_getValue((ApiQuantity*)request->altDens,
		  	            ApiUnit_Density_KGM3(),
			            FALSE,
			           errorCode);
   if( *errorCode != 0)  return FALSE;
   comp = ApiQuantity_getValue((ApiQuantity*)request->ctplBaseToAlt,
	 	  	        ApiUnit_Expansion_DIMLESS(),
			        TRUE,
			        errorCode);
   if( *errorCode != 0)  return FALSE;
    
   pres =  ApiQuantity_getValue((ApiQuantity*)request->altPres,
				ApiUnit_Pressure_BAR(),
				FALSE,
				errorCode);
   if( *errorCode != 0)  return FALSE;
   result =  compareNumber_met(buffer, bufLength, "ctplA11171bar",
                               ctplA11171bar, comp,772.3, -32.8, pres,15,
			       errorCode);
   if( result == FALSE) return result;
   result =  compareNumber_met(buffer, bufLength, "densA11171bar",
                               densA11171bar, compDens,772.3, -32.8, pres,15,
			       errorCode);
   if( result == FALSE) return result;
   compFp = ApiQuantity_getValue((ApiQuantity*)request->scaledCompAlt,
		 	         ApiUnit_ScaledComp_REV_BAR(),
				 FALSE,
				 errorCode);
   if( *errorCode != 0)  return FALSE;
   result =  compareNumber_met(buffer, bufLength, "fpA11171bar",
                               fpA11171bar, compFp,772.3, -32.8, pres,15,
			       errorCode);
   if( result == FALSE) return result;


   pres =  ApiQuantity_getValue((ApiQuantity*)request->altPres,
				ApiUnit_Pressure_KPA(),
				FALSE,
				errorCode);
   if( *errorCode != 0)  return FALSE;
   result =  compareNumber_met(buffer, bufLength, "ctplA11171kPa",
                               ctplA11171kPa, comp,772.3, -32.8, pres, 15,
			       errorCode);
   if( result == FALSE) return result;
   result =  compareNumber_met(buffer, bufLength, "densA11171kPa",
                               densA11171kPa, compDens,772.3, -32.8, pres, 15,
			       errorCode);
   if( result == FALSE) return result;
   compFp = ApiQuantity_getValue((ApiQuantity*)request->scaledCompAlt,
		 	         ApiUnit_ScaledComp_REV_KPA(),
				 FALSE,
				 errorCode);
   if( *errorCode != 0)  return FALSE;
   result =  compareNumber_met(buffer, bufLength, "fpA11171kPa",
                               fpA11171kPa, compFp,772.3, -32.8, pres, 15,
			       errorCode);
   if( result == FALSE) return result;
   

   /* 11.1.7.1 routines  for Refined Products */
   *errorCode = ApiCalculation_set11_1_7_1Parameters(request,
						   API_REFINED_PRODUCTS_NAME,
						   0, NULL,
						   -32.8, ApiUnit_Temperature_C(),
						   24.6, ApiUnit_Pressure_BAR(),
						   772.3,
						   ApiUnit_Density_KGM3(),

						   15.0, ApiUnit_Temperature_C(),
						   0, NULL);
   if( *errorCode != 0)  return FALSE;
   *errorCode = ApiCalculationRequest_performCalculation(request);
   if( *errorCode != 0)  return FALSE;
   comp = ApiQuantity_getValue((ApiQuantity*)request->ctplBaseToAlt,
			       ApiUnit_Expansion_DIMLESS(),
			       TRUE,
			       errorCode);
   if( *errorCode != 0)  return FALSE;
   compDens = ApiQuantity_getValue((ApiQuantity*)request->altDens,
		 	            ApiUnit_Density_KGM3(),
			           FALSE,
				   errorCode);
   if( *errorCode != 0)  return FALSE;
    
   pres =  ApiQuantity_getValue((ApiQuantity*)request->altPres,
				ApiUnit_Pressure_BAR(),
				FALSE,
				errorCode);
   if( *errorCode != 0)  return FALSE;
   result =  compareNumber_met(buffer, bufLength, "ctplB11171bar",
			       ctplB11171bar, comp,772.3, -32.8, pres,15,
			       errorCode);
   if( result == FALSE) return result;
   result =  compareNumber_met(buffer, bufLength, "densB11171bar",
			       densB11171bar, compDens,772.3, -32.8, pres,15,
			       errorCode);
   if( result == FALSE) return result;
   compFp = ApiQuantity_getValue((ApiQuantity*)request->scaledCompAlt,
		 	         ApiUnit_ScaledComp_REV_BAR(),
				 FALSE,
				 errorCode);
   if( *errorCode != 0)  return FALSE;
   result =  compareNumber_met(buffer, bufLength, "fpB11171bar",
			       fpB11171bar, compFp,772.3, -32.8, pres,15,
			       errorCode);
   if( result == FALSE) return result;


   pres =  ApiQuantity_getValue((ApiQuantity*)request->altPres,
				ApiUnit_Pressure_KPA(),
				FALSE,
				errorCode);
   if( *errorCode != 0)  return FALSE;
   result =  compareNumber_met(buffer, bufLength, "ctplB11171kPa",
                               ctplB11171kPa, comp,772.3, -32.8, pres,15,
			       errorCode);
   if( result == FALSE) return result; 
   result =  compareNumber_met(buffer, bufLength, "densB11171kPa",
                               densB11171kPa, compDens,772.3, -32.8, pres,15,
			       errorCode);
   if( result == FALSE) return result;
   compFp = ApiQuantity_getValue((ApiQuantity*)request->scaledCompAlt,
		 	         ApiUnit_ScaledComp_REV_KPA(),
				 FALSE,
				 errorCode);
   if( *errorCode != 0)  return FALSE;
   result =  compareNumber_met(buffer, bufLength, "fpB11171kPa",
                               fpB11171kPa, compFp,772.3, -32.8, pres,15,
			       errorCode);
   if( result == FALSE) return result;
   

   /* 11.1.7.1 routines  for Lubrication oil */
   *errorCode = ApiCalculation_set11_1_7_1Parameters(request,
						   API_LUBRICATING_OIL_NAME,
						   0, NULL,
						   50, ApiUnit_Temperature_C(),
						   15, ApiUnit_Pressure_BAR(),
						   900,
						   ApiUnit_Density_KGM3(),

						   15.0, ApiUnit_Temperature_C(),
						   0, NULL);
   if( *errorCode != 0)  return FALSE;
   *errorCode = ApiCalculationRequest_performCalculation(request);
   if( *errorCode != 0)  return FALSE;
   comp = ApiQuantity_getValue((ApiQuantity*)request->ctplBaseToAlt,
			       ApiUnit_Expansion_DIMLESS(),
			       TRUE,
			       errorCode);
   if( *errorCode != 0)  return FALSE;
   compDens = ApiQuantity_getValue((ApiQuantity*)request->altDens,
		 	            ApiUnit_Density_KGM3(),
			           FALSE,
				   errorCode);
   if( *errorCode != 0)  return FALSE;

   pres =  ApiQuantity_getValue((ApiQuantity*)request->altPres,
				ApiUnit_Pressure_BAR(),
				FALSE,
				errorCode);
   if( *errorCode != 0)  return FALSE;
   result =  compareNumber_met(buffer, bufLength, "ctplD11171bar",
			       ctplD11171bar, comp,900, 50, pres, 15,
			       errorCode);
   if( result == FALSE) return result;
   result =  compareNumber_met(buffer, bufLength, "densD11171bar",
			       densD11171bar, compDens,900, 50, pres, 15,
			       errorCode);
   if( result == FALSE) return result;
   compFp = ApiQuantity_getValue((ApiQuantity*)request->scaledCompAlt,
		 	         ApiUnit_ScaledComp_REV_BAR(),
				 FALSE,
				 errorCode);
   if( *errorCode != 0)  return FALSE;
   result =  compareNumber_met(buffer, bufLength, "fpD11171bar",
			       fpD11171bar, compFp,900, 50, pres, 15,
			       errorCode);
   if( result == FALSE) return result;


   pres =  ApiQuantity_getValue((ApiQuantity*)request->altPres,
				ApiUnit_Pressure_KPA(),
				FALSE,
				errorCode);
   if( *errorCode != 0)  return FALSE;
   result =  compareNumber_met(buffer, bufLength, "ctplD11171kPa",
                               ctplD11171kPa, comp,900, 50, pres,15,
			        errorCode);
   if( result == FALSE) return result;
   result =  compareNumber_met(buffer, bufLength, "densD11171kPa",
                               densD11171kPa, compDens,900, 50, pres,15,
			        errorCode);
   if( result == FALSE) return result;
   compFp = ApiQuantity_getValue((ApiQuantity*)request->scaledCompAlt,
		 	         ApiUnit_ScaledComp_REV_KPA(),
				 FALSE,
				 errorCode);
   if( *errorCode != 0)  return FALSE;
   result =  compareNumber_met(buffer, bufLength, "fpD11171kPa",
                               fpD11171kPa, compFp,900, 50, pres,15,
			        errorCode);
   if( result == FALSE) return result;


   /** And for alph60 supplied */
   *errorCode = ApiCalculation_set11_1_7_1Parameters(request,
					       API_COMMODITY_NOT_GIVEN,
					       0.00056, ApiUnit_ThermalExp_REV_C(),
					       50, ApiUnit_Temperature_C(),
					       15, ApiUnit_Pressure_BAR(),
					       900,
					       ApiUnit_Density_KGM3(),

					       15, ApiUnit_Temperature_C(),
					       0, NULL);
   if( *errorCode != 0)  return FALSE;
   *errorCode = ApiCalculationRequest_performCalculation(request);
   if( *errorCode != 0)  return FALSE;
   comp = ApiQuantity_getValue((ApiQuantity*)request->ctplBaseToAlt,
			       ApiUnit_Expansion_DIMLESS(),
			       TRUE,
			       errorCode);
   if( *errorCode != 0)  return FALSE;
   compDens = ApiQuantity_getValue((ApiQuantity*)request->altDens,
		 	            ApiUnit_Density_KGM3(),
			           FALSE,
				   errorCode);
   if( *errorCode != 0)  return FALSE;
    
   pres =  ApiQuantity_getValue((ApiQuantity*)request->altPres,
				ApiUnit_Pressure_BAR(),
				FALSE,
				errorCode);
   if( *errorCode != 0)  return FALSE;
   result =  compareNumber_alpha60_met(buffer, bufLength, "ctplC11171bar",
                                       ctplC11171bar, comp, 0.00056,900, 50, pres,
				       15,
				       errorCode);
   if( result == FALSE) return result;
   result =  compareNumber_alpha60_met(buffer, bufLength, "densC11171bar",
                                       densC11171bar, compDens, 0.00056,900, 50, pres,
				       15,
				       errorCode);
   if( result == FALSE) return result;
   compFp = ApiQuantity_getValue((ApiQuantity*)request->scaledCompAlt,
		 	         ApiUnit_ScaledComp_REV_BAR(),
				 FALSE,
				 errorCode);
   if( *errorCode != 0)  return FALSE;
   result =  compareNumber_alpha60_met(buffer, bufLength, "fpC11171bar",
                                       fpC11171bar, compFp, 0.00056,900, 50, pres,
				       15,
				       errorCode);
   if( result == FALSE) return result;


   pres =  ApiQuantity_getValue((ApiQuantity*)request->altPres,
				ApiUnit_Pressure_KPA(),
				FALSE,
				errorCode);
   if( *errorCode != 0)  return FALSE;
   result =  compareNumber_alpha60_met(buffer, bufLength, "ctplC11171kPa",
				       ctplC11171kPa, comp,0.00056, 900, 50, pres,
				       15,
				       errorCode);
   if( result == FALSE) return result;
   result =  compareNumber_alpha60_met(buffer, bufLength, "densC11171kPa",
				       densC11171kPa, compDens,0.00056, 900, 50, pres,
				       15,
				       errorCode);
   if( result == FALSE) return result;
   compFp = ApiQuantity_getValue((ApiQuantity*)request->scaledCompAlt,
		 	         ApiUnit_ScaledComp_REV_KPA(),
				 FALSE,
				 errorCode);
   if( *errorCode != 0)  return FALSE;
   result =  compareNumber_alpha60_met(buffer, bufLength, "fpC11171kPa",
				       fpC11171kPa, compFp,0.00056, 900, 50, pres,
				       15,
				       errorCode);
   if( result == FALSE) return result;


   /** Test ctpl functions */
   /* 11.1.6.2 routines  for Crude Oil */
   *errorCode = ApiCalculation_set11_1_6_2Parameters(request,
						 API_CRUDE_OIL_NAME,
						 0, NULL,
						 85.0, ApiUnit_Temperature_F(),
						 247.3, ApiUnit_Pressure_PSI(),
						 0.7943,
						 ApiUnit_Density_RELATIVE(),
						 0, NULL);
   if( *errorCode != 0)  return FALSE;
   *errorCode = ApiCalculationRequest_performCalculation(request);
   if( *errorCode != 0)  return FALSE;
   comp = ApiQuantity_getValue((ApiQuantity*)request->ctplObToBase,
			       ApiUnit_Expansion_DIMLESS(),
			       TRUE,
			       errorCode);
   if( *errorCode != 0)  return FALSE;
   compFp = ApiQuantity_getValue((ApiQuantity*)request->scaledCompOb,
		 	         ApiUnit_ScaledComp_REV_PSI(),
				 FALSE,
				 errorCode);
   if( *errorCode != 0)  return FALSE;
    
   density =  ApiQuantity_getValue((ApiQuantity*)request->obDens,
				   ApiUnit_Density_API(),
				   FALSE,
				   errorCode);
   if( *errorCode != 0)  return FALSE;
   result =  compareNumber(buffer, bufLength, "ctplA11162API",
                           ctplA11162API, comp, density, 85.0, 247.3,
			   errorCode);
   if( result == FALSE) return result;
   compDens =  ApiQuantity_getValue((ApiQuantity*)request->baseDens,
		 		    ApiUnit_Density_API(),
				    FALSE,
				    errorCode);
   if( *errorCode != 0)  return FALSE;
   result =  compareNumber(buffer, bufLength, "densA11162API",
                           densA11162API, compDens, density, 85.0, 247.3,
			   errorCode);
   if( result == FALSE) return result;
   result =  compareNumber(buffer, bufLength, "fpA11162API",
                           fpA11162API, compFp, density, 85.0, 247.3,
			   errorCode);
   if( result == FALSE) return result;

   density =  ApiQuantity_getValue((ApiQuantity*)request->obDens,
				   ApiUnit_Density_RELATIVE(),
				   FALSE,
				   errorCode);
   if( *errorCode != 0)  return FALSE;
   result =  compareNumber(buffer, bufLength, "ctplA11162RelDens",
                           ctplA11162RelDens, comp, density, 85.0, 247.3,
			   errorCode);
   if( result == FALSE) return result;
   compDens =  ApiQuantity_getValue((ApiQuantity*)request->baseDens,
				    ApiUnit_Density_RELATIVE(),
				    FALSE,
				    errorCode);
   if( *errorCode != 0)  return FALSE;
   result =  compareNumber(buffer, bufLength, "densA11162RelDens",
                           densA11162RelDens, compDens, density, 85.0, 247.3,
			   errorCode);
   if( result == FALSE) return result;
   result =  compareNumber(buffer, bufLength, "fpA11162RelDens",
                           fpA11162RelDens, compFp, density, 85.0, 247.3,
			   errorCode);
   if( result == FALSE) return result;


   /* 11.1.6.2 routines  for Refined Products */
   *errorCode = ApiCalculation_set11_1_6_2Parameters(request,
						 API_REFINED_PRODUCTS_NAME,
						 0, NULL,
						 85.0, ApiUnit_Temperature_F(),
						 247.3, ApiUnit_Pressure_PSI(),
						 0.7943,
						 ApiUnit_Density_RELATIVE(),
						 0, NULL);
   if( *errorCode != 0)  return FALSE;
   *errorCode = ApiCalculationRequest_performCalculation(request);
   if( *errorCode != 0)  return FALSE;
   comp = ApiQuantity_getValue((ApiQuantity*)request->ctplObToBase,
			       ApiUnit_Expansion_DIMLESS(),
			       TRUE,
			       errorCode);
   if( *errorCode != 0)  return FALSE;
   compFp = ApiQuantity_getValue((ApiQuantity*)request->scaledCompOb,
		 	         ApiUnit_ScaledComp_REV_PSI(),
				 FALSE,
				 errorCode);
   if( *errorCode != 0)  return FALSE;
     
   density =  ApiQuantity_getValue((ApiQuantity*)request->obDens,
				   ApiUnit_Density_API(),
				   FALSE,
				   errorCode);
   if( *errorCode != 0)  return FALSE;
   result =  compareNumber(buffer, bufLength, "ctplB11162API",
                           ctplB11162API, comp, density, 85.0, 247.3,
			   errorCode);
   if( result == FALSE) return result;
   compDens =  ApiQuantity_getValue((ApiQuantity*)request->baseDens,
		 		    ApiUnit_Density_API(),
				    FALSE,
				    errorCode);
   if( *errorCode != 0)  return FALSE;
   result =  compareNumber(buffer, bufLength, "densB11162API",
                           densB11162API, compDens, density, 85.0, 247.3,
			   errorCode);
   if( result == FALSE) return result;
   result =  compareNumber(buffer, bufLength, "fpB11162API",
                           fpB11162API, compFp, density, 85.0, 247.3,
			   errorCode);
   if( result == FALSE) return result;

   density =  ApiQuantity_getValue((ApiQuantity*)request->obDens,
				   ApiUnit_Density_RELATIVE(),
				   FALSE,
				   errorCode);
   if( *errorCode != 0)  return FALSE;
   result =  compareNumber(buffer, bufLength, "ctplB11162RelDens",
                           ctplB11162RelDens, comp, density, 85.0, 247.3,
			   errorCode);
   if( result == FALSE) return result;
   compDens =  ApiQuantity_getValue((ApiQuantity*)request->baseDens,
		 		    ApiUnit_Density_RELATIVE(),
				    FALSE,
				    errorCode);
   if( *errorCode != 0)  return FALSE;
   result =  compareNumber(buffer, bufLength, "densB11162RelDens",
                           densB11162RelDens, compDens, density, 85.0, 247.3,
			   errorCode);
   if( result == FALSE) return result;
   result =  compareNumber(buffer, bufLength, "fpB11162RelDens",
                           fpB11162RelDens, compFp, density, 85.0, 247.3,
			   errorCode);
   if( result == FALSE) return result;


   /* 11.1.6.2 routines  for Lubrication Oil */
   *errorCode = ApiCalculation_set11_1_6_2Parameters(request,
						 API_LUBRICATING_OIL_NAME,
						 0, NULL,
						 85.0, ApiUnit_Temperature_F(),
						 247.3, ApiUnit_Pressure_PSI(),
						 42.9, ApiUnit_Density_API(),
						 0, NULL);
   if( *errorCode != 0)  return FALSE;
   *errorCode = ApiCalculationRequest_performCalculation(request);
   if( *errorCode != 0)  return FALSE;
   comp = ApiQuantity_getValue((ApiQuantity*)request->ctplObToBase,
			       ApiUnit_Expansion_DIMLESS(),
			       TRUE,
			       errorCode);
   if( *errorCode != 0)  return FALSE;
   compFp = ApiQuantity_getValue((ApiQuantity*)request->scaledCompOb,
		 	         ApiUnit_ScaledComp_REV_PSI(),
				 FALSE,
				 errorCode);
   if( *errorCode != 0)  return FALSE;
     
   density =  ApiQuantity_getValue((ApiQuantity*)request->obDens,
				   ApiUnit_Density_API(),
				   FALSE,
				   errorCode);
   if( *errorCode != 0)  return FALSE;
   result =  compareNumber(buffer, bufLength, "ctplD11162API",
                           ctplD11162API, comp, density, 85.0, 247.3,
			   errorCode);
   if( result == FALSE) return result;
   compDens =  ApiQuantity_getValue((ApiQuantity*)request->baseDens,
		 		    ApiUnit_Density_API(),
				    FALSE,
				    errorCode);
   if( *errorCode != 0)  return FALSE;
   result =  compareNumber(buffer, bufLength, "densD11162API",
                           densD11162API, compDens, density, 85.0, 247.3,
			   errorCode);
   if( result == FALSE) return result;
   result =  compareNumber(buffer, bufLength, "fpD11162API",
                           fpD11162API, compFp, density, 85.0, 247.3,
			   errorCode);
   if( result == FALSE) return result;

   density =  ApiQuantity_getValue((ApiQuantity*)request->obDens,
				   ApiUnit_Density_RELATIVE(),
				   FALSE,
				   errorCode);
   if( *errorCode != 0)  return FALSE;
   result =  compareNumber(buffer, bufLength, "ctplD11162RelDens",
                           ctplD11162RelDens, comp, density, 85.0, 247.3,
			   errorCode);
   if( result == FALSE) return result;
   compDens =  ApiQuantity_getValue((ApiQuantity*)request->baseDens,
				    ApiUnit_Density_RELATIVE(),
				    FALSE,
				    errorCode);
   if( *errorCode != 0)  return FALSE;
   result =  compareNumber(buffer, bufLength, "denslD11162RelDens",
                           densD11162RelDens, compDens, density, 85.0, 247.3,
			   errorCode);
   if( result == FALSE) return result;
   result =  compareNumber(buffer, bufLength, "fpD11162RelDens",
                           fpD11162RelDens, compFp, density, 85.0, 247.3,
			   errorCode);
   if( result == FALSE) return result;


   /** The 11.1.6.2 functions for  alpha60 values */
   *errorCode = ApiCalculation_set11_1_6_2Parameters(request,
						 API_COMMODITY_NOT_GIVEN,
						 0.00056, ApiUnit_ThermalExp_REV_F(),
						 85.0, ApiUnit_Temperature_F(),
						 247.3, ApiUnit_Pressure_PSI(),
						 0.7943,
						 ApiUnit_Density_RELATIVE(),
						 0, NULL);
   if( *errorCode != 0)  return FALSE;
   *errorCode = ApiCalculationRequest_performCalculation(request);
   if( *errorCode != 0)  return FALSE;
   comp = ApiQuantity_getValue((ApiQuantity*)request->ctplObToBase,
			       ApiUnit_Expansion_DIMLESS(),
			       TRUE,
			       errorCode);
   if( *errorCode != 0)  return FALSE;
   compFp = ApiQuantity_getValue((ApiQuantity*)request->scaledCompOb,
		 	         ApiUnit_ScaledComp_REV_PSI(),
				 FALSE,
				 errorCode);
   if( *errorCode != 0)  return FALSE;
    
   density =  ApiQuantity_getValue((ApiQuantity*)request->obDens,
				   ApiUnit_Density_API(),
				   FALSE,
				   errorCode);
   if( *errorCode != 0)  return FALSE;
   result =  compareNumber_alpha60(buffer, bufLength, "ctplC11162API",
                                   ctplC11162API, comp, 
                                   0.00056, density, 85.0, 247.3,
	  		           errorCode);
   if( result == FALSE) return result;
   compDens =  ApiQuantity_getValue((ApiQuantity*)request->baseDens,
		 		    ApiUnit_Density_API(),
				    FALSE,
				    errorCode);
   if( *errorCode != 0)  return FALSE;
   result =  compareNumber_alpha60(buffer, bufLength, "densC11162API",
                                   densC11162API, compDens, 
                                   0.00056, density, 85.0, 247.3,
	  		           errorCode);
   if( result == FALSE) return result;
   result =  compareNumber_alpha60(buffer, bufLength, "fpC11162API",
                                   fpC11162API, compFp, 
                                   0.00056, density, 85.0, 247.3,
	  		           errorCode);
   if( result == FALSE) return result;
   
   density =  ApiQuantity_getValue((ApiQuantity*)request->obDens,
				   ApiUnit_Density_RELATIVE(),
				   FALSE,
				   errorCode);
   if( *errorCode != 0)  return FALSE;
   result =  compareNumber_alpha60(buffer, bufLength, "ctplC11162RelDens",
                                   ctplC11162RelDens, comp, 
                                   0.00056, density, 85.0, 247.3,
			           errorCode);
   if( result == FALSE) return result;
   compDens =  ApiQuantity_getValue((ApiQuantity*)request->baseDens,
		 	 	    ApiUnit_Density_RELATIVE(),
				    FALSE,
				    errorCode);
   if( *errorCode != 0)  return FALSE;
   result =  compareNumber_alpha60(buffer, bufLength, "densC11162RelDens",
                                   densC11162RelDens, compDens, 
                                   0.00056, density, 85.0, 247.3,
			           errorCode);
   if( result == FALSE) return result;
   result =  compareNumber_alpha60(buffer, bufLength, "fpC11162RelDens",
                                   fpC11162RelDens, compFp, 
                                   0.00056, density, 85.0, 247.3,
			           errorCode);
   if( result == FALSE) return result;

   /** Test ctpl functions */
   /* 11.1.7.2 routines  for Crude Oil */
   *errorCode = ApiCalculation_set11_1_7_2Parameters(request,
						   API_CRUDE_OIL_NAME,
						   0, NULL,
						   -32.8, ApiUnit_Temperature_C(),
						   24.6, ApiUnit_Pressure_BAR(),
						   772.3,
						   ApiUnit_Density_KGM3(),

						   15.0, ApiUnit_Temperature_C(),
						   0, NULL);
   if( *errorCode != 0)  return FALSE;
   *errorCode = ApiCalculationRequest_performCalculation(request);
   if( *errorCode != 0)  return FALSE;
   comp = ApiQuantity_getValue((ApiQuantity*)request->ctplObToBase,
			       ApiUnit_Expansion_DIMLESS(),
			       TRUE,
			       errorCode);
   if( *errorCode != 0)  return FALSE;
   compDens =  ApiQuantity_getValue((ApiQuantity*)request->baseDens,
		 	 	    ApiUnit_Density_KGM3(),
				    FALSE,
				    errorCode);
   if( *errorCode != 0)  return FALSE;
    
   pres =  ApiQuantity_getValue((ApiQuantity*)request->obPres,
				ApiUnit_Pressure_BAR(),
				FALSE,
				errorCode);
   if( *errorCode != 0)  return FALSE;
   result =  compareNumber_met(buffer, bufLength, "ctplA11172bar",
                               ctplA11172bar, comp,772.3, -32.8, pres,15,
			       errorCode);
   if( result == FALSE) return result;
   result =  compareNumber_met(buffer, bufLength, "densA11172bar",
                               densA11172bar, compDens,772.3, -32.8, pres,15,
			       errorCode);
   if( result == FALSE) return result;
   compFp = ApiQuantity_getValue((ApiQuantity*)request->scaledCompOb,
		 	         ApiUnit_ScaledComp_REV_BAR(),
				 FALSE,
				 errorCode);
   if( *errorCode != 0)  return FALSE;
   result =  compareNumber_met(buffer, bufLength, "fpA11172bar",
                               fpA11172bar, compFp,772.3, -32.8, pres,15,
			       errorCode);
   if( result == FALSE) return result;


   pres =  ApiQuantity_getValue((ApiQuantity*)request->obPres,
				ApiUnit_Pressure_KPA(),
				FALSE,
				errorCode);
   if( *errorCode != 0)  return FALSE;
   result =  compareNumber_met(buffer, bufLength, "ctplA11172kPa",
                               ctplA11172kPa, comp,772.3, -32.8, pres, 15,
			       errorCode);
   if( result == FALSE) return result;
   result =  compareNumber_met(buffer, bufLength, "densA11172kPa",
                               densA11172kPa, compDens,772.3, -32.8, pres, 15,
			       errorCode);
   if( result == FALSE) return result;
   compFp = ApiQuantity_getValue((ApiQuantity*)request->scaledCompOb,
		 	         ApiUnit_ScaledComp_REV_KPA(),
				 FALSE,
				 errorCode);
   if( *errorCode != 0)  return FALSE;
   result =  compareNumber_met(buffer, bufLength, "foA11172kPa",
                               fpA11172kPa, compFp,772.3, -32.8, pres, 15,
			       errorCode);
   if( result == FALSE) return result;
   

   /* 11.1.7.2 routines  for Refined Products */
   *errorCode = ApiCalculation_set11_1_7_2Parameters(request,
						   API_REFINED_PRODUCTS_NAME,
						   0, NULL,
						   -32.8, ApiUnit_Temperature_C(),
						   24.6, ApiUnit_Pressure_BAR(),
						   772.3,
						   ApiUnit_Density_KGM3(),

						   15.0, ApiUnit_Temperature_C(),
						   0, NULL);
   if( *errorCode != 0)  return FALSE;
   *errorCode = ApiCalculationRequest_performCalculation(request);
   if( *errorCode != 0)  return FALSE;
   comp = ApiQuantity_getValue((ApiQuantity*)request->ctplObToBase,
			       ApiUnit_Expansion_DIMLESS(),
			       TRUE,
			       errorCode);
   if( *errorCode != 0)  return FALSE;
   compDens =  ApiQuantity_getValue((ApiQuantity*)request->baseDens,
		 	 	    ApiUnit_Density_KGM3(),
				    FALSE,
				    errorCode);
   if( *errorCode != 0)  return FALSE;
    
   pres =  ApiQuantity_getValue((ApiQuantity*)request->obPres,
				ApiUnit_Pressure_BAR(),
				FALSE,
				errorCode);
   if( *errorCode != 0)  return FALSE;
   result =  compareNumber_met(buffer, bufLength, "ctplB11172bar",
			       ctplB11172bar, comp,772.3, -32.8, pres,15,
			       errorCode);
   if( result == FALSE) return result;
   result =  compareNumber_met(buffer, bufLength, "densB11172bar",
			       densB11172bar, compDens,772.3, -32.8, pres,15,
			       errorCode);
   if( result == FALSE) return result;
   compFp = ApiQuantity_getValue((ApiQuantity*)request->scaledCompOb,
		 	         ApiUnit_ScaledComp_REV_BAR(),
				 FALSE,
				 errorCode);
   if( *errorCode != 0)  return FALSE;
   result =  compareNumber_met(buffer, bufLength, "fpB11172bar",
			       fpB11172bar, compFp,772.3, -32.8, pres,15,
			       errorCode);
   if( result == FALSE) return result;


   pres =  ApiQuantity_getValue((ApiQuantity*)request->obPres,
				ApiUnit_Pressure_KPA(),
				FALSE,
				errorCode);
   if( *errorCode != 0)  return FALSE;
   result =  compareNumber_met(buffer, bufLength, "ctplB11172kPa",
                               ctplB11172kPa, comp,772.3, -32.8, pres,15,
			       errorCode);
   if( result == FALSE) return result;
   result =  compareNumber_met(buffer, bufLength, "densB11172kPa",
                               densB11172kPa, compDens,772.3, -32.8, pres,15,
			       errorCode);
   if( result == FALSE) return result;
   compFp = ApiQuantity_getValue((ApiQuantity*)request->scaledCompOb,
		 	         ApiUnit_ScaledComp_REV_KPA(),
				 FALSE,
				 errorCode);
   if( *errorCode != 0)  return FALSE;
   result =  compareNumber_met(buffer, bufLength, "fpB11172kPa",
                               fpB11172kPa, compFp,772.3, -32.8, pres,15,
			       errorCode);
   if( result == FALSE) return result;
   

   /* 11.1.7.2 routines  for Lubrication oil */
   *errorCode = ApiCalculation_set11_1_7_2Parameters(request,
						   API_LUBRICATING_OIL_NAME,
						   0, NULL,
						   50, ApiUnit_Temperature_C(),
						   15, ApiUnit_Pressure_BAR(),
						   900,
						   ApiUnit_Density_KGM3(),

						   15.0, ApiUnit_Temperature_C(),
						   0, NULL);
   if( *errorCode != 0)  return FALSE;
   *errorCode = ApiCalculationRequest_performCalculation(request);
   if( *errorCode != 0)  return FALSE;
   comp = ApiQuantity_getValue((ApiQuantity*)request->ctplObToBase,
			       ApiUnit_Expansion_DIMLESS(),
			       TRUE,
			       errorCode);
   if( *errorCode != 0)  return FALSE;
   compDens =  ApiQuantity_getValue((ApiQuantity*)request->baseDens,
		 	 	    ApiUnit_Density_KGM3(),
				    FALSE,
				    errorCode);
   if( *errorCode != 0)  return FALSE;

   pres =  ApiQuantity_getValue((ApiQuantity*)request->obPres,
				ApiUnit_Pressure_BAR(),
				FALSE,
				errorCode);
   if( *errorCode != 0)  return FALSE;
   result =  compareNumber_met(buffer, bufLength, "ctplD11172bar",
			       ctplD11172bar, comp,900, 50, pres, 15,
			       errorCode);
   if( result == FALSE) return result;
   result =  compareNumber_met(buffer, bufLength, "densD11172bar",
			       densD11172bar, compDens,900, 50, pres, 15,
			       errorCode);
   if( result == FALSE) return result;
   compFp = ApiQuantity_getValue((ApiQuantity*)request->scaledCompOb,
		 	         ApiUnit_ScaledComp_REV_BAR(),
				 FALSE,
				 errorCode);
   if( *errorCode != 0)  return FALSE;
   result =  compareNumber_met(buffer, bufLength, "fpD11172bar",
			       fpD11172bar, compFp,900, 50, pres, 15,
			       errorCode);
   if( result == FALSE) return result;

   pres =  ApiQuantity_getValue((ApiQuantity*)request->obPres,
				ApiUnit_Pressure_KPA(),
				FALSE,
				errorCode);
   if( *errorCode != 0)  return FALSE;
   result =  compareNumber_met(buffer, bufLength, "ctplD11172kPa",
                               ctplD11172kPa, comp,900, 50, pres,15,
			        errorCode);
   if( result == FALSE) return result;
   result =  compareNumber_met(buffer, bufLength, "densD11172kPa",
                               densD11172kPa, compDens,900, 50, pres,15,
			        errorCode);
   if( result == FALSE) return result;
   compFp = ApiQuantity_getValue((ApiQuantity*)request->scaledCompOb,
		 	         ApiUnit_ScaledComp_REV_KPA(),
				 FALSE,
				 errorCode);
   if( *errorCode != 0)  return FALSE;
   result =  compareNumber_met(buffer, bufLength, "fpD11172kPa",
                               fpD11172kPa, compFp,900, 50, pres,15,
			       errorCode);
   if( result == FALSE) return result;


   /** And for alph60 supplied */
   *errorCode = ApiCalculation_set11_1_7_2Parameters(request,
					       API_COMMODITY_NOT_GIVEN,
					       0.00056, ApiUnit_ThermalExp_REV_C(),
					       50, ApiUnit_Temperature_C(),
					       15, ApiUnit_Pressure_BAR(),
					       900,
					       ApiUnit_Density_KGM3(),

					       15, ApiUnit_Temperature_C(),
					       0, NULL);
   if( *errorCode != 0)  return FALSE;
   *errorCode = ApiCalculationRequest_performCalculation(request);
   if( *errorCode != 0)  return FALSE;
   comp = ApiQuantity_getValue((ApiQuantity*)request->ctplObToBase,
			       ApiUnit_Expansion_DIMLESS(),
			       TRUE,
			       errorCode);
   if( *errorCode != 0)  return FALSE;
   compDens =  ApiQuantity_getValue((ApiQuantity*)request->baseDens,
		 	 	    ApiUnit_Density_KGM3(),
				    FALSE,
				    errorCode);
   if( *errorCode != 0)  return FALSE;
    
   pres =  ApiQuantity_getValue((ApiQuantity*)request->obPres,
				ApiUnit_Pressure_BAR(),
				FALSE,
				errorCode);
   if( *errorCode != 0)  return FALSE;
   result =  compareNumber_alpha60_met(buffer, bufLength, "ctplC11172bar",
                                       ctplC11172bar, comp, 0.00056,900, 50, pres,
				       15,
				       errorCode);
   if( result == FALSE) return result;
   result =  compareNumber_alpha60_met(buffer, bufLength, "densC11172bar",
                                       densC11172bar, compDens, 0.00056,900, 50, pres,
				       15,
				       errorCode);
   if( result == FALSE) return result;
   compFp = ApiQuantity_getValue((ApiQuantity*)request->scaledCompOb,
		 	         ApiUnit_ScaledComp_REV_BAR(),
				 FALSE,
				 errorCode);
   if( *errorCode != 0)  return FALSE;
   result =  compareNumber_alpha60_met(buffer, bufLength, "fpC11172bar",
                                       fpC11172bar, compFp, 0.00056,900, 50, pres,
				       15,
				       errorCode);
   if( result == FALSE) return result;


   pres =  ApiQuantity_getValue((ApiQuantity*)request->obPres,
				ApiUnit_Pressure_KPA(),
				FALSE,
				errorCode);
   if( *errorCode != 0)  return FALSE;
   result =  compareNumber_alpha60_met(buffer, bufLength, "ctplC11172kPa",
				       ctplC11172kPa, comp,0.00056, 900, 50, pres,
				       15,
				       errorCode);
   if( result == FALSE) return result;
   result =  compareNumber_alpha60_met(buffer, bufLength, "densC11172kPa",
				       densC11172kPa, compDens,0.00056, 900, 50, pres,
				       15,
				       errorCode);
   if( result == FALSE) return result;
   compFp = ApiQuantity_getValue((ApiQuantity*)request->scaledCompOb,
		 	         ApiUnit_ScaledComp_REV_KPA(),
				 FALSE,
				 errorCode);
   if( *errorCode != 0)  return FALSE;
   result =  compareNumber_alpha60_met(buffer, bufLength, "fpC11172kPa",
				       fpC11172kPa, compFp,0.00056, 900, 50, pres,
				       15,
				       errorCode);
   if( result == FALSE) return result;


   return TRUE;
};




/**  
* A test routine that checks that all functions are defined correctly
* 
* @param buffer a buffer in which to report errors
* @param bufLength the length of the buffer
* @param errorCode a variable in which to return errors if any
*
* @return TRUE if all test run successfully, FALSE otherwise
*/
bool VBAExport_RoutineTest(char * buffer, int bufLength, int * errorCode){
  bool result = TRUE;
  
  result = VBAExport_RoutineTestctplBase(buffer,bufLength,errorCode);
  if( result == FALSE) return result;

  return result;
}


int main1(int argv, char **arg)  {
  int len = 5000;
  char buffer[len];
  int errorCode = 0;
  bool result;

  errorCode = ApiCalculationRequest_initialize();
  if( errorCode != 0){
    buffer[0] = '\0';
    ApiCalculationRequest_appendBuffer(buffer, len,"Initialization Error: ");
    ApiCalculation_getErrorDescription(errorCode, buffer, len);
    printf(" %s %s",buffer,API_EOL);
    return -1;
  }
  buffer[0] ='\0';
  result =  ApiCalculationTest_runTests(NULL, 0,  buffer, len, &errorCode);
  printf(" %s %s",buffer,API_EOL);
  if( errorCode != 0){
    buffer[0] ='\0';
    ApiCalculationRequest_appendBuffer(buffer, len,"Calculation Error occured: ");
    ApiCalculation_getErrorDescription(errorCode, buffer, len);
    printf(" %s %s",buffer,API_EOL);
  }
  if( result == TRUE) printf("All test cases run without error %s",API_EOL);
  else                printf("TESTS FAILED %s",API_EOL);

  printf("Checking VBAExport Functions \n");
  buffer[0] = '\0';
  result =  VBAExport_RoutineTest(buffer, len, &errorCode);
  if( errorCode != 0){
     ApiCalculationRequest_appendBuffer(buffer, len,API_EOL);
     ApiCalculation_getErrorDescription(errorCode, buffer, len);
     printf(" VBA Export routine error: %s\n",buffer);
  }
  if( result == TRUE) printf("All test cases run without error %s",API_EOL);
  else                printf("TESTS FAILED \n");

  return -1 * errorCode;
}

