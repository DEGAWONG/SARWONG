// SSARWONG.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"

myPara imgParameter;

int _tmain(int argc, _TCHAR* argv[])
{
	// 计算程序运行的时间
	clock_t runTime=clock();

	// 首先获取图像指针 测试成功
	unsigned char  **oriCharImg = loadBmpImage("./results/Noised.bmp", imgParameter);

	// 设置总体的参数 测试成功
	setMyPara(imgParameter);

	// 保留每一个参考窗口 SVDMat 规格 的 weight 权值
	float weightArray;

	// 转换成 float 类型的指针，得到了float类型的矩阵指针 带图测试成功
	// 动态分配 row * col 的内存 存储整幅图像
	float **oriFloatImg = new float  *[imgParameter.row];
	for (int i = 0; i < imgParameter.row; i++)
		oriFloatImg[i] = new float[imgParameter.col];
	// 待去噪图像与输出图像
	float **noisyImg = new float  *[imgParameter.row];
	for (int i = 0; i < imgParameter.row; i++)
		noisyImg[i] = new float[imgParameter.col];
	unsigned char **DENOISEDIMG = new unsigned char  *[imgParameter.row];
	for (int i = 0; i < imgParameter.row; i++)
		DENOISEDIMG[i] = new unsigned char[imgParameter.col];
	// 动态分配 (winRow * winCol) * [(row - winRow + 1) * （col - winCol + 1)]的内存
	// 用来存储图像参考窗口分裂成列向量之后的矩阵 带图测试成功
	float **referMat = new float  *[imgParameter.referMatRow];
	for (int i = 0; i < (imgParameter.referMatRow); i++)
		referMat[i] = new float[imgParameter.referMatCol];
	float **referMat_y = new float  *[imgParameter.referMatRow];
	for (int i = 0; i < (imgParameter.referMatRow); i++)
		referMat_y[i] = new float[imgParameter.referMatCol];
	float **referMat_w = new float  *[imgParameter.referMatRow];
	for (int i = 0; i < (imgParameter.referMatRow); i++)
		referMat_w[i] = new float[imgParameter.referMatCol];
	for (int i = 0; i < imgParameter.referMatRow; i++)
	{
		for (int j = 0; j < imgParameter.referMatCol; j++)
		{
			referMat_y[i][j] = 0;
			referMat_w[i][j] = 0;
		}
	}
	// 计算参考块与搜索块内各个对比块的距离 带图测试成功
	// 首先开辟 distMat  (searchWinRow * searchWinCol) * ((row - winRow )/skipStep + 1)*((col - winCol )/skipStep + 1)
	float **distMat = new float  *[imgParameter.distMatRow];
	for (int i = 0; i < imgParameter.distMatRow; i++)
		distMat[i] = new float[imgParameter.distMatCol];
	// 后面是 += 操作，是在原来的默认数值上进行计算，所以一定要先行清零
	for (int i = 0; i < imgParameter.distMatRow; i++)
	{
		for (int j = 0; j < imgParameter.distMatCol; j++)
		{
			distMat[i][j] = 0;
		}
	}
	// distMat 矩阵的转置  
	float **distTransMat = new float  *[imgParameter.distTransMatRow];
	for (int i = 0; i < imgParameter.distTransMatRow; i++)
		distTransMat[i] = new float[imgParameter.distTransMatCol];
	// 得到每一个需要做低秩近似的 winRow * winCol * simiMatNum 矩阵
	// 遍历存储平均值的所有行 如果每个参考框的搜索域内总大于 imgParameter.simiMatNum = 80 个可对比的窗口
	// SVDMat 规格 winRow * winCol * simiMatNum 
	float** SVDMat = new float  *[imgParameter.SVDMatRow];
	for (int i = 0; i < imgParameter.SVDMatRow; i++)
		SVDMat[i] = new float[imgParameter.SVDMatCol];
	// L_hat
	float **L_hat = new float *[imgParameter.SVDMatRow];
	for (int i = 0; i < imgParameter.SVDMatRow; i++)
		L_hat[i] = new float[imgParameter.SVDMatCol];
	
	// 输出图像的处理
	float **img_output = new float  *[imgParameter.row];
	for (int i = 0; i < imgParameter.row; i++)
		img_output[i] = new float[imgParameter.col];
	float **img_weight = new float  *[imgParameter.row];
	for (int i = 0; i < imgParameter.row; i++)
		img_weight[i] = new float[imgParameter.col];
	for (int i = 0; i < imgParameter.row; i++)
	{
		for (int j = 0; j < imgParameter.col; j++)
		{
			img_output[i][j] = 0;
			img_weight[i][j] = 0;
		}
	}
	/*先不翻转图像，保持与 MATLAB 数据一致*/

	//// 调换图像的方向
	//for (int i = 0; i < imgParameter.row / 2; i++)
	//{
	//	for (int j = 0; j < imgParameter.col; j++)
	//	{
	//		unsigned char tempPixel;
	//		tempPixel = oriCharImg[i][j];
	//		oriCharImg[i][j] = oriCharImg[imgParameter.row - 1 - i][j];
	//		oriCharImg[imgParameter.row - 1 - i][j] = tempPixel;
	//	}
	//}
	for (int i = 0; i < imgParameter.row; i++)
	{
		for (int j = 0; j < imgParameter.col; j++)
		{
			oriFloatImg[i][j] = (float)oriCharImg[i][j];
		}
	}
	// 归一化图像矩阵,名字没有变化，在原来的基础上做的归一化 带图测试成功
	regularMat(oriFloatImg, oriFloatImg, imgParameter);
	for (int i = 0; i < imgParameter.row; i++)
	{
		for (int j = 0; j < imgParameter.col; j++)
		{
			// 给去噪图像赋值为初始值，在循环中的第一次赋值给噪声图像
			img_output[i][j] = oriFloatImg[i][j];
		}
	}
	// 对得出的距离矩阵进行排序保留排序前的索引
	// 首先开辟 orderMat  (searchWinRow * searchWinCol) * ((row - winRow )/skipStep + 1)*((col - winCol )/skipStep + 1)
	// 与 distMat 的转置规格相同
	int **orderMat = new int*[imgParameter.distTransMatRow];
	for (int i = 0; i < imgParameter.distTransMatRow; i++)
		orderMat[i] = new int[imgParameter.distTransMatCol];
	//	构造辅助数组
	for (int i = 0; i < imgParameter.distTransMatRow; i++)
	{
		for (int j = 0; j < imgParameter.distTransMatCol; j++)
		{
			orderMat[i][j] = j;
		}
	}
	// 最后得出每一个参考块的相似块的平均矩阵，按列来存储
	float** averageMat = new float  *[imgParameter.winRow * imgParameter.winCol];
	for (int i = 0; i < imgParameter.winRow * imgParameter.winCol; i++)
		averageMat[i] = new float[imgParameter.distTransMatRow];
	for (int i = 0; i < (imgParameter.winRow * imgParameter.winCol); i++)
	{
		for (int j = 0; j < imgParameter.distTransMatRow; j++)
		{
			averageMat[i][j] = 0;
		}
	}
	// Z_hat
	float **Z_hat = new float *[imgParameter.SVDMatRow];
	for (int i = 0; i < imgParameter.SVDMatRow; i++)
	    Z_hat[i]=new float[imgParameter.SVDMatCol];
	for (int i = 0; i < imgParameter.SVDMatRow; i++)
	{
		for (int j = 0; j < imgParameter.SVDMatCol; j++)
		{
			Z_hat[i][j] = 0;
		}
	}
	// Y
	float **Y = new float *[imgParameter.SVDMatRow];
	for (int i = 0; i < imgParameter.SVDMatRow; i++)
	    Y[i]=new float[imgParameter.SVDMatCol];
	for (int i = 0; i < imgParameter.SVDMatRow; i++)
	{
		for (int j = 0; j < imgParameter.SVDMatCol; j++)
		{
			Y[i][j] = 0;
		}
	}
	// L_prev
	float **L_prev = new float *[imgParameter.SVDMatRow];
	for (int i = 0; i < imgParameter.SVDMatRow; i++)
		L_prev[i]=new float[imgParameter.SVDMatCol];
	for (int i = 0; i < imgParameter.SVDMatRow; i++)
	{
		for (int j = 0; j < imgParameter.SVDMatCol; j++)
		{
			L_prev[i][j] = 0;
		}
	}
	// 动态开辟	SVD_U SVD_S SVD_V 的内存空间 规格分别为 M*M M N*N
	float **SVD_U = new float *[imgParameter.SVDMatRow];
	for (int i = 0; i < imgParameter.SVDMatRow; i++)
		SVD_U[i] = new float[imgParameter.SVDMatRow];
	float *SVD_UArray = new float[imgParameter.SVDMatRow * imgParameter.SVDMatRow];
	for (int i = 0; i < imgParameter.SVDMatRow; i++)
	{
		for (int j = 0; j < imgParameter.SVDMatRow; j++)
		{
			SVD_U[i][j] = 0;
			SVD_UArray[i * imgParameter.SVDMatRow + imgParameter.SVDMatRow] = 0;
		}
	}
	float **SVD_S = new float *[imgParameter.SVDMatRow];
	for (int i = 0; i < imgParameter.SVDMatRow; i++)
		SVD_S[i] = new float[imgParameter.SVDMatCol];
	float **SVD_SRank = new float *[imgParameter.SVDMatRow];
	for (int i = 0; i < imgParameter.SVDMatRow; i++)
		SVD_SRank[i] = new float[imgParameter.SVDMatCol];
	float  *SVD_SArray = new float[imgParameter.SVDMatRow * imgParameter.SVDMatCol];
	for (int i = 0; i < imgParameter.SVDMatRow; i++)
	{
		for (int j = 0; j < imgParameter.SVDMatCol; j++)
		{
			SVD_S[i][j] = 0;
			SVD_SArray[i * imgParameter.SVDMatRow + j] = 0;
		}
	}
	float **SVD_V = new float *[imgParameter.SVDMatCol];
	for (int i = 0; i < imgParameter.SVDMatCol; i++)
		SVD_V[i] = new float[imgParameter.SVDMatCol];
	float *SVD_VArray = new float[imgParameter.SVDMatCol * imgParameter.SVDMatCol];
	for (int i = 0; i < imgParameter.SVDMatCol; i++)
	{
		for (int j = 0; j < imgParameter.SVDMatCol; j++)
		{
			SVD_V[i][j] = 0;
			SVD_VArray[i * imgParameter.SVDMatCol + imgParameter.SVDMatCol] = 0;
		}
	}
	// 建立 SVDMat 矩阵的行向量形式方便做 SVD 运算
	float* SVDArray = new float[imgParameter.SVDMatRow * imgParameter.SVDMatCol];

	/************************************************我是主循环华丽的分割线**************************************************/
	// 开始外层的整图循环
	for (int imgLoopNum = 0; imgLoopNum < imgParameter.lowRankLoopNum; imgLoopNum++)
	{
		// 将上个循环的去噪图像赋值给本次循环的噪声图像
		for (int i = 0; i < imgParameter.row; i++)
		{
			for (int j = 0; j < imgParameter.col; j++)
			{
				noisyImg[i][j] = img_output[i][j];
			}
		}
		reShapeMat2Col(noisyImg, referMat, imgParameter);	
		cacuDistance(referMat, distMat, imgParameter);
		matTranspose(distMat, distTransMat,imgParameter);
		sortIndex(distTransMat, orderMat, imgParameter);
		for (int i = 0; i < 80; i++)
		{
			printf("%d\n", orderMat[i][0]);
		}
		//循环得到所有的 SVDMat
		for (int i = 0; i < imgParameter.distTransMatRow; i++)
		{
			// 走完此循环，找到第一个 SVDMat 
			for (int j = 0; j < imgParameter.SVDMatCol; j++)
			{
				for (int k = 0; k < imgParameter.SVDMatRow; k++)
				{
					int transCol;
					int temp_row_0;
					int temp_col_0;
					int temp_row_1;
					int temp_col_1;
					int temp_row_2;
					int temp_col_2;
					// 第 i 行在原始的 图像 中的列
					temp_col_1 = (i / imgParameter.skipReferMatPerImgRow) * imgParameter.skipStep;
					// 第 i 行在原始的 图像 中的行
					temp_row_1 = (i % imgParameter.skipReferMatPerImgRow) * imgParameter.skipStep;
					// 第 i 行在原始的 searchMat 中的列
					temp_col_2 = orderMat[i][j] / imgParameter.searchWinRow;
					// 第 i 行在原始的 searchMat 中的行
					temp_row_2 = orderMat[i][j] % imgParameter.searchWinCol;					
					temp_col_0 = temp_col_1 - 0.5 * imgParameter.searchWinCol + temp_col_2;
					temp_row_0 = temp_row_1 - 0.5 * imgParameter.searchWinRow + temp_row_2;	
					transCol = temp_col_0 * (imgParameter.row - imgParameter.winRow + 1) + temp_row_0;
					SVDMat[k][j] = referMat[k][transCol];
				}
			}
			// 通过一个 SVD 计算平均值 averageArray 最后组成 averageMat 
			// 先清零
			for (int ii = 0; ii < imgParameter.SVDMatRow; ii++)
			{
				for (int jj = 0; jj < imgParameter.SVDMatCol; jj++)
				{
					averageMat[ii][i] += SVDMat[ii][jj] / imgParameter.simiMatNum;
				}
			}
			for (int ii = 0; ii < imgParameter.SVDMatRow; ii++)
			{
				for (int jj = 0; jj < imgParameter.SVDMatCol; jj++)
				{
					SVDMat[ii][jj] = SVDMat[ii][jj] - averageMat[ii][i];
				}
			}
			for (int ii = 0; ii < imgParameter.SVDMatRow; ii++)
			{
				for (int jj = 0; jj < imgParameter.SVDMatCol; jj++)
				{
					L_hat[ii][jj] = 0.0;
				}
			}
			//ADM 算法中含有计算 SVD 
			ADM_MixedDatafitLowRank(SVDMat, weightArray, L_hat,Z_hat,Y,L_prev,SVD_U,SVD_UArray,SVD_S,SVD_SRank,SVD_SArray,SVD_V,SVD_VArray,SVDArray,imgParameter);
			// 将平均值加回原 SVD 的近似矩阵
			for (int ii = 0; ii < imgParameter.SVDMatRow; ii++)
			{
				for (int jj = 0; jj < imgParameter.SVDMatCol; jj++)
				{
					L_hat[ii][jj] = L_hat[ii][jj] + averageMat[ii][i];
				}
			}

			// 走完此循环，找到第一个 ORDERMAT 的列数 
			for (int j = 0; j < imgParameter.simiMatNum; j++)
			{			
				int simiPatchCol;
				int refer_row_0;
				int refer_col_0;
				int refer_row_1;
				int refer_col_1;
				int refer_row_2;
				int refer_col_2;
				// 第 i 行在原始的 图像 中的列
				refer_col_1 = (i / imgParameter.skipReferMatPerImgRow) * imgParameter.skipStep;
				// 第 i 行在原始的 图像 中的行
				refer_row_1 = (i % imgParameter.skipReferMatPerImgRow) * imgParameter.skipStep;
				// 第 i 行在原始的 searchMat 中的列
				refer_col_2 = orderMat[i][j] / imgParameter.searchWinRow;
				// 第 i 行在原始的 searchMat 中的行
				refer_row_2 = orderMat[i][j] % imgParameter.searchWinCol;
				refer_col_0 = refer_col_1 - 0.5 * imgParameter.searchWinCol + refer_col_2;
				refer_row_0 = refer_row_1 - 0.5 * imgParameter.searchWinRow + refer_row_2;
				simiPatchCol = refer_col_0 * (imgParameter.row - imgParameter.winRow + 1) + refer_row_0;
				for (int k = 0; k < imgParameter.winRow * imgParameter.winCol; k++)
				{
					referMat_w[k][simiPatchCol] = referMat_w[k][simiPatchCol] + weightArray;
					referMat_y[k][simiPatchCol] = referMat_y[k][simiPatchCol] + weightArray * L_hat[k][j];
				}
			}
		}
		//for (int i = 0; i < 164; i++)
		//{
		//	if (i == 0)
		//	{
		//		printf("\n %d :", i + 1);
		//	}
		//	for (int j = 0; j < 5; j++)
		//	{

		//		printf("%f  ", referMat_y[j][i]);
		//		if (j == 4)
		//		{
		//			printf("\n %d :", i + 2);
		//		}
		//	}
		//}
		for (int i = 0; i < imgParameter.row; i++)
		{
			for (int j = 0; j < imgParameter.col; j++)
			{
				img_output[i][j] = 0.0;
				img_weight[i][j] = 0.0;
			}
		}
		for (int i = 0; i < imgParameter.referMatRow; i++)
		{
			for (int j = 0; j < (imgParameter.col - imgParameter.winCol + 1); j++)
			{
				for (int k = 0; k < (imgParameter.row - imgParameter.winRow + 1); k++)
				{
					img_output[k + i / 8][j + i % 8] = img_output[k + i / 8][j + i % 8] + referMat_y[i][j * (imgParameter.row - imgParameter.winRow + 1) + k];
					img_weight[k + i / 8][j + i % 8] = img_weight[k + i / 8][j + i % 8] + referMat_w[i][j * (imgParameter.row - imgParameter.winRow + 1) + k];
				}
			}
		}
		for (int i = 0; i < imgParameter.row; i++)
		{
			for (int j = 0; j < imgParameter.col; j++)
			{
				img_output[i][j] = img_output[i][j] / (img_weight[i][j] + imgParameter.errorOffset);
			}
		}

		// 计算整幅图的误差，判断是否达到误差要求
		imgParameter.lowRankTempError = 0.0;
		imgParameter.lowRankTempError_0 = 0.0;
		imgParameter.lowRankTempError_1 = 0.0;
		for (int i = 0; i < imgParameter.row; i++)
		{
			for (int j = 0; j < imgParameter.col; j++)
			{
				imgParameter.lowRankTempError_0 += ((img_output[i][j] - noisyImg[i][j])* (img_output[i][j] - noisyImg[i][j]));
				imgParameter.lowRankTempError_1 += (img_output[i][j] * img_output[i][j]);
			}
		}
		imgParameter.lowRankTempError = imgParameter.lowRankTempError_0 / imgParameter.lowRankTempError_1;
		if (imgParameter.lowRankTempError <= imgParameter.lowRankLoopError)
		{
			goto DENOISEDIMG;
		}
	}
	// 清除内存空间
	//delete[] * averageMat;
	//delete[]   averageMat;
	//delete[] * orderMat;
	//delete[]   orderMat;
	//delete[] * img_weight;
	//delete[]   img_weight;
	//delete[] * L_hat;
	//delete[]   L_hat;
	//delete[] * SVDMat;
	//delete[]   SVDMat;
	//delete[] * distTransMat;
	//delete[]   distTransMat;
	//delete[] * distMat;
	//delete[]   distMat;
	//delete[] * referMat_w;
	//delete[]   referMat_w;
	//delete[] * referMat_y;
	//delete[]   referMat_y;
	//delete[] * referMat;
	//delete[]   referMat;
	//delete[] * noisyImg;
	//delete[]   noisyImg;
	//delete[] * oriFloatImg;
	//delete[]   oriFloatImg;
	//delete[] * oriCharImg;
	//delete[]   oriCharImg;

		//// 清除内存空间
	//delete[] SVDArray;
	//delete[] SVD_VArray;
	//for (int i = 0; i < imgParameter.SVDMatCol; i++)
	//{
	//	delete[] SVD_V[i];
	//}
	//delete[]   SVD_V;
	//delete[]   SVD_SArray;
	//for (int i = 0; i < imgParameter.SVDMatCol; i++)
	//{
	//	delete[] SVD_SRank[i];
	//}
	//delete[]   SVD_SRank;
	//for (int i = 0; i < imgParameter.SVDMatCol; i++)
	//{
	//	delete[] SVD_S[i];
	//}
	//delete[]   SVD_S;
	//delete[]   SVD_UArray;
	//for (int i = 0; i < imgParameter.SVDMatCol; i++)
	//{
	//	delete[] SVD_U[i];
	//}
	//delete[]   SVD_U;
	//for (int i = 0; i < imgParameter.SVDMatCol; i++)
	//{
	//	delete[] L_prev[i];
	//}
	//delete[]   L_prev;
	//for (int i = 0; i < imgParameter.SVDMatCol; i++)
	//{
	//	delete[] Y[i];
	//}
	//delete[]   Y;
	//for (int i = 0; i < imgParameter.SVDMatCol; i++)
	//{
	//	delete[] Z_hat[i];
	//}
	//delete[]   Z_hat;
	
DENOISEDIMG:
	for (int i = 0; i < imgParameter.row; i++)
	{
		for (int j = 0; j < imgParameter.col; j++)
		{
			DENOISEDIMG[i][j] = (unsigned char)(img_output[i][j] * 255.0);
		}
	}
	// 完成去噪之后的图像写入到本文件夹的名字
	writeBmpFile("DENOISEDIMG.bmp", DENOISEDIMG, imgParameter);
	// 清除内存空间
	//delete[] * img_output;
	//delete[]   img_output;
	//delete[] * DENOISEDIMG;
	//delete[]   DENOISEDIMG;
	// 打印出运行时间
	printf(" %d\n", clock() - runTime);
	printf(" %d\n", clock() - runTime);
	return 0;
}

