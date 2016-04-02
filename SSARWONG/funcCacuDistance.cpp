/*******************************************************
**	函数作用：计算 80 * （imgParameter.searchWinRow）*（imgParameter.searchWinRow）
**	float ** referMat	    => 得到的观测矩阵
**	myPara  imgParameter		=> 图像本身参数
**	float ** postDistMat		=> 转置后的距离矩阵
**	float ** distMat			=> 待计算分配多大的空间规格：  (searchWinRow - winRow + 1)*(searchWinCol - winCol + 1) * ((row - winRow )/skipStep + 1)*((col - winCol )/skipStep + 1)
**	float ** distMat			=> 
**	注释： 参数需要改变   距离不要在一个函数中计算了									 行数													      列数
********************************************************/

# include "stdafx.h"

void cacuDistance(float** referMat, float** distMat, myPara imgParameter)
{
	// 遍历图像中所有的参考窗口
	// 列索引
	for (int i = 0; i * imgParameter.skipStep < (imgParameter.col - imgParameter.winCol + 1); i++)
	{
		// 行索引
		for (int j = 0; j * imgParameter.skipStep < (imgParameter.row - imgParameter.winRow + 1); j++)
		{
			// 此循环是为了遍历搜索框内的所有待对比列　　相等的时候不需要计算
			// 列索引
			for (int ii = 0; ii < imgParameter.searchWinCol; ii++)
			{
				// 行索引
				for (int jj = 0; jj < imgParameter.searchWinRow; jj++)
				{	
					// 当前的参考列
					int referMatCol = i * imgParameter.skipStep * (imgParameter.row - imgParameter.winRow + 1) + j * imgParameter.skipStep;				
					// 当前的参考列数的对比列
					int compareMatCol = (i * imgParameter.skipStep - 0.5 * imgParameter.searchWinCol + ii) * (imgParameter.row - imgParameter.winRow + 1) + (j * imgParameter.skipStep - 0.5 * imgParameter.searchWinRow + jj);
					if(
						((i * imgParameter.skipStep - 0.5 * imgParameter.searchWinRow + ii) < 0) ||
						((i * imgParameter.skipStep - 0.5 * imgParameter.searchWinRow + ii) > =(imgParameter.row - imgParameter.winRow + 1)) ||
						((j * imgParameter.skipStep - 0.5 * imgParameter.searchWinCol + jj) < 0) ||
						((j * imgParameter.skipStep - 0.5 * imgParameter.searchWinCol + jj) > =(imgParameter.col - imgParameter.winCol + 1))
					  )
					{
						distMat[(ii * imgParameter.searchWinRow + jj)][i*((imgParameter.row - imgParameter.winRow) / imgParameter.skipStep + 1) + j] = 20.0;
					}
					else 
					{
						for (int iii = 0; iii < imgParameter.winRow * imgParameter.winCol; iii++)
						{
							float tempDistValue = 0.0;
							//							  本列的行数遍历				当前的参考列数
							tempDistValue = referMat[iii][referMatCol] - referMat[iii][compareMatCol]; 
							tempDistValue = (tempDistValue * tempDistValue) / (imgParameter.winRow * imgParameter.winCol);
							distMat[(ii * imgParameter.searchWinRow + jj)][i*((imgParameter.col - imgParameter.winCol) / imgParameter.skipStep + 1) + j] += tempDistValue;
						}
					}
				}
			}
		}
	}
}