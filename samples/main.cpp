/**
 * @file mian.cpp
 * @date 2022/5/10
 * @author L.S.
 * @brief Image Format Transform
*/

#include <opencv2/opencv.hpp>
#include <dirent.h>
#include <iostream>
#include <string>
#include <vector>
#include <fstream>

using namespace std;
using namespace cv;

void getFilename(const string& root_folder, vector<string>& filename);
bool LoadYuvImageFromFile(vector<uint8_t>& dst, const int image_height,
						  const int image_width, const std::string& image_path);

int main()
{
	const string input_image_folder = "../data/image";
	const string output_image_folder = "../out/image";
	const int image_height = 240;
	const int image_width = 480;

	vector<string> filenames;
	getFilename(input_image_folder, filenames);

	for (const auto& image_name : filenames)
	{
		vector<uint8_t> raw_image;
		LoadYuvImageFromFile(raw_image, image_height, image_width, image_name);

		cv::Mat image_yuv = cv::Mat(image_height * 1.5, image_width, CV_8UC1, raw_image.data());
		cv::Mat image_jpg = cv::Mat(image_height, image_width, CV_8UC3);
		cv::cvtColor(image_yuv, image_jpg, COLOR_YUV2BGR_NV21);
		std::vector<int> param = std::vector<int>(2);
		param[0] = IMWRITE_JPEG_QUALITY;
		param[1] = 100; // default(95) 0-100

		std::stringstream namestream(image_name);
		std::string segment;
		std::string image_jpg_name;
		while(std::getline(namestream, segment, '/'))
		{
			if (strstr(segment.c_str(), ".yuv"))
			{
				segment.replace(segment.end()-3, segment.end(), "jpg");
				image_jpg_name = segment;
			}
		}

		cv::imwrite(output_image_folder + "/" + image_jpg_name, image_jpg, param);
// 		printf("save jpg: %s\n", (output_image_folder + "/" + image_jpg_name).c_str());
	}

	return 0;
}

/**
 * @brief get filename from root_folder
 * @param root_folder
 * @param filename
 */
void getFilename(const string& root_folder, vector<string>& filename)
{
	DIR* pDir;          //指向根目录结构体的指针
	struct dirent* ptr; //dirent结构体指针，具体结构看开头的注释
	// 使用dirent.h下的opendir()打开根目录，并返回指针
	if (!(pDir = opendir(root_folder.c_str())))
	{
		return;
	}
	// 使用dirent.h下的readdir逐个读取root下的文件
	while ((ptr = readdir(pDir)) != nullptr)
	{
		// 这里我理解他的指针应该是自动会指向到下一个文件，所以不用写指针的移动
		string sub_file = root_folder + "/" + ptr->d_name; // 当前指针指向的文件名
		if (ptr->d_type != 8 && ptr->d_type != 4)
		{ // 递归出口，当不是普通文件（8）和文件夹（4）时退出递归
			return;
		}
		// 普通文件直接加入到files
		if (ptr->d_type == 8)
		{
			// 相当于将命令下使用ls展示出来的文件中除了. 和 ..全部保存在files中
			// 当然下面可以写各种字符串的筛选逻辑，比如只要后缀有.jpg图片
			if (strcmp(ptr->d_name, ".") != 0 && strcmp(ptr->d_name, "..") != 0)
			{
				if (strstr(ptr->d_name, ".yuv"))
				{
					filename.push_back(sub_file);
				}
			} // 当前文件为文件夹(4)类型，那就以当前文件为root进行递归吧！
		}
		else if (ptr->d_type == 4)
		{
			if (strcmp(ptr->d_name, ".") != 0 && strcmp(ptr->d_name, "..") != 0)
			{
				getFilename(sub_file, filename);
			}
		}
	}
	// 关闭根目录
	closedir(pDir);
}

bool LoadYuvImageFromFile(vector<uint8_t>& dst, const int image_height,
									  const int image_width, const std::string& image_path)
{
	ifstream image_file;
	image_file.open(image_path, ios::in | ios::binary);
	if (image_file.fail())
	{
		printf("ERROR: open %s fail!\n", image_path.c_str());
		return false;
	}
	dst.resize(image_height * 1.5 * image_width);
	image_file.read(reinterpret_cast<char*>(dst.data()), dst.size());
	// 	copy(image_file.beg,  image_file.end, dst.data());

	image_file.close();
	return true;
}