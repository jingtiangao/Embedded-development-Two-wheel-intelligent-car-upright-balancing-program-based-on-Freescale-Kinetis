#define     ROW_MAX        50            //����
#define     LineMin        53            //��Ҫȷ��Ŀǰʵ��ֵΪ52 
#define     LineMax        185           //��Ҫȷ��Ŀǰʵ��ֵΪ186
#define     Line_Center    125           //��������,��Ҫȷ��(LineMin+LineMax)/2ԭ��ֵΪ124������ʵ�����е�ƫ
#define     Steer_Mid      3415
#define     Steer_Left     3815
#define     Steer_Right    3015
#define     PLEFT		   108
#define     PRIGHT		   141
#define     PITTIME0       10000        //��ʱ����ֵ
#define     SPEED_MAX      500
#define     SPEED_MIN      0
#define     PITMLD1temp    30 
#define     PITLD1temp     200
#define     First_row      24          //24       ��СΪ9��Ϊ8���У�ǰ8��Ϊ������
#define     Last_row       201		   //234      ʵ��ֵΪ195
#define     Temp_Width	   100         //Ѱ����ʼ��ʱ�õ���������������Ϊ��ʼ��ʱ
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


