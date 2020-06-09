#define     ROW_MAX        50            //行数
#define     LineMin        53            //需要确定目前实际值为52 
#define     LineMax        185           //需要确定目前实际值为186
#define     Line_Center    125           //赛道中线,需要确定(LineMin+LineMax)/2原测值为124，单在实现上有点偏
#define     Steer_Mid      3415
#define     Steer_Left     3815
#define     Steer_Right    3015
#define     PLEFT		   108
#define     PRIGHT		   141
#define     PITTIME0       10000        //定时器初值
#define     SPEED_MAX      500
#define     SPEED_MIN      0
#define     PITMLD1temp    30 
#define     PITLD1temp     200
#define     First_row      24          //24       最小为9，为8则不行，前8行为消隐区
#define     Last_row       201		   //234      实际值为195
#define     Temp_Width	   100         //寻找起始线时用到，当遇到起跑线为起始线时
#define     max_speed      450
#define     min_speed      0
#define     jibian         (27+Pre_Line_Save)/27
uchar  cabs_uchar(char b);
void get_path_left();
void get_path_right();
void car_control_after_a_field();
void      duoji_control();
uint  cabs_uint(int b);
void  is_will_be_out();
void set_speed();
void  interrupt 66 testt(void) ;
#define KP 6                  //8
#define KI 6                 //7
#define KD 1
#define KKP (x_error*x_error/220+3)
#define KKD 0


