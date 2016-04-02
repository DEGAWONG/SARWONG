/*******************************************************
**	函数作用：图像矩阵展开成 64 * (M - f + 1) *(N - f + 1)全参考矩阵
**	float ** referMat			 => 得到的所有行列的方形成列的全参考矩阵
**	myPara  imgParameter		 =>图像本身参数
**	float ** regularImg			 => 正则化后图像
**	注释：
********************************************************/

#include "stdafx.h"

void setMyPara(myPara &imgParameter)
{
	// 预处理参数设置
	imgParameter.skipStep = 6;
	imgParameter.winRow = 8;
	imgParameter.winCol = 8;
	imgParameter.searchWinRow = 44;
	imgParameter.searchWinCol = 44;
	imgParameter.simiMatNum = 80;

	imgParameter.distMatRow = (imgParameter.searchWinRow * imgParameter.searchWinCol);
	imgParameter.distMatCol = ((imgParameter.row - imgParameter.winRow) / imgParameter.skipStep + 1)*((imgParameter.col - imgParameter.winCol) / imgParameter.skipStep + 1);

	imgParameter.distTransMatRow = imgParameter.distMatCol;
	imgParameter.distTransMatCol = imgParameter.distMatRow;

	imgParameter.referMatRow = imgParameter.winRow * imgParameter.winCol;
	imgParameter.referMatCol = (imgParameter.row - imgParameter.winRow + 1)*(imgParameter.col - imgParameter.winCol + 1);

	imgParameter.skipReferMatPerImgRow = ((imgParameter.col - imgParameter.winCol) / imgParameter.skipStep + 1);

	// 低秩近似参数设置
	imgParameter.lambda_1 = 0.01;
	imgParameter.lambda_2 = 0.8;
	imgParameter.lambda_3 = 3.0;
	imgParameter.rou = 0.05;
	imgParameter.miu = 1.0;
	imgParameter.alpha = 1.05;

	imgParameter.SVDMatRow = imgParameter.winRow * imgParameter.winCol;
	imgParameter.SVDMatCol = imgParameter.simiMatNum;

	imgParameter.tau_1 = imgParameter.rou / (imgParameter.lambda_2 + imgParameter.lambda_3);
	imgParameter.tau_2 = imgParameter.lambda_1 / imgParameter.lambda_3;
	imgParameter.factor_0 = imgParameter.lambda_3 / (imgParameter.lambda_2 + imgParameter.lambda_3);

	imgParameter.SVDMatRank = 0;
	imgParameter.SVDMINRowCol = MIN(imgParameter.SVDMatRow, imgParameter.SVDMatCol);
	imgParameter.errorOffset = 1e-6;

	imgParameter.SVDEps = 0.000001;
	imgParameter.SVDKa = MAX(imgParameter.SVDMatRow, imgParameter.SVDMatCol) + 1;

	imgParameter.ADMLoopStopFlag = 0;
	imgParameter.ADMMaxLoopNum = 100;
	imgParameter.ADMMaxLoopError = 1e-1;
	imgParameter.ADMMaxTempError = 100.0;

	imgParameter.lowRankLoopNum = 20;
	imgParameter.lowRankLoopError = 1e-4;
	imgParameter.lowRankTempError = 100.0;
}