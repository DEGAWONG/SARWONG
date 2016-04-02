/*******************************************************
**	函数作用：低秩近似操作
**	float ** averageObserverImg	    => 得到的观测矩阵
**	myPara imgParameter		    => 图像本身参数
**	float ** postDistMat		=> 转置后的距离矩阵
**	float ** distMat			=> 待计算分配多大的空间规格：  (searchWinSizeRow - f + 1)*(searchWinSizeCol - f + 1) X (N - f )/step *(M - f )/step
**	注释： 参数需要改变   距离不要在一个函数中计算了		         行数					列数
********************************************************/ 
//# include "structMyPara.h"
//# include "funcLowRankDenoising.h"

# include "stdafx.h"
void lowRankDenoising(myPara imgParameter , float** averageObserverImg)
{
	float iteratError = 100;
	// 将初始的迭代误差设置为 100 视为极大
	for (int i = 0;(i < imgParameter.lowRankLoopNum) && (iteratError < imgParameter.lowRankLoopError); i++)
	{
		
	}
}