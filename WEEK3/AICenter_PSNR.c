#pragma warning(disable:4996)
#include <stdio.h>
#include <Windows.h>
#include <math.h>

int main(int argc, char* argv[]) {
	BITMAPFILEHEADER bmpFile1;
	BITMAPFILEHEADER bmpFile2;
	BITMAPINFOHEADER bmpInfo1;
	BITMAPINFOHEADER bmpInfo2;

	FILE* inputFile1 = NULL;
	FILE* inputFile2 = NULL;

	inputFile1 = fopen("AICenterY.bmp", "rb");
	inputFile2 = fopen("AICenterY_Noise.bmp", "rb");
	fread(&bmpFile1, sizeof(BITMAPFILEHEADER), 1, inputFile1);
	fread(&bmpInfo1, sizeof(BITMAPINFOHEADER), 1, inputFile1);
	fread(&bmpFile2, sizeof(BITMAPFILEHEADER), 1, inputFile2);
	fread(&bmpInfo2, sizeof(BITMAPINFOHEADER), 1, inputFile2);

	int width = bmpInfo1.biWidth;	//rgb 포함
	int height = bmpInfo1.biHeight;
	int size = bmpInfo1.biSizeImage;
	int bitCnt = bmpInfo1.biBitCount;
	int stride = (((bitCnt / 8) * width) + 3) / 4 * 4;


	printf("W: %d(%d)\nH: %d\nS: %d\nD: %d\n\n", width, stride, height, size, bitCnt);

	unsigned char* inputImg1 = NULL, * outputImg = NULL;
	unsigned char* inputImg2 = NULL;

	inputImg1 = (unsigned char*)calloc(size, sizeof(unsigned char));
	inputImg2 = (unsigned char*)calloc(size, sizeof(unsigned char));
	//outputImg = (unsigned char*)calloc(size, sizeof(unsigned char));

	fread(inputImg1, sizeof(unsigned char), size, inputFile1);
	fread(inputImg2, sizeof(unsigned char), size, inputFile2);

	//float Cb, Cr, R, G, B;
	unsigned char* Y1 = NULL;
	unsigned char* Y2 = NULL;
	unsigned char* Y3 = NULL;

	Y1 = (unsigned char*)calloc(height * width, sizeof(unsigned char));
	Y2 = (unsigned char*)calloc(height * width, sizeof(unsigned char));
	Y3 = (unsigned char*)calloc(height * width, sizeof(unsigned char));

	//input img는 고려할게 많음
	// rgb를 y로 바꿔서 영상 좌표를
	// outputImg 작업

	for (int j = 0;j < height;j++) {
		for (int i = 0;i < width;i++) {
			Y1[j * width  +i] = inputImg1[j * stride + 3 * i + 0];
			Y2[j * width + i] = inputImg2[j * stride + 3 * i + 0];
			//Y1 = (unsigned char)(Y1 > 255 ? 255 : (Y1 < 0 ? 0 : Y1));
			//Y2 = (unsigned char)(Y2 > 255 ? 255 : (Y2 < 0 ? 0 : Y2));
		}
	}


	double mse = 0, psnr;


	for (int j = 0;j < height;j++) {
		for (int i = 0;i < width;i++) {
			mse += (double)((Y2[j * width + i] - Y1[j * width + i]) * (Y2[j * width + i] - Y1[j * width + i]));
		}
	}

	mse /= (width * height);
	psnr = mse != 0.0 ? 10.0 * log10(255 * 255 / mse) : 99.99;
	printf("MSE = %.2lf\nPSNR = %.2lfdB\n", mse, psnr);


	//FILE* outputFile = fopen("Output.bmp", "wb");
	//fwrite(&bmpFile, sizeof(BITMAPFILEHEADER), 1, outputFile);
	//fwrite(&bmpInfo, sizeof(BITMAPINFOHEADER), 1, outputFile);
	//fwrite(outputImg, sizeof(unsigned char), size, outputFile);

	free(Y1);
	free(Y2);
	free(outputImg);
	free(inputImg1);
	free(inputImg2);
	fclose(inputFile1);
	fclose(inputFile2);
	//fclose(outputFile);
}