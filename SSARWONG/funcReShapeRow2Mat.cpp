/*******************************************************
**	�������ã�SVD һ��չ���ɾ��� 
**	float ** imgMat				 => ���
**	myPara  imgParameter		 =>ͼ�������
**	float * imgArray			 => ����
**	ע�ͣ�
********************************************************/

#include "stdafx.h"


void reShapeRow2Mat(float * imgArray, float ** imgMat, int row, int col)
{
	for (int i = 0; i < row; i++)
	{
		for (int j = 0; j < col; j++)
		{
			 imgMat[i][j] =	imgArray[i*col + j];
		}
	}
}