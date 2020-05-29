#include<opencv.hpp>
using namespace std;
using namespace cv;

void Hog(Mat refMat, float *ref_hist) {
	double pi = 3.1415926;
	Mat gx, gy;
	Mat mag, angle;
	Sobel(refMat, gx, CV_32F, 1, 0, 1);
	Sobel(refMat, gy, CV_32F, 0, 1, 1);
	cartToPolar(gx, gy, mag, angle, false);

	int cellsize = 16;
	int nX = refMat.cols / cellsize;
	int nY = refMat.rows / cellsize;

	int bins = 8;
	ref_hist = new float[bins];
	memset(ref_hist, 0, sizeof(float)*bins);
	int binsnum = 0;
	for (int n = 0; n < nX; n++) {
		for (int m = 0; m < nY; m++) {
			for (int n2 = n * 16; n2 < n * 16 + 16; n2++) {
				for (int m2 = m * 16; m2 < m * 16 + 16; m2++) {

					int angle_lh = angle.at<float>(n2, m2) / 45;
					float mag_l = mag.at<float>(n2, m2);
					ref_hist[angle_lh + binsnum * 8] += mag_l;

				}
			}
			binsnum++;
		}
	}
	delete[] ref_hist;

}

int main() {
	int bins = 8;

	float  *hist1 = new float[bins];
	memset(hist1, 0, sizeof(float)*bins);
	

	float  *hist2 = new float[bins];
	memset(hist2, 0, sizeof(float)*bins);


	Mat bgdimg = imread("E:\\dignal image\\img.png", 0);
	Mat obj = imread("E:\\dignal image\\template.png", 0);

	int  b_height = bgdimg.rows;
	int b_width = bgdimg.cols;

	int height = obj.rows;
	int width = obj.cols;

	float dif = 0;
	float mdif = 0;
	
	Rect boud;
	Hog(obj, hist1);

	float compare1=0, compare2=0;

	for (int n = 0; n < b_height - height; n++) {
		for (int m = 0; m < b_width - width; m++) {
			Mat compare = bgdimg(Rect(n, m, b_height, b_width));
			 Hog(compare,hist2);

			 for (int i = 0; i < hist1.size(); i++) {
				  dif += (hist1[i] - hist2[i])*(hist1[i] - hist2[i]);
			 }
	
			 if (dif <= mdif) {
				 mdif = dif;
				 boud = Rect(n, m, b_height, b_width);
			 }
		}
	}
	rectangle(bgdimg, boud, CV_RGB(255, 255, 255), 1, 8, 0);
	imshow("background", bgdimg);
	waitKey(0);
	delete[] hist1;
	delete[] hist2;
}