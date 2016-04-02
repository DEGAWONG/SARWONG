/*******************************************************
**	函数作用：图像矩阵展开成 64 * (M - f + 1) *(N - f + 1)全参考矩阵
**	float ** referMat			 => 得到的所有行列的方形成列的全参考矩阵
**	myPara  imgParameter		 =>图像本身参数
**	float ** regularImg			 => 正则化后图像
**	注释：
********************************************************/

#include "stdafx.h"

void reShapeMat2Col(float** regularImg, float** referMat, myPara imgParameter)
{
	for (int i = 0; i < imgParameter.col - imgParameter.winCol + 1; i++)
	{
		for (int j = 0; j < imgParameter.row - imgParameter.winRow + 1; j++)
		{
			for (int ii = 0; ii < imgParameter.winRow; ii++)
			{
				for (int jj = 0; jj < imgParameter.winCol; jj++)
				{
					referMat[ii * imgParameter.winCol + jj][i * (imgParameter.row - imgParameter.winRow + 1) + j] = regularImg[j + ii][i + jj];
				}
			}
		}
	}
}