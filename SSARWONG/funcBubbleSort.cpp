/*******************************************************
**	�������ã�ð�������㷨
**	float ** distMat		    => patch �������
**	myPara imgParameter		=> ͼ�������
**	float ** observerImg		=>������������
**	ע�ͣ�
********************************************************/

# include "stdafx.h"

void bubbleSort(float * waitingOrder_0, int * waitingOrder_1, myPara imgParameter)
{
	for (int i = 0; i<imgParameter.distMatRow - 1; i++)
	{
		for (int j = 0; j < imgParameter.distMatRow - 2; j++)
		{
			if (waitingOrder_0[j] > waitingOrder_0[j + 1])
			{
				float temp_0 = waitingOrder_0[j]; waitingOrder_0[j] = waitingOrder_0[j + 1]; waitingOrder_0[j + 1] = temp_0;
				int temp_1 = waitingOrder_1[j]; waitingOrder_1[j] = waitingOrder_1[j + 1]; waitingOrder_1[j + 1] = temp_1;
			}
		}
	}
}