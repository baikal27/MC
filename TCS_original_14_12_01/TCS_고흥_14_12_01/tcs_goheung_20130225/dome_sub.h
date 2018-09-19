extern int dome_cw(void); 
extern int dome_ccw(void); 
extern int dome_stop(void); 
extern int upper_slit_open (void);
extern int upper_slit_close(void);
extern int upper_slit_stop (void);

extern int lower_slit_open (void);
extern int lower_slit_close(void);
extern int lower_slit_stop (void);

extern int lamp_on (void);
extern int lamp_off(void);

extern int read_dome_home(int *DOME_HOME_SWITCH);
extern int read_dome_encoder(long *encoder);
extern int set_dome_encoder (long  encoder);

extern int init_dome_tcs(void);

extern int set_domeold(void);
extern int save_domeold(void); 
