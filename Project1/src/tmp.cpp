#include <iostream>

// CheckDiv 특수화를 위한 래퍼 객체
template <int N>
struct INT {
	static const int val = N;
};

using one = INT<1>;
using two = INT<2>;
using three = INT<3>;

// INT 선언 시 a, b, 타입의 val 인자를 사용한다.
// 이 경우 a, b가 val을 가지고 있지 않으면 오류 발생.
template <typename a, typename b>
struct Add {
	using result = INT<a::val + b::val>;
};
template <typename a, typename b>
struct Divide {
	using result = INT<a::val / b::val>;
};

// N을 D로 나누었을 때 나머지가 0인지 확인한다.
// D에 1이 더해진 INT타입을 다음 CheckDiv에 넣어 그 결과를 가져옴.
// 하나라도 true면 result는 true가 된다 (소수가 아니다)
// 아래의 특수화된 CheckDiv가 마지막이 된다.
template <typename N, typename D>
struct CheckDiv {
	static const bool result = (N::val % D::val == 0) ||
		CheckDiv<N, typename Add<D, one>::result>::result;
};
// D가 N을 2로 나눈것과 동일한 타입일 때에만 해당 구조체를 사용한다.
// 특수화 시 함수명 정의 뒤<> 내에서 산술 연산이 불가능하다.
// 따라서 int타입을 감싸는 래퍼 타입 INT와 그것을 나누는 Divide가 생김.
// 다만 이 경우 D라는 기호를 식 내에서 쓸 수가 없다. 그래서 N을 따로 2로 한번 더 나누어 준다.
// 또한 Divide의 result를 즉 어떤 타입의 ::를 컴파일러가 기본적으로 해당 타입의 값으로 해석한다.
// 이것은 값이 아닌 타입이므로 이 경우 typename을 붙여줘야 함.
template <typename N>
struct CheckDiv<N, typename Divide<N, two>::result> {
	static const bool result = (N::val % (N::val / 2) == 0);
};

// 소수가 아닐 경우 true가 되기에 !로 반전
template <typename N>
struct IsPrime {
	static const bool result = !CheckDiv<N, two>::result;
};
template <>
struct IsPrime<two> {
	static const bool result = true;
};
template <>
struct IsPrime<three> {
	static const bool result = true;
};

// // // // // // // //

// 유클리드 호제법을 통한 두 수의 최대공약수 계산
template <int X, int Y>
struct GCD {
	// 템플릿 타입 호출부에서 산술 연산 가능
	static const int val = GCD<Y, X% Y>::val;
};
// <X, 0> 템플릿 시 아래를 사용 (템플릿 특수화)
template <int X>
struct GCD<X, 0> {
	static const int val = X;
};

// N/D 꼴 기약분수를 정의하는 구조체
// 두 수의 최대공약수로 두 수를 나눠준다.
template <int N, int D = 1>
struct Ratio {
private:
	// 주입된 분자와 분모의 최대공약수
	static const int _gcd = GCD<N, D>::val;
public:
	static const int num = N / _gcd;
	static const int den = D / _gcd;
	// 아래의 RatioAdd등 해당 Ratio타입을 상속받는 타입 내에서도
	// Ratio원본 타입을 보존하기 위해 한번 더 정의
	using type = Ratio< num, den >;
};

// 두 분수의 연산 결과를 내부에서 정의하는 구조체와,
// 그 연산 결과를 상속받는 구조체 (내부에 정의된 연산 결과에 ::로 접근하는 수고를 덜기 위함)
// 즉, RatioAdd 자체가 Ratio로서 동작한다.
template <typename a, typename b>
struct _RatioAdd {
	using result = Ratio<a::num* b::den + b::num * a::den, a::den* b::den>;
};
template <typename a, typename b>
struct RatioAdd : _RatioAdd<a, b>::result {};

template <typename a, typename b>
struct _RatioSubtract {
	using result = Ratio<a::num* b::den - b::num * a::den, a::den* b::den>;
};
template <typename a, typename b>
struct RatioSubtract : _RatioSubtract<a, b>::result {};

template <typename a, typename b>
struct _RatioMultiply {
	using result = Ratio<a::num* b::num, a::den* b::den>;
};
template <typename a, typename b>
struct RatioMultiply : _RatioMultiply<a, b>::result {};

template <typename a, typename b>
struct _RatioDivide {
	using result = Ratio<a::num* b::den, a::den* b::num>;
};
template <typename a, typename b>
struct RatioDivide : _RatioDivide<a, b>::result {};

// 단위에 대한 정의
// 각 단위에 Ratio<> 타입이 들어가게 된다.
template <typename U, typename V, typename W>
struct Dim {
	using M = U; // 질량
	using L = V; // 길이
	using T = W; // 시간

	using type = Dim<M, L, T>;
};

// 두 Dim 타입을 받아 연산을 수행.
// 단순히 서로의 각 타입을 더하거나 뺀다.
// 두 '단위를 가진 값'을 곱하거나 나눌 경우 지수 법칙 적용 용도
template <typename U, typename V>
struct AddDim {
	using result = typename Dim<
		typename RatioAdd<typename U::M, typename V::M>::type,
		typename RatioAdd<typename U::L, typename V::L>::type,
		typename RatioAdd<typename U::T, typename V::T>::type>::type;
};
template <typename U, typename V>
struct SubtractDim {
	using result = typename Dim<
		typename RatioSubtract<typename U::M, typename V::M>::type,
		typename RatioSubtract<typename U::L, typename V::L>::type,
		typename RatioSubtract<typename U::T, typename V::T>::type>::type;
};

// 단위를 가진 값.
// T에 float, double등이 오게 된다.
template <typename T, typename D>
struct Quantity {
	T val;
	using dimType = D;

	//생성자
	Quantity(T val) : val(val) {}

	// 같은 자료형, 같은 단위를 사용하는 값 끼리만 덧셈, 뺄셈이 가능하다
	Quantity operator+(const Quantity<T, D>& quant) {
		return Quantity<T, D>(val + quant.val);
	}
	Quantity operator-(const Quantity<T, D>& quant) {
		return Quantity<T, D>(val - quant.val);
	}

	// 같은 자료형의 스칼라가 뒤에서 곱하거나 나누어질 경우 (this */ 1.0)
	Quantity operator*(T scalar) {
		return Quantity<T, D>(val * scalar);
	}
	Quantity operator/(T scalar) {
		return Quantity<T, D>(val / scalar);
	}

	// 자료형은 같으나 타입이 다른(D2) 값이 뒤에서 곱하거나 나누어질 경우 
	// 이 경우 새로운 타입이 리턴되므로 리턴형의 타입을 명확히 작성
	template<typename D2>
	Quantity<T, typename AddDim<D, D2>::result> operator*(const Quantity<T, D2>& quant) {
		return Quantity<T, typename AddDim<D, D2>::result>(val * quant.val);
	}
	template<typename D2>
	Quantity<T, typename SubtractDim<D, D2>::result> operator/(const Quantity<T, D2>& quant) {
		return Quantity<T, typename SubtractDim<D, D2>::result>(val / quant.val);
	}
};

// 스칼라(같은 T타입) 뒤에 Quantity가 곱하거나 나누어질 경우 (1.0 */ Q)
template <typename T, typename D>
Quantity<T, D> operator*(T scalar, const Quantity<T, D>& quant) {
	return Quantity<T, D>(scalar * quant.val);
}
template <typename T, typename D>
Quantity<T, D> operator/(T scalar, const Quantity<T, D>& quant) {
	return Quantity<T, D>(scalar / quant.val);
}

// ostream 출력
template<typename T, typename D>
std::ostream& operator<<(std::ostream& out, const Quantity<T, D>& quant) {
	out << quant.val << " kg^" << D::M::num << "/" << D::M::den << " m^" << D::L::num << "/" << D::L::den << " s^" << D::T::num << "/" << D::T::den;
	return out;
}

// // // // // // // //

int use_main() {
	//using value = INT<17>;
	//bool b = IsPrime<value>::result;

	using oneR = Ratio<1, 1>;
	using zeroR = Ratio<0, 1>;

	int k = GCD<1655, -265>::val;

	Quantity<double, Dim<oneR, zeroR, zeroR>> kg(2);
	Quantity<double, Dim<zeroR, oneR, zeroR>> meter(3);
	Quantity<double, Dim<zeroR, zeroR, oneR>> second(1);

	std::cout << Quantity<double, Dim<zeroR, zeroR, oneR>>(1)* Quantity<double, Dim<zeroR, zeroR, oneR>>(1) << std::endl;
	std::cout << Quantity<double, Dim<zeroR, oneR, zeroR>>(1)* Quantity<double, Dim<zeroR, oneR, zeroR>>(1) << std::endl;
	std::cout << Quantity<double, Dim<oneR, zeroR, zeroR>>(1)* Quantity<double, Dim<oneR, zeroR, zeroR>>(1) << std::endl;

	std::cout << Quantity<double, Dim<oneR, oneR, oneR>>(1)* Quantity<double, Dim<zeroR, zeroR, zeroR>>(1) << std::endl;

	auto F = kg * meter / (second * second);
	std::cout << "2 kg 물체를 3m/s^2의 가속도로 밀기위한 힘의 크기는? " << F
		<< std::endl;

}