/*******************************************************
**	函数作用：SVD 一行展开成矩阵 
**	float ** imgMat				 => 输出
**	myPara  imgParameter		 =>图像本身参数
**	float * imgArray			 => 输入
**	注释：
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