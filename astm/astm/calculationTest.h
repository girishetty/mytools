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
#ifndef CALCULATION_TEST_H
#define CALCULATION_TEST_H

#ifdef _WIN32
#define dll_export __declspec(dllexport)
#else
#define dll_export
#endif

/* protect code so it can be compiled with a C++ compiler */
#ifdef __cplusplus
extern "C" {
#endif

#include "util.h"

/** 
 *@file calculationTest.h
* Contains  test cases to test the VCF calculation functions.
*/ 

/**  @defgroup testGroup Test Cases  */
/*@{*/


/**
* Run  test  to verify the correct working of the VCF calculation
* function.
* This runs calculations for all the examples given
* in 11.1.6.1 - 11.1.7.3 and compares the results achieved
* to the expected results.
*
* @param buffer the buffer in which to report input and output values.
*        If #NULL no intermediate results are reported.
* @param bufLength the length of the buffer
*
* @param errBuf the buffer in which to report errors
* @param errLength the length of the buffer to report errors 
*
* @param errorCode a variable to report possible errors encountered. 
*        This is set to 0 if no errors were encountered, otherwise it
*        will be set to an appropriate error code. 
*
* @return #TRUE if all tests where run successfully, #FALSE otherwise
*/
dll_export bool ApiCalculationTest_runTests(char * buffer, 
				 int bufLength,
				 char *errBuf,
				 int errLength,
				 int *errorCode);


/**
* Run the test cases for the 11.1.6.1 calculations. 
* This runs all the examples  given
* in 11.1.6.1 and compares the results achieved
* to the expected results. 
*
* @param request the request to use to run the calculations
*
* @param errBuf the buffer in which to report errors
* @param errLength the length of the buffer to report errors 
*
* @param errorCode  a variable to report possible errors encountered. 
*        This is set to 0 if no errors were encountered, otherwise it
*        will be set to an appropriate error code. 
*
* @return #TRUE if all tests where run successfully, #FALSE otherwise
*/
dll_export bool  ApiCalculationTest_run11_1_6_1Tests(ApiCalculationRequest * request, 
					  char *errBuf,
					  int errLength,
					  int * errorCode);

/**
* Run the test cases for the 11.1.6.2 calculations. 
* This runs all the examples  given
* in 11.1.6.2 and compares the results achieved
* to the expected results. 
*
* @param request the request to use to run the calculations
*
* @param errBuf the buffer in which to report errors
* @param errLength the length of the buffer to report errors 
*
* @param errorCode  a variable to report possible errors encountered. 
*        This is set to 0 if no errors were encountered, otherwise it
*        will be set to an appropriate error code. 
*
* @return #TRUE if all tests where run successfully, #FALSE otherwise
*/
dll_export bool  ApiCalculationTest_run11_1_6_2Tests(ApiCalculationRequest * request, 
					  char *errBuf,
					  int errLength,
					  int * errorCode);


/**
* Run the test cases for the 11.1.6.3 calculations. 
* This runs all the examples  given
* in 11.1.6.3 and compares the results achieved
* to the expected results. 
*
* @param request the request to use to run the calculations
*
* @param errBuf the buffer in which to report errors
* @param errLength the length of the buffer to report errors 
*
* @param errorCode  a variable to report possible errors encountered. 
*        This is set to 0 if no errors were encountered, otherwise it
*        will be set to an appropriate error code. 
*
* @return #TRUE if all tests where run successfully, #FALSE otherwise
*/
dll_export bool  ApiCalculationTest_run11_1_6_3Tests(ApiCalculationRequest * request, 
					  char *errBuf,
					  int errLength,                       
					  int * errorCode);


/**
* Run the test cases for the 11.1.7.1 calculations. 
* This runs all the examples  given
* in 11.1.7.1 and compares the results achieved
* to the expected results. 
*
* @param request the request to use to run the calculations
*
* @param errBuf the buffer in which to report errors
* @param errLength the length of the buffer to report errors 
*
* @param errorCode  a variable to report possible errors encountered. 
*        This is set to 0 if no errors were encountered, otherwise it
*        will be set to an appropriate error code. 
*
* @return #TRUE if all tests where run successfully, #FALSE otherwise
*/
dll_export bool  ApiCalculationTest_run11_1_7_1Tests(ApiCalculationRequest * request, 	
					  char *errBuf,
					  int errLength,                       
					  int * errorCode);


/**
* Run the test cases for the 11.1.7.2 calculations. 
* This runs all the examples  given
* in 11.1.7.2 and compares the results achieved
* to the expected results. 
*
* @param request the request to use to run the calculations
*
* @param errBuf the buffer in which to report errors
* @param errLength the length of the buffer to report errors 
*
* @param errorCode  a variable to report possible errors encountered. 
*        This is set to 0 if no errors were encountered, otherwise it
*        will be set to an appropriate error code. 
*
* @return #TRUE if all tests where run successfully, #FALSE otherwise
*/
dll_export bool  ApiCalculationTest_run11_1_7_2Tests(ApiCalculationRequest * request, 
					  char *errBuf,
					  int errLength,                       
					  int * errorCode);

/**
* Run the test cases for the 11.1.7.3 calculations. 
* This runs all the examples  given
* in 11.1.7.3 and compares the results achieved
* to the expected results. 
*
* @param request the request to use to run the calculations
*
* @param errBuf the buffer in which to report errors
* @param errLength the length of the buffer to report errors 
*
* @param errorCode  a variable to report possible errors encountered. 
*        This is set to 0 if no errors were encountered, otherwise it
*        will be set to an appropriate error code. 
*
* @return #TRUE if all tests where run successfully, #FALSE otherwise
*/
dll_export bool  ApiCalculationTest_run11_1_7_3Tests(ApiCalculationRequest * request, 
					  char *errBuf,
					  int errLength,                       
					  int * errorCode);



/** 
* Set the expected results for the examples. Values are only set if the 
* corresponding quantity in the request object is non-null. 
* 
* @param request  the ::ApiCalculationRequest to use to set the results
*
* @param baseDens the base density, i.e. density at ApiCalculationRequest::baseTemp
*                  and ApiCalculationRequest::basePres
* @param dub  the unit of the baseDens
* @param ctlObToBase the  ctl  factor to transform from
*                     ApiCalculationRequest::obTemp to ApiCalculationRequest::baseTemp.
* @param scaledOb   The Fp factor to transform from
*                    ApiCalculationRequest::obTemp  and  ApiCalculationRequest::obPres  to 
*                    ApiCalculationRequest::baseTemp  and  ApiCalculationRequest::basePres 
* @param suo       The unit to use for scaledOb
* @param cplObToBase  the  cpl  factor to transform from
*                     ApiCalculationRequest::obPres to ApiCalculationRequest::basePres.
* @param ctplObToBase the  ctpl  factor to transform from
*                     ApiCalculationRequest::obPres and  ApiCalculationRequest::obTemp 
*                     to ApiCalculationRequest::basePres and ApiCalculationRequest::baseTemp
* @param baseVol the volume at  ApiCalculationRequest::baseTemp
*                  and ApiCalculationRequest::basePres
* @param vub   the unit for the baseVol
*
* @param ctlBaseToAlt  the  ctl  factor to transform from
                       ApiCalculationRequest::baseTemp to ApiCalculationRequest::altTemp.
* @param scaledAlt   The Fp factor to transform from
*                     ApiCalculationRequest::baseTemp  and  ApiCalculationRequest::basePres 
*                     to 
*                     ApiCalculationRequest::altTemp  and  ApiCalculationRequest::altPres
* @param sua    the unit to use for scaledAlt
* @param cplBaseToAlt  the  cpl  factor to transform from
*                     ApiCalculationRequest::basePres to ApiCalculationRequest::altPres.
* @param ctplBaseToAlt   the  ctpl  factor to transform from
*                     ApiCalculationRequest::basePres and  ApiCalculationRequest::baseTemp 
*                     to ApiCalculationRequest::altPres and ApiCalculationRequest::altTemp
* @param altDens   the density at ApiCalculationRequest::altPres 
*                 and ApiCalculationRequest::altTemp
* @param dua    the unit for  altDens
* @param altVol   the volume at  ApiCalculationRequest::altPres 
*                 and ApiCalculationRequest::altTemp
* @param vua   the unit for altVol
*
* @param obVol   the volume at ApiCalculationRequest::obPres 
*                 and ApiCalculationRequest::obTemp
* @param vuo   the unit for obVol
*
* @return 0 if all parameters were set successfully, an errorCode otherwise
*/
dll_export int ApiCalculationTest_setResult(ApiCalculationRequest * request,
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
				 double altVol,  ApiUnit* vua,
				 double obVol, ApiUnit * vuo);



/** 
* Perform a  calculation and test that the results are as expected
*
* @param request  the requested calculation
* @param result   the expected result
* @param wantException do we expect an error, for example because the
*        tempterature is deliberately out of range.
* @param desc a description of the test
* @param error a buffer to which to report the error
* @param errorLength the length of the error buffer
* @param errorCode  a variable to report possible errors encountered. 
*        This is set to 0 if no errors were encountered, otherwise it
*        will be set to an appropriate error code. 
*
* @return #TRUE if the results are as expected, #FALSE otherwise
*/
dll_export bool ApiCalculationTest_checkResult(ApiCalculationRequest * request,
				    ApiCalculationRequest * result,
				    bool  wantException,
				    char * desc,
				    char * error,
				    int errorLength,
				    int * errorCode);

/**
* Check whether a result is correct. We compare  the values of the 
* two quantity structures passed in using  the 
*  ApiQuantity_compareTo() function.
* 
* @param error the buffer to which to append the error
* @param errorLength the length of the error buffer
* @param expected the expected result
* @param result the result we received
* @param desc description of the test we are running
* @param param the parameter we are checking
* @param errorCode    a variable to report possible errors encountered. 
*        This is set to 0 if no errors were encountered, otherwise it
*        will be set to an appropriate error code. 
*
* @return #TRUE if the results are as expected, #FALSE otherwise
*/
dll_export bool ApiCalculationTest_checkForError(char * error,
				       int errorLength,
				       ApiQuantity * expected,
				       ApiQuantity  * result,
				       char * desc,
				       int param,
				       int *errorCode);

/*@}*/

/* close protective C++ bracket */
#ifdef __cplusplus
}
#endif

#endif /* CALCULATION_TEST_H  */
