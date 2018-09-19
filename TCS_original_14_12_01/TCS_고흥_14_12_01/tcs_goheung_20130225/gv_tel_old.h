extern int NO_OF_AXIS;
//========================================================
extern int  TELESCOPE_TYPE;
enum {_ALT_AZI, _RA_DEC};

extern int TELESCOPE_MODE;
enum {TEL_MODE_STOP, TEL_MODE_SLEW, TEL_MODE_TRACK, TEL_MODE_PARK, TEL_MODE_INIT};

extern int SLEW_MODE[3];
enum {_SLEW_MODE_ON,_SLEW_MODE_OFF};

extern int  GUIDE_MODE;
enum {GUIDE_MODE_OFF,GUIDE_MODE_HP,GUIDE_MODE_MOUSE,GUIDE_MODE_ST4};

extern d_m_s   SITE_LONGI,SITE_LATI;
extern double  SITE_HEIGHT;
extern double  DUT1_SEC,DUT1_DAY;

extern double  MOUNT_HA[20], MOUNT_DEC[20],ENC_TO_STEP[2];
extern double  PSI_FACTOR; // PSI_COUNT = PSI_DEGREE * PSI_FACTOR
extern long    INIT_COUNT[4], HOME_COUNT[4];
extern double  OFFSET_HA_COUNT,OFFSET_DEC_COUNT;
extern double  OFFSET_HA_ARCMINUTE,OFFSET_DEC_ARCMINUTE;
extern int     ENCODER_MODE;
extern double  SERVO_UPDATE_SEC;

extern double  MEAN_TRACKING_FREQ[4];
extern double  PARK_FREQ [4], PARK_SPEED [4],
               INIT_FREQ [4], INIT_SPEED [4],
               GUIDE_FREQ[4], GUIDE_SPEED[4],
               GSET_FREQ [4], GSET_SPEED [4],
               SLEW_FREQ [4], SLEW_SPEED [4];
extern switch_status SWSTATUS[4];
extern ramp_data VPROFILE[4];

extern char   STAR_STRING[150];
extern double MAX_ERROR_RA_ARCSEC, MAX_ERROR_DEC_ARCSEC;
extern double JDUTC_0;           

extern int   DER_TRACKING;
enum {DER_TRACKING_ON,DER_TRACKING_OFF};

extern int SW_LIMIT;

extern h_m_s  TARGET_RA_2000 ,CURRENT_RA ;
extern d_m_s  TARGET_DEC_2000,CURRENT_DEC;
extern char   TARGET_NAME[100];

extern h_m_s CURRENT_LAST;
extern int LINKDOME;

extern double HALIMITMAX,HALIMITMIN; 

extern ramp_data VPROFILE[4];
extern switch_status SWSTATUS[4];
extern home_para HOMEP[2];
extern int USEPRELOAD;
