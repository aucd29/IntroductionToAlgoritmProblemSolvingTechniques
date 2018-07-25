#include <cstdio>
#include <vector>
#include <algorithm>

using namespace std;

class Robot {
public:
	int x;				// 로봇의 초기 위치
	 int leftBound;		// 로봇의 초기 왼쪽 감시영역 끝 좌표
	int rightBound;	// 로봇의 초기 오른쪽 감시영역 끝 좌표

	Robot(int x, int r) {
		this->x = x;
		this->leftBound = x - r;
		this->rightBound = x + r;
	}

	bool operator < (const Robot& other) const {
		return this->x < other.x;
	}
};

/**
 * 로봇의 최대 이동 가능 거리가 D일 때, 전체 영역을 감시할 수 있는지 여부를 계산
 *
 * @param N		로봇의 수
 * @param L		감시 영역의 길이
 * @param D		최대 이동 가능 거리
 * @param robots
 */
bool isPossible(int N, int L, int D, const vector<Robot>& robots) {
	int leftEnd = 0;	// 감시 가능한 왼쪽 끝 좌표
	int rightEnd = L;	// 감시 대상의 오른쪽 끝 좌표

	for (int i = 0; i < N; i += 1) {	// 각 로봇을 왼쪽부터 조회하면서

		if (leftEnd <= robots[i].leftBound) {
			// 현재까지 감시 가능한 영역(leftEnd)와 robots[i]의 사이에 공백이 존재하는 경우
			// 로봇을 왼쪽으로 이동시켜 공백을 매꾼다

			int distance = robots[i].leftBound - leftEnd; // 이동시켜야 할 거리
			if (distance > D) {	// 이동거리 상한보다 더 이동시켜야만 하는 경우
				// 불가능으로 처리한다. 해당 빈 공간을 감시할 수 없다.
				return false;
			}

			// 그렇지 않으면 왼쪽으로 이동시킨다. 이 때, 접할정도까지만 이동시킨다.
			leftEnd = max(robots[i].rightBound - distance, leftEnd);
		} else {
			// 로봇이 이미 감시 완료된 영역을 포함하고 있을 때

			// 공백이 생기지 않는 선에서 로봇을 오른쪽으로 최대 D만큼 이동시킨다.
			int distance = min(D, leftEnd - robots[i].leftBound);

			// 이동시킨 후 새로 감시완료된 영역을 반영한다.
			leftEnd = max(robots[i].rightBound + distance, leftEnd);
		}

		// [0, rightEnd]까지 모두 감시가 완료된 경우 true, 아니면 false
		if(leftEnd >= rightEnd){
			break;
		}
	}
	// [0, rightEnd]까지 모두 감시가 완료된 경우 true, 아니면 false
	return leftEnd >= rightEnd;
}

/**
 * 전체 영역을 감시하기 위해 필요한 최소의 이동거리 상한을 계산하는 함수
 *
 * @param N
 * @param L
 * @param robots
 * @return
 */
int getMinimumMoveDistance(int N, int L, const vector<Robot>& robots) {
	long long lowerBound = 0;					// 전혀 이동이 필요 없는 경우
	long long upperBound = 2e9;					// 이론상 최대 이동 가능 거리

	while (lowerBound < upperBound) {
		// 이동 거리 상한을 설정한다
		int distanceLimit = (int)((lowerBound + upperBound) / 2);

		// 이후 가능 여부를 검사한다
		bool possible = isPossible(N, L, distanceLimit, robots);

		if (possible == true) {
			// 가능하다면 그 이하의 상한에 대해 조사한다
			upperBound = distanceLimit;
		} else {
			// 불가능하다면 더 큰 상한에 대해서 조사한다
			lowerBound = distanceLimit + 1;
		}
	}

	return lowerBound;
}

void testCase() {
	int N, R, L;
	scanf("%d%d%d", &N, &R, &L);

	vector<Robot> robots;
	for (int i = 0; i < N; i += 1) {
		int X;
		scanf("%d", &X);
		robots.push_back(Robot(X, R));
	}
	sort(robots.begin(), robots.end());

	int answer = getMinimumMoveDistance(N, L, robots);

	printf("%d\n", answer == 2e9 ? -1 : answer);
}

int main() {
	int caseNum;
	scanf("%d", &caseNum);
	for (int caseIndex = 1; caseIndex <= caseNum; caseIndex += 1) {
		testCase();
	}
}
