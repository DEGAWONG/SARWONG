/*******************************************************
**	�ṹ���ã��洢ͼ��Ľṹ�����������˲��Ĳ���

**	ע�ͣ����Ժ�ı�������Ĳ�������Ҫ�õ��ı����Ƚϳ���
**		  �������Ժ���Ż����������ȶ���ֵ���м����洢
**		  �����������ı�����
********************************************************/
struct myPara
{
	int skipStep;

	int row;
	int winRow;
	int searchWinRow;
	
	int col;
	int winCol;
	int searchWinCol;
	
	int simiMatNum;

	int referMatRow;
	int referMatCol;

	int distMatRow;
	int distMatCol;
	
	int distTransMatRow;
	int distTransMatCol;

	int skipReferMatPerImgRow;

	int lowRankLoopStopFlag;
	int lowRankLoopNum;
	int lowRankTempNum;
	float lowRankLoopError;
	float lowRankTempError;
	float lowRankTempError_0;
	float lowRankTempError_1;

	int SVDMatRow;
	int SVDMatCol;

	float lambda_1;
	float lambda_2;
	float lambda_3;

	float rou;
	float miu;
	float alpha;

	float tau_1;
	float tau_2;
	float factor_0;

	float errorOffset; // С�ķ�ĸ����

	float SVDEps; // SVD �����㾫�ȿ���

	int SVDKa;// ���� SVD �� MAX(m,n)+1

	float minUDiag; // SVD ����ֽ��еĶԽǾ������С��ֵ 
	int SVDMatRank; // SVD �������

	float SVDMINRowCol;//SVD �������е������е���С��


	int ADMLoopStopFlag;
	int ADMMaxTempNum; //ADM �����е�ʵʱѭ������
	int ADMMaxLoopNum; //ADM �����е����ѭ������ 
	float ADMMaxTempError; //ADM �����еĵ���ʵʱ���
	float ADMMaxTempError_0;
	float ADMMaxTempError_1;
	float ADMMaxLoopError; //ADM �����е�ѭ����ֹ���
};