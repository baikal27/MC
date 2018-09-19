extern int turn_on_timer(int handle, int control);
extern int turn_off_timer(int handle, int control);
extern int set_timer_interval(int handle, int control, double interval);
extern int set_panel_title(int handle, char title[]);
extern int set_control_visible(int panel_handle, int control);
extern int set_control_hidden (int panel_handle, int control);
extern int set_control_dim(int handle, int control);
extern int set_control_undim(int handle, int control);

extern void close_stdio(void);
extern void open_stdio(void);

extern int copy_panel_to_clipboard(int panel);
extern int copy_active_control_to_clipboard(int panel);

extern int intsort(int n, int arrin[], int indx[]);

extern int auto_plot_xy(int plot_handle, int control, double x[], double y[], int ndata, int type);
extern int auto_plot_xy_scatter(int plot_handle, int control, double x[], double y[], int ndata);
extern int auto_plot_xy_line(int plot_handle, int control, double x[], double y[], int ndata);

extern int set_on_color(int panel,int control);
extern int file_select(char file_ext[], char file_name[]);
extern int dir_select(char default_dir[], char dirname[]);
extern int get_fbody(char fname[], char fbody[]);
extern int get_dir  (char fname[], char dir[]);
extern int del_dir(char dirfname[], char fname[]);

extern int sort_double(int ndata, double yinp[], int indx[]);
extern int cubic_spline_interpolate(int n1, double y1[], int df, double y2[]);
extern int remove_outlier(double x[],int ndata, double range, double *me, double *si, int *nn);
