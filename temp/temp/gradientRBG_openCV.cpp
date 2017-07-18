#pragma warning(disable:4996)
#include <vector>
#include <stdio.h>
#include <opencv2/opencv.hpp>

using namespace cv;
using namespace std;

void createAlphaMat(Mat &mat)
{
	uchar startA = 255;
	uchar startR = 255;
	uchar startG = 0;
	uchar startB = 0;
	uchar endA = 127;
	uchar endR = 0;
	uchar endG = 0;
	uchar endB = 255;


	int n = 100;
	float dA = (endA - startA)*1.0f / n;
	float dR = (endR - startR)*1.0f / n;
	float dG = (endG - startG)*1.0f / n;
	float dB = (endB - startB)*1.0f / n;

	float A = startA;
	float R = startR;
	float G = startG;
	float B = startB;
	float n_row = mat.rows*1.0f / n;
	float n_top = n_row;

	for (int i = 0; i < mat.rows; ++i) {
		for (int j = 0; j < mat.cols; ++j) {
			Vec4b& rgba = mat.at<Vec4b>(i, j);
			rgba[0] = saturate_cast<uchar>(pow(B / UCHAR_MAX, 1 / 2.2) * UCHAR_MAX);
			rgba[1] = saturate_cast<uchar>(pow(G / UCHAR_MAX, 1 / 2.2) * UCHAR_MAX);
			rgba[2] = saturate_cast<uchar>(pow(R / UCHAR_MAX, 1 / 2.2) * UCHAR_MAX);
			rgba[3] = A ;

			if (i+1 >= n_top)
			{
				R += dR;
				G += dG;
				B += dB;
				A += dA;
				n_top += n_row;
			}
		}
	}
}

int main(int argv, char **argc)
{
	// Create mat with alpha channel
	Mat mat(512, 1024, CV_8UC4);
	createAlphaMat(mat);

	vector<int> compression_params;
	compression_params.push_back(CV_IMWRITE_PNG_COMPRESSION);
	compression_params.push_back(9);

	try {
		imwrite("gradientRGBA.png", mat, compression_params);
	}
	catch (runtime_error& ex) {
		fprintf(stderr, "Exception converting image to PNG format: %s\n", ex.what());
		return 1;
	}

	namedWindow("gradientRGBA", CV_WINDOW_AUTOSIZE);
	imshow("gradientRGBA", mat);
	waitKey();

	fprintf(stdout, "Saved PNG file with alpha data.\n");
	return 0;
}