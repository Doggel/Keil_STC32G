#include "telemeter.h"

//#pragma userclass (near=CEVENT)	
//	CEVENT_EXPORT(0,dl1b_init,NULL);
//#pragma userclass (near=default)
/*********************************************************************************************************************
* �ļ�����          zf_device_dl1b
* ��˾����          �ɶ���ɿƼ����޹�˾
* �汾��Ϣ          �鿴 libraries/doc �ļ����� version �ļ� �汾˵��
* ��������          MDK 5.37
* ����ƽ̨          MM32F527X_E9P
* ��������          https://seekfree.taobao.com/
* 
* �޸ļ�¼
* ����              ����                ��ע
* 2022-08-10        Teternal            first version
********************************************************************************************************************/
/*********************************************************************************************************************
* ���߶��壺
*                   ------------------------------------
*                   ģ��ܽ�            ��Ƭ���ܽ�
*                   SCL                 �鿴 zf_device_dl1b.h �� DL1B_SCL_PIN  �궨��
*                   SDA                 �鿴 zf_device_dl1b.h �� DL1B_SDA_PIN  �궨��
*                   XS                  �鿴 zf_device_dl1b.h �� DL1B_XS_PIN  �궨��
*                   VCC                 5V ��Դ
*                   GND                 ��Դ��
*                   ------------------------------------
********************************************************************************************************************/


uint8 dl1b_init_flag = 0;
uint8 dl1b_finsh_flag = 0;			//������־���ģ���Ƿ�ɹ���þ�������

#define dl1b_transfer_8bit_array(tdata, tlen, rdata, rlen)      (dl1b_iic_transfer_8bit_array((tdata), (tlen), (rdata), (rlen)))


//-------------------------------------------------------------------------------------------------------------------
// �������     ��� IIC �ӿڴ��� 8bit ���� ��д���ȡ
// ����˵��     *write_data     �������ݴ�Ż�����
// ����˵��     write_len       ���ͻ���������
// ����˵��     *read_data      ��ȡ���ݴ�Ż�����
// ����˵��     read_len        ��ȡ����������
// ���ز���     void            
// ʹ��ʾ��     iic_transfer_8bit_array(IIC_1, addr, data, 64, data, 64);
// ��ע��Ϣ     
//-------------------------------------------------------------------------------------------------------------------
void dl1b_iic_transfer_8bit_array (const uint8 *write_data, uint8 write_len, uint8 *read_data, uint8 read_len)
{
	iic_transfer(DL1B_DEV_ADDR,write_data,write_len,read_data,read_len);
}




//-------------------------------------------------------------------------------------------------------------------
// �������     �����Ժ���Ϊ��λ�ķ�Χ����
// ����˵��     void
// ���ز���     void
// ʹ��ʾ��     dl1b_get_distance();
// ��ע��Ϣ     �ڿ�ʼ������̲�����Ҳ���ô˺���
//-------------------------------------------------------------------------------------------------------------------
uint16 dl1b_get_distance (void)
{     
	uint8 Data_Buffer[3];
	int16 dl1b_distance_temp = 0;
	uint16 dl1b_distance_mm = 3333;
    if(dl1b_init_flag)
    {
        Data_Buffer[0] = DL1B_GPIO__TIO_HV_STATUS >> 8;
        Data_Buffer[1] = DL1B_GPIO__TIO_HV_STATUS & 0xFF;
        dl1b_transfer_8bit_array(Data_Buffer, 2, &Data_Buffer[2], 1);

        if(Data_Buffer[2])
        {

            Data_Buffer[0] = DL1B_SYSTEM__INTERRUPT_CLEAR >> 8;
            Data_Buffer[1] = DL1B_SYSTEM__INTERRUPT_CLEAR & 0xFF;
            Data_Buffer[2] = 0x01;
            dl1b_transfer_8bit_array(Data_Buffer, 3, Data_Buffer, 0);// clear Interrupt

            Data_Buffer[0] = DL1B_RESULT__RANGE_STATUS >> 8;
            Data_Buffer[1] = DL1B_RESULT__RANGE_STATUS & 0xFF;
            dl1b_transfer_8bit_array(Data_Buffer, 2, &Data_Buffer[2], 1);
            
            if(0x89 == Data_Buffer[2])
            {
                Data_Buffer[0] = DL1B_RESULT__FINAL_CROSSTALK_CORRECTED_RANGE_MM_SD0 >> 8;
                Data_Buffer[1] = DL1B_RESULT__FINAL_CROSSTALK_CORRECTED_RANGE_MM_SD0 & 0xFF;
                dl1b_transfer_8bit_array(Data_Buffer, 2, Data_Buffer, 2);
                dl1b_distance_temp = Data_Buffer[0];
                dl1b_distance_temp = (dl1b_distance_temp << 8) | Data_Buffer[1];
                
                if(dl1b_distance_temp > 4000 || dl1b_distance_temp < 0)
                {
                    dl1b_distance_mm = 4444;
                    dl1b_finsh_flag = 0;
                }
                else
                {
                    dl1b_distance_mm = dl1b_distance_temp;
                    dl1b_finsh_flag = 1;
                }
            }
            else
            {
                dl1b_distance_mm = 5555;
                dl1b_finsh_flag = 0;
            }
        }
        else
        {
            dl1b_distance_mm = 6666;
            dl1b_finsh_flag = 0;
			dl1b_init();
        }
    }
	return dl1b_distance_mm;
}



//-------------------------------------------------------------------------------------------------------------------
// �������     ��ʼ�� DL1B
// ����˵��     void
// ���ز���     uint8           1-��ʼ��ʧ�� 0-��ʼ���ɹ�
// ʹ��ʾ��     dl1b_init();
// ��ע��Ϣ
//-------------------------------------------------------------------------------------------------------------------
uint8 dl1b_init (void)
{
    uint8   return_state    = 0;
    uint8   Data_Buffer[2 + sizeof(dl1b_default_configuration)]; 
    uint16  time_out_count  = 0;
	do
    {
        delay_ms(50);
        DL1B_XS_PIN = 0;
        delay_ms(10);
        DL1B_XS_PIN = 1;
        delay_ms(50);

        Data_Buffer[0] = DL1B_FIRMWARE__SYSTEM_STATUS >> 8;
        Data_Buffer[1] = DL1B_FIRMWARE__SYSTEM_STATUS & 0xFF;
        dl1b_transfer_8bit_array(Data_Buffer, 2, &Data_Buffer[2], 1);
        return_state = (0x01 == (Data_Buffer[2] & 0x01)) ? (0) : (1);
        if(1 == return_state)
        {
            break;
        }

        Data_Buffer[0] = DL1B_I2C_SLAVE__DEVICE_ADDRESS >> 8;
        Data_Buffer[1] = DL1B_I2C_SLAVE__DEVICE_ADDRESS & 0xFF;
        memcpy(&Data_Buffer[2], (uint8 *)dl1b_default_configuration, sizeof(dl1b_default_configuration));
        dl1b_transfer_8bit_array(Data_Buffer, 2 + sizeof(dl1b_default_configuration), Data_Buffer, 0);

        while(1)
        {
            Data_Buffer[0] = DL1B_GPIO__TIO_HV_STATUS >> 8;
            Data_Buffer[1] = DL1B_GPIO__TIO_HV_STATUS & 0xFF;
            dl1b_transfer_8bit_array(Data_Buffer, 2, &Data_Buffer[2], 1);
            if(0x00 == (Data_Buffer[2] & 0x01))
            {
                time_out_count = 0;
                break;
            }
            if(DL1B_TIMEOUT_COUNT < time_out_count ++)
            {
                return_state = 1;
                break;
            }
            delay_ms(1);
        }

        dl1b_init_flag = 1;
    }while(0);
	
    return return_state;
}
