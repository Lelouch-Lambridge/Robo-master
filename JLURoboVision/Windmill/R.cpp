#include"Energy.h"

//----------------------------------------------------------------------------------------------------------------------
// �˺�������Ѱ������R��������
// ---------------------------------------------------------------------------------------------------------------------
bool Energy::findCenterROI(const cv::Mat& src) {
	float length = target_armor.size.height > target_armor.size.width ?
		target_armor.size.height : target_armor.size.width;

	Point2f p2p(flow_strip.center.x - target_point.x,
		flow_strip.center.y - target_point.y);
	p2p = p2p / pointDistance(flow_strip.center, target_point);//��λ��
	center_ROI = cv::RotatedRect(cv::Point2f(flow_strip.center + p2p * length * 1.7),
		Size2f(length * 1.7, length * 1.7), -90);
	return true;

}

//----------------------------------------------------------------------------------------------------------------------
// �˺�������Ѱ��ͼ���ڴ�糵������ĸ��R��
// ---------------------------------------------------------------------------------------------------------------------
bool Energy::findCenterR(const cv::Mat& src) {
    if (src.empty()) {
        if (show_wrong) cout << "empty!" << endl;
        return false;
    }
    static Mat src_bin;
    src_bin = src.clone();
    if (src.type() == CV_8UC3) {
        cvtColor(src_bin, src_bin, CV_BGR2GRAY);
    }
    std::vector<vector<Point> > center_R_contours;
    CenterRDilate(src_bin);
    if (show_process)imshow("R struct", src_bin);
    findContours(src_bin, center_R_contours, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_NONE);
    for (auto& center_R_contour : center_R_contours) {
        if (!isValidCenterRContour(center_R_contour)) {
            continue;
        }
        centerR = cv::minAreaRect(center_R_contour);
        float target_length =
            target_armor.size.height > target_armor.size.width ? target_armor.size.height : target_armor.size.width;
        circle_center_point = centerR.center;
        circle_center_point.y += target_length / 7.5;//ʵ����С���˵õ���������R���·�
        return true;
    }
    if (show_wrong)cout << "find center R false!" << endl;
    return false;

}

//----------------------------------------------------------------------------------------------------------------------
// �˺��������ж��ҵ��ľ��κ�ѡ���Ƿ�Ϊ���ܵķ糵����R��ѡ��
// ---------------------------------------------------------------------------------------------------------------------
bool Energy::isValidCenterRContour(const vector<cv::Point>& center_R_contour) {
    double cur_contour_area = contourArea(center_R_contour);
    if (cur_contour_area > _flow.Center_R_Control_area_max ||
        cur_contour_area < _flow.Center_R_Control_area_min) {
        //cout<<cur_contour_area<<" "<<energy_fan_param_.CONTOUR_AREA_MIN<<" "<<energy_fan_param_.CONTOUR_AREA_MAX<<endl;
        //cout<<"area fail."<<endl;
        return false;
        //ѡ�������С������
    }
    RotatedRect cur_rect = minAreaRect(center_R_contour);
    Size2f cur_size = cur_rect.size;
    float length = cur_size.height > cur_size.width ? cur_size.height : cur_size.width;//�����εĳ�������Ϊ��
    float width = cur_size.height < cur_size.width ? cur_size.height : cur_size.width;//�����εĶ̱�����Ϊ��
    if (length < _flow.Center_R_Control_length_min || width < _flow.Center_R_Control_width_min
        || length > _flow.Center_R_Control_length_max || width > _flow.Center_R_Control_width_max) {
        //cout<<"length width fail."<<endl;
//        cout << "length: " << length << '\t' << "width: " << width << '\t' << cur_rect.center << endl;
        return false;
        //���α߳�������
    }
    float length_width_ratio = length / width;//������γ����
    if (length_width_ratio > _flow.Center_R_Control_radio_max ||
        length_width_ratio < _flow.Center_R_Control_radio_min) {
        //cout<<"length width ratio fail."<<endl;
//        cout << "HW: " << length_width_ratio << '\t' << cur_rect.center << endl;
        return false;
        //����Ȳ�����
    }
    if (cur_contour_area / cur_size.area() < _flow.Center_R_Control_area_radio_min) {
        //        cout << "area ratio: " << cur_contour_area / cur_size.area() << '\t' << cur_rect.center << endl;
        return false;//�����Ծ��ε����ռ���ʲ�����
    }
    std::vector<cv::Point2f> intersection;
    if (rotatedRectangleIntersection(cur_rect, center_ROI, intersection) == 0) {
        return false;
    }
    else if (contourArea(intersection) < _flow.Center_R_Control_area_intersection_area_min) {
        //        cout << "R intersection: " << contourArea(intersection) << '\t' << cur_rect.center << endl;
        return false;
    }
    return true;
}


//----------------------------------------------------------------------------------------------------------------------
// �˺���������ʾͼ�������п��ܵķ糵���ĺ�ѡ��R
// ---------------------------------------------------------------------------------------------------------------------
void Energy::showCenterR(std::string windows_name, const cv::Mat& src) {
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
    //cvtColor(image2show, image2show, COLOR_GRAY2RGB);
    Point2f vertices[4];      //������ε�4������
    centerR.points(vertices);   //������ε�4������
    for (int i = 0; i < 4; i++)
        line(image2show, vertices[i], vertices[(i + 1) % 4], Scalar(255, 255, 0), 2);

    cv::circle(image2show, circle_center_point, 4, cv::Scalar(0, 0, 255), 2);//��ͼ���л��������㣬2��Բ�İ뾶

    imshow(windows_name, image2show);
}