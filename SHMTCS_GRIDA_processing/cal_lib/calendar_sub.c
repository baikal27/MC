///////////////////////////////////////////////////////////////////////////////////////////////
//	program name : CALSUB.for
//	CALSUB���� �޷��� ����� �� ������ �� ���α׷����� ��� �ִ�.
//	���� ����� ��ȯ ���α׷��� ���Ῡ �����Ѵ�. 
//	SCTOMC (Solar Calendar To Moon Calendar)�� ��� ��¥�� ���� ��¥�� ��ȯ�Ͽ� �ְ�, 
//	MCTOSC (Moon Calendar To Solar Calendar)�� ���� ��¥�� ��� ��¥�� ȯ���Ͽ� �ش�.
//	�� ���α׷� ��� ����� ȯ���� ���� ����Ÿ ������ �ʿ�� �Ѵ�.
//	���� �� ����Ÿ ���� �̸��� SMCALCON.INP�� �������� c:\cal\lib ���丮�� ����ȴ�.
//	SCTOMC�� MCTOSC�� �θ��� ���� �ݵ�� ������ ���� SMCALCON.INP�� ����,
//	�� ������ �д� ä�� ��ȣ iw�� �����α׷��� �����Ͽ��� �Ѵ�.
//	open(iw,file='c:\cal\lib\smcalcon.inp',form='binary',status='old')

//������ 
//        subroutine sctomc(year,month,day,iw, yy,mm,dd,mganji)
//        subroutine mctosc(year,month,day,iw, yy,mm,dd)
//���Է� �Ķ����
//	integer year,month,day : �Է� ��¥. SCTOMC (MCTOSC)�� ��� ��(��)�� ��¥
//        integer iw : �Է� ����Ÿ ���� SMCALCON.INP ���� ä�� ��ȣ
//����� �Ķ����
//	integer yy,mm,dd : �Է� ��¥�� �����Ǵ� ��(��)�� ��¥
//	integer mganji : SCTOMC���� �������� ȯ��� ���� ���� ��ȣ
//
// ������ ��Ÿ CALSUB�� �ִ� ��Ÿ �����α׷����� ������ �����Ѵ�.
//
//����� ���� ��¥��, ���� ��ȣ ��� ���α׷�
//   integer function dayofmon(year,month) : �ش� �� (���)�� ��¥ ��
//   integer function fundate(year,month,day) : �ش� ��¥�� ���� ��ȣ.
//             (��,��, ... ,��) ===> (1,2, ... ,7)
//
//������, ����, ���� ��ȣ ��� ���α׷�. �� ���� ��ȣ�� �����ϴ� 60 ������ �Ʒ�
//		gethname�� ������ ��.
//        integer function fundgg(year,month,day) : �ش� ��¥�� ���� ��ȣ
//        integer function funmgg(year,month) : �ش� ���� ���� ��ȣ
//        integer function funygg(year) : �ش� ���� ���� ��ȣ
//
//��60���� �ʱ�ȭ ���α׷�
//        subroutine gethname(hnamegg)
//        character*4 hnamegg(0:60)
//	��� : hnamegg(0) = '����'
//               hnamegg(1) = '����', ......, hnamegg(60) = '����'
//////////////////////////////////////////////////////////////////////////////////////////////

#include<stdio.h>
#include<stdlib.h>
#include<math.h>
#include<string.h>
//#include "c:\\lib\\cal_lib\\type_def.h"
#include "ssbsub.h"
#include "cpabtsub.h"
#include "mathsub.h"
/***********************************************************************************************/
void cal_con(int y1, int m1, int d1, int y2, int m2, int d2, int indx)
//This routine converts a lunar calendar date to the corresponding solar calendar date and
//vice versa.
//input : y1, m1, d1(solar calendar date)
//		  y2, m2, d2(lunar calendar date)
//		  indx
//At the exit of the routine
//	if(indx = 1) (y2,m2,d2) is assigned to the lunar calendar date
//						corresponding to (y1, m1, d1)
//	if(indx = 2) (y1, m1, d1) is assigned to the solar calendar date
//						corresponding to (y2, m2, d2)
//
//Note : In the main program do not use "74" for I/O channel no.
//		 "74" is used in the routine to read the input data file
//		 'c:\cal\lib\cal_con.dat'.
{
	FILE *in;
	
	int i, jdi;
	static int ijd[2400], ly[2400], lm[2400];
	static int first, ii;
	double fjd, ut;

	const char* file="cal_con.dat";	
	
	first = 0;

	if(first == 0)
	{
		first = 1;
		ii = 0;
	
		if((in=fopen(file, "r")) == NULL)
		{
			printf("Could not open %s file..\n", file);
		}
	
		fscanf(in, "%d", &ii);

		for( i =0; i< ii; i++)
			fscanf(in, "%d, %d %d", &ly[i], &lm[i], &ijd[i]);
	
		if(fclose(in) !=0)
			fprintf(stderr, "Error in closing files..\n");
	}

	if(indx == 2)
	{
		for(i = 0; i < ii;i++)
		{
			if(y2 == ly[i] && m2 == lm[i])
			{
				fjd = ijd[i] + 0.6 + d2 - 1.0;
				jdtoc(fjd, 0, &y1, &m1, &d1, &ut);
				return ;
			}
		}
		printf("epoch out of range....\n");
		return ;
	}

//	jdi = (int)get_jd (y1, m1, d1, 1.0);
	jdi = (int)get_jd(y1, m1, d1, 1.0);

	for(i=0; i < ii-1; i++)
	{
		if(jdi >= ijd[i] && jdi < ijd[i+1])
		{
			y2 = ly[i];
			m2 = lm[i];
			d2 = jdi - ijd[i] +1;
			return ;
		}
	}
		printf("epoch out of range...\n");
		return ;

}
/***********************************************************************************************/	


/***********************************************************************************************/
void sctomc(int year, int month, int day, FILE *iw, int *yy, int *mm, int *dd, char *mganji)
//This routine convert the solar calendar date to lunar calendar date.
//Before calling this routine you should open the input data file in the main program as follows.
//
//open(iw, file=fname, form ='binary', status ='old')
//
//where fname is usually 'smcalcon.inp'.
//
//input : year, month, day -> solar calendar date
//		  iw -> data file channel
//		  fname -> input data file name, usually 'smcalcon.inp'
//
//output : yy, mm, dd -> lunar calendar date
//  	   mganji -> ganji for the month
//Note : If the input date is out of range of the data file, dd is to set -1
{
	int ijd, mm1, mm2, i;
	static int ijd1, ijd2;
	ijd1 = -1;

//	ijd = (int)(get_jd (year, month, day,0.0)+0.1);
	ijd = (int)(get_jd(year, month, day,0.0)+0.1);

	if(ijd >= ijd1 && ijd < ijd2)
	{
		*mm = mm1;
		*dd = ijd - ijd1 + 1;
		return;
	}

	if(ijd1 < ijd)
	{
		rewind(iw);
		for(i = 0 ; i< 200; i++)
		fscanf(iw, "%d %d %d %s", &ijd1, yy, &mm1, mganji);
		if(ijd1 > ijd)
		{
			*dd = -1;
			ijd1 = -1;
			return;
		}
	}
	
		do { 
		for(i=0 ; i < 200 ; i++)
			fscanf(iw, "%d %d %d %s", &ijd1, yy, &mm2, mganji);
		if(ijd >= ijd1 && ijd < ijd2)
		{
			*mm = mm1;
			*dd = ijd - ijd1 + 1;
			return;
		}
		
		mm1 = mm2;
		ijd1 = ijd2;
		} while(1);
	
}
/***********************************************************************************************/



/***********************************************************************************************/
void mctosc(int year, int month, int day, FILE *iw, int *yys, int *mms, int *dds)
//This routine convert the lunar calendar date to solar calendar date.
//Before calling this routine you should open the input data file in the main program as follows.
//
//open(iw, file = 'fname', form = 'binary', status = 'old')
//
//where fname is usually 'smcalcon.inp'.
//
//The input data file smcalcom.inp contains the data in binary format for lunar<-->solar calendar conversion
//
//input : year, month, day -> lunar calendar date
//iw : data file channel
//
//output : yys, mms, dds -> solar calendar date
//
//Note : If the input date is out of range of the data file, dd is to set -1

{
	static int yy, mm;
	int dd;
	long in[5];
	static char mganji[6];

	static int ijd;
	double fjd, rhour;

	yy = -10000;

	if(yy == year && mm == month)	
	{
		fjd = ijd + day + 0.1;
		jdtoc(fjd, 0, yys, mms, dds, &rhour);

		return;
	}

	if((year > yy) || (year == yy && month > mm))
		rewind(iw);
	
	do{
//		if(fscanf(iw,"%d %d %d %s", &ijd, &yy, &mm, &mganji) != 4) break;
		fread(in, sizeof(int), 3, iw);
		fread(mganji, sizeof(char), 4, iw);

		ijd = in[0];
		yy = in[1];
		mm = in[2];

		if(yy > year)
		{
			rewind(iw);

		fread(in, sizeof(int), 2, iw);
		fread(mganji, sizeof(char), 4, iw);
		ijd = in[0];
		yy = in[1];
		mm = in[2];
			
//		if(fscanf(iw, "%d %d %d %s", &ijd, &yy, &mm, mganji) != 4) break;

			if(yy > year) break;

		}

		if(yy == year && mm==month)
		{
			fjd = ijd + day + 0.1;
			jdtoc(fjd, 0, yys, mms, dds, &rhour);

			return;
		}
	} while(mm != 13);

	dd = -1;

	return;
}

/***********************************************************************************************/



/***********************************************************************************************/	
int dayofmon(int year, int month)
//Function to return # of days in a month of year
{
	int result;

	int day, ndayt[13]={0,31,29,31,30,31,30,31,31,30,31,30,31};
	const double ut = 0.0;

	if(month != 2)
		result = ndayt[month];
	else
	{
		day = 1;
//		result = (int)(get_jd (year, month+1, day, ut) - get_jd (year, month, day, ut) + 0.1);
		result = (int)(get_jd(year, month+1, day, ut) - get_jd(year, month, day, ut) + 0.1);
	}

	return result;
}
/***********************************************************************************************/



/***********************************************************************************************/
int fundate(int year, int month, int day)
//This routine calculate the date no. such that 1,2,......,7 for Sunday, Monday, ...., Saturday
{
	int ii, result;
	double rjd, temp;
	const double ut = 0.0;
	//JD of 1991, 11, 10 Sunday
	const double rjd0 = 2448570.5;
	
//	rjd = funjd (year, month, day, ut);
	rjd = get_jd(year, month, day, ut);
	temp = rjd - rjd0;
	ii = (int)(temp / 7.0);
	temp = temp - (ii*7.0);
	if(temp < 0.0)
		temp = temp + 7.0;
	result = (int)(temp + 1.1);

	if(result < 0)
		result = result + 7;

	return result;
}
/***********************************************************************************************/



/***********************************************************************************************/
int fundgg(int year, int month, int day)
//This routine calculate daily ganji no.
{
	int ii, result;
	double rjd, temp;
	const double ut = 0.0;
	//JD of 1991 10 21 GAPJA
	const double rjd0 = 2448550.5;

//	rjd = funjd (year, month, day, ut);
	rjd = get_jd(year, month, day, ut);

    temp = rjd - rjd0;
	ii = (int)(temp / 60.0);
	temp = temp - (ii*60.0);
	if(temp < 0)
		temp = temp + 60.0;
	result = (int)(temp + 1.1);
	if(result < 0)
		result = result + 60;

	return result;
}
/***********************************************************************************************/





/***********************************************************************************************/
int funmgg(int year, int month)
//This routine calculate monthly ganji no.
{
	int ii, result;
	double temp;
	//���� 1992���� 1���� ���� : ����(39)
	const double ii0 = 1992.0*12.0 + 1.0;
	
	temp = (year * 12.0 + month) - ii0 + 39.1;
	ii = (int)(temp / 60.0);
	temp = temp - (ii*60.0);
	if(temp < 0.0)
		temp = temp + 60.0;
	result = (int)(temp + 0.1);
	if(result > 60)
		result = result - 60;
	if(result == 0)
		result = 60;

	return result;
}
/***********************************************************************************************/



/***********************************************************************************************/
int funygg(int year)
//This routine calculate yearly ganji no.
{
	int ii, result;
	double temp;
	//1984�� ���� : ����
	const int y0 = 1984;
	
	temp = year - y0 + 0.1;
	ii = (int)(temp / 60.0);
	temp = temp - (ii*60.0);
	if(temp < 0.0)
		temp = temp + 60.0;
	result = (int)temp + 1;
	if(result > 60)
		result = result - 60;
	if(result == 0)
		result = 60;

	return result;
}
/***********************************************************************************************/



/***********************************************************************************************/
void gethname(char *hnamegg[])
//�̰� ¥�µ� 3�ð��̳� �����߳�.. �ð� �Ʊ����..�Ѥ�;;;
{
	char *gan[11] = {"\n","��", "��", "��", "��", "��", "��", "��", "��" , "��", "��"};
	char *ji[13]={"\n", "��", "��", "��", "��", "��", "��", "��", "��", "��", "��", "��", "��"};

	int i,j,n;

	char buffer[80];
	
	
	//60���� �̸� ���ϱ�...
	for(n = 1; n <= 60; n++)
	{
		i = (int)fmod(n, 10);
		if(i == 0)
			i = 10;
	
		j = (int)fmod(n,12);
		if(j == 0)
			j = 12;

		strcpy(buffer, gan[i]);
		strcat(buffer, ji[j]);

		hnamegg[n] = (char *)malloc(strlen(buffer) + 1);

		strcpy(hnamegg[n], buffer);
	}

	hnamegg[0] = "����";

}
/***********************************************************************************************/



/***********************************************************************************************/
void towang(int year, double jd[])
//funroot()�� �����μ��� slong�� ���ؼ� �˾ƺ��� ��.�Ѥ�
//Subroutine to calculate 4 TOWANG YONGSA epoch (KST)
//input : year
//output : jd[4], epoch for each towang yongsa in KST
{
	double theta[4];
	double x1, x2, f0;
	const double facc = 1.0/206265.0;

	double slong, time;

	int i;

	//ecliptic lingitude for each TOWANG YONGSA
	theta[0] = deg_to_rad(297.0);
	theta[1] = deg_to_rad(27.0);
	theta[2] = deg_to_rad(117.0);
	theta[3] = deg_to_rad(207.0);

	//approximate epoch for each TOWANG YONGSA
	jd[0] = get_jd(year, 1, 17, 0.0);
	jd[1] = get_jd(year, 4, 17, 0.0);
	jd[2] = get_jd(year, 7, 20, 0.0);
	jd[3] = get_jd(year, 10, 20, 0.0);

	for(i = 0 ; i < 4 ; i++)
	{
		x1 = jd[i] - 5.0;
		x2 = jd[i] + 5.0;

		f0 = theta[i];

//		time  = funroot(slong, f0, x1, x2, facc);
		time  = funroot(       f0, x1, x2, facc);
		jd[i] = tdttokst(time);
	}

}
/***********************************************************************************************/



/***********************************************************************************************/
void hansig(int year, FILE *iw, int *month, int *day)
//Sunroutine to calculate the date of hansig.
//Hansig is defined as the 105th day after Dongji.
//
//input : year, iw -> input channel no. for SS24.DAT
//output : month, day -> date of Hansig of year
//
//"IMPORTANCE" : You should open SS24.DAT before clling this routine.
//SS24.DAT is used to find the date of Dongji.
{
	int yyyy, nn;
	int month1, day1,month2, day2;
	double jd, rday, temp;

	//find the date of Dongji in the previous year
	rewind(iw);

	do { 
		fscanf(iw,"%d %d %d %lf", &nn, &yyyy, &month1, &rday);
	if(nn == 22 && yyyy == (year - 1)) break;

	} while(month1 != 13);

		day1 = (int)rday;
		//julian day of dongji
		jd = get_jd(yyyy, month1, day1, 0.1);

		//Hansig is 105 day after Dongji
		jd = jd + 105.0;

		//convert jd to calendar date
		jdtoc(jd, 0, &yyyy, &month2, &day2, &temp);

		*month = month2;
		*day = day2;

	return;
}
/***********************************************************************************************/



/***********************************************************************************************/
void sambok(int year, FILE *iw, int month[], int day[])
//Subroutine to calculate the dates of Sambok.
//
//input : year, iw -> input channel no. for SS24.DAT
//output : month[3], day[3] -> dates of Chobok, Chungbok, Malbok
//
//"IMPORTANT" : You should open SS24.DAT before clling this routine.
//SS24.DAT is used to find the date of Haji & Ipchu.
{
	int yy, mm, dd, nn;
	double jd, jd0, rday, temp;
	int month1, day1, month2, day2;

	rewind(iw);
	
	do {
	//find the date of Haji
	fscanf(iw,"%d %d %d %lf", &nn, &yy, &mm, &rday);
	if(nn == 10 && yy == year) break;
	} while(mm != 13);

	dd = (int)(rday);

	//Julian day of Haji
	jd = get_jd(yy, mm, dd, 0.1);

	//calculate Haji's IIJIN
	nn = fundgg(yy, mm, dd);

	nn = (int)(7 - fmod(nn, 10));	
	if(nn < 0)
		nn = nn + 10;

	//jd0 is the first Kyungil after Haji(including Haji)

	jd0 = jd + nn;

	//Chobok
	jd = jd0 + 20.0;
	jdtoc(jd, 0, &yy, &month1, &day1, &temp);
	
	month[0] = month1;
	day[0] = day1;

	//Chungbok
	jd = jd0 + 30.0;
	jdtoc(jd, 0, &yy, &month2, &day2, &temp);

	month[1] = month2;
	day[1] = day2;

	//Next, Ipchu
	
	do {
	//find the date of Ipchu
	fscanf(iw,"%d %d %d %lf", &nn, &yy, &mm, &rday);
	if(nn == 13 && yy == year) break;
	} while(mm != 13);

	dd = (int)(rday);
	
	//julian date of Ipchu
	jd = get_jd(yy, mm, dd, 0.1);

	//calculate Ipchu's IIJIN
	nn = fundgg(yy, mm, dd);
	nn = (int)(7 - fmod(nn, 10));
	if(nn < 0)
		nn = nn + 10;

	//jd0 is the first Kyungil after Ipchu (including Ipchu). ex->Malbok
	jd0 = jd + nn;

	jdtoc(jd0, 0, &yy, &month[2], &day[2], &temp);
}
/***********************************************************************************************/



/***********************************************************************************************/
int dayofsun(int year)
//Function to calculate the # of syndays in year
{
	int nn;
	double jd0, jd1;
	double result;

	nn = 7 - fundate(year, 1, 1);
	//JD of first sunday
	jd0 = get_jd(year, 1, 1, 0.1) + nn;
	nn = fundate(year, 12, 31) -7;
	if(nn < 0)
		nn = nn +1;

	//JD of last Sunday
	jd1 = get_jd(year, 12, 31, 0.2) -nn;

	result = (jd1 - jd0)/7.0 + 1.0;
	return result;
}
/***********************************************************************************************/

//--------------------------------------------------------------------------//
//			subroutine getmymm(no24, nnn, year, month, myear, mmon)			//
//--------------------------------------------------------------------------//
//	This routine determines lunar year and lunar month						//
//--------------------------------------------------------------------------//
void	getmymm(int no24[], int nnn, int year, int month, int *myear, int *mmon)
{
	int ii, i;

	*mmon = -100;

//printf("nnn = %d no24 = %d\n", nnn, no24[1]);

	for(i = 1; i <= nnn; i ++)
	{
		ii = (int)(no24[i] / 2.0);
		if(ii > *mmon) *mmon = ii;
	}

	if(*mmon > month)
	{
		*myear = year - 1;
	}
	else
	{
		*myear = year;
	}


	if(nnn == 1 && (no24[1] / 2.0) > (*mmon * 1.0)) *mmon = -*mmon;

	if(*myear == 1965 && *mmon == -8) *mmon = 9;
	if(*myear == 1984 && *mmon == 12) *mmon = 11;
	if(*myear == 1985 && *mmon == 1)
	{
		*myear = 1984;
		*mmon = 12;
	}
	if(*myear == 1985 && *mmon == -1) *mmon = 1;
	if(*myear == 2033 && *mmon == -1) *mmon = 11;
	if(*myear == 2034 && *mmon == -1) *mmon = 1;

	return;
}
//--------------------------------------------------------------------------//
//					subroutine ss24(y, m, d, eflag, hnamegg)				//
//--------------------------------------------------------------------------//
void ss24(FILE *iw1, FILE *iw2, int y[], int m[], double d[], int eflag, char hnamegg[])
{
	int day;
	int year,month;
	static int nnn = 0;
	static int no24[4];
	static int y24[5];
	static int m24[5];
	static double d24[5];
	double ut, jd, jd0, jd1, rday;
	int hour;
	double minute;
	int ii, i;
	int myear, mmon, in;

	day = (int)(d[0]);

	ut = 0.0;
	jd0 = get_jd(y[0], m[0], day, ut);

	day = (int)(d[4]);

	ut = 0.0;
	jd1 = get_jd(y[4],m[4], day, ut);

	do
	{
		if(fscanf(iw1, "%d %d %d %lf\n", &ii, &year, &month, &rday) != 4) goto first;

		day = (int)rday;
		ut = (rday - day) * 24.0;
		jd = get_jd(year, month, day, ut);

		if(jd > jd1)
		{

			getmymm(no24, nnn, year, month, &myear, &mmon);

			if(mmon > 0) in = funmgg(myear, mmon);

			else in = 0;

			i = 0;

			getdhms(d[i], &day, &hour, &minute);

			fprintf(iw2, "%2d %5d %3d %3d %3d %5.1f %5d %4d\n", 
				i, y[i], m[i], day, hour, minute, myear, mmon);

//printf("%2d %5d %3d %3d %3d %5.1f %5d %4d\n", i, y[i], m[i], day, hour, minute, myear, mmon);

			for(i = 1; i <= 3; i ++)
			{
				getdhms(d[i], &day, &hour, &minute);

			fprintf(iw2, "%2d %5d %3d %3d %3d %5.1f \n", 
				i, y[i], m[i], day, hour, minute);
			}

			nnn = 1;
			no24[nnn] = ii;
			y24[nnn] = year;
			m24[nnn] = month;
			d24[nnn] = rday;

			return;
		}

		if((jd >= jd0) && (jd < jd1))
		{
			nnn ++;
			no24[nnn] = ii;
			y24[nnn] = year;
			m24[nnn] = month;
			d24[nnn] = rday;

		}
		
	}while (month != 13);

first:	eflag = -1;
	return;
}

//--------------------------------------------------------------------------//







