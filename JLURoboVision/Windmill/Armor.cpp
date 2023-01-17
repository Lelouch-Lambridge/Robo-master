#include"Energy.h"


//----------------------------------------------------------------------------------------------------------------------
// �˺�������Ѱ��ͼ�������еĴ�糵װ�װ�ģ��
// ---------------------------------------------------------------------------------------------------------------------
int Energy::findArmors(const cv::Mat& src) {
	if (src.empty()) {
        if (show_wrong) cout << "empty!" << endl;
        return 0;
    }
	static Mat dst;
	dst = src.clone();
	if (src.type() == CV_8UC3) {
		cvtColor(dst, dst, CV_BGR2GRAY);//����ȡ��ͨ����Ƶ�ļ�����ת��Ϊ��ͨ��
	}
	std::vector<vector<Point> > armor_contours;
	std::vector<vector<Point> > armor_contours_external;//����������ȥ��������ֻ��������������ȥ��������Ӱ�졣

	ArmorDilate(dst);//ͼ�����ͣ���ֹͼ��Ͽ���������Ѱ��
	if (show_process) imshow("Armorstuct", dst);
	findContours(dst, armor_contours, CV_RETR_LIST, CV_CHAIN_APPROX_NONE);
	findContours(dst, armor_contours_external, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_NONE);
	for (int i = 0; i < armor_contours_external.size(); i++)//ȥ��������
	{
		int external_contour_size = armor_contours_external[i].size();
		for (int j = 0; j < armor_contours.size(); j++) {
			int all_size = armor_contours[j].size();
			if (external_contour_size == all_size) {
				swap(armor_contours[j], armor_contours[armor_contours.size() - 1]);
				armor_contours.pop_back();//�����������
				break;
			}
		}

	}
	for (auto armor_contour : armor_contours) {
		if (!isValidArmorContour(armor_contour)) {
			continue;
		}
		armors.emplace_back(cv::minAreaRect(armor_contour));//�ش�����װ�װ嵽armors������
	}
	return static_cast<int>(armors.size());
}


//----------------------------------------------------------------------------------------------------------------------
// �˺��������ж��ҵ���װ�װ�ߴ��Ƿ�ϸ�
// ---------------------------------------------------------------------------------------------------------------------
bool Energy::isValidArmorContour(const vector<cv::Point>& armor_contour) {
	double cur_contour_area = contourArea(armor_contour);
	if (cur_contour_area > _flow.armor_contour_area_max ||
		cur_contour_area < _flow.armor_contour_area_min) {
		return false;
	}
	RotatedRect cur_rect = minAreaRect(armor_contour);
	Size2f cur_size = cur_rect.size;
	float length = cur_size.height > cur_size.width ? cur_size.height : cur_size.width;
	float width = cur_size.height < cur_size.width ? cur_size.height : cur_size.width;
	if (length < _flow.armor_contour_length_min || width < _flow.armor_contour_width_min ||
		length >  _flow.armor_contour_length_max || width > _flow.armor_contour_width_max) {
		return false;
	}
	float length_width_ratio = length / width;
	if (length_width_ratio > _flow.armor_contour_hw_ratio_max ||
		length_width_ratio < _flow.armor_contour_hw_ratio_min) {
		return false;
	}
	return true;
}

//----------------------------------------------------------------------------------------------------------------------
// �˺���������ʾͼ��������װ�װ�
// ---------------------------------------------------------------------------------------------------------------------
void Energy::showArmors(std::string windows_name, const cv::Mat& src) {
	if (src.empty())return;
	static Mat image2show;

	if (src.type() == CV_8UC1) // �ڰ�ͼ��
	{
		cvtColor(src, image2show, COLOR_GRAY2RGB);

	}
	else if (src.type() == CV_8UC3) //RGB ��ɫ
	{
		image2show = src.clone();
	}
	for (const auto& armor : armors) {
		Point2f vertices[4];      //������ε�4������
		armor.points(vertices);   //������ε�4������
		for (int i = 0; i < 4; i++)
			line(image2show, vertices[i], vertices[(i + 1) % 4], Scalar(0, 0, 255), 2);
	}
	imshow(windows_name, image2show);
}