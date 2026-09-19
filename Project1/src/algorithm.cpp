// C++의 알고리즘 라이브러리의 함수는 대부분은 다음 두 형태를 따른다.
// 1. 어떤 자료구조의 알고리즘 적용 범위(시작과 끝)를 반복자로 제공
template<typename iter>
void do_something(iter begin, iter end) {}
// 
// 2. 특수한 조건 정의를 위한 bool 반환 함수 객체(predicate)까지 제공하는 형태
template<typename iter, typename pred>
void do_something(iter begin, iter end, pred pred) {}

// int 비교 함수객체
struct int_compare {
	bool operator()(const int& a, const int& b) const {
		return a > b;
	}
};
#include <iostream>
#include <algorithm>
#include <vector>
#include <functional>
void algorithmTest() {
	std::vector<int> vec;
	// 정렬
	// sort : 일반적 정렬
	// 재약 : 반복자가 원소에 임의 접근이 가능한 RandomAccessIterator를 만족해야 한다.
	// ++ 또는 -- 만이 가능한 연결 리스트 등의 양방향 BidirectionalIterator는 안됨.
	std::sort(vec.begin(), vec.end());
	// 정렬은 < 연산자를 통해 오름차순으로 이루어진다. (true면 앞에, false면 뒤에)
	// 내림차수 정렬을 원할 경우 > 연산을 수행하는 함수 객체를 전달한다.
	std::sort(vec.begin(), vec.end(), int_compare());
	// <functional> 에 위와 동일한 템플릿 함수객체 greater가 존재함
	std::sort(vec.begin(), vec.end(), std::greater<int>());

	// partial_sort : 배열의 일부분만 정렬, 배열의 일부가 아닌, 전체 배열을(기준 설정 가능) 대상으로 한다.
	// 이러면 처음 3개 원소만이 정렬된다. 즉 전체 원소 중 가장 작은 3개가 앞에 온다.
	std::partial_sort(vec.begin(), vec.begin() + 3, vec.end());

	// stable_sort : 원소들 간 상대적 위치를 그대로 유지하는 정렬. 일반적인 정렬과 사용법은 동일. (조금 더 느리다)


	// 원소 제거

	// 일반적인 원소 하나 제거
	vec.erase(vec.begin() + 3);
	// 범위 제거
	vec.erase(vec.begin(), vec.begin() + 3);

	// 벡터에서 3에 해당하는 원소를 전부 지운 후, 지워지고 정렬된 부분의 마지막 바로 다음 위치를 반환한다.
	// 1234536738930
	// 리스트를 순회하면서
	// 3이 등장하면 건너뛰고, 이후부터 앞으로 1칸 앞에 값 복사.
	// 또 3이 등장하면 건너뛰고, 이후부터 2칸 앞에 값 복사
	// 이렇게 마지막까지 하다보면 3이 제외된 원소들이 앞에 모이고, 뒤엔 필요없는 값들이 남게 된다.
	// 1245678908930
	// 뒤 8930 의 8의 위치를 반환한다.
	// ( 반환 반복자의 타입이 ForwardIterator로, 리스트, 셋, 맵 모두 사용가능 )
	auto itr = std::remove(vec.begin(), vec.end(), 3);
	// 이후 필요없는 부분을 지워준다.
	vec.erase(itr, vec.end());

	// 조건에 따른 삭제 시에는 remove_if()를 사용한다.
	// bool을 반환하는 함수 객체 또는 함수 자체를 전달한다.
	struct is_odd {
		bool operator()(const int& i) { return i % 2 == 1; }
	};
	itr = std::remove_if(vec.begin(), vec.end(), is_odd());

	// 주의점
	// 만약 조건을 만족하는 처음 2개의 원소만을 제거하는 기능이 필요하다고 하자.
	// 그러면 함수객체에 카운터같은 필드 변수를 추가하는 방법이 있겠다.
	// 하지만 remove_if가 그 객체를 그대로 사용하지 않을 가능성이 있다. 
	// 예를 들어 객체 복사가 내부에서 이루어진다면? (실제로 복사가 이루어진다)
	// 별개의 필드값 때문에 원치 않는 결과가 나올 수 있다.
	// 이 경우 카운터를 외부에 두고, 필드에 레퍼런스를 주는 방식으로 복사시에도 같은
	// 변수를 가리키도록 해결할 수는 있으나, 깔끔하지는 않다.
	// 이를 해결하기 위해 람다 함수 및 capture list를 활용

	// 람다 함수
	// [capture list] (받는 인자 즉 매개변수) -> 리턴 타입 { 함수 본체; }
	// 함수 본체 내에서 리턴하는 값의 리턴 타입이 자동 추론될 경우 리턴 타입 및 -> 생략 가능
	auto func = [](int i) -> bool {return i % 2 == 1;};
	func(4); // 직접 호출
	itr = std::remove_if(vec.begin(), vec.end(), func);

	// 람다 함수는 함수 외부 변수들에 접근할 수 없다.
	// 접근이 가능하도록 하려면 caputre list를 통해 레퍼런스를 받아와야 한다.
	// 다음의 문법이 적용된다.
	// [] : 아무것도 캡쳐하지 않음
	// [&a, b] a를 레퍼런스로 캡쳐, b는 const인 복사본으로 캡쳐
	// [&] 외부 모든 변수들을 레퍼런스로 캡쳐
	// [=] 외부 모든 변수들을 const 복사본으로 캡쳐

	// Tip. 또한 만약 람다 함수가 클래스 내부에 있을 경우, 클래스의 필드는 캡쳐할 수 없다.
	// 이 경우 [this]를 통해 포인터 캡쳐, this->field 로 활용

	// 처음 두 홀수 원소만 제거하는 예시
	int num_erased = 0;
	itr = std::remove_if(vec.begin(), vec.end(), [&num_erased](int i) {
		if (num_erased >= 2)
			return false;
		else if (i % 2 == 1) {
			num_erased++;
			return true;
		}
		return false;
		});


	// 원소 수정 (transform)

	// 벡터 전체에 1 더하기
	// begin 부터 end에 1씩 더한 결과를 begin(_Dest) 부터 저장한다.
	// 즉 저장할 벡터의 begin 부터의 원소들의 개수는 
	// 최소한 결과의 개수 이상이어야 한다.
	std::transform(vec.begin(), vec.end(), vec.begin(),
		[](int i) { return i + 1; });

	// 원소 탐색 (find find_if)
	// 처음부터 순회하며 원소 탐색, 미발견 시 end()반환
	// 모든 원소를 찾고 싶다면, while등의 반복문을 활용해야 함.
	// 전수 조사 방식(O(n))을 사용하기 때문에, 일부 자료구조에서는 비효율적일 수 있음
	// 이 경우 해당 자료구조에서 기본 제공하는 find 함수 사용이 권장됨 (vector는 기본 find가 없음)
	itr - std::find(vec.begin(), vec.end(), 3);

	// any_of : 범위 내 하나라도 조건이 충족되면 true 반환
	// all_of : 범위 내 모든 원소가 조건을 충족하면 true 반환

	// find_if, any_of, all_of는 모두 람다 사용 가능
}