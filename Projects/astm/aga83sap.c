/*     A G A S A P 
 *     
 *     Frame-program for AGA interface
 *
 *     Version 1.0.B, 09.01.2001
 *
 *     (C) Copyright SAP AG 2000
 *
 * 
 * This program calls the AGA functions CALCDL, CALCGS and ORIFICE
 *
 * To build this program, the following sources are necessary:
 *   aga3sa.c aga8plus.c grosssa.c detailsa.c             
 *   aga83.h  aga8plus.h aga8gs.h   ga8dl.h   aga3.h
 *
 * these AGA routines can be ordered from the
 *   A.G.A. Distribution Center
 *   P.O. Box 79230
 *   Baltimore, MD 21279-0230
 *
 * Commandline:
 * ------------
 * parameter 0:    call of the agasap program
 * parameter 1:    AGA function to be called
 * parameter 2..n: parameters of the AGA function        
 *
 * Example:  agasap calcgs 2 0.581 0.003 0.006 0.0 0.0 270 6 288.15
 *                         0.101325 288.15 0.101325 288.15 0.101325 288.15   
 */

#include "aga83.h"
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <math.h>

#ifdef _WIN32
#include <fcntl.h>
#include <io.h>
#endif

#if defined(SAPonOS400)
#ifndef PROTOTYPES
#define PROTOTYPES
#endif
#ifdef SAPwithCHAR_ASCII
#include "o4ascii.h"
#endif
#endif

static void help ( void );

static char *table[] = {
             /* 0      Functions with structures OIBIA8 / OIBEA8
                                                 OIBIA7 / OIBEA7 */
                        "calcgs", 

             /* 1      Functions with structures OIBIA6 / OIBEA6 */
                        "calcdl",

             /* 2      Functions with structures OIBIA9 / OIBEA9 */
                        "orifice"
                       };

#if defined(SAPonOS400)
int main(int argc, char *argv[])
#else
int main(argc, argv)
     int   argc;
     char *argv[];
#endif 
     
{

/*...declare variables...*/
  int    pos, i, j, method, code, ntaps, matorf, matpipe, 
		 ifluid, nploc, cdflag;

  double tf, pf, tb, pb, hv, grgr, tgr, pgr, td, pd, th, ts, 
         ps, zf, zb, zs, rhotp, rhob, rhos, fpvs, mwgas, grb,
         grs, xp[5], xi[21], dox, torf, dm, tpipe, qv, hw, visc,
         kfac, doc, dmc, factr, zairs, ev, cd, fn, fc, fsl, fa,
         cprm, y, fb, fr, fpb, ftb, ftf, fgr, avgvel, red, beta;

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

/*...check arguments...*/
  if (argc == 1) 
     {
      printf("%s \n","ERROR:FUNCTION_NOT_FOUND");
      exit(0);
     };
  if (0==strcmp(argv[1],"-h") || 0==strcmp(argv[1],"-H") ||
      0==strcmp(argv[1],"HELP") || 0==strcmp(argv[1],"help"))
     {
      help();
      exit(0);
     };

/*...proceed command...*/
  for (pos=0;pos<=1 && 0!=strcmp(argv[1],table[pos]); pos++) {};


/*...prepare parameters for functions ...*/
 switch(pos)
    {
     case  0:;
       /* import: OIBIA8/OIBIA7
          export: OIBEA8/OIBEA7       
       */
/*
printf("%d \n",argc);
for (pos=0;pos<=argc;pos++) printf("%s \n",argv[pos]);break;
*/
       if (argc < 17)
         {
           printf("%s \n","ERROR:TO_FEW_PARAMETERS");
           break;
         };

       if (argc > 17)
         {
           printf("%s \n","ERROR:TO_MANY_PARAMETERS");
           break;
         };
       
       sscanf(argv[2], "%d",&method);
       if (method == 1)
       {
         sscanf(argv[4], "%lf",&hv);
         xp[1] = 0;
       }
       else
       {
         sscanf(argv[4], "%lf",&xp[1]);
         hv = 0;
       };
       sscanf(argv[3], "%lf",&grgr);
       sscanf(argv[5], "%lf",&xp[2]);
       sscanf(argv[6], "%lf",&xp[3]);
       sscanf(argv[7], "%lf",&xp[4]); 
       sscanf(argv[8], "%lf",&tf);
       sscanf(argv[9], "%lf",&pf);
       sscanf(argv[10],"%lf",&tb);
       sscanf(argv[11],"%lf",&pb);
       sscanf(argv[12],"%lf",&tgr);
       sscanf(argv[13],"%lf",&pgr);
       sscanf(argv[14],"%lf",&td);
       sscanf(argv[15],"%lf",&pd);
       sscanf(argv[16],"%lf",&th);


       code=CALCGS(&method,&grgr,&hv,xp,&tf,&pf,&tb,&pb,&tgr,&pgr,
                   &td,&pd,&th,
                   &ts,&ps,&zf,&zb,&zs,&rhotp,&rhob,&rhos,&fpvs,
                   &mwgas,&grb,&grs);

       printf("%d \n %le \n %le \n %le \n %le \n %le \n %le \n %le \n",
               code,xp[0],ts,ps,zf,zb,zs,rhotp);
       printf("%le \n %le \n %le \n %le \n %le \n %le \n",
               rhob,rhos,fpvs,mwgas,grb,grs);
      
       break;    
 
     case 1:;
      /* import: OIBIA6
         export: OIBEA6
      */

       if (argc < 29)
         { 
           printf("%s \n","ERROR:TO_FEW_PARAMETERS");
           break;
         };

       if (argc > 29)
         {
           printf("%s \n","ERROR:TO_MANY_PARAMETERS");
           break;
         };

       for (j=0;j<=20;j++) sscanf(argv[j+2], "%lf",&xi[j]);
       sscanf(argv[23], "%lf",&tf);
       sscanf(argv[24], "%lf",&pf);
       sscanf(argv[25], "%lf",&tb);
       sscanf(argv[26], "%lf",&pb);
       sscanf(argv[27], "%lf",&tgr);
       sscanf(argv[28], "%lf",&pgr);

       code=CALCDL(xi,&tf,&pf,&tb,&pb,&tgr,&pgr,
                   &ts,&ps,&zf,&zb,&zs,&rhotp,&rhob,&rhos,&fpvs,
                   &mwgas,&grgr,&grb,&grs);

       printf("%d \n %le \n %le \n %le \n %le \n %le \n %le \n",
              code,ts,ps,zf,zb,zs,rhotp);
       printf("%le \n %le \n %le \n %le \n %le \n %le \n %le \n", 
              rhob,rhos,fpvs,mwgas,grgr,grb,grs);
   
       break;


     case  2:;
       /* import: OIBIA9
          export: OIBEA9       
       */

       if (argc < 22)
         {
           printf("%s \n","ERROR:TO_FEW_PARAMETERS");
           break;
         };

       if (argc > 22)
         {
           printf("%s \n","ERROR:TO_MANY_PARAMETERS");
           break;
         };

       sscanf(argv[2], "%d ",&ntaps);
       sscanf(argv[3], "%lf",&pf);
       sscanf(argv[4], "%lf",&tf);
       sscanf(argv[5], "%d",&matorf);
       sscanf(argv[6], "%lf",&dox);
       sscanf(argv[7], "%lf",&torf);
       sscanf(argv[8], "%d",&matpipe);
       sscanf(argv[9], "%lf",&dm);
       sscanf(argv[10],"%lf",&tpipe);
       sscanf(argv[11],"%lf",&rhotp);
       sscanf(argv[12],"%lf",&rhos);
       sscanf(argv[13],"%lf",&hw);
       sscanf(argv[14],"%lf",&visc);
       sscanf(argv[15],"%lf",&kfac);
       sscanf(argv[16],"%d",&ifluid);
       sscanf(argv[17],"%d",&nploc);
       sscanf(argv[18],"%lf",&fpvs);
       sscanf(argv[19],"%lf",&grs);
       sscanf(argv[20],"%lf",&factr);
       sscanf(argv[21],"%lf",&zairs);

/* Standard temperature must be 519.67 DEG R (60 DEG FAH)
   Standard pressure must be 14.73 PSI                    */
       
       ts=60.0+459.67;
       ps=14.73; 


/*
      qv=-1.;
      doc=-1.;
      dmc=-1.;
      ev=-1.;
      cd=-1.;
      fn=-1.;
      fc=-1.;
      fsl=-1.;
      fa=-1.;
      cprm=-1.;
      y=-1.;
      fb=-1.;
      fr=-1.;
      fpb=-1.;
      ftb=-1.;
      ftf=-1.;
      fgr=-1.;
      cdflag=0;
      avgvel=-1.;
      red=-1.;
      beta=-1.;
*/

       code=ORIFICE(&ntaps,&pf,&tf,&matorf,&dox,&torf,&matpipe,&dm,
                    &tpipe,&rhotp,&rhos,&hw,&visc,&kfac,&ifluid,
                    &ts,&ps,&nploc,&fpvs,&grs,&factr,&zairs,
                    &qv,&doc,&dmc,&ev,&cd,&fn,&fc,&fsl,&fa,&cprm,
                    &y,&fb,&fr,&fpb,&ftb,&ftf,&fgr,&cdflag,&avgvel,
                    &red,&beta);

       printf("%d \n %le \n %le \n %le \n %le \n %le \n %le \n %le \n",
               code,qv,doc,dmc,ev,cd,fn,fc);
       printf("%le \n %le \n %le \n %le \n %le \n %le \n %le \n %le \n",
               fsl,fa,cprm,y,fb,fr,fpb,ftb);
       printf("%le \n %le \n %d \n %le \n %le \n %le \n",
               ftf,fgr,cdflag,avgvel,red,beta);
 
      
       break;    

     default : 
       printf("%s \n","ERROR:FUNCTION_NOT_FOUND");
       break;                               /* error message */
   }
return 0;
}

/*   Internal functions
 *   ================== */

static void help( void )
{
#define NL "\n"
   printf( NL
   "**********************************************"             NL
   "*                                            *"             NL 
   "*   AGASAP - Gas Calculation                 *"             NL
   "*   version 1.0.B used by IS-OIL             *"             NL
   "*                                            *"             NL 
   "**********************************************"             NL NL
   "Installed functions:"                                       NL
   "--------------------"                                       NL
   "1. CALCGS"                                                  NL
   "2. CALCDL"                                                  NL
   "3. ORIFICE"                                                 NL NL
   "Commandline:"                                               NL
   "------------"                                               NL 
   "Parameter 0:    call of the agasap program"                 NL
   "Parameter 1:    AGA function to be called"                  NL
   "Parameter 2..n: parameters of the AGA function"             NL NL
   "Example:"                                                   NL
   "agasap calcgs 2 0.581 0.003 0.006 0.0 0.0 270 6 288.15"     NL
   "              0.101325 288.15 0.101325 288.15 0.101325 288.15" NL NL
   "Help functionality:"                                        NL
   "-------------------"                                        NL NL
   "   agasap [-h | -H | help | HELP]"                          NL NL
	 );
   return;
 
}
