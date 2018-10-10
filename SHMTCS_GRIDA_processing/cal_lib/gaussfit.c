#include <userint.h>
//-----------------------------------------------------------------------------
#include <ansi_c.h>
#include "type_def.h"
#include "jplcpabtsub.h"
#include "define_num.h"
#include "global_vari.h"
#include "calc_rv.h"
#include "rv_sub.h"

extern int main_handle;
extern void display_message(char buff[]);
//-----------------------------------------------------------------------------
int read_init_para(void);
int update_para(void);
int gauss_fit_1c(double y[], int ndata, int npara, double para[],double res[]);
int gauss_fit_2c(double y[], int ndata, int npara, double para[],double res[]);
int gauss_fit_3c(double y[], int ndata, int npara, double para[],double res[]);
int gaussfit_2c(void);
int parabola_fit(double para[], double *vc);

int    NPARA_FIT,NDATA_FIT,X0_FIT;
double PARA_FIT[NPMAX_FIT],ERROR_FIT[NPMAX_FIT];
double RESI_FIT[NDMAX_FIT];
int IX1,IX2;
//-----------------------------------------------------------------------------
double dy[MAX_ND_RESAMPLED],cmat[NDMAX_FIT][NPMAX_FIT],DPARA[NPMAX_FIT];
double qmat[NPMAX_FIT][NPMAX_FIT],aty[NPMAX_FIT],use;

static char buff[356];
//-----------------------------------------------------------------------------
int update_para()
//-----------------------------------------------------------------------------
{
    gauss_fit_3c(&BFCCF[X0_FIT],NDATA_FIT,NPARA_FIT,PARA_FIT,RESI_FIT);
    
    return 0;
}
//-----------------------------------------------------------------------------
int gauss_fit(double y[], int ndata, int npara,double para[],double res[])
//-----------------------------------------------------------------------------
//  y = a exp[-1/2 (x-xc)^2/sigma^2 ] + b
//  para[0] = a
//  para[1] = sigma
//  para[2] = xc
//  para[3] = b
//  dy/dsigma = a exp[] [(x-xc)^2/sigma^3]
//  dy/dxc    = a exp[] [(x-xc)/sigma^2  ]
//-----------------------------------------------------------------------------
{
 double dy[NDMAX_FIT],cmat[NDMAX_FIT][4], dpara[4];
 double qmat[4][5],aty[NDMAX_FIT],use;
 int i,j;
 double dx,q,qq,expq,xi;
 double scale,base,sigma,xc;

    if(ndata>NDMAX_FIT)
      {
       MessagePopup("gauss_fit error"," data points should be <= 100");
       return 1;
      }

    scale = para[0];
    sigma = para[1];
    xc    = para[2];
    base  = para[3];
    
    for(i=0;i<ndata;i++)
       {
        xi = i;
        dx    = xi-xc;
        q     = dx/sigma;
        qq    = q*q;
        expq  = exp(-qq/2.0);
        dy[i] = y[i] - (scale*expq+base);
        
        cmat[i][0] = expq;
        cmat[i][1] = scale*expq *qq/sigma;
        cmat[i][2] = scale*expq *q /sigma;
        cmat[i][3] = 1.0;
       }
       
    if(clsq(cmat[0],dy,qmat[0],aty,NDMAX_FIT,ndata,4,npara,dpara,res,&use)!=0)
      {
//     MessagePopup("","error in gauss fit");
       return 2;
      }
    
    for(i=0;i<4;i++) para[i] += dpara[i];

    return 0;
}
//-----------------------------------------------------------------------------
int gauss_fit_1c(double y[], int ndata, int npara, double para[],double res[])
//-----------------------------------------------------------------------------
//  y = a exp[-1/2 (x-xc)^2/sigma^2 ] + b
//  para[0] = a
//  para[1] = sigma
//  para[2] = xc
//  para[3] = b
//  dy/dsigma = a exp[] [(x-xc)^2/sigma^3]
//  dy/dxc    = a exp[] [(x-xc)/sigma^2  ]
//-----------------------------------------------------------------------------
{
 double dy[NDMAX_FIT],cmat[NDMAX_FIT][4],dpara[4];
 double qmat[4][4],aty[4],use;
 int i,j;
 double xi;
 double dx1,q1,qq1,expq1;
 double scale1,sigma1,xc1,base;

    if(ndata>NDMAX_FIT)
      {
       MessagePopup("gauss_fit error"," data points should be <= 100");
       return 1;
      }

    scale1 = para[0];
    sigma1 = para[1];
    xc1    = para[2];
    base   = para[3];
    
    for(i=0;i<ndata;i++)
       {
        xi = i*1.0+X0_FIT;
        
        dx1    = xi-xc1;
        q1     = dx1/sigma1;
        qq1    = q1*q1;
        expq1  = exp(-qq1/2.0);
        
        cmat[i][0] = expq1;
        cmat[i][1] = scale1*expq1 *qq1/sigma1;
        cmat[i][2] = scale1*expq1 * q1/sigma1;
        cmat[i][3] = 1.0;
        
        dy[i] = y[i] - (scale1*expq1 + base);
       }
       
    if(clsq(cmat[0],dy,qmat[0],aty,NDMAX_FIT,ndata,4,npara,dpara,res,&use)!=0)
      {
       MessagePopup("","error in gauss fit");
       return 2;
      }
    
    for(i=0;i<npara;i++)
       {
        para[i] += dpara[i];
        ERROR_FIT[i] = sqrt(qmat[i][i])*use;
       }
    
    sprintf(buff,"%7.3lf %7.3lf %7.3lf %7.3lf %7.3lf %7.3lf %7.3lf %7.3lf\n",
            dpara[0],dpara[1],dpara[2],dpara[3],dpara[4],dpara[5],dpara[6],use);
    display_message(buff);

    return 0;
}
//-----------------------------------------------------------------------------
int gauss_fit_2c(double y[], int ndata, int npara, double para[],double res[])
//-----------------------------------------------------------------------------
//  y = a exp[-1/2 (x-xc)^2/sigma^2 ] + b
//  para[0] = a
//  para[1] = sigma
//  para[2] = xc
//  para[3] = b
//  dy/dsigma = a exp[] [(x-xc)^2/sigma^3]
//  dy/dxc    = a exp[] [(x-xc)/sigma^2  ]
//-----------------------------------------------------------------------------
{
 int i,j;
 double xi;
 double dx1,q1,qq1,expq1;
 double dx2,q2,qq2,expq2;
 double scale1,sigma1,xc1,base;
 double scale2,sigma2,xc2;

    if(ndata>NDMAX_FIT)
      {
       MessagePopup("gauss_fit error"," data points should be <= 100");
       return 1;
      }

    base   = para[0];

    scale1 = para[1];
    sigma1 = para[2];
    xc1    = para[3];
    
    scale2 = para[4];
    sigma2 = para[5];
    xc2    = para[6];
    
    for(i=0;i<ndata;i++)
       {
        xi = i*1.0+X0_FIT;
        
        dx1    = xi-xc1;
        q1     = dx1/sigma1;
        qq1    = q1*q1;
        expq1  = exp(-qq1/2.0);
        
        dx2    = xi-xc2;
        q2     = dx2/sigma2;
        qq2    = q2*q2;
        expq2  = exp(-qq2/2.0);

        cmat[i][0] = 1.0;
        
        cmat[i][1] = expq1;
        cmat[i][2] = scale1*expq1 *qq1/sigma1;
        cmat[i][3] = scale1*expq1 * q1/sigma1;
        
        cmat[i][4] = expq2;
        cmat[i][5] = scale2*expq2 *qq2/sigma2;
        cmat[i][6] = scale2*expq2 * q2/sigma2;
        
        dy[i] = y[i] - (scale1*expq1 + scale2*expq2 + base);
       }
       
    if(clsq(cmat[0],dy,qmat[0],aty,NDMAX_FIT,ndata,NPMAX_FIT,npara,DPARA,res,&use)!=0)
      {
       MessagePopup("","error in gauss fit");
       return 2;
      }
    
    for(i=0;i<npara;i++)
       {
        para[i] += DPARA[i];
        ERROR_FIT[i] = sqrt(qmat[i][i])*use;
       }
    
    
    for(i=1;i<NPARA_FIT;i++) {sprintf(buff,"%7.3lf",DPARA[i]); display_message(buff);}
    display_message("\n");
    
    return 0;
}
//-----------------------------------------------------------------------------
int gaussfit_2c()
//-----------------------------------------------------------------------------
//  y = a exp[-1/2 (x-xc)^2/sigma^2 ] + b
//  para[0] = a
//  para[1] = sigma
//  para[2] = xc
//  para[3] = b
//  dy/dsigma = a exp[] [(x-xc)^2/sigma^3]
//  dy/dxc    = a exp[] [(x-xc)/sigma^2  ]
//-----------------------------------------------------------------------------
{
 double cmat[NDMAX_FIT][NPMAX_FIT],dpara[NPMAX_FIT];
 double qmat[NPMAX_FIT][NPMAX_FIT],aty[NPMAX_FIT],use;
 int i,j,k;
 double xi;
 double dx1,q1,qq1,expq1;
 double dx2,q2,qq2,expq2;
 double scale1,sigma1,xc1,base;
 double scale2,sigma2,xc2;
 double temp;
 double dy[MAX_ND_RESAMPLED],resi[MAX_ND_RESAMPLED];
 int nPARA_FIT = 6;
 
    scale1 = PARA_FIT[0];
    sigma1 = PARA_FIT[1];
    xc1    = PARA_FIT[2];
    
    scale2 = PARA_FIT[3];
    sigma2 = PARA_FIT[4];
    xc2    = PARA_FIT[5];
    
//  base   = PARA_FIT[6];
    base   = 0;
    
    for(i=IX1;i<=IX2;i++)
       {
        xi = i*1.0;
        
        dx1    = xi-xc1;
        q1     = dx1/sigma1;
        qq1    = q1*q1;
        expq1  = exp(-qq1/2.0);
        
        dx2    = xi-xc2;
        q2     = dx2/sigma2;
        qq2    = q2*q2;
        expq2  = exp(-qq2/2.0);
        
        cmat[i][0] = expq1;
        cmat[i][1] = scale1*expq1 *qq1/sigma1;
        cmat[i][2] = scale1*expq1 * q1/sigma1;
        
        cmat[i][3] = expq2;
        cmat[i][4] = scale2*expq2 *qq2/sigma2;
        cmat[i][5] = scale2*expq2 * q2/sigma2;
        
        cmat[i][6] = 1.0;
        
        BFCCF[i] = (scale1*expq1 + scale2*expq2 + base);
       }
//  calculate dy
    for(i=0;i<NN;i++)
       {
        temp = 0;
        for(k=IX1;k<=IX2;k++) temp += DM[i*MM+k]*BFCCF[k];
        dy[i] = YP_OBJ[i+MP] - temp;
       }
        
//  calculate condition matrix
    for(i=0;i<NN;i++)
    for(j=0;j<nPARA_FIT;j++)
       {
        temp = 0;
        for(k=IX1;k<=IX2;k++) temp += DM[i*MM+k]*cmat[k][j];
        U[i*NPMAX_FIT+j] = temp;
       }
       
    if(clsq(U,dy,qmat[0],aty,NN,NN,NPMAX_FIT,nPARA_FIT,dpara,resi,&use)!=0)
      {
       MessagePopup("","error in gauss fit");
       return 2;
      }
    
    for(i=0;i<nPARA_FIT;i++)
       {
        PARA_FIT[i] += dpara[i];
        ERROR_FIT[i] = sqrt(qmat[i][i])*use;
       }
    
    sprintf(buff,"%7.3lf %7.3lf %7.3lf %7.3lf %7.3lf %7.3lf %7.3lf %7.3lf\n",
            dpara[0],dpara[1],dpara[2],dpara[3],dpara[4],dpara[5],dpara[6],use);
    display_message(buff);
    
//    sprintf(buff,"%7.3lf %7.3lf %7.3lf %7.3lf %7.3lf %7.3lf %7.3lf %7.3lf\n",
//            PARA_FIT[0],PARA_FIT[1],PARA_FIT[2],PARA_FIT[3],PARA_FIT[4],PARA_FIT[5],PARA_FIT[6],use);
//    display_message(buff);
    
    
    return 0;
}
//-----------------------------------------------------------------------------
int gauss_fit_3c(double y[], int ndata, int npara, double para[],double res[])
//-----------------------------------------------------------------------------
//  y = a exp[-1/2 (x-xc)^2/sigma^2 ] + b
//  para[0] = a
//  para[1] = sigma
//  para[2] = xc
//  para[3] = b
//  dy/dsigma = a exp[] [(x-xc)^2/sigma^3]
//  dy/dxc    = a exp[] [(x-xc)/sigma^2  ]
//-----------------------------------------------------------------------------
{
 int i,j;
 double xi;
 double base;
 double scale1,sigma1,xc1,dx1,q1,qq1,expq1;
 double scale2,sigma2,xc2,dx2,q2,qq2,expq2;
 double scale3,sigma3,xc3,dx3,q3,qq3,expq3;

    if(ndata>NDMAX_FIT)
      {
       MessagePopup("gauss_fit error"," data points should be <= 100");
       return 1;
      }

    base   = para[0];

    scale1 = para[1];
    sigma1 = para[2];
    xc1    = para[3];
    
    scale2 = para[4];
    sigma2 = para[5];
    xc2    = para[6];
    
    scale3 = para[7];
    sigma3 = para[8];
    xc3    = para[9];
    
    for(i=0;i<ndata;i++)
       {
        xi = i*1.0+X0_FIT;
        
        dx1    = xi-xc1;
        q1     = dx1/sigma1;
        qq1    = q1*q1;
        expq1  = exp(-qq1/2.0);
        
        dx2    = xi-xc2;
        q2     = dx2/sigma2;
        qq2    = q2*q2;
        expq2  = exp(-qq2/2.0);
        
        dx3    = xi-xc3;
        q3     = dx3/sigma3;
        qq3    = q3*q3;
        expq3  = exp(-qq3/2.0);

        cmat[i][0] = 1.0;
        
        cmat[i][1] = expq1;
        cmat[i][2] = scale1*expq1 *qq1/sigma1;
        cmat[i][3] = scale1*expq1 * q1/sigma1;
        
        cmat[i][4] = expq2;
        cmat[i][5] = scale2*expq2 *qq2/sigma2;
        cmat[i][6] = scale2*expq2 * q2/sigma2;
        
        cmat[i][7] = expq3;
        cmat[i][8] = scale3*expq3 *qq3/sigma3;
        cmat[i][9] = scale3*expq3 * q3/sigma3;
        
        dy[i] = y[i] - (scale1*expq1 + scale2*expq2+ scale3*expq3 + base);
       }
       
    if(clsq(cmat[0],dy,qmat[0],aty,NDMAX_FIT,ndata,NPMAX_FIT,npara,DPARA,res,&use)!=0)
      {
       MessagePopup("","error in gauss fit");
       return 2;
      }
    
    for(i=0;i<npara;i++)
       {
        para[i] += DPARA[i];
        ERROR_FIT[i] = sqrt(qmat[i][i])*use;
       }
    
    for(i=1;i<NPARA_FIT;i++) {sprintf(buff,"%7.3lf",DPARA[i]); display_message(buff);}
    display_message("\n");
    
    return 0;
}
//-----------------------------------------------------------------------------
int parabola_fit(double para[], double *vc)
//-----------------------------------------------------------------------------
{
 double cmat[NDMAX_FIT][NPMAX_FIT];
 double qmat[NPMAX_FIT][NPMAX_FIT],aty[NPMAX_FIT],use;
 double resi[MAX_ND_RESAMPLED];
 double xi;
 int i,ndata,npara;
 double xx[100],yy[100],xc;
 
    for(i=IX1;i<=IX2;i++)
       {
        xi = i*1.0;
        
        cmat[i-IX1][0] = xi*xi;
        cmat[i-IX1][1] = xi;
        cmat[i-IX1][2] = 1;
       }
       
    ndata = IX2-IX1+1;
    npara = 3;
       
    if(clsq(cmat[0],&BFCCF[IX1],qmat[0],aty,NDMAX_FIT,ndata,NPMAX_FIT,npara,para,resi,&use)!=0)
      {
       MessagePopup("","error in gauss fit");
       return 2;
      }
      
    xc = -para[1]/(2.0*para[0]);
    
    for(i=0;i<NDATA_FIT;i++) {xx[i]=X0_FIT+i; yy[i]=BFCCF[i+X0_FIT]-RESI_FIT[i];}
    
    PlotXY(main_handle,MAIN_GRAPHCCF,xx,yy,NDATA_FIT,VAL_DOUBLE,VAL_DOUBLE,VAL_SCATTER,
            VAL_EMPTY_SQUARE, VAL_SOLID, 1, VAL_RED);
            
    *vc = (xc-MP)*DELV_KM;
//  ev1= ERROR_FIT[3]*DELV_KM;
    sprintf(buff,"dv = %10.3lf km/s\n",*vc);
    display_message(buff);
    
    return 0;
}
//-----------------------------------------------------------------------------
