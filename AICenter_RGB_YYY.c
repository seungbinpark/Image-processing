#pragma warning(disable:4996)
#include <stdio.h>
#include <Windows.h>

int main(int argc, char* argv[]) {
	BITMAPFILEHEADER bmpFile;
	BITMAPINFOHEADER bmpInfo;

	FILE* inputFile = NULL;

	inputFile = fopen("AICenter.bmp", "rb");
	fread(&bmpFile, sizeof(BITMAPFILEHEADER), 1, inputFile);
	fread(&bmpInfo, sizeof(BITMAPINFOHEADER), 1, inputFile);

	int width = bmpInfo.biWidth;	//rgb 포함
	int height = bmpInfo.biHeight;		
	int size = bmpInfo.biSizeImage;
	int bitCnt = bmpInfo.biBitCount;
	int stride = (((bitCnt / 8) * width) + 3) / 4 * 4;


	printf("W: %d(%d)\nH: %d\nS: %d\nD: %d\n\n", width, stride, height, size, bitCnt);

	unsigned char* inputImg = NULL, * outputImg = NULL;
	inputImg = (unsigned char*)calloc(size, sizeof(unsigned char));
	outputImg = (unsigned char*)calloc(size, sizeof(unsigned char));
	fread(inputImg, sizeof(unsigned char), size, inputFile);


	float Y,Cb,Cr;


	//input img는 고려할게 많음
	// rgb를 y로 바꿔서 영상 좌표를
	// outputImg 작업

	//for (int j = 0;j < height;j++) {
	//	for (int i = 0;i < width;i++) {
	//		Y = 0.299 * inputImg[j * stride + 3 * i + 2] + 0.587 * inputImg[j * stride + 3 * i + 1] + 0.114 * inputImg[j * stride + 3 * i + 0];
	//		Cb = -0.169 * inputImg[j * stride + 3 * i + 2] + -0.331 * inputImg[j * stride + 3 * i + 1] + 0.500 * inputImg[j * stride + 3 * i + 0];
	//		Cr = 0.500 * inputImg[j * stride + 3 * i + 2] - 0.419 * inputImg[j * stride + 3 * i + 1] - 0.0813 * inputImg[j * stride + 3 * i + 0];
	//	}
	//}



	for (int j = 0;j < height;j++) {
		for (int i = 0;i < width;i++) {
			Y = 0.299 * inputImg[j * stride + 3 * i + 2] + 0.587 * inputImg[j * stride + 3 * i + 1] + 0.114 * inputImg[j * stride + 3 * i + 0];
			Cb = -0.169 * inputImg[j * stride + 3 * i + 2] + -0.331 * inputImg[j * stride + 3 * i + 1] + 0.500 * inputImg[j * stride + 3 * i + 0];
			Cr = 0.500 * inputImg[j * stride + 3 * i + 2] - 0.419 * inputImg[j * stride + 3 * i + 1] - 0.0813 * inputImg[j * stride + 3 * i + 0];

			outputImg[j * stride + 3 * i + 2] = (unsigned char)(Y > 255 ? 255 : (Y < 0 ? 0 : Y));
			outputImg[j * stride + 3 * i + 1] = (unsigned char)(Y > 255 ? 255 : (Y < 0 ? 0 : Y));
			outputImg[j * stride + 3 * i + 0] = (unsigned char)(Y > 255 ? 255 : (Y < 0 ? 0 : Y));


			outputImg[j * stride + 3 * i + 0] = Y;
			outputImg[j * stride + 3 * i + 1] = Y;
			outputImg[j * stride + 3 * i + 2] = Y;

		}
	}


	FILE* outputFile = fopen("Output.bmp", "wb");
	fwrite(&bmpFile, sizeof(BITMAPFILEHEADER), 1, outputFile);
	fwrite(&bmpInfo, sizeof(BITMAPINFOHEADER), 1, outputFile);
	fwrite(outputImg, sizeof(unsigned char), size, outputFile);

	free(outputImg);
	free(inputImg);
	fclose(inputFile);
	fclose(outputFile);
}