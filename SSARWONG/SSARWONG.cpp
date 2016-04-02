// SSARWONG.cpp : �������̨Ӧ�ó������ڵ㡣
//

#include "stdafx.h"

myPara imgParameter;

int _tmain(int argc, _TCHAR* argv[])
{
	// ����������е�ʱ��
	clock_t runTime=clock();

	// ���Ȼ�ȡͼ��ָ�� ���Գɹ�
	unsigned char  **oriCharImg = loadBmpImage("./results/Noised.bmp", imgParameter);

	// ��������Ĳ��� ���Գɹ�
	setMyPara(imgParameter);

	// ����ÿһ���ο����� SVDMat ��� �� weight Ȩֵ
	float weightArray;

	// ת���� float ���͵�ָ�룬�õ���float���͵ľ���ָ�� ��ͼ���Գɹ�
	// ��̬���� row * col ���ڴ� �洢����ͼ��
	float **oriFloatImg = new float  *[imgParameter.row];
	for (int i = 0; i < imgParameter.row; i++)
		oriFloatImg[i] = new float[imgParameter.col];
	// ��ȥ��ͼ�������ͼ��
	float **noisyImg = new float  *[imgParameter.row];
	for (int i = 0; i < imgParameter.row; i++)
		noisyImg[i] = new float[imgParameter.col];
	unsigned char **DENOISEDIMG = new unsigned char  *[imgParameter.row];
	for (int i = 0; i < imgParameter.row; i++)
		DENOISEDIMG[i] = new unsigned char[imgParameter.col];
	// ��̬���� (winRow * winCol) * [(row - winRow + 1) * ��col - winCol + 1)]���ڴ�
	// �����洢ͼ��ο����ڷ��ѳ�������֮��ľ��� ��ͼ���Գɹ�
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
	// ����ο������������ڸ����Աȿ�ľ��� ��ͼ���Գɹ�
	// ���ȿ��� distMat  (searchWinRow * searchWinCol) * ((row - winRow )/skipStep + 1)*((col - winCol )/skipStep + 1)
	float **distMat = new float  *[imgParameter.distMatRow];
	for (int i = 0; i < imgParameter.distMatRow; i++)
		distMat[i] = new float[imgParameter.distMatCol];
	// ������ += ����������ԭ����Ĭ����ֵ�Ͻ��м��㣬����һ��Ҫ��������
	for (int i = 0; i < imgParameter.distMatRow; i++)
	{
		for (int j = 0; j < imgParameter.distMatCol; j++)
		{
			distMat[i][j] = 0;
		}
	}
	// distMat �����ת��  
	float **distTransMat = new float  *[imgParameter.distTransMatRow];
	for (int i = 0; i < imgParameter.distTransMatRow; i++)
		distTransMat[i] = new float[imgParameter.distTransMatCol];
	// �õ�ÿһ����Ҫ�����Ƚ��Ƶ� winRow * winCol * simiMatNum ����
	// �����洢ƽ��ֵ�������� ���ÿ���ο�������������ܴ��� imgParameter.simiMatNum = 80 ���ɶԱȵĴ���
	// SVDMat ��� winRow * winCol * simiMatNum 
	float** SVDMat = new float  *[imgParameter.SVDMatRow];
	for (int i = 0; i < imgParameter.SVDMatRow; i++)
		SVDMat[i] = new float[imgParameter.SVDMatCol];
	// L_hat
	float **L_hat = new float *[imgParameter.SVDMatRow];
	for (int i = 0; i < imgParameter.SVDMatRow; i++)
		L_hat[i] = new float[imgParameter.SVDMatCol];
	
	// ���ͼ��Ĵ���
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
	/*�Ȳ���תͼ�񣬱����� MATLAB ����һ��*/

	//// ����ͼ��ķ���
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
	// ��һ��ͼ�����,����û�б仯����ԭ���Ļ��������Ĺ�һ�� ��ͼ���Գɹ�
	regularMat(oriFloatImg, oriFloatImg, imgParameter);
	for (int i = 0; i < imgParameter.row; i++)
	{
		for (int j = 0; j < imgParameter.col; j++)
		{
			// ��ȥ��ͼ��ֵΪ��ʼֵ����ѭ���еĵ�һ�θ�ֵ������ͼ��
			img_output[i][j] = oriFloatImg[i][j];
		}
	}
	// �Եó��ľ�������������������ǰ������
	// ���ȿ��� orderMat  (searchWinRow * searchWinCol) * ((row - winRow )/skipStep + 1)*((col - winCol )/skipStep + 1)
	// �� distMat ��ת�ù����ͬ
	int **orderMat = new int*[imgParameter.distTransMatRow];
	for (int i = 0; i < imgParameter.distTransMatRow; i++)
		orderMat[i] = new int[imgParameter.distTransMatCol];
	//	���츨������
	for (int i = 0; i < imgParameter.distTransMatRow; i++)
	{
		for (int j = 0; j < imgParameter.distTransMatCol; j++)
		{
			orderMat[i][j] = j;
		}
	}
	// ���ó�ÿһ���ο�������ƿ��ƽ�����󣬰������洢
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
	// ��̬����	SVD_U SVD_S SVD_V ���ڴ�ռ� ���ֱ�Ϊ M*M M N*N
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
	// ���� SVDMat �������������ʽ������ SVD ����
	float* SVDArray = new float[imgParameter.SVDMatRow * imgParameter.SVDMatCol];

	/************************************************������ѭ�������ķָ���**************************************************/
	// ��ʼ������ͼѭ��
	for (int imgLoopNum = 0; imgLoopNum < imgParameter.lowRankLoopNum; imgLoopNum++)
	{
		// ���ϸ�ѭ����ȥ��ͼ��ֵ������ѭ��������ͼ��
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
		//ѭ���õ����е� SVDMat
		for (int i = 0; i < imgParameter.distTransMatRow; i++)
		{
			// �����ѭ�����ҵ���һ�� SVDMat 
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
					// �� i ����ԭʼ�� ͼ�� �е���
					temp_col_1 = (i / imgParameter.skipReferMatPerImgRow) * imgParameter.skipStep;
					// �� i ����ԭʼ�� ͼ�� �е���
					temp_row_1 = (i % imgParameter.skipReferMatPerImgRow) * imgParameter.skipStep;
					// �� i ����ԭʼ�� searchMat �е���
					temp_col_2 = orderMat[i][j] / imgParameter.searchWinRow;
					// �� i ����ԭʼ�� searchMat �е���
					temp_row_2 = orderMat[i][j] % imgParameter.searchWinCol;					
					temp_col_0 = temp_col_1 - 0.5 * imgParameter.searchWinCol + temp_col_2;
					temp_row_0 = temp_row_1 - 0.5 * imgParameter.searchWinRow + temp_row_2;	
					transCol = temp_col_0 * (imgParameter.row - imgParameter.winRow + 1) + temp_row_0;
					SVDMat[k][j] = referMat[k][transCol];
				}
			}
			// ͨ��һ�� SVD ����ƽ��ֵ averageArray ������ averageMat 
			// ������
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
			//ADM �㷨�к��м��� SVD 
			ADM_MixedDatafitLowRank(SVDMat, weightArray, L_hat,Z_hat,Y,L_prev,SVD_U,SVD_UArray,SVD_S,SVD_SRank,SVD_SArray,SVD_V,SVD_VArray,SVDArray,imgParameter);
			// ��ƽ��ֵ�ӻ�ԭ SVD �Ľ��ƾ���
			for (int ii = 0; ii < imgParameter.SVDMatRow; ii++)
			{
				for (int jj = 0; jj < imgParameter.SVDMatCol; jj++)
				{
					L_hat[ii][jj] = L_hat[ii][jj] + averageMat[ii][i];
				}
			}

			// �����ѭ�����ҵ���һ�� ORDERMAT ������ 
			for (int j = 0; j < imgParameter.simiMatNum; j++)
			{			
				int simiPatchCol;
				int refer_row_0;
				int refer_col_0;
				int refer_row_1;
				int refer_col_1;
				int refer_row_2;
				int refer_col_2;
				// �� i ����ԭʼ�� ͼ�� �е���
				refer_col_1 = (i / imgParameter.skipReferMatPerImgRow) * imgParameter.skipStep;
				// �� i ����ԭʼ�� ͼ�� �е���
				refer_row_1 = (i % imgParameter.skipReferMatPerImgRow) * imgParameter.skipStep;
				// �� i ����ԭʼ�� searchMat �е���
				refer_col_2 = orderMat[i][j] / imgParameter.searchWinRow;
				// �� i ����ԭʼ�� searchMat �е���
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

		// ��������ͼ�����ж��Ƿ�ﵽ���Ҫ��
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
	// ����ڴ�ռ�
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

		//// ����ڴ�ռ�
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
	// ���ȥ��֮���ͼ��д�뵽���ļ��е�����
	writeBmpFile("DENOISEDIMG.bmp", DENOISEDIMG, imgParameter);
	// ����ڴ�ռ�
	//delete[] * img_output;
	//delete[]   img_output;
	//delete[] * DENOISEDIMG;
	//delete[]   DENOISEDIMG;
	// ��ӡ������ʱ��
	printf(" %d\n", clock() - runTime);
	printf(" %d\n", clock() - runTime);
	return 0;
}

