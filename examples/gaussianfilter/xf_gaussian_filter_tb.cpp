/***************************************************************************
 Copyright (c) 2016, Xilinx, Inc.
 All rights reserved.

 Redistribution and use in source and binary forms, with or without modification,
 are permitted provided that the following conditions are met:

 1. Redistributions of source code must retain the above copyright notice,
 this list of conditions and the following disclaimer.

 2. Redistributions in binary form must reproduce the above copyright notice,
 this list of conditions and the following disclaimer in the documentation
 and/or other materials provided with the distribution.

 3. Neither the name of the copyright holder nor the names of its contributors
 may be used to endorse or promote products derived from this software
 without specific prior written permission.

 THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
 THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
 IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
 INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
 OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
 EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

 ***************************************************************************/

#include "xf_gaussian_filter_config.h"


using namespace std;


int main(int argc, char **argv) {

	if (argc != 2)
	{
		printf("Usage: <executable> <input image path> \n");
		return -1;
	}

	cv::Mat in_img, out_img, ocv_ref, in_img_gau;
	cv::Mat in_gray, in_gray1, diff;

	in_img = cv::imread(argv[1], 1); // reading in the color image
	if (!in_img.data) {
		printf("Failed to load the image ... !!!");
		return -1;
	}

	extractChannel(in_img, in_gray, 1);

	ocv_ref.create(in_gray.rows, in_gray.cols, in_gray.depth()); // create memory for output image

#if FILTER_WIDTH==3
	float sigma = 0.5f;
#define FILTER 3
#endif
#if FILTER_WIDTH==7
	float sigma=1.16666f;
	#define FILTER 7
#endif
#if FILTER_WIDTH==5
	float sigma = 0.8333f;
	#define FILTER 5
#endif

	// OpenCV Gaussian filter function
	cv::GaussianBlur(in_gray, ocv_ref, cvSize(FILTER_WIDTH, FILTER_WIDTH),FILTER_WIDTH / 6.0, FILTER_WIDTH / 6.0, cv::BORDER_CONSTANT);

	imwrite("output_ocv.png", ocv_ref);

	out_img.create(in_gray.rows, in_gray.cols, in_gray.depth()); // create memory for output image

#if NO
	xF::Mat<XF_8UC1, HEIGHT, WIDTH, XF_NPPC1> imgInput(in_gray.rows,in_gray.cols);
	xF::Mat<XF_8UC1, HEIGHT, WIDTH, XF_NPPC1> imgOutput(in_gray.rows,in_gray.cols);

	imgInput.copyTo(in_gray.data);
	#if __SDSCC__
	TIME_STAMP_INIT
	#endif
	xFGaussianBlur<FILTER, XF_BORDER_CONSTANT,XF_8UC1, HEIGHT, WIDTH, XF_NPPC1>(imgInput, imgOutput, sigma);
	#if __SDSCC__
	TIME_STAMP
	#endif
	out_img.data = imgOutput.copyFrom();


#endif
#if RO

	xF::Mat<XF_8UC1,HEIGHT,WIDTH,XF_NPPC8> imgInput(in_gray.rows,in_gray.cols);
	xF::Mat<XF_8UC1,HEIGHT,WIDTH,XF_NPPC8> imgOutput(in_gray.rows,in_gray.cols);

	imgInput.copyTo(in_gray.data);


	#if __SDSCC__
	TIME_STAMP_INIT
	#endif

	xFGaussianBlur<FILTER, XF_BORDER_CONSTANT, XF_8UC1, HEIGHT, WIDTH, XF_NPPC8>(imgInput, imgOutput, sigma);

	#if __SDSCC__
	TIME_STAMP
	#endif

	out_img.data = imgOutput.copyFrom();

#endif

	imwrite("output_hls.png", out_img);
	absdiff(ocv_ref, out_img, diff); // Compute absolute difference image
	imwrite("error.png", diff); // Save the difference image for debugging purpose

	// 	Find minimum and maximum differences.

	double minval = 256, maxval = 0;
	int cnt = 0;
	for (int i = 0; i < in_img.rows; i++) {
		for (int j = 0; j < in_img.cols; j++) {
			uchar v = diff.at<uchar>(i, j);
			if (v > 0)
				cnt++;
			if (minval > v)
				minval = v;
			if (maxval < v)
				maxval = v;
		}
	}
	float err_per = 100.0 * (float) cnt / (in_img.rows * in_img.cols);
	printf(
			"Minimum error in intensity = %f\n\
				Maximum error in intensity = %f\n\
				Percentage of pixels above error threshold = %f\n",
			minval, maxval, err_per);

	if(err_per > 1){
		printf("\nTest failed\n");
		return -1;
	}
	else{
		printf("\nTest Pass\n");
	return 0;
	}
}
