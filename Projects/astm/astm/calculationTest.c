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

#include <stdio.h>
#include <string.h>

/**
* Run all  the tests
*/
bool ApiCalculationTest_runTests(char * buffer, 
				 int bufLength,
				 char * errBuf,
				 int errLength,
				 int *errorCode){
   ApiCalculationRequest * request;
   bool result = TRUE, tmp;

   request = ApiCalculationRequest_init(errorCode);
   if( *errorCode != 0) return FALSE;
   request->intermediate = buffer;
   request->intLength = bufLength;
   request->getParameterName = ApiCalculation_getParameterName;
   request->getOilProductName = ApiCalculation_getProductName;

   tmp = ApiCalculationTest_run11_1_6_1Tests(request, errBuf, errLength, errorCode);
   if( *errorCode != 0) return FALSE;
   if( tmp == FALSE) result = FALSE; 

   tmp = ApiCalculationTest_run11_1_6_2Tests(request, errBuf, errLength, errorCode);
   if( *errorCode != 0) return FALSE;
   if( tmp == FALSE) result = FALSE;

   tmp = ApiCalculationTest_run11_1_6_3Tests(request, errBuf, errLength,  errorCode);
   if( *errorCode != 0) return FALSE;
   if( tmp == FALSE) result = FALSE;

   tmp = ApiCalculationTest_run11_1_7_1Tests(request, errBuf, errLength, errorCode);
   if( *errorCode != 0) return FALSE;
   if( tmp == FALSE) result = FALSE;

   tmp = ApiCalculationTest_run11_1_7_2Tests(request, errBuf, errLength, errorCode);
   if( *errorCode != 0) return FALSE;
   if( tmp == FALSE) result = FALSE;

   tmp = ApiCalculationTest_run11_1_7_3Tests(request, errBuf, errLength, errorCode);
   if( *errorCode != 0) return FALSE;
   if( tmp == FALSE) result = FALSE;

   return result;
}


/** 
* Set the expected results for the examples
* 
*/
int ApiCalculationTest_setResult(ApiCalculationRequest * request,

			         double baseDens, ApiUnit * dub,
				 double ctlObToBase,
				 double scaledOb, ApiUnit * suo,
				 double cplObToBase,
				 double ctplObToBase,
				 double baseVol, ApiUnit *vub,

				 double ctlBaseToAlt,
				 double scaledAlt, ApiUnit * sua,
				 double cplBaseToAlt,
				 double ctplBaseToAlt,
				 double altDens, ApiUnit* dua,
				 double altVol,  ApiUnit * vua,

				 double obVol, ApiUnit * vuo){
  int errorCode = 0;
  ApiUnit *unit = ApiUnit_Expansion_DIMLESS();

  if( request == NULL) return NULL_POINTER_EXCEPTION;

  if( request->baseDens != NULL){
    errorCode =  ApiQuantity_setValue((ApiQuantity*)request->baseDens, baseDens, dub);
    if( errorCode != 0) return errorCode;
  }

  if( request->ctlObToBase != NULL){
    errorCode =  ApiQuantity_setValue((ApiQuantity*)request->ctlObToBase, 
				      ctlObToBase,
				      unit);
    if( errorCode != 0) return errorCode;
  }

  if( request->scaledCompOb != NULL){
    errorCode =  ApiQuantity_setValue((ApiQuantity*)request->scaledCompOb, 
				      scaledOb,
				      suo);
    if( errorCode != 0) return errorCode;
  }

  if( request->cplObToBase != NULL){
    errorCode =  ApiQuantity_setValue((ApiQuantity*)request->cplObToBase, 
				      cplObToBase,
				      unit);
    if( errorCode != 0) return errorCode;
  }

  if( request->ctplObToBase != NULL){
    errorCode =  ApiQuantity_setValue((ApiQuantity*)request->ctplObToBase, 
				      ctplObToBase,
				      unit);
    if( errorCode != 0) return errorCode;
  }

  if( request->baseVol != NULL){
    errorCode =  ApiQuantity_setValue((ApiQuantity*)request->baseVol, baseVol, vub); 
    if( errorCode != 0) return errorCode;
  }

  if( request->ctlBaseToAlt != NULL){
    errorCode =  ApiQuantity_setValue((ApiQuantity*)request->ctlBaseToAlt , 
				      ctlBaseToAlt,
				      unit);
    if( errorCode != 0) return errorCode;
  }

  if( request->scaledCompAlt != NULL){
    errorCode =  ApiQuantity_setValue((ApiQuantity*)request->scaledCompAlt, 
				      scaledAlt,
				      sua);
    if( errorCode != 0) return errorCode;
  }

  if( request->cplBaseToAlt != NULL){
    errorCode =  ApiQuantity_setValue((ApiQuantity*)request->cplBaseToAlt , 
				      cplBaseToAlt,
				      unit);
    if( errorCode != 0) return errorCode;
  }

  if( request->ctplBaseToAlt != NULL){
    errorCode =  ApiQuantity_setValue((ApiQuantity*)request->ctplBaseToAlt , 
				      ctplBaseToAlt,
				      unit);
    if( errorCode != 0) return errorCode;
  }

  if( request->altDens != NULL){
    errorCode =  ApiQuantity_setValue((ApiQuantity*)request->altDens, altDens, dua); 
    if( errorCode != 0) return errorCode;
  }

  if( request->obVol != NULL){
    errorCode =  ApiQuantity_setValue((ApiQuantity*)request->obVol, obVol, vuo); 
    if( errorCode != 0) return errorCode;
  }

  if( request->altVol != NULL){
    errorCode =  ApiQuantity_setValue((ApiQuantity*)request->altVol, altVol, vua); 
    if( errorCode != 0) return errorCode;
  }
  return errorCode;
}


/** 
* Do a calculation and test that the results are as expected
*/
bool ApiCalculationTest_checkResult(ApiCalculationRequest * request,
				    ApiCalculationRequest * result,
				    bool  wantException,
				    char * desc,
				    char * error,
				    int errorLength,
				    int * errorCode){
  bool test = TRUE;
  bool tmp;

  *errorCode = 0;

  *errorCode = ApiCalculationRequest_performCalculation(request);
  if( *errorCode != 0){
    if( wantException == TRUE){  /** We expected an error - test finish successful */
	ApiCalculationRequest_appendBuffer(request->intermediate, 
					   request->intLength,
					   API_EOL);
	ApiCalculation_getErrorDescription(*errorCode,
					   request->intermediate,
					   request->intLength);
	ApiCalculationRequest_appendBuffer(request->intermediate, 
					   request->intLength,
					   API_EOL);
       *errorCode = 0;
       return test;
    }
    else {
       ApiCalculationRequest_appendBuffer(error, errorLength,API_EOL); 
       ApiCalculationRequest_appendBuffer(error, errorLength,"Unexpected error: "); 
       ApiCalculation_getErrorDescription(*errorCode, error, errorLength);
       ApiCalculationRequest_appendBuffer(error, errorLength,API_EOL); 
       return FALSE;
    }
  }


  /** Check that we have a result object for comparison */
  if( result == NULL) {
    *errorCode = NULL_POINTER_EXCEPTION;
    result = FALSE;
    return test;
  }

  /** Start comparing */
  tmp = ApiCalculationTest_checkForError(error, 
					 errorLength,
					 (ApiQuantity*)result->baseDens,
					 (ApiQuantity*)request->baseDens,
					 desc,
					 API_BASE_DENS_NAME,
					 errorCode);
  if( tmp == FALSE || *errorCode != 0)  test = FALSE;

  tmp = ApiCalculationTest_checkForError(error, 
					 errorLength,
					 (ApiQuantity*)result->ctlObToBase,
					 (ApiQuantity*)request->ctlObToBase,
					 desc,
					 API_CTL_OB_TO_BASE_NAME,
					 errorCode);
  if( tmp == FALSE || *errorCode != 0)   test = FALSE;

  tmp = ApiCalculationTest_checkForError(error, 
					 errorLength,
					 (ApiQuantity*)result->cplObToBase,
					 (ApiQuantity*)request->cplObToBase,
					 desc,
					 API_CPL_OB_TO_BASE_NAME,
					 errorCode);
  if( tmp == FALSE || *errorCode != 0)   test = FALSE;

  tmp = ApiCalculationTest_checkForError(error, 
					 errorLength,
					 (ApiQuantity*)result->ctplObToBase,
					 (ApiQuantity*)request->ctplObToBase,
					 desc,
					 API_CTPL_OB_TO_BASE_NAME,
					 errorCode);
  if( tmp == FALSE || *errorCode != 0)  test  = FALSE;

  tmp = ApiCalculationTest_checkForError(error, 
					 errorLength,
					 (ApiQuantity*)result->scaledCompOb,
					 (ApiQuantity*)request->scaledCompOb,
					 desc,
					 API_SCALED_COMP_OB_NAME,
					 errorCode);
  if( tmp == FALSE || *errorCode != 0)  test  = FALSE;

  tmp = ApiCalculationTest_checkForError(error, 
					 errorLength,
					 (ApiQuantity*)result->baseVol,
					 (ApiQuantity*)request->baseVol,
					 desc,
					 API_BASE_VOL_NAME,
					 errorCode);

  if( tmp == FALSE || *errorCode != 0)  test  = FALSE;

  tmp = ApiCalculationTest_checkForError(error, 
					 errorLength,
					 (ApiQuantity*)result->ctlBaseToAlt,
					 (ApiQuantity*)request->ctlBaseToAlt,
					 desc,
					 API_CTL_BASE_TO_ALT_NAME,
					 errorCode);  
  if( tmp == FALSE || *errorCode != 0)  test  = FALSE;

  tmp = ApiCalculationTest_checkForError(error, 
					 errorLength,
					 (ApiQuantity*)result->cplBaseToAlt,
					 (ApiQuantity*)request->cplBaseToAlt,
					 desc,
					 API_CPL_BASE_TO_ALT_NAME,
					 errorCode);
  if( tmp == FALSE || *errorCode != 0)  test  = FALSE;

  tmp = ApiCalculationTest_checkForError(error, 
					 errorLength,
					 (ApiQuantity*)result->ctplBaseToAlt,
					 (ApiQuantity*)request->ctplBaseToAlt,
					 desc,
					 API_CTPL_BASE_TO_ALT_NAME,
					 errorCode);
  if( tmp == FALSE || *errorCode != 0)  test  = FALSE;


  tmp = ApiCalculationTest_checkForError(error, 
					 errorLength,
					 (ApiQuantity*)result->scaledCompAlt,
					 (ApiQuantity*)request->scaledCompAlt,
					 desc,
					 API_SCALED_COMP_ALT_NAME,
					 errorCode);
  if( tmp == FALSE || *errorCode != 0)  test  = FALSE;

  tmp = ApiCalculationTest_checkForError(error, 
					 errorLength,
					 (ApiQuantity*)result->altDens,
				  	 (ApiQuantity*)request->altDens,
					 desc,
					 API_ALT_DENS_NAME,
					 errorCode);
  if( tmp == FALSE || *errorCode != 0)  test  = FALSE;

  tmp = ApiCalculationTest_checkForError(error, 
					 errorLength,
					 (ApiQuantity*)result->obVol,
					 (ApiQuantity*)request->obVol,
					 desc,
					 API_OB_VOL_NAME,
					 errorCode);
  if( tmp == FALSE || *errorCode != 0)  test  = FALSE;

  tmp = ApiCalculationTest_checkForError(error, 
					 errorLength,
					 (ApiQuantity*)result->altVol,
					 (ApiQuantity*)request->altVol,
					 desc,
					 API_ALT_VOL_NAME,
					 errorCode);
  if( tmp == FALSE || *errorCode != 0)  test  = FALSE;


  return test;
}

/**
* Check whether a result is correct
*/
bool  ApiCalculationTest_checkForError(char * error,
				       int errorLength,
				       ApiQuantity * expected,
				       ApiQuantity  * result,
				       char * desc,
				       int param,
				       int *errorCode){
  bool test = TRUE;
  int c1;

  *errorCode = 0;

  if( expected != NULL){
    /** Check that we have a result */
    if( result == NULL) {
      test = FALSE;
      *errorCode = NULL_POINTER_EXCEPTION;
      if( error != NULL){
	ApiCalculationRequest_appendBuffer(error, errorLength,desc);
	ApiCalculationRequest_appendBuffer(error, errorLength,API_EOL);
	ApiCalculationRequest_appendBuffer(error, errorLength,
					   "Unexpected null pointer while checking ");
	ApiCalculation_getParameterName(param, error, errorLength);
	ApiCalculationRequest_appendBuffer(error, errorLength,API_EOL);
      }
      return test;
    }

    /** Perform the comparison between result and expected */
    c1 = ApiQuantity_compareTo(result,expected,errorCode);
    if( *errorCode != 0){
      test = FALSE;
      if( error != NULL){
	ApiCalculationRequest_appendBuffer(error, errorLength,desc);
	ApiCalculationRequest_appendBuffer(error, errorLength,API_EOL);
	ApiCalculationRequest_appendBuffer(error, errorLength,
					   "Unexpected error while checking ");
	ApiCalculation_getParameterName(param, error, errorLength);
	ApiCalculationRequest_appendBuffer(error, errorLength,API_EOL);
      }
      return test;
    }

    /** Report an error if ther tow quantities are not equal */
    if( c1 != 0){
      test = FALSE;
      if( error != NULL){
	ApiUnit *u = ApiQuantity_givenUnit( (ApiQuantity*)expected, errorCode);
	double value;

	ApiCalculationRequest_appendBuffer(error, errorLength,desc);
	ApiCalculationRequest_appendBuffer(error, errorLength,API_EOL);
	ApiCalculationRequest_appendBuffer(error, errorLength,"For ");
	ApiCalculation_getParameterName(param, error, errorLength);
	ApiCalculationRequest_appendBuffer(error, errorLength," Expected ");
	if( *errorCode != 0){	
	   ApiCalculationRequest_appendBuffer(error, errorLength,"Unexpected error ");
	   return test;
	}
	value = ApiQuantity_getValue((ApiQuantity*)expected, u, FALSE,errorCode);
	if( *errorCode != 0){	
	   ApiCalculationRequest_appendBuffer(error, errorLength,"Unexpected error");
	   return test;
	}
	ApiCalculationRequest_appendDouble(error, errorLength,value);
	ApiUnit_unitName(u, error, errorLength);
	ApiCalculationRequest_appendBuffer(error, errorLength," got ");
	value = ApiQuantity_getValue((ApiQuantity*)result, u, FALSE,errorCode);
	if( *errorCode != 0){	
	   ApiCalculationRequest_appendBuffer(error, errorLength,"Unexpected error ");
	   return test;
	}
	ApiCalculationRequest_appendDouble(error, errorLength,value);
	ApiUnit_unitName(u, error, errorLength);
	ApiCalculationRequest_appendBuffer(error, errorLength,API_EOL);
      }
    }
  }
  return test;
}

