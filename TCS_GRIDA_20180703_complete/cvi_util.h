#ifndef _CVI_UTIL_H
#define _CVI_UTIL_H
int turn_on_timer(int handle, int control);
int turn_off_timer(int handle, int control);
int set_timer_interval(int handle, int control, double interval);
int set_panel_title(int handle, char title[]);
int set_control_visible(int panel_handle, int control);
int set_control_hidden (int panel_handle, int control);
int set_control_dim(int handle, int control);
int set_control_undim(int handle, int control);

void close_stdio(void);
void open_stdio(void);

int copy_panel_to_clipboard(int panel);
int copy_active_control_to_clipboard(int panel);

int intsort(int n, int arrin[], int indx[]);

int auto_plot_xy(int plot_handle, int control, double x[], double y[], int ndata, int type);
int auto_plot_xy_scatter(int plot_handle, int control, double x[], double y[], int ndata);
int auto_plot_xy_line(int plot_handle, int control, double x[], double y[], int ndata);

int set_on_color(int panel,int control);
int file_select(char file_ext[], char file_name[]);
int dir_select(char default_dir[], char dirname[]);
int get_fbody(char fname[], char fbody[]);
int get_dir  (char fname[], char dir[]);
int del_dir(char dirfname[], char fname[]);

int sort_double(int ndata, double yinp[], int indx[]);
int cubic_spline_interpolate(int n1, double y1[], int df, double y2[]);
int remove_outlier(double x[],int ndata, double range, double *me, double *si, int *nn);
#endif
