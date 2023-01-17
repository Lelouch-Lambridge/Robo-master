#include"Energy.h"

//----------------------------------------------------------------------------------------------------------------------
// �˺�������Ѱ��ͼ�������еĴ�糵��Ҷ
// ---------------------------------------------------------------------------------------------------------------------
int Energy::findFans(const cv::Mat& src) {
    if (src.empty()) {
        if (show_wrong) cout << "empty!" << endl;
        return 0;
    }
    static Mat src_bin;
    src_bin = src.clone();
    if (src.type() == CV_8UC3) {
        cvtColor(src_bin, src_bin, CV_BGR2GRAY);//����ȡ��ͨ����Ƶ�ļ�����ת��Ϊ��ͨ��
    }
    std::vector<vector<Point> > fan_contours;
    FanDilate(src_bin);//ͼ�����ͣ���ֹͼ��Ͽ���������Ѱ��
    if (show_process)imshow("fan struct", src_bin);
    findContours(src_bin, fan_contours, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_NONE);

    for (auto& fan_contour : fan_contours) {
        if (!isValidFanContour(src_bin, fan_contour)) {
            continue;
        }
        fans.emplace_back(cv::minAreaRect(fan_contour));
    }
    if (fans.size() < last_fans_cnt) {
        last_fans_cnt = static_cast<int>(fans.size());
        return -1;//Ѱ�ҵ�����Ҷ����һ֡�٣�˵����֡���󣬷���-1
    }
    last_fans_cnt = static_cast<int>(fans.size());
    return static_cast<int>(fans.size());
}

//----------------------------------------------------------------------------------------------------------------------
// �˺��������ж��ҵ��ľ��κ�ѡ���Ƿ�Ϊ��Ҷ
// ---------------------------------------------------------------------------------------------------------------------
bool Energy::isValidFanContour(cv::Mat& src, const vector<cv::Point>& fan_contour) {
    double cur_contour_area = contourArea(fan_contour);
    if (cur_contour_area > _flow.flow_area_max ||
        cur_contour_area < _flow.flow_area_min) {
        //cout<<cur_contour_area<<" "<<energy_fan_param_.CONTOUR_AREA_MIN<<" "<<energy_fan_param_.CONTOUR_AREA_MAX<<endl;
        //cout<<"area fail."<<endl;
        return false;
        //ѡ�������С������
    }
    RotatedRect cur_rect = minAreaRect(fan_contour);
    Size2f cur_size = cur_rect.size;
    float length = cur_size.height > cur_size.width ? cur_size.height : cur_size.width;//�����εĳ�������Ϊ��
    float width = cur_size.height < cur_size.width ? cur_size.height : cur_size.width;//�����εĶ̱�����Ϊ��
    if (length < _flow.flow_length_min || width <_flow.flow_width_min ||
        length > _flow.flow_length_max || width > width > _flow.flow_width_max) {
        //cout<<"length width fail."<<endl;
//        cout << "length: " << length << '\t' << "width: " << width << '\t' << cur_rect.center << endl;
        return false;
        //���α߳�������
    }
    float length_width_ratio = length / width;//������γ����
    if (length_width_ratio > _flow.flow_aim_max ||
        length_width_ratio < _flow.flow_aim_min) {
        //cout<<"length width ratio fail."<<endl;
//        cout << "HW: " << length_width_ratio << '\t' << cur_rect.center << endl;
        return false;
        //����Ȳ�����
    }
    //    cout << cur_contour_area / cur_size.area() << endl;
    if (cur_contour_area / cur_size.area() < _flow.flow_area_ratio_min) {
        //        cout << cur_contour_area / cur_size.area() << endl;
        //        cout << "area ratio: " << cur_contour_area / cur_size.area() << '\t' << cur_rect.center << endl;
        return false;//�����Ծ��ε����ռ���ʲ�����
    }
    return true;
}

//----------------------------------------------------------------------------------------------------------------------
// �˺���������ʾͼ����������Ҷ
// ---------------------------------------------------------------------------------------------------------------------
void Energy::showFans(std::string windows_name, const cv::Mat& src) {
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
    for (const auto& fan : fans) {
        Point2f vertices[4];      //������ε�4������
        fan.points(vertices);   //������ε�4������
        for (int i = 0; i < 4; i++)
            line(image2show, vertices[i], vertices[(i + 1) % 4], Scalar(255, 0, 0), 2);
    }
    imshow(windows_name, image2show);
}
