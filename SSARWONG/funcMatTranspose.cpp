/*******************************************************
**	函数作用：图像矩阵转置函数 专用来对距离矩阵进行转置
**	float ** preTranspose	    => 转置前图像
**	myPara  imgParameter			=>图像本身参数
**	float ** postTranspose		=> 转置后图像
**	注释：
********************************************************/

# include "stdafx.h"

void matTranspose(float** preTranspose,float** postTranspose,myPara imgParameter)
{
	for (int i = 0;i < imgParameter.distMatRow;i++)
	{
		for (int j = 0;j < imgParameter.distMatCol;j++)
		{
			postTranspose[j][i] = preTranspose[i][j];
		}
	}
}