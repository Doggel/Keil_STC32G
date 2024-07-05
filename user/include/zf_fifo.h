/*********************************************************************************************************************
 * COPYRIGHT NOTICE
 * Copyright (c) 2020,��ɿƼ�
 * All rights reserved.
 * ��������QQȺ��һȺ��179029047(����)  ��Ⱥ��244861897(����)  ��Ⱥ��824575535
 *
 * �����������ݰ�Ȩ������ɿƼ����У�δ��������������ҵ��;��
 * ��ӭ��λʹ�ò������������޸�����ʱ���뱣����ɿƼ��İ�Ȩ������
 *
 * @file       		fifo
 * @company	   		�ɶ���ɿƼ����޹�˾
 * @author     		��ɿƼ�(QQ790875685)
 * @version    		�鿴doc��version�ļ� �汾˵��
 * @Software 		MDK FOR C251 V5.60
 * @Target core		STC32G12K128
 * @Taobao   		https://seekfree.taobao.com/
 * @date       		2020-4-14
 ********************************************************************************************************************/



#ifndef __ZF_FIFO_H
#define __ZF_FIFO_H


typedef struct
{
    unsigned char *buffer;                                                          // ����ָ��
    unsigned long    head;                                                             // ����ͷָ�� ����ָ��յĻ���
    unsigned long    end;                                                              // ����βָ�� ����ָ��ǿջ��棨����ȫ�ճ��⣩
    unsigned long    size;                                                             // ����ʣ���С
    unsigned long    max;                                                              // �����ܴ�С
}fifo_struct;

typedef enum
{
    FIFO_SUCCESS,

    FIFO_BUFFER_NULL,
    FIFO_SPACE_NO_ENOUGH,
    FIFO_DATA_NO_ENOUGH,
}fifo_state_enum;

typedef enum
{
    FIFO_READ_AND_CLEAN,
    FIFO_READ_ONLY,
}fifo_operation_enum;

fifo_state_enum fifo_init           (fifo_struct *fifo, unsigned char *buffer_addr, unsigned long size);
void            fifo_head_offset    (fifo_struct *fifo, unsigned long offset);
void            fifo_end_offset     (fifo_struct *fifo, unsigned long offset);
void            fifo_clear          (fifo_struct *fifo);
unsigned long          fifo_used           (fifo_struct *fifo);

fifo_state_enum fifo_read_buffer    (fifo_struct *fifo, unsigned char *dat, unsigned long *length, fifo_operation_enum flag);
fifo_state_enum fifo_write_buffer   (fifo_struct *fifo, unsigned char *dat, unsigned long length);

#endif


