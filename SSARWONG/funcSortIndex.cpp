/*******************************************************
**	函数作用：对单列的向量进行排序，保留排序前的序号索引
**	float ** distMat		    => patch 距离矩阵
**	myPara imgParameter		=> 图像本身参数
**	float ** observerImg		=>用来索引回填
**	float ** sortIndex		=> 包含排序后产生的原始索引
**	float ** orderMat	    => 待排序的辅助列数组 与 disMat 规格相同 
**	注释：
********************************************************/

# include "stdafx.h"

void sortIndex(float** distTransMat, int** orderMat, myPara imgParameter)
{
	// 遍历 distTransMat 矩阵的每一行，其中每一行的所有列列进行大小排序
	for (int i = 0; i < imgParameter.distTransMatRow; i++)
	{
		bubbleSort(distTransMat[i], orderMat[i],imgParameter);
	}
}