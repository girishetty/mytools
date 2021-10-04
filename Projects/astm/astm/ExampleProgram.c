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
#include <stdlib.h>
#include <stdio.h>


/**
* This program demonstrates how to use the Api VCF library functions.
*  
* It calculates the test cases used to compare calculation results
*/

/*
* Perform a calculation and record the result. 
*
* @param calcReq the calculation request describing the result
* @param unit the unit for the base density to print
* @param file the file to which to append the data
* @param if dens the density is reported, otherwise the ctpl with an
*        eol is reported
*/ 
void doCalculation(ApiCalculationRequest * calcReq,
                   ApiUnit * unit,
                   FILE * file,
		   int dens){
  int errorCode = 0;
  char buffer[100];
  double value,rounded;

  /*  perform the calculation */
  errorCode = ApiCalculationRequest_performCalculation(calcReq);

  /* report the error if necessary */
  if( errorCode != 0){
     buffer[0] = '\0';
     ApiCalculation_getErrorDescription(errorCode, buffer, 100);
     fprintf(file,"%s,%s",buffer,buffer);
  }

  /* report the result */
  else{
      /* report base density in requested units */ 
      if( dens != 0){
	value = ApiQuantity_getValue( (ApiQuantity*)calcReq->baseDens,
                                        unit,
	 			        FALSE,
				        &errorCode);
	rounded = ApiQuantity_getValue( (ApiQuantity*)calcReq->baseDens,
                                         unit,
	 			         TRUE,
				         &errorCode);
      }
      /* report the ctpl value */
      else {
         value = ApiQuantity_getValue( (ApiQuantity*)calcReq->ctplBaseToAlt,
                                       ApiUnit_Expansion_DIMLESS(),
	  			       FALSE,
				       &errorCode);
         rounded = ApiQuantity_getValue( (ApiQuantity*)calcReq->ctplBaseToAlt,
                                        ApiUnit_Expansion_DIMLESS(),
	  		 	        TRUE,
				        &errorCode);
      }

      /* report error or value necessary */
      if( errorCode != 0){
	   buffer[0] = '\0';
           ApiCalculation_getErrorDescription(errorCode,buffer, 100);
	   fprintf(file,"%s,%s",buffer,buffer);
      }
      else{
	fprintf(file,"%20.10f,%20.10f",value,rounded);
      }
  }

   /* report the correct ending, i.e. comma if density eol otherwise */
   if( dens != 0) fprintf(file,",");
   else fprintf(file,"\r\n");
   errorCode = 0;                                    
};


/**
* Parse a line, set up the calculation request and report the result
* 
* @param calcReq the ApiCalculationRequest structure to use
* @param line the line to parse
* @param file the file to which to write the data
*
* @return 0 if the calculation can be done, -1 if the line cannot be parsed
*/
int parseLine(ApiCalculationRequest * calcReq,
               char *line,
               FILE *file){
  ApiUnit *temp = NULL, *obDensUnit = NULL, *obPresUnit = NULL; 
  int commodityName = API_COMMODITY_NOT_GIVEN;
  double baseTemp,obPres, obDens, obTemp;
  char *pos1,*pos2;
  int errorCode;

  pos1 = strstr(line,",");  // end of Atrn/Ptr
  if( pos1 == NULL) return -1;
  pos1++;
  pos1 = strstr(pos1,",");  // end of Row
  if( pos1 == NULL) return -1;
  pos1++;
  pos2 = strstr(pos1,",");  // end of Commodity
  if( pos2 == NULL) return -1;

  /* Determine the commodity type */
  *pos2 = '\0';
  if( strcmp(pos1,"Crude") == 0) {
        commodityName = API_CRUDE_OIL_NAME;
  }
  else if( strcmp(pos1,"Products") == 0) {
     commodityName = API_REFINED_PRODUCTS_NAME;
  }
  else if( strcmp(pos1,"Lubricants") == 0){
    commodityName = API_LUBRICATING_OIL_NAME;
  }
  else {
     *pos2 = ',';
     return -1;
  }
  *pos2 = ',';
  pos2++;


  /** Determine the temperature unit */
  pos1 = pos2;
  pos2 = strstr(pos1,",");  // end of TempUnit
  if( pos2 == NULL) return -1;
  *pos2 = '\0';
  if( strcmp(pos1,"deg F") == 0){
    temp = ApiUnit_Temperature_F();
  }
  else if( strcmp(pos1,"deg C") == 0){
    temp = ApiUnit_Temperature_C();
  }
  else{
     *pos2 = ',';
     return -1;
  }
  *pos2 = ',';
  pos2++;


  /** Determine the base temperature */
  pos1 = pos2;
  pos2 = strstr(pos1,",");  // end of TempBase
  if( pos2 == NULL) return -1;
  *pos2 = '\0';
  baseTemp =  atof(pos1);
  *pos2 = ',';
  pos2++;


  /** Determine the pressure unit */
  pos1 = pos2;
  pos2 = strstr(pos1,",");  // end of PressUnit
  if( pos2 == NULL) return -1;
  *pos2 = '\0';
  if( strcmp(pos1,"psig") == 0){
    obPresUnit = ApiUnit_Pressure_PSI();
  }
  else if( strcmp(pos1,"kPa(gauge)") == 0){
    obPresUnit = ApiUnit_Pressure_KPA();
  }
  else{
     *pos2 = ',';
     return -1;
  }
  *pos2 = ',';
  pos2++;


  /** Determine the density unit */
  pos1 = pos2;
  pos2 = strstr(pos1,",");  // end of DensUnit
  if( pos2 == NULL) return -1;
  *pos2 = '\0';
  if( strcmp(pos1,"API") == 0){
    obDensUnit = ApiUnit_Density_API();
  }
  else if( strcmp(pos1,"Relative Density") == 0){
    obDensUnit = ApiUnit_Density_RELATIVE();
  }
  else if( strcmp(pos1,"kg/m3") == 0){
    obDensUnit = ApiUnit_Density_KGM3();
  }
  else{
     *pos2 = ',';
     return -1;
  }
  *pos2 = ',';
  pos2++;

  /** Determine the observed temperature */
  pos1 = pos2;
  pos2 = strstr(pos1,",");  // end of Temp
  if( pos2 == NULL) return -1;
  *pos2 = '\0';
  obTemp =  atof(pos1);
  *pos2 = ',';
  pos2++;


  /** Determine the observed pressure */
  pos1 = pos2;
  pos2 = strstr(pos1,",");  // end of Pressure
  if( pos2 == NULL) return -1;
  *pos2 = '\0';
  obPres =  atof(pos1);
  *pos2 = ',';
  pos2++;

  /** Determine the observed Density */
  pos1 = pos2;
  pos2 = strstr(pos1,",");  // end of Density
  if( pos2 != NULL){   // followed by more commas
     *pos2 = '\0';  
     obDens =  atof(pos1);
  }
  else{                // not followed by commas, let's clean up the line
    obDens = strtod(pos1,&pos2);
    if( pos2 != NULL) *pos2  = '\0';
  }

  /** Start the output with the line value containing all the input parameters */
  fprintf(file,"%s,",line);

  /** Set the parameters  for calculation of base density, i.e a 11.1.6.2
   * calculation */
  errorCode = ApiCalculation_setParameters(calcReq,
					   commodityName,  
					   0, NULL,
					   obTemp, temp,
					   baseTemp, temp,
					   obPres, obPresUnit,
					   0, ApiUnit_Pressure_PSI(),
					   obDens, obDensUnit,
					   baseTemp, temp,
					   0, ApiUnit_Pressure_PSI(),

					   0, NULL,
					   0, NULL,
					   0, NULL);
  if( errorCode != 0) return -1;
  errorCode = 0;
  doCalculation(calcReq, obDensUnit, file, 1);

  /** Set the parameters for the calculation of ctpl, i.e. a 11.1.6.1 calculation */
  errorCode = ApiCalculation_setParameters(calcReq,
					   commodityName,  
					   0, NULL,
					   baseTemp, temp,
					   obTemp, temp,
					   0, ApiUnit_Pressure_PSI(),
					   obPres, obPresUnit,
					   obDens, obDensUnit,
					   baseTemp, temp,
					   0, ApiUnit_Pressure_PSI(),

					   0, NULL,
					   0, NULL,
					   0, NULL);
  if( errorCode != 0) return -1;
  errorCode = 0;
  doCalculation(calcReq, obDensUnit, file, 0);


  return 0;  
};

int bmain(int argv, char **arg) {
  int len = 5000;
  char buffer[len];
  int errorCode = 0;
  ApiCalculationRequest * request;
  FILE *input,*output;
  int byte,index;
  bool firstLine = TRUE;

  /* open the files for reading and writing */
  if( argv < 3){
    printf("Need two command line arguments: input and output file\n");
    return -1;
  }
  input = fopen(arg[1],"r");
  if( input == NULL){
    printf("Can't open %s\n",arg[1]);
    return -1;
  }
  output = fopen(arg[2],"w");
  if( output == NULL){
    printf("Can't open %s\n",arg[2]);
    return -1;
  }

  /** Create and initialize request */ 
  errorCode = ApiCalculationRequest_initialize();
  if( errorCode != 0){
    buffer[0] = '\0';
    ApiCalculationRequest_appendBuffer(buffer, len,"Initialization Error: ");
    ApiCalculation_getErrorDescription(errorCode, buffer, len);
    printf("%s\n",buffer);
    return -1;
  }
  request = ApiCalculationRequest_init(&errorCode);
  if( errorCode != 0){
    buffer[0] = '\0';
    ApiCalculationRequest_appendBuffer(buffer, len,"Initialization Error: ");
    ApiCalculation_getErrorDescription(errorCode, buffer, len);
    printf("%s\n",buffer);
    return -1;
  }

  /** Read a line  and perform calculation */
  index = 0;
  while(  (byte = fgetc(input))  != -1 ){
    if( byte == -1 || byte == '\n' || byte == '\r'){
	if( index >= 100) index = 100;
        buffer[index] = '\0';
	if( firstLine == TRUE){   // this is the header line 
	  fprintf(output,"%s\r\n",buffer);
	  firstLine = FALSE;
	}
	else{
           index = parseLine(request, buffer, output);
	   if( index != 0) {
	     printf("Error parsing: %s\n",buffer);
	   }
	}
	index = 0;

	// check whether the next character is \r or \n
        byte = fgetc(input);
	if( byte != '\r' && byte != '\n') ungetc(byte,input);
    }
    else{
       if( index < 100 -1) buffer[index++] = (char)byte;
    }
    if( byte == -1) break;
  }


  /** dispose of the request and clean all resources */
  ApiCalculationRequest_cleanUp( request);
  ApiCalculationRequest_destroy();
  fclose(input);
  fclose(output);
}

