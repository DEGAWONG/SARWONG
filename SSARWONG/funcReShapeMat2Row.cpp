/*******************************************************
**	函数作用：SVD 矩阵展开成 一行
**	float ** imgMat				 => 输入
**	myPara  imgParameter		 =>图像本身参数
**	float * imgArray			 => 输出
**	注释：
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