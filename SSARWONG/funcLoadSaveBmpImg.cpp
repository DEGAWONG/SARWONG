/*******************************************************
**	函数作用：从计算机读取图片到二维矩阵
**	char  *lpFileName		=> 读取文件的物理地址
**	myPara  imgParameter		=> 读取图像本身参数
**	float ** distMat			=> 返回指向图像矩阵的二维指针  
**	注释： 本函数有返回值		 
********************************************************/

# include "stdafx.h"

BITMAPFILEHEADER orgBmpfile;
BITMAPINFOHEADER orgBmpinfo;
RGBQUAD orgRgbpalette[256];

BITMAPFILEHEADER FileHeader;
BITMAPINFOHEADER InfoHeader;
RGBQUAD          bmiColors[256];

void setupBMPFileHearder(const int row, const int col, BITMAPFILEHEADER &FileHeader)
{
	int offset = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER) + 256 * sizeof(RGBQUAD);
	FileHeader.bfType = BMP_HEADER_MARKER;
	FileHeader.bfReserved1 = 0;
	FileHeader.bfReserved2 = 0;
	FileHeader.bfSize = row*col + offset;
	FileHeader.bfOffBits = offset;
}
void setupBMPInforHearder(const int row, const int col, BITMAPINFOHEADER &InfoHeader)
{
	InfoHeader.biSize = 40;
	InfoHeader.biPlanes = 1;
	InfoHeader.biCompression = 0L;
	InfoHeader.biXPelsPerMeter = 0;
	InfoHeader.biYPelsPerMeter = 0;
	InfoHeader.biClrUsed = 0;
	InfoHeader.biClrImportant = 0;
	InfoHeader.biWidth = col;
	InfoHeader.biHeight = row;
	InfoHeader.biBitCount = 8;
	InfoHeader.biSizeImage = row * (((col % 4) == 0) ? col : col - (col % 4));
}
void setupBMPColorBoard(RGBQUAD *colorboard, int num)
{
	for (int i = 0; i < num; i++)
	{
		colorboard[i].rgbRed = (unsigned char)i;
		colorboard[i].rgbGreen = (unsigned char)i;
		colorboard[i].rgbBlue = (unsigned char)i;
		colorboard[i].rgbReserved = (unsigned char)0;
	}
}

unsigned char **loadBmpImage(char *lpFileName, myPara &imgParameter)
{
	int   i;
	FILE  *fp;	
	errno_t err = fopen_s(&fp, lpFileName, "rb");

	if (err == 0) 
	{ 
		printf("The Source Image Was Opened!\n");
	}
	else
	{
		printf("The File Was Not Open...\n");
		exit(0);
	};
	fseek(fp, 0, SEEK_SET);
	fread(&orgBmpfile,sizeof(BITMAPFILEHEADER),1,fp); 
	fread(&orgBmpinfo,sizeof(BITMAPINFOHEADER),1,fp);
	fread(&orgRgbpalette,sizeof(RGBQUAD),1,fp);
	// 存储图像的宽度与高度
	imgParameter.row = orgBmpinfo.biHeight;
	imgParameter.col = orgBmpinfo.biWidth;
	// 生成与图像长度与宽度规格相同的指针
	unsigned char  **tempimage;
	tempimage = new unsigned char  *[orgBmpinfo.biHeight];
	for(i=0;i<orgBmpinfo.biHeight;i++)
		tempimage[i] = new unsigned char[orgBmpinfo.biWidth];

	int scanwidth;
	scanwidth=(((orgBmpinfo.biWidth/(8/orgBmpinfo.biBitCount))+3)/4)*4;
	fseek(fp,orgBmpfile.bfOffBits,SEEK_SET);
	for(i=orgBmpinfo.biHeight-1;i>=0;i--) 
		fread(tempimage[i], sizeof(unsigned char), scanwidth, fp);
	fclose(fp);     
	return tempimage;
}
void writeBmpFile(char *lpFileName, unsigned char ** dealedImg, myPara imgParameter)
{
	FILE * filePoint;
	errno_t err;

	// Open for read (will fail if file "crt_fopen_s.c" does not exist)
	err = fopen_s(&filePoint, lpFileName, "wb");

	long int imgSize = imgParameter.row * (((imgParameter.col % 4) == 0) ? imgParameter.col : imgParameter.col - (imgParameter.col % 4));
	// a. 写文件头
	setupBMPFileHearder(imgParameter.row, imgParameter.col, FileHeader);
	fwrite(&FileHeader, sizeof(BITMAPFILEHEADER), 1, filePoint);
	// b. 写信息头
	setupBMPInforHearder(imgParameter.row, imgParameter.col, InfoHeader);
	fwrite(&InfoHeader, sizeof(BITMAPINFOHEADER), 1, filePoint);
	// c. 写调色版
	setupBMPColorBoard(bmiColors, 256);
	fwrite(bmiColors, sizeof(RGBQUAD), 256, filePoint);
	// 将图像的二维指针转换成一维指针写入图像
	unsigned char * outputImg = new unsigned char[imgParameter.row * imgParameter.col];
	for (int i = 0; i < imgParameter.row; i++)
	{
		for (int j = 0; j < imgParameter.col; j++)
		{
			outputImg[i*imgParameter.col + j] = dealedImg[i][j];
		}
	}
	// d. 写位图数据	
	fwrite(outputImg, sizeof(unsigned char), imgSize, filePoint);
	fclose(filePoint);
}
