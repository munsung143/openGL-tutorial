#include <iostream>

#include <vector>
void vectorTest() {
	std::vector<int> vec;
	vec.push_back(10);
	vec.push_back(20);
	// std::vector<int>::size_type 은 size_t 타입을 나타냄, vec.size()는 size_t 타입의 값
	for (std::vector<int>::size_type i = 0; i < vec.size(); i++) {
		std::cout << vec[i];
	}

	// iterator타입은 일종의 포인터처럼 동작(*연산자 오버로딩 되어 있음)
	// begin()은 시작 위치, end()는 끝 바로 다음 위치
	// 주의1. iterator는 벡터에 원소 삽입, 삭제 시마다 초기화 된다. (각 원소의 주소값들이 변경되기 때문)
	for (std::vector<int>::iterator itr = vec.begin(); itr != vec.end(); itr++) {
		std::cout << *itr;
	}
	// 삽입, 삭제 시에도 iterator타입 사용 가능
	vec.insert(vec.begin() + 2, 15);

	// const iterator 타입을 통해 해당 반복자가 참조하는 것에 대한 수정을 방지할 수 있다.
	// reverse iterator 타입은 벡터를 역으로 참조할 수 있게 해준다.
	// rbegin()은 끝부분의 위치를, rend()는 첫부분 바로 전의 위치를 나타냄, 동일하게 ++연산을 통해 다음(역)으로 이동
	for (std::vector<int>::const_reverse_iterator ritr = vec.crbegin(); ritr != vec.crend(); ritr++) {
		std::cout << *ritr;
	}

	//범위 기반 for문 사용 예시
	for (const int& val : vec) {
		std::cout << val;
	}
}
#include <list>
void listTest() {
	std::list<int> lst;
	lst.push_back(10);
	lst.push_back(20);
	// 리스트는 C#의 연결리스트와 동일한 개념으로, []등을 통한 인덱스 접근이 기본적으로 정의되어 있지 않음.
	// 반복자에 사용 가능한 연산은 ++, -- 뿐. (BidirectionalIterator)
	// insert에 반복자와 값을 전달하여 원소 삽입 가능
	// 원소를 삽입, 삭제 하여도 반복자가 무효화 되지 않음. (원소들의 주소값은 변하지 않기 때문)
}

#include <set>
void setTest() {
	std::set<int> st;
	// 1. 셋은 내부에 원소들이 이진 탐색 트리 형태로 저장된다. (어떤 노드를 기준으로 큰것은 오른쪽, 작은것은 왼쪽 자식으로)
	// 즉 logN의 삽입, 삭제, 탐색 복잡도를 가지며, 각 원소가 어디에 존재하는지에 대한 정보보다는 해당 원소가 있느냐/없느냐에 중점이 맞춰진다.
	// (한쪽으로 치우처진 트리가 될 경우 N의 복잡도를 가지게 된다. 이를 완화하기 위해 RB트리 등이 도입된다)
	// 2. 중복된 원소를 허용하지 않는다.
	st.insert(10);
	st.insert(20);
	// 셋 순회 시에는 중위 순회(왼쪽>뿌리>오른쪽 순서) 방식을 사용하여 크기별로 정렬된 값들이 나온다.
	for (std::set<int>::iterator itr = st.begin(); itr != st.end(); itr++) {
		std::cout << *itr;
	}
	// 값 탐색 시 itr을 반환, 존재하지 않는다면 end()가 나오게 된다.
	auto itr = st.find(20);
}

// 셋이 나의 클래스를 다룰 수 있도록 하고 싶다면, 즉 set<MyClass>를 사용하려면,
// 해당 클래스의 크기 비교를 위한 < 연산자가 오버로딩 되어있어야 한다.
// 셋은 크기를 a < b, b < a로 두번 비교하여, 둘 다 false일 경우, 중복된 원소라고 판단하여 제외한다.
class MyClass {
public:
	int priority;
	MyClass(int priority) : priority(priority) {}
	bool operator<(const MyClass& c) const {
		return priority < c.priority;
	}
};
// 비교 연산을 함수 객체를 통해 클래스 외부에서 선언할 수도 있다.
// 이 경우 해당 타입을 set의 템플릿 인자로 함께 넘겨준다. set<MyClass, MyClassCmp>
struct MyClassCmp {
	bool operator()(const MyClass& a, const MyClass& b) {
		return a.priority < b.priority;
	}
};

#include <map>
void mapTest() {
	std::map < std::string, double> mp;
	// 맵에는 pair타입 객체를 할당할 수 있다. [] 사용도 가능
	mp.insert(std::pair<std::string, double>("kim", 3.3));
	mp.insert(std::make_pair("den", 1.2));
	mp["kal"] = 4.4;

	// 셋과 마찬가지로 key의 크기에 따른 트리 형태로 저장된다.
	// key의 크기 순으로 출력된다.
	for (std::map < std::string, double>::iterator itr = mp.begin(); itr != mp.end(); itr++) {
		std::cout << itr->first << itr->second;
	}

	// []로 존재하지 않는 키로 접근 시, 해당 키에 대한 값을 추가해버린다.
	// 따라서 이 경우(혹은 일반적으로) 키의 존재 여부를 먼저 확인하는것이 좋음.
	std::string key = "kal";
	auto itr = mp.find(key);
	if (itr != mp.end()) {
		std::cout << mp[key];
	}
}

void multiTest() {
	//중복된 요소를 저장할 수 있는 셋과 맵
	std::multiset<int> mset;
	mset.insert(3);
	mset.insert(3);

	std::multimap<int, std::string> mmap;
	mmap.insert(std::make_pair(1, "k"));
	mmap.insert(std::make_pair(1, "v"));

	//	equals.range() 함수를 통해 전해진 값과 동일한 원소들을 순회할 수 있는, 
	// 예를 들어 1,2,3,3,3,4,5 라고 저장되었다고 한다면,
	// 3,3,3 에 해당하는 시작(begin) 및 끝(end) iterator가 저장된
	// pair객체를 리턴받을 수 있다. pair이기 때문에 각자 first, second로 접근
	std::pair<std::multiset<int>::iterator, std::multiset<int>::iterator> srange = mset.equal_range(3);
	for (auto itr = srange.first; itr != srange.second; itr++) {
		std::cout << *itr;
	}

	auto mrange = mmap.equal_range(1);
	for (auto itr = mrange.first; itr != mrange.second; itr++) {
		std::cout << itr->first << itr->second;
	}
}

#include <unordered_set>
#include <unordered_map>
void unorderedTest() {
	// unordered map의 경우 C#의 딕셔너리와 가장 유사한 개념 (해시 함수 사용)
	// 기본 타입 및 string의 경우 해시 함수가 제공되어 그냥 사용이 가능하지만
	// 커스텀 클래스의 경우 직접 해시 함수를 만들어야 한다(기본 제공 해시 함수 활용 가능)
}

// 커스텀 클래스를 unoroderd에 사용하기 위해 필요한 것
// 1. == 연산자 오버로딩
// 2. hash<> 함수객체 특수화
class MyClass2 {
	int val;
	std::string str;

public:
	// 만일 두 객체가 동일한 해시값을 가질 경우(해시 충돌)
	// 해당 해시에 리스트된 값들 간의 동등성 비교를 위하여 == 오버로딩
	bool operator==(const MyClass2& c) const {
		return (val == c.val && str == c.str);
	}
	// hash<MyClass2> 함수 객체에 private 값들 공유를 위함
	friend struct std::hash<MyClass2>;
};
// std의 hash<> 함수 객체에 대한 특수화
namespace std {
	template<>
	struct hash<MyClass2> {
		size_t operator()(const MyClass2& t) const {
			// 기본 해시 함수객체 hash<>
			hash<std::string> hash_str;
			// 기본 해시와 val의 비트 연산을 통한 해시값 생성
			return t.val ^ hash_str(t.str);
		}
	};
}