extern DWORD set_axis_absolute(long lAxisNo);
extern DWORD set_axis_relative(long lAxisNo);

extern DWORD read_counter   (char caxis, long *lParameter);
extern DWORD read_encoder   (char caxis, long *lParameter);
extern DWORD read_cw_limit  (char caxis, int *state);
extern DWORD read_ccw_limit (char caxis, int *state);
extern DWORD read_home_limit(char caxis, int *state);
extern DWORD read_axis_all  (char caxis, double *pcounts, double *ecounts, int *cw,int *home,int *ccw);


extern DWORD read_dir      (char caxis, int *pbDir);
extern DWORD read_gate     (char caxis, DWORD *gate);
extern DWORD read_slew_mode(char caxis, DWORD *slew_mode);
extern DWORD read_speed    (char caxis, double *dpVelocity); 
extern DWORD read_couenc   (char caxis, long *counter, long *encoder);

extern DWORD set_counter   (char caxis, long lParameter);
extern DWORD set_encoder   (char caxis, long lParameter);

extern DWORD slew_stop(char caxis, long lpos);
extern DWORD go_const(char caxis, double speed);
extern DWORD motor_estop(char caxis);
extern DWORD motor_stop (char caxis);

extern DWORD get_motion_mode(long lAxisNo, DWORD *upStatus);

extern DWORD read_motfile(char *fname);
extern DWORD save_motfile(char *fname);

extern DWORD read_userinput_bit(long laxis, long ibit, DWORD *bit_value);
extern DWORD set_userout_bit(long laxis, long ibit, DWORD bit_value);

extern DWORD read_cpuinput_bit(long ibit, DWORD *bit_value);
extern DWORD write_cpuoutput_bit(long ibit, DWORD bit_value);
extern DWORD set_cpuouput_bitmode(long bit, DWORD mode);  
//-----------------------------------------------------------------------------------------------
extern int set_home_search_para(void);
extern DWORD goto_home(long axis, long dir);

extern DWORD set_preload_on (char caxis);
extern DWORD set_preload_off(char caxis);

extern DWORD preload_on (void);
extern DWORD preload_off(void);
//-----------------------------------------------------------------------------------------------
// handpaddle
extern int read_hp_status (hp_status *hp);

// focus adjustment
//extern int adjust_focus(int dir, int gst_mode);
//extern int init_focus(void);

