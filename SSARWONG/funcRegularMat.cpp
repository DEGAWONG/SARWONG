/*******************************************************
**	函数作用：图像矩阵正则化函数
**	float ** oriImg	    	=> 正则化前图像
**	myPara  imgParameter		=>图像本身参数
**	float ** regularImg		=> 正则化后图像
**	注释：
********************************************************/
# include "stdafx.h"

void regularMat(float** oriImg,float** regularImg,myPara imgParameter)
{
	for ( int i = 0;i < imgParameter.row;i++ )
	{
		for ( int j = 0;j < imgParameter.col;j++ )
		{
			regularImg[i][j] = oriImg[i][j] / 255.0;
		}
	}
}
