#include "gyroscope.h"
#include "timer.h"

#define dt 0.01
#define FILTER_NUM 100	// ƽ���˲��㷨��������

int16 gyro_gyro_x,gyro_gyro_y,gyro_gyro_z;
int16 gyro_acc_x,gyro_acc_y,gyro_acc_z;

float fil_Gyro_z;				//���崦���Ľ��ٶ�ֵ
float Gyro_z=0;					//����Ĵ�����ٶȵ��м����
float Angle_Z=90;				//���ó�ʼ�Ƕ�Ϊ90��
float Gyroscope_FIFO[11]={0};
int gyro_i=0;
static float gyro[FILTER_NUM],sum_gyro;	// ƽ���˲��㷨�ĳ�ʼֵ

float fil_acc_y = 0;				//���崦���ļ��ٶ�ֵ
float Acc_y = 0;
float accy_FIFO[11]={0};
int accy_i=0;
static float accy_arr[FILTER_NUM],sum_acc_y;	// ƽ���˲��㷨�ĳ�ʼֵ
//-------------------------------------------------------------------------------------------------------------------
// �������     ICM20602 �Լ�
// ����˵��     void
// ���ز���     uint8           1-�Լ�ʧ�� 0-�Լ�ɹ�
// ʹ��ʾ��     icm20602_self_check();
// ��ע��Ϣ     �ڲ�����
//-------------------------------------------------------------------------------------------------------------------
static uint8 icm20602_self_check (void)
{
    uint8 dat = 0, return_state = 0;
    uint16 timeout_count = 0;

    while(0x12 != dat)                                                          // �ж� ID �Ƿ���ȷ
    {
        if(timeout_count ++ > ICM20602_TIMEOUT_COUNT)
        {
            return_state =  1;
            break;
        }
        dat = iic_read_reg(ICM20602_DEV_ADDR, ICM20602_WHO_AM_I);
        delay_ms(10);
    }
    return return_state;
}


//-------------------------------------------------------------------------------------------------------------------
// �������     ��ʼ�� ICM20602
// ����˵��     void
// ���ز���     uint8           1-��ʼ��ʧ�� 0-��ʼ���ɹ�
// ʹ��ʾ��     icm20602_init();
// ��ע��Ϣ
//-------------------------------------------------------------------------------------------------------------------
uint8 icm20602_init (void)
{
	
    uint8 val = 0x0, return_state = 0;
    uint16 timeout_count = 0;

    delay_ms(10);                                                        // �ϵ���ʱ
//#if ICM20602_USE_SOFT_IIC
//    soft_iic_init(&icm20602_iic_struct, ICM20602_DEV_ADDR, ICM20602_SOFT_IIC_DELAY, ICM20602_SCL_PIN, ICM20602_SDA_PIN);
//#else
//    spi_init(ICM20602_SPI, SPI_MODE0, ICM20602_SPI_SPEED, ICM20602_SPC_PIN, ICM20602_SDI_PIN, ICM20602_SDO_PIN, SPI_CS_NULL);
//    gpio_init(ICM20602_CS_PIN, GPO, GPIO_HIGH, GPO_PUSH_PULL);
//#endif

    do
    {
        if(icm20602_self_check())
        {
            // �������������˶�����Ϣ ������ʾ����λ��������
            // ��ô���� ICM20602 �Լ������ʱ�˳���
            // ���һ�½�����û������ ���û������ܾ��ǻ���
            
			while(1)
			{
				printf("icm20602 self check error");
				delay_ms(200);
			}
            return_state = 1;
            break;
        }

        iic_write_reg(ICM20602_DEV_ADDR, ICM20602_PWR_MGMT_1, 0x80);                     // ��λ�豸
        delay_ms(2);

        do
        {                                                                       // �ȴ���λ�ɹ�
            val = iic_read_reg(ICM20602_DEV_ADDR, ICM20602_PWR_MGMT_1);
            if(timeout_count ++ > ICM20602_TIMEOUT_COUNT)
            {
                // �������������˶�����Ϣ ������ʾ����λ��������
                // ��ô���� ICM20602 �Լ������ʱ�˳���
                // ���һ�½�����û������ ���û������ܾ��ǻ���
				while(1)
				{
					printf("icm20602 reset error.\r\n");
					delay_ms(200);
				}
                return_state = 1;
                break;
            }
        }while(0x41 != val);
        if(1 == return_state)
        {
            break;
        }
        iic_write_reg(ICM20602_DEV_ADDR, ICM20602_PWR_MGMT_1,0x01);                 // ʱ������
        iic_write_reg(ICM20602_DEV_ADDR, ICM20602_PWR_MGMT_2,0x00);                 // ���������Ǻͼ��ٶȼ�
        iic_write_reg(ICM20602_DEV_ADDR, ICM20602_CONFIG,0x01);                 // 176HZ 1KHZ
        iic_write_reg(ICM20602_DEV_ADDR, ICM20602_SMPLRT_DIV,0x07);                 // �������� SAMPLE_RATE = INTERNAL_SAMPLE_RATE / (1 + SMPLRT_DIV)
        
		iic_write_reg(ICM20602_DEV_ADDR, ICM20602_GYRO_CONFIG,ICM20602_GYR_SAMPLE);  // ��2000 dps
		// ICM20602_GYRO_CONFIG�Ĵ���
        // ����Ϊ:0x00 ����������Ϊ:��250 dps     ��ȡ�������������ݳ���131           ����ת��Ϊ������λ�����ݣ���λΪ����/s
        // ����Ϊ:0x08 ����������Ϊ:��500 dps     ��ȡ�������������ݳ���65.5          ����ת��Ϊ������λ�����ݣ���λΪ����/s
        // ����Ϊ:0x10 ����������Ϊ:��1000dps     ��ȡ�������������ݳ���32.8          ����ת��Ϊ������λ�����ݣ���λΪ����/s
        // ����Ϊ:0x18 ����������Ϊ:��2000dps     ��ȡ�������������ݳ���16.4          ����ת��Ϊ������λ�����ݣ���λΪ����/s
        
		iic_write_reg(ICM20602_DEV_ADDR, ICM20602_ACCEL_CONFIG,ICM20602_ACC_SAMPLE);  // ��8g
		// ICM20602_ACCEL_CONFIG�Ĵ���
        // ����Ϊ:0x00 ���ٶȼ�����Ϊ:��2g          ��ȡ���ļ��ٶȼ����� ����16384      ����ת��Ϊ������λ�����ݣ���λ��g(m/s^2)
        // ����Ϊ:0x08 ���ٶȼ�����Ϊ:��4g          ��ȡ���ļ��ٶȼ����� ����8192       ����ת��Ϊ������λ�����ݣ���λ��g(m/s^2)
        // ����Ϊ:0x10 ���ٶȼ�����Ϊ:��8g          ��ȡ���ļ��ٶȼ����� ����4096       ����ת��Ϊ������λ�����ݣ���λ��g(m/s^2)
        // ����Ϊ:0x18 ���ٶȼ�����Ϊ:��16g         ��ȡ���ļ��ٶȼ����� ����2048       ����ת��Ϊ������λ�����ݣ���λ��g(m/s^2)
       
		iic_write_reg(ICM20602_DEV_ADDR, ICM20602_ACCEL_CONFIG_2, 0x03);                 // Average 4 samples   44.8HZ   //0x23 Average 16 samples


    }while(0);

	init_gyrodata();	// ʹ�����Ǳ��־����ȶ����ȴ�Լ20ms����ȡ100��float���͵ĳ�ʼֵ������gyro[gyro_i]���顣������Ϊ ƽ���˲��㷨 �ĳ�ʼֵ
    init_accy_data();	// ��ȡ��ʼ��accyֵ
	printf("icm20602 init done.\r\n");
	return return_state;
}



//-------------------------------------------------------------------------------------------------------------------
//  @brief      ��ȡICM20602���ٶȼ�����
//  @param      NULL
//  @return     void
//  @since      v1.0
//  Sample usage:				ִ�иú�����ֱ�Ӳ鿴��Ӧ�ı�������
//-------------------------------------------------------------------------------------------------------------------
void Get_gyro_accdata(void)
{
    uint8 dat[6];
    iic_read_regs(ICM20602_DEV_ADDR, ICM20602_ACCEL_XOUT_H, dat, 6);  
    gyro_acc_x = (int16)(((uint16)dat[0]<<8 | dat[1]));
    gyro_acc_y = (int16)(((uint16)dat[2]<<8 | dat[3]));
    gyro_acc_z = (int16)(((uint16)dat[4]<<8 | dat[5]));
}
//-------------------------------------------------------------------------------------------------------------------
// �������     ��ʼ����������ʹ�����Ǳ��־����ȶ����ȴ�Լ20ms��ȡfilter_num��float���͵ĳ�ʼֵ������acc_y[accy_i]����
// ����˵��     
// ���ز���     
// ʹ��ʾ��     
// ��ע��Ϣ		
//-------------------------------------------------------------------------------------------------------------------
void init_accy_data()
{
	Get_gyro_accdata();		// ��ȡ���ٶ�����
	accy_arr[accy_i]=gyro_acc_y;
	fil_acc_y=0.0;
	for(accy_i=0;accy_i<FILTER_NUM;accy_i++)
	{
		sum_acc_y+=accy_arr[accy_i];
	}
}

//-------------------------------------------------------------------------------------------------------------------
// �������     ����ƽ���˲��㷨 ����y���ٶȡ�acc_y[]����ĳ�ֵ��init_accy_data()��ȡ
// ����˵��     
// ���ز���     
// ʹ��ʾ��     
// ��ע��Ϣ		
//-------------------------------------------------------------------------------------------------------------------
void accy_filter()
{
	float sum=0;
	static int acct_flag;

	Get_gyro_accdata();		// ��ȡ���ٶ�����
	
	Acc_y = (float)(gyro_acc_y-sum_acc_y/FILTER_NUM);
	if(abs(Acc_y)<10)	//���ٶ�С��10ʱ  Ĭ��ΪС����ֹ  
	{
		Acc_y=0;
	}
	for(acct_flag=1;acct_flag<10;acct_flag++)
	{	
		accy_FIFO[acct_flag-1]=accy_FIFO[acct_flag];//FIFO ����
	}
	accy_FIFO[9]=Acc_y;
	for(acct_flag=0;acct_flag<10;acct_flag++)
	{	            
		sum+=accy_FIFO[acct_flag];//��ǰ����ĺϣ���ȡƽ��ֵ
	}
	fil_acc_y=sum/10;
}


//-------------------------------------------------------------------------------------------------------------------
//  @brief      ��ȡICM20602����������
//  @param      NULL
//  @return     void
//  @since      v1.0
//  Sample usage:				ִ�иú�����ֱ�Ӳ鿴��Ӧ�ı�������
//-------------------------------------------------------------------------------------------------------------------
void Get_gyro_gyro(void)
{
    uint8 dat[6];
    iic_read_regs(ICM20602_DEV_ADDR, ICM20602_GYRO_XOUT_H, dat, 6);  
    gyro_gyro_x = (int16)(((uint16)dat[0]<<8 | dat[1]));
    gyro_gyro_y = (int16)(((uint16)dat[2]<<8 | dat[3]));
    gyro_gyro_z = (int16)(((uint16)dat[4]<<8 | dat[5]));
}

/**************************************************************************
�������ܣ�	��ʼ����������ʹ�����Ǳ��־����ȶ����ȴ�Լ20ms��ȡ100��float���͵ĳ�ʼֵ������gyro[gyro_i]���顣
			������Ϊ ƽ���˲��㷨 �ĳ�ʼֵ
��ڲ�������
����  ֵ����
**************************************************************************/
void init_gyrodata()
{
 
	gyro[gyro_i]=gyro_gyro_z;
	fil_Gyro_z=0.0;
	for(gyro_i=0;gyro_i<FILTER_NUM;gyro_i++)
	{
		Get_gyro_gyro();		// ��ȡ���ٶ�����
		sum_gyro+=gyro[gyro_i];
	}
}

/**************************************************************************
�������ܣ�����ƽ���˲��㷨 ������ٶȡ�gyro[]����ĳ�ֵ��init_gyrodata()��ȡ
��ڲ�������
����  ֵ����
**************************************************************************/
// �˲����������������
void Gyroscope_newValues()
{
	float sum=0;
	static int Gyro_flag;

	Get_gyro_gyro();		// ��ȡ���ٶ�����

	Gyro_z = (float)(gyro_gyro_z-sum_gyro/100)/16.3835;
	if(abs(Gyro_z)<3)//���ٶ�С��3ʱ  Ĭ��ΪС����ֹ  
	{
		Gyro_z=0;
	}
	for(Gyro_flag=1;Gyro_flag<10;Gyro_flag++)
	{	
		Gyroscope_FIFO[Gyro_flag-1]=Gyroscope_FIFO[Gyro_flag];//FIFO ����
	}
	Gyroscope_FIFO[9]=Gyro_z;
	for(Gyro_flag=0;Gyro_flag<10;Gyro_flag++)
	{	            
		sum+=Gyroscope_FIFO[Gyro_flag];//��ǰ����ĺϣ���ȡƽ��ֵ
	}
	fil_Gyro_z=sum/10;
}

/**************************************************************************
�������ܣ��Խ��ٶȻ��� �õ��Ƕ�
��ڲ�������
����  ֵ����
**************************************************************************/
void Get_angle()
{
	Gyroscope_newValues();			//ʹ�������ǻ�ȡ���ٶ�
	Angle_Z-=fil_Gyro_z*dt;
	if(Angle_Z>=360) Angle_Z=Angle_Z-360;
	if(Angle_Z<=-360) Angle_Z=Angle_Z+360;
}
