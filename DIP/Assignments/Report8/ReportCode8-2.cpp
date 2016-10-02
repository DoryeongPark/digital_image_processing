#include<opencv2/core/core.hpp>
#include<opencv2/highgui/highgui.hpp>
#include<opencv2/imgproc/imgproc.hpp>
#include<iostream>	
#include<string>

using namespace cv;
using namespace std;

int main() {

	Mat img1, img2; //Image 1 - Lenna.png, Image 2 - stuff_color.jpg

	Mat img_merged; //Variable to store image merged temporary

	vector<Mat> edges; //Storage for result images from ReportCode8-1.cpp [8 Images]
	vector<Mat> img1_channels; //Storage for Red, Green, Blue channels splited from image1
	vector<Mat> img2_channels; //Storage for Red, Green, Blue channels splited from image2

	vector<Mat> results; //Storage for results
	results.resize(8);

	//Loads 2 Images
	img1 = imread("Lenna.png", CV_LOAD_IMAGE_COLOR);
	img2 = imread("stuff_color.jpg", CV_LOAD_IMAGE_COLOR);
	resize(img2, img2, Size(img2.cols / 2, img2.rows / 2), 0, 0, CV_INTER_NN);

	if (img1.empty() || img2.empty()) {//If images are failed to be loaded...
		cout << "Can't load image" << endl;
		exit(0);
	}

	string filename = "Image _E .jpg"; //File name to be parsed

	//Loads result images(Each edges) created from ReportCode1.cpp
	for (int i = 0; i < 2; ++i) {
		for (int j = 0; j < 4; ++j) {

			//Converts integer to string(i - image, j - edges)
			string changed1 = string(to_string(i + 1));
			string changed2 = string(to_string(j + 1));

			//Appends numbers on file name
			filename.replace(5, 1, changed1);
			filename.replace(8, 1, changed2);
			
			//Load image
			Mat img_loaded = imread(filename, CV_LOAD_IMAGE_GRAYSCALE);

			//If it is failed to be loaded...
			if (img_loaded.empty())
				cout << img_loaded << " <- Failed to be loaded" << endl;

			edges.emplace_back(img_loaded);//Stores loaded image in stroage edges
		}
	}

	//-------------------Image1 - R'GB Image-------------------
	
	split(img1, img1_channels);//Splits image 1
	
	//Red channel + Sobel edge image1 = img_merged
	add(edges[0], img1_channels[2], img_merged);
	img1_channels.pop_back();
	img1_channels.emplace_back(img_merged);//Replaces Red channel with img_merged

	merge(img1_channels, results[0]);//Merges RGB channels and stores it

	//-------------------Image2 - R'GB Image-------------------

	split(img2, img2_channels);//Splits image 2

	//Red channel + Sobel edge image2 = img_merged
	add(edges[4], img2_channels[2], img_merged);
	img2_channels.pop_back();
	img2_channels.emplace_back(img_merged);//Replaces Red channel with img_merged

	merge(img2_channels, results[4]);//Merges RGB channels

	//-------------------Image1 - RG'B Image-------------------

	split(img1, img1_channels);//Splits image 1

	//Green channel + Laplacian edge image 1 = img_merged
	add(edges[1], img1_channels[1], img_merged);
	img1_channels[1] = img_merged;//Replaces Green channel with img_merged

	merge(img1_channels, results[1]);//Merges RGB channels and stores it

	//-------------------Image2 - RG'B Image-------------------

	split(img2, img2_channels);//Splits image 2

	//Green channel + Laplacian edge image 2 = img_merged
	add(edges[5], img2_channels[2], img_merged);
	img2_channels[1] = img_merged;//Replaces Green channel with img_merged

	merge(img2_channels, results[5]);//Merges RGB channels and stores it

	//-------------------Image1 - RGB' Image-------------------

	split(img1, img1_channels);//Splits image 1

	//Blue channel + Canny edge image 1 = img_merged
	add(edges[2], img1_channels[0], img_merged);
	img1_channels[0] = img_merged;//Replaces Blue channel with img_merged

	merge(img1_channels, results[2]);//Merges RGB channels and stores it

	//-------------------Image2 - RGB' Image-------------------

	split(img2, img2_channels);//Splits image 2

	//Blue channel + Canny edge image 2 = img_merged
	add(edges[6], img2_channels[0], img_merged);
	img2_channels[0] = img_merged;//Replaces Blue channel with img_merged

	merge(img2_channels, results[6]);//Merges RGB channels and stores it
	
	//-------------------Image1 - R''G''B Image-------------------

	split(img1, img1_channels);//Splits image1
	
	//Red channel + Laplacian of Gaussian edge image 1 = img_merged
	add(edges[3], img1_channels[2], img_merged);
	img1_channels[2] = img_merged;//Replaces Red channel with img_merged
	
	//Green channel + Laplacian of Gaussian edge image 2 = img_merged
	add(edges[3], img1_channels[1], img_merged);
	img1_channels[1] = img_merged;//Replaces Green channel with img_merged
	
	merge(img1_channels, results[3]);//Merges RGB channels and stores it
	

	//-------------------Image2 - R''G''B Image-------------------

	split(img2, img2_channels);//Splits image2
	
	//Red Channel = Laplacian of Gaussian edge image 1 = img_merged
	add(edges[7], img2_channels[2], img_merged);
	img2_channels[2] = img_merged;//Replaces Red channel with img_merged

	//Green channel + Laplacian of Gaussian edge image 2 = img_merged
	add(edges[7], img2_channels[1], img_merged);
	img2_channels[1] = img_merged;//Replaces Green channel with img_merged 
	
	merge(img2_channels, results[7]);//Merges RGB channels and stores it
	
	//Image names
	vector<string> img_names = { "img1_r+edge.jpg", "img1_g+edge.jpg", "img1_b+edge.jpg", "img1_rg+edge.jpg",
								 "img2_r+edge.jpg", "img2_g+edge.jpg", "img2_b+edge.jpg", "img2_rg+edge.jpg" };
	auto i = 0;
	
	//Shows all images in storage
	for (auto& s : img_names) 
		imshow(s, results[i++]);
	
	waitKey(0);
	
	i = 0;
	//Saves all images in storage
	for (auto& s : img_names) 
		imwrite(s, results[i++]);
	
	destroyAllWindows();
}