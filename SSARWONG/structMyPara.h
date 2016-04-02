/*******************************************************
**	结构作用：存储图像的结构参数与设置滤波的参数

**	注释：在以后的遍历矩阵的操作中需要用到的变量比较长，
**		  可以在以后的优化内容中事先对数值进行计算后存储
**		  到另外命名的变量中
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

	float errorOffset; // 小的分母补偿

	float SVDEps; // SVD 的运算精度控制

	int SVDKa;// 计算 SVD 的 MAX(m,n)+1

	float minUDiag; // SVD 矩阵分解中的对角矩阵的最小数值 
	int SVDMatRank; // SVD 矩阵的秩

	float SVDMINRowCol;//SVD 求解矩阵中的行列中的最小数


	int ADMLoopStopFlag;
	int ADMMaxTempNum; //ADM 处理中的实时循环次数
	int ADMMaxLoopNum; //ADM 处理中的最大循环次数 
	float ADMMaxTempError; //ADM 处理中的当次实时误差
	float ADMMaxTempError_0;
	float ADMMaxTempError_1;
	float ADMMaxLoopError; //ADM 处理中的循环终止误差
};