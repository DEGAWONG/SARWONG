/*******************************************************
**	函数作用：
**	myPara imgParameter		=> 图像本身参数
**	float ** 
**	float ** 
**	注释：
********************************************************/

# include "stdafx.h"

void ADM_MixedDatafitLowRank(float** SVDMat, float& weightArray, float **L_hat,float **Z_hat,float **Y,float **L_prev,float **SVD_U,float *SVD_UArray,float **SVD_S,float **SVD_SRank,float  *SVD_SArray,float **SVD_V,float *SVD_VArray,float* SVDArray,myPara imgParameter)
{
	// 保留初始的 SVDMat 以便初始化给 L_prev
	for (int i = 0; i < imgParameter.SVDMatRow; i++)
	{
		for (int j = 0; j < imgParameter.SVDMatCol; j++)
		{
			L_hat[i][j] = SVDMat[i][j];
		}
	}
	// 将停止标志置零
	imgParameter.ADMLoopStopFlag = 0;
	imgParameter.ADMMaxTempNum = 0;
	// 迭代条件需要满足次数与误差
	while (imgParameter.ADMLoopStopFlag == 0)
	{
		// 计算当前循环次数
		imgParameter.ADMMaxTempNum++;
		// 保留 L_hat
		for (int i = 0; i < imgParameter.SVDMatRow; i++)
		{
			for (int j = 0; j < imgParameter.SVDMatCol; j++)
			{
				L_prev[i][j] = L_hat[i][j];
			}		
		}
		// 更新 L_hat SVD 为做SVD分解的矩阵
		float **SVD = new float *[imgParameter.SVDMatRow];
		for (int i = 0; i < imgParameter.SVDMatRow; i++)
			SVD[i] = new float[imgParameter.SVDMatCol];
		for (int i = 0; i < imgParameter.SVDMatRow; i++)
		{
			for (int j = 0; j < imgParameter.SVDMatCol; j++)
			{
				SVD[i][j] = 0;
			}
		}

		for (int i = 0; i < imgParameter.SVDMatRow; i++)
		{
			for (int j = 0; j < imgParameter.SVDMatCol; j++)
			{
				SVD[i][j] = SVDMat[i][j] - imgParameter.factor_0 * (Z_hat[i][j] - Y[i][j] / imgParameter.lambda_3);
			}
		}
		reShapeMat2Row(SVD, SVDArray, imgParameter);

		int SVD_FLAG = cacuSVD(SVDArray, imgParameter.SVDMatRow, imgParameter.SVDMatCol, SVD_UArray, SVD_VArray, imgParameter.SVDEps,imgParameter.SVDKa);

		reShapeRow2Mat(SVDArray, SVD_S, imgParameter.SVDMatRow, imgParameter.SVDMatCol);
		reShapeRow2Mat(SVD_UArray, SVD_U, imgParameter.SVDMatRow, imgParameter.SVDMatRow);
		reShapeRow2Mat(SVD_VArray, SVD_V, imgParameter.SVDMatCol, imgParameter.SVDMatCol);

		// 找到 SVD 分解中对角矩阵的最小数值同时将对角矩阵的元素变为减掉最小值之后的元素值方便后面计算
		// 首先假设 imgParameter.minUDiag 为 100 便于赋值
		imgParameter.minUDiag = 100.0;
		for (int i = 0; i < imgParameter.SVDMatRow ; i++)
		{
			if (imgParameter.minUDiag > SVD_S[i][i])
				imgParameter.minUDiag = SVD_S[i][i];
		}
		for (int i = 0; i < imgParameter.SVDMatRow; i++)
		{
			for (int j = 0; j < imgParameter.SVDMatCol; j++)
			{
				 SVD_SRank[i][j]= SVD_S[i][j];
			}
		}
		for (int i = 0; i < imgParameter.SVDMatRow; i++)
		{
			//此处经验证，公式求解正确
			SVD_S[i][i] = abs(SVD_S[i][i]) / SVD_S[i][i] * (MAX(abs(SVD_S[i][i]) - imgParameter.tau_1 / (abs(SVD_S[i][i]) - imgParameter.minUDiag + imgParameter.errorOffset), 0));
		}
		// 下方计算的中间值
		float **SVDMidMat = new float *[imgParameter.SVDMatRow];
		for (int i = 0; i < imgParameter.SVDMatRow; i++)
			SVDMidMat[i] = new float[imgParameter.SVDMatCol];
		for (int i = 0; i < imgParameter.SVDMatRow; i++)
		{
			for (int j = 0; j < imgParameter.SVDMatCol; j++)
			{
				SVDMidMat[i][j] = 0;
				for (int k = 0; k < imgParameter.SVDMatRow; k++)
				{
					SVDMidMat[i][j] += SVD_U[i][k] * SVD_S[k][j];
				}
			}
		}
		// 更新 L_hat
		// 因为是 USV'，但此时的 V 已经转置 故在 L_hat[i][j] += SVDMidMat[i][k] * SVD_V[k][j]
		for (int i = 0; i < imgParameter.SVDMatRow; i++)
		{
			for (int j = 0; j < imgParameter.SVDMatCol; j++)
			{
				L_hat[i][j] = 0;
				for (int k = 0; k < imgParameter.SVDMatCol; k++)
				{
					L_hat[i][j] += SVDMidMat[i][k] * SVD_V[k][j];
				}
			}
		}

		// 更新 Z_hat SVDMidMat 为 imgParameter.SVDMatRow * imgParameter.SVDMatCol 的矩阵，可用来存储中间数值
		for (int i = 0; i < imgParameter.SVDMatRow; i++)
		{
			for (int j = 0; j < imgParameter.SVDMatCol; j++)
			{
				SVDMidMat[i][j] = SVDMat[i][j] - L_hat[i][j] - Y[i][j] / imgParameter.lambda_3;
				// 做排除 0 做分母的补偿
				Z_hat[i][j] = abs(SVDMidMat[i][j]) / SVDMidMat[i][j] * (MAX(0, abs(SVDMidMat[i][j]) - imgParameter.tau_2));
			}	
		}
		// 更新 Y
		for (int i = 0; i < imgParameter.SVDMatRow; i++)
		{
			for (int j = 0; j < imgParameter.SVDMatCol; j++)
			{
				Y[i][j] = Y[i][j] + imgParameter.miu * (Z_hat[i][j] - SVDMat[i][j] + L_hat[i][j]);
			}
		}
		imgParameter.miu = imgParameter.alpha * imgParameter.miu;

		// 计算每次循环的误差，判断是否达到误差要求
		imgParameter.ADMMaxTempError = 0.0;
		imgParameter.ADMMaxTempError_0 = 0.0;
		imgParameter.ADMMaxTempError_1 = 0.0;
		for (int i = 0; i < imgParameter.SVDMatRow; i++)
		{
			for (int j = 0; j < imgParameter.SVDMatCol; j++)
			{
				imgParameter.ADMMaxTempError_0 += ((L_hat[i][j] - L_prev[i][j])* (L_hat[i][j] - L_prev[i][j]));
				imgParameter.ADMMaxTempError_1 += (L_hat[i][j] * L_hat[i][j]);
			}
		}
		imgParameter.ADMMaxTempError = imgParameter.ADMMaxTempError_0 / imgParameter.ADMMaxTempError_1;
		
		//清除内存空间
			for (int i = 0; i < imgParameter.SVDMatRow; i++)
			{
			delete[] SVDMidMat[i];
			}
		delete[]   SVDMidMat;
		for (int i = 0; i < imgParameter.SVDMatRow; i++)
		{
			delete[] SVD[i];
		}
		delete[]   SVD;
		if ((imgParameter.ADMMaxTempNum >= imgParameter.ADMMaxLoopNum) || (imgParameter.ADMMaxTempError < imgParameter.ADMMaxLoopError))
		{
			imgParameter.ADMLoopStopFlag = 1;
		}
	}

	imgParameter.SVDMatRank = 0;
	for (int i = 0; i < MIN(imgParameter.SVDMatRow, imgParameter.SVDMatCol); i++)
	{
		if (SVD_SRank[i][i] > 0)
			imgParameter.SVDMatRank++;
	}


	weightArray = float(imgParameter.SVDMINRowCol - imgParameter.SVDMatRank + 1) / float(imgParameter.SVDMINRowCol);

}