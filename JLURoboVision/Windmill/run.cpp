#include"Energy.h"

using namespace std;
using namespace cv;

void Energy::run(cv::Mat& src) {
	clearAll();
	initImage(src);
	if (show_process)imshow("bin", src);
	if (findArmors(src) < 1)return;//Ѱ�����д�糵װ�װ�
	if (show_energy) showArmors("armor", src);
	if (!findFlowStripFan(src)) {//Ѱ��ͼ�����������ڵ���Ҷ
		if (!findFlowStripWeak(src)) return;
	}
	else {
		if (show_energy)showFlowStripFan("strip fan", src);
		if (!findTargetInFlowStripFan()) return;//��������������Ѱ��װ�װ�
		if (!findFlowStrip(src)) return;//Ѱ��������
	}
	if (show_energy) showFlowStrip("strip", src);
	findCenterROI(src);//Ѱ��R�귶Χ(��СR���ⷶΧ������߼���ٶȣ�������ʶ���ʣ�
	if (!findCenterR(src)) return;//Ѱ������R
	if (show_energy) showCenterR("R", src);
	fans_cnt = findFans(src);//Ѱ��ͼ����������Ҷ
	if (show_energy) showFans("fans", src);

	getTargetPolarAngle();//�������װ�װ�����ת��Ϊ������
	if (energy_rotation_init) {
		initRotation();
		return;
	}
	getPredictPoint(target_point);
	if (show_energy) showPredictPoint("Predict", src);
	if (show_data) showData(target_point,predict_point,circle_center_point);
}



int main() {
	Energy energy;
	Mat src;
	VideoCapture cap("H:/Photo/red_big.avi");
	cap.set(CAP_PROP_FRAME_WIDTH, 640);			//��������������
	cap.set(CAP_PROP_FRAME_HEIGHT, 480);		//������������߶�
	cap.set(CAP_PROP_BRIGHTNESS, 20);			//���������������
	cap.set(CV_CAP_PROP_FPS, 200);				//�����������֡��
	do {
		//ʱ�����Ĳ���
		double t, tc;
		t = getTickCount();
		cap >> src;
		resize(src, src, Size(640, 480));
		energy.run(src);
		//ʱ�����Ĳ���
		tc = (getTickCount() - t) / getTickFrequency();
		printf("time consume %.5f\n", tc);    //��ʾ���ķ�ʱ��Ķ���
		waitKey(1);
	} while (true);
}