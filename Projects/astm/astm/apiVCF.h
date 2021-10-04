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
#ifndef API_VCF_INCLUDE_H
#define API_VCF_INCLUDE_H

/* protect code so it can be compiled with a C++ compiler */
#ifdef __cplusplus
extern "C" {
#endif

/**
* @mainpage API VCF C Library
*
* <h3> Introduction</h3>
* The library allows a user to calculate Volume Correction Factors. 
* <p>
* The input parameters are quantities. All these quantities are defined in
* quantity.h. Qauntity values can only be set and retrieved if the 
* desired unit is given. Structures that define the available units are
* defined in unit.h. 
* <p>
* Before any calculations is performed the calculation context should be 
* initialized using the function ApiCalculationRequest_initialize(), which
* initializes all global constants used in the calculation. After all 
* calculations are performed the function ApiCalculationRequest_destroy() 
* should be called to free all resources.
* <p>
* The compiled Windows DLL calls the initialization routines mentioned in the
* previous paragraph upon initial loading of the DLL. The function 
* ApiCalculationRequest_destroy() is called upon unloading of the DLL. In addition
* the DLL runs ApiCalculation_runTests() upon loading to ensure proper 
* calculation precision.   
* <p>
* A calculation involves to set the input parameters  in the 
* ::ApiCalculationRequest structure and calling 
*   ApiCalculationRequest_performCalculation(ApiCalculationRequest *request),
* which will set the calculated output parameters in the 
* ::ApiCalculationRequest structure passed in. The calculation might  alter
* some of the input parameters, especially the  temperature, since the 
* calculation involves an ITPS68 shift of the temperature.
* The calculation can optionaly report input and output parameter if
* an intermediate buffer ApiCalculationRequest::intermediate is supplied. If
* intermediate results are also desired set  
* ApiCalculationRequest::reportIntermediate to #TRUE. If the
* intermediate buffer is not #NULL, functions should
* be supplied for  ApiCalculationRequest::getOilProductName and
* ApiCalculationRequest::getParameterName. Suitable functions 
* are defined in util.h. 
* <p>
* Some utility functions that make it easier to set the  input parameters are 
* defined in util.h. The function:
* <ul>
*   <li>ApiCalculation_set11_1_6_1Parameters()
*     sets the parameters for a 11.1.6.1 type calculation.
*   <li>ApiCalculation_set11_1_6_2Parameters()
*     sets the parameters for a 11.1.6.2 type calculation.
*   <li>ApiCalculation_set11_1_7_1Parameters()
*     sets the parameters for a 11.1.7.2 type calculation.
*   <li>ApiCalculation_set11_1_7_1Parameters()
*     sets the parameters for a 11.1.7.2 type calculation.
* </ul>
* <p>
* A test is provided that runs several test cases to ensure that the library
* performs to the desired precision. The function  ApiCalculation_runTests()
* will run these tests and return #TRUE if all tests are passed.
*<p>
* Thus, to run a 11.1.6.1 type calculation
* one  would use the following code:
* <pre>
\#ifdef WIN32
  \#include "WindowsExportTable.h"
\#else
  \#include <apiVCF/apiVCF.h>
\#endif
\#include <stdlib.h>
\#include <stdio.h>

int main(int argv, char **arg) {
  int len = 5000;
  char buffer[len];
  int errorCode = 0;
  bool result;
  ApiCalculationRequest * request;

  errorCode = ApiCalculationRequest_initialize();
  if( errorCode != 0){
    buffer[0] = '\0';
    ApiCalculationRequest_appendBuffer(buffer, len,"Initialization Error: ");
    ApiCalculation_getErrorDescription(errorCode, buffer, len);
    printf(" \%s \\n",buffer);
    return -1;
  }
  request = ApiCalculationRequest_init(&errorCode);
  if( errorCode != 0){
    buffer[0] = '\0';
    ApiCalculationRequest_appendBuffer(buffer, len,"Initialization Error: ");
    ApiCalculation_getErrorDescription(errorCode, buffer, len);
    printf("\%s \\n",buffer);
    return -1;
  }

  buffer[0] ='\0';
  request->intermediate = buffer;
  request->intLength = len;
  request->reportIntermediate = TRUE;
  request->getParameterName = ApiCalculation_getParameterName;
  request->getOilProductName = ApiCalculation_getProductName;

  errorCode = ApiCalculation_set11_1_6_1Parameters(request,
						   API_CRUDE_OIL_NAME,
						   0, NULL,
						   -27.7, ApiUnit_Temperature_F(),
						   0, ApiUnit_Pressure_PSI(),
						   17.785, ApiUnit_Density_API(),
						   987.99,  ApiUnit_Volume_M3());
  if( errorCode != 0){
    buffer[0] = '\0';
    ApiCalculationRequest_appendBuffer(buffer, len,"Initialization Error: ");
    ApiCalculation_getErrorDescription(errorCode, buffer, len);
    printf(" \%s \\n",buffer);
    return -1;
  }

  errorCode = ApiCalculationRequest_performCalculation(request);
  if( errorCode != 0){
    ApiCalculationRequest_appendBuffer(buffer, len,"Error: ");
    ApiCalculation_getErrorDescription(errorCode, buffer, len);
  }
  printf(" \%s\\n",buffer);
  ApiCalculationRequest_cleanUp( request);
  ApiCalculationRequest_destroy();
  return -1 * errorCode;
}
* </pre>
*<p>
* All calculation constants, like for example conversion factors, are defined in
* constants.h. All possible error codes are defined in error.h. The 
* human readable names for the commodities, error codes  and input and output
* parameters are defined in util.src. The numerical names for the commodity and
* for the input and output parameters are defined in  parameternames.h.
*
*
* <h3>Installation</h3>
* <h4>Linux, Unix, Mac OSX </h4>
* To install use the following commands:
<pre>
./configure
./make
./make check
./make install
</pre>
* which will install the libary into /usr/local/lib. The configure script 
* has several additional options that determine the final install path and
* the type of libraries build. Type:
<pre> 
./configure --help
</pre>
* to get a list of the available options.
*
*<h4>Windows</h4>
*The windows part consists of four Visual Studio projects
*<ul>
*  <li><dl><dt>ApiVCFBase<dt>
*          <dd> This contains the function described by this  documentation and
*               builds the Windows DLL. By default the MFC library is linked 
*               dynamically with this DLL.</dd>
*       </dl>
*  <li><dl><dt>ApiVCFExcel<dt>
*          <dd> Contains the Excel Add-In functionality. This library is statically
*                linked with the AouVCFBase functions and the MFC. 
*                This project contains a directory, hlp, which contains the 
*                file used to create the context sensitive Windows help file. This
*                 needs to be build and updated independently.
*          </dd>
*       </dl>
*  <li><dl><dt>ExcellAddInstall<dt>
*          <dd> A console application that install the registry key  to automatically
*               load the Api VCF Excel Add-In.
*          </dd>
*       </dl>
*  <li><dl><dt>Api VCF Calculation<dt>
*          <dd>The project that performs all the Windows install procedures.
*          </dd>
*       </dl>
*</ul>
*These projects are bundled together in the Visual Studio Solution 
*"ApiVCFSolution". 
*
* The file "WindowsExportTable.h", summarizes all exported functions suitable to be 
* include in a Windows project. The major difference between it and "apiVCF.h" is 
* that the functions are defined with  __declspec(dllimport) and 
* as  __stdcall.
*<p>
* Some library functions can be called from Visual Basic, they are
* collected in VBAExport.h. To call a function from the library, declare
* the desired function:
*<pre>
* Declare Function ctplA11161API Lib "apiVCFBase.dll" Alias "ctplA11161API" (ByVal dens As Double, ByVal temp As Double, ByVal pres As Double, ByRef errorCode As Integer) As Double
</pre>
* and then the function can be called from Visual Basic.
* 
* <h3>Example programs</h3>
* There is an example program (src/ExampleProgram.c), which calculates the 
* values prescribed as test cases. 
*/

/** @file apiVCF.h
* Convienence include file that includes all files that are needed 
* to perform an API VCF calculation. On Windows, use 
* WindowsExportTable.h instead, it exports the same functions but declares
* them as __stdcall.
*/ 

/* Include all the include files needed by the calculation */
#include "parameternames.h"
#include "constants.h"
#include "error.h"
#include "unit.h"
#include "oilproducts.h"
#include "quantity.h"
#include "calculation.h"

#include "quantityTest.h"
#include "calculationTest.h"
#include "util.h"
#include "VBAExport.h"

/* close protective C++ bracket */
#ifdef __cplusplus
}
#endif

#endif /* API_CONSTANTS_H  */
