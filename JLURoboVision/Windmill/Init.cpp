#include"Energy.h"
extern McuData mcu_data;

//----------------------------------------------------------------------------------------------------------------------
// �˺���Ϊ���캯��
// ---------------------------------------------------------------------------------------------------------------------
Energy::Energy() {
	initEnergy();
	initEnergyPartParam();//���������ز������г�ʼ��
}
//----------------------------------------------------------------------------------------------------------------------
// �˺���Ϊ����������������������ΪĬ��
// ---------------------------------------------------------------------------------------------------------------------
Energy::~Energy() = default;

//----------------------------------------------------------------------------------------------------------------------
// �˺������������س�Ա�������г�ʼ��
// ---------------------------------------------------------------------------------------------------------------------
void Energy::initEnergy() {
	show_energy=true;//�Ƿ���ʾͼ��
	show_process= false;//�Ƿ���ʾ���Թ���
	show_wrong = false;//�Ƿ���ʾ����
	show_data = true;//�Ƿ���ʾ����

	fans_cnt = 0;//��Ҷ������ʼ��
    last_fans_cnt=0;//��һ֡����Ҷ����
	last_target_polar_angle_judge_rotation = -1000;//��һ֡������װ�װ�ļ�����Ƕȣ������ж�����
	clockwise_rotation_init_cnt = 0;//װ�װ�˳ʱ����ת����
	anticlockwise_rotation_init_cnt = 0;//װ�װ���ʱ����ת����
	energy_rotation_init = true;//�������жϷ糵��ת������Ϊtrue
	predict_rad = 0;//Ԥ����ǰ��
	predict_rad_norm = 25;// Ԥ����ǰ�ǵľ���ֵ
	predict_point = Point(0, 0);//Ԥ�������ʼ��
}

//----------------------------------------------------------------------------------------------------------------------
// �˺������������ز������г�ʼ��
// ---------------------------------------------------------------------------------------------------------------------
void Energy::initEnergyPartParam() {
	_flow.BLUE_GRAY_THRESH = 100;//�з���ɫʱ����ֵ
	_flow.RED_GRAY_THRESH = 180;//�з���ɫʱ����ֵ

	_flow.armor_contour_area_max = 500; //װ�װ�����ɸѡ����
	_flow.armor_contour_area_min = 180;
	_flow.armor_contour_length_max = 50;
	_flow.armor_contour_length_min = 10;
	_flow.armor_contour_width_max = 30;
	_flow.armor_contour_width_min = 0;
	_flow.armor_contour_hw_ratio_max = 3;
	_flow.armor_contour_hw_ratio_min = 1;

	_flow.flow_strip_fan_contour_area_max = 2000;//������������Ҷ�����ɸѡ����
	_flow.flow_strip_fan_contour_area_min = 500;
	_flow.flow_strip_fan_contour_length_max = 100;
	_flow.flow_strip_fan_contour_length_min = 60;
	_flow.flow_strip_fan_contour_width_max = 52;
	_flow.flow_strip_fan_contour_width_min = 20;
	_flow.flow_strip_fan_contour_hw_ratio_max = 2.8;
	_flow.flow_strip_fan_contour_hw_ratio_min = 1.2;
	_flow.flow_strip_fan_contour_area_ratio_max = 0.58;
	_flow.flow_strip_fan_contour_area_ratio_min = 0.34;

	_flow.Strip_Fan_Distance_max = 56;//��������װ�װ�������
	_flow.Strip_Fan_Distance_min = 28;

	_flow.flow_strip_contour_area_max = 700;//��������ز���ɸѡ
	_flow.flow_strip_contour_area_min = 50;
	_flow.flow_strip_contour_length_max = 55;
	_flow.flow_strip_contour_length_min = 40;//32
	_flow.flow_strip_contour_width_max = 20;
	_flow.flow_strip_contour_width_min = 4;
	_flow.flow_strip_contour_hw_ratio_min = 3;
	_flow.flow_strip_contour_hw_ratio_max = 7;
	_flow.flow_strip_contour_area_ratio_min = 0.6;
	_flow.flow_strip_contour_intersection_area_min = 100;

	_flow.target_intersection_contour_area_min = 40;//�غ����

	_flow.twin_point_max = 20;

	_flow.Center_R_Control_area_max = 200;//����R��ɸѡ��ز���
	_flow.Center_R_Control_area_min = 40;
	_flow.Center_R_Control_length_max = 20;
	_flow.Center_R_Control_length_min = 6;
	_flow.Center_R_Control_width_max = 20;
	_flow.Center_R_Control_width_min = 6;
	_flow.Center_R_Control_radio_max = 2;
	_flow.Center_R_Control_radio_min = 1;
	_flow.Center_R_Control_area_radio_min = 0.6;
	_flow.Center_R_Control_area_intersection_area_min = 10;

	_flow.flow_area_max = 5000;//��Ҷɸѡ��ز���
	_flow.flow_area_min = 1500;
	_flow.flow_length_max = 100;
	_flow.flow_length_min = 45;
	_flow.flow_width_max = 52;
	_flow.flow_width_min = 10;
	_flow.flow_aim_max = 3.5;
	_flow.flow_aim_min = 1.2;
	_flow.flow_area_ratio_min = 0.6;
}


//----------------------------------------------------------------------------------------------------------------------
// ��ո���vector����
// ---------------------------------------------------------------------------------------------------------------------
void Energy::clearAll() {
    fans.clear();
    armors.clear();
    flow_strip_fans.clear();
    target_armors.clear();
    flow_strips.clear();
}

//----------------------------------------------------------------------------------------------------------------------
// �˺�����ͼ�����Ԥ�������
// ---------------------------------------------------------------------------------------------------------------------
void Energy::initImage(cv::Mat& src) {
	if (src.type() == CV_8UC3) {
		cvtColor(src, src, COLOR_BGR2GRAY);
	}
	if (mcu_data.enemy_color == ENEMY_BLUE) {
		threshold(src, src, _flow.BLUE_GRAY_THRESH, 255, THRESH_BINARY);
	}
	else if (mcu_data.enemy_color == ENEMY_RED) {
		threshold(src, src, _flow.RED_GRAY_THRESH, 255, THRESH_BINARY);
	}
	
	if (show_process) imshow("bin", src);
	if (show_energy || show_process)waitKey(1);

}
