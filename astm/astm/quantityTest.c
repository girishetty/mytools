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


#include "quantityTest.h"
#include "calculation.h"
#include <string.h>
#include <math.h>

/** 
* The generic functions that get run by all 
* quantity tests. They are only defined in this file
*/

/** Type definitions for the test structure  */
typedef struct api_quantity_test ApiQuantityTest; 

/**
* The structure that holds the data to test 
*/
struct api_quantity_test{
  ApiUnit **units;              /** The available units */
  int unitMax;                  /** The number of available units */
  double **convertTest;         /** The numbers to check for conversion */
                                /** The second array is unitMax of length */ 
  int convertMax;               /** The length of the first array parameter */ 
  double **roundTest;           /** The numbers to check for rounding */
                                /** The second array is unitMax of length */ 
  int roundMax;                 /** The length of the first array paramete */
  double **roundResult;         /** The expected rounding result */
  ApiUnit *wrongUnit;           /** A wrong unit */
  ApiQuantity *compareQuant;    /** A quantity used to compare data */
};


bool  ApiQuantityTest_convertValue( ApiQuantity * quant,
				    double * value,
				    ApiUnit **units,
				    int maxUnit,
				    char * buffer,
				    int len,
				    int * errorCode){
  bool result = TRUE;
  int i;
  double calc;
  char tmp[100];

  *errorCode = 0;

  for( i = 0; i < maxUnit; i++){
    tmp[0] = '\0';
    ApiUnit_unitName(   units[i], tmp,  100);
    calc = ApiQuantity_getValue( quant, units[i], FALSE, errorCode);
    if( *errorCode != 0){
        result = FALSE;
	ApiCalculationRequest_appendBuffer(buffer,len, API_EOL);
        ApiCalculationRequest_appendBuffer(buffer, 
					   len,
				     "ApiQuantity_getValue failed");
    }       
    if( fabs( calc - value[i]) >= API_EPSILON ){
      result = FALSE;
      ApiCalculationRequest_appendBuffer(buffer,len, API_EOL);
      ApiCalculationRequest_appendBuffer(buffer,len," For unit ");
      ApiUnit_unitName( units[i], buffer, len);
      ApiCalculationRequest_appendBuffer(buffer,len," conversion failed. Expected");
      ApiCalculationRequest_appendDouble(buffer,len,value[i]);
      ApiCalculationRequest_appendBuffer(buffer,len," got ");
      ApiCalculationRequest_appendDouble(buffer,len,calc);

      ApiCalculationRequest_appendBuffer(buffer,len," orignal is ");
      calc = ApiQuantity_getValue( quant, 
				   ApiQuantity_givenUnit(quant,errorCode), 
                                   FALSE, errorCode);
      ApiCalculationRequest_appendDouble(buffer,len,calc);
      ApiCalculationRequest_appendBuffer(buffer,len," ");
      ApiUnit_unitName(ApiQuantity_givenUnit(quant,errorCode), 
                       buffer, len);

    }
    else result = TRUE;
  }

  return result;
};



/**
* Run a conversion test
*
* @param quant the quantity object to test 
* @param tester the  ApiQuantityTest containing the test data
* @param buffer the buffer to which to add the reason for failure
* @param len the length of the buffer
* @param errorCode an errorCode to set if the test failed 
*
* @return TRUE if the test was successful, FALSE otherwise
*/
bool ApiQuantityTest_testConversion(ApiQuantity * quant,
				    ApiQuantityTest * tester,
				    char *buffer,
				    int len,
				    int * errorCode){
  bool result = TRUE; 
  int i,j, m;
  bool tmp;
  double calc = 0;


  /** Check that all parameters are initialized */
  *errorCode  = 0;
  if( quant == NULL || tester == NULL){
    *errorCode = NULL_POINTER_EXCEPTION;
    result = FALSE;
    return result; 
  }
  if( tester->convertTest == NULL ||
      tester->units == NULL){
    *errorCode = NULL_POINTER_EXCEPTION;
    result = FALSE;
    return result; 
  }

  /** 
   * And run the conversion test
   */
  for( i = 0; i < tester->convertMax; i++){
    for(j = 0; j < tester->unitMax ; j++){
      /** First set the value to  the relevant value and check */
      *errorCode =  ApiQuantity_setValue(quant, 
					 tester->convertTest[i][j],
					 tester->units[j]);
      if( *errorCode != 0) {
           return result;
	   result = FALSE;
      }
      tmp = ApiQuantityTest_convertValue( quant,
					  tester->convertTest[i],
					  tester->units,
					  tester->unitMax,
					  buffer,
					  len,
					  errorCode);
      if( tmp == FALSE){
	result = FALSE;
      }

      /** Calculate the value for a given unit and  rerun conversion check */
      for( m = 0; m < tester->unitMax; m++){
	 calc =  ApiQuantity_getValue( quant, 
                                        tester->units[m], 
                                        FALSE, 
					errorCode);
         if( *errorCode != 0){
             result = FALSE;
	     ApiCalculationRequest_appendBuffer(buffer,len, API_EOL);
             ApiCalculationRequest_appendBuffer(buffer, 
						len,
						"ApiQuantity_getValue failed");
	 }
         *errorCode =  ApiQuantity_setValue(quant, calc, tester->units[m]);
         if( *errorCode != 0) {
	      result = FALSE;
              return result; 
	 }
   
	 tmp = ApiQuantityTest_convertValue( quant,
					     tester->convertTest[i],
					     tester->units,
					     tester->unitMax,
					     buffer,
					     len,
					     errorCode);
	 if( tmp == FALSE){
	   result = FALSE;
	 }

	 /** Check that compareTo works */

	 /** First check for equal */
         *errorCode = ApiQuantity_setValue(tester->compareQuant, 
					   calc,
					   tester->units[m]);
          if( *errorCode != 0) {
	       result = FALSE;
               return result; 
	  }
          if( !ApiQuantity_compareTo( tester->compareQuant, 
				      quant,
				      errorCode) == 0){
	        ApiCalculationRequest_appendBuffer(buffer,len, API_EOL);
		ApiCalculationRequest_appendBuffer(buffer,len," For unit ");
		ApiUnit_unitName( tester->units[m], buffer, len);
		ApiCalculationRequest_appendBuffer(buffer,len," we expected ");
		ApiCalculationRequest_appendDouble(buffer,
					     len,
					     ApiQuantity_getValue(quant,
								  tester->units[m],
								  FALSE,
								  errorCode));
		ApiCalculationRequest_appendBuffer(buffer,len," and ");
		ApiCalculationRequest_appendDouble(buffer,
						   len,
					     ApiQuantity_getValue(tester->compareQuant,
								  tester->units[m],
								  FALSE,
								  errorCode));
		ApiCalculationRequest_appendBuffer(buffer,len," to be equal.");
	  }
          if( *errorCode != 0) {
	       result = FALSE;
               return result; 
	  }
      }

      /** The next is only checked for the first unit */
      if( tester->unitMax > 0){
  	  m  = 0;
          
	  /** Then for smaller than */
          *errorCode = ApiQuantity_setValue(tester->compareQuant, 
					     calc - 2 * API_EPSILON,
					     tester->units[m]);
          if( *errorCode != 0) {
	       result = FALSE;
               return result; 
	  }        
          if( !ApiQuantity_compareTo( tester->compareQuant, 
				      quant,
				      errorCode) < 0){
	        result = FALSE;
	        ApiCalculationRequest_appendBuffer(buffer,len, API_EOL);
		ApiCalculationRequest_appendBuffer(buffer,len," For unit ");
		ApiUnit_unitName( tester->units[m], buffer, len);
		ApiCalculationRequest_appendBuffer(buffer,len," we expected ");
		ApiCalculationRequest_appendDouble(buffer,
					     len,
					     ApiQuantity_getValue(quant,
								  tester->units[m],
								  FALSE,
								  errorCode));
		ApiCalculationRequest_appendBuffer(buffer,len," to be larger than ");
		ApiCalculationRequest_appendDouble(buffer,
					     len,
					     ApiQuantity_getValue(tester->compareQuant,
								  tester->units[m],
								  FALSE,
								  errorCode));
	  }
          if( *errorCode != 0) {
	       result = FALSE;
               return result; 
	  }
     


	  /** Then for larger than */
	  *errorCode = ApiQuantity_setValue(tester->compareQuant, 
					     calc + 2 * API_EPSILON,
					     tester->units[m]);
          if( *errorCode != 0) {
	       result = FALSE;
               return result;
	  }        
          if( !ApiQuantity_compareTo( tester->compareQuant, 
				      quant,
				      errorCode) > 0){	
 	        result = FALSE;
	        ApiCalculationRequest_appendBuffer(buffer,len, API_EOL);
		ApiCalculationRequest_appendBuffer(buffer,len," For unit ");
		ApiUnit_unitName( tester->units[m], buffer, len);
		ApiCalculationRequest_appendBuffer(buffer,len," we expected ");
		ApiCalculationRequest_appendDouble(buffer,
					     len,
					     ApiQuantity_getValue(quant,
								  tester->units[m],
								  FALSE,
								  errorCode));
		ApiCalculationRequest_appendBuffer(buffer,len," to be smaller than ");
		ApiCalculationRequest_appendDouble(buffer,
					     len,
					     ApiQuantity_getValue(tester->compareQuant,
								  tester->units[m],
								  FALSE,
								  errorCode));
	  }
          if( *errorCode != 0) {
	       result = FALSE;
               return result; 
	  }


      }
    }
  } 
 
  return result;
};

/**
* Run a rounding test
*
* @param quant the quantity object to test 
* @param tester the  ApiQuantityTest containing the test data
* @param buffer the buffer to which to add the reason for failure
* @param len the length of the buffer
* @param errorCode an errorCode to set if the test failed 
*
* @return TRUE if the test was successful, FALSE otherwise
*/
bool ApiQuantityTest_testRounding(ApiQuantity * quant,
				  ApiQuantityTest * tester,
				  char *buffer,
				  int len,
				  int * errorCode){

  bool result = TRUE;
  int i,j;
  double calc;


  /** Check that all parameters are initialized */
  *errorCode  = 0;
  if( quant == NULL || tester == NULL){
    *errorCode = NULL_POINTER_EXCEPTION;
    result = FALSE;
    return result; 
  }
  if( tester->roundTest == NULL ||
      tester->roundResult == NULL ||
      tester->units == NULL){
    *errorCode = NULL_POINTER_EXCEPTION;
    result = FALSE;
    return result; 
  }

  /** 
   * And run the rounding tests
   */
  for( i = 0; i < tester->roundMax; i++){
    for(j = 0; j < tester->unitMax; j++){
      *errorCode =  ApiQuantity_setValue(quant, 
					 tester->roundTest[i][j],
					 tester->units[j]);
      if( *errorCode != 0) {
           return result;
	   result = FALSE;
      }
      calc =  ApiQuantity_getValue( quant,
				    tester->units[j], 
				    TRUE,
				    errorCode);
      if( *errorCode != 0){
	result = FALSE;
	ApiCalculationRequest_appendBuffer(buffer, len, API_EOL);
	ApiCalculationRequest_appendBuffer(buffer,
					   len,
					   "Can't retrieve the rounded value");
      }
      if(  fabs( calc - tester->roundResult[i][j] ) >= API_EPSILON ){
	result = FALSE;
	ApiCalculationRequest_appendBuffer(buffer,len, API_EOL);
	ApiCalculationRequest_appendBuffer(buffer,len," Rounding failed for ");
	ApiUnit_unitName(tester->units[j], buffer, len);
	ApiCalculationRequest_appendBuffer(buffer,len," we expected ");
	ApiCalculationRequest_appendDouble(buffer, len,tester->roundResult[i][j]);
	ApiCalculationRequest_appendBuffer(buffer,len," but got ");
	ApiCalculationRequest_appendDouble(buffer, len,calc);
      }
      else result = TRUE;
    }
  }

  return result;
};

/**
* Run the standard quantity test cases
*
* @param quant the quantity object to test 
* @param tester the  ApiQuantityTest containing the test data
* @param buffer the buffer to which to add the reason for failure
* @param len the length of the buffer
* @param errorCode an errorCode to set if the test failed 
*
* @return TRUE if the test was successful, FALSE otherwise
*/
bool ApiQuantityTest_testCases(ApiQuantity * quant,
			       ApiQuantityTest * tester,
			       char *buffer,
			       int len,
			       int * errorCode){
  bool result = TRUE;
  bool tmp;

  tmp = ApiQuantityTest_testConversion(quant,
					  tester,
					  buffer,
					  len,
					  errorCode);
  if( tmp == FALSE || *errorCode != 0){
    result = FALSE;
  } 

  tmp = ApiQuantityTest_testRounding(quant,
				     tester,
				     buffer,
				     len,
				     errorCode);
  if( tmp == FALSE || *errorCode != 0){
    result = FALSE;
  }

  /** Check that wrong units are correctly rejected */
  *errorCode = ApiQuantity_setValue(quant, 0, NULL);
  if( *errorCode == 0){
     result = FALSE;
     ApiCalculationRequest_appendBuffer(buffer,len,API_EOL);
     ApiCalculationRequest_appendBuffer(buffer,
					len,
				  "A null ApiUnit is allowed in ApiQuantity_setValue");
  }
  else *errorCode = 0;
  ApiQuantity_getValue( quant, NULL, FALSE, errorCode);
  if( *errorCode == 0){
     result = FALSE;
     ApiCalculationRequest_appendBuffer(buffer,len,API_EOL);
     ApiCalculationRequest_appendBuffer(buffer,
					len,
				  "A null ApiUnit is allowed in ApiQuantity_getValue");
  }
  else *errorCode = 0;

  *errorCode = ApiQuantity_setValue(quant,0, tester->wrongUnit);
  if( *errorCode == 0){
     result = FALSE;
     ApiCalculationRequest_appendBuffer(buffer,len,API_EOL);
     ApiCalculationRequest_appendBuffer(buffer,
					len,
				  "A wrong ApiUnit is allowed in ApiQuantity_setValue");
  }
  else *errorCode = 0;
  ApiQuantity_getValue( quant, tester->wrongUnit, FALSE, errorCode);
  if( *errorCode == 0){
     result = FALSE;
     ApiCalculationRequest_appendBuffer(buffer,len,API_EOL);
     ApiCalculationRequest_appendBuffer(buffer,
					len,
				  "A wrong ApiUnit is allowed in ApiQuantity_getValue");
  }
  else *errorCode = 0;



  return result;
};

/* -------------------------------------------------------------- */
/*  Tests for the specific quantities                             */
/* -------------------------------------------------------------- */

/**
* The Temperature tests 
*/

/**
* Run the  test that checks whether IPTS90 to IPTS68 works correctly
*
* @param quant the quantity object to test 
* @param buffer the buffer to which to add the reason for failure
* @param len the length of the buffer
* @param errorCode an errorCode to set if the test failed 
*
* @return TRUE if the test was successful, FALSE otherwise
*/
bool ApiQuantityTest_testIPTSConv(ApiTemperature * quant,
				  char *buffer,
				  int len,
				  int * errorCode){
  bool result = TRUE;   
  double adjust1[2] = {-27.7, -27.712499233089};
  double adjust2[2] = {301.93, 301.993163042978};
  double adjust3[2] = {48.04, 48.043878159606};
  double adjust4[2] = { 85.0, 85.013358222928};
  double *adjust[4] = { adjust1, adjust2, adjust3, adjust4};

  int i;
  double calc;

  for( i = 0; i < 4; i++){
       *errorCode = ApiQuantity_setValue((ApiQuantity*)quant, 
					 adjust[i][0],
					 ApiUnit_Temperature_F());
       if( *errorCode != 0){
          result = FALSE;
          ApiCalculationRequest_appendBuffer(buffer,len,API_EOL);
          ApiCalculationRequest_appendBuffer(buffer,
					     len,
				      "Unexpected error for  ApiQuantity_setValue");
	  return result;
       }

       /** Now adjust the value */ 
       ApiTemperature_convertToIPTS68(quant, errorCode);
       if( *errorCode != 0){
          result = FALSE;
          ApiCalculationRequest_appendBuffer(buffer,len,API_EOL);
          ApiCalculationRequest_appendBuffer(buffer,
					     len,
                       "Unexpected error for ApiTemperature_convertToIPTS68 ");
	  return result;
       }

       /** And check the result */
       calc = ApiQuantity_getValue( (ApiQuantity*) quant,
				    ApiUnit_Temperature_F(),
				    FALSE,
				    errorCode);
       if( *errorCode != 0){
          result = FALSE;
          ApiCalculationRequest_appendBuffer(buffer,len,API_EOL);
          ApiCalculationRequest_appendBuffer(buffer,
					     len,
                       "Unexpected error for  ApiQuantity_getValue ");
	  return result;
       }
       if( fabs( calc - adjust[i][1] ) >= API_EPSILON ){
	 result = FALSE;
	 ApiCalculationRequest_appendBuffer(buffer,len, API_EOL);
	 ApiCalculationRequest_appendBuffer(buffer,len," For IPTS68 conversion expected ");
	 ApiCalculationRequest_appendDouble(buffer,len,adjust[i][1]);
	 ApiCalculationRequest_appendBuffer(buffer,len," got ");
	 ApiCalculationRequest_appendDouble(buffer,len,calc);
       }

       /** Check that we cannot readjust the temperature */
       ApiTemperature_convertToIPTS68(quant, errorCode);
       if( *errorCode == 0){
          result = FALSE;
          ApiCalculationRequest_appendBuffer(buffer,len,API_EOL);
          ApiCalculationRequest_appendBuffer(buffer,
					     len,
					     "Temperature can be adjusted twice ");
	  return result;
       }
  }

  return result;
};


bool ApiQuantity_temperatureTest( char *buffer,
				  int len,
				  int * errorCode){
  ApiTemperature *temperature;  
  bool result = TRUE, tmp;


  /* Set the conversion values */
  double convert1[2] = {50.0, 10.0};
  double convert2[2] = {392.9, 200.5};
  double convert3[2] = {-237.28, -149.6};
  double convert4[2] = {31.892, -0.06};
  double *convert[4] = {convert1, convert2, convert3, convert4};

  /** Set the rounding values */
  double rounding1[2] = { 10.05, 5.34};
  double rounding2[2] = { -10.05, -5.34};
  double rounding3[2] = {3.5678, 10.499 };
  double rounding4[2] = { 4.666,  -10.499};
  double *rounding[4] = {rounding1, rounding2, rounding3, rounding4 };

  /** And the desired rounding results */
  double roundingValue1[2] = {10.0, 5.35};
  double roundingValue2[2] = {-10.0, -5.35};
  double roundingValue3[2] = {3.6, 10.50};
  double roundingValue4[2] = {4.7, -10.50};
  double *roundValue[4] =  { roundingValue1, 
			      roundingValue2,
			      roundingValue3,
			      roundingValue4};

  /* Set the available ApiUnit values */
  ApiUnit * units[2] = { ApiUnit_Temperature_F(),
			 ApiUnit_Temperature_C()};

  /** Initialize the ApiQuantity Test object */
  ApiQuantityTest  tester;

  *errorCode = 0;

  ApiCalculationRequest_appendBuffer(buffer,len, API_EOL);
  ApiCalculationRequest_appendBuffer(buffer,len,"Running Temperature test: ");


  temperature = ApiTemperature_init(0,
				    ApiUnit_Temperature_F(),
				    TRUE,
				    errorCode);
  if( *errorCode != 0) {
        ApiCalculationRequest_appendBuffer(buffer,len, " Initialization failed"); 
        return result; 
  }

  tester.convertTest = convert;
  tester.unitMax = 2;
  tester.convertMax = 4;
  tester.units = units;

  tester.roundTest = rounding;
  tester.roundMax = 4;
  tester.roundResult = roundValue;

  tester.wrongUnit =  ApiUnit_Density_API();

  tester.compareQuant =  (ApiQuantity*)ApiTemperature_init(0,
					     ApiUnit_Temperature_F(),
					     TRUE,
					     errorCode);

  if( *errorCode != 0) {
     if( temperature != NULL) ApiQuanity_cleanUp((ApiQuantity*)temperature);
     temperature = NULL;
     ApiCalculationRequest_appendBuffer(buffer,len, " Initialization failed");
     return result;
  }


  tmp = ApiQuantityTest_testCases( (ApiQuantity*)temperature,
				   &tester,
				   buffer,
				   len,
				   errorCode);
  if( tmp == FALSE) result = FALSE;

  tmp = ApiQuantityTest_testIPTSConv( temperature,
				      buffer,
				      len,
				      errorCode);
  if( tmp == FALSE) result = FALSE;

  if( result )  ApiCalculationRequest_appendBuffer(buffer,len, " passed"); 

  if( temperature != NULL) ApiQuanity_cleanUp((ApiQuantity*)temperature);
  temperature = NULL;
  if( tester.compareQuant  != NULL) {
      ApiQuanity_cleanUp((ApiQuantity*)tester.compareQuant );
  }
  tester.compareQuant = NULL;

  return result;
}



/**
* The Pressure tests 
*/

bool ApiQuantity_pressureTest( char *buffer,
			       int len,
			       int * errorCode){
  ApiPressure *pressure;  
  bool result = FALSE;

  /* Set the conversion values */
  double convert1[3] = {10, 68.94757, 0.6894757};
  double convert2[3] = {-6789.8870,-46814.620922459, -468.14620922459};
  double convert3[3] = {6789.99, 46815.3310824, 468.1533108243};
  double convert4[3] = {-5678.9, -39154.6355273, -391.54635527299996};
  double convert5[3] = {12.334, 85.039932838,0.85039932838};
  double *convert[5] = { convert1, convert2, convert3, convert4, convert5};

  /** Set the rounding values */
  double rounding1[3] = {10.567,  16.567, 10.567};
  double rounding2[3] = {25.34, 20.34, 20.34};
  double rounding3[3] = {-43.78, -45.78, -45.78};
  double *rounding[3] = {rounding1, rounding2, rounding3};

  /** And the desired rounding results */
  double roundValue1[3] = {11, 15, 10.55};
  double roundValue2[3] = {25, 20, 20.35};
  double roundValue3[3] = {-44, -45, -45.80};
  double *roundValue[3] = { roundValue1, 
			    roundValue2,
			    roundValue3};

  /* Set the available ApiUnit values */
  ApiUnit * units[3] = { ApiUnit_Pressure_PSI(),
			 ApiUnit_Pressure_KPA(),
			 ApiUnit_Pressure_BAR()};

  /** Initialize the ApiQuantity Test object */
  ApiQuantityTest  tester;

  *errorCode = 0;

  pressure = ApiPressure_init(0,
			      ApiUnit_Pressure_PSI(),
			      TRUE,
			      errorCode);


  ApiCalculationRequest_appendBuffer(buffer,len,API_EOL);
  ApiCalculationRequest_appendBuffer(buffer,len,"Running Pressure test: ");


  if( *errorCode != 0) {
      ApiCalculationRequest_appendBuffer(buffer,len, " Initialization failed");
      return result; 
  }

  tester.convertTest = convert;
  tester.unitMax = 3;
  tester.convertMax = 5;
  tester.units = units;

  tester.roundTest = rounding;
  tester.roundMax = 3;
  tester.roundResult = roundValue;

  tester.wrongUnit =  ApiUnit_Density_API();


  tester.compareQuant =  (ApiQuantity*)ApiPressure_init(0,
							ApiUnit_Pressure_PSI(),
							TRUE,
							errorCode);
  if( *errorCode != 0) {
     if( pressure != NULL) ApiQuanity_cleanUp((ApiQuantity*)pressure);
     pressure = NULL;
     ApiCalculationRequest_appendBuffer(buffer,len, " Initialization failed");
     return result;
  }

  result = ApiQuantityTest_testCases( (ApiQuantity*)pressure,
				      &tester,
				      buffer,
				      len,
				      errorCode);
  if( result) ApiCalculationRequest_appendBuffer(buffer,len,"passed ");

  if( pressure != NULL) ApiQuanity_cleanUp((ApiQuantity*)pressure);
  pressure = NULL;
  if( tester.compareQuant  != NULL) {
      ApiQuanity_cleanUp((ApiQuantity*)tester.compareQuant );
  }
  tester.compareQuant = NULL;

  return result;
}


/**
* The Density tests 
*/

bool ApiQuantity_densityTest( char *buffer,
			       int len,
			       int * errorCode){
  ApiDensity *density;  
  bool result = FALSE;

  /* Set the conversion values */
  double convert1[3] = {10.0, 0.010009849692097023, 14004.5764};
  double convert2[3] = {1999.9999, 2.0019698383209077, -60.81961446598072};
  double convert3[3] = {56.789, 0.05684493541644979, 2357.7279138565564};
  double convert4[3] = {6789.995678, 6.796683614676843, -110.68102297796484};
  double *convert[4] = { convert1, convert2, convert3, convert4};

  /** Set the rounding values */
  double rounding1[3] = {790.53, 0.78999991, 790.55};
  double rounding2[3] = {790.56, 0.79005, 790.551};
  double *rounding[3] = {rounding1, rounding2};

  /** And the desired rounding results */
  double roundValue1[3] = {790.5, 0.7900, 790.6};
  double roundValue2[3] = {790.6, 0.7900, 790.6};
  double *roundValue[3] = { roundValue1, roundValue2};

  /* Set the available ApiUnit values */
  ApiUnit * units[3] = { ApiUnit_Density_KGM3(),
			 ApiUnit_Density_RELATIVE(),
			 ApiUnit_Density_API()};

  /** Initialize the ApiQuantity Test object */
  ApiQuantityTest  tester;

  *errorCode = 0;

  ApiCalculationRequest_appendBuffer(buffer,len,API_EOL);
  ApiCalculationRequest_appendBuffer(buffer,len,"Running Density test: ");


  density = ApiDensity_init(0,
			    ApiUnit_Density_KGM3(),
			    TRUE,
			    errorCode);
  if( *errorCode != 0) {
      ApiCalculationRequest_appendBuffer(buffer,len, " Initialization failed");
      return result; 
  }

  tester.convertTest = convert;
  tester.unitMax = 3;
  tester.convertMax = 4;
  tester.units = units;

  tester.roundTest = rounding;
  tester.roundMax = 2;
  tester.roundResult = roundValue;

  tester.wrongUnit =   ApiUnit_Pressure_PSI();;

  tester.compareQuant =  (ApiQuantity*)ApiDensity_init(0,
						       ApiUnit_Density_KGM3(),
						       TRUE,
						       errorCode);
  if( *errorCode != 0) {
    if( density != NULL) ApiQuanity_cleanUp((ApiQuantity*)density);
    density = NULL;
    ApiCalculationRequest_appendBuffer(buffer,len, " Initialization failed");
    return result;
  }

  
  result = ApiQuantityTest_testCases( (ApiQuantity*)density,
				      &tester,
				      buffer,
				      len,
				      errorCode);
  if( result) ApiCalculationRequest_appendBuffer(buffer,len,"passed ");


  if( density != NULL) ApiQuanity_cleanUp((ApiQuantity*)density);
  density = NULL;
  if( tester.compareQuant  != NULL) {
      ApiQuanity_cleanUp((ApiQuantity*)tester.compareQuant );
  }
  tester.compareQuant = NULL;

  return result;
}



/**
* The Volume tests 
*/

bool ApiQuantity_volumeTest( char *buffer,
			     int len,
			     int * errorCode){
  ApiVolume *volume;  
  bool result = TRUE;

  *errorCode = 0;

  volume = ApiVolume_init(0,
			  ApiUnit_Volume_M3(),
			  TRUE,
			  errorCode);

  ApiCalculationRequest_appendBuffer(buffer,len,API_EOL);
  ApiCalculationRequest_appendBuffer(buffer,len,"Running Volume test: ");


  /* Check that only the orignal works for getValue */
  ApiQuantity_getValue( (ApiQuantity*)volume, ApiUnit_Volume_LITER(), FALSE, errorCode);
  if( *errorCode == 0){
    *errorCode = INVALID_UNIT;
    result = FALSE;
  }

  /** Check that the rounding fails */
  ApiQuantity_getValue( (ApiQuantity*)volume, ApiUnit_Volume_M3(), TRUE, errorCode);
  if( *errorCode == 0){
    *errorCode = UNSUPPORTED_FUNCTION;
    result = FALSE;
  }

  if( result) ApiCalculationRequest_appendBuffer(buffer,len,"passed ");

  if( volume != NULL) ApiQuanity_cleanUp((ApiQuantity*)volume);
  volume = NULL;

  return result;
}


/**
* The Expansion Factor tests 
*/

bool ApiQuantity_expansionFactorTest( char *buffer,
				      int len,
				      int * errorCode){
  ApiExpansionFactor *factor;  
  bool result = FALSE;

  /* Set the conversion values */
  double convert1[1] = {1.005678};
  double convert2[1] = {0.994578};
  double convert3[1] = {0.870911};
  double convert4[1] = {1.000191};
  double *convert[4] = { convert1, convert2, convert3, convert4};

  /** Set the rounding values */
  double rounding1[1] = {1.019851328373};
  double rounding2[1] = {1.004858068990};
  double rounding3[1] = {0.988457250925};
  double rounding4[1] = {1.057742015457};
  double *rounding[4] = {rounding1, rounding2,rounding3,rounding4};

  /** And the desired rounding results */
  double roundValue1[1] = {1.01985};
  double roundValue2[1] = {1.00486};
  double roundValue3[1] = {0.98846};
  double roundValue4[1] = {1.05774};
  double *roundValue[4] = { roundValue1, 
			    roundValue2,
			    roundValue3,
			    roundValue4};

  /* Set the available ApiUnit values */
  ApiUnit * units[1] = { ApiUnit_Expansion_DIMLESS()};

  /** Initialize the ApiQuantity Test object */
  ApiQuantityTest  tester;

  *errorCode = 0;

  factor = ApiExpansionFactor_init(0,
				   ApiUnit_Expansion_DIMLESS(),
				   TRUE,
				   errorCode);


  ApiCalculationRequest_appendBuffer(buffer,len,API_EOL);
  ApiCalculationRequest_appendBuffer(buffer,len,"Running Expansion Factor test: ");

  if( *errorCode != 0) {
     ApiCalculationRequest_appendBuffer(buffer,len, " Initialization failed");
     return result;
  }

  tester.convertTest = convert;
  tester.unitMax = 1;
  tester.convertMax = 4;
  tester.units = units;

  tester.roundTest = rounding;
  tester.roundMax = 4;
  tester.roundResult = roundValue;

  tester.wrongUnit =   ApiUnit_Pressure_PSI();


  tester.compareQuant =  (ApiQuantity*)ApiExpansionFactor_init(1.0,
                                                       ApiUnit_Expansion_DIMLESS(),
						       TRUE,
						       errorCode);
  if( *errorCode != 0) {
    if( factor != NULL) ApiQuanity_cleanUp((ApiQuantity*)factor);
    factor = NULL;
    ApiCalculationRequest_appendBuffer(buffer,len, " Initialization failed");
    return result;
  }

  result = ApiQuantityTest_testCases( (ApiQuantity*)factor,
				      &tester,
				      buffer,
				      len,
				      errorCode);
  if( result) ApiCalculationRequest_appendBuffer(buffer,len,"passed ");


  if( factor != NULL) ApiQuanity_cleanUp((ApiQuantity*)factor);
  factor = NULL;
  if( tester.compareQuant  != NULL) {
      ApiQuanity_cleanUp((ApiQuantity*)tester.compareQuant );
  }
  tester.compareQuant = NULL;

  return result;
}


/**
* The Scaled Compression  Factor tests 
*/

bool ApiQuantity_scaledFactorTest( char *buffer,
				   int len,
				   int * errorCode){
  ApiScaledFactor *factor;  
  bool result = FALSE;

  /* Set the conversion values */
  double convert1[3] = {4.578, 0.6639827915617621, 66.39827915617622};
  double convert2[3] = {0.93456, 0.1355464739366449, 13.554647393664489};
  double convert3[3] = {1.6789, 0.2435038682291486, 24.350386822914864};
  double convert4[3] = {3.52316, 0.5109911777891519, 51.09911777891519};
  double *convert[4] = { convert1, convert2, convert3, convert4};

  /** Set the rounding values */
  double rounding1[3] = {0.135678, 0.135678, 0.135678};
  double rounding2[3] = {0.2456789, 0.2456789, 0.2456789};
  double *rounding[2] = {rounding1, rounding2};

  /** And the desired rounding results */
  double roundValue1[3] = {0.136, 0.1357, 0.14};
  double roundValue2[3] = {0.246, 0.2457, 0.25};
  double *roundValue[2] = { roundValue1, roundValue2 };


  /* Set the available ApiUnit values */
  ApiUnit * units[3] = { ApiUnit_ScaledComp_REV_PSI(),
			 ApiUnit_ScaledComp_REV_KPA(),
			 ApiUnit_ScaledComp_REV_BAR()};

  /** Initialize the ApiQuantity Test object */
  ApiQuantityTest  tester;

  *errorCode = 0;

  ApiCalculationRequest_appendBuffer(buffer,len,API_EOL);
  ApiCalculationRequest_appendBuffer(buffer,
				     len,
				     "Running Scaled Compression Factor test: ");


  factor = ApiScaledFactor_init(0,
				ApiUnit_ScaledComp_REV_PSI(),
				TRUE,
				errorCode);
  if( *errorCode != 0) {
      ApiCalculationRequest_appendBuffer(buffer,len, " Initialization failed");
      return result;
  }

  tester.convertTest = convert;
  tester.unitMax = 3;
  tester.convertMax = 4;
  tester.units = units;

  tester.roundTest = rounding;
  tester.roundMax = 2;
  tester.roundResult = roundValue;

  tester.wrongUnit =   ApiUnit_Pressure_PSI();

  tester.compareQuant =  (ApiQuantity*)ApiScaledFactor_init(1.0,
							    ApiUnit_ScaledComp_REV_PSI(),
							    TRUE,
							    errorCode);
  if( *errorCode != 0) {
    if( factor != NULL) ApiQuanity_cleanUp((ApiQuantity*)factor);
    factor = NULL;
    ApiCalculationRequest_appendBuffer(buffer,len, " Initialization failed xx");
    return result;
  }

  result = ApiQuantityTest_testCases( (ApiQuantity*)factor,
				      &tester,
				      buffer,
				      len,
				      errorCode);
  if( result) ApiCalculationRequest_appendBuffer(buffer,len,"passed ");


  if( factor != NULL) ApiQuanity_cleanUp((ApiQuantity*)factor);
  factor = NULL;
  if( tester.compareQuant  != NULL) {
      ApiQuanity_cleanUp((ApiQuantity*)tester.compareQuant );
  }
  tester.compareQuant = NULL;

  return result;
}


/**
* The Thermal Expansion Factor tests 
*/

bool ApiQuantity_alpha60Test( char *buffer,
			      int len,
			      int * errorCode){
  ApiAlpha60 *factor;  
  bool result = FALSE;

  /* Set the conversion values */
  double convert1[2] = {0.0005027, 0.00090486};
  double convert2[2] = {0.0007749, 0.00139482};
  double convert3[2] = {0.00057634, 0.001037412};
  double convert4[2] = {0.0003801, 0.00068418};
  double *convert[4] = { convert1, convert2, convert3, convert4};

  /** Set the rounding values */
  double rounding1[2] = {0.00025198, 0.00025198};
  double rounding2[2] = {0.00081636, 0.00081636};
  double rounding3[2] = {0.0000016, 0.0000016};
  double *rounding[3] = {rounding1, rounding2, rounding3};

  /** And the desired rounding results */
  double roundValue1[2] = {0.0002520, 0.0002520};
  double roundValue2[2] = {0.0008164, 0.0008164 };
  double roundValue3[2] = {0.0000016, 0.0000016};
  double *roundValue[3] = { roundValue1, roundValue2, roundValue3 };


  /* Set the available ApiUnit values */
  ApiUnit * units[2] = { ApiUnit_ThermalExp_REV_F(),
                         ApiUnit_ThermalExp_REV_C() };

  /** Initialize the ApiQuantity Test object */
  ApiQuantityTest  tester;

  *errorCode = 0;

  ApiCalculationRequest_appendBuffer(buffer,len,API_EOL);
  ApiCalculationRequest_appendBuffer(buffer,len,"Running Thermal Expansion Factor test: ");

  factor = ApiAlpha60_init(0,
			   ApiUnit_ThermalExp_REV_F(),
			   TRUE,
			   errorCode);
  if( *errorCode != 0) {
     ApiCalculationRequest_appendBuffer(buffer,len, " Initialization failed");
     return result;
  }

  tester.convertTest = convert;
  tester.unitMax = 2;
  tester.convertMax = 4;
  tester.units = units;

  tester.roundTest = rounding;
  tester.roundMax = 3;
  tester.roundResult = roundValue;

  tester.wrongUnit =   ApiUnit_Pressure_PSI();

  tester.compareQuant =  (ApiQuantity*)ApiAlpha60_init(1.0,
						       ApiUnit_ThermalExp_REV_F(),
						       TRUE,
						       errorCode);
  if( *errorCode != 0) {
    if( factor != NULL) ApiQuanity_cleanUp((ApiQuantity*)factor);
    factor = NULL;
    ApiCalculationRequest_appendBuffer(buffer,len, " Initialization failed");
    return result;
  }

  result = ApiQuantityTest_testCases( (ApiQuantity*)factor,
				      &tester,
				      buffer,
				      len,
				      errorCode);
  if( result) ApiCalculationRequest_appendBuffer(buffer,len,"passed ");


  if( factor != NULL) ApiQuanity_cleanUp((ApiQuantity*)factor);
  factor = NULL;
  if( tester.compareQuant  != NULL) {
      ApiQuanity_cleanUp((ApiQuantity*)tester.compareQuant );
  }
  tester.compareQuant = NULL;

  return result;
}

bool ApiQuantity_runTest( char *buffer,
			  int len,
			  int * errorCode){
  bool result = TRUE, tmp;

  tmp  =  ApiQuantity_temperatureTest( buffer, len , errorCode);
  if( tmp == FALSE) result = FALSE;

  tmp  =  ApiQuantity_pressureTest( buffer, len , errorCode);
  if( tmp == FALSE) result = FALSE;

  tmp =  ApiQuantity_densityTest( buffer, len , errorCode);
  if( tmp == FALSE) result = FALSE;

  tmp =  ApiQuantity_volumeTest( buffer, len , errorCode);
  if( tmp == FALSE) result = FALSE;

  tmp =  ApiQuantity_expansionFactorTest( buffer, len , errorCode);
  if( tmp == FALSE) result = FALSE;

  tmp =  ApiQuantity_scaledFactorTest( buffer, len , errorCode);
  if( tmp == FALSE) result = FALSE;

  tmp =  ApiQuantity_alpha60Test( buffer, len , errorCode);
  if( tmp == FALSE) result = FALSE;

  return result;
}

