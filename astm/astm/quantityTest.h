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


#ifndef API_QUANTITY_TEST_H
#define API_QUANTITY_TEST_H

#ifdef _WIN32
#define dll_export __declspec(dllexport)
#else
#define dll_export
#endif

#include "quantity.h"
#include "constants.h"

/* protect code so it can be compiled with a C++ compiler */
#ifdef __cplusplus
extern "C" {
#endif

/** @file quantityTest.h
* Test routines that test the quantity routines.
*/ 


/**
* Run the tests for the Temperature quantity
*
* @param buffer the buffer to which to add the reason for failure
* @param len the length of the buffer
* @param errorCode an errorCode to set if the test failed 
*
* @return TRUE if the test was successful, FALSE otherwise
*/
dll_export bool ApiQuantity_temperatureTest( char *buffer,
				  int len,
				  int * errorCode);
 

/**
* Run the tests for the Pressure quantity
*
* @param buffer the buffer to which to add the reason for failure
* @param len the length of the buffer
* @param errorCode an errorCode to set if the test failed 
*
* @return TRUE if the test was successful, FALSE otherwise
*/
dll_export bool ApiQuantity_pressureTest( char *buffer,
			       int len,
			       int * errorCode);


/**
* Run the tests for the Density quantity
*
* @param buffer the buffer to which to add the reason for failure
* @param len the length of the buffer
* @param errorCode an errorCode to set if the test failed 
*
* @return TRUE if the test was successful, FALSE otherwise
*/
dll_export bool ApiQuantity_densityTest( char *buffer,
			      int len,
			      int * errorCode);
 
/**
* Run the tests for the Volume quantity
*
* @param buffer the buffer to which to add the reason for failure
* @param len the length of the buffer
* @param errorCode an errorCode to set if the test failed 
*
* @return TRUE if the test was successful, FALSE otherwise
*/
dll_export bool ApiQuantity_volumeTest( char *buffer,
			     int len,
			     int * errorCode);

/**
* Run the tests for the Expansion Factor quantity
*
* @param buffer the buffer to which to add the reason for failure
* @param len the length of the buffer
* @param errorCode an errorCode to set if the test failed 
*
* @return TRUE if the test was successful, FALSE otherwise
*/
dll_export bool ApiQuantity_expansionFactorTest( char *buffer,
				      int len,
				      int * errorCode);


/**
* Run the tests for the Scaled Compression  Factor quantity
*
* @param buffer the buffer to which to add the reason for failure
* @param len the length of the buffer
* @param errorCode an errorCode to set if the test failed 
*
* @return TRUE if the test was successful, FALSE otherwise
*/
dll_export bool ApiQuantity_scaledFactorTest( char *buffer,
				   int len,
				   int * errorCode);
 

/**
* Run the tests for the Thermale Expansion  Factor quantity
*
* @param buffer the buffer to which to add the reason for failure
* @param len the length of the buffer
* @param errorCode an errorCode to set if the test failed 
*
* @return TRUE if the test was successful, FALSE otherwise
*/
dll_export bool ApiQuantity_alpha60Test( char *buffer,
			      int len,
			      int * errorCode);


/**
* Run the tests for the Quantity object
*
* @param buffer the buffer to which to add the reason for failure
* @param len the length of the buffer
* @param errorCode an errorCode to set if the test failed 
*
* @return TRUE if the test was successful, FALSE otherwise
*/
dll_export bool ApiQuantity_runTest( char *buffer,
			  int len,
			  int * errorCode);
 




/* close protected C++ bracket */
#ifdef __cplusplus
}
#endif

#endif /* API_QUANTITY_TEST_H  */
