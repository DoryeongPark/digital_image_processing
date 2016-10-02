#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

#include <iostream>
#include <string>
#include <vector>

using namespace std;

struct point {
	char c = 0;
	int x = 0;
	int y = 0;

	float angle = 0;

	bool used = false;

	point() = default;

	point(const int x, const int y) {
		this->x = x;
		this->y = y;
	}

	point(const char c, const int x, const int y) {
		this->c = c;
		this->x = x;
		this->y = y;
	}
};

struct line {
	point p1;
	point p2;

	line(point p1, point p2) {
		this->p1 = p1;
		this->p2 = p2;
	}
};


class Polygon {
private:
	vector<point> points;
	vector<float> angles;
	vector<line> lines;

	int size;
public:
	Polygon(const int _size) {

		size = _size;

		char c;
		int x, y;

		for (int i = 0; i < size; ++i) {
			cout << "점 입력 -> ";
			cin >> c >> x >> y;
			points.emplace_back(point{ c, x, y });
		}

		point lowest = points[0];
		int lowest_index = 0;

		for (int i = 0; i < points.size(); ++i) {
			if (lowest.y > points[i].y) {
				lowest = points[i];
				lowest_index = i;
			}
		}

		point temp = points[0];
		points[0] = lowest;
		points[lowest_index] = temp;

		angles.emplace_back(0);

		for (int i = 1; i < points.size(); ++i) {
			points[i].angle = ComputeAngle(points[0], points[i]);
			angles.emplace_back(points[i].angle);
		}

		heap_sort();

		auto new_points = points;

		for (int i = 1; i < angles.size(); ++i) {
			for (int j = 1; j < points.size(); ++j) {
				if (angles[i] == points[j].angle && points[j].used == false) {
					new_points[i] = points[j];
					points[j].used = true;
					break;
				}
			}
		}

		points = new_points;

		for (int i = 0; i < points.size(); ++i) {
			if (i == points.size() - 1)
				lines.emplace_back(line{ points[i], points[0] });
			else
				lines.emplace_back(line{ points[i], points[i + 1] });
		}
	}

	void print_result() {

		cout << endl;

		cout << "수평각 계산 횟수 : " << angle_compare << endl;
		cout << "각의 비교 횟수 : " << compare << endl;

		cout << "순서 : " << endl;

		for (auto a : points)
			cout << a.c << "[" << a.x << ", " << a.y << "] -> ";

		cout << points[0].c << "[" << points[0].x << ", " << points[0].y << "]" << endl;

		cout << endl;
	}

	bool is_inside(point p) {

		auto count = 0;
		auto checkline = line{ p, point{ 1000, p.y } };

		int frequency = 0;

		for (auto& a : lines) 
			if (is_intersection(a, checkline)) 
				++count;
		

		if (is_intersection(lines[0], checkline) && is_intersection(lines.back(), checkline))
			--count;

		if (count % 2 == 0)
			return false;
		else
			return true;
	}

	void show_polygon(const point p) {

		auto max_point = 0;

		for (int i = 0; i < points.size(); ++i) {
			if (points[i].x > max_point)
				max_point = points[i].x;
			
			if (points[i].y > max_point)
				max_point = points[i].y;
		}

		if (p.x > max_point)
			max_point = p.x;

		if (p.y > max_point)
			max_point = p.y;

		auto width = max_point * 40 + 20;
		auto height = width;

		cv::Mat destImage(width, height, CV_8U, CV_RGB(255, 255, 255));

		for (int i = 0; i < lines.size(); ++i)
			cv::line(destImage, cv::Point(lines[i].p1.x * 40, lines[i].p1.y * 40), cv::Point(lines[i].p2.x * 40, lines[i].p2.y * 40), CV_RGB(0, 0, 0));
		

		cv::circle(destImage, cv::Point(p.x * 40, p.y * 40), 3, CV_RGB(255, 0, 0), -1);
						
		cv::flip(destImage, destImage, 0);
		cv::imshow("Polygon", destImage);
	}


private:

	int angle_compare = 0;//수평각 비교 회수
	int compare = 0;//비교 회수

	float ComputeAngle(point p1, point p2) {

		++angle_compare;

		int dx, dy;
		int ax, ay;

		float t;

		dx = p2.x - p1.x; ax = abs(dx);
		dy = p2.y - p1.y; ay = abs(dy);

		t = (ax + ay == 0) ? 0 : (float)dy / (ax + ay);

		if (dx < 0)
			t = 2 - t;
		else if (dy < 0)
			t = 4 + t;

		return t * 90.0;
	}

	//두 인덱스의 스왑 구현
	void swap(const float& x, const float& y) {
		auto temp = angles[x];
		angles[x] = angles[y];
		angles[y] = temp;
	}

	void make_heap(const float& root, const float& last_node) {
		float parent, left_son, right_son, son, root_value;
		parent = root;//부모 인덱스 - 루트 인덱스
		root_value = angles[root];//루트 인덱스의 값
		left_son = parent * 2;//현재 부모 인덱스의 왼쪽 자식 노드 인덱스
		right_son = left_son + 1;//현재 부모 인덱스의 오른쪽 자식 노드 인덱스

		while (left_son < last_node) {
			//만약 끝 노드가 오른쪽 노드와 같거나 아래에 있고 오른쪽 자식 노드값이 왼쪽 자식 노드값보다 작을 경우
			if (++compare && right_son <= last_node && angles[left_son] < angles[right_son]) {
				son = right_son;//현재 비교할 자식을 오른쪽 자식으로 설정
			}
			else {
				son = left_son;//현재 비교할 자식을 왼쪽 자식으로 설정
			}

			if (++compare && root_value < angles[son]) {//현재 루트의 값이 자식보다 작을 경우
				angles[parent] = angles[son]; //자식의 값을 위로 올린다.(덮어 쓰기)
				parent = son; //자식 노드를 부모로 설정한다.
				left_son = parent * 2;//현재 부모를 기준으로 왼쪽 자식 노드 인덱스 재설정
				right_son = left_son + 1;//현재 부모를 기준으로 오른쪽 자식 노드 인덱스 재설정 
			}
			else {
				break;//현재 부모가 자식보다 클 경우 종료
			}
		}
		angles[parent] = root_value;  //루트에 있었던 값을 비어 있는 자리로 옮긴다.
	}

	void heap_sort() {
		//힙 구성
		for (int i = size / 2; i >= 1; i--)
			make_heap(i - 1, size - 1);

		for (int i = size - 1; i >= 1; i--) {
			swap(0, i);//루트 노드와 마지막 노드를 교환한 뒤 
			make_heap(0, i - 1);//루트 노드였던 값을 제외하고 힙을 재구성
		}
	}

	bool is_intersection(line l1, line l2) {

		auto high_p = (l1.p1.y > l1.p2.y) ? l1.p1 : l1.p2;
		auto low_p = (l1.p1.y <= l1.p2.y) ? l1.p1 : l1.p2;

		if (!(low_p.y <= l2.p1.y && l2.p1.y < high_p.y))
			return false;

		float inclination = static_cast<float>(high_p.y - low_p.y) / (high_p.x - low_p.x);
		float y_intercept = static_cast<float>(high_p.y) - inclination * high_p.x;

		auto x_intersection = static_cast<float>(l2.p1.y - y_intercept) / inclination;

		if (l2.p1.x <= x_intersection)
			return true;
		else
			return false;
	}
};

int main() {

	int num_p;
	int input_x, input_y;

	cout << "점의 개수를 입력해 주세요 >> ";

	cin >> num_p;

	Polygon p{ num_p };

	p.print_result();

	cout << "점의 좌표를 입력해 주세요 >> ";

	cin >> input_x >> input_y;

	auto input = point{ input_x, input_y };

	p.show_polygon(input);

	if (p.is_inside(input))
		cout << "점은 다각형 안에 존재합니다." << endl;
	else
		cout << "점은 다각형 안에 존재하지 않습니다." << endl;

	cv::waitKey(0);

}