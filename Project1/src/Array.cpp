class Array {
	const int dim; // 몇 차원 배열인지
	int* size; // 각 차원별 사이즈 정보를 가진 배열

	// 트리형 배열의 맨 끝 노드를 제외한 나머지는 Address구조체의 배열을 사용.
	// 다중 포인터 사용을 피하기 위함.
	struct Address {
		int level;
		void* next;
	};
	Address* top; // 최상단 노드
	friend Int; // Int 클래스에서는 Array의 요소들 자유롭게 사용
	friend Iterator;
public:
	Array(int dim, int* array_size); // 기본 생성자
	Array(const Array& arr); // 복사 생성자
	~Array(); // 소멸자
	void init_address(Address* current); // 내부 요소 초기화
	void del_address(Address* current); // 내부 요소 제거
	Int operator[](const int index);
};

// 첨자[]를 활용한 배열 요소 탐색을 위한 중간 클래스
class Int {
	void* data;
	int level;
	Array* array;
public:
	Int(int index, int _level, void* _data, Array* _array); // 생성자
	Int operator[](const int index);
	operator int();
};

class Iterator {
	int* location; // 현 위치를 나타내는 배열
	Array* arr;
public:
	Iterator& operator++();
};


/////////////

Array::Array(int dim, int* array_size) : dim(dim) {
	size = new int[dim];
	for (int i = 0; i < dim; i++) {
		size[i] = array_size[i];
	}
	top = new Address;
	top->level = 0;
	init_address(top);
}
Array::Array(const Array& arr) : dim(arr.dim) {
	size = new int[dim];
	for (int i = 0; i < dim; i++) {
		size[i] = arr.size[i];
	}
	top = new Address;
	top->level = 0;
	init_address(top);
}
Array::~Array() {
	del_address(top);
	delete[] size;
}

// 재귀호출을 통한 깊이우선 방식으로 배열 초기화
void Array::init_address(Address* current) {
	// 마지막일 경우 정수 배열 생성
	// 아닐 경우 Address배열 생성
	if (!current) return;
	if (current->level == dim - 1) {
		current->next = new int[size[current->level]];
		return;
	}
	current->next = new Address[size[current->level]];
	for (int i = 0; i < size[current->level]; i++) {
		Address* ad = static_cast<Address*>(current->next) + i;
		ad->level = current->level + 1;
		init_address(ad);
	}
}
void Array::del_address(Address* current) {
	if (!current) return;
	// level이 dim-1보다 작을 때만 재귀하여 탐색해준다.
	// 아닐 경우엔 마지막이라는 의미이니 그냥 지우면 됨.
	for (int i = 0; current->level < dim - 1 && i < size[current->level]; i++) {
		Address* ad = static_cast<Address*>(current->next) + i;
		del_address(ad);
	}
	delete[](current->next);
}
Int Array::operator[](const int index) {
	return Int(index, 1, static_cast<void*>(top), this);
}
Int Int::operator[](const int index) {
	return Int(index, level + 1, data, array);
}
Int::operator int() {
	return *static_cast<int*>(data);
}
Int::Int(int index, int _level, void* _data, Array* _array)
	: level(_level), data(_data), array(_array) {
	// 레벨에 비정상적으로 낮은 값이 들어오거나, 인덱스가 범위를 초과할 경우
	if (_level < 1 || index >= _array->size[_level - 1]) {
		_data = nullptr;
		return;
	}
	// 최초로 _data에는 Address 구조체의 포인터가 온다. (Address* 캐스팅)
	// 만약 현재 레벨과 차원이 동일하다면, Address의 next는 정수 배열을 (int*캐스팅)
	// 더 낮다면 다른 Address배열을 가리키고 있을 것이다. (Address* 캐스팅)
	// 해당 배열에서 index번째 요소를 가져와 다시 void*로 변환한다
	if (level == _array->dim) {
		_data = static_cast<void*>(static_cast<int*>(
			static_cast<Array::Address*>(data)->next) + index);
	}
	else {
		_data = static_cast<void*>(static_cast<Array::Address*>(
			static_cast<Array::Address*>(data)->next) + index);
	}
}
Iterator& Iterator::operator++() {
	// size가 5면, location[0]은 0~4까지
	if (location[0] >= arr->size[0]) {
		return *this;
	}
	bool carry = false;
	int i = arr->dim - 1;
	do {
		location[i]++;
		// 맨 마지막 원소에서 ++ 될 경우 location은
		// size[0], 0, 0 으로 범위를 초과. 이게 나오면 끝난걸로 간주하기 위해
		// i > 0 조건을 넣음.
		if (location[i] >= arr->size[i] && i > 0) {
			carry = true;
			location[i] = 0;
			i--;
		}
		else {
			carry = false;
		}

	} while (carry && i >= 0);
}