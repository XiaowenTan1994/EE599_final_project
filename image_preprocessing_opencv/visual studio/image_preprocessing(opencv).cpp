#include "opencv2/imgproc.hpp"
#include "opencv2/Core.hpp"
#include "opencv2/highgui.hpp"
#include <iostream>
#include <cstdlib>
#include <vector>
#include <cstdlib>
#include <deque>
#include <queue>
#include <iterator>

using namespace std;
using namespace cv;

void imageRotate(Mat pic, Mat & rotated, double degree)
{
	//define the rotate center with a 2d data structure
	Point2f center;
	center.x = float(pic.cols / 2.);
	center.y = float(pic.rows / 2.);
	//C++: Mat getRotationMatrix2D(Point2f center, double angle, double scale)
	Mat rotateMatrix = getRotationMatrix2D(center, degree, 1);
	//warpAffine(pic, rotated1, rotateMatrix,Size(pic.cols, pic.rows) );
	//fit the size of the rotated pic
	int width_rotate = int(pic.rows * fabs(sin(degree)) + pic.cols * fabs(cos(degree)));
	int height_rotate = int(pic.cols * fabs(sin(degree)) + pic.rows * fabs(cos(degree)));
	warpAffine(pic, rotated, rotateMatrix, Size(width_rotate, height_rotate), 1, 0, Scalar(255, 255, 255));
	imshow("test", rotated);
}

double findDegree(const Mat pic)
{
	//detecting rotated angle
	//doing the edge detection preprocessing
	Mat dst, cdst;
	Canny(pic, dst, 50, 200, 3);
	cvtColor(dst, cdst, COLOR_GRAY2BGR);
	//imshow("cdst", cdst);
	//a set of lines with two numbers to store the polar coordinate system para
	float sum = 0;
	vector<Vec2f> lines;
	//the fifth para may vary based on diff fig
	//try several times with different threshold
	int threshold = 300;
	//HoughLines(dst, lines, 1, CV_PI / 180, 50, 0, 0);
	while (threshold > 0 && lines.size()< pic.rows/90)
	{
		//break;
		HoughLines(dst, lines, 1, CV_PI / 180, threshold, 0, 0);
		threshold -= 50;
	}
	
	queue<float> degrees;
	int ttt = 0;
	for (int i = 0; i < lines.size(); i++)
	{
		//plot the detected lines
		//translate the polar coordinate to cartesian coordinate
		float rho = lines[i][0], theta = lines[i][1];
		degrees.push(theta);
		//cout << i<<":"<<theta << endl;
		sum += theta;
		Point pt1, pt2;
		double a = cos(theta), b = sin(theta);
		double x0 = a*rho, y0 = b*rho;
		pt1.x = cvRound(x0 + 1000 * (-b));
		pt1.y = cvRound(y0 + 1000 * (a));
		pt2.x = cvRound(x0 - 1000 * (-b));
		pt2.y = cvRound(y0 - 1000 * (a));
		line(cdst, pt1, pt2, Scalar(0, 0, 255), 1, LINE_AA);
	}
	imshow("detected lines", cdst);
	
	//take the average value of degree for rotating
	//cout << "ttt:" << ttt << endl;
	//cout << "sum:" << sum << endl;
	//cout << "average" << sum / lines.size() << endl;

	//wipe out the extreme value
	//can be modify to the range of degree that occurs the most
	//use average value to wipe out the extreme value is not a best choice
	float average = sum / lines.size();
	for (int i = 0; i < lines.size(); i++)
	{
		float h = degrees.front();
		degrees.pop();
		if (h - average >= 1.)
		{
			sum -= h;
			ttt++;
		}
	}
	double degree = double(sum / (lines.size()-ttt)) * 180 / CV_PI - 90;
	//cout << degree << endl;
	return degree;
}


//doing horizontal projection
void h_projection(vector<int> & count, Mat pic)
{
	//actually counting pixls, between lines and works there are space
	
	for (int i = 0; i < pic.rows; i++)
	{
		for (int j = 0; j < pic.cols; j++)
		{
			if (pic.ptr<uchar>(i)[j] <= 50)
			{
				//add to account when the piexl is all black
				count[i]++;
			}
		}
	}

}

//draw horizontal projection
void draw_h_projection(vector<int> & count,Mat pic)
{
	Mat projection = Mat(pic.rows, pic.cols, CV_8UC1);
	projection = Scalar(255);
	for (int i = 0; i < projection.rows; i++)
	{
		for (int j = 0; j < count[i]; j++)
		{
			projection.ptr<uchar>(i)[j] = 0;
		}
	}

	imshow("h_p", projection);
}


//doing vertical projection
void v_projection(vector<int> & count, Mat pic)
{
	//actually counting pixls, between lines and works there are space

	//for (int i = 0; i < pic.cols; i++)
	//{
	//	for (int j = 0; j < pic.rows; j++)
	//	{
	//		if (pic.ptr<uchar>(j)[i] <= 50)
	//		{
	//			//add to account when the piexl is all black
	//			count[i]++;
	//		}
	//	}
	//}
	//int cut=0;
	for (int i = 0; i < pic.cols; i++)
	{
		for (int j = 0; j < pic.rows; j++)
		{
			int t = pic.ptr<uchar>(j)[i];
			if (t<=50)
			{
				//cut = i;
				//cout <<"start:"<< i<<endl;
				//add to account when the piexl is all black
				count[i]++;
			}
			
		}
	
	}

	//cout << int(pic.ptr<uchar>(16)[84]) << endl;
	//imwrite("ori.png", pic);
	//Mat show = pic(Rect(0, 0, 84, pic.rows)).clone();
	//imwrite("show.png", show);
}

void draw_v_projection(vector<int> & count, Mat pic, int num)
{
	Mat projection = Mat(pic.rows, pic.cols, CV_8UC1);
	projection = Scalar(255);
	for (int i = 0; i <count.size(); i++)
	{
		for (int j = pic.rows-1; j >= pic.rows-count[i]; j--)
		{
			projection.ptr<uchar>(j)[i] = 0;
		}
	}

	string name = "v_p" + to_string(num);
	imshow(name, projection);
}


//cut lines
void cut_lines(vector<Mat> & line_pic, vector<int> count,const Mat pic)
{
	int threshold=5;
	int start = 0;
	int end = 0;
	for (int i = 0; i < pic.rows; i++)
	{
		if (count[i] > 2 && start == 0)
		{
			start = i;
		}
		else if (count[i] <=2  && start>0)
		{
			end = i;
			Mat temp = pic(Rect(0, start-threshold, pic.cols, end - start+2*threshold)).clone();
			//imshow("cut", temp);
			line_pic.push_back(temp);
			start = 0;
			end = 0;

		}
	}
}


//print lines
void print_lines(const vector<Mat> line_pic)
{
	for (int i = 0; i < line_pic.size(); i++)
	{
		string name = "line" + to_string(i);
		// << name << endl;
		imshow(name, line_pic[i]);

	}
}


int main()
{
	string picname = "13.png";
	Mat rotated = imread(picname);
	//cout << int(rotated.ptr<uchar>(32)[84]) << endl;
	//imshow("test",pic);
	//rotate image in a certain degree
	Mat pic;
	double degree = findDegree(rotated);
	imageRotate(rotated, pic, degree);

	//locate position of lines
	//draw horizontal projection of text
	vector<int> count(pic.rows, 0);
	h_projection(count, pic);

	//draw the horizontal projection
	draw_h_projection(count, pic);

	//cut pic to lines
	vector<Mat> line_pic;
	cut_lines(line_pic, count, pic);
	
	//print lines
	print_lines(line_pic);
	
	queue<Mat> word_pic;
	for (int i = 0; i < line_pic.size(); i++)
	{
		Mat trys;
		cvtColor(line_pic[i], trys, COLOR_BGR2GRAY);
		//for each line,count its vertical projection
		vector<int> word_count(line_pic[i].cols, 0);
		v_projection(word_count,trys);
		draw_v_projection(word_count, trys,i);
	}

	//Mat ori = imread("ori.png", 0);
	//int a= ori.ptr<uchar>(19)[84] ;
	//cout << a << endl;
	//Canny(ori, cn, 50, 200, 3);
	//imshow("cn", cn);
	//vector<int> ccc(ori.cols, 0);
	//v_projection(ccc, ori);

	////vector<int> vertical_pos(aaa.cols, 0);
	//////vector<char_range_t> v_peek_range;
	////for (int i = 0; i < aaa.rows; i++)
	////{
	////	uchar* p = aaa.ptr<uchar>(i);
	////	for (int j = 0; j < aaa.cols; j++)
	////	{
	////		if (p[j] ==0)
	////		{
	////			vertical_pos[j]++;
	////		}
	////	}
	////}

	////vector<int>::iterator max = std::max_element(std::begin(vertical_pos), std::end(vertical_pos));
	////int height = *max;
	////int width = vertical_pos.size();
	////Mat project = Mat::zeros(height, width, CV_8UC1);
	////for (int i = 0; i < project.cols; i++)
	////{
	////	for (int j = project.rows - 1; j >= project.rows - vertical_pos[i]; j--)
	////	{
	////		//std::cout << "j:" << j << "i:" << i << std::endl;
	////		project.at<uchar>(j, i) = 255;
	////	}
	////}

	////imshow("vertical projection", project);

	////for each line do vertical projection
	//vector<vector<int>> count_word(line_pic.size(), vector<int>());
	//int h = 0;
	//iter=line_pic.begin();
	//vector<int> count_temp(aaa.cols, 0);
	//for (int i = 0; i < aaa.cols; i++)
	//{
	//	for (int j = 0; j < aaa.rows; j++)
	//	{
	//		if (aaa.ptr<uchar>(j)[i] <= 100)
	//		{
	//			count_temp[i]++;
	//		}
	//	}
	//}

	//for (int i = 0; i < aaa.rows; i++)
	//{
	//	uchar* p = aaa.ptr<uchar>(i);
	//	for (int j = 0; j < aaa.cols; j++)
	//	{
	//		if (p[j] <= 100)  //ÊÇºÚÉ«ÏñËØ
	//		{
	//			count_temp[j]++;
	//		}
	//	}
	//}

	//while (iter != line_pic.end())
	//{
		//Mat temp =line_pic.front();
		//vector<int> count_temp(temp.cols, 0);
		//for (int i = 0; i < temp.cols; i++)
		//{
		//	for (int j = 0; j < temp.rows; j++)
		//	{
		//		if (temp.ptr<uchar>(i)[j]<= 50)
		//		{
		//			cout << "in" << endl;
		//			count_temp[i]++;
		//		}
		//	}
		//}
		//count_word[h] = count_temp;
		//iter++;
		//h++;
		//break;	
		
	//}


	//draw the vertical projection
	//imwrite("test.jpg", aaa);
	//Mat v_projection = Mat(aaa.rows, aaa.cols, CV_8UC1);
	//v_projection = Scalar(255);
	////imshow("fff", v_projection);
	//for (int i = 0; i < aaa.cols; i++)
	//{
	//	for (int j = aaa.rows-1; j >= aaa.rows-count_temp[i]; j--)
	//	{
	//		v_projection.ptr<uchar>(j)[i] = 0;
	//	}
	//}

	//imshow("v_p", v_projection);

	//int height = aaa.rows;
	//int width = aaa.cols;
	//Mat project = Mat::zeros(height, width, CV_8UC1);
	//cout << project.cols<<endl;
	//for (int i = 0; i < project.cols; i++)
	//{
	//	cout << i << " ";
	//	for (int j = project.rows - 1; j >= project.rows-1-count_temp[i]; j--)
	//	{
	//		//std::cout << "j:" << j << "i:" << i << std::endl;
	//		project.at<uchar>(j, i) = 255;
	//	}
	//}
	//imshow("vertical projection", project);

	//for (int i = 0; i < pic.cols; i++)
	//{
	//	for (int j = 0; j < pic.rows; j++)
	//	{
	//		if (pic.ptr<uchar>(i)[j] == 0)
	//		{
	//			//add to account when the piexl is all black
	//			count[i]++;
	//		}
	//	}
	//}

	//imshow("test2", rotated);


	waitKey();
	return 0;

}