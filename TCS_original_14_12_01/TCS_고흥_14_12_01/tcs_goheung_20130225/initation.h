extern int  init_tcs(void); 
extern char CloseDevice(void);   
extern int  read_set_axispara(void);
extern int  detect_module(void);
extern int  set_axis_para(long laxis);

extern DWORD get_motion_mode(long lAxisNo, DWORD *upStatus);
extern DWORD read_motfile(char *fname);
extern DWORD save_motfile(char *fname);
extern DWORD set_axis_absolute(long lAxisNo);
extern DWORD set_axis_relative(long lAxisNo);
   
