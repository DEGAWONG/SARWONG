/*******************************************************
**	�������ã�SVD ����չ���� һ��
**	float ** imgMat				 => ����
**	myPara  imgParameter		 =>ͼ�������
**	float * imgArray			 => ���
**	ע�ͣ�
********************************************************/

#include "stdafx.h"

void reShapeMat2Row(float ** imgMat,float * imgArray, myPara  imgParameter)
{
	for (int i = 0; i < imgParameter.SVDMatRow; i++)
	{
		for (int j = 0; j < imgParameter.SVDMatCol; j++)
		{
			imgArray[i*imgParameter.SVDMatCol + j] = imgMat[i][j];
		}
	}
}