extern int open_hip_cat(void);
extern int close_hip_cat(void);
extern int scan_hip(void);
extern int read_hip_onestar(int hdno_inp, char sbuff[], int format);

extern int open_oes(void);
extern int read_oesb8(FILE *inf);
extern int get_hip_no(int hd_no);
extern int get_hd_no(int hip_no);
extern int hr_to_hipno(int hr_no);
extern double cal_mass(double vmag,double par_mas);
extern int read_ep(int star_no);


typedef struct H_REC
        {int hpno,hdno;
         double ra; double dec; double par;double epar;
         double ux; double uy;
         double vm; double bv;
         double bmv;
         char q;    char sp[14];
        } hip_rec_type;
        
extern hip_rec_type HIP_DATA;
