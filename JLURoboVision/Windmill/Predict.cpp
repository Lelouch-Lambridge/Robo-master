#include"Energy.h"

//----------------------------------------------------------------------------------------------------------------------
// �˺���������������ת������г�ʼ��
// ---------------------------------------------------------------------------------------------------------------------
void Energy::initRotation() {
    if (target_polar_angle >= -180 && last_target_polar_angle_judge_rotation >= -180
        && fabs(target_polar_angle - last_target_polar_angle_judge_rotation) < 30) {
        //target_polar_angle��last_target_polar_angle_judge_rotation�ĳ�ֵ��Ϊ1000������-180��ʾ�տ�ʼ��֡��Ҫ
        //�����߱ȽϽӽ�����˵��û���л�Ŀ�꣬��˿�������˳��ʱ����ж�
        if (target_polar_angle < last_target_polar_angle_judge_rotation) clockwise_rotation_init_cnt++;
        else if (target_polar_angle > last_target_polar_angle_judge_rotation) anticlockwise_rotation_init_cnt++;
    }
    //���ڸտ�ʼԲ���жϲ�׼���Ƕȱ仯���ܼ������������Ҫ�ڽǶ����������仯�㹻��ʱ�ſ�ȷ���Ƿ�Ϊ˳��ʱ��
    if (clockwise_rotation_init_cnt == 15) {
        energy_rotation_direction = 1;//˳ʱ��仯30�Σ�ȷ��Ϊ˳ʱ��
        cout << "rotation: " << energy_rotation_direction << endl;
        energy_rotation_init = false;
    }
    else if (anticlockwise_rotation_init_cnt == 15) {
        energy_rotation_direction = -1;//��ʱ��仯30�Σ�ȷ��Ϊ˳ʱ��
        cout << "rotation: " << energy_rotation_direction << endl;
        energy_rotation_init = false;
    }
    last_target_polar_angle_judge_rotation = target_polar_angle;
}

//----------------------------------------------------------------------------------------------------------------------
// �˺�����ȡԤ�������
// ---------------------------------------------------------------------------------------------------------------------
void Energy::getPredictPoint(cv::Point target_point) {
        if (energy_rotation_direction == 1) predict_rad = predict_rad_norm;
        else if (energy_rotation_direction == -1) predict_rad = -predict_rad_norm;
        rotate(target_point);
  
}

//----------------------------------------------------------------------------------------------------------------------
// �˺���������ʾԤ���
// ---------------------------------------------------------------------------------------------------------------------
void Energy::showPredictPoint(std::string windows_name, const cv::Mat& src) {
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
    cv::Point2f point = predict_point;
    cv::circle(image2show, point, 4, cv::Scalar(255, 0, 255));//��ͼ���л��������㣬2��Բ�İ뾶
    imshow(windows_name, image2show);
}
