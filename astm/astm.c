/*     A S T M 
 *     
 *     Frame-program for API interface
 *
 *     Version 1.3.B, 14.09.1998
 *
 *     (C) Copyright SAP AG 1998
 *
 * 
 * This program calls the API chap. 11.1 VCF subroutines
 * which must be linked in the program.
 *
 * To build this program, the following sources are necessary:
 *   api_tabs.c den_tabs.c sg_tabs.c tab_5960.c tcommon.c 
 *   api_tabs.h den_tabs.h sg_tabs.h tab_5960.h tcommon.h
 *
 * these API routines can be ordered from the following address:
 *   American Petroleum Institute
 *   Measurement Coordination
 *   1220 L Street, N.W.
 *   Washington, DC 20005
 *
 * Commandline:
 * ------------
 * parameter 0:    call of the ASTM-program
 * parameter 1..n: parameters of the API-subroutines        
 *
 * Example:        /usr/sap/SO3/SYS/exe/run/astm tab24c 0.000492 15.5
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>

#ifdef _WIN32
#include <fcntl.h>
#include <io.h>
#endif

#include "api_tabs.h"
#include "sg_tabs.h"
#include "den_tabs.h"
#include "tab_5960.h"

static void help ( void );

void tabfden(float*,float*,long,float*,long*);

void tabfvcf(float*,float*,float*,float*,long*);

static char *table[] = {
             /* 0..12  Functions with structures OIBIA1 / OIBEA1 */
                        "tab5a",   "tab5b",  "tab5d", 
                        "tab23a",  "tab23b", "tab23d",
                        "tab53a",  "tab53b", "tab53d",
                        "tabfden", "tab59a", "tab59b",
                        "tab59d",

             /* 13..25 Functions with structures OIBIA2 / OIBEA2 */
                        "tab6a",   "tab6b",  "tab6d",     
                        "tab24a",  "tab24b", "tab24d", 
                        "tab54a",  "tab54b", "tab54d",
                        "tabfvcf", "tab60a", "tab60b",
                        "tab60d",              

             /* 26..29 Functions with structures OIBIA3 / OIBEA3 */
                        "tab6c", "tab24c", "tab54c",
                        "tab60c"
                       }; 
float      pf1, pf2;
long int   pli1;

float      rf1, rf2;
long int   rli1;
                        
int pos,i;
int main(argc,argv)
 int argc; char *argv[];
 
{

/*  if on NT then do output in binary mode */
#ifdef _WIN32 
        if (_setmode( _fileno( stdout ), _O_BINARY ) == -1)
        {
                printf("can't switch stdout to _O_BINARY\n");
                exit(0);
        }
        if (_setmode( _fileno( stderr ), _O_BINARY ) == -1)
        {
                printf("can't switch stderr to _O_BINARY\n");
                exit(0);
        }
#endif

 if (argc == 1) 
    {
     printf("%s \n","ERROR:FUNCTION_NOT_FOUND");
     exit(0);
    };
 if (0==strcmp(argv[1],"-h") || 0==strcmp(argv[1],"-H") ||
     0==strcmp(argv[1],"HELP") || 0==strcmp(argv[1],"help"))
    {
 /*  printf("%s \n","HELP screen ..........."); */
     help();
     exit(0);
    };

 for (pos=0;pos<=29 && 0!=strcmp(argv[1],table[pos]); pos++) {};


/* Prepare parametes for functions */

 switch(pos)
    {
     case  0:; case  1:; case  2:;
     case  3:; case  4:; case  5:;
     case  6:; case  7:; case  8:;
     case  9:; case 10:; case 11:;
     case 12:;
       /* import: OIBIA1 -TDICH, -TSTMP, -HYDRO
          export: OIBEA1 -BDICH, -CSTAT         
       */

       if (argc < 5)
         {
           printf("%s \n","ERROR:TO_FEW_PARAMETERS");
           break;
         };

       if (argc > 5)
         {
           printf("%s \n","ERROR:TO_MANY_PARAMETERS");
           break;
         };
       
       sscanf(argv[2], "%f",&pf1);
       sscanf(argv[3], "%f",&pf2);
       sscanf(argv[4], "%ld",&pli1);
  
       switch(pos)
         {
           case 0:  tab5a(&pf1,&pf2,pli1,&rf1,&rli1); break;
           case 1:  tab5b(&pf1,&pf2,pli1,&rf1,&rli1); break;
           case 2:  tab5d(&pf1,&pf2,pli1,&rf1,&rli1); break;
           case 3:  tab23a(&pf1,&pf2,pli1,&rf1,&rli1); break;
           case 4:  tab23b(&pf1,&pf2,pli1,&rf1,&rli1); break;
           case 5:  tab23d(&pf1,&pf2,pli1,&rf1,&rli1); break;
           case 6:  tab53a(&pf1,&pf2,pli1,&rf1,&rli1); break;
           case 7:  tab53b(&pf1,&pf2,pli1,&rf1,&rli1); break;
           case 8:  tab53d(&pf1,&pf2,pli1,&rf1,&rli1); break;
           case 9:  tabfden(&pf1,&pf2,pli1,&rf1,&rli1); break;
           case 10: tab59a(&pf1,&pf2,pli1,&rf1,&rli1); break;
           case 11: tab59b(&pf1,&pf2,pli1,&rf1,&rli1); break;
           case 12: tab59d(&pf1,&pf2,pli1,&rf1,&rli1); break;

         }; 
     
       printf("%e \n %d \n",rf1,rli1); 
      
       break;    
 
    case 13:; case 14:; case 15:;
    case 16:; case 17:; case 18:; 
    case 19:; case 20:; case 21:; 
    case 22:; case 23:; case 24:; 
    case 25:; 
      /* import: OIBIA2 -BDICH, -MTTMP
         export: OIBEA2 -VCF1,  -VCF2, -CSTAT
      */

       if (argc < 4)
         { 
           printf("%s \n","ERROR:TO_FEW_PARAMETERS");
           break;
         };

       if (argc > 4)
         {
           printf("%s \n","ERROR:TO_MANY_PARAMETERS");
           break;
         };

       sscanf(argv[2], "%f",&pf1);
       sscanf(argv[3], "%f",&pf2);
      
       switch(pos)
         {
           case 13:  tab6a(&pf1,&pf2,&rf1,&rf2,&rli1); break;
           case 14:  tab6b(&pf1,&pf2,&rf1,&rf2,&rli1); break;
           case 15:  tab6d(&pf1,&pf2,&rf1,&rf2,&rli1); break;
           case 16:  tab24a(&pf1,&pf2,&rf1,&rf2,&rli1); break;
           case 17:  tab24b(&pf1,&pf2,&rf1,&rf2,&rli1); break;
           case 18:  tab24d(&pf1,&pf2,&rf1,&rf2,&rli1); break;
           case 19:  tab54a(&pf1,&pf2,&rf1,&rf2,&rli1); break;
           case 20:  tab54b(&pf1,&pf2,&rf1,&rf2,&rli1); break;
           case 21:  tab54d(&pf1,&pf2,&rf1,&rf2,&rli1); break;
           case 22:  tabfvcf(&pf1,&pf2,&rf1,&rf2,&rli1); break;
           case 23:  tab60a(&pf1,&pf2,&rf1,&rf2,&rli1); break;
           case 24:  tab60b(&pf1,&pf2,&rf1,&rf2,&rli1); break;
           case 25:  tab60d(&pf1,&pf2,&rf1,&rf2,&rli1); break;

         };
       
       printf("%e \n %e \n %d \n",rf1,rf2,rli1);
   
       break;

    case 26:; case 27:; case 28:; 
    case 29:;
      /* import: OIBIA3 -COEFF, -TSTMP
         export: OIBEA3 -VCF1, -VCF2, -CSTAT
      */

       if (argc < 4)
         { 
           printf("%s \n","ERROR:TO_FEW_PARAMETERS");
           break;
         };

       if (argc > 4)
         {
           printf("%s \n","ERROR:TO_MANY_PARAMETERS");
           break;
         };

       sscanf(argv[2], "%f",&pf1);
       sscanf(argv[3], "%f",&pf2);
     
       switch(pos)
         {
           case 26:  tab6c(&pf1,&pf2,&rf1,&rf2,&rli1); break;
           case 27:  tab24c(&pf1,&pf2,&rf1,&rf2,&rli1); break;
           case 28:  tab54c(&pf1,&pf2,&rf1,&rf2,&rli1); break;
           case 29:  tab60c(&pf1,&pf2,&rf1,&rf2,&rli1); break;

         };

     
       printf("%e \n %e \n %d \n",rf1,rf2,rli1);

       break;
     default : 
       printf("%s \n","ERROR:FUNCTION_NOT_FOUND");
       break;                               /* error message */
   }
return 0;
}

/*   Internal functions
 *   ================== */


void tabfden(float *api, float *degf, long int ihydro,
                  float *api60, long int *iflag)
{
   *api60 = *api;
   *iflag = 0L;
   return;
}


void tabfvcf(float *api60, float *degf, float *vcfc,
                  float *vcfp, long int *iflag)
{
   *vcfc  = 1.F;
   *vcfp  = 1.F;
   *iflag = 0L;
   return;
}


static void help( void )
{
#define NL "\n"
   printf( NL
   "**********************************************"             NL
   "*                                            *"             NL 
   "*   ASTM - Quantity Conversion/Calculation   *"             NL
   "*      version 1.3.B used by IS-OIL          *"             NL
   "*                                            *"             NL 
   "**********************************************"             NL NL
   "Syntax:"                                                    NL
   "         astm <tablename> <parameters of conversion table>" NL NL
   "Example: astm tab24c 0.000492 15.5"                         NL NL
   "Installed API-tables:"                                      NL
   "---------------------"                                      NL NL
   "Tables with IO parameters like structures OIBIA1/OIBEA1"    NL NL
   "   tab5a   tab5b   tab5d"                                   NL
   "   tab23a  tab23b  tab23d"                                  NL
   "   tab53a  tab53b  tab53d"                                  NL 
   "   tabfden"                                                 NL 
   "   tab59a  tab59b  tab59d"                                  NL NL
   "   import: density temperature hydro-indicator"             NL
   "   export: density calculation-status"                      NL NL
   "Tables with IO parameters like structures OIBIA2/OIBEA2"    NL NL
   "   tab6a   tab6b   tab6d"                                   NL
   "   tab24a  tab24b  tab24d"                                  NL
   "   tab54a  tab54b  tab54d"                                  NL 
   "   tabfvcf"                                                 NL
   "   tab60a  tab60b  tab60d"                                  NL NL
   "   import: density temperature"                             NL
   "   export: vol-corr-fact. vol-corr-fact. culation-status"   NL NL
   "Tables with IO parameters like structures OIBIA3/OIBEA3"    NL NL
   "   tab6c   tab24c  tab54c"                                  NL 
   "   tab60c"                                                  NL NL
   "   import: density temperature"                             NL
   "   export: vol-corr-fact. vol-corr-fact. culation-status"   NL NL
   "Help functionality:"                                        NL
   "-------------------"                                        NL NL
   "   astm [-h | -H | help | HELP]"                            NL NL
   "   This screen"                                             NL NL
   "Example:"                                                   NL
   "--------"                                                   NL NL
   "   Commandline:"                                            NL NL
   "     astm tab24c 0.000492 15.5"                             NL NL
   "   Response on standard output:"                            NL NL
   "     1.021700e+00"                                          NL
   "      1.021700e+00"                                         NL 
   "      0"                                                    NL
	 );
   return;
}
