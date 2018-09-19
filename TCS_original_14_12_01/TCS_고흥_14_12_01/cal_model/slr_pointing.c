//----------------------------------------------------------------------------
// function to read pointing parameters
int read_pointing_para(char file_name[], double para[])
//----------------------------------------------------------------------------
{
 FILE *fin;
 char ssss[300];
 int i;
 
//  open the file to contial pointing model
//  The noumber of pointing parameters is confined to six right now.
//  It may be expaded later.

    if((fin = fopen(ssss,"r"))==NULL) return -1; // file open error
    for(i=0;i<6;i++)
       {
        fgets(ssss,100,fin);
        sscanf(ssss,"%lf",&para[i]);
       }
    fclose(fin);
    
    return 0;
}
//----------------------------------------------------------------------------
// function to calculate delta altitude and azimuth for given altitude, azimuth, and point model parameters
int calc_dalt_dazi(double para[], double altdeg, double azideg, double *daltdeg, double *dazideg)
//----------------------------------------------------------------------------
{
// input 
// para[]: array to contain poijt model parameters
// altdeg: altitude, uint in degree
// azideg: azimuth , uint in degree

// output
// daltdeg: delta altitude in degree
// dazideg: delta azimuth  in degree
//

double altrad,azirad,pi314;

//  convert degree to radian    
    pi314  = acos(-1.0);
    altrad = altdeg*pi314/180.0;
    azirad = (azideg-180.0)*pi314/180.0;
    
//  calculate delta azimuth in degree    
    *dazideg = para[0]*
              +para[2]*tan(altrad)*sin(azirad)
              +para[3]*tan(altrad)*cos(azirad)
              +para[4]*tan(altrad)
              +para[5]/cos(altrad);
         
             
//  calculate delta altitude in degree    
    *dazideg = para[1]
              +para[2]*cos(azirad)
              -para[3]*sin(azirad)
              +para[4];
return 0;
}
//----------------------------------------------------------------------------
